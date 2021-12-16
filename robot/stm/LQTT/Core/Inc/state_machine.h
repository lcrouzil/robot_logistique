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

#define S_MOVE_STOP		0x10
#define S_MOVE_FORWARD 	0x11
#define S_MOVE_LEFT 	0x12
#define S_MOVE_RIGHT	0x13
#define S_INTERSECTION	0x20

uint8_t state = 0;
uint8_t stateOld = 0;

void state_machine_run() {

	switch (state) {

	/**
	 * Stop le robot en cas d'obstacle
	 */
	case S_MOVE_STOP: {

		if(state != stateOld) {
			// TODO Moteur robot arrêtés
			stateOld = state;
		}

		if(flags.obstacle == 0b10) { // flag levé & pas d'obstacle
			flags.fObstacle = 0;
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
			// TODO Moteur robot en avant
			stateOld = state;
		}

		if(flags.sensorLeft == 0b10) { // flag levé & piste noir
			flags.fSensorLeft = 0;
			state = S_MOVE_LEFT;
			break;
		}
		if(flags.sensorRight == 0b10) { // flag levé & piste noir
			flags.fSensorRight = 0;
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
			// TODO Moteur robot vers la droite
			stateOld = state;
		}

		if(flags.sensorLeft == 0b10) {  // flag levé & piste noir
			flags.fSensorLeft = 0;
			state = S_INTERSECTION;
			break;
		}
		if(flags.sensorRight == 0b11) {  // flag levé & piste blanche
			flags.fSensorRight = 0;
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
			//TODO Moteur robot vers la gauche
			stateOld = state;
		}

		if(flags.sensorLeft == 0b11) {  // flag levé & piste blanche
			flags.fSensorLeft = 0;
			state = S_MOVE_FORWARD;
			break;
		}
		if(flags.sensorRight == 0b10) {  // flag levé & piste noir
			flags.fSensorRight = 0;
			state = S_INTERSECTION;
			break;
		}

		break;
	}

	/**
	 * Le robot se situe sur une intersection
	 */
	case S_INTERSECTION: {

		if(state != stateOld) {
			//TODO Moteur robot vers la gauche
			stateOld = state;
		}

		if(flags.fButton) { // flag levé
			state = S_MOVE_FORWARD;
		}

		break;

	}

	default: {

		state = S_MOVE_FORWARD;

		break;
	}

	}

}


#endif /* INC_STATE_MACHINE_H_ */
