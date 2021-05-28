/**
****************************************************************
* @file myoslib/src/os_lsm6dsl.c
* @author Aiden Kearney - s4529666
* @date 28/03/2021
* @brief OS lsm6dsl driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* lsm6dsl_init      () - Initialises the values needed to read from the lsm6dsl.
* lsm6dsl_read_xyz  () - Reads the magnitude of acceleration for a given axis.
***************************************************************
*/

#include "os_i2c.h"
#include "os_lsm6dsl.h"
#include <drivers/i2c.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/printk.h>
#include <zephyr.h>

/**
 * @brief Helper that applies the arithmetic to raw lsm6dsl accelerometor values
 * to ms^-2
 *
 * @param data - array containing the low and high register values for a given
 * axis.
 * @return double - the accelerometor value for a given axis.
 */
double lsm6dsl_accel_convert(uint8_t data[2]) {
    double dval;
    int16_t raw_val = (data[0] | (data[1] << 8));
    dval = (double)(raw_val)*LSM6DSL_SENSITIVITY * SENSOR_G_DOUBLE / 1000;
    return dval;
}

/**
 * @brief Writes a bit into the lsm6dsl CTRL1_XL register to enable
 * accelerometor reading.
 *
 */
void lsm6dsl_init() {
    int ret;
    uint8_t data[1];

    data[0] = 0b01000000;
    ret = i2c_write_bytes(LSM6DSL_I2C_ADDR, i2c_dev, CTRL1_XL, &data[0], 1);
    if (ret) {
        printk(
            "Error writing to the CTRL1_XL register: error code (%d)\n", ret);
        return;
    }
}

/**
 * @brief returns the magnitude of acceleration on the axis defined in the
 * input.
 *
 * @param axis - The axis to read from.
 * @return double - The magnitude of acceleration.
 */
double lsm6dsl_read_xyz(char axis) {
    uint8_t data[2]; // Array used to store register values.
    int ret = 0;     // Used to detect read errors.

    /** Switch statement to decide which axis to read **/
    switch (axis) {
        case 'x':
            ret = i2c_read_bytes(
                LSM6DSL_I2C_ADDR, i2c_dev, OUTX_L_XL, data, sizeof(data));
            break;

        case 'y':
            ret = i2c_read_bytes(
                LSM6DSL_I2C_ADDR, i2c_dev, OUTY_L_XL, data, sizeof(data));
            break;

        case 'z':
            ret = i2c_read_bytes(
                LSM6DSL_I2C_ADDR, i2c_dev, OUTZ_L_XL, data, sizeof(data));
            break;

        default:
            break;
    }
    if (!ret) {
        return lsm6dsl_accel_convert(data);
    } else {
        printk("An error has occured reading the %c axis with the error %d\r\n",
            axis, ret);
        return (double)ret;
    }
}

/**
 * @brief Returns an array containing the register values for the x, y, and z
 * acceleration.
 *
 * @return uint8_t* - A 6 byte array containing the register values
 * corresponding to sensors.
 */
uint8_t *lsm6dsl_read_raw() {
    uint8_t data[6]; // Array used to store register values.
    uint8_t *sensorVals =
        k_malloc(sizeof(uint8_t) * 6); // Array used to store sensor values.

    /** Read all axis **/
    i2c_read_bytes(LSM6DSL_I2C_ADDR, i2c_dev, OUTX_L_XL, data, sizeof(data));

    memcpy(sensorVals, data, sizeof(uint8_t) * 6);

    return sensorVals;
}