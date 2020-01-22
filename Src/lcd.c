/*
 * lcd.c
 *
 *  Created on: Nov 28, 2019
 *      Author: Szymon Jedliński
 */

#include "lcd.h"

/**
 * Initialization of LCD
 */
void LCD_Init(void) {
	// 4 bit initialization, accordingly to datasheet
//	HAL_Delay(50);  	 // wait for > 40ms
//	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
//	HAL_Delay(5);  		 // wait for > 4.1ms
//	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
//	HAL_Delay(1);  		 // wait for > 100us
//	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
//	HAL_Delay(10);
//	LCD_Send_Cmd(0x20); // Function set --> DB5 = 1, DL = 0 (4-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
//	HAL_Delay(10);
//
//	// display initialization
//	LCD_Send_Cmd(0x28); // Function set --> DB5 = 1, DL=0 (4 bit interface), N = 1 (2 line display) F = 0 (5x8 characters)
//	HAL_Delay(1);
//	LCD_Send_Cmd(0x08); // Display on/off control --> DB3 = 0, D = 0 (display off), C = 0 (cursor off), B = 0 (cursor blink off)
//	HAL_Delay(1);
//	LCD_Send_Cmd(0x01); // Clear Display --> DB = 0, cursor returns to the home position, address = 0
//	HAL_Delay(1);
//	LCD_Send_Cmd(0x06); // Entry mode set --> DB2 = 1, I/D = 1 (increment cursor), S = 0 (no shift)
//	HAL_Delay(1);
//	LCD_Send_Cmd(0x0C); // Display on/off control --> D = 1 (display on), C and B = 0. (cursor and cursor blink off)

	HAL_Delay(60);  	 // wait for > 40ms
	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
	HAL_Delay(10);  		 // wait for > 4.1ms
	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
	HAL_Delay(10);  		 // wait for > 100us
	LCD_Send_Cmd(0x30); // Function set --> DB5 = 1, DL = 1 (8-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
	HAL_Delay(50);
	LCD_Send_Cmd(0x20); // Function set --> DB5 = 1, DL = 0 (4-bit interface), N = 0 (1 line display), F = 0 (5x8 characters)
	HAL_Delay(50);

	// display initialization
	LCD_Send_Cmd(0x28); // Function set --> DB5 = 1, DL=0 (4 bit interface), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(5);
	LCD_Send_Cmd(0x08); // Display on/off control --> DB3 = 0, D = 0 (display off), C = 0 (cursor off), B = 0 (cursor blink off)
	HAL_Delay(5);
	LCD_Send_Cmd(0x01); // Clear Display --> DB = 0, cursor returns to the home position, address = 0
	HAL_Delay(5);
	LCD_Send_Cmd(0x06); // Entry mode set --> DB2 = 1, I/D = 1 (increment cursor), S = 0 (no shift)
	HAL_Delay(5);
	LCD_Send_Cmd(0x0C); // Displ
}

/**
 * Sending command to LCD via I2C
 *
 * @param cmd Sent command
 */
void LCD_Send_Cmd(char cmd) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = data_u | 0x0C;  //en=1, rs=0
	data_t[1] = data_u | 0x08;  //en=0, rs=0
	data_t[2] = data_l | 0x0C;  //en=1, rs=0
	data_t[3] = data_l | 0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t, 4,
			100);
//	for(uint8_t i=0; i<255; ++i){
//		HAL_I2C_Master_Transmit (&hi2c1, i,(uint8_t *) data_t, 4, 100);
//	}
}

/**
 * Sending data to LCD via I2C
 *
 * @param data Data to be sent
 */
void LCD_Send_Data(char data) {
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = data_u | 0x0D;  //en=1, rs=1
	data_t[1] = data_u | 0x09;  //en=0, rs=1
	data_t[2] = data_l | 0x0D;  //en=1, rs=1
	data_t[3] = data_l | 0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t*) data_t, 4,
			100);
}

/**
 * Clear data on screen
 */
void LCD_Clear(void) {
	LCD_Send_Cmd(0x80); // set DDRAM address (display data RAM)
	for (int i = 0; i < 70; i++) {
		LCD_Send_Data(' ');
	}
}

/**
 * Display string on the screen
 *
 * @param str String to be displayed
 */
void LCD_Send_String(char *str) {
	while (*str)
		LCD_Send_Data(*str++);
}

/**
 * Display string on specified position
 *
 * @param line Position on Y-axis
 * @param position Position on X-axis
 * @param str Displayed string
 */
void LCD_Print_X_Y(uint8_t line, uint8_t position, char *str) // line range -> 0-1 (first and second row)
{		// position range -> 0-15(counting columns starting from left to right)
	if (line == 0) {
		uint8_t line_x_y = 0x80 + position;
		LCD_Send_Cmd(line_x_y);
		while (*str)
			LCD_Send_Data(*str++);
	} else if (line == 1) {
		uint8_t line_x_y = 0x80 | (0x40 + position);
		LCD_Send_Cmd(line_x_y);
		while (*str)
			LCD_Send_Data(*str++);
	}
}

/**
 * Set cursor on given position.
 *
 * @param line Position on Y-axis
 * @param position Position on X-axis
 */
void LCD_Set_Cursor_X_Y(uint8_t line, uint8_t position) {
	if (line == 0) {
		uint8_t line_x_y = 0x80 + position;
		LCD_Send_Cmd(line_x_y);
	} else if (line == 1) {
		uint8_t line_x_y = 0x80 | (0x40 + position);
		LCD_Send_Cmd(line_x_y);
	}
}
