#ifndef UARTE_271
#define UARTE_271

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <nrfx_uarte.h>
#include <nrfx_rtc.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_systick.h>
#include <nrf_gpio.h>
#include <dwt.h>
#include <nrf.h>

#define PIN_TXD0 20
#define PIN_RXD0 22

#define PIN_TXD1 36 // P1.04
#define PIN_RXD1 37 // P1.05

#define UARTE_RX_BUFFER_SIZE 100
#define UARTE_TX_BUFFER_SIZE 100

void uarte_init();

void uarte0_write(char* tx_buffer, size_t bufferSize);
char uarte0_read();

void uarte1_write(char* tx_buffer, size_t bufferSize);
char uarte1_read();

void uart_bb_tx(uint32_t pin, uint8_t data, float baudrate);

#endif