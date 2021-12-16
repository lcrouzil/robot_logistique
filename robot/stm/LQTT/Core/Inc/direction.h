/*
 * directions.h
 *
 *  Created on: Dec 15, 2021
 *      Author: quentin
 */

#ifndef INC_DIRECTION_H_
#define INC_DIRECTION_H_

#include "main.h"

struct Direction_s {
	char dir;
	uint8_t id;
};
typedef struct Direction_s Direction_t;

/**
 * Demande si le tableau circulaire est vide
 *
 * @return true si vide
 */
uint8_t direction_empty();

/**
 * Demande si le tableau circulaire est plein
 *
 * @return true si plein
 */
uint8_t direction_full();

/**
 * Ajoute une direction au tableau circulaire des m_array
 *
 * @return true si la direction à été enregistrée, false si le tableau est complet
 */
uint8_t direction_add(Direction_t dir);

/**
 * Récupère une direction et la met dans la variable passée en paramètre
 *
 * @return true si une valeur à pu être lu, false si le tableau est vide
 */
uint8_t direction_get(Direction_t* dir);

#endif /* INC_DIRECTION_H_ */
