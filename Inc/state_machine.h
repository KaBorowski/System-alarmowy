/*
 * state_machine.h
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_


#include "main.h"


typedef enum{
	INIT = 0,
	WAIT_FOR_CARD,
	WAIT_FOR_PIN
}StateMachineStatesType;

typedef enum{
	FALSE,
	TRUE
}FlagState;

typedef struct{

	StateMachineStatesType actualState;
	StateMachineStatesType prevState;
	FlagState intruder;
	FlagState armed;

}StateMachine;

extern void STATEMACHINE_Init();

extern StateMachine stateMachine;


#endif /* STATE_MACHINE_H_ */
