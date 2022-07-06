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

/* In codice Arduino...
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif */

#include "project.h"
#include <stdbool.h>

bool checkForBeat(int32 sample);
int16 averageDCEstimator(int32 *p, uint16 x);
int16 lowPassFIRFilter(int16 din);
int32 mul16(int16 x, int16 y);


/* [] END OF FILE */
