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
 * \defgroup pid Controle PID
 * @{
 *
 */


#ifndef PID_H_
#define PID_H_

/**
* Realiza o calculo matematico do PID
* \param (dValue) Valor desejado
* \param (mValue) Valor medido
* \return angulo
*/
double pid_controller( double dValue, double mValue );
/**
* Inicializa as variaveis da formula
*/
void pid_init(void);

#endif /* PID_H_ */

/** @} */
/** @} */