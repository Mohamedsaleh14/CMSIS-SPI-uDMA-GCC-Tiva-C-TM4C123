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
 * File:			uDMAC.h
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	uDMA driver.
 */
#ifndef HAL_UDMA_H_
#define HAL_UDMA_H_

#define UDMA_BUFFER_SIZE		32

typedef enum{
	DMA_IDLE = 0,
	DMA_READING_CHANNEL_DATA = 1,
	DMA_READING_SOURCE_END_POINTER = 2,
	DMA_READING_DESTINATION_END_POINTER = 3,
	DMA_READING_SOURCE_DATA = 4,
	DMA_WRITING_DESTINATION_DATA = 5,
	DMA_WAITING_REQUEST_CLEAR = 6,
	DMA_WRITING_CHANNEL_DATA = 7,
	DMA_STALLED = 8,
	DMA_DONE = 9,
	DMA_UNDEFINED_0 = 0xA,
	DMA_UNDEFINED_1 = 0xB,
	DMA_UNDEFINED_2 = 0xC,
	DMA_UNDEFINED_3 = 0xD,
	DMA_UNDEFINED_4 = 0xE,
	DMA_UNDEFINED_5 =0xF
}UDMA_state_T;

typedef struct{
	uint8_t num_configured_channels;
	UDMA_state_T state_machine_status;
	uint8_t master_enable_status;
}UDMA_status_T;

uint16_t UDMA_ssi2_app_rx_data[UDMA_BUFFER_SIZE];

extern void UDMA_Init(void);
extern void UDMA_SetChSwRqt(uint32_t channel_num);
extern void UDMA_SetSSI2TxData(uint16_t* buffer);
extern void UDMA_UpdateSSI2RxData(void);
extern uint32_t UDMA_GetWaitOnRqtStatus(void);
extern UDMA_status_T UDMA_GetStatus(void);



#endif /* HAL_UDMA_H_ */
