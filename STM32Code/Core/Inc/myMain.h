/*
 * myMain.h
 *
 *  Created on: Apr 24, 2023
 *      Author: 00coo
 */

#ifndef SRC_MYMAIN_H_
#define SRC_MYMAIN_H_
#endif /* SRC_MYMAIN_H_ */

#include "gpio.h"

// This should be somewhere else
typedef struct
{
	int32_t velocity;
	int32_t position;
	uint32_t lastEncoderValue;
}encoder_packet;

void myInit(
	DAC_HandleTypeDef* p_hdac,
	TIM_HandleTypeDef* p_htim3,
	TIM_HandleTypeDef* p_htim4,
	TIM_HandleTypeDef* p_htim9_arg
);

void myLoopInternals();

void my_HAL_TIM_PeriodElapsedCallback();
