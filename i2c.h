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

#ifndef I2C_H_
#define I2C_H_

/** Macro Read = 1*/
#define I2C_READ 0x01
/** Macro Write = 0*/
#define I2C_WRITE 0x00


/** 
* Inicializa I2C
*/
void		i2c_init(void);
/** 
* Recebe dados com ack
* \return TWDR - Dado com reconhecimento
*/
uint8_t		i2c_read_ack(void);
/** 
* Recebe dados sem ack
* * \return TWDR - Dado sem reconhecimento
*/
uint8_t		i2c_read_nack(void);
/** 
* Transmissao de dados do sensor
* \param  *bufferIn Pontero da variavel que recebe o dado
* \param  length Quantidade de bytes a ser lido
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t 		i2c_receive(uint8_t* bufferIn, uint16_t length);
/** 
* Habilita a comunicacao I2C com o servo
* \param (address) endereco do servo
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t		i2c_start(uint8_t address);
/**
* Finaliza I2C
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t		i2c_stop(void);
/**
* Transmissao de multiplos dados para o servo
* \param slaveAddr endereco do servo
* \param *data Pontero do buffer de dados a ser enviado
* \param length quantidade de bytes a serem gravados
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t		i2c_transmit(uint8_t slaveAddr, uint8_t* data, uint16_t length);
/**
* Transmissao de um byte para o servo
* \param data dado a ser enviado
* \return 0 - Sucesso, 1 - sem sucesso
*/
uint8_t		i2c_write(uint8_t data);

#endif /* I2C_H_ */

/** @} */
/** @} */


