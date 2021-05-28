/**
****************************************************************
* @file myoslib/src/os_hci
* @author Aiden Kearney - s4529666
* @date 6/04/2021
* @brief hardware command interface OS interactions driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* i2c_reg_cmd      () - Command handler for i2creg command.
* lsm6dsl_read_cmd () - Command handler for lsm6dsl command.
***************************************************************
*/

/* Include ***************************************************/
#include "globals.h"
#include "hci_packet.h"
#include "os_hci.h"
#include "os_lsm6dsl.h"
#include "os_spi.h"
#include <ctype.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <drivers/uart.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <stdlib.h>
#include <string.h>
#include <sys/printk.h>
#include <usb/usb_device.h>
#include <version.h>
#include <zephyr.h>

int SIDToAddrArray[5] = {0b11010100, 0b00111100, 0b10111010, 0b10111010,
    0b10111110}; // Array used to store sensor IDs.

char *SIDToSensName[5] = {"LSM6DSL", "LIS3MDL", "LPS22HB", "VL53L0X",
    "HTS221"}; // Array to store sensor names.

/**
 * @brief Performs processing on a packet recieved over SPI intended for the
 * i2creg function.
 *
 * @param packet - The packet that was recieved
 */
void i2creg_process_packet(uint8_t *packet) {
    if (packet[0] != 0xAA) {
        shell_fprintf(shell_backend_uart_get_ptr(), SHELL_ERROR,
            "The packet preamble is incorrect!\r\n");
        return;
    } else {
        shell_print(shell_backend_uart_get_ptr(),
            "Read the byte 0x%X from the register 0x%X from the %s", packet[4],
            packet[6], SIDToSensName[packet[3]]);
    }
}

/**
 * @brief Performs processing on a packet recieved over SPI intended for the
 * lsm6dsl function.
 *
 * @param packet - The packet that was recieved.
 * @param command - The command parameter that was entered - determines which
 * value(s) to print.
 */
void lsm6dsl_process_packet(uint8_t *packet, char command) {
    if (packet[0] != 0xAA) {
        shell_fprintf(shell_backend_uart_get_ptr(), SHELL_ERROR,
            "The packet preamble is incorrect!\r\n");
        return;
    } else {
        uint8_t xData[2];
        uint8_t yData[2];
        uint8_t zData[2];
        xData[0] = packet[4];
        xData[1] = packet[6];
        yData[0] = packet[7];
        yData[1] = packet[8];
        zData[0] = packet[9];
        zData[1] = packet[10];
        switch (command) {
            case 'x':
                shell_print(shell_backend_uart_get_ptr(), "X: %lfm/s^2",
                    lsm6dsl_accel_convert(xData));
                break;

            case 'y':
                shell_print(shell_backend_uart_get_ptr(), "Y: %lfm/s^2",
                    lsm6dsl_accel_convert(yData));
                break;

            case 'z':
                shell_print(shell_backend_uart_get_ptr(), "Z: %lfm/s^2",
                    lsm6dsl_accel_convert(zData));
                break;

            case 'a':
                shell_print(shell_backend_uart_get_ptr(),
                    "X: %lfm/s^2\tY: %lfm/s^2\tZ: %lfm/s^2",
                    lsm6dsl_accel_convert(xData), lsm6dsl_accel_convert(yData),
                    lsm6dsl_accel_convert(zData));
                break;

            default:
                break;
        }
    }
}

/**
 * @brief Command handler for the i2creg command.
 *
 * @param shell - Shell pointer of the parent shell.
 */
void i2c_reg_cmd(
    const struct shell *shell, size_t argc, char **argv, void *data) {
    /** Check input parameters **/
    int type = 0;
    int regval = 0;
    if (argv[1][0] == 'r') {
        type = 0x01;
    } else if (argv[1][0] == 'w') {
        type = 0x02;
    }
    int SID = argv[2][0] - '0';
    int regAddr = strtol(argv[3], NULL, 16);
    if (argc == 5) {
        regval = strtol(argv[4], NULL, 16);
    }

    /** Create the packet we wish to send over SPI **/
    uint8_t *sendPacket =
        create_packet(type, SID, SIDToAddrArray[SID], regAddr, regval);

    /** Send the packet **/
    spi_send_message(sendPacket);

    k_free(sendPacket);
    k_msleep(500);
    uint8_t *recieveBuff = spi_recieve_message();
    i2creg_process_packet(recieveBuff);

    k_free(recieveBuff);
}

/**
 * @brief - Command handler for the lsm6dsl command.
 *
 * @param shell - A shell pointer to the parent shell.
 */
void lsm6dsl_read_cmd(
    const struct shell *shell, size_t argc, char **argv, void *data) {
    bool argErr = false; // keeps track of if there is an argument error.
    if (argv[1][0] != 'r') {
        argErr = true;
    }

    /** Here we are sending a special packet to indicate we are running this
     * command **/
    int SID = 6;     // Reserved for lsm command.
    int type = 0x01; // We are requesting data.

    uint8_t *sendPacket = create_packet(type, SID, SIDToAddrArray[1], 0, 0);

    /** Send the packet **/
    spi_send_message(sendPacket);

    /** Free memory used **/
    k_free(sendPacket);

    /** Delay to allow for response **/
    k_msleep(500);

    /** Store recieved message **/
    uint8_t *recieveBuff = spi_recieve_message();
    lsm6dsl_process_packet(recieveBuff, argv[2][0]);

    k_free(recieveBuff);
}