/**
*   Main file for testing MAX30101 Library.
    Last update: il codice funziona con la spo2 anche per FIFO bits diversi da 25, per come ho scritto il codice la prima misurazione 
    non è da considerare perchè vengono superati i 200 samples però dalla successiva sono giuste.
    Questo branch serve a provare una nuova funzione, che prende 5 secondi di misura e introduce un Hamming window per cercare di migliorare
    l'heart rate ma sembra essere troppo variabile così come nell'altro branch. Non so se è dovuto alla mancanza di pressione ma sembra strano
    perchè la variabilità è davvero alta. Proverò con l'elastico per capire se migliora un minimo, altrimenti c'è da cambiare il codice per l'HR.
    Emanuele 16.05 18:07
*/

#include "project.h"
#include "MAX30101.h"
#include "stdio.h"
#include "I2C_Interface.h"
#include "SpO2.h"

#define UART_DEBUG

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)

CY_ISR_PROTO(MAX30101_ISR);
    Timer_Init();
    isr_1_StartEx(Count);

uint8_t flag_temp = 0;
uint8_t flag_1s = 0;
extern uint8 count;

int main(void)
{
    // Variables
    MAX30101_Data data;
    data.head = 0;
    data.tail = 0;
    char msg[50];
    void (*print_ptr)(const char*) = &(UART_Debug_PutString);
    uint8_t active_leds = 2;
    uint8_t rp, wp, flag = 0;
    int32_t spo2; //SPO2 value
    int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
    int32_t heartRate; //heart rate value
    int8_t validHeartRate; //indicator to show if the heart rate calculation is valid
    uint32_t irBuffer[200]; //infrared LED sensor data
    uint32_t redBuffer[200];  //red LED sensor data
    int32_t bufferLength = 200;
    int num_samples;
    int j=0;
    int FIFO_max_size = 25;
    const uint8_t RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
    uint8_t rates[RATE_SIZE]; //Array of heart rates
    uint8_t rateSpot = 0;
    long lastBeat = 0; //Time at which the last beat occurred
    uint8_t x;
    uint8_t beatsPerMinute;
    uint8_t beatAvg;
    
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
        MAX30101_SetFIFOAlmostFull(FIFO_max_size);

        // enable fifo rollover
        MAX30101_EnableFIFORollover();
        
        // 8 samples averaged
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_2);
        
        // Set LED Power level
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x05);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x05);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_3, 0x1F);
        
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_4, 0x1F);
        
        // Set ADC Range
        MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_2048);
        
        // Pulse width
        MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_411);
        
        // Set Sample Rate
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_50);
        
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
    
    for(;;)
    {
        if (flag_temp == 1)
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
                MAX30101_ReadFIFO(num_samples, active_leds, &data, j);
                //int samples = data.head - data.tail;
                //sprintf(msg, "%d\r\n", samples);
                //debug_print(msg);
                for (int i=0;i<num_samples;i++){
                    sprintf(msg, "red: %lu\r", data.red[data.tail+i]);
                    debug_print(msg);
                    sprintf(msg, "ir: %lu\r", data.IR[data.tail+i]);
                    debug_print(msg);
                    //data.tail++;
                    redBuffer[j] = data.red[data.tail+i];
                    irBuffer[j] = data.IR[data.tail+i];
                    j++;
                    sprintf(msg, "number of samples taken: %d\r", j);
                    debug_print(msg);
                    /*if(flag_1s) {
                        sprintf(msg, "spo2: %ld\r", spo2);
                        debug_print(msg);
                        sprintf(msg, "validspo2: %d\r", validSPO2);
                        debug_print(msg);
                        sprintf(msg, "HR: %ld\r", heartRate);
                        debug_print(msg);
                        sprintf(msg, "validHR: %d\r\n", validHeartRate);
                        debug_print(msg);   
                        flag_1s = 0;
                    }*/
                }
                //data.tail = 0;
                //data.head = 0;
            }
            flag_temp = 0;
            if(j>=bufferLength) {
                maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
                //flag_1s = 1;
                j=bufferLength - 50;
                for (int i=50; i<bufferLength; i++) {
                    redBuffer[i-50] = redBuffer[i];
                    irBuffer[i-50] = irBuffer[i];
                }
                sprintf(msg, "spo2: %ld\r", spo2);
                debug_print(msg);
                sprintf(msg, "validspo2: %d\r", validSPO2);
                debug_print(msg);
                sprintf(msg, "HR: %ld\r", heartRate);
                debug_print(msg);
                sprintf(msg, "validHR: %d\r\n", validHeartRate);
                debug_print(msg); 
            }
        }
    }
}

CY_ISR(MAX30101_ISR)
{
    Connection_LED_Write(!Connection_LED_Read());
    MAX30101_INT_ClearInterrupt();
    flag_temp = 1;
}

/* [] END OF FILE */
