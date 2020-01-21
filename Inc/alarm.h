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
#define ID_LENGTH					5

#define USERS_LIMIT					10
#define FLASH_DATA_SIZE				USERS_LIMIT*(PASSWORD_LENGTH+ID_LENGTH)

#define DOOR_UNLOCKED				HAL_GPIO_WritePin(DOOR_ENABLE_GPIO_Port, DOOR_ENABLE_Pin, GPIO_PIN_RESET);
#define DOOR_LOCKED					HAL_GPIO_WritePin(DOOR_ENABLE_GPIO_Port, DOOR_ENABLE_Pin, GPIO_PIN_SET);
#define BUZZER_ON					HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_SET);
#define BUZZER_OFF					HAL_GPIO_WritePin(BUZZER_ENABLE_GPIO_Port, BUZZER_ENABLE_Pin, GPIO_PIN_RESET);
#define KEYPAD_LOCK					HAL_NVIC_DisableIRQ(EXTI9_5_IRQn)
#define KEYPAD_UNLOCK				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn)

#define SUCCESS						1
#define FAILURE						0

#define NO_USER						255

//TODO: Card status
typedef struct{
	uint8_t card_id[ID_LENGTH];
	char pass[PASSWORD_LENGTH];
	BoolType isActive;
}UserType;

typedef enum{
	WAITING_FOR_CARD = 0,
	WAITING_FOR_PIN,
	PIN_TYPED,
	ACCESS_GRANTED,
	ACCESS_DENIED
} AuthorizationStatusType;

typedef enum{
	UNDETECTED,
	DETECTED
} IntruderStatusType;

typedef uint8_t OperationResult;

extern void ALARM_Init();
extern void ALARM_CheckIfIntruderDetected();
extern void ALARM_CheckIfCardDetected();
extern void ALARM_CheckIfPinCorrect();
extern void ALARM_CheckIfAdminOperation();

extern AuthorizationStatusType authorizationStatus;
extern IntruderStatusType intruderStatus;
extern UserType userList[USERS_LIMIT];


#endif /* ALARM_H_ */
