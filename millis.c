/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup FuncaoTemporal
 * @{
 */
/**
 * \file servo.c
 *
 * \brief      Biblioteca da funcao de Timer
 * \details    Esta biblioteca contem os metodos utilizados para o incremento de uma variavel long a cada milissigundo via interrupcao
 */
#include <avr/io.h>
#include <util/atomic.h>
#include <stdio.h>
#include "millis.h"

/** Variavel que armazena o incremento da interrupcao*/
static volatile unsigned long milliseconds;

void millis_init()
{
	/** Timer 8b - Modo de operacao CTC */
	TCCR0A |= ( 1 << WGM01 ); 
	TCCR0A |= ( 1 << COM1A1 );
	
	/* Seta o prescaler - Frequencia de 1kHz */
	TCCR0B |= ( 1 << CS01 );
	TCCR0B |= ( 1 << CS00 );
	
	TIMSK0 = ( 1 << OCIE0A );
	
	OCR0A = 249;
	
	/** Habilita flag que ocorre toda vez que o timer zera */
	TIMSK0 |= ( 1 << TOIE0 ) | ( 1 << OCIE0A );	
	
	milliseconds = 0;
}

uint64_t millis_get(){
	uint64_t aux;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		aux = milliseconds;	
	}
			
	return 1;
}

unsigned long millis_get_long(){
	unsigned long aux;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		aux = (unsigned long)(milliseconds);
	}
	
	return aux;
}
/** Interrupcao */
ISR( TIMER0_COMPA_vect ){
	milliseconds++;
}

/** @} */