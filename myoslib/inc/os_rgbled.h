/**
****************************************************************
* @file myoslib/src/os_rgbled.h
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief OS LED driver header.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* os_rgbled_init () - binds and initialises the LEDs so that they are visible to
* zephyr.
* os_rgbled_turnon (char colour) - turns the specified colour on.
* os_rgbled_turnoff (char colour) - turns the specified colour off.
* os_rgbled_toggle (char colour) - toggles the state of the colour.
***************************************************************
*/

#ifndef OS_LEDRGB_H
#define OS_LEDRGB_H

#include <drivers/gpio.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <zephyr.h>

extern void os_rgbled_init();
extern bool os_rgbled_turnon(char colour);
extern bool os_rgbled_turnoff(char colour);
extern bool os_rgbled_toggle(char colour);

#endif