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

#define UDMA_CHANNEL_12		(12)
#define UDMA_CHANNEL_13		(13)

static void Controlbase(void);
static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address);
static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word);

static uint8_t udma_control_structure[512] __attribute__ ((aligned(1024)));
static uint8_t udma_buffer_tx[64] = {0};
static uint8_t udma_buffer_rx[64] = {0};


static void Controlbase(void)
{
	uint32_t control_word_ch12 = 0;
	uint32_t control_word_ch13 = 0;

	control_word_ch12 = (0x03<<26) //source address is not increment
						|(0x06<<14) //Arbitration size is 64 bytes transfers
						|(0x07<<4) //transfer size in bits-1
						|(0x01<<3) //Next Useburst
						|(0x01);

	if((((uint32_t)udma_control_structure & ~(0x3FF)) == (uint32_t)udma_control_structure)
			&& ((uint32_t)udma_control_structure >= 0x20000000))
	{
		SYSCTL->RCGCDMA = (0x01); 	//Enable clock for DMA
		UDMA->CFG = (0x01);			//Enable uDMA controller
		UDMA->CTLBASE = (uint32_t)udma_control_structure;
		UDMA->PRIOSET = (1<<12);		//Setting priority for Receiving from SSI2
		UDMA->CHMAP1  = (1<<17 /*Map channel 12 to SSI2 Rx*/ )|(1<<21/*Map channel 13 to SSI2 Tx*/);
		UDMA->REQMASKSET = ~((1<<12)|(1<<13)); 	//Masking all channels to be not requested except for channel 12, 13
		CfgDMAChSrcAdd(UDMA_CHANNEL_12, SSI2->DR);
		CfgDMAChDesAdd(UDMA_CHANNEL_12, (uint32_t)udma_buffer_rx);
		CfgDMAChContrWrd(UDMA_CHANNEL_12, control_word_ch12);


	}
	else
	{
		//TODO: ASSERT(ERROR)
		while(1);
	}
}

static void CfgDMAChSrcAdd(uint8_t channel, uint32_t end_address)
{
	uint32_t* ptr;
	ptr = (uint32_t*)(udma_control_structure + (uint32_t)(channel<<4)); //point to channel source address container
	*ptr = (end_address>>24)&0xFF;
	ptr++;
	*ptr = (end_address>>16)&0xFF;
	ptr++;
	*ptr = (end_address>>8)&0xFF;
	ptr++;
	*ptr = (end_address)&0xFF;
}

static void CfgDMAChDesAdd(uint8_t channel, uint32_t end_address)
{
	uint32_t* ptr;
	ptr = (uint32_t*)(udma_control_structure + (uint32_t)(channel<<4) + (uint32_t)0x04); //point to channel destination address container
	*ptr = (end_address>>24)&0xFF;
	ptr++;
	*ptr = (end_address>>16)&0xFF;
	ptr++;
	*ptr = (end_address>>8)&0xFF;
	ptr++;
	*ptr = (end_address)&0xFF;
}

static void CfgDMAChContrWrd(uint8_t channel, uint32_t control_word)
{
	uint32_t* ptr;
	ptr = (uint32_t*)(udma_control_structure + (uint32_t)(channel<<4) + (uint32_t)0x08); //point to channel destination address container
	*ptr = (control_word>>24)&0xFF;
	ptr++;
	*ptr = (control_word>>16)&0xFF;
	ptr++;
	*ptr = (control_word>>8)&0xFF;
	ptr++;
	*ptr = (control_word)&0xFF;
}

void UDMA_Init(void)
{
	Controlbase();
}

