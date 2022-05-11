/**
*   Main file for testing MAX30101 Library.
*/

#include "project.h"
#include "MAX30101.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "SpO2.h"
#include "HeartRate.h"


#define UART_DEBUG

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)

CY_ISR_PROTO(MAX30101_ISR);

uint8_t flag_temp = 0;
uint32_t redBuffer[200];
uint32_t irBuffer[200];
int8_t num_samples;
uint8_t i;
int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

int main(void)
{
    // Variables
    MAX30101_Data data;
    data.head = 0;
    data.tail = 0;
    char msg[50];
    void (*print_ptr)(const char*) = &(UART_Debug_PutString);
    uint8_t active_leds = 1;
    uint8_t rp, wp, flag = 0;
    bufferLength = 200;
    
    // Initialization
    MAX30101_Start();
    UART_Debug_Start();

    CyDelay(100);
    
    debug_print("**************************\r\n");
    debug_print("         MAX30101         \r\n");
    debug_print("**************************\r\n");
    
    if (MAX30101_IsDevicePresent() == MAX30101_OK)
    {
        // Check if device is present
        debug_print("Device found on I2C bus\r\n");
        Connection_LED_Write(1);
        
        // Read revision and part id
        uint8_t rev_id, part_id = 0;
        MAX30101_ReadPartID(&part_id);
        MAX30101_ReadRevisionID(&rev_id);
        sprintf(msg,"Revision ID: 0x%02X\r\n", rev_id);
        debug_print(msg);
        sprintf(msg,"Part ID: 0x%02X\r\n", part_id);
        debug_print(msg);
        
        debug_print("Registers before configuration\r\n");
        MAX30101_LogRegisters(print_ptr);
        
        // Soft reset sensor
        MAX30101_Reset();
        CyDelay(100);
        
        // Wake up sensor
        MAX30101_WakeUp();
        
        MAX30101_DisableALCOverflowInt();
        MAX30101_DisableTempReadyInt();
        MAX30101_DisablePPGReadyInt();
        MAX30101_EnableFIFOAFullInt();
     
        // set 28 samples to trigger interrupt
        MAX30101_SetFIFOAlmostFull(28);

        // enable fifo rollover
        MAX30101_EnableFIFORollover();
        
        // 8 samples averaged
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_2);
        
        // Set LED Power level
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0A);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0A);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_3, 0x0A);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_4, 0x0A);
        
        // Set ADC Range
        MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_4096);
        
        // Pulse width
        MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_411);
        
        // Set Sample Rate
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_400);
        
        // Set mode
        MAX30101_SetMode(MAX30101_SPO2_MODE);
        
        // Enable Slots
        MAX30101_DisableSlots();
        
        debug_print("Registers after configuration\r\n");
        MAX30101_LogRegisters(print_ptr);
    }
    
    debug_print("\r\n\r\n");
    
    isr_MAX30101_StartEx(MAX30101_ISR);
    // Clear FIFO
    MAX30101_ClearFIFO();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    for (i=0;i<100;i++) {
    num_samples = data.head - data.tail;
    while(num_samples == 0)
    {
        MAX30101_ReadReadPointer(&rp);
        MAX30101_ReadWritePointer(&wp);
        //Calculate the number of readings we need to get from sensor
        num_samples = wp - rp;
        if (num_samples <= 0) 
            num_samples += 32;
        MAX30101_ReadFIFO(num_samples, active_leds, &data);
    }
    
    for(int k = 0; k<num_samples;k++) 
    {
        redBuffer[k] = getFIFORed(&data);
        irBuffer[k] = getFIFOIR(&data);
        data.tail++;
        data.tail %= 32;
        sprintf(msg,  "Red=%ul\r\n", redBuffer[k]);
        debug_print(msg);
        sprintf(msg,  "IR=%ul\r\n", irBuffer[k]);
        debug_print(msg);
    }
    }
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    
    for(;;) {
    for (int i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }
    
    //take 25 sets of samples before calculating the heart rate.
    for (int i = 75; i < 100; i++)
    {
    //We're finished with this sample so move to next sample
    num_samples = data.head - data.tail;
    while(num_samples == 0)
    {
        MAX30101_ReadReadPointer(&rp);
        MAX30101_ReadWritePointer(&wp);
        //Calculate the number of readings we need to get from sensor
        num_samples = wp - rp;
        if (num_samples <= 0) 
            num_samples += 32;
        MAX30101_ReadFIFO(num_samples, active_leds, &data);
    }
    redBuffer[i] = getFIFORed(&data);
        irBuffer[i] = getFIFOIR(&data);
        data.tail++;
        data.tail %= 32;
    //send samples and calculation result to terminal program through UART
    sprintf(msg,  "Red=%d\r\n", redBuffer[i]);
    debug_print(msg);
    sprintf(msg,  "IR=%d\r\n", irBuffer[i]);
    debug_print(msg);
    sprintf(msg,  "heartRate=%d\r\n", heartRate);
    debug_print(msg);
    sprintf(msg,  "validHeartRate=%d\r\n", validHeartRate);
    debug_print(msg);
    sprintf(msg,  "SpO2=%d\r\n", spo2);
    debug_print(msg);
    sprintf(msg,  "validSpO2%u\r\n", validSPO2);
    debug_print(msg);
      
    }
    //After gathering 25 new samples recalculate HR and SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  
        /*if (flag_temp == 1)
        {
            MAX30101_IsFIFOAFull(&flag);
            if (flag > 0)
            {   
                MAX30101_ReadReadPointer(&rp);
                MAX30101_ReadWritePointer(&wp);
                //Calculate the number of readings we need to get from sensor
                num_samples = wp - rp;
                if (num_samples <= 0) 
                    num_samples += 32; //Wrap condition
                // Print out number of samples
                sprintf(msg, "%d\r\n", num_samples);
                debug_print(msg);
                // Read FIFO
                MAX30101_ReadFIFO(num_samples, active_leds, &data);
                
                sprintf(msg,  "%ul\r\n", getFIFORed(&data));
                
                debug_print(msg);
            }
            flag_temp = 0;
        }*/
        }
    }


CY_ISR(MAX30101_ISR)
{
    Connection_LED_Write(!Connection_LED_Read());
    MAX30101_INT_ClearInterrupt();
    flag_temp = 1;
}

/* [] END OF FILE */
