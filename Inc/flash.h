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

#define __IO volatile

#define FLASH_BASE_ADDR			0x8000000

#define FLASH_USER_START_ADDR   FLASH_BASE_ADDR+(GetSectorSize(FLASH_SECTOR_2)*FLASH_SECTOR_2)   /* Start @ of user Flash area */

#define DATA_32                 ((uint32_t)0x12345678)

#define FLASH_USER_SIZE				(PASSWORD_LENGTH+ID_LENGTH+1)
#define FLASH_DATA_SIZE				USERS_LIMIT*FLASH_USER_SIZE
#define ID_OFFSET(index)			index * FLASH_USER_SIZE + 1
#define PASS_OFFSET(index)			i * FLASH_USER_SIZE + ID_LENGTH + 1
#define STATUS_OFFSET(index)		i * FLASH_USER_SIZE + ID_LENGTH + PASSWORD_LENGTH + 1

uint8_t FLASH_ReadUsers(UserType *userList);
void FLASH_WriteUsers(UserType *userList, uint8_t usersAmount);

#endif /* FLASH_H_ */
