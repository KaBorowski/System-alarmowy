/*
 * admin.h
 *
 *  Created on: 12 sty 2020
 *      Author: Karol Borowski
 */

#ifndef ADMIN_H_
#define ADMIN_H_

#include "main.h"
#include "alarm.h"

#define ADMIN_MAX_DATA_SIZE				ID_LENGTH + PASSWORD_LENGTH
#define ADMIN_MIN_FRAME_LENGTH			5
#define ADMIN_RECEIVE_LENGTH			ADMIN_MIN_FRAME_LENGTH + 1

#define ADMIN_LENGTH_OFFSET				2
#define ADMIN_COMMAND_OFFSET			3
#define ADMIN_DATA_OFFSET				4
#define ADMIN_LOCKER_MODULE_ADDR		ADMIN_DATA_OFFSET
#define ADMIN_LOCKER_PORT_ADR			(ADMIN_DATA_OFFSET + ADMIN_MODULE_ADDR_SIZE)
#define ADMIN_OPERATION_RESULT_OFFSET 	5
#define ADMIN_CRC_OFFSET(data_length)	(ADMIN_MIN_FRAME_LENGTH + data_length -1)

#define ADMIN_CRC						0
#define ADMIN_START1					255
#define ADMIN_START2					254

#define	ADMIN_ADD_USER					10
#define ADMIN_DELETE_USER				11
#define ADMIN_UNLOCK_USER				12
#define ADMIN_EDIT_USER					13
#define ADMIN_AUTHENTICATION			14

#define ADMIN_OPERATION_SUCCESS			20
#define ADMIN_OPERATION_FAIL			21
#define ADMIN_SEND_USER					22

#define ADMIN_ID_INDEX					0
#define ADMIN_PASS_INDEX				ID_LENGTH

typedef struct {

	uint8_t length;
	uint8_t command;
	uint8_t data[ADMIN_MAX_DATA_SIZE];

} AdminFrame_TypeDef;

void ADMIN_Init();
void ADMIN_UART_RxCpltCallback();
void ADMIN_SendFrame(uint8_t command, uint8_t *data, uint8_t data_length);
void ADMIN_SendResponse();
uint8_t ADMIN_CheckIfReceivedMsg();

extern const uint8_t adminPassword[PASSWORD_LENGTH];
extern AdminFrame_TypeDef admin_frame;

#endif /* ADMIN_H_ */
