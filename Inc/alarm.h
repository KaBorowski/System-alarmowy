/*
 * alarm.h
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "main.h"
#include "rc552.h"

#define DENY_MAX_AMOUNT				5
#define PASSWORD_LENGTH				4

#define DOOR_UNLOCKED				HAL_GPIO_WritePin(DOOR_ENABLE_GPIO_Port, DOOR_ENABLE_Pin, GPIO_PIN_RESET);
#define DOOR_LOCKED					HAL_GPIO_WritePin(DOOR_ENABLE_GPIO_Port, DOOR_ENABLE_Pin, GPIO_PIN_SET);
#define BUZZER_ON					HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_SET);
#define BUZZER_OFF					HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_RESET);


typedef struct{
	uint8_t card_id[MFRC522_MAX_LEN];
	uint8_t pass[PASSWORD_LENGTH];
}UserType;

typedef enum{
	WAITING_FOR_CARD,
	WAITING_FOR_PIN,
	PIN_TYPED,
	ACCESS_GRANTED,
	ACCESS_DENIED
} AuthorizationStatusType;

typedef enum{
	UNDETECTED,
	DETECTED
} IntruderStatusType;

extern void ALARM_Init();
extern void ALARM_KeyboardLocked();
extern void ALARM_KeyboardUnlocked();

//extern
//extern UserType userList[2];


#endif /* ALARM_H_ */