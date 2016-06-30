/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \file adc.c
 *
 * \brief      Biblioteca ADC
 * \details    Biblioteca de funcoes do conversor ADC
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>
/** Inicia o conversor ADC*/
void adc_init( void );
/** Mensura o sinal analogico na porta*/
uint16_t read_adc( uint8_t porta );
/** Le do potenciomentro*/
uint16_t read_pot( void );


#endif /* ADC_H_ */
/** @} */