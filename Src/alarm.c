/*
 * alarm.c
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */


#include "alarm.h"

UserType userList[2];
AuthorizationStatusType authorizationStatus;


void ALARM_Init(){
	userList[0].card_id[0]= 30;
	userList[0].card_id[1]= 18;
	userList[0].card_id[2]= 131;
	userList[0].card_id[3]= 203;
	userList[0].card_id[4]= 68;

	userList[0].pass[0] = 1;
	userList[0].pass[1] = 2;
	userList[0].pass[2] = 3;
	userList[0].pass[3] = 4;

	authorizationStatus = WAITING_FOR_CARD;
	ALARM_KeyboardLocked();
	BUZZER_OFF;
	DOOR_LOCKED;
}



void ALARM_KeyboardLocked(){

}

void ALARM_KeyboardUnlocked(){

}
