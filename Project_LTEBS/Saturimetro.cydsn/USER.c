#include "project.h"
#include "MAX30101.h"
#include "USER.h"
#include "parameters.h"
#include "setting_parameters.h"

#define UART_DEBUG

#ifdef UART_DEBUG
    
    #define DEBUG_TEST 1
    
#else
    
    #define DEBUG_TEST 0
    
#endif

#define debug_print(msg) do { if (DEBUG_TEST) UART_Debug_PutString(msg);} while (0)
#define FIFO_max_size 25

void (*print_ptr)(const char*) = &(UART_Debug_PutString);
extern uint8_t j;


//With this function we perform the communication between GUI and PSOC by sending a letter which represents each setting parameter
void USER(uint8 x)
{
    switch (x) {
        
        //ADC range
        case 'a':
        ADC_range = MAX30101_ADC_RANGE_2048;
        flag_ADC = 1; // the flag is activated in order to set the other parameters for the first time.
                      // When we change a single parameter we activate in the main the "settingParameters" function
                      // which takes as input all the 5 parameters. this flag is needed in order to set the value of the remaining 4 ones, otherwise
                      // the function considers other inputs equal to 0
        break;
        
        case 'b':
        ADC_range = MAX30101_ADC_RANGE_4096;
        flag_ADC = 1;
        break;
        
        case 'c':
        ADC_range = MAX30101_ADC_RANGE_8192;
        flag_ADC = 1;
        break;
        
        case 'd':
        ADC_range = MAX30101_ADC_RANGE_16384;
        flag_ADC = 1;
        break;
        
        
        //sample rate
        case 'e':
        Samples = MAX30101_SAMPLE_RATE_50; //In these 4 cases we modify the sample rate and the sample average accordingly to have a sampling frequency always equal to 50
        Average = MAX30101_SAMPLE_AVG_1;
        flag_SR = 1;
        break;
        
        case 'f':
        Samples = MAX30101_SAMPLE_RATE_100;
        Average = MAX30101_SAMPLE_AVG_2;
        flag_SR = 1;
        break;
        
        case 'g':
        Samples = MAX30101_SAMPLE_RATE_200;
        Average = MAX30101_SAMPLE_AVG_4;
        flag_SR = 1;
        break;
        
        case 'h':
        Samples = MAX30101_SAMPLE_RATE_400;
        Average = MAX30101_SAMPLE_AVG_8;
        flag_SR = 1;
        break;
        
        //pulse width
        case 'i':
        Pulse_width = MAX30101_PULSEWIDTH_69;
        flag_PW = 1;
        break;
        
        case 'l':
        Pulse_width = MAX30101_PULSEWIDTH_118;
        flag_PW = 1;
        break;
        
        case 'm':
        Pulse_width = MAX30101_PULSEWIDTH_215;
        flag_PW = 1;
        break;
        
        case 'n':
        Pulse_width = MAX30101_PULSEWIDTH_411;
        flag_PW = 1;
        break;
        
        //pulse amplitudes: from 0.2mA a 6.2mA with a STEP equal to 0.4
        case 'o':
        Pulse_amp = 0x01; //0.2
        flag_PA = 1;
        break;
        
        case 'p':
        Pulse_amp = 0x03; //0.6
        flag_PA = 1;
        break;
        
        case 'q':
        Pulse_amp = 0x05; //1
        flag_PA = 1;
        break;
        
        case 'r':
        Pulse_amp = 0x07; //1.4
        flag_PA = 1;
        break;
        
        case 's':
        Pulse_amp = 0x09; //1.8
        flag_PA = 1;
        break;
        
        case 't':
        Pulse_amp = 0x0B; //2.2
        flag_PA = 1;
        break;
        
        case 'u':
        Pulse_amp = 0x0D; //2.6
        flag_PA = 1;
        break;
        
        case 'v':
        Pulse_amp = 0x0F; //3
        flag_PA = 1;
        break;
        
        case 'z':
        Pulse_amp = 0x11; //3.4
        flag_PA = 1;
        break;
        
        case 'A':
        Pulse_amp = 0x13; //3.8
        flag_PA = 1;
        break;
        
        case 'B':
        Pulse_amp = 0x15;  //4.2
        flag_PA = 1;
        break;
        
        case 'C':
        Pulse_amp = 0x17; //4.6
        flag_PA = 1;
        break;
        
        case 'D':
        Pulse_amp = 0x19; //5
        flag_PA = 1;
        break;
        
        case 'E':
        Pulse_amp = 0x1B; //5.4
        flag_PA = 1;
        break;
        
        case 'F':
        Pulse_amp = 0x1D; //5.8
        flag_PA = 1;
        break;
        
        case 'G':
        Pulse_amp = 0x1F; //6.2
        flag_PA = 1;
        break;
    }
        
}    
