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

#include "HeartRate.h"
#include "Timer.h"
#include "isr.h"
#include "project.h"
#include "MAX30101.h"

int16 IR_AC_Max = 20;
int16 IR_AC_Min = -20;

int16 IR_AC_Signal_Current = 0;
int16 IR_AC_Signal_Previous;
int16 IR_AC_Signal_min = 0;
int16 IR_AC_Signal_max = 0;
int16 IR_Average_Estimated;

int16 positiveEdge = 0;
int16 negativeEdge = 0;
int32 ir_avg_reg = 0;

int16 cbuf[32];
uint8 offset = 0;

static const uint16_t FIRCoeffs[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};

//  Heart Rate Monitor functions takes a sample value and the sample number
//  Returns true if a beat is detected
//  A running average of four samples is recommended for display on the screen.
bool checkForBeat(int32 sample)

{
  bool beatDetected = false;

  //  Save current state
  IR_AC_Signal_Previous = IR_AC_Signal_Current;
  
  //This is good to view for debugging
  //Serial.print("Signal_Current: ");
  //Serial.println(IR_AC_Signal_Current);

  //  Process next data sample
  IR_Average_Estimated = averageDCEstimator(&ir_avg_reg, sample);
  IR_AC_Signal_Current = lowPassFIRFilter(sample - IR_Average_Estimated);

  //  Detect positive zero crossing (rising edge)
  if ((IR_AC_Signal_Previous < 0) & (IR_AC_Signal_Current >= 0))
  {
  
    IR_AC_Max = IR_AC_Signal_max; //Adjust our AC max and min
    IR_AC_Min = IR_AC_Signal_min;

    positiveEdge = 1;
    negativeEdge = 0;
    IR_AC_Signal_max = 0;

    //if ((IR_AC_Max - IR_AC_Min) > 100 & (IR_AC_Max - IR_AC_Min) < 1000)
    if (((IR_AC_Max - IR_AC_Min) > 20) & ((IR_AC_Max - IR_AC_Min) < 1000))
    {
      //Heart beat!!!
      beatDetected = true;
    }
  }

  //  Detect negative zero crossing (falling edge)
  if ((IR_AC_Signal_Previous > 0) & (IR_AC_Signal_Current <= 0))
  {
    positiveEdge = 0;
    negativeEdge = 1;
    IR_AC_Signal_min = 0;
  }

  //  Find Maximum value in positive cycle
  if (positiveEdge & (IR_AC_Signal_Current > IR_AC_Signal_Previous))
  {
    IR_AC_Signal_max = IR_AC_Signal_Current;
  }

  //  Find Minimum value in negative cycle
  if (negativeEdge & (IR_AC_Signal_Current < IR_AC_Signal_Previous))
  {
    IR_AC_Signal_min = IR_AC_Signal_Current;
  }
  
  return(beatDetected);
}

//  Average DC Estimator
int16_t averageDCEstimator(int32 *p, uint16_t x)
{
  *p += ((((long) x << 15) - *p) >> 4);
  return (*p >> 15);
}

//  Low Pass FIR Filter
int16_t lowPassFIRFilter(int16 din)
{  
  cbuf[offset] = din;

  int32_t z = mul16(FIRCoeffs[11], cbuf[(offset - 11) & 0x1F]);
  
  for (uint8 i = 0 ; i < 11 ; i++)
  {
    z += mul16(FIRCoeffs[i], cbuf[(offset - i) & 0x1F] + cbuf[(offset - 22 + i) & 0x1F]);
  }

  offset++;
  offset %= 32; //Wrap condition

  return(z >> 15);
}

//  Integer multiplier
int32 mul16(int16 x, int16 y)
{
  return((long)x * (long)y);
}


/* [] END OF FILE */





void loop()
{
    
    
    long irValue = getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    //ogni ms incremento la variabile di 1.
    long delta = count - lastBeat;
    lastBeat = count;

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for ( x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  sprintf(msg, "IR=%ld", irValue);
  debug_print(msg);
  sprintf(msg, "BPM=%u", beatsPerMinute);
  debug_print(msg);
  sprintf(msg, "Avg BPM=%ld", beatAvg);
  debug_print(msg);
  

  if (irValue < 5000)
    debug_print("no finger");
}
*/
