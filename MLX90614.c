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
 * \defgroup sens MLX90614
 * @{
 *
 */
/**
 * \file MLX90614.c
 *
 * \brief      Biblioteca de funcoes do sensor
 * \details    Biblioteca de funcoes para a comunicacao entre o sensor e a central
 */

#include "MLX90614.h"
#include <stdio.h>

#include "i2c.h"

void MLX90614_init()
{
	/** Inicializa a comunicao I2C */
	i2c_init();
}

uint8_t get_chip_temp( double *varTemp)
{
	uint8_t erro;
	/** Le do sensor e grava em varTemp */
	erro = read_sensor( TCHIP_ADDR, varTemp );
	if( erro )
	{		
		//printf("Reading Error\n");
		return 1;
	}	
	/** Converte a temperatura para Celsius */
	*varTemp *= 0.02;
	*varTemp -= 273.15;
	
	return 0;
}

uint8_t get_object1_temp( double *varTemp )
{
	uint8_t erro;
	/** Le do sensor e grava em varTemp */
	erro = read_sensor( TOBJECT1_ADDR, varTemp );
	if( erro )
	{
		//printf("Reading Error\n");
		return 1;
	}
	
	/** Converte a temperatura para Celsius */
	*varTemp *= 0.02;
	*varTemp -= 273.15;
	
	return 0;
}

uint8_t get_object2_temp( double *varTemp )
{
	uint8_t erro;
	/** Le do sensor e grava em varTemp */
	erro = read_sensor( TOBJECT2_ADDR, varTemp );
	if( erro )
	{
		//printf("Reading Error\n");
		return 1;
	}
	
	/** Converte a temperatura para Celsius */
	*varTemp *= 0.02;
	*varTemp -= 273.15;
	
	return 0;
}

uint16_t read_sensor( uint8_t address, double *varTemp )
{
	int8_t erro;
	uint16_t temp;
	uint8_t bufferIn[3];
		
	/** Verifica se a condicao start foi concluida */
	erro = i2c_start( (MLX90614_ADDR<<1)|I2C_WRITE );	
	if(erro)
	{
		//printf("Start Error\n");
		return 1;
	}	
	/** Envia o endereco do registrador */
	erro = i2c_write( address );
	if(erro)
	{
		//printf("Write Error\n");
		return 1;	
	}	
	/** Verifica se a condicao start foi concluida novamente */
	erro = i2c_start( (MLX90614_ADDR<<1)|I2C_READ );
	if(erro)
	{
		//printf("Repeated Start Error\n");
		return 1;
	}	
	/** Recebe 2 bytes de temperatura lido e 1 byte de crc */
	erro = i2c_receive( bufferIn, 3 );
	if(erro)
	{
		//printf("Receive Error\n");
		return 1;
	}	
	/** Condicao de parada */
	i2c_stop();
	/** Salva a temperatura na variavel indicada pelo pontero */
	uint16_t temp16 = bufferIn[0] | (((uint16_t)bufferIn[1]) << 8 );		
	*varTemp = (double)(temp16);	
	
	return 0;	
}

uint8_t crc_byte( uint8_t data, uint8_t crc )
{
	crc ^= data;
	
	uint8_t i = 0;
	for( i ; i < 8 ; i++ )
	{
		crc = crc & 0x80 ? crc << 1 ^ 0x07 : crc << 1;
	}
	
	return crc;
}


uint8_t crc_buff( uint8_t *p, uint8_t len )
{
	uint8_t crc = 0;
	
	while( len-- )
	{
		crc = crc_byte( *(p++), crc );
	}
	
	return crc;
	
}

uint8_t set_emissivity( float emissivity )
{
	int8_t erro;
	/** Verifica o valor para a emissivilidade*/
	if( (emissivity<0) || (emissivity>1) )
	{
		printf("Valor incorreto para emissibilidade" );
		return 1;
	}
	
	/** Necessario zerar o registrador antes de setar a emissibilidade */
	erro = i2c_start( (MLX90614_ADDR<<1)|I2C_WRITE );
	if(erro)
	{
		printf("Erro Start\n");
		return 1;
	}
	
	/** Envia o comando e endereco do registrador */
	erro = i2c_write( 0x25 );
	if(erro)
	{
		return 1;
	}
	/** Zera a parte alta */
	erro = i2c_write( 0x00 );
	if(erro)
	{
		return 1;
	}
	/** Zera a parte baixa */
	erro = i2c_write( 0x00 );
	if(erro)
	{
		return 1;
	}
	/** Envia o PEC*/
	erro = i2c_write( 0x83 );
	if(erro)
	{
		return 1;
	}
	/** Finaliza a transmissao*/
	i2c_stop();
	/** Converte a variavel para o sensor */
	uint16_t emiss = (int)( emiss*65535 );
	
	uint8_t dataBuf[5];
	
	/** Endereco do dispositivo */
	dataBuf[0] = (MLX90614_ADDR<<1)|I2C_WRITE;
	
	/** Comando e endereco do resgistrador */
	dataBuf[1] = EMISS_COEF_ADDR;
	
	/** Parte baixa da emissividade */
	dataBuf[2] = emiss & 0xFF;
	
	/** Parte alta da emissividade */
	dataBuf[3] = emiss >> 8;
	
	/** PEC */
	dataBuf[4] = crc_buff( dataBuf, 4 );
	
	/** Verifica se a condicao start foi concluida */
	erro = i2c_start( dataBuf[0] );
	if(erro)
	{
		return 1;
	}
	
	/** Envia o comando e endereco do registrador */
	erro = i2c_write( dataBuf[1] );
	if(erro)
	{
		return 1;
	}
	/** Envia a parte baixa */
	erro = i2c_write( dataBuf[2] );
	if(erro)
	{
		return 1;
	}
	/** Envia a parte alta */
	erro = i2c_write( dataBuf[3] );
	if(erro)
	{
		return 1;
	}
	/** Envia o PEC */
	erro = i2c_write( dataBuf[4] );
	if(erro)
	{
		return 1;
	}
	/** Finaliza a transmissao*/
	i2c_stop();
	
	return 0;
}
/** @} */
/** @} */