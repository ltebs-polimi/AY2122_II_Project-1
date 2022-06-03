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
volatile long count;
uint8 ch_received;

CY_ISR (Count) {
     count++;
}

/*CY_ISR(Custom_ISR_RX)
{
    ch_received = UART_Debug_GetChar();
    switch(ch_received)
    {
        case '1':
            Pin_LED_Write(1);
            Timer_Start();
            break;
        case '2':
            Pin_LED_Write(0);
            Timer_Stop();
            break;
        default:
            break;
    }
}*/
/* [] END OF FILE */
