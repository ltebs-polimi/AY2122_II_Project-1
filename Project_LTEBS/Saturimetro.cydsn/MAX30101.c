/**
*   Source file for the MAX30101 library.
*/

#include "I2C_Interface.h"
#include "I2C_Master.h"
#include "MAX30101.h"
#include "string.h"
#include "stdio.h"

//==============================================
//          MACROS
//==============================================

/**
*   \brief Mask for Power Ready interrupt.
*/
#define MAX30101_INT_PWR_RDY_MASK   0xFE

/**
*   \brief Mask for FIFO A FULL interrupt.
*/
#define MAX30101_INT_FIFO_A_FULL_MASK   0x7F

/**
*   \brief Enable FIFO A FULL interrupt.
*/
#define MAX30101_INT_FIFO_A_FULL_ENABLE 0x80

/**
*   \brief Disable FIFO A FULL interrupt.
*/
#define MAX30101_INT_FIFO_A_FULL_DISABLE 0x00

/**
*   \brief Mask for PPG ready interrupt.
*/
#define MAX30101_INT_PPG_RDY_MASK     0xBF

/**
*   \brief Enable PPG ready interrupt.
*/
#define MAX30101_INT_PPG_RDY_ENABLE 0x40

/**
*   \brief Disable PPG ready interrupt.
*/
#define MAX30101_INT_PPG_RDY_DISABLE 0x00

/**
*   \brief Mask for FIFO overflow interrupt.
*/
#define MAX30101_INT_ALC_OVF_MASK     0xDF

/**
*   \brief Enable OVERFLOW interrupt.
*/
#define MAX30101_INT_ALC_OVF_ENABLE 0x20

/**
*   \brief Disable OVERFLOW interrupt.
*/
#define MAX30101_INT_ALC_OVF_DISABLE 0x00

/**
*   \brief Mask for temperature data ready interrupt.
*/
#define MAX30101_INT_TMP_RDY_MASK     0xFD

/**
*   \brief Enable temperature ready interrupt.
*/
#define MAX30101_INT_TMP_RDY_ENABLE 0x02

/**
*   \brief Disable temperature ready interrupt.
*/
#define MAX30101_INT_TMP_RDY_DISABLE 0x00

/**
*   \brief Mask for sample average settings.
*/
#define MAX30101_SMP_AVG_MASK    0x1F

/**
*   \brief Mask for fifo rollover.
*/
#define MAX30101_FIFO_ROLLOVER_MASK    0xEF

/**
*   \brief Mask for sample average settings.
*/
#define MAX30101_FIFO_ROLLOVER_ENABLE    0x10

/**
*   \brief Mask for sample average settings.
*/
#define MAX30101_FIFO_ROLLOVER_DISABLE    0x00

/**
*   \brief Mask for FIFO A Full samples.
*/
#define MAX30101_FIFO_A_FULL_MASK   0xF0

/**
*   \brief Mask for shutdown bit.
*/
#define MAX30101_SHUTDOWN_MASK 0x7F

/**
*   \brief Enable shutdown
*/
#define MAX30101_SHUTDOWN_ENABLE 0x80

/**
*   \brief Disable shutdown.
*/
#define MAX30101_SHUTDOWN_DISABLE 0x00

/**
*   \brief Reset bit mask.
*/
#define MAX30101_RESET_MASK 0xBF

/**
*   \brief Set reset bit.
*/
#define MAX30101_RESET_ENABLE 0x40

/**
*   \brief Mode mask.
*/
#define MAX30101_MODE_MASK 0xF8

/**
*   \brief SPO2 ADC Range mask.
*/
#define MAX30101_SPO2_ADC_RANGE_MASK 0x9F 

/**
*   \brief SPO2 Sample Rate mask.
*/
#define MAX30101_SPO2_SAMPLE_RATE_MASK 0xE3

/**
*   \brief SPO2 Sample Rate mask.
*/
#define MAX30101_SPO2_PULSEWIDTH_MASK 0xFC

/**
*   \brief Multi-LED SLOT1 mask.
*/
#define MAX30101_SLOT1_MASK  		0xF8

/**
*   \brief Multi-LED SLOT2 mask.
*/
#define MAX30101_SLOT2_MASK  		0x8F

/**
*   \brief Multi-LED SLOT3 mask.
*/
#define MAX30101_SLOT3_MASK  		0xF8

/**
*   \brief Multi-LED SLOT4 mask.
*/
#define MAX30101_SLOT4_MASK  		0x8F

#define MAX30101_SHIFT(resolution) 4-resolution

//==============================================
//          FUNCTION PROTOTYPESS
//==============================================

static uint8_t MAX30101_BitMask(uint8_t reg_addr, uint8_t mask, uint8_t thing);

static uint8_t MAX30101_WriteRegister(uint8_t reg_addr, uint8_t reg_data);


// Start the device
uint8_t MAX30101_Start(void)
{
    I2C_Peripheral_Start();
    return MAX30101_Reset();
}

// Check if device is present on I2C bus
uint8_t MAX30101_IsDevicePresent(void)
{
    if (I2C_Peripheral_IsDeviceConnected(MAX30101_I2C_ADDRESS) == I2C_NO_ERROR)
    {
        return MAX30101_OK;
    }
    
    return MAX30101_DEV_NOT_FOUND;
}

//==============================================
//          INTERRUPT RELATED FUNCTIONS
//==============================================

// Check interrupt status
uint8_t MAX30101_IsFIFOAFull(uint8_t* flag)
{
    // Read register
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_INT_ST_1, &temp);
    *flag = temp & (~MAX30101_INT_FIFO_A_FULL_MASK);
    return error;
    
}

uint8_t MAX30101_IsPPGReady(uint8_t* flag)
{
    // Read register
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_INT_ST_1, &temp);
    *flag = temp & (~MAX30101_INT_PPG_RDY_MASK);
    return error;
}
uint8_t MAX30101_IsALCOverflow(uint8_t* flag)
{
    // Read register
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_INT_ST_1, &temp);
    *flag = temp & (~MAX30101_INT_ALC_OVF_MASK);
    return error;
}
uint8_t MAX30101_IsPowerReady(uint8_t* flag)
{
    // Read register
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_INT_ST_1, &temp);
    *flag = temp & (~MAX30101_INT_PWR_RDY_MASK);
    return error;
}
uint8_t MAX30101_IsTempReady(uint8_t* flag)
{
    // Read register
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_INT_ST_2, &temp);
    *flag = temp & (~MAX30101_INT_TMP_RDY_MASK);
    return error;
}
    
// Enable interrupts
uint8_t MAX30101_EnableFIFOAFullInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_FIFO_A_FULL_MASK, MAX30101_INT_FIFO_A_FULL_ENABLE);
}
uint8_t MAX30101_EnablePPGReadyInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_INT_PPG_RDY_MASK, MAX30101_INT_PPG_RDY_ENABLE);
}
uint8_t MAX30101_EnableALCOverflowInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_INT_ALC_OVF_MASK, MAX30101_INT_ALC_OVF_ENABLE);
}

uint8_t MAX30101_EnableTempReadyInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_2, MAX30101_INT_TMP_RDY_MASK, MAX30101_INT_TMP_RDY_ENABLE);
}

// Disable interrupts
uint8_t MAX30101_DisableFIFOAFullInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_FIFO_A_FULL_MASK, MAX30101_INT_FIFO_A_FULL_DISABLE);
}

uint8_t MAX30101_DisablePPGReadyInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_INT_PPG_RDY_MASK, MAX30101_INT_PPG_RDY_DISABLE);
}

uint8_t MAX30101_DisableALCOverflowInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_INT_ALC_OVF_MASK, MAX30101_INT_ALC_OVF_DISABLE);
}

uint8_t MAX30101_DisableTempReadyInt(void)
{
    return MAX30101_BitMask(MAX30101_INT_EN_1, MAX30101_INT_TMP_RDY_MASK, MAX30101_INT_TMP_RDY_DISABLE);
}

//==============================================
//          FIFO FUNCTIONS
//==============================================
// Read Write pointer
uint8_t MAX30101_ReadWritePointer(uint8_t* wr)
{
    return MAX30101_ReadRegister(MAX30101_FIFO_WP, wr);
}

// Read Overflow counter
uint8_t MAX30101_ReadOverflowCounter(uint8_t* oc)
{
    return MAX30101_ReadRegister(MAX30101_FIFO_OVF_CNT, oc);
}

// Read Read Pointer
uint8_t MAX30101_ReadReadPointer(uint8_t* rr)
{
    return MAX30101_ReadRegister(MAX30101_FIFO_RP, rr);
}

// Clear FIFO
uint8_t MAX30101_ClearFIFO(void)
{
    uint8_t error = MAX30101_WriteRegister(MAX30101_FIFO_WP, 0x00);
    if ( error == MAX30101_OK)
    {
        error = MAX30101_WriteRegister(MAX30101_FIFO_RP, 0x00);
        if ( error == MAX30101_OK)
        {
            // Read current mode to determine number of active leds
            uint8_t temp_value;
            error = MAX30101_ReadRegister(MAX30101_MODE_CONF, &temp_value);
            uint8_t current_mode = temp_value & 0x07;
            uint8_t active_leds;
            switch(current_mode)
            {
                case MAX30101_HR_MODE:
                    active_leds = 1;
                    break;
                case MAX30101_SPO2_MODE:
                    active_leds = 2;
                    break;
                case MAX30101_MULTI_MODE:
                    active_leds = 3;
                    break;
            }
            uint8_t fifo_values[active_leds*3];
            // Read 1 from FIFO to clear the overflow counter
            error = MAX30101_ReadRawFIFOBytes(1, active_leds, fifo_values);
        }
    }
    return error;
}

uint8_t MAX30101_ReadRawFIFOBytes(uint8_t num_samples, uint8_t active_leds, uint8_t* data)
{
    // We need to read a number of bytes equal to num_samples + 3 * active_leds
    uint16_t bytes_left_ro_read = num_samples * 3 * active_leds;
    if (I2C_Peripheral_ReadRegisterMulti(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA, bytes_left_ro_read, data) == I2C_NO_ERROR)
    {
        return MAX30101_OK;
    }
    else
    {
        return MAX30101_DEV_NOT_FOUND;
    }
}

// Read FIFO Data
uint8_t MAX30101_ReadRawFIFO(uint8_t num_samples, uint8_t active_leds, uint32_t* data)
{
    // We need to read a number of bytes equal to num_samples + 3 * active_leds
    uint16_t bytes_left_ro_read = num_samples * 3 * active_leds;
    uint8_t error;
    //uint8_t error = I2C_Peripheral_WriteRegisterNoData(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA);
    //if ( error == I2C_NO_ERROR)
    //{
        uint8_t sample_counter = 0;
        while(bytes_left_ro_read > 0)
        {
            uint16_t bytes_to_get =  active_leds * 3;
            
            bytes_left_ro_read -= bytes_to_get;
            
            uint8_t temp[sizeof(uint32_t)];
            uint8_t temp_bytes[3];
            uint32_t tempLong;
            
            error = I2C_Peripheral_ReadRegisterMulti(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA, 3, temp_bytes);
            
            //Burst read three bytes - RED
            temp[3] = 0;
            temp[2] = temp_bytes[0];
            temp[1] = temp_bytes[1];
            temp[0] = temp_bytes[2];

            //Convert array to long
            memcpy(&tempLong, temp, sizeof(tempLong));
    		
            //Zero out all but 18 bits
    		tempLong &= 0x3FFFF; 
            data[sample_counter] = tempLong;
            
            if (active_leds > 1)
            {
                I2C_Peripheral_ReadRegisterMulti(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA, 3, temp_bytes);
               
                //Burst read three bytes - IR
                temp[3] = 0;
                temp[2] = temp_bytes[0];
                temp[1] = temp_bytes[1];
                temp[0] = temp_bytes[2];
            
                //Convert array to long
                memcpy(&tempLong, temp, sizeof(tempLong));
        		
                //Zero out all but 18 bits
        		tempLong &= 0x3FFFF; 
                sample_counter += 1;
                data[sample_counter] = tempLong;
            }
            if (active_leds > 2)
            {
                I2C_Peripheral_ReadRegisterMulti(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA,3, temp_bytes);
                
                //Burst read three bytes - GREEN
                temp[3] = 0;
                temp[2] = temp_bytes[0];
                temp[1] = temp_bytes[1];
                temp[0] = temp_bytes[2];

                //Convert array to long
                memcpy(&tempLong, temp, sizeof(tempLong));
        		
                //Zero out all but 18 bits
        		tempLong &= 0x3FFFF; 
                sample_counter += 1;
                data[sample_counter] = tempLong;
            }
            sample_counter += 1;
            
        }
    //}
    return error;
}

// Read FIFO Data
uint8_t MAX30101_ReadFIFO(uint8_t num_samples, uint8_t active_leds, MAX30101_Data* data)
{
    // We need to read a number of bytes equal to num_samples + 3 * active_leds
    uint16_t bytes_left_ro_read = num_samples * 3 * active_leds;
    uint8_t error = MAX30101_OK;
    
    // Read current resolution so that we know how much shift to apply
    uint8_t resolution = 0;
    I2C_Peripheral_ReadRegister(MAX30101_I2C_ADDRESS, MAX30101_SPO2_CONF, &resolution);
    resolution &= (~MAX30101_SPO2_PULSEWIDTH_MASK);
    
    I2C_Master_MasterSendStart(MAX30101_I2C_ADDRESS, I2C_Master_WRITE_XFER_MODE);
    I2C_Master_MasterWriteByte(MAX30101_FIFO_DATA);
    I2C_Master_MasterSendRestart(MAX30101_I2C_ADDRESS, I2C_Master_READ_XFER_MODE);
    
    //I2C_Peripheral_WriteRegisterNoData(MAX30101_I2C_ADDRESS, MAX30101_FIFO_DATA);
    //I2C_Peripheral_StartReadNoAddress(MAX30101_I2C_ADDRESS);
    
    while(bytes_left_ro_read > 0)
    {
        uint16_t bytes_to_get =  active_leds * 3;
        
        bytes_left_ro_read -= bytes_to_get;
        
        data->head++; //Advance the head of the storage struct
        data->head %= BUFFER_STORAGE_SIZE; //Wrap condition
        
        uint8_t temp[sizeof(uint32_t)];
        uint32_t tempLong;
        
        //error = I2C_Peripheral_ReadBytes(temp_bytes, 3);
        
        //Burst read three bytes - RED
        temp[3] = 0;
        temp[2] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
        temp[1] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
        temp[0] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);

        //Convert array to long
        memcpy(&tempLong, temp, sizeof(tempLong));
		
        //Zero out all but 18 bits
		tempLong &= 0x3FFFF; 
        
        // Shift according to resolution
        tempLong = tempLong >> (MAX30101_SHIFT(resolution));
        
        data->red[data->head] = tempLong; //Store this reading into the sense array
        
        if (active_leds > 1)
        {
            //error = I2C_Peripheral_ReadBytes(temp_bytes, 3);
           
            //Burst read three bytes - IR
            temp[3] = 0;
            temp[2] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
            temp[1] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
            temp[0] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
        
            //Convert array to long
            memcpy(&tempLong, temp, sizeof(tempLong));
    		
            //Zero out all but 18 bits
    		tempLong &= 0x3FFFF;
            tempLong = tempLong >> (MAX30101_SHIFT(resolution));
            
            data->IR[data->head] = tempLong; //Store this reading into the sense array
        }
        if (active_leds > 2)
        {
            //error = I2C_Peripheral_ReadBytes(temp_bytes, 3);
            
            //Burst read three bytes - GREEN
            temp[3] = 0;
            temp[2] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
            temp[1] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);
            temp[0] = I2C_Master_MasterReadByte(I2C_Master_ACK_DATA);

            //Convert array to long
            memcpy(&tempLong, temp, sizeof(tempLong));
    		
            //Zero out all but 18 bits
    		tempLong &= 0x3FFFF; 
            
            tempLong = tempLong >> (MAX30101_SHIFT(resolution));
            
            data->green[data->head] = tempLong; //Store this reading into the sense array
        }
    }
    
    I2C_Master_MasterSendStop();
    return error;
}

//==============================================
//    MAX30101 FIFO CONFIGURATION FUNCTIONS
//==============================================

/// Set number of averaged samples
uint8_t MAX30101_SetSampleAverage(uint8_t samples)
{
    return MAX30101_BitMask(MAX30101_FIFO_CONF, MAX30101_SMP_AVG_MASK, samples);
}

// Enable FIFO rollover
uint8_t MAX30101_EnableFIFORollover(void)
{
    return MAX30101_BitMask(MAX30101_FIFO_CONF, MAX30101_FIFO_ROLLOVER_MASK, MAX30101_FIFO_ROLLOVER_ENABLE);
}

// Disable FIFO rollover
uint8_t MAX30101_DisableFIFORollover(void)
{
    return MAX30101_BitMask(MAX30101_FIFO_CONF, MAX30101_FIFO_ROLLOVER_MASK, MAX30101_FIFO_ROLLOVER_DISABLE);
}

// Set number of samples for FIFO Almost Full
uint8_t MAX30101_SetFIFOAlmostFull(uint8_t samples)
{
    return MAX30101_BitMask(MAX30101_FIFO_CONF, MAX30101_FIFO_A_FULL_MASK, 32-samples);
}

//==============================================
//     MAX30101 MODE CONFIGURATION FUNCTIONS
//==============================================

// Shutdown the MAX30101
uint8_t MAX30101_Shutdown(void)
{
    return MAX30101_BitMask(MAX30101_MODE_CONF, MAX30101_SHUTDOWN_MASK, MAX30101_SHUTDOWN_ENABLE);
}

// Wake Up the MAX30101
uint8_t MAX30101_WakeUp(void)
{
    return MAX30101_BitMask(MAX30101_MODE_CONF, MAX30101_SHUTDOWN_MASK, MAX30101_SHUTDOWN_DISABLE);    
}

// Reset the MAX30101
uint8_t MAX30101_Reset(void)
{
    return MAX30101_BitMask(MAX30101_MODE_CONF, MAX30101_RESET_MASK, MAX30101_RESET_ENABLE);    
}

// Set current mode of operation
uint8_t MAX30101_SetMode(uint8_t mode)
{
    return MAX30101_BitMask(MAX30101_MODE_CONF, MAX30101_MODE_MASK, mode);
}

//==============================================
//     MAX30101 SPO2 CONFIGURATION FUNCTIONS
//==============================================
// Set SpO2 ADC Range
uint8_t MAX30101_SetSpO2ADCRange(uint8_t range)
{
    return MAX30101_BitMask(MAX30101_SPO2_CONF, MAX30101_SPO2_ADC_RANGE_MASK, range);
}

// Set SpO2 Sample Rate
uint8_t MAX30101_SetSpO2SampleRate(uint8_t sr)
{
    return MAX30101_BitMask(MAX30101_SPO2_CONF, MAX30101_SPO2_SAMPLE_RATE_MASK, sr);
}

// Set SpO2 Pulse Widht
uint8_t MAX30101_SetSpO2PulseWidth(uint8_t pw)
{
    return MAX30101_BitMask(MAX30101_SPO2_CONF, MAX30101_SPO2_PULSEWIDTH_MASK, pw);
}

// Set pulse amplitude for a channel
uint8_t MAX30101_SetLEDPulseAmplitude(uint8_t led_channel, uint8_t pa)
{
    // Read register
    uint8_t error = MAX30101_OK;
    uint8_t reg_data;
    error = I2C_Peripheral_ReadRegister(MAX30101_I2C_ADDRESS, MAX30101_LED1_PA + led_channel, &reg_data);
    if (error == I2C_NO_ERROR)
    {
        reg_data |= pa;
        if(I2C_Peripheral_WriteRegister(MAX30101_I2C_ADDRESS, MAX30101_LED1_PA + led_channel, reg_data) != I2C_NO_ERROR)
        {
          error = MAX30101_ERROR;
        }
    }
    else
    {
        error = MAX30101_DEV_NOT_FOUND;
    }
    return error;
}

//======================================================
//    MAX30101 MULTI LED MODE CONFIGURATION FUNCTIONS
//======================================================
// Enable given slot
uint8_t MAX30101_EnableSlot(uint8_t slot, uint8_t led)
{
    switch(slot)
    {
        case 1:
            return MAX30101_BitMask(MAX30101_MULTI_LED_1, MAX30101_SLOT1_MASK, led);
            break;
        case 2:
            return MAX30101_BitMask(MAX30101_MULTI_LED_1, MAX30101_SLOT2_MASK, led << 4);
            break;
        case 3:
            return MAX30101_BitMask(MAX30101_MULTI_LED_2, MAX30101_SLOT3_MASK, led);
            break;
        case 4:
            return MAX30101_BitMask(MAX30101_MULTI_LED_2, MAX30101_SLOT4_MASK, led << 4);
            break;
        default:
            return MAX30101_ERROR;
            break;
    } 
}

// Disable all slots configurations
uint8_t MAX30101_DisableSlots(void)
{
    uint8_t error = MAX30101_WriteRegister(MAX30101_MULTI_LED_1, 0x00);
    if ( error == MAX30101_OK)
    {    
        error = MAX30101_WriteRegister(MAX30101_MULTI_LED_2, 0x00);
    }
    
    return error;
}

//======================================================
//            MAX30101 DIE TEMPERATURE FUNCTIONS
//======================================================
uint8_t MAX30101_ReadTemperature(float* temperature)
{
    uint8_t integer, frac = 0;
    uint8_t error = MAX30101_ReadRegister(MAX30101_TEMP_INT, &integer);
    if ( error == MAX30101_OK)
    {
        error = MAX30101_ReadRegister(MAX30101_TEMP_FRACT, &frac);
        if ( error == MAX30101_OK)
        {
            *temperature = ((float)(integer)) + frac * 0.0625;
        }
    }
    
    return error;
}

uint8_t MAX30101_ReadRawTemperature(int8_t* integer, uint8_t* frac)
{
    uint8_t temp;
    uint8_t error = MAX30101_ReadRegister(MAX30101_TEMP_INT, &temp);
    if ( error == MAX30101_OK)
    {
        *integer = (int8_t)temp;
        error = MAX30101_ReadRegister(MAX30101_TEMP_FRACT, frac);
    }
    
    return error;
}

uint8_t MAX30101_StartTemperatureConversion(void)
{
    return MAX30101_WriteRegister(MAX30101_TEMP_CONF, 0x01);
}

//======================================================
//            MAX30101 PART/REVISION ID FUNCTIONS
//======================================================
// Read part ID number
uint8_t MAX30101_ReadPartID(uint8_t* part_id)
{
    return MAX30101_ReadRegister(MAX30101_PART_ID, part_id);
    
}

// Read revision ID number
uint8_t MAX30101_ReadRevisionID(uint8_t* revision_id)
{
    return MAX30101_ReadRegister(MAX30101_REVISION_ID, revision_id);
}

//======================================================
//            MAX30101 HELPER FUNCTIONS
//======================================================
// Simple helper function to read a register from the MAX30101
uint8_t MAX30101_ReadRegister(uint8_t reg_addr, uint8_t* reg_value)
{
    uint8_t error = MAX30101_OK;
    uint8_t reg_data;
    if(I2C_Peripheral_ReadRegister(MAX30101_I2C_ADDRESS, reg_addr, &reg_data) == I2C_NO_ERROR)
    {
        *reg_value = reg_data;
    }
    else
    {
        error = MAX30101_DEV_NOT_FOUND;
    }
    return error;
}

// Log all registers
uint8_t MAX30101_LogRegisters(void (*print_fun)(const char*))
{
    uint8_t reg_list[] = {MAX30101_INT_ST_1,
                                MAX30101_INT_ST_2,
                                MAX30101_INT_EN_1,
                                MAX30101_INT_EN_2,
                                MAX30101_FIFO_WP,
                                MAX30101_FIFO_OVF_CNT,
                                MAX30101_FIFO_RP,
                                MAX30101_FIFO_DATA,
                                MAX30101_FIFO_CONF,
                                MAX30101_MODE_CONF,
                                MAX30101_SPO2_CONF,
                                MAX30101_LED1_PA,
                                MAX30101_LED2_PA,
                                MAX30101_LED3_PA,
                                MAX30101_LED4_PA,
                                MAX30101_MULTI_LED_1,
                                MAX30101_MULTI_LED_2,
                                MAX30101_TEMP_INT,
                                MAX30101_TEMP_FRACT,
                                MAX30101_TEMP_CONF,
                                MAX30101_REVISION_ID,
                                MAX30101_PART_ID};
    uint8_t error = MAX30101_OK;
    for (uint8_t i = 0; i < 22; i++)
    {
        error = MAX30101_PrintRegister(print_fun, reg_list[i]);
        if (error != MAX30101_OK)
            break;
    }
    return error;
}

uint8_t MAX30101_PrintRegister(void (*print_fun)(const char*), uint8_t reg_addr)
{
    uint8_t value;
    uint8_t error = MAX30101_ReadRegister(reg_addr, &value);
    if (error == MAX30101_OK)
    {
        char msg[20];
        sprintf(msg, "[0x%02X] - 0x%02X\r\n", reg_addr, value);
        print_fun(msg);
    }
    return error;
}

// Simple helper function to write a register to the MAX30101
static uint8_t MAX30101_WriteRegister(uint8_t reg_addr, uint8_t reg_data)
{
    uint8_t error = MAX30101_OK;
    if(I2C_Peripheral_WriteRegister(MAX30101_I2C_ADDRESS, reg_addr, reg_data) != I2C_NO_ERROR)
    {
        error = MAX30101_DEV_NOT_FOUND;
    }
    return error;
}

// Simple helper function to perform bit mask operations
static uint8_t MAX30101_BitMask(uint8_t reg_addr, uint8_t mask, uint8_t thing)
{
    uint8_t reg_data;
    uint8_t error = MAX30101_ReadRegister(reg_addr, &reg_data);
    if (error == MAX30101_OK)
    {
        reg_data = reg_data & mask;
        error = MAX30101_WriteRegister(reg_addr, reg_data | thing);
    }
    return error;
}

/* [] END OF FILE */
