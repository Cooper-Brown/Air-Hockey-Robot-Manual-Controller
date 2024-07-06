/*
 * myMain.c
 *
 *  Created on: Apr 24, 2023
 *      Author: 00coo
 */
#include "myMain.h"

uint32_t xRotCount = 0;
uint32_t yRotCount = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Xz_Pin)
	{
		xRotCount++;
	}
	// y encoder
	else if (GPIO_Pin == Yz_Pin)
	{
		yRotCount++;
	}
	else if (GPIO_Pin == ESTOP_Pin)
	{
		x_stop();
		y_stop();
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

	}
}



// https://www.youtube.com/watch?v=yeVCuAdRnZU
void updateEncoderData(encoder_packet *out_packet, TIM_HandleTypeDef *htim)
{
	static uint8_t firstUpdate;
	uint32_t currentCounterValue = __HAL_TIM_GET_COUNTER(htim);

	if (!firstUpdate)
	{
		out_packet -> velocity = 0;
		out_packet->lastEncoderValue = 0;
		firstUpdate = 1;
	}
	else
	{
		// no movement
		if (currentCounterValue == out_packet->lastEncoderValue)
		{
			out_packet -> velocity = 0;
		}
		// up movement
		else if (currentCounterValue > out_packet->lastEncoderValue)
		{
			// up movement but overflown
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
			{
				out_packet -> velocity = -(out_packet->lastEncoderValue) - (__HAL_TIM_GET_AUTORELOAD(htim) - currentCounterValue);
			}
			// up movement, no overflow
			else
			{
				out_packet -> velocity = currentCounterValue - out_packet->lastEncoderValue;
			}
		}
		// down movement
		else
		{
			// down movement but overflown
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
			{
				out_packet -> velocity = currentCounterValue - out_packet->lastEncoderValue;
			}
			// up movement, no overflow
			else
			{
				out_packet -> velocity = currentCounterValue + (__HAL_TIM_GET_AUTORELOAD(htim) - out_packet->lastEncoderValue);
			}
		}
	}
	out_packet -> position += out_packet -> velocity;
	out_packet->lastEncoderValue = currentCounterValue;
}

TIM_HandleTypeDef* p_htim3;
TIM_HandleTypeDef* p_htim4;
TIM_HandleTypeDef* p_htim9;
DAC_HandleTypeDef* p_hdac;
encoder_packet xEncoderData;
encoder_packet yEncoderData;
void my_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM9)
	{
	    updateEncoderData(&xEncoderData, p_htim3);
	    updateEncoderData(&yEncoderData, p_htim4);
	    detectXLimits();
	}
}

void detectXLimits(){
	static uint8_t limitStopped = 1;
	uint8_t insideXLimits = 0;
	if ((xEncoderData.position < 500) || (xEncoderData.position > 12000)) {
		insideXLimits = 1;
	}
	else {
		insideXLimits = 0;
		limitStopped = 0;
	}
	if ((!limitStopped) && (insideXLimits)){
		x_stop();
		limitStopped = 1;
	}
}

uint32_t offSpeed = 0x00000000;
uint32_t onSpeed = 0x00000100;

void myInit(
		DAC_HandleTypeDef* p_hdac_arg,
		TIM_HandleTypeDef* p_htim3_arg,
		TIM_HandleTypeDef* p_htim4_arg,
		TIM_HandleTypeDef* p_htim9_arg)
{
	p_hdac = p_hdac_arg;
	p_htim3 = p_htim3_arg;
	p_htim4 = p_htim4_arg;
	p_htim9 = p_htim9_arg;
	HAL_TIM_Encoder_Start(p_htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(p_htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(p_htim9);
	HAL_DAC_Start(p_hdac, DAC_CHANNEL_1);
	// HAL_DAC_Start(p_hdac, DAC_CHANNEL_2);
	HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0x00000000); // set off speeds
	// HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x000007FF);
	y_stop();
	x_stop();
	HAL_Delay(100);
}

void myLoopInternals()
{
	//x_stop();
	//HAL_Delay(2000);
	//x_goLeft();
	//HAL_Delay(2000);
	HAL_Delay(1);
}



void x_goLeft(){
	if (xEncoderData.position < 500) {return;}
	HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, onSpeed);
	HAL_GPIO_WritePin(GPIOA, Xdir_Pin, GPIO_PIN_RESET); // x direction
	HAL_GPIO_WritePin(GPIOA, Xen_Pin, GPIO_PIN_RESET); // x enable
	return;
}
void x_goRight(){
	if (xEncoderData.position > 12000) {return;}
	HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, onSpeed);
	HAL_GPIO_WritePin(GPIOA, Xdir_Pin, GPIO_PIN_SET); // x direction
	HAL_GPIO_WritePin(GPIOA, Xen_Pin, GPIO_PIN_RESET); // x enable
	return;
}
void x_stop(){
	HAL_GPIO_WritePin(GPIOA, Xen_Pin, GPIO_PIN_SET);
	return;
}
void y_goUp(){return;}
void y_goDown(){return;}
void y_stop(){
	HAL_GPIO_WritePin(GPIOA, Yen_Pin, GPIO_PIN_RESET);
	return;
}

const char* xLeftCommand = "x.l";
const char* xRightCommand = "x.r";
const char* xStopCommand = "x.s";
const char* yUpCommand = "y.u";
const char* yDownCommand = "y.d";
const char* yStopCommand = "y.s";

void commandReceived(char* charAccumulator, uint8_t commandSize){
	char commandInput[commandSize+1];
	int c;
	for (c = 0; c < commandSize; c++)
		commandInput[c] = charAccumulator[c];
	commandInput[c] = '\0';

	if (!strcmp(xLeftCommand, commandInput)){
		x_goLeft();
	}
	else if (!strcmp(xRightCommand, commandInput)){
		x_goRight();
	}
	else if (!strcmp(xStopCommand, commandInput)){
		x_stop();
	}
	else if (!strcmp(yUpCommand, commandInput)){
		y_goUp();
	}
	else if (!strcmp(yDownCommand, commandInput)){
		y_goDown();
	}
	else if (!strcmp(yStopCommand, commandInput)){
		y_stop();
	}
}

