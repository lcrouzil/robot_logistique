/*
 * sensor.h
 *
 *  Created on: 16 déc. 2021
 *      Author: Tristan
 */


#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "main.h"
#include "flags.h"
TIM_HandleTypeDef* ultrason_htim_trigg;
TIM_HandleTypeDef* ultrason_htim_echo;

uint8_t flag_timer_sonar = 0;
uint8_t tmp1 = 1;
uint16_t value_echo = 0;
double value_distance = 0;

void sensors_init(TIM_HandleTypeDef* htim7,TIM_HandleTypeDef* htim6)
{
	ultrason_htim_trigg = htim7;
	ultrason_htim_echo = htim6;
}

void sensors_ultason()
{
	if(tmp1)
	{
		 HAL_TIM_Base_Start_IT(ultrason_htim_trigg);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
		 tmp1 = 0;
	}
	if (flag_timer_sonar)
	{
		//Calcul apres reception signal :
		value_distance = value_echo * 0.17;  //distance en mm
		flag_timer_sonar = 0;
		tmp1 = 1;
		// TODO levé flag obstacle
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	if(GPIO_Pin == GPIO_PIN_13) // Button
	{
		flags.fButton = 1;
	}

	if(GPIO_Pin == GPIO_PIN_12) // Capteur droit
	{
		flags.fSensorRight = 1;
		flags.vSensorRight = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
	}

	if(GPIO_Pin == GPIO_PIN_14) // Capteur gauche
	{
		flags.fSensorLeft = 1;
		flags.vSensorLeft = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	}

	if(GPIO_Pin == GPIO_PIN_15) // Capteur balle
	{
		flags.fSensorBall = 1;
		flags.vSensorBall = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
	}

	if(GPIO_Pin == GPIO_PIN_1) // Sonar echo
	{
		if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
		{
			HAL_TIM_Base_Start(ultrason_htim_echo);
			value_echo = __HAL_TIM_GET_COUNTER(ultrason_htim_echo);
		}
		else
		{
			value_echo = __HAL_TIM_GET_COUNTER(ultrason_htim_echo) -value_echo;
			HAL_TIM_Base_Stop(ultrason_htim_echo);
			flag_timer_sonar = 1 ;
		}
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_TIM_Base_Stop_IT(ultrason_htim_trigg);
	HAL_TIM_Base_Init(ultrason_htim_trigg);

}



#endif /* INC_SENSOR_H_ */
