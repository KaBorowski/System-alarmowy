/*
 * sonar.h
 *
 *  Created on: 11 gru 2019
 *      Author: Karol Borowski
 */

#ifndef SONAR_H_
#define SONAR_H_

#include "main.h"

#define SONAR_SIGNAL_DURATION			1//us
#define SONAR_MAX_ECHO_TIME				1176//us
#define SONAR_ECHO_TIMEOUT				1176 //us
#define SONAR_DELAY						1000

#define SONAR_SEND_SIGNAL				HAL_GPIO_WritePin(SONAR_TRIG_GPIO_Port, SONAR_TRIG_Pin, GPIO_PIN_SET)
#define SONAR_STOP_SENDING				HAL_GPIO_WritePin(SONAR_TRIG_GPIO_Port, SONAR_TRIG_Pin, GPIO_PIN_RESET)
//#define SONAR_ECHO_STATE

#define SONAR_INTRUDER_DETECTION		100
#define SONAR_DETECTION_VALUE			10

void SONAR_count();

#endif /* SONAR_H_ */
