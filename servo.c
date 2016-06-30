/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup Servo-motor
 * @{
 */
/**
 * \defgroup Servo Servo
 * @{
 *
 */
/**
 * \file servo.c
 *
 * \brief      Biblioteca de funcoes para o servo-motor
 * \details    Biblioteca com os metodos necessarios para a configuracao da rotacao do servo
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

#include "servo.h"

/** Valor maximo para controle */
#define TOPVALUE	4999
/** Pulso maximo */
#define PULSEMAX	600
/** Pulso minimo */
#define PULSEMIN	100
/** Angulo maximo */
#define	ANGLEMAX	180
/** Angulo minimo */
#define ANGLEMIN	0
/** Variavel de controle do angulo para a interrupcao*/
int newAngleFlag;
/** Novo angulo calculado para o servo */
int newAngle;

void servo_init()
{		
	/** Fast PWM 16bits - NON-INVERTING MODE - Top = ICR1 */
	TCCR1A |= ( 1 << WGM11 );
	TCCR1B |= ( 1 << WGM12 );
	TCCR1B |= ( 1 << WGM13 );
	
	/** Preescaler - clk/64 */
	TCCR1B |= ( 1 << CS10 );
	TCCR1B |= ( 1 << CS11 );
	
	// TOP = ( F_CPU / ( F_PWM * PRESC ) ) - 1
	ICR1 = TOPVALUE;
	
	/** Envia sinal para OC1A/PB1 - NON-INVERTING MODE */
	TCCR1A |= ( 1 << COM1A1 );	
	
	/** Seta PB1 como output - Pino 9 do arduino uno */
	DDRB |= ( 1 << PB1 );
	
	/** Habilita flag de overflow */
	TIMSK1 |= ( 1 << TOIE1 );
	
	/** Inicia com valor minimo - 0 degrees */
	OCR1A = PULSEMIN;	
	newAngle = ANGLEMIN;
	newAngleFlag = 0;
}

void set_servo( int32_t angle )
{
	double pulseDif;
	int32_t pulseNew;
	
	if( angle >= 0 && angle <= 180 )
	{
		/** Calculo do novo angulo*/
		pulseDif = PULSEMAX - PULSEMIN;
		pulseNew = ( (pulseDif*angle) / ANGLEMAX ) + PULSEMIN;
		
		/** Desabilita interrupcoes */
		cli();
		
		/** Seta novo angulo */
		newAngle = pulseNew;
		newAngleFlag = 1;
		
		/** Habilita interrupcoes */
		sei();
	}
}
/** Tratameto da interrupcao */
ISR( TIMER1_OVF_vect )
{
	if( newAngleFlag )
	{
		/** Coloca o novo angulo para o servo*/
		OCR1A = newAngle;
		newAngleFlag = 0;
	}
}


/** @} */
/** @} */