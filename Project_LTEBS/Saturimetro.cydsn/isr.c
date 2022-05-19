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
#define UART_DEBUG

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)


#include "project.h"
#include "isr.h"
long count=0;
char msg[50];

CY_ISR_PROTO(Count) {
    count++;
    //sprintf(msg, "millis: %ld\r\n", count);
    //debug_print(msg);
}
/* [] END OF FILE */
