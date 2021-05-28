/**
****************************************************************
* @file myoslib/src/os_i2c.h
* @author Aiden Kearney - s4529666
* @date 28/03/2021
* @brief OS i2c driver header.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* i2c_write_bytes () - Writes bites to an i2c device and register.
* i2c_read_bytes  () - Reads bites from an i2c device and register.
* i2c_init        () - initialises the i2c driver for zephyr.
***************************************************************
*/

#ifndef OS_I2C_H
#define OS_I2C_H

#include <drivers/i2c.h>
#include <errno.h>
#include <stdio.h>
#include <sys/printk.h>
#include <zephyr.h>

extern int i2c_write_bytes(uint8_t device_addr, const struct device *i2c_dev,
    uint8_t addr, uint8_t *data, uint32_t num_bytes);

extern int i2c_read_bytes(uint8_t device_addr, const struct device *i2c_dev,
    uint8_t addr, uint8_t *data, uint32_t num_bytes);

extern void i2c_init();

extern const struct device *i2c_dev;

#endif