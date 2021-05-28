/**
****************************************************************
* @file myoslib/src/hci_packet.c
* @author Aiden Kearney - s4529666
* @date 6/04/2021
* @brief hardware command interface packet driver header.
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

#ifndef HCI_PACKET_H__
#define HCI_PACKET_H__

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

extern uint8_t *create_packet(int type, uint8_t SID, uint8_t deviceAddr,
    uint8_t registerAddr, uint8_t regVal);

extern void create_i2c_return_packet();

extern void create_lsm_return_packet(
    uint8_t *packetArr, int SID, uint8_t regVals[6]);

#endif