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
/**
 * \file uart.c
 *
 * \brief      Biblioteca de funcoes para a comunicao serial
 * \details    Biblioteca com os metodos necessarios para a comunicacao entre a central e um dispositivo serial, como a USB para o MATLAB
 */

#include <avr/io.h>
#include <stdio.h>

#include "uart.h"

/** Arduino Uno Crystal Oscillator Frequency = 16MHz */
#define FOSC 16000000

/** Arquivo de saida */
static FILE mystout = FDEV_SETUP_STREAM( putchar_buf, getchar_buf, _FDEV_SETUP_RW);

int putchar_buf( int c, FILE *__stream)
{
	output[ind++%128] = c;
	usart_transmit( c );
	
	return c;
}

int getchar_buf( FILE *__stream)\
{
	char c;
	c = usart_receive();
	
	return c;
}

void usart_init( uint32_t ubrr )
{
	/** Formato do pacote usart
		Data: 8 bits
		Stop: 1 bit
		Parity: None
		Baud rate: parametro ubrr
	*/	
	uint32_t auxUBRR = (FOSC/(16*ubrr))-1;
	
	/** Set baud rate */
	UBRR0H = (unsigned char)(auxUBRR>>8);
	UBRR0L = (unsigned char)auxUBRR;
	
	/** Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	/** Pacote 8 bits */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	
		
	/** Sobrescreve a saida*/
	stdout = &mystout;
	/** Sobrescreve a entrada*/
	stdin = &mystout;
}

unsigned char usart_receive( void )
{
	/** Espera o dado ser recebido */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/** Pega e retorna o dado recebido do buffer */
	return UDR0;
}

void usart_transmit( uint8_t data )
{
	/** Aguarda por espaco no buffer */
	while( !( UCSR0A & (1<<UDRE0)) );
	
	/** Coloca o dado no buffer e envia o dado */
	UDR0 = data;
}

uint8_t usart_available()
{	
	if( (UCSR0A & (1<<RXC0)) )
	{
		return 1;	
	}
	else
	{
		return 0;
	}
}


/** @} */
