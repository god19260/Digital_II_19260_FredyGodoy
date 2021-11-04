
/*-----------------------------------------------------------------------------
 ----------------------------L I B R E R I A S---------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
/*-----------------------------------------------------------------------------
 -----------------V A R I A B L E S   A   I M P L E M T E N T A R--------------
 -----------------------------------------------------------------------------*/
uint32_t ui32Period;
uint16_t estado, botonazo, antirrebote,timer0_count;
uint32_t i = 0;
/*-----------------------------------------------------------------------------
 ------------ P R O T O T I P O S   D E   F U N C I O N E S -------------------
 -----------------------------------------------------------------------------*/
void delay(uint32_t msec);
void delay1ms(void);
void semaforo(void);
/*-----------------------------------------------------------------------------
 --------------------- I N T E R R U P C I O N E S ----------------------------
 -----------------------------------------------------------------------------*/

void Int_GPIOF(void){
    estado = GPIOIntStatus(GPIO_PORTF_BASE, true);      //lectura de interrupcion
    if (estado==16){
        antirrebote=1;
    }
    else{
        antirrebote=0;
    }
    GPIOIntClear(GPIO_PORTF_BASE,estado);              //se apaga bandera de interrupcion
}

void Timer0IntHandler(void)
{
    timer0_count=!timer0_count;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);         //se limpia la bandera de interrupcion
    //cuenta1++;

}

/*-----------------------------------------------------------------------------
 ------------------------------ S E T   U P -----------------------------------
 -----------------------------------------------------------------------------*/
int main(void)
 {
    //-------CONFIGURACION DEL RELOJ
    SysCtlClockSet(SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL);            //reloj externo, cristal de 16mhz, fosc 40mhz, pll como reloj de sistema
    //-------CONFIGURACION DE ENTRADAS Y SALIDAS
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);                                                // Se asigna reloj a puerto F
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTF_CR_R = 0x0f;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);               //se define salida de leds en PF[3:1]
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);                                          //se define entrada de boton en PF4
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);     //boton como weak pull up
    //-------CONFIGURACION DE TIMER
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);                                                // Se asigna reloj a timer 0
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);                                            //cuenta periodica
    TimerLoadSet(TIMER0_BASE, TIMER_A, 19999999);                                               //cuenta de 4m casi cada 1sec
    TimerEnable(TIMER0_BASE, TIMER_A);                                                          //se habilita el timer de 32bits
    //-------CONFIGURACION DE INTERRUPCIONES
    //botones
    IntEnable(INT_GPIOF);                                                                       //se habilita la interrupcion en el puerto F
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4);                                                 //se especifica que pines tendran la interrupcion
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);                             //interrupcion en flanco de bajada
    //timer0
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//se habilita interrupcion del timer0
    //interrupciones globales
    IntMasterEnable();                                                                          //se habilitan las interrupciones globales
    //valor inicial de la variable
    timer0_count=0;
    /*-----------------------------------------------------------------------------
    -------------------------- M A I N   L O O P ---------------------------------
    -----------------------------------------------------------------------------*/
    while (1)
    {
  /*      //PARTE POST LAB
        if(timer0_count==0)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);
        }
        else if (timer0_count==1)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 6);
        }
*/

        //PARTE COMENTADA DE LAB
        if(antirrebote==1 && estado==16)
        {
            antirrebote=0;
            semaforo();
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        }

    }
}


/*-----------------------------------------------------------------------------
 ------------------------- F U N C I O N E S ----------------------------------
 -----------------------------------------------------------------------------*/
void delay(uint32_t msec)
{
    for (i = 0; i < msec; i++)
    {
        delay1ms();
    }

}

void delay1ms(void)
{
    SysTickDisable();
    SysTickPeriodSet(16000);
    SysTickEnable();

    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0); //Pg. 138

}

void semaforo (void)
{
    //verde normal
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);     //led en verde
    delay(5000);
    //verde titilante
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);     //led en verde titilante
    delay(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);     //led en verde titilante
    delay(500);
    //amarillo
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);     //led en amarillo
    delay(3000);
    //rojo
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);      //led en rojo
    delay(5000);
    return;
}
