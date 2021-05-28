/**
****************************************************************
* @file myoslib/src/os_i2c.c
* @author Aiden Kearney - s4529666
* @date 28/03/2021
* @brief OS i2c driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* i2c_write_bytes () - Writes bites to an i2c device and register.
* i2c_read_bytes  () - Reads bites from an i2c device and register.
* i2c_init        () - initialises the i2c driver for zephyr.
***************************************************************
*/

#include "os_i2c.h"
#include <drivers/i2c.h>
#include <errno.h>
#include <stdio.h>
#include <sys/printk.h>
#include <zephyr.h>

const struct device *i2c_dev; // i2c device binding

/**
 * @brief Write bytes to the specified i2c device and registers.
 *
 * @param device_addr - the address of the device to write to.
 * @param i2c_dev - the zephyr device struct.
 * @param addr - the register address to write to.
 * @param data - the data to write to the register.
 * @param num_bytes - the number of bytes to write.
 * @return int - the success of the operation, 0 if successful, -EIO if
 * unsuccesful.
 */
int i2c_write_bytes(uint8_t device_addr, const struct device *i2c_dev,
    uint8_t addr, uint8_t *data, uint32_t num_bytes) {
    struct i2c_msg msgs[2];

    /* Send the address to write to */
    msgs[0].buf = &addr;
    msgs[0].len = 1U;
    msgs[0].flags = I2C_MSG_WRITE;

    /* Data to be written, and STOP after this. */
    msgs[1].buf = data;
    msgs[1].len = num_bytes;
    msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

    return i2c_transfer(i2c_dev, &msgs[0], 2, device_addr >> 1);
}

/**
 * @brief reads bytes from an i2c address/device.
 *
 * @param device_addr - The device address being read from.
 * @param i2c_dev - The zephyr device struct for i2c.
 * @param addr - the register address to read from.
 * @param data - array that data will be written to.
 * @param num_bytes - number of bytes to read.
 * @return int - the success of the operation, 0 if successful, -EIO if
 * unsuccesful.
 */
int i2c_read_bytes(uint8_t device_addr, const struct device *i2c_dev,
    uint8_t addr, uint8_t *data, uint32_t num_bytes) {
    struct i2c_msg msgs[2];

    /* Setup I2C messages */

    /* Send the address to read from */
    msgs[0].buf = &addr;
    msgs[0].len = 1U;
    msgs[0].flags = I2C_MSG_WRITE;

    /* Read from device. STOP after this. */
    msgs[1].buf = data;
    msgs[1].len = num_bytes;
    msgs[1].flags = I2C_MSG_RESTART | I2C_MSG_READ | I2C_MSG_STOP;

    return i2c_transfer(i2c_dev, &msgs[0], 2, device_addr >> 1);
}

/**
 * @brief attempts to bind the the i2c zephyr driver. Returns and throws an
 * error if not succesful.
 *
 */
void i2c_init() {
    i2c_dev = device_get_binding("I2C_2");
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }
}