/**
****************************************************************
* @file myoslib/src/os_log.c
* @author Aiden Kearney - s4529666
* @date 15/03/2021
* @brief log OS driver (responsible for queues and threads)
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* os_log_init () - Initialises the log boolean variables.
* os_log_send () - Sends a message to the queue to be displayed.
* os_log_set_visibility () - Sets the visibility of different
* types of messages.
***************************************************************
*/

#include <device.h>
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <string.h>
#include <sys/__assert.h>
#include <sys/printk.h>
#include <zephyr.h>

struct LogData {
    void *fifo_reserved;       /* 1st word reserved for use by fifo */
    char *logMsg;              /** String we are sending **/
    char type;                 // type of message being sent
    const struct shell *shell; // Shell we're printing to.
};

/** booleans that track whether messages will be displayed **/
bool error_enable;
bool log_enable;
bool debug_enable;

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/** prototype for later **/
void log_print();

K_FIFO_DEFINE(QueueLogFIFO);

/** Create thread **/
K_THREAD_DEFINE(log_print_id, 1024, log_print, NULL, NULL, NULL, 7, 0, 0);

/**
 * @brief Initialises the initial visibility variables to true.
 */
void os_log_init() {
    /** Initialise the booleans to true **/
    error_enable = true;
    log_enable = true;
    debug_enable = true;
}

/**
 * @brief Reads from our log queue and prints messaged, with a colour based on
 * what type they are.
 */
void log_print() {
    while (1) {
        struct LogData *recvLog = k_fifo_get(&QueueLogFIFO, K_FOREVER);
        if ((recvLog->type == 'e') && (error_enable)) {
            shell_fprintf(recvLog->shell, SHELL_ERROR, "%s", recvLog->logMsg);
        } else if (((recvLog->type) == 'l') && (log_enable)) {
            shell_fprintf(recvLog->shell, SHELL_INFO, "%s", recvLog->logMsg);
        } else if (((recvLog->type) == 'd') && (debug_enable)) {
            shell_fprintf(recvLog->shell, SHELL_OPTION, "%s", recvLog->logMsg);
        }
        k_free(recvLog);
    }
}

/**
 * @brief sets which log messages will be displayed.
 * @param type - the type of message; error, log, debug, or all, we want.
 * @return true - if function executes succesfully.
 * @return false  - if an invalid argument is given.
 */
bool os_log_set_visibility(char type) {
    switch (type) {
        case 'e':
            error_enable = true;
            log_enable = false;
            debug_enable = false;
            break;

        case 'l':
            error_enable = false;
            log_enable = true;
            debug_enable = false;
            break;

        case 'd':
            error_enable = false;
            log_enable = false;
            debug_enable = true;
            break;

        case 'a':
            error_enable = true;
            log_enable = true;
            debug_enable = true;
            break;

        default:
            return true;
            break;
    }
    return false;
}

/**
 * @brief Helper function that performs the memory allocation and struct
 * population to send a message to the error queue.
 * @param message - The message we want to be displayed.
 * @param type - The type we want the message to be.
 * @param shell - The current shell pointer.
 */
void os_log_send(char *message, char type, const struct shell *shell) {
    struct LogData tx_data = {.logMsg = message, .type = type, .shell = shell};

    size_t size = sizeof(struct LogData);
    char *mem_ptr = k_malloc(size);
    __ASSERT_NO_MSG(mem_ptr != 0);

    memcpy(mem_ptr, &tx_data, size);

    k_fifo_put(&QueueLogFIFO, mem_ptr);

    k_msleep(100); // random debug delay to make sure recieve thread has
                   // time to recieve.
}