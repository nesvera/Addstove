
/**
\mainpage AddStove - Controle de Fluxo de Gas
\author Daniel Nesvera, Ricardo Grando <daniel.nesvera@ecomp.ufsm.br,ricardo.grando@ecomp.ufsm.br>
\version v0
\date 30/06/2016
*/

/**
 * \addtogroup FuncaoPrincipal
 * @{
 * 
 */
/**
 * \file main.c
 *
 * \brief      Funcao principal do projeto
 * \details    A funcao principal inicializa e escalona as protothreads de acordo com suas variaveis de estado.
 */

#include "pt.h" 
#include "uart.h" 
#include "servo.h"
#include "pid.h"
#include "MLX90614.h"
#include "millis.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

#include <stdio.h> /** For printf(). */
/** ID das protothreads */
static struct pt sensor, matlab, PID_PWM;
/** Variaveis de estado que definem o estado das protothreads */
static int sensorFlag, matlabFlag, PWMFlag;
/** variaveis globais */
static double atual_temp, desired_temp, angulo_pid;

/**
* Protothread de leitura do sensor
*/
static int
readFromSensor(struct pt *pt)
{	
	PT_BEGIN(pt);	
	while(1) {
		/** Espera ateh que a flag da protothread seja setada. */
		PT_WAIT_UNTIL(pt, sensorFlag != 0);
		
		/* O sensor paro de funciona poxa vida
		int n = get_object1_temp(&atual_temp);
		if (n){
			printf("erro");
		}
		*/
		int aux = read_pot();
		atual_temp = (double)(aux);
		
		/** Reseta a flag do sensor e seta a do envio pra android */
		sensorFlag = 0;
		matlabFlag = 1;		
	}
	PT_END(pt);
}
/**
* Protothread de envio para MATLAB
*/
static int
sendToMatlab(struct pt *pt)
{
	PT_BEGIN(pt);
	while(1) {
		/** Espera ateh que a flag da protothread seja setada. */
		PT_WAIT_UNTIL(pt, matlabFlag != 0);
		/** Envia o buffer pro matlab le via uart*/
		printf("$%.2lf,%.2lf;*\n\r", atual_temp, angulo_pid );
				
		/** Reseta a flag do MATLAB e seta a do PWM */
		matlabFlag = 0;
		PWMFlag = 1;		
	}
	PT_END(pt);
}
/** 
* Protothread do PWM
*/
static int
setPWM(struct pt *pt)
{
	PT_BEGIN(pt);
	while(1) {
		/** Espera ateh que a flag da protothread seja setada. */
		PT_WAIT_UNTIL(pt, PWMFlag != 0);
		/** Realiza o calculo PID com as temperatura*/
		angulo_pid = pid_controller( desired_temp, atual_temp );
		
		int aux = (int)angulo_pid;
		/** Seta o angulo no servo*/
		set_servo( aux );		
		
		/** Reseta a flag de PWM e seta a do sensor */
		PWMFlag = 0;
		sensorFlag = 1;		
	}
	PT_END(pt);
}

int
main(void)
{
	/** Inicia sensor*/
	MLX90614_init();
	/** Inicia PID*/
	pid_init();
	/** Inicia servo*/
	servo_init();
	/** Seta o baud rate*/
	usart_init( (uint32_t)9600 );
	/** Inicia ADC*/
	adc_init();
	/** Habilita interrupcao*/
	sei();
	

	/** Seta coeficiente de emissibilidade da agua */
	if( !set_emissivity( 0.2 ) )
	{
		printf("Erro configurando Emissividade!\n");
	}
	
	/** Seta a temperatura desejada*/
	desired_temp = 40;
	/** E o angulo em zero*/
	angulo_pid = 0;
	/** Inicializa as protothreads com PT_INIT(). */
	PT_INIT(&sensor);
	PT_INIT(&matlab);
	PT_INIT(&PID_PWM);
	/** Seta a flag do sensor */
	sensorFlag = 1; 
	/**
	* O loop infinito escalona repetidamente as protothreads chamando suas funcoes
	* de protothreads e passando um pontero para a variavel de estado da protothread como
	* argumento	
	*/	
	while(1) {
		readFromSensor(&sensor); 
		sendToMatlab(&matlab);
		setPWM(&PID_PWM);
	}
}
/** @} */