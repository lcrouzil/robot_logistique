/*
 * motors.h
 *
 *  Created on: Dec 15, 2021
 *      Author: Tristan
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"

TIM_HandleTypeDef* motors_htim;

void motors_init(TIM_HandleTypeDef* htim)
{

	motors_htim = htim;

	HAL_TIM_PWM_Init(motors_htim);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
}

void motors_goforward()
{

	 HAL_TIM_PWM_Start(motors_htim, TIM_CHANNEL_3);

	 HAL_TIM_PWM_Start(motors_htim, TIM_CHANNEL_4);
}

void motors_goleft()
{
	HAL_TIM_PWM_Stop(motors_htim, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(motors_htim, TIM_CHANNEL_4);
}

void motors_goright()
{
	HAL_TIM_PWM_Start(motors_htim, TIM_CHANNEL_3);

	HAL_TIM_PWM_Stop(motors_htim, TIM_CHANNEL_4);

}

void motors_stop()
{
	 HAL_TIM_PWM_Stop(motors_htim, TIM_CHANNEL_3);

	 HAL_TIM_PWM_Stop(motors_htim, TIM_CHANNEL_4);
}

#endif /* INC_MOTORS_H_ */
