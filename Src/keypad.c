/*
 * keypad.c
 *
 *  Created on: Dec 15, 2019
 *      Author: Boro
 */


#include "keypad.h"
#include "lcd.h"


//***** Library variables *****//

static uint32_t DebouncingFactor = 10000;
static char key;
static uint16_t StateColumn1;
static uint16_t StateColumn2;
static uint16_t StateColumn3;
static uint8_t pressCounter = 0;

char pin[PASSWORD_LENGTH];

BoolType isPressed = FALSE;
ButtonClickedType clickedColumn;


//***** Functions definition *****//
void static resetRows();

char KEYPAD_ReadKey(uint16_t GPIO_Pin)
{
		ButtonClickedType row = NONE;
		isPressed = FALSE;

		/* Debouncing */
		for(uint32_t i = 0;i < DebouncingFactor;i++){}
		if(HAL_GPIO_ReadPin(KEYPAD_COLUMN_PORT, GPIO_Pin)) return key;

		isPressed = TRUE;
		pressCounter++;

		/* Check in which row is clicked button */
		HAL_GPIO_WritePin (KEY_R1_GPIO_Port, KEY_R1_Pin, GPIO_PIN_SET);

		if (HAL_GPIO_ReadPin (KEYPAD_COLUMN_PORT, GPIO_Pin))		row = R1;
		if(row == NONE){
			HAL_GPIO_WritePin (KEY_R2_GPIO_Port, KEY_R2_Pin, GPIO_PIN_SET);
			if (HAL_GPIO_ReadPin (KEYPAD_COLUMN_PORT, GPIO_Pin))	row = R2;
		}
		if (row == NONE){
			HAL_GPIO_WritePin (KEY_R3_GPIO_Port, KEY_R3_Pin, GPIO_PIN_SET);
			if (HAL_GPIO_ReadPin (KEYPAD_COLUMN_PORT, GPIO_Pin))	row = R3;
		}
		if (row == NONE){
			HAL_GPIO_WritePin (KEY_R4_GPIO_Port, KEY_R4_Pin, GPIO_PIN_SET);
			if (HAL_GPIO_ReadPin (KEYPAD_COLUMN_PORT, GPIO_Pin))	row = R4;
		}
		resetRows();

		switch(GPIO_Pin)
		{
			case KEY_C1_Pin:
				switch (row) {
					case R1: return '1';
					case R2: return '4';
					case R3: return '7';
					case R4: return '*';
					default: return key;
				}
				break;

			case KEY_C2_Pin:
				switch (row) {
					case R1: return '2';
					case R2: return '5';
					case R3: return '8';
					case R4: return '0';
					default: return key;
				}
				break;

			case KEY_C3_Pin:
				switch (row) {
					case R1: return '3';
					case R2: return '6';
					case R3: return '9';
					case R4: return '#';
					default: return key;
				}
				break;

			default: return 'q';
		}

}

void KEYPAD_EXTI_Callback(uint16_t GPIO_Pin){
	StateColumn1 = HAL_GPIO_ReadPin(KEYPAD_COLUMN_PORT, KEY_C1_Pin);
	StateColumn2 = HAL_GPIO_ReadPin(KEYPAD_COLUMN_PORT, KEY_C2_Pin);
	StateColumn3 = HAL_GPIO_ReadPin(KEYPAD_COLUMN_PORT, KEY_C3_Pin);

	switch (GPIO_Pin)
	{
		case KEY_C1_Pin:
			if (StateColumn2 && StateColumn3)
			{
				key = KEYPAD_ReadKey(GPIO_Pin);
				break;
			}
			else
			{
				break;
			}
		case KEY_C2_Pin:
			if (StateColumn1 && StateColumn3)
			{
				key = KEYPAD_ReadKey(GPIO_Pin);
				break;
			}
			else
			{
				break;
			}
		case KEY_C3_Pin:
			if (StateColumn1 && StateColumn2)
			{
				key = KEYPAD_ReadKey(GPIO_Pin);
				break;
			}
			else
			{
				break;
			}
		}

		if (isPressed == TRUE)	{
			pin[pressCounter-1] = (char)key;
			LCD_Print_X_Y(1, pressCounter+9, "*");
		}

		if (pressCounter >= PASSWORD_LENGTH) {
			pressCounter = 0;
			authorizationStatus = PIN_TYPED;
		}
}

void KEYPAD_ResetCounter(){
	pressCounter = 0;
}

void static resetRows()
{
	HAL_GPIO_WritePin (KEY_R1_GPIO_Port, KEY_R1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (KEY_R2_GPIO_Port, KEY_R2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (KEY_R3_GPIO_Port, KEY_R3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (KEY_R4_GPIO_Port, KEY_R4_Pin, GPIO_PIN_RESET);
}
