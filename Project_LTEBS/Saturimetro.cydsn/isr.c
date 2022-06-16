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
volatile long count=0;
volatile uint8 flag_SM=0;
volatile uint8 SM=0;

CY_ISR_PROTO(Count) 
{
    if (flag_SM==1)
    {
     count++;
        if (count==5) SM=1;
    }
    
}
/* [] END OF FILE */
