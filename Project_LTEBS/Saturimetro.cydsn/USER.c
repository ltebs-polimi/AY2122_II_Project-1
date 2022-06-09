#include "project.h"
#include "MAX30101.h"
    float x;

void USER(void)
{
    if(x==69) MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_69);
    if(x==118) MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_118);
    if(x==215) MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_215);
    if(x==411) MAX30101_SetSpO2PulseWidth(MAX30101_PULSEWIDTH_411);
    
    if(x==50) 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_50);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_1);
    }
    if(x==100) 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_100);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_2);
    }
    if(x==200) 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_200);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_4);
    }
    if(x==400) 
    {
        MAX30101_SetSpO2SampleRate(MAX30101_SAMPLE_RATE_400);
        MAX30101_SetSampleAverage(MAX30101_SAMPLE_AVG_8);
    }
    
    if(x==0.2) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x01);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x01);
    }
    if(x==0.6) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x03);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x03);
    }
    if(x==1.0) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x05);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x05);
    }
    if(x==1.4) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x07);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x07);
    }
    if(x==1.8) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x09);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x09);
    }
    if(x==2.2) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0B);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0B);
    }
    if(x==2.6) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0D);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0D);
    }
    if(x==3.0) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x0F);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x0F);
    }
    if(x==3.4) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x11);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x11);
    }
    if(x==3.8) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x13);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x13);
    }
    if(x==4.2) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x15);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x15);
    }
    if(x==4.6) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x17);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x17);
    }
    if(x==5.0) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x19);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x19);
    }
    if(x==5.4) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1B);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1B);
    }
    if(x==5.8) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1D);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1D);
    }
    if(x==6.2) 
    {
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_1, 0x1F);
        MAX30101_SetLEDPulseAmplitude(MAX30101_LED_2, 0x1F);
    }
    
    if(x==2048) MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_2048);
    if(x==4096) MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_4096);
    if(x==8192) MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_8192);
    if(x==16384) MAX30101_SetSpO2ADCRange(MAX30101_ADC_RANGE_16384);


    //LED_PW 69 118 215 411
    //SAMPLES 50 100 200 400
    // CURRENT 0.2 A 6.2 STEP 0.4
    // SPO2 2048 4096 8192 16384
    
}