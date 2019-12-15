/*
 * alarm.c
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */


#include "alarm.h"
#include "string.h"
#include "flash.h"

//#define USERS_INIT

static void usersInit();

UserType userList[USERS_LIMIT];
AuthorizationStatusType authorizationStatus;
IntruderStatusType intruderStatus;


void ALARM_Init(){
	usersInit();
#ifdef USERS_INIT
	userList[0].card_id[0]= 30;
	userList[0].card_id[1]= 18;
	userList[0].card_id[2]= 131;
	userList[0].card_id[3]= 203;
	userList[0].card_id[4]= 68;
	userList[0].pass[0] = '1';
	userList[0].pass[1] = '2';
	userList[0].pass[2] = '3';
	userList[0].pass[3] = '4';

	userList[1].card_id[0]= 224;
	userList[1].card_id[1]= 243;
	userList[1].card_id[2]= 62;
	userList[1].card_id[3]= 82;
	userList[1].card_id[4]= 127;
	userList[1].pass[0] = '2';
	userList[1].pass[1] = '1';
	userList[1].pass[2] = '3';
	userList[1].pass[3] = '7';

	FLASH_WriteUsers(userList);

#else
	FLASH_ReadUsers(userList);
#endif

	authorizationStatus = WAITING_FOR_CARD;
	intruderStatus = UNDETECTED;
	KEYPAD_LOCK;
	BUZZER_OFF;
	DOOR_LOCKED;
}

static void usersInit(){
	for (uint8_t i=0; i<USERS_LIMIT; ++i){
		memset(userList[i].card_id, 0, ID_LENGTH);
		memset(userList[i].pass, 0, PASSWORD_LENGTH);
	}
}



