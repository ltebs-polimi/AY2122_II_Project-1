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
#include "parameters.h"
#include "setting_parameters.h"

#define FIFO_max_size 25

extern uint8_t j;

void setting_parameters( uint16 ADC_range, uint16 Pulse_width, uint16 Pulse_amp, uint16 Sample_rate, uint16 Samples) 
{
    MAX30101_Shutdown();
    MAX30101_Reset();
    
    // Wake up sensor
    MAX30101_WakeUp();        
    MAX30101_EnableFIFOAFullInt();
 
    // set 25 samples to trigger interrupt
    MAX30101_SetFIFOAlmostFull(FIFO_max_size);

    // enable fifo rollover
    MAX30101_EnableFIFORollover();
    
    // samples averaged       
    MAX30101_SetSampleAverage(Samples);
    // Set LED Power level
    MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1,Pulse_amp);        
    MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2,Pulse_amp); 
           
    // Set ADC Range
    MAX30101_SetSpO2ADCRange(ADC_range);
   
    
    // Pulse width
    MAX30101_SetSpO2PulseWidth(Pulse_width);
    
    // Set Sample Rate
    MAX30101_SetSpO2SampleRate(Sample_rate);
    
    // Set mode
    MAX30101_SetMode(MAX30101_SPO2_MODE);
    
    // Enable Slots
    MAX30101_DisableSlots();
    
    //debug_print("ADC RANGE 2048\r\n");
    j=0;

    //MAX30101_INT_ClearInterrupt();
    MAX30101_ClearFIFO();
    MAX30101_WakeUp(); 
    CyDelay(100);
    
    ADC_range = 0;
}

/* [] END OF FILE */
