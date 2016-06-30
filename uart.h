/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup ComunicaoSerial
 * @{
 */


#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdio.h>

/** Output do putchar*/
static char output[128];

/** Pontero do output do putchar*/
static char ind = 0;


/** 
* Inicializa o usart
* \param ubrr Baund Rate
*/
void usart_init( uint32_t ubrr );

/** 
* Recebe o dado
* \return Dado recebido pelo buffer
*/
unsigned char usart_receive( void );

/**
* Transmite o dado
* \param data Dado a ser enviado pelo buffer
*/
void usart_transmit( uint8_t data );

/**
* Verifica se chegou algum caracter no buffer da usart
* \return 0 - sucesso, 1 sem sucesso.
*/
uint8_t usart_available( void );

/**
* Metodo para sobrescrever a funcao printf
* \param c Char a ser enviado pelo buffer
* \param *__stream stream
* \return char
*/
int putchar_buf( int c, FILE *__stream );

/**
* Metodo para sobrescrever a funcao getchar
* \param *__stream stream
* \return char
*/
int getchar_buf( FILE *__stream);

#endif /* UART_H_ */

/** @} */
