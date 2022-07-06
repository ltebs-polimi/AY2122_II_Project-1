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

void USER(uint8 x)
{
    switch (x) {
        
        //ADC range
        case 'a':
        ADC_range = MAX30101_ADC_RANGE_2048;
        flag_ADC = 1;
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
        Samples = MAX30101_SAMPLE_RATE_50;
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
        
        //pulse amplitudes
        case 'o':
        Pulse_amp = 0x01;
        flag_PA = 1;
        break;
        
        case 'p':
        Pulse_amp = 0x03;
        flag_PA = 1;
        break;
        
        case 'q':
        Pulse_amp = 0x05;
        flag_PA = 1;
        break;
        
        case 'r':
        Pulse_amp = 0x07;
        flag_PA = 1;
        break;
        
        case 's':
        Pulse_amp = 0x09;
        flag_PA = 1;
        break;
        
        case 't':
        Pulse_amp = 0x0B;
        flag_PA = 1;
        break;
        
        case 'u':
        Pulse_amp = 0x0D;
        flag_PA = 1;
        break;
        
        case 'v':
        Pulse_amp = 0x0F;
        flag_PA = 1;
        break;
        
        case 'z':
        Pulse_amp = 0x11;
        flag_PA = 1;
        break;
        
        case 'A':
        Pulse_amp = 0x13;
        flag_PA = 1;
        break;
        
        case 'B':
        Pulse_amp = 0x15;
        flag_PA = 1;
        break;
        
        case 'C':
        Pulse_amp = 0x17;
        flag_PA = 1;
        break;
        
        case 'D':
        Pulse_amp = 0x19;
        flag_PA = 1;
        break;
        
        case 'E':
        Pulse_amp = 0x1B;
        flag_PA = 1;
        break;
        
        case 'F':
        Pulse_amp = 0x1D;
        flag_PA = 1;
        break;
        
        case 'G':
        Pulse_amp = 0x1F;
        flag_PA = 1;
        break;
    }
        
    
    /*if(x== '69') MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_69);
    if(x== '118') MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_118);
    if(x== '215') MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_215);
    if(x== '411') MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_411);
    
    if(x== '50') 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_50);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_1);
    }
    if(x=='100') 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_100);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_2);
    }
    if(x=='200') 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_200);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_4);
    }
    if(x=='400') //Sample Rate 400
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_400);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_8);
    }
    
    if(x=='0.2') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x01);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x01);
    }
    if(x=='0.6') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x03);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x03);
    }
    if(x=='1.0') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x05);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x05);
    }
    if(x=='1.4') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x07);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x07);
    }
    if(x=='1.8') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x09);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x09);
    }
    if(x=='2.2') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0B);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0B);
    }
    if(x=='2.6') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0D);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0D);
    }
    if(x=='3.0') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0F);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0F);
    }
    if(x=='3.4') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x11);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x11);
    }
    if(x=='3.8') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x13);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x13);
    }
    if(x=='4.2') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x15);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x15);
    }
    if(x=='4.6') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x17);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x17);
    }
    if(x=='5.0') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x19);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x19);
    }
    if(x=='5.4') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1B);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1B);
    }
    if(x=='5.8') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1D);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1D);
    }
    if(x=='6.2') 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1F);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1F);
    }
    */



    //LED_PW: 69 118 215 411
    //SAMPLES: 50 100 200 400
    // CURRENT: da 0.2 a 6.2 con STEP 0.4
    // SPO2: 2048 4096 8192 16384
    
}