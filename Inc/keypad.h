/*
 * keypad.h
 *
 *  Created on: Dec 15, 2019
 *      Author: Szymon Jedliński
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "main.h"
#include "alarm.h"

#define KEYPAD_COLUMN_PORT			KEY_C1_GPIO_Port
#define KEYPAD_ROW_PORT				KEY_R1_GPIO_Port

typedef enum {
	NONE, C1, C2, C3, R1, R2, R3, R4
} ButtonClickedType;

char KEYPAD_ReadKey(uint16_t GPIO_Pin);
void KEYPAD_EXTI_Callback(uint16_t GPIO_Pin);
void KEYPAD_ResetCounter();

extern char pin[PASSWORD_LENGTH];

#endif /* KEYPAD_H_ */
