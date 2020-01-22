/*
 * state_machine.c
 *
 *  Created on: 5 gru 2019
 *      Author: Boro
 */

#include "state_machine.h"

StateMachine stateMachine;

/**
 * Initialize state machine and its flags
 */
void STATEMACHINE_Init() {
	stateMachine.actualState = INIT;
	stateMachine.prevState = INIT;
	stateMachine.armed = TRUE;
	stateMachine.intruder = FALSE;
}
