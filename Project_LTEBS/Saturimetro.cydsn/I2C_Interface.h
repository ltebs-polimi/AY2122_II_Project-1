/** 
 * \file I2C_Interface.h
 * \brief Hardware specific I2C interface.
 *
 * This is an interface to the I2C peripheral. If you need to port 
 * this C-code to another platform, you could simply replace this
 * interface and still use the code.
 *
 * \author Davide Marzorati
 * \date September 12, 2019
*/

#ifndef I2C_Interface_H
    #define I2C_Interface_H
    
    #include "cytypes.h"
    
    /**
    *   \brief No error generated during I2C transaction.
    */
    #define I2C_NO_ERROR 0
    
    /**
    *   \brief Error condition for device not found on I2C bus.
    */
    #define I2C_DEV_NOT_FOUND 1
    
    /**
    *   \brief Generic error condition for I2C communication.
    */
    #define I2C_ERROR 2
    
    /** \brief Start the I2C peripheral.
    *   
    *   This function starts the I2C peripheral so that it is ready to work.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   1retval #I2C_ERROR if peripheral could not be started.
    */
    uint8_t I2C_Peripheral_Start(void);
    
    /** \brief Stop the I2C peripheral.
    *   
    *   This function stops the I2C peripheral from working.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   1retval #I2C_ERROR if peripheral could not be stopped.
    */
    uint8_t I2C_Peripheral_Stop(void);
    

    /** \brief Stop the I2C peripheral.
    *   
    *   This function stops the I2C peripheral from working.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   1retval #I2C_ERROR if peripheral could not be stopped.
    */
    uint8_t I2C_Peripheral_SendStop(void);
    
    /**
    *   \brief Read one byte over I2C.
    *   
    *   This function performs a complete reading operation over I2C from a single
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be read.
    *   \param data Pointer to a variable where the byte will be saved.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                            uint8_t register_address,
                                            uint8_t* data);
    
    
    /** 
    *   \brief Read multiple bytes over I2C.
    *   
    *   This function performs a complete reading operation over I2C from multiple
    *   registers.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be read.
    *   \param register_count Number of registers we want to read.
    *   \param data Pointer to an array where data will be saved.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                                uint8_t register_address,
                                                uint16_t register_count,
                                                uint8_t* data);
    
    /** 
    *   \brief Read multiple bytes over I2C without specifying register address.
    *   
    *   This function performs a complete reading operation over I2C from multiple
    *   registers without specifying the register from which to read.
    *   \param[in] device_address I2C address of the device to talk to.
    *   \param[in] register_count Number of registers we want to read.
    *   \param[out] data Pointer to an array where data will be saved.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_ReadRegisterMultiNoAddress(uint8_t device_address,
                                                      uint16_t register_count, 
                                                      uint8_t* data);
    
    /** 
    *   \brief Start read transaction with a repeated start.
    *   
    *   This function starts a reading transaction by sending a
    *   repeated start.
    *   \param[in] device_address I2C address of the device to talk to.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_StartReadNoAddress(uint8_t device_address);
    
    /** 
    *   \brief Read bytes from I2C.
    *   
    *   This function reads a certain amount of bytes without sending
    *   any start/stop condition.
    *   \param[in] len number of bytes to read.
    *   \param[out] data pointer to array where data will be stored.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_ReadBytes(uint8_t* data, uint8_t len);
    
    /** 
    *   \brief Write a byte over I2C.
    *   
    *   This function performs a complete writing operation over I2C to a single 
    *   register.
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the register to be written.
    *   \param data Data to be written
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_WriteRegister(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t data);
    
    /** 
    *   \brief Write multiple bytes over I2C.
    *   
    *   This function performs a complete writing operation over I2C to multiple
    *   registers
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be written.
    *   \param register_count Number of registers that need to be written.
    *   \param data Array of data to be written
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_WriteRegisterMulti(uint8_t device_address,
                                            uint8_t register_address,
                                            uint8_t register_count,
                                            uint8_t* data);
    
    /** 
    *   \brief Write single byte over I2C.
    *   
    *   This function performs a complete writing operation over I2C to multiple
    *   registers
    *   \param device_address I2C address of the device to talk to.
    *   \param register_address Address of the first register to be written.
    *   \retval #I2C_NO_ERROR if no error was generated.
    *   \retval #I2C_DEV_NOT_FOUND if device didn't acknowledge start condition.
    *   \retval #I2C_ERROR for other error condition.
    */
    uint8_t I2C_Peripheral_WriteRegisterNoData(uint8_t device_address,
                                            uint8_t register_address);
    
    
    /**
    *   \brief Check if device is connected over I2C.
    *
    *   This function checks if a device is connected over the I2C lines.
    *   \param device_address I2C address of the device to be checked.
    *   \param connection pointer where the connection status will be stored
    *   \retval #I2C_NO_ERROR if device is present on the bus.
    *   \retval #I2C_DEV_NOT_FOUND if device is not present on the bus.
    */
    uint8_t I2C_Peripheral_IsDeviceConnected(uint8_t device_address);
    
#endif // I2C_Interface_H
/* [] END OF FILE */
