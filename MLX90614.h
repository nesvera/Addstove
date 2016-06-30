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

#ifndef MLX90614_H_
#define MLX90614_H_

#include <stdint.h>

/** Endereco padrao do escravo do sensor */
#define MLX90614_ADDR	0x5A	
/** Endereco da temperatura do chip */		
#define TCHIP_ADDR		0x06
/** Endereco da temperatura do objeto 1 */			
#define TOBJECT1_ADDR	0x07
/** Endereco da temperatura do objeto 2 */			
#define TOBJECT2_ADDR	0x08			

/** Endereco do registrador do coeficiente de emissividade */
#define EMISS_COEF_ADDR 0x24

/** Inicia a comunicacao I2C */
void MLX90614_init();
/**
* Pega a temperatura do chip do sensor 
* \param (*varTemp) Pontero da variavel que recebe esse valor
*/					
uint8_t get_chip_temp( double *varTemp );
/**
* Pega a temperatura do alvo do sensor, retorna em Celsius
* \param (*varTemp) Pontero da variavel que recebe esse valor
 */	
uint8_t get_object1_temp( double *varTemp );
/** 
* Em desenvolvimento 
* \param (*varTemp) Pontero da variavel que recebe esse valor
*/	
uint8_t get_object2_temp( double *varTemp );
/** 
* Retorna um valor de 16-bits lido pelo sensor
* \param (address) Registrador a ser lido no sensor
* \param (*varTemp) Pontero da variavel que recebe esse valor
*/	
uint16_t read_sensor( uint8_t address, double *varTemp );
/**
* Seta a emissidade do objeto
* A emissividade eh gravada em um inteiro de 16 bits
* alcance 0.1 a 1.0 , padrao 1.0
* \param (emiss) valor a ser gravado no sensor
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t set_emissivity( float emiss );
/**
* Realiza o CRC de todos os bytes
* \param (data) dado do buffer
* \param crc CRC acumulado
* \return CRC acumulado
*/
uint8_t crc_byte( uint8_t data, uint8_t crc );
/**
* Envia o buffer recursivamente para crc_byte
* \param (*p) Pontero do vetor
* \param len Tamanho dele
* \return CRC
*/
uint8_t crc_buff( uint8_t *p, uint8_t len );



#endif /* MLX90614_H_ */
/** @} */
/** @} */