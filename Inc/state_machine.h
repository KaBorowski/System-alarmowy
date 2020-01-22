/*
 * state_machine.h
 *
 *  Created on: 5 gru 2019
 *      Author: Karol Borowski
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "main.h"

typedef enum {
	INIT = 0, WAIT_FOR_CARD, WAIT_FOR_PIN, ADMIN_OPERATION
} StateMachineStatesType;

typedef struct {

	StateMachineStatesType actualState;
	StateMachineStatesType prevState;
	BoolType intruder;
	BoolType armed;

} StateMachine;

extern void STATEMACHINE_Init();

extern StateMachine stateMachine;

#endif /* STATE_MACHINE_H_ */
