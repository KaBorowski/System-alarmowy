/*
 * flash.c
 *
 *  Created on: Nov 28, 2019
 *      Author: Karol Borowski
 */

#include "flash.h"
#include "string.h"

//static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);
static void readFlash(uint8_t *data_container, uint16_t data_size);
static void writeFlash(uint8_t *data, uint16_t data_size);

/**
 * Reading data about all users from FLASH memory
 *
 * @param userList Container for users data
 * @return Amount of users
 */
uint8_t FLASH_ReadUsers(UserType *userList) {
	uint8_t flashData[FLASH_DATA_SIZE];
	uint8_t usersAmount = 0;

	readFlash(flashData, FLASH_DATA_SIZE);

	usersAmount = flashData[0];
	for (uint8_t i = 0; i < USERS_LIMIT; ++i) {
		memcpy(userList[i].card_id, flashData + ID_OFFSET(i),
		ID_LENGTH);
		memcpy(userList[i].pass, flashData + PASS_OFFSET(i),
		PASSWORD_LENGTH);
		userList[i].isActive = *(flashData + STATUS_OFFSET(i));
	}

	return usersAmount;

}

/**
 * Writting data about users into FLASH memory
 *
 * @param userList Users array to be saved
 * @param usersAmount Current amount od users
 */
void FLASH_WriteUsers(UserType *userList, uint8_t usersAmount) {
	uint8_t flashData[FLASH_DATA_SIZE];
	memset(flashData, 0, FLASH_DATA_SIZE);

	flashData[0] = usersAmount;
	for (uint8_t i = 0; i < USERS_LIMIT; ++i) {
		memcpy(flashData + ID_OFFSET(i), userList[i].card_id, ID_LENGTH);
		memcpy(flashData + PASS_OFFSET(i), (uint8_t*) userList[i].pass,
		PASSWORD_LENGTH);
		memcpy(flashData + STATUS_OFFSET(i), &userList[i].isActive, 1);
	}

	writeFlash(flashData, FLASH_DATA_SIZE);
}

/**
 * Writting data into FLASH memory
 *
 * @param data Data written to memory
 * @param data_size Size of written data
 */
static void writeFlash(uint8_t *data, uint16_t data_size) {
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t FirstSector = 0, Address = FLASH_USER_START_ADDR;
	uint32_t SectorError = 0;

	HAL_FLASH_Unlock();

	/* Erase the user Flash area
	 (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	/* Get the 1st sector to erase */
//	  FirstSector = GetSector(FLASH_USER_START_ADDR);
	FirstSector = 2;
	/* Get the number of sector to erase from 1st sector*/

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FirstSector;
	EraseInitStruct.NbSectors = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
		HAL_FLASH_GetError();
	}

	/* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
	 you have to make sure that these data are rewritten before they are accessed during code
	 execution. If this cannot be done safely, it is recommended to flush the caches by setting the
	 DCRST and ICRST bits in the FLASH_CR register. */
	__HAL_FLASH_DATA_CACHE_DISABLE();
	__HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

	__HAL_FLASH_DATA_CACHE_RESET();
	__HAL_FLASH_INSTRUCTION_CACHE_RESET();

	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_DATA_CACHE_ENABLE();

	/* Program the user Flash area word by word
	 (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	Address = FLASH_USER_START_ADDR;

	for (int i = 0; i < data_size; i++) {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, Address, data[i])
				== HAL_OK) {
			Address = Address + sizeof(uint8_t);
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	 to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();
}

/**
 * Read data from FLASH memory
 *
 * @param data_container Cointaner for data
 * @param data_size Size of read data
 */
static void readFlash(uint8_t *data_container, uint16_t data_size) {
	uint32_t address = FLASH_USER_START_ADDR;
	__IO uint8_t data8 = 0;
	uint8_t data_index = 0;

	for (data_index = 0; data_index < data_size; data_index++) {
		data8 = *(__IO uint8_t*) address;
		data_container[data_index] = data8;
		address = address + sizeof(uint8_t);
	}

}

///**
// * @brief  Gets the sector of a given address
// * @param  None
// * @retval The sector of a given address
// */
//static uint32_t GetSector(uint32_t Address) {
//	uint32_t sector = 0;
//
//	if ((Address < FLASH_SECTOR_1) && (Address >= FLASH_SECTOR_0)) {
//		sector = FLASH_SECTOR_0;
//	} else if ((Address < FLASH_SECTOR_2) && (Address >= FLASH_SECTOR_1)) {
//		sector = FLASH_SECTOR_1;
//	} else if ((Address < FLASH_SECTOR_3) && (Address >= FLASH_SECTOR_2)) {
//		sector = FLASH_SECTOR_2;
//	} else if ((Address < FLASH_SECTOR_4) && (Address >= FLASH_SECTOR_3)) {
//		sector = FLASH_SECTOR_3;
//	} else if ((Address < FLASH_SECTOR_5) && (Address >= FLASH_SECTOR_4)) {
//		sector = FLASH_SECTOR_4;
//	} else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_5))*/
//	{
//		sector = FLASH_SECTOR_5;
//	}
//
//	return sector;
//}

/**
 * Gets sector Size
 *
 * @param  Sector Sector number
 * @return The size of a given sector
 */
static uint32_t GetSectorSize(uint32_t Sector) {
	uint32_t sectorsize = 0x00;

	if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1)
			|| (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3)) {
		sectorsize = 16 * 1024;
	} else if (Sector == FLASH_SECTOR_4) {
		sectorsize = 64 * 1024;
	} else {
		sectorsize = 128 * 1024;
	}
	return sectorsize;
}

