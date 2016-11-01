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
 * File:			SPID.c
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	SPI driver.
 */


#include <stdint.h>
#include "../common.h"
#include "SPID.h"
#include "../cmsis/LM4F120H5QR.h"

#if (IS_SPI == 1)

#define SCR		1		//Serial clock rate BitRate = Sysclk /(CPSDVSR *(1+SCR))
#define SSI_ENABLE				(0x02)
#define RECEIVE_FIFO_NOT_EMPTY	(1<<2)
#define TRANSMIT_FIFO_NOT_FULL	(1<<1)

static SPID_SSI_T enabled_spi;
#ifdef SPI_0
SPID_Status_T ssi0_status;
#endif

#ifdef SPI_1
SPID_Status_T ssi1_status;
#endif

#ifdef SPI_2
SPID_Status_T ssi2_status;
#endif

#ifdef SPI_3
SPID_Status_T ssi3_status;
#endif

#ifdef SPI_0
static void SSI0Init(void);
#endif
#ifdef SPI_1
static void SSI1Init(void);
#endif
#ifdef SPI_2
static void SSI2Init(void);
#endif
#ifdef SPI_3
static void SSI3Init(void);
#endif


#ifdef SPI_0
static void SSI0Init(void)
{
	SYSCTL->RCGCGPIO |= 0x01; //Enable clock portA
	GPIOA->AFSEL |= 0x3C;			//Pins, 2,3,4,5
	GPIOA->PCTL |= ((2<<2)|(2<<3)|(2<<4)|(2<<5));
	GPIOA->DEN  |= 0x3C;

	if(((SSI0->CR1)&0x02) == 0)		//if peripheral is disabled proceed with configuration
	{
		SSI0->CR1 = (0
#if (SSI0_MASTER_SLAVE == 0)
				|(SSI0_SLAVE_OUTPUT_MODE<<3)
#else
				|(SSI0_EOT<<4)
#endif
				|(SSI0_MASTER_SLAVE<<2)
				|(SSI0_TEST_MODE<<0));

		/*Configure SPI clk 20Mhz*/
		SSI0->CPSR = 0x02;	//Divide system clock by 2
		SSI0->CR0 = ((SCR<<8)
				|(SSI0_SPH<<7)
				|(SSI0_SPO<<6)
				|(SSI0_FRF<<4)
				|(SSI0_DSS));
#if(SSI0_ENABLE_DMA_RX == 1)
		SSI0->DMACTL = 0x01;
#endif
#if(SSI0_ENABLE_DMA_TX == 1)
		SSI0->DMACTL |=(1<<1);
#endif
	}
}
#endif

#ifdef SPI_1
static void SSI1Init(void)
{
	SYSCTL->RCGCGPIO |= 0x20; //Enable clock  portF
	GPIOF->AFSEL |= 0x0F;			//Pins, 0,1,2,3
	GPIOF->PCTL |= ((2<<0)|(2<<1)|(2<<2)|(2<<3));
	GPIOF->DEN |= 0x0F;

	if(((SSI1->CR1)&0x02) == 0)		//if peripheral is disabled proceed with configuration
	{
		SSI1->CR1 = (8
#if (SSI1_MASTER_SLAVE == 0)
				|(SSI1_SLAVE_OUTPUT_MODE<<3)
#else
				|(SSI1_EOT<<4)
#endif
				|(SSI1_MASTER_SLAVE<<2)
				|(SSI1_TEST_MODE<<0));

		/*Configure SPI clk 20Mhz*/
		SSI1->CPSR = 0x02;	//Divide system clock by 2
		SSI1->CR0 = ((SCR<<8)
				|(SSI1_SPH<<7)
				|(SSI1_SPO<<6)
				|(SSI1_FRF<<4)
				|(SSI1_DSS));
	}
}
#endif

#ifdef SPI_2
static void SSI2Init(void)
{
	SYSCTL->RCGCGPIO |= 0x02; //Enable clock  portB
	GPIOB->AFSEL |= 0xF0;			//Pins, 4,5,6,7
	GPIOF->PCTL |= ((2<<4)|(2<<5)|(2<<6)|(2<<7));
	GPIOB->DEN |= 0xF0;

	if(((SSI2->CR1)&0x02) == 0)		//if peripheral is disabled proceed with configuration
	{
		SSI2->CR1 = (0
#if (SSI2_MASTER_SLAVE == 0)
				|(SSI2_SLAVE_OUTPUT_MODE<<3)
#else
				|(SSI2_EOT<<4)
#endif
				|(SSI2_MASTER_SLAVE<<2)
				|(SSI2_TEST_MODE<<0));

		/*Configure SPI clk 20Mhz*/
		SSI2->CPSR = 0x02;	//Divide system clock by 2
		SSI2->CR0 = ((SCR<<8)
				|(SSI2_SPH<<7)
				|(SSI2_SPO<<6)
				|(SSI2_FRF<<4)
				|(SSI2_DSS));
	}
}
#endif

#ifdef SPI_3
static void SSI3Init(void)
{
	SYSCTL->RCGCGPIO |= 0x08; //Enable clock portD
	GPIOD->AFSEL |= 0x0F;			//Pins, 0,1,2,3
	GPIOD->PCTL |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));
	GPIOD->DEN |= 0x0F;

	if(((SSI3->CR1)&0x02) == 0)		//if peripheral is disabled proceed with configuration
	{
		SSI3->CR1 = (0
#if (SSI3_MASTER_SLAVE == 0)
				|(SSI3_SLAVE_OUTPUT_MODE<<3)
#else
				|(SSI3_EOT<<4)
#endif
				|(SSI3_MASTER_SLAVE<<2)
				|(SSI3_TEST_MODE<<0));

		/*Configure SPI clk 20Mhz*/
		SSI3->CPSR = 0x02;	//Divide system clock by 2
		SSI3->CR0 = ((SCR<<8)
				|(SSI3_SPH<<7)
				|(SSI3_SPO<<6)
				|(SSI3_FRF<<4)
				|(SSI3_DSS));
	}
}
#endif

void SPID_Init(SPID_SpiPort_T ssix)
{
	static uint8_t first_entry = 0;
	if(first_entry == TRUE)
	{
		enabled_spi.ssi0_isenabled = 0;
		enabled_spi.ssi1_isenabled = 0;
		enabled_spi.ssi2_isenabled = 0;
		enabled_spi.ssi3_isenabled = 0;
#ifdef SPI_0
		ssi0_status.ssi_busy = 0;
		ssi0_status.receive_fifo_full = 0;
		ssi0_status.receive_fifo_not_empty = 0;
		ssi0_status.transmit_fifo_not_full = 1;
		ssi0_status.transmit_fifo_empty = 1;
#endif
#ifdef SPI_1
		ssi1_status.ssi_busy = 0;
		ssi1_status.receive_fifo_full = 0;
		ssi1_status.receive_fifo_not_empty = 0;
		ssi1_status.transmit_fifo_not_full = 1;
		ssi1_status.transmit_fifo_empty = 1;
#endif
#ifdef SPI_2
		ssi2_status.ssi_busy = 0;
		ssi2_status.receive_fifo_full = 0;
		ssi2_status.receive_fifo_not_empty = 0;
		ssi2_status.transmit_fifo_not_full = 1;
		ssi2_status.transmit_fifo_empty = 1;
#endif
#ifdef SPI_3
		ssi3_status.ssi_busy = 0;
		ssi3_status.receive_fifo_full = 0;
		ssi3_status.receive_fifo_not_empty = 0;
		ssi3_status.transmit_fifo_not_full = 1;
		ssi3_status.transmit_fifo_empty = 1;
#endif
		first_entry = FALSE;
	}
	SYSCTL->RCGCSSI |= (1<<(uint8_t)ssix);		//Enable clock for SPI peripheral

	switch(ssix){
#ifdef SPI_0
	case (SSI_0):
			SSI0Init();
			break;
#endif
#ifdef SPI_1
	case (SSI_1):
			SSI1Init();
			break;
#endif
#ifdef SPI_2
	case (SSI_2):
			SSI2Init();
			break;
#endif
#ifdef SPI_3
	case (SSI_3):
			SSI3Init();
			break;
#endif
	default:
		//Error
		while(1)
		{

		}
		break;
	}

}

void SPID_Enable (SPID_SpiPort_T ssix)
{
	switch(ssix)
	{
#ifdef SPI_0
	case SSI_0:
		SSI0->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi0_isenabled = 1;
		break;
#endif
#ifdef SPI_1
	case SSI_1:
		SSI1->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi1_isenabled = 1;
		break;
#endif
#ifdef SPI_2
	case SSI_2:
		SSI2->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi2_isenabled = 1;
		break;
#endif
#ifdef SPI_3
	case SSI_3:
		SSI3->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi3_isenabled = 1;
		break;
#endif
	default:
		//error
		while(1)
		{

		}
		break;
	}

}

void SPID_Disable(SPID_SpiPort_T ssix)
{
	switch(ssix)
	{
#ifdef SPI_0
	case SSI_0:
		SSI0->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi0_isenabled = 0;
		break;
#endif
#ifdef SPI_1
	case SSI_1:
		SSI1->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi1_isenabled = 0;
		break;
#endif
#ifdef SPI_2
	case SSI_2:
		SSI2->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi2_isenabled = 0;
		break;
#endif
#ifdef SPI_3
	case SSI_3:
		SSI3->CR1 |= (SSI_ENABLE);
		enabled_spi.ssi3_isenabled = 0;
		break;
#endif
	default:
		//error
		while(1)
		{

		}
		break;
	}
}


SPID_SSI_T SPID_GetEnabledSPI(void)
{
	return enabled_spi;
}

#ifdef SPI_0
uint8_t SPID_SSI0SendData(uint16_t data)
{
	uint8_t is_sw_ok = SW_NOK;
	if( (SSI0->SR&(1<<1)) == TRANSMIT_FIFO_NOT_FULL )
	{
		SSI0->DR = data;
		is_sw_ok = SW_OK;
	}
	else
	{
		//Error
		is_sw_ok = SW_NOK
	}
	return is_sw_ok;
}
uint16_t SPID_SSI0ReceiveData(void)
{
	uint16_t return_val = 0;
	if( (SSI0->SR&(1<<2)) == RECEIVE_NOT_EMPTY)
	{
		return_val = SSI0->DR;
	}
	return return_val;
}
SPID_Status_T SPID_GetSSI0Status(void)
{
	ssi0_status = SSI0->SR;
	return ssi0_status;
}
#endif


#ifdef SPI_1
uint8_t SPID_SSI1SendData(uint16_t data)
{
	uint8_t is_sw_ok = SW_NOK;
	if( (SSI1->SR&(1<<1)) == TRANSMIT_FIFO_NOT_FULL )
	{
		SSI1->DR = data;
		is_sw_ok = SW_OK;
	}
	else
	{
		//Error
		is_sw_ok = SW_NOK
	}
	return is_sw_ok;
}
uint16_t SPID_SSI1ReceiveData(void)
{
	uint16_t return_val = 0;
	if( (SSI1->SR&(1<<2)) == RECEIVE_NOT_EMPTY)
	{
		return_val = SSI1->DR;
	}
	return return_val;
}
SPID_Status_T SPID_GetSSI1Status(void)
{
	ssi1_status = SSI1->SR;
	return ssi1_status;
}
#endif



#ifdef SPI_2
uint8_t SPID_SSI2SendData(uint16_t data)
{
	uint8_t is_sw_ok = SW_NOK;
	if( (SSI2->SR&(1<<1)) == TRANSMIT_FIFO_NOT_FULL )
	{
		SSI2->DR = data;
		is_sw_ok = SW_OK;
	}
	else
	{
		//Error
		is_sw_ok = SW_NOK
	}
	return is_sw_ok;
}
uint16_t SPID_SSI2ReceiveData(void)
{
	uint16_t return_val = 0;
	if( (SSI2->SR&(1<<2)) == RECEIVE_NOT_EMPTY)
	{
		return_val = SSI2->DR;
	}
	return return_val;
}
SPID_Status_T SPID_GetSSI2Status(void)
{
	ssi2_status = SSI2->SR;
	return ssi2_status;
}
#endif


#ifdef SPI_3
uint8_t SPID_SSI3SendData(uint16_t data)
{
	uint8_t is_sw_ok = SW_NOK;
	if( (SSI3->SR&(1<<1)) == TRANSMIT_FIFO_NOT_FULL )
	{
		SSI3->DR = data;
		is_sw_ok = SW_OK;
	}
	else
	{
		//Error
		is_sw_ok = SW_NOK
	}
	return is_sw_ok;
}
uint16_t SPID_SSI3ReceiveData(void)
{
	uint16_t return_val = 0;
	if( (SSI3->SR&(1<<2)) == RECEIVE_NOT_EMPTY)
	{
		return_val = SSI3->DR;
	}
	return return_val;
}
SPID_Status_T SPID_GetSSI3Status(void)
{
	ssi3_status = SSI3->SR;
	return ssi3_status;
}
#endif


#endif


