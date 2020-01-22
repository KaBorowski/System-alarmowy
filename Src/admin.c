/*
 * admin.c
 *
 *  Created on: 12 sty 2020
 *      Author: Karol Borowski
 */

#include "admin.h"
#include "string.h"
#include "lcd.h"
#include "state_machine.h"

extern UART_HandleTypeDef *huart_admin;

/* Private functions----------------------------------------------------------*/
//static void clearFrame(AdminFrame_TypeDef *frame);

/* Private variables ---------------------------------------------------------*/
static volatile TransmissionStatus admin_receive_status = IDLE;
static uint8_t rx_buffer[ADMIN_MIN_FRAME_LENGTH + ADMIN_MAX_DATA_SIZE];
static uint8_t rx_it = 255;
static uint8_t rx_index = 0;

const uint8_t adminPassword[PASSWORD_LENGTH] = { 1, 2, 3, 4 };
AdminFrame_TypeDef admin_frame;

/**
 * Initialize admin uart
 */
void ADMIN_Init() {
	HAL_UART_Receive_IT(huart_admin, &rx_it, 1);
}

/**
 * Receive byte interrupt handler
 */
void ADMIN_UART_RxCpltCallback() {
	//check if start bit
	if (rx_it == ADMIN_START1 && admin_receive_status != RECEIVING) {
		rx_index = 0;
		rx_buffer[rx_index] = rx_it;
		rx_index++;
	} else if (rx_it == ADMIN_START2
			&& rx_index == 1&& rx_buffer[rx_index-1] == ADMIN_START1) {
		rx_index = ADMIN_LENGTH_OFFSET;
		rx_buffer[rx_index - 1] = ADMIN_START2;
		admin_receive_status = RECEIVING;
	} //get data length
	else if (admin_receive_status == RECEIVING) {
		rx_buffer[rx_index] = rx_it;
		rx_index++;
		if (rx_index > (ADMIN_MIN_FRAME_LENGTH + ADMIN_MAX_DATA_SIZE))
			rx_index = 0;
	} else {
		rx_index = 0;
		memset(rx_buffer, 0, ADMIN_MIN_FRAME_LENGTH + ADMIN_MAX_DATA_SIZE);
	}
	if (rx_it == ADMIN_CRC
			&& admin_receive_status
					== RECEIVING && rx_index == ADMIN_CRC_OFFSET(rx_buffer[ADMIN_LENGTH_OFFSET])) {
		rx_index = 0;
		admin_frame.length = rx_buffer[ADMIN_LENGTH_OFFSET];
		admin_frame.command = rx_buffer[ADMIN_COMMAND_OFFSET];
		for (uint8_t i = 0; i < (admin_frame.length - 1); i++) {
			admin_frame.data[i] = rx_buffer[ADMIN_DATA_OFFSET + i];
		}
		memset(rx_buffer, 0, ADMIN_MIN_FRAME_LENGTH + ADMIN_MAX_DATA_SIZE);
		admin_receive_status = RECEIVED;
	} //add bit to buffer

	HAL_UART_Receive_IT(huart_admin, &rx_it, 1);
}

/**
 * Check if full frame is received
 *
 * @return Received command or FALSE if nothing was received
 */
uint8_t ADMIN_CheckIfReceivedMsg() {
	if (admin_receive_status == RECEIVED) {
		admin_receive_status = IDLE;
		return admin_frame.command;
	} else
		return FALSE;
}


//static void clearFrame(AdminFrame_TypeDef *frame) {
//	frame->command = 0;
//	frame->length = 0;
//	memset(frame->data, 0, ADMIN_MAX_DATA_SIZE * sizeof(frame->data[0]));
//}

/*
 * Send frame to admin
 */
void ADMIN_SendFrame(uint8_t command, uint8_t *data, uint8_t data_length) {
	uint8_t frame_content[ADMIN_MIN_FRAME_LENGTH + ADMIN_MAX_DATA_SIZE];

	if (data == NULL && data_length != 0)
		data_length = 0;
	else if (data_length > ADMIN_MAX_DATA_SIZE)
		data_length = ADMIN_MAX_DATA_SIZE;

	frame_content[0] = ADMIN_START1;
	frame_content[1] = ADMIN_START2;
	frame_content[ADMIN_LENGTH_OFFSET] = data_length + 1;
	frame_content[ADMIN_COMMAND_OFFSET] = command;

	for (int i = 0; i < data_length; i++) {
		frame_content[ADMIN_DATA_OFFSET + i] = data[i];
	}

	frame_content[ADMIN_CRC_OFFSET(data_length)] = ADMIN_CRC;

	HAL_UART_Transmit(huart_admin, frame_content,
			ADMIN_CRC_OFFSET(data_length) + 1, 100);

}

