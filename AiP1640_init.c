#include "AiP1640_init.h"
void aip1640_start_command(){
    gpio_put(PIN_SCL, 1);
    gpio_put(PIN_SDA, 1);
    sleep_us(2);
    gpio_put(PIN_SDA, 0);
    sleep_us(2);
}

void aip1640_stop_command(){
    gpio_put(PIN_SCL, 0);
    gpio_put(PIN_SDA, 0);
    sleep_us(2);
    gpio_put(PIN_SCL, 1);
    sleep_us(2);
    gpio_put(PIN_SDA, 1);
    sleep_us(2);
}

void write_byte(uint8_t data){
    for(int i = 0; i < 8; ++i){
        gpio_put(PIN_SCL, 0);
        sleep_us(2);
        
        gpio_put(PIN_SDA, data & (1 << i) ? 1 : 0);
        sleep_us(2);

        gpio_put(PIN_SCL, 1);
        sleep_us(2);
    }
}

void aip1640_init(){

    // set GPIO pin and GPIO direction
    gpio_init(PIN_SCL);
    gpio_set_dir(PIN_SCL, GPIO_OUT);
    gpio_init(PIN_SDA);
    gpio_set_dir(PIN_SDA, GPIO_OUT);

    // set pull-up resistors
    gpio_pull_up(PIN_SCL);
    gpio_pull_up(PIN_SDA);

    // set data commands
    aip1640_start_command();
    write_byte(0x40);// set address self-increment mode
    aip1640_stop_command();

    // clear RAM
    aip1640_start_command();
    write_byte(0xC0);// set display address
    for(int i = 0; i < 16; ++i){
        write_byte(0x00);
    }
    aip1640_stop_command();

    aip1640_start_command();
    write_byte(0x88);// set display brightness
    aip1640_stop_command();
}