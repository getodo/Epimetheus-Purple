/**
****************************************************************
* @file myoslib/src/os_rgbled.c
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief OS LED driver.
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

#include "os_rgbled.h"
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <zephyr.h>

/** LED DEFINES **/
#define LED1_NODE DT_ALIAS(led1)
#define LED1      DT_GPIO_LABEL(LED1_NODE, gpios)
#define PINR      DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGSR    DT_GPIO_FLAGS(LED1_NODE, gpios)

#define LED2_NODE DT_ALIAS(led2)
#define LED2      DT_GPIO_LABEL(LED2_NODE, gpios)
#define PING      DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGSG    DT_GPIO_FLAGS(LED2_NODE, gpios)

#define LED3_NODE DT_ALIAS(led3)
#define LED3      DT_GPIO_LABEL(LED3_NODE, gpios)
#define PINB      DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGSB    DT_GPIO_FLAGS(LED3_NODE, gpios)

/** LED Variables **/
const struct device *red;
const struct device *green;
const struct device *blue;

/** Keep track of the current status of the LEDs **/
bool red_on;
bool blue_on;
bool green_on;

/**
 * @brief binds the LEDs and assigns them to a device struct, and sets their
 * status to 0
 */
void os_rgbled_init() {
    red = device_get_binding(LED1);
    gpio_pin_configure(red, PINR, GPIO_OUTPUT_ACTIVE | FLAGSR);

    green = device_get_binding(LED2);
    gpio_pin_configure(green, PING, GPIO_OUTPUT_ACTIVE | FLAGSG);

    blue = device_get_binding(LED3);
    gpio_pin_configure(blue, PINB, GPIO_OUTPUT_ACTIVE | FLAGSB);

    gpio_pin_set(red, PINR, 0);
    gpio_pin_set(green, PING, 0);
    gpio_pin_set(blue, PINB, 0);
    red_on = false;
    blue_on = false;
    green_on = false;
}

/**
 * @brief Turns the specified colour channel on.
 * @param colour - The colour we want to turn on (r, g, or b)
 * @return true - If the function encounters an argument error.
 * @return false - If the function executes succesfully.
 */
bool os_rgbled_turnon(char colour) {
    switch (colour) {
        case 'r':
            gpio_pin_set(red, PINR, 1);
            red_on = true;
            break;
        case 'g':
            gpio_pin_set(green, PING, 1);
            green_on = true;
            break;
        case 'b':
            gpio_pin_set(blue, PINB, 1);
            blue_on = true;
            break;
        default:
            return true;
    }
    return false;
}

/**
 * @brief turns the selected colour channel off.
 * @param colour - the colour channel we want to turn off.
 * @return true - If the function encounters an argument error.
 * @return false - If the function executes succesfully.
 */
bool os_rgbled_turnoff(char colour) {
    switch (colour) {
        case 'r':
            gpio_pin_set(red, PINR, 0);
            red_on = false;
            break;
        case 'g':
            gpio_pin_set(green, PING, 0);
            green_on = false;
            break;
        case 'b':
            gpio_pin_set(blue, PINB, 0);
            blue_on = false;
            break;
        default:
            return true;
    }
    return false;
}

/**
 * @brief Toggles the current state of the colour channel. If it is on, it will
 * be off, and vice versa.
 * @param colour - the colour we want to toggle.
 * @return true - If the function encounters an argument error.
 * @return false - If the function executes succesfully.
 */
bool os_rgbled_toggle(char colour) {
    switch (colour) {
        case 'r':
            red_on = !red_on;
            gpio_pin_set(red, PINR, (int)red_on);
            break;
        case 'g':
            green_on = !green_on;
            gpio_pin_set(green, PING, (int)green_on);
            break;
        case 'b':
            blue_on = !blue_on;
            gpio_pin_set(blue, PINB, (int)blue_on);
            break;
        default:
            return true;
    }
    return false;
}