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
 * File:			SRHL.c
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	Sender/Receiver Handler.
 */
#include <stdint.h>
#include "../common.h"
#include "SRHL.h"
#include "TIMD.h"
#include "uDMA.h"
#include "SPID.h"
#include "../cmsis/LM4F120H5QR.h"
#include "../cmsis/core_cm4.h"
#include "../cmsis/core_cmFunc.h"

static void GPIOsInit(void);
static void ChipSelect(void);
static void ChipDeselect(void);

static void ChipSelect(void)
{
	GPIOE->DATA &= ~(1<<0);
}
static void ChipDeselect(void)
{
	GPIOE->DATA |= (1<<0);
}
static void GPIOsInit(void)
{
	SYSCTL->RCGCGPIO |= 0x12; //Enable clock for port E and port B
	GPIOE->DIR |= ((1<<0)|(1<<4));  // make both PINs as output
	GPIOE->DEN |= ((1<<0)|(1<<4));
	GPIOE->DATA &= ~(1<<4);  // output zero
	GPIOE->DATA |= (1<<0);

	GPIOB->DR4R |= (1<<1);
	GPIOB->PUR |= (1<<1);
	GPIOB->DEN |= (1<<1);

	/*
	 * Configure HOST IRQ line
	 *
	 * GPIO PB2 as input
	 * Pad Configuration ---> 2ms, rising edge,Type STD WPU
	 *
	 * Interrupt type rising edge
	 * GPIO interrupt clear then disable
	 * enable interrupt for portB inside the processor vtable
	 * Processor interrupt enable
	 */
	GPIOB->DIR &= ~(1<<2);
	GPIOB->DR2R |= (1<<2);
	GPIOB->PDR |= (1<<2);
	GPIOB->IEV |= (1<<2);
	GPIOB->ICR |= (1<<2);
	GPIOB->IM &= ~(1<<2);
	// set interrupt handler for PB2 TODO
}

Fd_t SRHL_IfOpen(char* pIfName , unsigned long flags)
{
	/*
	 * Initialize PE4 (nHIB) as GPIO, LOW
	 * Initialize PE0 (CS) as GPIO HIGH
	 * Enable pull up on PB1
	 */
	GPIOsInit();

	/*
	 * Delay 1ms
	 */
	TIMD_WaitTimerA(1000);
	/*
	 * Enable WLAN interrupt
	 */
	//CC3100_InterruptEnable();
	return 0;
}


int SRHL_IfClose(Fd_t Fd)
{
	/*
	 * Disable WLAN interrupt
	 */
	//CC3100_InterruptDisable();
	return 0;
}

int SRHL_IfWrite(Fd_t Fd , char* pBuff , int Len)
{
	uint8_t dumy_len = 4;
	SPID_Status_T spi_status;
	int len_to_return;
	char* local_buf;
	local_buf = pBuff;
	len_to_return = Len;
	ChipSelect();
	while(Len>0)
	{
		if(Len<=4)
		{
			local_buf = pBuff;
			dumy_len = (uint8_t)Len;
//			spi_status = SPID_GetSSI2Status();
//			while(!((spi_status.transmit_fifo_empty == 1)&&(spi_status.receive_fifo_not_empty == 0)))
//			{
//				spi_status = SPID_GetSSI2Status();
//			}
			UDMA_SetSSI2TxData((uint8_t*)local_buf, dumy_len);
			UDMA_TxTransferSize(dumy_len);
			UDMA_RxTransferSize(dumy_len);
			UDMA_SSI2Enable();
			pBuff += dumy_len;
			Len = 0;
		}
		else
		{
			local_buf = pBuff;
			dumy_len = 4;
//			spi_status = SPID_GetSSI2Status();
//			while(!((spi_status.transmit_fifo_empty == 1)&&(spi_status.receive_fifo_not_empty == 0)))
//			{
//				spi_status = SPID_GetSSI2Status();
//			}
			UDMA_SetSSI2TxData((uint8_t*)local_buf, dumy_len);
			UDMA_TxTransferSize(dumy_len);
			UDMA_SSI2Enable();
			pBuff += dumy_len;
			Len-=dumy_len;
		}
	}
	ChipDeselect();
	return 0;
}

int SRHL_IfRead(Fd_t Fd , char* pBuff , int Len)
{
	uint8_t dumy_len = 4;
	uint8_t iterator = 0;
	uint8_t dumy_tx_buff[] = {0xFF, 0xFF, 0xFF, 0xFF};
	int len_to_return;
	len_to_return = Len;
	UDMA_SetSSI2TxData(dumy_tx_buff, dumy_len);
	ChipSelect();
	while(Len>0)
	{
		if(Len<=4)
		{
			dumy_len = (uint8_t)Len;
			UDMA_TxTransferSize(dumy_len);
			UDMA_RxTransferSize(dumy_len);
			UDMA_SSI2Enable();
			UDMA_UpdateSSI2RxData(dumy_len);
			for(iterator=0;iterator<dumy_len;iterator++)
			{
				*pBuff = UDMA_ssi2_app_rx_data[UDMA_BUFFER_SIZE-dumy_len+iterator];
				pBuff++;
			}
			Len = 0;
		}
		else
		{
			if((Len%4)==0)
			{
				dumy_len = 4;
			}
			else
			{
				dumy_len = Len;
			}
			UDMA_TxTransferSize(dumy_len);
			UDMA_RxTransferSize(dumy_len);
			UDMA_SSI2Enable();
			UDMA_UpdateSSI2RxData(dumy_len);
			for(iterator=0;iterator<dumy_len;iterator++)
			{
				*pBuff = UDMA_ssi2_app_rx_data[UDMA_BUFFER_SIZE-dumy_len+iterator];
				pBuff++;
			}
			Len-=dumy_len;
		}
	}
	ChipDeselect();
	return len_to_return;
}




