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

#include <stdint.h>

#ifndef MILLIS_H_
#define MILLIS_H_
/** Inicia o contador*/
void millis_init( void );
/**
* Metodo que retorna o valor armazenado em milisseggundos
* \return millisegundos
*/
uint64_t millis_get( void );
/**
* Metodo que retorna o valor armazenado em milisseggundos em long
* \return millisegundos
*/
unsigned long millis_get_long(void);

#endif /* MILLIS_H_ */

/** @} */