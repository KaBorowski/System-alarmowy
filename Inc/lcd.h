/*
 * lcd.h
 *
 *  Created on: Nov 28, 2019
 *      Author: Szymon Jedliński
 */

#ifndef LCD_H_
#define LCD_H_

#include "main.h"
#include "stm32f4xx_hal.h"

#define SLAVE_ADDRESS_LCD 0x7E // change this according to your setup

extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

void LCD_Init(void);   // initialize LCD
void LCD_Send_Cmd(char cmd);  // send command to the LCD
void LCD_Send_Data(char data);  // send data to the LCD
void LCD_Send_String(char *str);  // send string to the LCD
void LCD_Clear(void); // clear the LCD
void LCD_Print_X_Y(uint8_t line, uint8_t position, char *str); // display string in defined line and position
void LCD_Set_Cursor_X_Y(uint8_t line, uint8_t position); //set cursor to the defined line and position

#endif /* LCD_H_ */
