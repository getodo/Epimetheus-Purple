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

/** Includes **************************************************/
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include <drivers/uart.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <sys/printk.h>
#include <usb/usb_device.h>
#include <zephyr.h>

/** User Includes *********************************************/
#include "globals.h"
#include "os_spi.h"

/** Variable Declarations *************************************/
uint8_t rxBuffer[SPI_BUFFER_SIZE];
uint8_t txBuffer[SPI_BUFFER_SIZE];
uint8_t dummyRxBuffer[SPI_BUFFER_SIZE];
uint8_t dummyTxBuffer[SPI_BUFFER_SIZE];

/** SPI configuration structs **/
const struct device *spi;
struct spi_config spi_cfg = {0};
struct spi_cs_control spi_cs;

/** Rx Buffer **/
struct spi_buf rx_buf = {
    .buf = rxBuffer,
    .len = sizeof(rxBuffer),
};

/** Dummy Rx used for read transcieve **/
struct spi_buf dummyRx_buf = {
    .buf = dummyRxBuffer,
    .len = sizeof(dummyRxBuffer),
};

struct spi_buf_set rx = {.buffers = &rx_buf, .count = 1}; // SPI recieve buffer

struct spi_buf_set dummyRx = {
    .buffers = &dummyRx_buf, .count = 1}; // Dummy recieve buffer struct

/** Tx Buffer **/
struct spi_buf tx_buf = {
    .buf = txBuffer, .len = sizeof(txBuffer)}; // SPI transmit buffer

/** Dummy TX Buffer **/
struct spi_buf dummyTx_buf = {.buf = dummyTxBuffer,
    .len = sizeof(dummyTxBuffer)}; // SPI dummy transmit vuffer

struct spi_buf_set tx = {.buffers = &tx_buf, .count = 1}; // SPI tx buffer set

struct spi_buf_set dummyTx = {
    .buffers = &dummyTx_buf, .count = 1}; // SPI dummy tx vuffer set

/**
 * @brief Calls the nessecary commands needed to initialise and configure SPI
 *
 */
void spi_init() {

    spi = device_get_binding("SPI_1");

    if (spi == NULL) {
        printk("Could not find SPI driver\n");
        return;
    }

    spi_cfg.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER;
    spi_cfg.frequency = 2000000;
    spi_cfg.slave = 0;

    spi_cs.gpio_dev = device_get_binding("GPIO_0");
    spi_cs.gpio_pin = 31;
    spi_cs.delay = 0;
    spi_cs.gpio_dt_flags = GPIO_ACTIVE_LOW;
    spi_cfg.cs = &spi_cs;
}

/**
 * @brief Makes a copy of the input array and sends it over SPI.
 *
 * @param transmitBuff - The array to be copied and sent over SPI.
 */
void spi_send_message(uint8_t transmitBuff[SPI_BUFFER_SIZE]) {
    /** Pedantic copy of RxBuffer just to make sure there is no possibility of
     * changing it **/
    memcpy(txBuffer, transmitBuff, sizeof(uint8_t) * SPI_BUFFER_SIZE);

    spi_transceive(spi, &spi_cfg, &tx, &dummyRx);
}

/**
 * @brief Read a message from SPI and return a pointer to its packet data.
 *
 * @return uint8_t* - The packet data recieved over SPI.
 */
uint8_t *spi_recieve_message() {
    /** Recieve message to send via SPI. **/
    if (spi_read(spi, &spi_cfg, &rx) != 0) {
        shell_fprintf(shell_backend_uart_get_ptr(), SHELL_ERROR,
            "Error reading from SPI\r\n");
    }
#ifdef debug
    shell_print(shell_backend_uart_get_ptr(), "We have recieved:\r\n");

    for (size_t i = 0; i < SPI_BUFFER_SIZE; i++) {
        shell_fprintf(shell_backend_uart_get_ptr(), SHELL_INFO,
            "Char %d:\t%X\r\n", i, rxBuffer[i]);
    }
#endif
    uint8_t *recvBuffCopy = k_malloc(sizeof(uint8_t) * SPI_BUFFER_SIZE);
    memcpy(recvBuffCopy, rxBuffer, sizeof(uint8_t) * SPI_BUFFER_SIZE);
    return recvBuffCopy;
}

int spi_receive_ultrasonic_data() {
    if (spi_read(spi, &spi_cfg, &rx) == 0) {
        if (rxBuffer[0] == 0xAA) {
            return (int)rxBuffer[1];
        }
    }
    return -1;
}