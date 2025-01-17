/*
 * sonar.c
 *
 *  Created on: 11 gru 2019
 *      Author: Karol Borowski
 */

#include "sonar.h"
#include "alarm.h"

typedef enum {
	SENDING_IMPULS, WAIT_FOR_ECHO, COUNT_ECHO_TIME
} SonarStateType;

static void checkIfIntruder();
static void countDistance(uint32_t time);

static uint32_t counter;
static uint32_t delay;
static SonarStateType sonarState;
static uint8_t distance = 200;
static uint8_t impulsCounter = 0;

/**
 * Function to operate sonar
 *
 * Function set value on TRIGGER pin and read time when ECHO pin
 * is on HIGH state
 *
 */
void SONAR_count() {
	counter++;
	switch (sonarState) {
	case SENDING_IMPULS: {
		delay++;
		if (counter == SONAR_SIGNAL_DURATION
				&& HAL_GPIO_ReadPin(SONAR_TRIG_GPIO_Port, SONAR_TRIG_Pin)) {
			counter = 0;
			delay = 0;
			sonarState = WAIT_FOR_ECHO;
			SONAR_STOP_SENDING;
		}
		if (!HAL_GPIO_ReadPin(SONAR_TRIG_GPIO_Port, SONAR_TRIG_Pin) &&
		delay == SONAR_DELAY) {
			delay = 0;
			counter = 0;
			SONAR_SEND_SIGNAL;
		}
		break;
	}
	case WAIT_FOR_ECHO: {
		if (HAL_GPIO_ReadPin(SONAR_ECHO_GPIO_Port, SONAR_ECHO_Pin)) {
			counter = 0;
			sonarState = COUNT_ECHO_TIME;
		} else if (counter >= SONAR_ECHO_TIMEOUT) {
			counter = 0;
			distance = 200;
			sonarState = SENDING_IMPULS;
		}
		break;
	}
	case COUNT_ECHO_TIME: {
		if (!HAL_GPIO_ReadPin(SONAR_ECHO_GPIO_Port, SONAR_ECHO_Pin)) {
			countDistance(counter);
			checkIfIntruder();
			sonarState = SENDING_IMPULS;
			counter = 0;
		}
	}
	default:
		break;
	}
}

/**
 *
 */
static void checkIfIntruder() {
	if (distance <= SONAR_INTRUDER_DETECTION) {
		++impulsCounter;
		if (impulsCounter >= SONAR_DETECTION_VALUE) {
			impulsCounter = 0;
			intruderStatus = DETECTED;
		}
	} else {
		impulsCounter = 0;
		intruderStatus = UNDETECTED;
	}
}

/**
 * Count distance between sensor and object knowing impuls time
 *
 * @param time Time when echo was on HIGH state
 */
static void countDistance(uint32_t time) {
	distance = (time * 34) / 200;
}

