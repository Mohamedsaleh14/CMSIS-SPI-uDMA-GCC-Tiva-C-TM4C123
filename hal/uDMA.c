/*
 * Copyright (c) 2016, Mohamed Saleh
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Mohamed Saleh nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Mohamed Saleh ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Mohamed Saleh BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File:			uDMAC.c
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	uDMA driver.
 */

#include <stdint.h>
#include "uDMA.h"
#include "../cmsis/LM4F120H5QR.h"
#include "../ERRH/ERRH.h"

#define UDMA_CHANNEL_12		(12)
#define UDMA_CHANNEL_13		(13)

/*************************************************************************/
/*				      local variable declaration						   */
/*************************************************************************/
 uint32_t udma_control_structure[256] __attribute__ ((aligned(1024)));
 uint8_t udma_buffer_tx[UDMA_BUFFER_SIZE] ;
 uint8_t udma_buffer_rx[UDMA_BUFFER_SIZE] ;
 uint8_t tx_transfer_size = UDMA_BUFFER_SIZE;
 uint8_t rx_transfer_size = UDMA_BUFFER_SIZE;
static uint32_t control_word_ch12 =  0;
static uint32_t control_word_ch13 =	0;

/*************************************************************************/
/*				      local Functions declaration					   */
/*************************************************************************/
static void SSI2DMAConfiguration(void);
static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word);

/*************************************************************************/
/*						  local Functions							   */
/*************************************************************************/
static void SSI2DMAConfiguration(void)
{
	if((((uint32_t)udma_control_structure & ~(0x3FF)) == (uint32_t)udma_control_structure)
			&& ((uint32_t)udma_control_structure >= 0x20000268))
	{
		SYSCTL->RCGCDMA = (0x01); 	//Enable clock for DMA
		UDMA->CFG = (uint32_t)(0x01);			//Enable uDMA controller
		UDMA->CTLBASE = (uint32_t)udma_control_structure;
		UDMA->ALTCLR = (uint32_t)((1<<12)|(1<<13)); //Using Primary control structure
		UDMA->PRIOSET = (1<<12);		//Setting priority for Receiving from SSI2
		UDMA->CHMAP1  = (1<<17 /*Map channel 12 to SSI2 Rx*/ )|(1<<21/*Map channel 13 to SSI2 Tx*/);
		UDMA->REQMASKSET = ~(uint32_t)((1<<12)|(1<<13)); 	//Masking all channels to be not requested except for channel 12, 13

		CfgDMAChSrcAdd(UDMA_CHANNEL_12, (uint32_t)&(SSI2->DR));
		CfgDMAChDesAdd(UDMA_CHANNEL_12, (uint32_t)(udma_buffer_rx+(UDMA_BUFFER_SIZE-1)));
		CfgDMAChContrWrd(UDMA_CHANNEL_12, control_word_ch12);

		CfgDMAChSrcAdd(UDMA_CHANNEL_13, (uint32_t)(udma_buffer_tx+(UDMA_BUFFER_SIZE-1)));
		CfgDMAChDesAdd(UDMA_CHANNEL_13, (uint32_t)&(SSI2->DR));
		CfgDMAChContrWrd(UDMA_CHANNEL_13, control_word_ch13);
	}
	else
	{
		//TODO: ASSERT(ERROR)
		while(1);
	}
}

static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address)
{
	udma_control_structure[channel*4] = end_address;
}

static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address)
{
	udma_control_structure[(channel*4)+1] = end_address;
}

static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word)
{
	udma_control_structure[(channel*4)+2] = control_word;
}


/*************************************************************************/
/*						  Exported Functions							   */
/*************************************************************************/
void UDMA_Init(void)
{
	control_word_ch12 = (0<<30)	//destination address increment
										|(0<<28)		//destination data size
										|(3<<26)		//source address increment
										|(0<<24)		//source data size
										|(2<<14)		//Arbitration size
										|((UDMA_BUFFER_SIZE-1)<<4)		//Transfer size (minus 1)
										|(0<<3)		//next useburst
										|(1<<0);		//Basic mode
	control_word_ch13 =	(3<<30)	//destination address increment
										|(0<<28)		//destination data size
										|(0<<26)		//source address increment
										|(0<<24)		//source data size
										|(2<<14)		//Arbitration size
										|((UDMA_BUFFER_SIZE-1)<<4)		//Transfer size (minus 1)
										|(0<<3)		//next useburst
										|(1<<0);		//Basic mode
	SSI2DMAConfiguration();
}

void UDMA_SetChSwRqt(uint32_t channel_num)
{
	UDMA->SWREQ = (1<<channel_num);
}

UDMA_status_T UDMA_GetStatus(void)
{
	uint32_t temp = 0;
	UDMA_status_T return_val;
	temp = UDMA->STAT;
	return_val.master_enable_status = (uint8_t)(temp&0x01);
	return_val.num_configured_channels = (uint8_t)((temp>>16)&0x1F);
	return_val.state_machine_status = (UDMA_state_T)((temp>>4)&0xF);

	if((return_val.state_machine_status == DMA_IDLE)
		||(return_val.state_machine_status == DMA_READING_CHANNEL_DATA)
		||(return_val.state_machine_status == DMA_READING_SOURCE_END_POINTER)
		||(return_val.state_machine_status == DMA_READING_DESTINATION_END_POINTER)
		||(return_val.state_machine_status == DMA_READING_SOURCE_DATA)
		||(return_val.state_machine_status == DMA_WRITING_DESTINATION_DATA)
		||(return_val.state_machine_status == DMA_WAITING_REQUEST_CLEAR)
		||(return_val.state_machine_status == DMA_WRITING_CHANNEL_DATA)
		||(return_val.state_machine_status == DMA_STALLED)
		||(return_val.state_machine_status == DMA_DONE))
	{
		//CLEAR ERROR
	}
	else
	{
		//SET ERROR
	}
	return return_val;
}

uint32_t UDMA_GetWaitOnRqtStatus(void)
{
	return (UDMA->WAITSTAT);
}

void UDMA_SetSSI2TxData(uint8_t* buffer, uint8_t len)
{
	uint32_t iterator = 0;
	for(iterator=0;iterator<len;iterator++)
	{
		udma_buffer_tx[tx_transfer_size-len+iterator] = *buffer;
		buffer++;
	}
}


void UDMA_UpdateSSI2RxData(uint8_t len)
{
	uint32_t iterator = 0;
	uint32_t receive_iterator = 0;
	while(((UDMA->ENASET)&(1<<12)) == (1<<12));
	for(iterator=(tx_transfer_size-len);iterator<len;iterator++)
	{
		UDMA_ssi2_app_rx_data[receive_iterator] = udma_buffer_rx[iterator];
		receive_iterator++;
	}
}

void UDMA_SSI2Enable(void)
{
	UDMA->ENASET = (uint32_t)((1<<12)|(1<<13)); //Enable
}

/* size can be from 1 to 256 */
void UDMA_RxTransferSize(uint8_t size)
{
	control_word_ch12 &= ~(0x3FF<<4);
	control_word_ch12 |= ((size-1)<<4);
	CfgDMAChDesAdd(UDMA_CHANNEL_12, (uint32_t)(udma_buffer_rx+(UDMA_BUFFER_SIZE-1)));
	CfgDMAChContrWrd(UDMA_CHANNEL_12, control_word_ch12);
	tx_transfer_size = size;
}

/* size can be from 1 to 255 */
void UDMA_TxTransferSize(uint8_t size)
{
	control_word_ch13 &= ~(0x3FF<<4);
	control_word_ch13 |= ((size-1)<<4);
	CfgDMAChSrcAdd(UDMA_CHANNEL_13, (uint32_t)(udma_buffer_tx+(UDMA_BUFFER_SIZE-1)));
	CfgDMAChContrWrd(UDMA_CHANNEL_13, control_word_ch13);
	tx_transfer_size = size;
}

uint8_t UDMA_GetTxTransferSize(void)
{
	return tx_transfer_size;
}

uint8_t UDMA_GetRxTransferSize(void)
{
	return rx_transfer_size;
}











