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
#include "HeartRate.h"
#include "Timer.h"
#include "isr.h"
#include "USER.h"


#define SLEEPTIMER_INTERVAL_COUNTER (768u)
#define UART_DEBUG
#define FIFO_max_size 25

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)

CY_ISR_PROTO(MAX30101_ISR);
//CY_ISR_PROTO(WAKEUP_TIMER);
    

uint8_t flag_temp = 0;
uint8_t flag_1s = 0;
volatile int gotInterrupt = 0;
uint8_t j=0;

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
    int32_t somma=0;
    int32_t avg_hr=0;
    int num_samples;
    
    
    extern volatile long count;
    extern volatile uint8 SM;
    extern volatile uint8 flag_SM;   
    int i=0;
    int f=1;

    
    //SET BY THE USER

    
    // Initialization
    MAX30101_Start();
    UART_Debug_Start();
    CyDelay(100);    
    if (MAX30101_IsDevicePresent() == MAX30101_OK)
    {
        // Check if device is present
        Connection_LED_Write(1);
        
        // Read revision and part id
        uint8_t rev_id, part_id = 0;
        MAX30101_ReadPartID(&part_id);
        MAX30101_ReadRevisionID(&rev_id);
        
        debug_print("Registers before configuration\r\n");
        MAX30101_LogRegisters(print_ptr);
        
        // Soft reset sensor
        MAX30101_Reset();
        CyDelay(100);
       
        // Wake up sensor
        MAX30101_WakeUp();        
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
        
        // Set Sample Rate
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_100);
        
        // Set mode
        MAX30101_SetMode(MAX30101_SPO2_MODE);
        
        // Enable Slots
        MAX30101_DisableSlots();
        
        debug_print("Registers after configuration\r\n");
        MAX30101_LogRegisters(print_ptr);
    }
    
    
    isr_MAX30101_StartEx(MAX30101_ISR);
    //isr_Timer_StartEx(WAKEUP_TIMER);
    //isr_SM_StartEx(Count);
    isr_RX_StartEx(Custom_ISR_RX);
    //SleepTimer_Start(); 
    
    // Clear FIFO
    MAX30101_ClearFIFO();
    
    CyGlobalIntEnable; /* Enable global interrupts. */
      
    for(;;)
    {       
        /*int32 irValue=getIR(&data);  
        if(irValue<10000) 
        {
           flag_SM=1;
            
            if(SM==1)
            {
                flag_SM=0;
                //debug_print("SLEEP_MODE\r\n");
                CyDelay(50);
                CyPmSaveClocks();
                CyPmAltAct(PM_ALT_ACT_TIME_NONE,PM_ALT_ACT_SRC_CTW);
                CyPmRestoreClocks();
                count=0;
            }
        }               
        else if(irValue>=10000)
        { 
            SM=0;
            flag_SM=0;
            count=0;
        }*/
        
        
        while(gotInterrupt == 0)
        {
            MAX30101_IsFIFOAFull(&flag);
            if(flag>0)
            {
                MAX30101_ReadReadPointer(&rp);
                MAX30101_ReadWritePointer(&wp);
                //Calculate the number of readings we need to get from sensor
                num_samples = wp - rp;
                if (num_samples <= 0) num_samples += 32; //Wrap condition                
                // Read FIFO
                MAX30101_ReadFIFO(num_samples, active_leds, &data, j);
                if(!gotInterrupt)
                {
                    for (i=0;i<num_samples;i++)
                    {                    
                        redBuffer[j] = data.red[data.tail+i];
                        irBuffer[j] = data.IR[data.tail+i];
                        j++;
                        debug_print("1,");
                        sprintf(msg, "%ld,", data.IR[data.tail+i]);
                        debug_print(msg);
                        debug_print("2,");
                        sprintf(msg, "%ld,", data.red[data.tail+i]);
                        debug_print(msg);
                        sprintf(msg, "%d,", j);
                        debug_print(msg);
                        
                        if(j<bufferLength)
                        {
                            debug_print("0,");
                            debug_print("0,");
                            debug_print("0,");
                            debug_print("0\n");
                        }
                    }    
                }
                if(j>=bufferLength) 
                {
                    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
                    j=bufferLength - 50;
                    for (int i=50; i<bufferLength; i++) 
                    {
                        redBuffer[i-50] = redBuffer[i];
                        irBuffer[i-50] = irBuffer[i];
                    }
                    debug_print("3,");
                    sprintf(msg, "%ld,", spo2);
                    debug_print(msg);
                    debug_print("4,");
                    sprintf(msg, "%ld\n", avg_hr);
                    debug_print(msg);
                    if(heartRate > 50 && heartRate < 160) 
                    {
                        somma += heartRate;
                        if(f==3)
                        {
                            somma = somma/3;
                            //debug_print("4,");
                            //sprintf(msg, "%ld\n", somma);
                            //debug_print(msg);
                            f = 0;
                            avg_hr = somma;
                            somma = 0;
                            
                        }
                        f++;
                    }
                }
            }
            gotInterrupt = 0;
        }
    } 
}   

/*CY_ISR(WAKEUP_TIMER)
{
    SleepTimer_GetStatus();
}*/

CY_ISR(MAX30101_ISR)
{
    Connection_LED_Write(!Connection_LED_Read());
    MAX30101_INT_ClearInterrupt();
}

