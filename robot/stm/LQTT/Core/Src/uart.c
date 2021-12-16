/*
* uart.c
*
*  Created on: Dec 14, 2021
*      Author: quentin
*/

#include "uart.h"
#include "string.h"

// STM32 components
UART_HandleTypeDef* m_uart;

// Buffer
#define m_separator '\n'
#define m_buffer_size 256
char m_buffer[m_buffer_size] = {0};
uint16_t m_buffer_i = 0;

void uart_init(UART_HandleTypeDef* uart) {
	m_uart = uart;
}

void uart_write(char* input) {
	uint16_t size = strlen(input);
	uint8_t status =  HAL_UART_Transmit(m_uart, input, size, 10);

}

/**
 * @brief Lit les octets entrant de l'UART, retourne une chaine de caractères contenant un message s'il y en a un
 *
 * @param output Chaine de caractère dans laquelle est inséré le topic et le message
 * @return uint8_t 0 si rien n'a été lu
 */
uint16_t uart_read(char* output) {

	// Lit les données en entré de la liaison série
	char pre_buffer[m_buffer_size] = {0};
	HAL_UART_Receive(m_uart, (uint8_t*)pre_buffer, m_buffer_size -m_buffer_i, 30);
	uint16_t i = strlen(pre_buffer);
	if(!i) {
		return 0;
	}
	strncpy(m_buffer +m_buffer_i, pre_buffer, i);


	m_buffer_i += i;
	m_buffer[m_buffer_i] = 0;

	// Recherche un message complet
	i = 0;
	uint8_t c = '\0';
	uint8_t c_prev = '\0';
	while(  (c != m_separator || c != c_prev) && i < m_buffer_i  ) {
		c_prev = c;
		c = m_buffer[i];
		i++;
	}

	// Extrait le message s'il est complet
	if(c == c_prev && c!= '\0') {

		// Copie le message dans le buffer de sortie
		strncpy(output, m_buffer, i);
		output[i] = 0;

		// Enlève le message trouvé du buffer
		uint16_t j = 0, k = i;
		while(i < m_buffer_size) {
			m_buffer[j] = m_buffer[i];
			j++; i++;
		}
		m_buffer_i -= k;

		return k;
	}

	return 0;
}
