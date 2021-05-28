/**
****************************************************************
* @file myoslib/src/os_hci
* @author Aiden Kearney - s4529666
* @date 6/04/2021
* @brief hardware command interface OS interactions driver header.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* i2c_reg_cmd      () - Command handler for i2creg command.
* lsm6dsl_read_cmd () - Command handler for lsm6dsl command.
***************************************************************
*/

#ifndef __OS_HCI_H
#define __OS_HCI_H

#include <device.h>
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/__assert.h>
#include <sys/printk.h>
#include <zephyr.h>

extern void i2c_reg_cmd(
    const struct shell *shell, size_t argc, char **argv, void *data);

extern void lsm6dsl_read_cmd(
    const struct shell *shell, size_t argc, char **argv, void *data);

#endif