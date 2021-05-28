/**
****************************************************************
* @file myoslib/src/os_lsm6dsl.h
* @author Aiden Kearney - s4529666
* @date 28/03/2021
* @brief OS lsm6dsl driver header.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* lsm6dsl_init      () - Initialises the values needed to read from the lsm6dsl.
* lsm6dsl_read_xyz  () - Reads the magnitude of acceleration for a given axis.
***************************************************************
*/

#ifndef OS_LSM6DSL_H
#define OS_LSM6DSL_H

#include "os_i2c.h"
#include <drivers/i2c.h>
#include <errno.h>
#include <stdio.h>
#include <sys/printk.h>
#include <zephyr.h>

/** Sensor Defines **/
#define LSM6DSL_I2C_ADDR    0b11010100
#define SENSOR_G            9806650LL
#define SENSOR_G_DOUBLE     (SENSOR_G / 1000000.0)
#define OUTX_L_XL           0x28
#define OUTY_L_XL           0x2A
#define OUTZ_L_XL           0x2C
#define CTRL1_XL            0x10
#define LSM6DSL_SENSITIVITY (61LL / 1000.0)

extern void lsm6dsl_init();

extern double lsm6dsl_read_xyz(char axis);

extern uint8_t *lsm6dsl_read_raw();

extern double lsm6dsl_accel_convert(uint8_t data[2]);

#endif