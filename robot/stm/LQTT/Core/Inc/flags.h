/*
 * flags.h
 *
 *  Created on: Dec 15, 2021
 *      Author: quentin
 */

#ifndef INC_FLAGS_H_
#define INC_FLAGS_H_

union {
	uint16_t value;
	struct {
		unsigned fOrderLeft :1;
		unsigned fOrderRight :1;
		unsigned fOrderForward :1;

		unsigned vSensorLeft :1;
		unsigned fSensorLeft :1;
		unsigned vSensorRight :1;
		unsigned fSensorRight :1;
		unsigned vSensorBall :1;
		unsigned fSensorBall :1;

		unsigned vObstable :1;
		unsigned fObstacle :1;
		unsigned fButton :1;

		unsigned :4;
	};
	struct {
		unsigned :3;
		unsigned sensorLeft :2;
		unsigned sensorRight :2;
		unsigned sensorBall :2;
		unsigned obstacle :2;
		unsigned :5;
	};
} flags;

#endif /* INC_FLAGS_H_ */
