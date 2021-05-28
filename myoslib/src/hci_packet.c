/**
****************************************************************
* @file myoslib/src/hci_packet.c
* @author Aiden Kearney - s4529666
* @date 6/04/2021
* @brief hardware command interface packet driver.
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* create_packet            () - Creates a packet to be sent to the Disco board.
* create_i2c_return_packet () - Creates a packet for the i2c command to be sent
* to the argon board.
* create_lsm_return_packet () - Creates a packet for the lsm
* command to be sent to the argon board.
***************************************************************
*/

#include "globals.h"
#include "hci_packet.h"
#include <ctype.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <drivers/uart.h>
#include <logging/log.h>
#include <logging/log_ctrl.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/printk.h>
#include <usb/usb_device.h>
#include <version.h>
#include <zephyr.h>

/**
 * @brief Create an array used by our SPI packets.
 *
 * @param type - 0x01 if the packet is a request, 0x02 if it is a response.
 * @param SID - The sensor ID we are sending the packet to.
 * @param deviceAddr - The i2c address of the device we are sending to.
 * @param registerAddr - The i2c register address we are sending to.
 * @param regVal - The value we are writing to the register.
 * @return uint8_t* - An array containing the data formatted as a packet.
 */
uint8_t *create_packet(int type, uint8_t SID, uint8_t deviceAddr,
    uint8_t registerAddr, uint8_t regVal) {
    uint8_t *bufferArr = k_malloc(sizeof(uint8_t) * SPI_BUFFER_SIZE);
    uint8_t preamble = 0xAA;
    uint8_t length = 0;
    if (regVal != 0) {
        length = sizeof(SID) + sizeof(deviceAddr) + sizeof(registerAddr) +
                 sizeof(regVal);
    } else {
        length = sizeof(SID) + sizeof(deviceAddr) + sizeof(registerAddr);
    }

    bufferArr[0] = preamble;
    bufferArr[1] = type;
    bufferArr[2] = length;
    bufferArr[3] = SID;
    bufferArr[4] = deviceAddr;
    bufferArr[5] = registerAddr;
    bufferArr[6] = regVal;

    return bufferArr;
}

/**
 * @brief Create a return packet to be sent to the argon.
 *
 * @param packetArr - The array that the packet data will be placed into.
 * @param SID - The sensor ID of the sensor we retrieved data from.
 * @param sensorVal - The value retrieved from the sensor.
 */
void create_i2c_return_packet(
    uint8_t *packetArr, int SID, int sensorVal, int regval) {
    // TODO: Add an echo to confirm successfull write.
    packetArr[0] = 0xAA;      // Preamble
    packetArr[1] = 0x02;      // Response
    packetArr[2] = 2;         // Size
    packetArr[3] = SID;       // Sensor we read from.
    packetArr[4] = sensorVal; // Value we read.
    packetArr[5] = regval;    // Register we read from.
}

/**
 * @brief Create a return packet for the lsm6dsl command
 *
 * @param packetArr - The array we will modify with packet information.
 * @param SID - The sensor ID - 6 in this case. Parameter is here in case of
 * future code change.
 * @param regVals - A 6 bit array containing the XL,XH,YL,YH,ZL,ZH registers
 * retrieved from the i2c driver.
 */
void create_lsm_return_packet(uint8_t *packetArr, int SID, uint8_t regVals[6]) {
    packetArr[0] = 0xAA;       // Preamble
    packetArr[1] = 0x02;       // Response
    packetArr[2] = 2;          // Size
    packetArr[3] = SID;        // Sensor we read from.
    packetArr[4] = regVals[0]; // XL
    packetArr[5] = regVals[1]; // XH
    packetArr[6] = regVals[2]; // YL
    packetArr[7] = regVals[3]; // YH
    packetArr[8] = regVals[4]; // ZL
    packetArr[9] = regVals[5]; // ZH
}