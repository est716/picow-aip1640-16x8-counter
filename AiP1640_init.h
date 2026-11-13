#ifndef AIP1640_INIT_H
#define AIP1640_INIT_H
#include <stdio.h>
#include "pico/stdlib.h"

#define PIN_SCL 0
#define PIN_SDA 1

void aip1640_start_command();
void aip1640_stop_command();
void write_byte(uint8_t data);
void aip1640_init();
#endif