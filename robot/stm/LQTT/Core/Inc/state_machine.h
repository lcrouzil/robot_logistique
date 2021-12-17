/*
 * state_machine.h
 *
 *  Created on: Dec 15, 2021
 *      Author: quentin
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "main.h"
#include "flags.h"
#include "stdio.h"

#define S_INIT			0x01
#define S_MOVE_STOP		0x10
#define S_MOVE_FORWARD 	0x11
#define S_MOVE_LEFT 	0x12
#define S_MOVE_RIGHT	0x13
#define S_INTERSECTION	0x20
#define S_ORDER_FORWARD 0x30
#define S_ORDER_LEFT 	0x31
#define S_ORDER_RIGHT 	0x32

uint8_t state = 0;
uint8_t stateOld = 0;

Direction_t dir;

char buf[100] = {0};
uint8_t size;

void state_machine_run() {

	if(state != stateOld) {
		HAL_Delay(1);
	}

	switch (state) {

	/**
	 *
	 */
	case S_INIT: {

		if(state != stateOld) {
			// Code
			stateOld = state;
		}

		if(flags.fButton) { // flag levé
			flags.fButton = 0;
			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

	/**
	 * Stop le robot en cas d'obstacle
	 */
	case S_MOVE_STOP: {

		if(state != stateOld) {
			motors_stop();
			stateOld = state;
		}

		if(!flags.vObstacle) { // flag levé & pas d'obstacle
			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

	/**
	 * Déplace le robot tout droit
	 */
	case S_MOVE_FORWARD: {

		if(state != stateOld) {
			motors_moveForward();
			stateOld = state;
		}

		if(flags.vObstacle) {
			state = S_MOVE_STOP;
			break;
		}
		if(!flags.vSensorLeft) { // flag levé & piste noir
			state = S_MOVE_LEFT;
			break;
		}
		if(!flags.vSensorRight) { // flag levé & piste noir
			state = S_MOVE_RIGHT;
			break;
		}

		break;
	}

	/**
	 * Déplace le robot vers la droite
	 */
	case S_MOVE_RIGHT: {

		if(state != stateOld) {
			motors_moveRight();
			stateOld = state;
		}

		if(flags.vObstacle) {
			state = S_MOVE_STOP;
			break;
		}
		if(!flags.vSensorLeft) {  // flag levé & piste noir
			state = S_INTERSECTION;//S_INTERSECTION;
			break;
		}
		if(flags.vSensorRight) {  // flag levé & piste blanche
			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

	/**
	 * Déplace le robot vers la gauche
	 */
	case S_MOVE_LEFT: {

		if(state != stateOld) {
			motors_moveLeft();
			stateOld = state;
		}

		if(flags.vObstacle) {
			state = S_MOVE_STOP;
			break;
		}
		if(flags.vSensorLeft) {  // flag levé & piste blanche
			state = S_MOVE_FORWARD;
			break;
		}
		if(!flags.vSensorRight) {  // flag levé & piste noir
			state = S_INTERSECTION;//S_INTERSECTION;
			break;
		}

		break;
	}

	/**
	 * Le robot se situe sur une intersection
	 */
	case S_INTERSECTION: {

		if(state != stateOld) {
			motors_stop();
			stateOld = state;

			if(dir.dir != '\0') {
				size = sprintf(buf, "field/robot/ROBOT5/status {\"status\": %d}\n\n", dir.id);
				buf[size] = '\0';
				uart_write(buf);
			}
		}

		if(flags.fButton) {
			flags.fButton = 0;

			size = sprintf(buf, "field/robot/ROBOT5/button {\"package\": %s}\n\n", flags.vSensorBall ? "true" : "false");
			buf[size] = '\0';
			uart_write(buf);
		}

		if(!(flags.fOrderForward || flags.fOrderLeft || flags.fOrderRight)) {
			if(direction_get(&dir)) {
				utils_setDirectionsFlag(dir);
			}
		}
		else {

			if(flags.fOrderForward) {
				state = S_ORDER_FORWARD;
			}
			if(flags.fOrderLeft) {
				state = S_ORDER_LEFT;
			}
			if(flags.fOrderRight) {
				state = S_ORDER_RIGHT;
			}
			flags.fOrderForward = 0;
			flags.fOrderLeft = 0;
			flags.fOrderRight = 0;
		}



		break;
	}

	/**
	 *
	 */
	case S_ORDER_FORWARD: {

		if(state != stateOld) {
			motors_moveForward();
			stateOld = state;
		}

		if(flags.vSensorLeft && flags.vSensorRight) { // piste blanche & piste blanche

			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

	/**
	 *
	 */
	case S_ORDER_LEFT: {

		if(state != stateOld) {
			motors_moveLeft();
			stateOld = state;
		}

		if(flags.vSensorRight) { // piste blanche

			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

	/**
	 *
	 */
	case S_ORDER_RIGHT: {

		if(state != stateOld) {
			motors_moveRight();
			stateOld = state;
		}

		if(flags.vSensorLeft) { // piste blanche

			state = S_MOVE_FORWARD;
			break;
		}

		break;
	}

//	/**
//	 *
//	 */
//	case S_: {
//
//		if(state != stateOld) {
//			// Code
//			stateOld = state;
//		}
//
//		if() { // flag levé
//
//			state = S_;
//			break;
//		}
//
//		break;
//	}

	default: {

		state = S_INIT;

		break;
	}

	}

}


#endif /* INC_STATE_MACHINE_H_ */
