/**
*   \file MAX30101_Defs.h
*   
*   \brif Defines for MAX30101 Library.
*/

#ifndef __MAX30101_DEFS_H__
    #define __MAX30101_DEFS_H__
    
    #include "cytypes.h"
    
    //==============================================
    //              MAX30101 ERRORS
    //==============================================
    /**
    *   \brief I2C Address of MAX30101.
    */
    #define MAX30101_I2C_ADDRESS 0x57
    
    /**
    *   \brief Error condition triggered when device is not found on I2C bus.
    */
    #define MAX30101_DEV_NOT_FOUND 1
    
    /**
    *   \brief No error generated.
    */
    #define MAX30101_OK 0
    
    /**
    *   \brief Generic error condition related to MAX30101.
    */
    #define MAX30101_ERROR 2
    
    //==============================================
    //  MAX30101 MACROS FOR REGISTERS CONFIGURATION
    //==============================================
    
    /**
    *   \brief MAX30101 Heart Rate mode.
    */
    #define MAX30101_HR_MODE 0x02
    
    /**
    *   \brief MAX30101 SpO2 mode. mode.
    */
    #define MAX30101_SPO2_MODE 0x03

    /**
    *   \brief MAX30101 Multi LED mode.
    */
    #define MAX30101_MULTI_MODE 0x07
    
    /**
    *   \brief MAX30101 LED channel 1 (RED).
    */
    #define MAX30101_LED_1 0x00
    
    /**
    *   \brief MAX30101 LED channel 2 (IR).
    */
    #define MAX30101_LED_2 0x01
    
    /**
    *   \brief MAX30101 LED channel 3 (GREEN).
    */
    #define MAX30101_LED_3 0x02
    
    /**
    *   \brief MAX30101 LED channel 4 (GREEN).
    */
    #define MAX30101_LED_4 0x03
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_1 0x00
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_2 0x20
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_4 0x40
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_8 0x60
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_16 0x80
    
    /**
    *   \brief MAX30101 no average in FIFO.
    */
    #define MAX30101_SAMPLE_AVG_32 0xA0
    
    /**
    *   \brief MAX30101 SpO2 range set to 2048 nA.
    */
    #define MAX30101_ADC_RANGE_2048  0x00
    
    /**
    *   \brief MAX30101 SpO2 range set to 4096 nA.
    */
    #define MAX30101_ADC_RANGE_4096  0x20
    
    /**
    *   \brief MAX30101 SpO2 range set to 8192 nA.
    */
    #define MAX30101_ADC_RANGE_8192  0x40
    
    /**
    *   \brief MAX30101 SpO2 range set to 16384 nA.
    */
    #define MAX30101_ADC_RANGE_16384 0x60
    
    /**
    *   \brief MAX30101 SpO2 sample rate set to 50 samples.
    */
    #define MAX30101_SAMPLE_RATE_50 0x00
    
    /**
    *   \brief MAX30101 SpO2 sample rate set to 100 samples.
    */
    #define MAX30101_SAMPLE_RATE_100 0x04
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 200 samples.
    */
    #define MAX30101_SAMPLE_RATE_200 0x08
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 400 samples.
    */
    #define MAX30101_SAMPLE_RATE_400 0x0C
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 800 samples.
    */
    #define MAX30101_SAMPLE_RATE_800 0x10
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 1000 samples.
    */
    #define MAX30101_SAMPLE_RATE_1000 0x14
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 1600 samples.
    */
    #define MAX30101_SAMPLE_RATE_1600 0x18
    
    /**
    *   \brief MAX30101 SpO2 sample rate set tot 3200 samples.
    */
    #define MAX30101_SAMPLE_RATE_3200 0x1C
    
    /**
    *   \brief MAX30101 Pulse Width set to 69.
    */
    #define MAX30101_PULSEWIDTH_69	    0x00
    
    /**
    *   \brief MAX30101 Pulse Width set to 118.
    */
    #define MAX30101_PULSEWIDTH_118  	0x01
    
    /**
    *   \brief MAX30101 Pulse Width set to 215.
    */
    #define MAX30101_PULSEWIDTH_215  	0x02
    
    /**
    *   \brief MAX30101 Pulse Width set to 411.
    */
    #define MAX30101_PULSEWIDTH_411  	0x03
    
    /**
    *   \brief MAX30101 Slot RED enabled.
    */
    #define MAX30101_SLOT_RED   0x01
    
    /**
    *   \brief MAX30101 Slot IR enabled.
    */
    #define MAX30101_SLOT_IR    0x02
    
    /**
    *   \brief MAX30101 Slot GREEN enabled.
    */
    #define MAX30101_SLOT_GREEN 0x03
    
    /**
    *   \brief MAX30101 No Slot enabled.
    */
    #define MAX30101_SLOT_NONE  0x00
    
    //==============================================
    //==============================================
    //              I2C REGISTERS
    //==============================================
    //==============================================
    
    //==============================================
    //     MAX30101 INTERRUPT STATUTS I2C REGISTERS
    //==============================================
    
    /**
    *   \brief Interrupt Status 1 register.
    *
    *   Whenever an interrupt is triggered, the MAX30101 pulls the active-low
    *   interrupt pin into its low state until the interrupt is cleared. This
    *   register contains the status of the interrupts.
    *
    *   The register is structured as follows:
    *   |   B7   |   B6   |    B5  |   B4   |   B3   |   B2   |   B1   |   B0   |
    *   | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | 
    *   | A_FULL | PPG_RDY | ALC_OVF | | | | |PWR_RDY |
    *
    *   **A_FULL: FIFO Almost Full Flag**
    *
    *   In SpO2 and HR modes, this interrupt triggers when the FIFO write pointer 
    *   has a certain number of free spaces remaining. The trigger number can be set by the 
    *   #MAX30101_FIFO_CONF register. The interrupt is cleared by reading the #MAX30101_INT_ST_1 register.
    *
    *   **PPG_RDY: New FIFO Data Ready**
    *
    *   In SpO2 and HR modes, this interrupt triggers when there is a new sample in the data FIFO. 
    *   The interrupt is cleared by reading the #MAX30101_INT_ST_1, or by reading the #MAX30101_FIFO_DATA register.
    *
    *   **ALC_OVF: Ambient Light Cancellation Overflow**
    *
    *   This interrupt triggers when the ambient light cancellation function 
    *   of the SpO2/HR photodiode has reached its maximum limit,
    *   and therefore, ambient light is affecting the output of the ADC.
    *   The interrupt is cleared by reading the #MAX30101_INT_ST_1.
    *
    *   **PWR_RDY: Power Ready Flag**
    *
    *   On power-up or after a brownout condition, when the supply voltage VDD transitions
    *   from below the undervoltage lockout (UVLO) voltage to above the UVLO voltage, a 
    *   power-ready interrupt is triggered to signal that the module is powered-up and ready to collect data.
    */
    #define MAX30101_INT_ST_1 0x00
    
    /**
    *   \brief Interrupt Status 2 register.
    *
    *   Whenever an interrupt is triggered, the MAX30101 pulls the active-low
    *   interrupt pin into its low state until the interrupt is cleared. This
    *   register contains the status of the interrupts.
    
    *   The register is structured as follows:
    *   |   B7   |   B6   |    B5  |   B4   |   B3   |   B2   |   B1   |   B0   |
    *   | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | 
    *   |        |         |         | | | | DIE_TMP_RDY | |
    *
    *   **DIE_TEMP_RDY: Internal Temperature Ready Flag**
    *
    *   When an internal die temperature conversion is finished, 
    *   this interrupt is triggered so the processor can read the temperature data registers. 
    *   The interrupt is cleared by reading either the #MAX30101_INT_ST_2 or the #MAX3031_TFRAC register.
    */
    #define MAX30101_INT_ST_2 0x01
    
    //==============================================
    //   MAX30101 INTERRUPT ENABLE I2C REGISTERS
    //==============================================
    /**
    *   \brief Interrupt Enable 1 register.
    *
    *   Each source of hardware interrupt, with the exception of power ready, can be disabled in a 
    *   software register within the MAX30101 IC. The power-ready interrupt cannot be disabled 
    *   because the digital state of the module is reset upon a brownout condition (low power supply voltage),
    *   and the default condition is that all the interrupts are disabled. 
    *   Also, it is important for the system to know that a brownout condition has occurred, 
    *   and the data within the module is reset as a result.
    *   The unused bits should always be set to zero for normal operation.
    *
    *   The register is structured as follows:
    *   |   B7   |   B6   |    B5  |   B4   |   B3   |   B2   |   B1   |   B0   |
    *   | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | 
    *   | A_FULL_EN | PPG_RDY_EN | ALC_OVF_EN | | | | | |
    *
    */
    #define MAX30101_INT_EN_1 0x02
    
    /**
    *   \brief Interrupt Enable 2 register.
    *
    *   Each source of hardware interrupt, with the exception of power ready, can be disabled in a 
    *   software register within the MAX30101 IC. The power-ready interrupt cannot be disabled 
    *   because the digital state of the module is reset upon a brownout condition (low power supply voltage),
    *   and the default condition is that all the interrupts are disabled. 
    *   Also, it is important for the system to know that a brownout condition has occurred, 
    *   and the data within the module is reset as a result.
    *   The unused bits should always be set to zero for normal operation.
    *
    *   The register is structured as follows:
    *   |   B7   |   B6   |    B5  |   B4   |   B3   |   B2   |   B1   |   B0   |
    *   | ------ | ------ | ------ | ------ | ------ | ------ | ------ | ------ | 
    *   |        |         |         | | | | DIE_TMP_RDY_EN | |
    */
    #define MAX30101_INT_EN_2 0x03
    
    //==============================================
    //        MAX30101 FIFO I2C REGISTERS
    //==============================================
    /**
    *   \brief FIFO write pointer register.
    *
    *   The FIFO Write Pointer points to the location where the MAX30101 writes the next sample. 
    *   This pointer advances for each sample pushed on to the FIFO.
    *   It can also be changed through the I2C interface when MODE[2:0] in #MAX30101_MODE_CONF is 010, 011, or 111.
    *
    *   The register is structured as follows:
        <table>
        <caption id="fifo_wp">MAX30101 FIFO Write Pointer Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td>-<td>-<td colspan=5, style="text-align:center">FIFO_WR_PTR[4:0]
        </table>
    */
    #define MAX30101_FIFO_WP 0x04
    
    /**
    *   \brief FIFO overflow counter register.
    *
    *   When the FIFO is full, samples are not pushed on to the FIFO, samples are lost. 
    *   OVF_COUNTER counts the number of samples lost. It saturates at 0x1F.
    *   When a complete sample is “popped” (i.e., removal of old FIFO data and shifting the samples down) 
    *   from the FIFO (when the read pointer advances), OVF_COUNTER is reset to zero.
    *
    *   The register is structured as follows:
        <table>
        <caption id="fifo_ovf">MAX30101 FIFO Overflow Counter Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td>-<td>-<td colspan=5, style="text-align:center">OVF_COUNTER[4:0]
        </table>
    */
    #define MAX30101_FIFO_OVF_CNT 0x05
    
    /**
    *   \brief FIFO read pointer register.
    *
    *   The FIFO Read Pointer points to the location from where the processor gets the next sample
    *   from the FIFO through the I2C interface. This advances each time a sample is popped from
    *   the FIFO. The processor can also write to this pointer after reading the samples 
    *   to allow rereading samples from the FIFO if there is a data communication error.
    *
    *   The register is structured as follows:
        <table>
        <caption id="fifo_rp">MAX30101 FIFO Read Pointer Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td>-<td>-<td colspan=5, style="text-align:center">FIFO_RD_PTR[4:0]
        </table>
    */
    #define MAX30101_FIFO_RP 0x06
    
    /**
    *   \brief FIFO data register.
    *
    *   The circular FIFO depth is 32 and can hold up to 32 samples of data.
    *   The sample size depends on the number of LED channels (a.k.a. channels) configured as active. 
    *   As each channel signal is stored as a 3-byte data signal, the FIFO width can be 3 bytes, 6 bytes, 9 bytes, 
    *   or 12 bytes in size. The #MAX30101_FIFO_DATA register in the I2C register map points to the next sample to be read 
    *   from the FIFO. #MAX30101_FIFO_RP points to this sample. Reading #MAX30101_FIFO_DATA register, 
    *   does not automatically increment the I2C register address. Burst reading this register, 
    *   reads the same address over and over.
    *   Each sample is 3 bytes of data per channel (i.e., 3 bytes for RED, 3 bytes for IR, etc.). 
    *   The FIFO registers (0x04–0x07) can all be written and read, but in practice only the #MAX30101_FIFO_RP
    *   register should be written to in operation. The others are automatically incremented or filled with data 
    *   by the MAX30101. When starting a new SpO2 or heart rate conversion, it is recommended to first clear 
    *   the #MAX30101_FIFO_WP, #MAX30101_OVF_CNT, and #MAX30101_FIFO_RP registers to all zeroes (0x00) 
    *   to ensure the FIFO is empty and in a known state. When reading the MAX30101 registers in one
    *   burst-read I2C transaction, the register address pointer typically increments so that the next byte of
    *   data sent is from the next register, etc. The exception to this is the #MAX30101_FIFO_DATA data register,
    *   register 0x07. When reading this register, the address pointer does not increment, but the #MAX30101_FIFO_RP does.
    *   So the next byte of data sent represents the next byte of data available in the FIFO.
    *
    *   The register is structured as follows:
        <table>
        <caption id="fifo_data">MAX30101 FIFO Data Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td colspan=8, style="text-align:center">FIFO_DATA[7:0]
        </table>
    */
    #define MAX30101_FIFO_DATA 0x07
    
    //==============================================
    //   MAX30101 CONFIGURATION I2C REGISTERS
    //==============================================
    
    /**
    *   \brief FIFO Configuration register.
    *   
    *   This register allows to configure the FIFO behaviour.
    
    *   It is structured as follows:
        <table>
        <caption id="fifo_conf">MAX30101 FIFO Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td colspan=3, style="text-align:center">SMP_AVG[2:0]<td> FIFO_ROL_LOVER_EN<td colspan=4, style="text-align:center">FIFO_A_FULL[3:0]
        </table>
    *
    *   **Bits 7:5: Sample Averaging (SMP_AVE)**
    *
    *   To reduce the amount of data throughput, adjacent samples (in each individual channel)
    *   can be averaged and decimated on the chip by setting this register.
    *
    *   | SMP_AVG[2:0] | NO. OF SAMPLES AVG PER SAMPLE |
    *   |:------------:|:-----------------------------:|
    *   |   000        |  1 (no averaging)             |
    *   |   001        |                 2             |
    *   |   010        |                 4             |
    *   |   011        |                 8             |
    *   |   100        |                16             |
    *   |   101        |                32             |
    *   |   110        |                32             |
    *   |   111        |                32             |
    *
    *   **Bit 4: FIFO Rolls on Full (FIFO_ROLLOVER_EN)**
    *
    *   This bit controls the behavior of the FIFO when the FIFO becomes
    *   completely filled with data. If FIFO_ROLLOVER_EN is set (1), 
    *   the FIFO Address rolls over to zero and the FIFO continues to fill 
    *   with new data. If the bit is not set (0), then the FIFO is not updated 
    *   until FIFO_DATA is read or the WRITE/READ pointer positions are changed.
    *   
    *   **Bits 3:0: FIFO Almost Full Value (FIFO_A_FULL)**
    *   
    *   This register sets the number of data samples (3 bytes/sample) remaining in the FIFO when the interrupt is issued. 
    *   For example, if this field is set to 0x0, the interrupt is issued when there is 0 data samples remaining in the 
    *   FIFO (all 32 FIFO words have unread data). Furthermore, if this field is set to 0xF, the interrupt is issued when
    *   15 data samples are remaining in the FIFO (17 FIFO data samples have unread data).
    *   <table>
        <caption id="fifo_a_full">MAX30101 FIFO Almost Full Settings</caption>
        <tr><th>FIFO_A_FULL[3:0]<th>EMPTY DATA SAMPLES WHEN<br\>INTERRUPT IS ISSUED       <th>UNREAD DATA SAMPLES IN FIFO<br\>WHEN INTERRUPT IS ISSUED
        <tr><td>0x0h<td>0<td>32
        <tr><td>0x1h<td>1<td>31
        <tr><td>0x2h<td>2<td>30
        <tr><td>0x3h<td>3<td>29
        <tr><td>..<td>..<td>..
        <tr><td>0xFh<td>15 <td> 17
        </table>
    */
    #define MAX30101_FIFO_CONF 0x08
    
    /**
    *   \brief MAX30101 Mode configuration register.
    *
    *   This register is structured as follows:
    *
        <table>
        <caption id="fifo_conf">MAX30101 FIFO Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>SHDN<td>RESET<td>-<td>-<td>-<td colspan=3, style="text-align:center">MODE[2:0]
        </table>
    *
    *   **Bit 7: Shutdown Control (SHDN)**
    *
    *   The part can be put into a power-save mode by setting this bit to one. 
    *   While in power-save mode, all registers retain their values, and write/read operations function as normal.
    *   All interrupts are cleared to zero in this mode.
    *
    *   **Bit 6: Reset Control (RESET)**
    *   
    *   When the RESET bit is set to one, all configuration, threshold, and data registers are reset
    *   to their power-on-state through a power-on reset. The RESET bit is cleared automatically back 
    *   to zero after the reset sequence is completed. 
    *   Note: Setting the RESET bit does not trigger a PWR_RDY interrupt event.
    *
    *   **Bits 2:0: Mode Control**
    *
    *   These bits set the operating state of the MAX30101. Changing modes does not change any other setting,
    *   nor does it erase any previously stored data inside the data registers.
    *
       <table>
        <caption id="mode_table">MAX30101 Mode settings</caption>
        <tr><th>MODE[2:0]                      <th>MODE       <th>ACTIVE LED CHANNELS
        <tr><td>000<td colspan=2, style="text-align:center">Do not use
        <tr><td>001<td colspan=2, style="text-align:center">Do not use
        <tr><td>010<td> Heart Rate mode <td> Red only
        <tr><td>011<td> SpO2 mode <td> Red and IR
        <tr><td>100-110<td colspan=2, style="text-align:center">Do not use
        <tr><td>111<td> Multi-LED mode <td> Green, RED, and/or IR
        </table>
    */
    #define MAX30101_MODE_CONF 0x09
    
    /**
    *   \brief Sp02 Configuration register.
    *
    *   This register configures the sensor for SpO2 mode.
    *
    *   It is structured as follows:
        <table>
        <caption id="spo2_conf">MAX30101 SpO2 Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0                      
        <tr><td>-<td colspan=2, style="text-align:center">SPO2_ADC_RGE[1:0]<td colspan=3, style="text-align:center">SPO2_SR[2:0]<td colspan=2, style="text-align:center"> LED_PW[1:0]
        </table>
    *
    *   **Bits 6:5: SpO2 ADC Range Control**
    *   This register sets the SpO2 sensor ADC’s full-scale range:
        <table>
        <caption id="spo2_adc_rge">MAX30101 SPo2 ADC Range Settings</caption>
        <tr><th>SPO2_ADC_RGE[1:0]<th>LSB SIZE (pA)<th>FULL SCALE (nA)
        <tr><td>00<td>7.81<td>2048
        <tr><td>01<td>15.63<td>4096
        <tr><td>02<td>31.25<td>8192
        <tr><td>03<td>62.5<td>16384
        </table>
    *
    *   **Bits 4:2: SpO2 Sample Rate Control**
    *
    *   These bits define the effective sampling rate with one sample consisting
    *   of one IR pulse/conversion, one RED pulse/ conversion, and one GREEN 
    *   pulse/conversion. The sample rate and pulse-width are related in that 
    *   the sample rate sets an upper bound on the pulse-width time.
    *   If the user selects a sample rate that is too high for the selected 
    *   LED_PW setting, the highest possible sample rate is programmed instead into the register.
    *
        <table>
        <caption id="spo2_sr">MAX30101 SPo2 Sample Rate Settings</caption>
        <tr><th>SPO2_SR[2:0]<th>SAMPLES PER SECOND
        <tr><td>000<td>50
        <tr><td>001<td>100
        <tr><td>010<td>200
        <tr><td>011<td>400
        <tr><td>101<td>800
        <tr><td>110<td>1600
        <tr><td>111<td>3200
        </table>
    *
    *   **Bits 1:0: LED Pulse Width Control and ADC Resolution**
    *   These bits set the LED pulse width (the IR, Red, and Green have the same pulse width), 
    *   and, therefore, indirectly sets the integration time of the ADC in each sample. 
    *   The ADC resolution is directly related to the integration time.
    *
        <table>
        <caption id="led_pw">MAX30101 LED Pulse Width Settings</caption>
        <tr><th>LED_PW[1:0]<th>PULSE WIDTH (uS) <th> ADC RESOLUTION (bits)
        <tr><td>00<td>69 (68.95) <td>15
        <tr><td>01<td>118 (117.78) <td> 16
        <tr><td>10<td>215 (215.44) <td> 17
        <tr><td>11<td>411 (410.75) <td> 18
        </table>
    */
    #define MAX30101_SPO2_CONF 0x0A
    
    /**
    *   \brief MAX30101 LED1 Pulse Amplitude register.
    *
    *   This register allows to configure the pulse amplitude for LED1.
    *   The register is structured as follows:
        <table>
        <caption id="led_pa">MAX30101 FIFO Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td colspan=8, style="text-align:center">LED1_PA[7:0]
        </table>
    *
    *   The pulse amplitude is determined by the followint table:
    *
        <table>
        <caption id="led_pa">MAX30101 LED Pulse Amplitude Settings</caption>
        <tr><th>LEDx_PA[7:0]<th>TYPICAL LED CURRENT (mA)
        <tr><td>0x00h<td>0.0
        <tr><td>0x01h<td>0.2
        <tr><td>0x02h<td>0.4
        <tr><td>...<td>...
        <tr><td>0x0fh<td>3.0
        <tr><td>...<td>...
        <tr><td>0x1fh<td>6.2
        <tr><td>...<td>...
        <tr><td>0x3fh<td>12.6
        <tr><td>...<td>...
        <tr><td>0x7fh<td>25.4
        <tr><td>...<td>...
        <tr><td>0xFFh<td>51.0
        </table>
    */
    #define MAX30101_LED1_PA 0x0C
    
    
    /**
    *   \brief MAX30101 LED2 Pulse Amplitude register.
    *
    *   Please refer to \ref led_pa for the 
    *   configuration of this register.
    */
    #define MAX30101_LED2_PA 0x0D
    
    /**
    *   \brief MAX30101 LED3 Pulse Amplitude register.
    *   
    *   Please refer to \ref led_pa for the 
    *   configuration of this register.
    */
    #define MAX30101_LED3_PA 0x0E
    
    /**
    *   \brief MAX30101 LED4 Pulse Amplitude register.
    *
    *   Please refer to \ref led_pa for the 
    *   configuration of this register.
    */
    #define MAX30101_LED4_PA 0x0F
    
    /**
    *   \brief MAX30101 Multi-LED Mode Configuration register - 1.
    *
    *   In multi-LED mode, each sample is split into up to four time slots, 
    *   SLOT1 through SLOT4. #MAX30101_MULTI_LED_1 and #MAX30101_MULTI_LED_2
    *   control registers determine which LED is active in each time slot, 
    *   making for a very flexible configuration. Each slot generates a 3-byte
    *   output into the FIFO. One sample comprises all active slots, for example
    *   if SLOT1 and SLOT2 are non-zero, then one sample is 2 x 3 = 6 bytes. 
    *   If SLOT1 through SLOT3 are all non-zero, then one sample is 3 x 3 = 9 bytes.
    *   The slots should be enabled in order (i.e., SLOT1 should not be disabled if SLOT2 
    *   or SLOT3 are enabled). 
    *   Both LED3 and LED4 are wired to Green LED. Green LED sinks current out 
    *   of #MAX30101_LED3_PA and #MAX30101_LED4_PA configuration in Multi-LED Mode 
    *   and SLOTx[2:0] = 011.
    *
    *   The register is structured as follows:
    *
    *   <table>
        <caption id="multi_led_1">MAX30101 FIFO Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td colspan=3, style='text-align:center'>SLOT2[2:0]<td>-<td colspan=3, style="text-align:center">SLOT1[2:0]
        </table>
    *
    *   The settings for #MAX30101_MULTI_LED_1 and #MAX30101_MULTI_LED_2 registers
    *   allows to setup the following Multi-LED modes:
        <table>
        <caption id="multi_mode">MAX30101 Multi-LED Mode Configuration</caption>
        <tr><th>SLOTx[2:0] Setting <th>WHICH LED IS ACTIVE <th>LED PULSE AMPLITUDE SETTING
        <tr><td>000<td>None (time slot is disabled) <td>N/A (Off)
        <tr><td>001<td>LED1 (RED) <td> #MAX30101_LED1_PA
        <tr><td>010<td>LED2 (IR) <td>#MAX30101_LED2_PA
        <tr><td rowspan=2>011*<td>LED3 (GREEN) <td>#MAX30101_LED3_PA[7:0]
        <tr><td>LED4 (GREEN) <td>#MAX30101_LED4_PA[7:0]
        <tr><td>100<td>None <td>N/A (Off)
        <tr><td>101<td>RESERVED<td>RESERVED
        <tr><td>110<td>RESERVED<td>RESERVED
        <tr><td>111<td>RESERVED<td>RESERVED
        </table>
    *
    */
    #define MAX30101_MULTI_LED_1 0x11
    
    /**
    *   \brief MAX30101 Multi-LED Mode Configuration register - 2.
    *
    *   Please refer to #MAX30101_MULTI_LED_1 for the explanation of the register.
    *   The register is structured as follows:
    *
    *   <table>
        <caption id="multi_led_2">MAX30101 FIFO Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td colspan=3, style='text-align:center'>SLOT2[2:0]<td>-<td colspan=3, style="text-align:center">SLOT1[2:0]
        </table>
    */
    #define MAX30101_MULTI_LED_2 0x12
    
    //==============================================
    //        MAX30101 TEMPERATURE REGISTERS
    //==============================================
    
    /**
    *   \brief MAX30101 Temperature Integer register.
    *
    *   The on-board temperature ADC output is split into two registers, 
    *   one to store the integer temperature and one to store the fraction. *
    *   Both should be read when reading the temperature data, 
    *   and the equation below shows how to add the two registers together:
    *   \f$T_{measured} = T_{integer} + T_{fraction}\f$.
    *
    *   This register stores the integer temperature data in 2’s complement format, 
    *   where each bit corresponds to 1°C.
    *
    *   <table>
        <caption id="temp_int">MAX30101 Temperature Integer Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td colspan=8, style="text-align:center">TINT[7:0]
        </table>

    *
        <table>
        <caption id="temp_int">MAX30101 Temperature Values</caption>
        <tr><th>REGISTER VALUE (hex)<th> TEMPERATURE (°C)
        <tr><td>0x00<td>0
        <tr><td>0x01<td>+1
        <tr><td>...<td>...
        <tr><td>0x7e<td>+126
        <tr><td>0x7f<td>+127
        <tr><td>0x80<td>-128
        <tr><td>0x81<td>-127
        <tr><td>...<td>...
        <tr><td>0xFE<td>-2
        <tr><td>0xFF<td>-1
        </table>
    */
    #define MAX30101_TEMP_INT   0x1F
    
    /**
    *   \brief MAX30101 Temperature Fraction register.
    *
    *   This register stores the fractional temperature data in increments of 0.0625°C.
    *   If this fractional temperature is paired with a negative integer,
    *   it still adds as a positive fractional value (e.g., -128°C + 0.5°C = -127.5°C).
    *   This register is structured as follows:
    *
        <table>
        <caption id="temp_fract">MAX30101 Temperature Fraction Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td>-<td>-<td>-<td colspan=4, style="text-align:center">TFRAC[3:0]
        </table>

    *
    */
    #define MAX30101_TEMP_FRACT 0x20
    
    /**
    *   \brief MAX30101 Temperature Configuration register..
    *
    *   This register has only one bit which is used to trigger 
    *   temperature measurements. This is a self-clearing bit which, when set, 
    *   initiates a single temperature reading from the temperature sensor. 
    *   This bit clears automatically back to zero at the conclusion of the 
    *   temperature reading when the bit is set to one.
    *   This register is structured as follows:
    *
        <table>
        <caption id="temp_en">MAX30101 Temperature Configuration Register</caption>
        <tr><th>B7<th>B6<th>B5<th>B4<th>B3<th>B2<th>B1<th>B0
        <tr><td>-<td>-<td>-<td>-<td>-<td>-<td>-<td>TEMP_EN
        </table>
    *
    */
    #define MAX30101_TEMP_CONF    0x21
    
    //==============================================
    //        MAX30101 PART ID REGISTERS
    //==============================================
    
    /**
    *   \brief MAX30101 Revision ID register.
    *   
    *   This register holds the value for the revision ID number
    *   of the MAX30101.
    */
    #define MAX30101_REVISION_ID    0xfe
    
    /**
    *   \brief MAX30101 Part ID.
    *   
    *   This register holds the value for the revision ID number
    *   of the MAX30101: the expected value to be read is 0x15.
    */
    #define MAX30101_PART_ID    0xff
#endif

/* [] END OF FILE */
