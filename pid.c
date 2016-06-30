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
/**
 * \file pid.c
 *
 * \brief      Biblioteca de funcoes para o calculo PID
 * \details    Biblioteca com os metodos necessarios para o calculo do erro PID
 */
#include "pid.h"

/**
 * Constante critica
 */
#define KC			0.1
/**
 * Constante proporcional
 */
#define PCONSTANT	KC*0.7
/**
 * Constante integral
 */
#define ICONSTANT	KC*0.3
/**
 * Constante derivativo
 */
#define DCONSTANT	KC*1
/**
 * Maior valor para 16 bits
 */
#define MAXDOUBLE	65536
/**
 * Valor intermediario do maior valor para 16 bits
 */
#define MAXITERM	MAXDOUBLE/2
/**
 * Erro maximo
 */
#define MAXERROR	MAXDOUBLE/PCONSTANT
/**
 * Erro de soma maximo
 */
#define MAXSUMERROR MAXDOUBLE/ICONSTANT
/**
 * Ultimo valor processado
 */
double lastProcValue;
/**
 * Erro da soma atual
 */
double sumError;

double pid_controller( double dValue, double mValue )
{
	double error, pTerm, iTerm, dTerm, ret, temp;
	
	/** error = valor desejado - valor medido */
	error = dValue - mValue;
	
	/** Calcula a constante PROPORCIONAl */
	if( error > MAXERROR )
	{
		pTerm = MAXDOUBLE;	
		
	}else if( error < -MAXERROR )
	{
		pTerm = -MAXDOUBLE;
		
	}else
	{
		pTerm = PCONSTANT*error;	
		
	}
	
	/** Calcula a constante INTEGRAl */
	temp = sumError + error;
	
	if( temp > MAXSUMERROR )
	{
		iTerm = -MAXITERM;
		sumError = MAXSUMERROR;
		
	}else if( temp < -MAXSUMERROR )
	{
		iTerm = -MAXITERM;
		sumError = -MAXSUMERROR;
		
	}else
	{
		iTerm = ICONSTANT*sumError;
		sumError = temp;
	}
	
	/** Calcula a constante DERIVATIVA */
	dTerm = DCONSTANT*(lastProcValue - mValue);
	
	lastProcValue = mValue;
	
	ret = (pTerm + iTerm + dTerm);
	
	if( ret > 180 )
	{
		ret = 180;
		
	}else if( ret < 0 )
	{
		ret = 0;
		
	}
	
	return ret;
}

void pid_init()
{
	/** Inicia valores */
	lastProcValue = 0;
	sumError = 0;
}

/** @} */
/** @} */