#include <zephyr.h>
#include <stdio.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <sys/__assert.h>
#include <string.h>

#ifndef __HAL_ULTRASONIC_H
#define __HAL_ULTRASONIC_H

void hal_init_ultrasonic();
uint8_t hal_ping_ultrasonic();

#endif