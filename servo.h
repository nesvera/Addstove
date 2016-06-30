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

#include <stdint.h>

#ifndef SERVO_H_
#define SERVO_H_

/** 
* Servo trabalho com frequencia de 50Hz = 20ms
*/
void servo_init( void );
/** 
* Seta um angulo para o servo
* \param (angle) Angulo desejado
*/
void set_servo( int32_t angle );


#endif /* SERVO_H_ */

/** @} */
/** @} */