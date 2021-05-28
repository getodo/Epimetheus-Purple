#include <zephyr.h>
#include <drivers/uart.h>
#include <usb/usb_device.h>
#include <device.h>
#include <drivers/spi.h>
#include <drivers/gpio.h>
#include <shell/shell.h>
#include <sys/printk.h>
#include <shell/shell_uart.h>
#include <stdio.h>
#include <ctype.h>
#include <drivers/uart.h>
#include <logging/log.h>
#include <logging/log_ctrl.h>
#include <stdlib.h>

#define PA4_NODE DT_ALIAS(pa4)

#define PA4	DT_GPIO_LABEL(PA4_NODE, gpios)
#define PIN_PA4	DT_GPIO_PIN(PA4_NODE, gpios)
#define FLAGS_PA2 DT_GPIO_FLAGS(PA4_NODE, gpios)

#define PB2_NODE DT_ALIAS(pb2)

#define PB2	DT_GPIO_LABEL(PB2_NODE, gpios)
#define PIN_PB2	DT_GPIO_PIN(PB2_NODE, gpios)
#define FLAGS_PB2 DT_GPIO_FLAGS(PB2_NODE, gpios)

const struct device* pina_4;
const struct device* pinb_2;

long pulseRisingEdge = 0;
long pulseFallingEdge = 0;
uint8_t pulseTick = 0;

void hal_init_ultrasonic() {

    int ret_pina_4;
    int ret_pinb_2;

    pina_4 = device_get_binding(PA4);
    if (pina_4 == NULL) {
        printk("Could not find gpio\n");
        return;
    }

    ret_pina_4 = gpio_pin_configure(pina_4, PIN_PA4, GPIO_INPUT | FLAGS_PA2);
    if (ret_pina_4 < 0) {
        return;
    }

    pinb_2 = device_get_binding(PB2);
    if (pinb_2 == NULL) {
        printk("Could not find gpio\n");
        return;
    }

    ret_pinb_2 = gpio_pin_configure(pinb_2, PIN_PB2, GPIO_OUTPUT_ACTIVE | FLAGS_PB2);
    if (ret_pinb_2 < 0) {
        return;
    }
}

uint8_t hal_ping_ultrasonic() {

    pulseTick = 0;

    // Set TRIG low for few usec
    gpio_pin_set(pinb_2, PIN_PB2, 0); 
    k_usleep(3);

    // Output 10usec TRIG
    gpio_pin_set(pinb_2, PIN_PB2, 1); 
    k_usleep(10);
    gpio_pin_set(pinb_2, PIN_PB2, 0);

    // Wait for ECHO pin raising edge
    while (gpio_pin_get(pina_4, PIN_PA4) == 0) {
        pulseTick++;
        k_usleep(1);
        // Timeout
        if (pulseTick > 15000) {
            printk("Ultra Sonic Sensor timed out waiting for echo raising edge\n");
            return 0;
        }
    }

    pulseTick = 0;

    // Measure ECHO pulse 
    while (gpio_pin_get(pina_4, PIN_PA4) == 1) {
        pulseTick++;
        k_usleep(2);
        // Timeout
        if (pulseTick > 15000) {
            printk("Ultra Sonic Sensor timed out measuring echo\n");
            return 0;
        }
    }
    printf("pulseTick = %d\n", pulseTick);
    return pulseTick;
}