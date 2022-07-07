/*
    LAB PROJECT 2022, Group 1
    Emanuele Falli
    Federico Monterosso
    Francesca Terranova
*/

#include "project.h"
#include "MAX30101.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "SpO2_HR.h"
#include "Timer.h"
#include "isr.h"
#include "USER.h"
#include "setting_parameters.h"

#define UART_DEBUG
#define FIFO_max_size 25 
/*25 has been set as the maximum size so that half a second is measured every
time the interrupt is triggere by the filling of the FIFO*/

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)

CY_ISR_PROTO(MAX30101_ISR);

uint8_t flag_temp = 0; 
/*flag_temp is activated inside the isr code of the MAX30101 and ensures that the sampling frequency of 
the main code is executed at the desired frequency equal to that of the filling of the FIFO*/

uint8_t j=0; 
/*this flag controls the number of samples before reaching the 200 samples 
(4 seconds of measurement at a sample rate of 50 Sa/s) that trigger the actuation of the maxim function 
for spo2 and hr calculation*/
uint8 flag_start = 1;


int main(void)
{
    // Variables
    MAX30101_Data data;
    data.head = 0;
    data.tail = 0;
    char msg[50];
    uint8_t active_leds = 2;
    uint8_t rp, wp, flag = 0;
    int32_t spo2; //SPO2 value calculated through the maxim function given by the manufacturer
    int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
    int32_t heartRate; //heart rate value 
    int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
    uint32_t irBuffer[200]; //infrared LED sensor data
    uint32_t redBuffer[200];  //red LED sensor data
    int32_t bufferLength = 200;
    int32_t somma=0; 
    int32_t avg_hr=0;
    int num_samples;
 
    int i=0;
    int f=1;
    
    // Initialization
    MAX30101_Start();
    UART_Debug_Start();
    CyDelay(100);    
    if (MAX30101_IsDevicePresent() == MAX30101_OK)
    {
        // Check if device is present
        Connection_LED_Write(1);
        
        // Soft reset sensor
        MAX30101_Reset();
        CyDelay(100);
       
        // Wake up sensor
        MAX30101_WakeUp();       
        // Only the FIFO_AFULL interrupt is activated, therefore the isr code works only in this case.
        MAX30101_EnableFIFOAFullInt();
     
        // set 25 samples to trigger interrupt
        MAX30101_SetFIFOAlmostFull(FIFO_max_size);

        // enable fifo rollover
        MAX30101_EnableFIFORollover();
        
        // samples averaged       
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_2);
        // Set LED Power level
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1F);        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1F);
               
        // Set ADC Range
        MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_8192);
        
        // Pulse width
        MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_411);
        
        /* Set Sample Rate, in a way to ensure that 50 Sa/s is always kept constant, therefore the 
        sample average needs to be configured properly when changing the sample rate through the GUI*/ 
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_100);
        
        // Set mode (always spo2 mode in this project)
        MAX30101_SetMode(MAX30101_SPO2_MODE);
        
        // Enable Slots
        MAX30101_DisableSlots();
        
    }
    
    isr_MAX30101_StartEx(MAX30101_ISR);
    isr_RX_StartEx(Custom_ISR_RX);
    
    // Clear FIFO
    MAX30101_ClearFIFO();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
      
    for(;;)
    {    
        if(flag_temp == 1)
        /*this flag is controlled by the isr of the MAX (issued only by the filling of the FIFO because
        the other interrupts are all disabled*/
        {
            MAX30101_IsFIFOAFull(&flag);
            if(flag>0)
            {
                MAX30101_ReadReadPointer(&rp);
                MAX30101_ReadWritePointer(&wp);
                //Calculate the number of readings we need to get from sensor (always 25 in this case)
                num_samples = wp - rp;
                if (num_samples <= 0) num_samples += 32; //Wrap condition                
                // Read FIFO
                MAX30101_ReadFIFO(num_samples, active_leds, &data);
                for (i=0;i<num_samples;i++)
                {                    
                    //the 25 samples are stored separately inside two buffers of length 200
                    redBuffer[j] = data.red[data.tail+i];
                    irBuffer[j] = data.IR[data.tail+i];
                    j++;
                    debug_print("1,");
                    /*these numbers help us in intercepting the right value inside the python code,
                    so that they are placed in the corresponding array accordingly and are displayed
                    inside the plot or saved in the labels of Spo2 and HR*/
                    sprintf(msg, "%ld,", data.IR[data.tail+i]);
                    debug_print(msg);
                    debug_print("2,");
                    sprintf(msg, "%ld,", data.red[data.tail+i]);
                    debug_print(msg);
                    
                    if(j<bufferLength)
                    {
                        /*these null values ensure that the array length sent via UART to the GUI 
                        is always constant; in this way when, after the iterations, we obtain a measure
                        of Spo2 and HR, we can save them appropriately.*/
                        debug_print("0,");
                        debug_print("0,");
                        debug_print("0,");
                        debug_print("0\n");
                    }
                }
                
                if(j>=bufferLength) 
                {
                    //we have reached 200 samples and the maxim function is triggered
                    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
                    j=bufferLength - 50;
                    /*50 samples are deleted, and replaced with a new second of measurement
                    coming from the sensor*/
                    for (int i=50; i<bufferLength; i++) 
                    {
                        /*The remaining 150 samples are shifter up at the beginning of the array so 
                        that the last second is entered at the end and the appropriate sequence is kept.*/
                        redBuffer[i-50] = redBuffer[i];
                        irBuffer[i-50] = irBuffer[i];
                    }
                    /*the spo2 and HR values are generated and will replace the empty values shown before,
                    to be sent via UART to the GUI*/
                    debug_print("3,");
                    sprintf(msg, "%ld,", spo2);
                    debug_print(msg);
                    debug_print("4,");
                    sprintf(msg, "%ld\n", avg_hr);
                    debug_print(msg);
                    if(heartRate > 60 && heartRate < 120) 
                    {
                        //an average of 10 values of HR is performed to obtain a more constant value
                        somma += heartRate;
                        if(f==10)
                        {
                            somma = somma/10;
                            f = 0;
                            avg_hr = somma;
                            somma = 0;
                        }
                        f++;
                    }
                }
            }
            
            if(flag_ADC == 1)
            {
                /*this series of flags represent each parameter that can be changed via the GUI, each
                flag is activated when changing a specific parameter and the setting parameters function
                is performed.*/
                if(flag_start)
                {
                    /*the flag_start is activated the first time the code is run, after the user activates a changing of a
                    parameter inside the GUI; it is done in a way to ensure that all the other parameters not changed
                    are still initialized to a default variable, otherwise the program would not function anymore.*/
                    Pulse_amp = 0x1F;
                    Pulse_width = MAX30101_PULSEWIDTH_411;
                    Samples = MAX30101_SAMPLE_RATE_100;
                    Average = MAX30101_SAMPLE_AVG_2;
                    flag_start = 0;
                }
                setting_parameters(ADC_range, Pulse_width, Pulse_amp, Samples, Average);
                flag_ADC = 0;
                
            }
            
            if(flag_PW == 1)
            {
                if(flag_start)
                {
                    Pulse_amp = 0x1F;
                    Samples = MAX30101_SAMPLE_RATE_100;
                    Average = MAX30101_SAMPLE_AVG_2;
                    ADC_range = MAX30101_ADC_RANGE_4096;
                    flag_start = 0;
                }
                setting_parameters(ADC_range, Pulse_width,  Pulse_amp, Samples, Average);
                flag_PW = 0;
            }
            
            if(flag_PA == 1)
            {
                if(flag_start)
                {
                    Pulse_width = MAX30101_PULSEWIDTH_411;
                    Samples = MAX30101_SAMPLE_RATE_100;
                    Average = MAX30101_SAMPLE_AVG_2;
                    ADC_range = MAX30101_ADC_RANGE_4096;
                    flag_start = 0;
                }
                setting_parameters(ADC_range, Pulse_width,  Pulse_amp, Samples, Average);
                flag_PA = 0;
            }
            
            if(flag_SR == 1)
            {
                if(flag_start)
                {
                    Pulse_width = MAX30101_PULSEWIDTH_411;
                    Pulse_amp = 0x1F;
                    ADC_range = MAX30101_ADC_RANGE_4096;
                    flag_start = 0;
                }
                setting_parameters(ADC_range, Pulse_width,  Pulse_amp, Samples, Average);
                flag_SR = 0;
            }
            flag_temp = 0;
        }        
    } 
}   


CY_ISR(MAX30101_ISR)
{
    flag_temp = 1;
    Connection_LED_Write(!Connection_LED_Read());
    MAX30101_INT_ClearInterrupt();
}

