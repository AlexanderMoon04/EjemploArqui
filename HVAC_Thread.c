 // FileName:        HVAC_Thread.c
 // Dependencies:    HVAC.h
 // Processor:       MSP432
 // Board:           MSP432P401R
 // Program version: CCS V8.3 TI
 // Company:         Texas Instruments
 // Description:     Definici�n de las funciones del thread de HVAC_Thread().
 // Authors:         Jos� Luis Chac�n M. y Jes�s Alejandro Navarro Acosta.
 // Updated:         11/2018

#include "hvac.h"                           // Incluye definici�n del sistema.

/*****  SE DECLARARON LAS VARIABLES Y FUNCIONES PARA REALIZAR EL DALAY CON EL TIMER ******** */
bool retraso = false;
void Timer32_INT1 (void); // Funci�n de interrupci�n.
void Delay_ms (uint32_t time); // Funci�n de delay.
/***********************************************************************************************/

/*********************************THREAD******************************************
 * Function: HVAC_Thread
 * Preconditions: None.
 * Overview: Realiza la lectura de la temperatura y controla salidas actualizando
 *           a su vez entradas. Imprime estados. Tambi�n contiene el heartbeat.
 * Input:  Apuntador vac�o que puede apuntar cualquier tipo de dato.
 * Output: None.
 *
 ********************************************************************************/

void *HVAC_Thread(void *arg0)
{
    SystemInit();
    System_InicialiceTIMER();            // SE A�ADIO LA FUNCION PARA INICIALIZAR EL TIMER
    HVAC_InicialiceIO();
    HVAC_InicialiceADC();
    HVAC_InicialiceUART();

    while(1)
    {
        contadorApg = 0;
        Select_Menu = 0;
        GPIO_setOutput(LED_RGB_PORT, LED_Rojo, 0);
        GPIO_setOutput(LED_RGB_PORT, LED_Verde, 0);
        GPIO_setOutput(LED_RGB_PORT, LED_Azul, 0);

        while(Enc_Apg == ENCENDIDO){
            HVAC_ActualizarEntradas();
            HVAC_Enc_Apg_Check();
            HVAC_PrintState();
        }

    }
}



/* *********  FUNCIONES PARA REALIZAR EL DELAY CON EL TIMER ********* */
void Delay_ms(uint32_t time)
{
    T32_EnableTimer1(); // Habilita timer.
    T32_EnableInterrupt1(); // Habilita interrupci�n.
    // Carga de valor en milisegundos.
    T32_SetLoadValue1(time*(__SYSTEM_CLOCK/1000));
    retraso = true;
    while(retraso); // While enclavado.
}
void Timer32_INT1(void)
{
    T32_ClearInterruptFlag1(); // Al llegar a la interrupci�n
    retraso = false; // desenclava el while.
    contadorApg = 0;
}

