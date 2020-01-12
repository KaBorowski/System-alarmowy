/*
 * alarm.c
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */


#include "alarm.h"
#include "string.h"
#include "flash.h"
#include "state_machine.h"
#include "sonar.h"
#include "keypad.h"
#include "lcd.h"


#define USERS_INIT

static void usersInit();

UserType userList[USERS_LIMIT];
AuthorizationStatusType authorizationStatus;
IntruderStatusType intruderStatus;

static uint8_t card_status;
static uint8_t card_id[MFRC522_MAX_LEN];
static uint8_t deny_counter = 0;

extern StateMachine stateMachine;


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
	stateMachine.actualState = WAIT_FOR_CARD;
	KEYPAD_LOCK;
	BUZZER_OFF;
	DOOR_LOCKED;
}

void ALARM_CheckIfIntruderDetected(){
	if (!stateMachine.intruder && intruderStatus == DETECTED && stateMachine.armed){
		stateMachine.intruder = TRUE;
		BUZZER_ON;
		LCD_Clear();
		LCD_Print_X_Y(0, 0, "ALARM ACTIVATED!");
		LCD_Print_X_Y(1, 0, "INTRUDER!");
	}
}

void ALARM_CheckIfCardDetected(){
	card_status = MFRC522_Check(card_id);
	switch (card_status) {
		case MI_OK:	//CARD DETECTED
			stateMachine.prevState = stateMachine.actualState;
			stateMachine.actualState = WAIT_FOR_PIN;
			authorizationStatus = WAITING_FOR_PIN;
			KEYPAD_UNLOCK;
			LCD_Clear();
			LCD_Print_X_Y(0, 0, "Card detected");
			LCD_Print_X_Y(1, 0, "Password: ");
			break;
		case MI_ERR: //CARD UNDETECTED
//					LCD_Clear();
//					LCD_Print_X_Y(0, 0, "Error!");
//					LCD_Print_X_Y(1, 0, "Card Undetected");
		default:
			break;
	}
}

void ALARM_CheckIfPinCorrect(){
	if (authorizationStatus == PIN_TYPED){
		uint8_t user = 0;
		authorizationStatus = ACCESS_DENIED;
		for (int i=0; i<2; ++i){
			user = MFRC522_Compare(card_id, userList[i].card_id);
			if (user == MI_OK){
//						if (strcmp(userList[i].pass, pin) == 0)	authorizationStatus = ACCESS_GRANTED;
				authorizationStatus = ACCESS_GRANTED;
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "ACCESS_GRANTED");
				for (uint8_t j=0; j<PASSWORD_LENGTH; ++j){
					if(userList[i].pass[j] != pin[j]) authorizationStatus = ACCESS_DENIED;

				}

			}
		}
		if (authorizationStatus == ACCESS_GRANTED){//access granted
			if(stateMachine.armed && !stateMachine.intruder){
				stateMachine.armed = FALSE;
				DOOR_UNLOCKED;
			}
			else if(stateMachine.armed && stateMachine.intruder){
				stateMachine.intruder = FALSE;
				BUZZER_OFF;
			}
			else if(!stateMachine.armed){
				stateMachine.armed = TRUE;
				DOOR_LOCKED;
			}
			KEYPAD_LOCK;
			stateMachine.prevState = stateMachine.actualState;
			stateMachine.actualState = WAIT_FOR_CARD;
			authorizationStatus = WAITING_FOR_CARD;

		}else{//TODO: wrong authorization
			deny_counter++;
			if (deny_counter >= DENY_MAX_AMOUNT){
				//TODO: Block this card
				deny_counter = 0;
				KEYPAD_LOCK;
				stateMachine.prevState = stateMachine.actualState;
				stateMachine.actualState = WAIT_FOR_CARD;
				authorizationStatus = WAITING_FOR_CARD;
			}
			else{
				authorizationStatus = WAITING_FOR_PIN;
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "ACCESS_DENIED");
				LCD_Print_X_Y(1, 0, "ENTER PIN AGAIN");
			}
		}
		KEYPAD_ResetCounter();
	}
}

static void usersInit(){
	for (uint8_t i=0; i<USERS_LIMIT; ++i){
		memset(userList[i].card_id, 0, ID_LENGTH);
		memset(userList[i].pass, 0, PASSWORD_LENGTH);
	}
}



