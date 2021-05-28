/**
****************************************************************
* @file myoslib/src/cli_time.c
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief CLI time driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* cli_time_init () - Initialises the time command in the CLI.
***************************************************************
*/

#include "cli_time.h"
#include <posix/time.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <sys/printk.h>
#include <sys/timeutil.h>
#include <zephyr.h>

/**
 * @brief Uses the zephyrOS clock API to get the current time. by default, time
 * is displayed in seconds, but specifying a will return seconds, minutes, and
 * hours.
 * @param shell - Current shell handle.
 * @param argc - Number of input arguments.
 * @param argv - Input arguments.
 * @return int - 0
 */
static int print_time(const struct shell *shell, size_t argc, char **argv) {
    struct timespec tp;
    struct tm tm;

    clock_gettime(CLOCK_REALTIME, &tp);

    gmtime_r(&tp.tv_sec, &tm);

    /** If user has selected seconds **/
    if ((argv[1][0] == 's') || (argv[1][0] == 'S') || (argc == 1)) {
        shell_print(shell, "%d seconds", tm.tm_sec);

        /** Otherwise if they've selected to display hour, minute, second **/
    } else if ((argv[1][0] == 'a') || (argv[1][0] == 'A')) {
        shell_print(shell, "%d hours, %d minutes, %d seconds", tm.tm_hour,
            tm.tm_min, tm.tm_sec);

        /** if they've entered and incorrect argument **/
    } else {
        shell_print(shell, "Incorrect argument.\r\nUsage:\r\n s - prints the "
                           "current seconds elapsed in the session\r\n a - "
                           "prints the hours, minutes, and seconds elapsed.");
    }

    return 0;
}

/**
 * @brief Initialises the time CLI command.
 */
void cli_time_init() {
    SHELL_CMD_ARG_REGISTER(
        time, NULL, "Show the current time in ticks", print_time, 1, 1);
}