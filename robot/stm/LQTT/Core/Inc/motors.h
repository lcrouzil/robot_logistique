/*
 * motors.h
 *
 *  Created on: Dec 15, 2021
 *      Author: Tristan
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"

TIM_HandleTypeDef* motors_htimG;
TIM_HandleTypeDef* motors_htimD;

void motors_init(TIM_HandleTypeDef* htimG,TIM_HandleTypeDef* htimD)
{

	motors_htimG = htimG;
	motors_htimD = htimD;

	HAL_TIM_PWM_Init(motors_htimG);
	HAL_TIM_PWM_Init(motors_htimD);

	//HAL_TIM_PWM_Start(motors_htimG, TIM_CHANNEL_3);
	//HAL_TIM_PWM_Start(motors_htimD, TIM_CHANNEL_1);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
}

void motors_goforward()
{

	 HAL_TIM_PWM_Start(motors_htimG, TIM_CHANNEL_3);

	 HAL_TIM_PWM_Start(motors_htimD, TIM_CHANNEL_1);
}

void motors_goleft()
{
	HAL_TIM_PWM_Stop(motors_htimG, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(motors_htimD, TIM_CHANNEL_1);
}

void motors_goright()
{
	HAL_TIM_PWM_Start(motors_htimG, TIM_CHANNEL_3);

	HAL_TIM_PWM_Stop(motors_htimD, TIM_CHANNEL_1);

}

void motors_stop()
{
	 HAL_TIM_PWM_Stop(motors_htimG, TIM_CHANNEL_3);

	 HAL_TIM_PWM_Stop(motors_htimD, TIM_CHANNEL_1);
}

#endif /* INC_MOTORS_H_ */
