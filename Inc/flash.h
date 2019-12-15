/*
 * flash.h
 *
 *  Created on: Nov 28, 2019
 *      Author: Karol Borowski
 */

#ifndef FLASH_H_
#define FLASH_H_


#include "main.h"
#include "alarm.h"

//#define __IO volatile

#define FLASH_BASE_ADDR			0x8000000

#define FLASH_USER_START_ADDR   FLASH_BASE_ADDR+(GetSectorSize(FLASH_SECTOR_2)*FLASH_SECTOR_2)   /* Start @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)



void FLASH_ReadUsers(UserType *userList);
void FLASH_WriteUsers(UserType *userList);


#endif /* FLASH_H_ */
