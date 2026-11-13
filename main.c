#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "AiP1640_init.h"
// Lighting pattern
const uint8_t display_data[10] = {0x00, 0x01, 0x08, 0x0F, 0x41, 0x49, 0x4F, 0x79, 0x7F};
// Light pattern corresponding to the thousands digit
const uint8_t v3_index[10] = {8,0,7,5,3,6,8,1,8,6};
// Light pattern corresponding to the hundreds digit
const uint8_t v2_index[10] = {4,0,5,5,2,5,5,1,5,5};
// Light pattern corresponding to the tens digit
const uint8_t v1_index[10] = {4,0,5,5,2,5,5,1,5,5};
// Light pattern corresponding to the units digit
const uint8_t v0_index[10] = {8,8,6,8,8,7,7,8,8,8};
const uint TIME_INTERVAL = 100;

// void thousands_digit(bool enable, uint8_t data){
//     write_byte(enable ? display_data[v3_index[data]] : 0x00);
//     write_byte(enable ? display_data[v2_index[data]] : 0x00);
//     write_byte(enable ? display_data[v1_index[data]] : 0x00);
//     write_byte(enable ? display_data[v0_index[data]] : 0x00);
// }

void hundreds_digit(bool enable, uint8_t data){
    write_byte(0x00);
    write_byte(enable ? display_data[v3_index[data]] : 0x00);
    write_byte(enable ? display_data[v2_index[data]] : 0x00);
    write_byte(enable ? display_data[v1_index[data]] : 0x00);
    write_byte(enable ? display_data[v0_index[data]] : 0x00);
}

void tens_digit(bool enable, uint8_t data){
    write_byte(0x00);
    write_byte(enable ? display_data[v3_index[data]] : 0x00);
    write_byte(enable ? display_data[v2_index[data]] : 0x00);
    write_byte(enable ? display_data[v1_index[data]] : 0x00);
    write_byte(enable ? display_data[v0_index[data]] : 0x00);
}

void ones_digit(bool enable, uint8_t data){
    write_byte(0x00);
    write_byte(enable ? display_data[v3_index[data]] : 0x00);
    write_byte(enable ? display_data[v2_index[data]] : 0x00);
    write_byte(enable ? display_data[v1_index[data]] : 0x00);
    write_byte(enable ? display_data[v0_index[data]] : 0x00);
}

int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    aip1640_init();

    int32_t dist = 999;
    

    aip1640_start_command();
    write_byte(0x40);// address self-increment mode
    aip1640_stop_command();

    aip1640_start_command();
    write_byte(0xC0);// set address of display register (from 0xC0 start)
    for(int32_t i = 0; i <= dist; ++i){
        // thousand(i / 1000 > 0, (i / 1000) % 10);
        write_byte(0x00);
        hundreds_digit(i / 100 > 0, (i / 100) % 10);
        tens_digit(i / 10 > 0, (i / 10) % 10);
        ones_digit(true, i % 10);
        sleep_ms(TIME_INTERVAL);
    }
    aip1640_stop_command();
    while (true) {
        tight_loop_contents();
    }
}
