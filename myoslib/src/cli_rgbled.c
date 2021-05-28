/**
****************************************************************
* @file myoslib/src/cli_rgbled.c
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief CLI LED driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* cli_led_init () - Initialises the LED command in the CLI.
***************************************************************
*/

#include "cli_rgbled.h"
#include "os_log.h"
#include "os_rgbled.h"
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr.h>

/**
 * @brief Shell command that takes in 3 commands (o(n), (of)f, t(oggle)) and 3
 * colours (r, g, b) and performs that action on that colour LED.
 * @param shell - The current shell parameter.
 * @param argc  - the input arguments
 * @param argv  - the number of input arguments.
 */
void led_set(const struct shell *shell, size_t argc, char **argv) {
    bool argError; // Triggered if there is an error reading the input.
    char logMsg[30];

    if (argc > 3) {
        argError = true;

    } else {
        switch (argv[1][0]) {
            case 'o':
                argError = os_rgbled_turnon(argv[2][0]);
                sprintf(logMsg, "%c LED turned on\r\n", argv[2][0]);
                os_log_send(logMsg, 'l', shell);
                break;

            case 'f':
                argError = os_rgbled_turnoff(argv[2][0]);
                sprintf(logMsg, "%c LED turned off\r\n", argv[2][0]);
                os_log_send(logMsg, 'l', shell);
                break;

            case 't':
                sprintf(logMsg, "%c LED toggled\r\n", argv[2][0]);
                os_log_send(logMsg, 'l', shell);
                argError = os_rgbled_toggle(argv[2][0]);
                break;

            default:
                argError = true;
                break;
        }
    }

    if (argError) {
        os_log_send("DEBUG: something has triggered the LED mode set error "
                    "detection\r\n",
            'd', shell);
        os_log_send(
            "Incorrect argument.\r\nUsage:\r\n led m c where m is mode and c "
            "is colour\r\nMode options:\r\no - on\r\nf - off\r\nt - toggle the "
            "led\r\nColour options:\r\nr - red\r\ng - green\r\nb - blue\r\n",
            'e', shell);
    }
}

/**
 * @brief Initialises the LED CLI comamnd.
 */
void cli_led_init() {
    SHELL_CMD_ARG_REGISTER(led, NULL, "Set the LED colour", led_set, 3, 0);
}