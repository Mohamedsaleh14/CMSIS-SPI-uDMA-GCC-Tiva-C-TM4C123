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
 * File:			SPID.h
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	SPI driver.
 */

#ifndef HAL_SPID_H_
#define HAL_SPID_H_

/** SPI Configuration */
#define IS_SPI					1		// 1:there is SPI in system 0:there is no SPI in system

#define SPI_0	//Configure SSI0
//#define SPI_1	//Configure SSI1
//#define SPI_2	//Configure SSI2
//#define SPI_3	//Configure SSI3


#ifdef SPI_0
#define SSI0_MASTER_SLAVE		1		// 1:Master  0:Slave
#define SSI0_SPH					0		//Serial Clock Phase
#define SSI0_SPO					0		//Serial Clock Polarity
#define SSI0_FRF					0		//Frame Format Select
#define SSI0_DSS					0		//Data Size Select
#define SSI0_TEST_MODE			0		// 1:Test mode enabled   0:Test mode disabled
#if (SSI0_MASTER_SLAVE == 0)
#define SSI0_SLAVE_OUTPUT_MODE	0		// 1:Tx output disabled	0:Tx output enabled
#else
#define SSI0_EOT					0		// 1:End of transmit interrupt interrupt mode enabled 0: Indicate FIFO is half full of less
#endif
#endif

#ifdef SPI_1
#define SSI1_MASTER_SLAVE		0		// 1:Master  0:Slave
#define SSI1_SPH					0		//Serial Clock Phase
#define SSI1_SPO					0		//Serial Clock Polarity
#define SSI1_FRF					0		//Frame Format Select
#define SSI1_DSS					0		//Data Size Select
#define SSI1_TEST_MODE			0		// 1:Test mode enabled   0:Test mode disabled
#if (SSI1_MASTER_SLAVE == 0)
#define SSI1_SLAVE_OUTPUT_MODE	0		// 1:Tx output disabled	0:Tx output enabled
#else
#define SSI1_EOT					0		// 1:End of transmit interrupt interrupt mode enabled 0: Indicate FIFO is half full of less
#endif
#endif

#ifdef SPI_2
#define SSI2_MASTER_SLAVE		0		// 1:Master  0:Slave
#define SSI2_SPH					0		//Serial Clock Phase
#define SSI2_SPO					0		//Serial Clock Polarity
#define SSI2_FRF					0		//Frame Format Select
#define SSI2_DSS					0		//Data Size Select
#define SSI2_TEST_MODE			0		// 1:Test mode enabled   0:Test mode disabled
#if (SSI2_MASTER_SLAVE == 0)
#define SSI2_SLAVE_OUTPUT_MODE	0		// 1:Tx output disabled	0:Tx output enabled
#else
#define SSI2_EOT					0		// 1:End of transmit interrupt interrupt mode enabled 0: Indicate FIFO is half full of less
#endif
#endif

#ifdef SPI_3
#define SSI3_MASTER_SLAVE		0		// 1:Master  0:Slave
#define SSI3_SPH					0		//Serial Clock Phase
#define SSI3_SPO					0		//Serial Clock Polarity
#define SSI3_FRF					0		//Frame Format Select
#define SSI3_DSS					0		//Data Size Select
#define SSI3_TEST_MODE			0		// 1:Test mode enabled   0:Test mode disabled
#if (SSI3_MASTER_SLAVE == 0)
#define SSI3_SLAVE_OUTPUT_MODE	0		// 1:Tx output disabled	0:Tx output enabled
#else
#define SSI3_EOT					0		// 1:End of transmit interrupt interrupt mode enabled 0: Indicate FIFO is half full of less
#endif
#endif


typedef enum{
	SSI_0,
	SSI_1,
	SSI_3,
	SSI_4
}SPID_SpiPort_T;



#endif /* HAL_SPID_H_ */
