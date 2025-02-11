/*
 * i2c.h
 *
 *  Created on: Sep 25, 2020
 *      Author: Joe Krachey
 *      Author: Aksel Torgerson
 *      Author: Matthew Kesler
 */

#ifndef I2C_H_
#define I2C_H_

#include "msp.h"
#include <stdint.h>

/**********************************************************************************************
 * Initializes the EUSCI_B1 interface as an I2C bus
 **********************************************************************************************/
void i2c_init(void);

/**********************************************************************************************
 * Read 2 bytes of data from the I2C device.
 **********************************************************************************************/
uint16_t i2c_read_16(uint8_t slave_address, uint8_t dev_address);

/**********************************************************************************************
 * Write 2 bytes of data to the I2C device specified by slave_address
 **********************************************************************************************/
void i2c_write_16(uint8_t slave_address, uint8_t dev_address, uint16_t data);



#endif /* I2C_H_ */
