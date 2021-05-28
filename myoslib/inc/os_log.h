/**
****************************************************************
* @file myoslib/src/os_log.h
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief log OS driver header
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* os_log_init () - Initialises the log boolean variables.
* os_log_send () - Sends a message to the queue to be displayed.
* os_log_set_visibility () - Sets the visibility of different
* types of messages.
***************************************************************
*/

#ifndef OS_LOG_H__
#define OS_LOG_H__

#include <device.h>
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <string.h>
#include <sys/__assert.h>
#include <sys/printk.h>
#include <zephyr.h>

extern void os_log_init();

extern void os_log_send(char *message, char type, const struct shell *shell);

extern bool os_log_set_visibility(char type);

#endif