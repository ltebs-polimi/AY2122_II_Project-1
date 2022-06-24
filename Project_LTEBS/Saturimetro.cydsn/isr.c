/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "project.h"
#include "isr.h"
#include "USER.h"
volatile long count=0;
volatile uint8 flag_SM=0;
volatile uint8 SM=0;
uint8 x;
extern volatile int gotInterrupt;
extern uint32_t redBuffer[200];
extern uint32_t irBuffer[200];

/*CY_ISR (Count) 
{
    if (flag_SM==1)
    {
     count++;
        if (count==5) SM=1;
    }
}*/

CY_ISR (Custom_ISR_RX) 
{
    x = UART_Debug_GetChar();
    gotInterrupt = 1;
    USER(x);
    CyDelay(100);
    /*for (int k = 0; k<200; k++)
    {
        redBuffer[k] = 0;
        irBuffer[k] = 0;
    }*/
}
/* [] END OF FILE */
