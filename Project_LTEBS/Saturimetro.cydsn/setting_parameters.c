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
#include "setting_parameters.h"

#define FIFO_max_size 25

extern uint8_t j;

//Function used to chage the parameters of our sensor. As input we have the ADC range, the pulse width, the pulse ampl of the leds
// the sample rate and the average sample.
void setting_parameters( uint16 ADC_range, uint16 Pulse_width, uint16 Pulse_amp, uint16 Sample_rate, uint16 Samples) 
{
    MAX30101_Shutdown(); //every time we call this function we need shutdown our sensor and reset it to the power-on-state mode
    MAX30101_Reset();
    /*These 4 registers are the ones we have set at the beginning of the main.c file but every time we modify them it's like we are performing
    the inizialization of the sensor with the new values.*/
    // Wake up sensor
    MAX30101_WakeUp();        
    MAX30101_EnableFIFOAFullInt();
 
    // set 25 samples to trigger interrupt
    MAX30101_SetFIFOAlmostFull(FIFO_max_size);

    // enable fifo rollover
    MAX30101_EnableFIFORollover();
 
    //Now we are setting the registers of our sensor accordingly to the parameters we have changed from the GUI.    
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
    
    j=0; //index of the buffer returns to 0

    //MAX30101_INT_ClearInterrupt();
    MAX30101_ClearFIFO();
    MAX30101_WakeUp(); 
    CyDelay(100);
    
}

/* [] END OF FILE */
