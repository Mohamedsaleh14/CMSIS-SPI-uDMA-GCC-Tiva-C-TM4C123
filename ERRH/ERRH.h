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
 * File:			ERRH.h
 * Author:		Mohamed Saleh (mohamedsaleh.elec@gmail.com).
 * Version:		1.0.0.
 * Description:	Error Handler.
 */

#ifndef ERRH_ERRH_H_
#define ERRH_ERRH_H_

#define NUM_OF_ERRORS			5
#define NUM_OF_ERR_GROUPS		2

typedef enum{
	EER_NOT_ACTIVE = 0,
	ERR_NOT_TESTED,
	ERR_PASS,
	ERR_FAIL
} ERRH_status_T;

typedef enum{
	SPI_RECEIVE_TIMEOUT = 0,
	SPI_INIT_FAILED = 1,
	SPI_TRANSMIT_FAIL_FIFO_FULL = 2,
	UDMA_CONTROL_STRUCTURE_FAIL =3,
	UDMA_STATUS_UNDEFINED = 4
}ERRH_erros_id_T;

typedef enum{
	SPI_DRIVER = 0,
	UDMA_DRIVER =1
} ERRH_errors_group_T;
























#endif /* ERRH_ERRH_H_ */
