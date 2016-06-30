/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup Conversor ADC
 * @{
 * 
 */
/**
 * \file adc.c
 *
 * \brief      Biblioteca ADC
 * \details    Biblioteca de funcoes do conversor ADC
 */

#include "adc.h"
#include <stdio.h>

void adc_init()
{
	/** Usa canal em 0v */
	ADMUX = 0x0F;
	
	/** Tensao de referencia = AVcc */
	ADMUX |= ( 1 << REFS0 );
	
	/** Resolucao de 10 bits com alinhamento a direita */
	ADMUX &= ~( 1 << ADLAR );
	
	/** Prescalar = 32 e habilita o conversor */
	ADCSRA = ( 1 << ADPS2 ) | ( 1 << ADPS1 );
	
}

uint16_t read_adc( uint8_t porta )
{
	uint16_t conversao;
	
	/** Define a porta */
	ADMUX |= ( 0x0F & porta );
	
	/** Habilita o ADC */
	ADCSRA |= ( 1 << ADEN );
	
	/** Inicia conversor */
	ADCSRA |= ( 1 << ADSC );
	
	/** Espera o termino da conversao */
	while( ADCSRA & ( 1 << ADSC ) );

	/** Retorna para o canal OV */
	conversao = ADCL;
	conversao = ( ADCH << 8 ) + conversao;	
	ADMUX &= ~0x0F;
	
	return conversao;
	
}

uint16_t read_pot( void )
{
	uint32_t potenciomentro;
	uint32_t temp;
	/** Porta 0 */
	potenciomentro = read_adc( 0 );
	/** Simula a temperatura do sensor*/
	temp = ( potenciomentro*( (double)150/1023 )  );
	
	return (uint16_t)temp;	
}
/** @} */