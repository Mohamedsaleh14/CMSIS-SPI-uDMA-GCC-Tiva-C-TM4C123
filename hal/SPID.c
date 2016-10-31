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
#include "SPID.h"
#include "../cmsis/LM4F120H5QR.h"

#if (IS_SPI == 1)

static void SSI0Init(void);
static void SSI1Init(void);
static void SSI2Init(void);
static void SSI3Init(void);


#ifdef SPI_0
static void SSI0Init(void)
{
	SYSCTL->RCGCGPIO |= 0x01; //Enable clock portA
	GPIOA->AFSEL |= 0x3C;			//Pins, 2,3,4,5
	GPIOA->PCTL |= ((2<<2)|(2<<3)|(2<<4)|(2<<5));
	GPIOA->DEN  |= 0x3C;

	if(((SSI0->CR1)&0x02) == 0)		//if peripheral is disabled proceed with configuration
	{
		SSI0->CR1 = ((EOT<<4)
				|(SSI0_SLAVE_OUTPUT_MODE<<3)
				|(SSI0_MASTER_SLAVE<<2)
				|(SSI0_TEST_MODE<<0));
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
		SSI1->CR1 = ((EOT<<4)
				|(SSI1_SLAVE_OUTPUT_MODE<<3)
				|(SSI1_MASTER_SLAVE<<2)
				|(SSI1_TEST_MODE<<0));
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
		SSI2->CR1 = ((EOT<<4)
				|(SSI2_SLAVE_OUTPUT_MODE<<3)
				|(SSI2_MASTER_SLAVE<<2)
				|(SSI2_TEST_MODE<<0));
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
		SSI3->CR1 = ((EOT<<4)
				|(SSI3_SLAVE_OUTPUT_MODE<<3)
				|(SSI3_MASTER_SLAVE<<2)
				|(SSI3_TEST_MODE<<0));
	}
}
#endif

void SPID_Init(SPID_SpiPort_T sssix)
{
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



#endif










