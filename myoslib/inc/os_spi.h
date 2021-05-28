/**
****************************************************************
* @file myoslib/src/os_spi.c
* @author Aiden Kearney - s4529666
* @date 6/04/2021
* @brief OS SPI driver (for particle argon).
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* spi_init            () - Initialises SPI for the particle argon.
* spi_recieve_message () - Recieves a message from SPI.
* spi_send_message    () - Sends a message over SPI.
***************************************************************
*/

#ifndef OS_SPI_H__
#define OS_SPI_H__

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <drivers/uart.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <sys/printk.h>
#include <usb/usb_device.h>
#include <zephyr.h>

#include "globals.h"

extern void spi_init();

extern uint8_t *spi_recieve_message();

extern void spi_send_message(uint8_t transmitBuff[SPI_BUFFER_SIZE]);

extern int spi_receive_ultrasonic_data();

#endif