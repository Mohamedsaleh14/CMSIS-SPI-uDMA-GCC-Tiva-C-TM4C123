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
 * File:			TIMD.c
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	Timer driver.
 */
#include <stdint.h>
#include "../common.h"
#include "TIMD.h"
#include "../cmsis/LM4F120H5QR.h"
#include "../ERRH/ERRH.h"
#include "../cmsis/core_cm4.h"
#include "../cmsis/cmsis_nvic.h"
#include "uDMA.h"
#include "SPID.h"
#include "../ERRH/ERRH.h"

#define ONE_MS_TICKER_NUM	(80000)		//Configure SysTick to interrupt every 1ms

typedef struct{
	uint8_t task_trigger_5ms :1;
	uint8_t task_trigger_10ms :1;
	uint8_t RESERVED :6;
}Task_Trigger_T;

static uint8_t systick_interrupt_counter = 0;
static Task_Trigger_T triggered_tasks;

static void Systick_InterruptHandler(void);
static void Task_Initialization(void);
static void Task_5ms(void);
static void Task_10ms(void);


static void Systick_InterruptHandler(void)
{
	uint8_t temp = 0;
	systick_interrupt_counter++;
	temp = systick_interrupt_counter%5;

	if(temp == 0)
	{
		if (triggered_tasks.task_trigger_5ms == FALSE_BOOL)
		{
			triggered_tasks.task_trigger_5ms = TRUE_BOOL;
		}
		else
		{
			//Error TASK 5 ms overlap
			while(1)
			{

			}
		}
	}

	if(systick_interrupt_counter == 10)
	{
		if (triggered_tasks.task_trigger_10ms == FALSE_BOOL)
		{
			triggered_tasks.task_trigger_10ms = TRUE_BOOL;
		}
		else
		{
			//Error TASK 10 ms overlap
			while(1)
			{

			}
		}
	}
}

static void Task_5ms(void)
{
	uint16_t buffer_tx[UDMA_BUFFER_SIZE];
	uint16_t i =1;
	for(i=1;i<=UDMA_BUFFER_SIZE;i++)
	{
		buffer_tx[i-1] = i;
	}
	UDMA_SetSSI2TxData(buffer_tx);

//	for(i=1;i<2000;i++);

	UDMA_EnableAgain();
	UDMA->ENASET = (uint32_t)((1<<12)|(1<<13)); //Enable
	SPID_Enable(SSI_2);
}

static void Task_10ms(void)
{

}

static void Task_Initialization(void)
{
	UDMA_Init();
	SPID_Init(SSI_2);
}

void TIMD_SchInit(void)
{
	uint32_t status = 1;
	NVIC_SetVector(SysTick_IRQn, (uint32_t)Systick_InterruptHandler);
	status = SysTick_Config(ONE_MS_TICKER_NUM);
	if(status != 0)
	{
		//error
		while(1)
		{

		}
	}
	Task_Initialization();
}

void TIMD_SchAppl(void)
{
	if(triggered_tasks.task_trigger_5ms == TRUE_BOOL)
	{
		Task_5ms();
		triggered_tasks.task_trigger_5ms = FALSE_BOOL;
	}

	if(triggered_tasks.task_trigger_10ms == TRUE_BOOL)
	{
		Task_10ms();
		triggered_tasks.task_trigger_10ms = FALSE_BOOL;
	}
}














