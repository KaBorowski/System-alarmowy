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
#include "admin.h"


#define USERS_INIT

static void usersInit();
static void clearUser(uint8_t index);
static uint8_t getUserIndex(uint8_t *id);
static OperationResult addUser(uint8_t *id, char *pass);
static OperationResult removeUser(uint8_t *id);
static OperationResult editUser(uint8_t *id, char *pass);
static OperationResult unblockUser(uint8_t *id);

UserType userList[USERS_LIMIT];
AuthorizationStatusType authorizationStatus;
IntruderStatusType intruderStatus;
uint8_t usersAmount = 2;

static uint8_t card_status;
static uint8_t card_id[MFRC522_MAX_LEN];
static uint8_t deny_counter = 0;
static char new_user_password[PASSWORD_LENGTH];

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
	userList[0].isActive = TRUE;

	userList[1].card_id[0]= 224;
	userList[1].card_id[1]= 243;
	userList[1].card_id[2]= 62;
	userList[1].card_id[3]= 82;
	userList[1].card_id[4]= 127;
	userList[1].pass[0] = '2';
	userList[1].pass[1] = '1';
	userList[1].pass[2] = '3';
	userList[1].pass[3] = '7';
	userList[1].isActive = TRUE;


//	FLASH_WriteUsers(userList);

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
			if (stateMachine.prevState == ADMIN_OPERATION){
				stateMachine.prevState = stateMachine.actualState;
				stateMachine.actualState = ADMIN_OPERATION;
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "Card detected");
				LCD_Print_X_Y(1, 0, "User Added");
				addUser(card_id, new_user_password);
				HAL_Delay(3000);
			}
			else {
				stateMachine.prevState = stateMachine.actualState;
				stateMachine.actualState = WAIT_FOR_PIN;
				authorizationStatus = WAIT_FOR_CARD;
				KEYPAD_UNLOCK;
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "Card detected");
				LCD_Print_X_Y(1, 0, "Password: ");
			}
			break;
		case MI_ERR:
			break;
		default:
			break;
	}
}

void ALARM_CheckIfPinCorrect(){
	if (authorizationStatus == PIN_TYPED){
		uint8_t userIndex = 0;
		authorizationStatus = ACCESS_DENIED;
		userIndex = getUserIndex(card_id);
		if (userIndex != NO_USER){
//			if (strcmp(userList[i].pass, pin) == 0)	authorizationStatus = ACCESS_GRANTED;
			authorizationStatus = ACCESS_GRANTED;
			if (userList[userIndex].isActive == FALSE) authorizationStatus = ACCESS_DENIED;
			for (uint8_t j=0; j<PASSWORD_LENGTH; ++j){
				if(userList[userIndex].pass[j] != pin[j]) authorizationStatus = ACCESS_DENIED;

			}
		}
		else authorizationStatus = ACCESS_DENIED;

		if (authorizationStatus == ACCESS_GRANTED){//access granted
			LCD_Clear();
			LCD_Print_X_Y(0, 0, "ACCESS_GRANTED");
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

		}else{
			deny_counter++;
			if (deny_counter >= DENY_MAX_AMOUNT){
				//TODO: Block this card
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "ACCESS_DENIED");
				LCD_Print_X_Y(1, 0, "USER BLOCKED");
				userList[userIndex].isActive = FALSE;
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
				LCD_Print_X_Y(1, 0, "Password: ");
			}
		}
		KEYPAD_ResetCounter();
	}
}

void ALARM_CheckIfAdminOperation(){
	uint8_t command = ADMIN_CheckIfReceivedMsg();
	uint8_t data[ADMIN_MAX_DATA_SIZE];

	if (command == ADMIN_AUTHENTICATION){
		if (strcmp((char *)admin_frame.data, (char *)adminPassword)){
			if (stateMachine.actualState != ADMIN_OPERATION){
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "Admin Panel");
				ADMIN_SendFrame(ADMIN_OPERATION_SUCCESS, NULL, 0);
				stateMachine.prevState = stateMachine.actualState;
				stateMachine.actualState = ADMIN_OPERATION;
				for (uint8_t i=0; i<usersAmount; ++i){
					HAL_Delay(50);
					memcpy(data, userList[i].card_id, ID_LENGTH*sizeof(uint8_t));
					memcpy(data + ID_LENGTH, &userList[i].isActive, 1);
					ADMIN_SendFrame(ADMIN_SEND_USER, data, ID_LENGTH + 1);
				}
			}else{
				LCD_Clear();
				stateMachine.prevState = WAIT_FOR_CARD;
				stateMachine.actualState = WAIT_FOR_CARD;
			}

		}else{
			ADMIN_SendFrame(ADMIN_OPERATION_FAIL, NULL, 0);
		}
	}

	if (stateMachine.actualState == ADMIN_OPERATION){
		switch(command){
			case ADMIN_ADD_USER:{
				stateMachine.prevState = stateMachine.actualState;
				stateMachine.actualState = WAIT_FOR_CARD;
				LCD_Clear();
				LCD_Print_X_Y(0, 0, "SCAN NEW CARD");
				LCD_Print_X_Y(1, 0, "TO ADD USER");
				strncpy(new_user_password, (char *)admin_frame.data, ID_LENGTH);
				break;
			}
			case ADMIN_DELETE_USER:{
				removeUser(admin_frame.data+ADMIN_ID_INDEX);
				break;
			}
			case ADMIN_UNLOCK_USER:{
				unblockUser(admin_frame.data+ADMIN_ID_INDEX);
				break;
			}
			case ADMIN_EDIT_USER:{
				editUser(admin_frame.data+ADMIN_ID_INDEX, (char *)(admin_frame.data+ADMIN_PASS_INDEX));
				break;
			}
			default: break;
		}
	}

}

static void clearUser(uint8_t index){
	memset(userList[index].card_id, 0, ID_LENGTH);
	memset(userList[index].pass, 0, PASSWORD_LENGTH);
	userList[index].isActive = FALSE;
}

static void usersInit(){
	for (uint8_t i=0; i<USERS_LIMIT; ++i){
		memset(userList[i].card_id, 0, ID_LENGTH);
		memset(userList[i].pass, 0, PASSWORD_LENGTH);
		userList[i].isActive = FALSE;
	}
}

static uint8_t getUserIndex(uint8_t *id){
	uint8_t userIndex;
	uint8_t user = 0;
	for (userIndex=0; userIndex<USERS_LIMIT; ++userIndex){
		user = MFRC522_Compare(id, userList[userIndex].card_id);
		if (user == MI_OK) return userIndex;
	}
	return NO_USER;
}

static OperationResult addUser(uint8_t *id, char *pass){
	if (usersAmount >= USERS_LIMIT) return FAILURE;
	memcpy(userList[usersAmount].card_id, id, ID_LENGTH);
	strncpy(userList[usersAmount].pass, pass, PASSWORD_LENGTH);
	userList[usersAmount].isActive = TRUE;
	usersAmount++;
	return SUCCESS;
}

static OperationResult removeUser(uint8_t *id){
	uint8_t index=0;
	if (usersAmount <= 0) return FAILURE;
	index = getUserIndex(id);
	if (index == NO_USER) return FAILURE;
	clearUser(index);
	for (uint8_t i=index; i<usersAmount-1; ++i){
		memcpy(userList[i].card_id, userList[i+1].card_id, ID_LENGTH);
		strncpy(userList[i].pass, userList[i+1].pass, PASSWORD_LENGTH);
		userList[i].isActive = userList[i+1].isActive;
	}
	clearUser(--usersAmount);

	return SUCCESS;
}

static OperationResult editUser(uint8_t *id, char *pass){
	uint8_t index=0;
	index = getUserIndex(id);
	if (index == NO_USER) return FAILURE;
	strncpy(userList[index].pass, pass, PASSWORD_LENGTH);
	return SUCCESS;
}

static OperationResult unblockUser(uint8_t *id){
	uint8_t index=0;
	index = getUserIndex(id);
	if (index == NO_USER) return FAILURE;
	userList[index].isActive = TRUE;
	return SUCCESS;
}








