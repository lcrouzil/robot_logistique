/*
 * utils.h
 *
 *  Created on: Dec 15, 2021
 *      Author: quentin
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "main.h"
#include "string.h"
#include "direction.h"
#include "flags.h"
#include "uart.h"

/**
 * Renvoie la taille du tableau de caractère en sortie
 */
uint16_t utils_getMessage(char* in, char* out, uint16_t size) {

	uint16_t i = 0;
	while(in[i++] != ' ');
	uint16_t j = 0;
	while(i < size -2) { // -2 pour ignorer les deux \n à la fin
		out[j++] = in[i++];
	}
	out[j] = '\0';

	return j;
}

Direction_t utils_parseJson(char* json, uint16_t size) {

	uint16_t i = 1;
	Direction_t msg = {0};

	while(json[i] != '}') {
		char keyBuf[20] = {0};
		char valBuf[20] = {0};

		// Get key
		while(json[i++] != '"');
		uint8_t j = 0;
		while(json[i] != '"')
			keyBuf[j++] = json[i++];
		keyBuf[j] = '\0';

		i++;

		// Get value
		while(json[i++] != ':');
		uint8_t k = 0;
		while(json[i] != ',' && json[i] != '}')
			valBuf[k++] = json[i++];
		valBuf[k] = '\0';

		// Understand data
		if(!strcmp(keyBuf, "direction")) {
			msg.dir = valBuf[1];
		}
		if(!strcmp(keyBuf, "id")) {
			msg.id = 0;
			for(int l = 0; l < k; l++) {
				msg.id = msg.id*10 + (valBuf[l] -0x30);
			}
		}
	}

	return msg;
}

void utils_setDirectionsFlag(Direction_t dir) {

	switch(dir.dir) {
	case 'F':
		flags.fOrderForward = 1;
		break;
	case 'R':
		flags.fOrderRight = 1;
		break;
	case 'L':
		flags.fOrderLeft = 1;
		break;
	default:
		break;
	}

}

void utils_readAndSaveDir() {


	  char message_in[256] = {0};
	  uint16_t size = uart_read(message_in);

	  if(size) {

		  char message_message[256] = {0};
		  uint16_t size2 = utils_getMessage(message_in, message_message, size);
		  Direction_t dir = utils_parseJson(message_message, size2);
		  direction_add(dir);
	  }

}


#endif /* INC_UTILS_H_ */
