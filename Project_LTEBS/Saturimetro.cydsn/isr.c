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
uint8 x;

// This interrupt is called when PSoC receives different parameters from the GUI
CY_ISR (Custom_ISR_RX) 
{
    x = UART_Debug_GetChar();   
    USER(x); //The commands are taken as input by the function USER
    CyDelay(100);
}
/* [] END OF FILE */
