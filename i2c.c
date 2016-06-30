/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup Sensor
 * @{
 */
/**
 * \defgroup I2C Comunicacao I2C
 * @{
 *
 */
/**
 * \file i2c.c
 *
 * \brief      Biblioteca de funcoes da comunicao I2C
 * \details    Biblioteca de funcoes do protocolo necesarrio para a troca de dados entre o sensor e a central
 */
#include <avr/io.h>
#include <util/twi.h>
#include <stdio.h>

#include "i2c.h"

/** Arduino Uno Crystal Oscillator Frequency = 16MHz */
#define FOSC		16000000L
/** I2C bitrate = 100KHz */
#define BITRATE		100000L
/** Preescala*/
#define PRESCALER	1

void i2c_init(void)
{
	DDRC |= (1<<4) | (1<<5);
	TWBR = (uint8_t)(((FOSC/BITRATE)-16)/(2*PRESCALER));
}

uint8_t i2c_read_ack(void)
{
	/** Recebe o dado com reconhecimento de chegada */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	/** Espera pelo fim da trasmissao */
	while( !(TWCR & (1<<TWINT)) );

	/** Retorna dado recebido por TWDR */
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	/** Recebe o dado sem reconhecimento de chegada */
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	/** Espera pelo fim da trasmissao */
	while( !(TWCR & (1<<TWINT)) );
	
	/** Retorna dado recebido por TWDR */
	return TWDR;
}

uint8_t i2c_receive(uint8_t *bufferIn, uint16_t length)
{
	uint16_t i = 0;
	while( i < (length-1) )
	{
		/** Recebe byte a byte com reconhecimento de chegada, parte baixa e alta */
		bufferIn[i++] = i2c_read_ack();
	}
	/** Recebe o ultimo byte de CRC */
	bufferIn[length-1] = i2c_read_nack();
	
	return 0;
}

uint8_t i2c_start(uint8_t slaveAddr)
{
	uint8_t twst;
		
	/** Habilita a comunicacao I2C e gera a condicao de inicio */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	/** Espera ate a flag ser setada */
	while( !(TWCR & (1<<TWINT)) );
	
	/** Verifica o registrador de status para a confirmacao da condicao de inicio*/
	twst = TW_STATUS & 0xF8;
	if( (twst != TW_START) && (twst != TW_REP_START) )
	{ 
		printf("erro caralho\n");
		return 1; 
	}
	
	/** coloca o endereco do escravo no registrador de dados */
	TWDR = slaveAddr;
	
	/** Comeca a transmissao e aguarda a resposta*/
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	
	/** Verifica se o servo reconheceu leitura( 0x40 ) ou escrita( 0x18 )	*/
	twst = TW_STATUS;
	if( ( twst != TW_MT_SLA_ACK ) && ( twst != TW_MR_SLA_ACK ) )
	{
		//printf("erro ack\n");
		return 1;
	}
	
	return 0;
}

uint8_t i2c_stop(void)
{
	/** Condicao de parada*/
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	return 0;
}

uint8_t i2c_transmit(uint8_t slaveAddr, uint8_t *bufferOut, uint16_t length)
{
	/** Inicia transmissao*/
	uint8_t startFlag = i2c_start( slaveAddr + I2C_WRITE );
	if( startFlag )
	{
		printf("erro\n");
		return 1;
	}
	/** envia os length bytes para o servo*/	
	uint16_t i = 0;
	while( i < length )
	{
		if(i2c_write(bufferOut[i]))
		{
			return 1;
		}
		i++;
	}	
	/** Finaliza transmissao*/
	i2c_stop();
	
	return 0;
}

uint8_t i2c_write(uint8_t data)
{
	/** bota o dado no registrador de dados */
	TWDR = data;
	
	/** inicia a transmissao do dado */
	TWCR = (1<<TWINT) | (1<<TWEN);

	/** Espera pelo fim da trasmissao */
	while( !(TWCR & (1<<TWINT)) );
	
	/** Verifica o ack da trasmissao */
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
	{ 
		return 1; 
	}
	
	return 0;
}

/** @} */
/** @} */