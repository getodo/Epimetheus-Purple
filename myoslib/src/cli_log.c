/**
****************************************************************
* @file myoslib/src/cli_Log.c
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief log command line interface driver
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* cli_log_init () - Initialises the log console command.
***************************************************************
*/

#include "os_log.h"
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr.h>
/**
 * @brief depending on input, allows or dissalows certain messages from
 * appearing
 * @param shell - pointer to the shell.
 * @param argc, argv - standard inputs.
 * @retval none.
 */
void filter_logs(const struct shell *shell, size_t argc, char **argv) {
    bool argerror; // tracks if an argument error has occured

    if (argc != 2) {
        argerror = true;
    } else {
        argerror = os_log_set_visibility(argv[1][0]);
    }

    if (argerror) {
        os_log_send("Argument Error\r\nUsage: log <option>\r\ne - Display only "
                    "errors\r\nl - Display only log items\r\nd - display only "
                    "debug messages\r\na - display all messages\r\n",
            'e', shell);
    }
}

/**
 * @brief Initialises the command in the console.
 */
void cli_log_init() {
    SHELL_CMD_ARG_REGISTER(
        log, NULL, "Filter what messages appear in console", filter_logs, 2, 0);
}