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
#include "MAX30101.h"
#include "USER.h"
uint16 ADC_range ;
uint16 Samples ;
uint16 Pulse_width ;
uint16 Pulse_amp ;
uint16 Average ;
uint8 flag_ADC ;
uint8 flag_PW ;
uint8 flag_PA ;
uint8 flag_SR;


void setting_parameters( uint16 ADC_range, uint16 Pulse_width, uint16 Pulse_amp, uint16 Sample_rate, uint16 Average);
/* [] END OF FILE */
