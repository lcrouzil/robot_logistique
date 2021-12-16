/*
 * direction.c
 *
 *  Created on: Dec 15, 2021
 *      Author: quentin
 */

#include "direction.h"

#define MAX 10

Direction_t m_array[MAX]; // Tableau circulaire de direction à prendre
uint8_t m_i = 0; // Emplacement de la première direction [0; max[
uint8_t m_n = 0; // Nombre de m_array actuellement enregistrées [0; max[

uint8_t direction_empty() {

	return m_n == 0;
}

uint8_t direction_full() {
	return m_n == MAX;
}

uint8_t direction_add(Direction_t dir) {

	if(!direction_full()) {
		m_array[(m_i +m_n)%MAX] = dir;
		m_n++;

		return 1;
	}

	return 0;
}

uint8_t direction_get(Direction_t* dir) {

	if(!direction_empty()) {
		Direction_t dirTmp = m_array[m_i];
		m_i++;
		m_n--;

		dir->dir = dirTmp.dir;
		dir->id = dirTmp.id;

		return 1;
	}

	return 0;
}
