#include "oled.h"

void oled_cmd(char i2c, char cmd){
    i2c_start(i2c);
    i2c_address_w(i2c, 0x3C);
    i2c_data_w(i2c, 0x00); // 0x00 = Command mode
    i2c_data_w(i2c, cmd);
    i2c_stop(i2c);
}

// --- ADD THIS MISSING FUNCTION ---
void oled_data(char i2c, char data){
    i2c_start(i2c);
    i2c_address_w(i2c, 0x3C);
    i2c_data_w(i2c, 0x40); // 0x40 = Data mode (PIXELS!)
    i2c_data_w(i2c, data);
    i2c_stop(i2c);
}

void oled_init(char i2c){
    const unsigned char sequence[]={
       0xA8, 0x3F,
       0xD3, 0x00,
       0x40,
       0xA1,
       0xC8,
       0xDA, 0x02,
       0x81, 0x7F,
       0xA4,
       0xA6,
       0xD5, 0x80,
       0x8D, 0x14
    };

    uint32_t len = sizeof(sequence) / sizeof(sequence[0]);
    for(int i = 0; i < len; i++){
        oled_cmd(i2c, sequence[i]);
    }

    oled_clear(i2c);
    oled_cmd(i2c, 0xAF); // Display On
}

void oled_clear(char i2c) {
    for (int page = 0; page < 8; page++) {
        oled_cmd(i2c, 0xB0 + page); 
        oled_cmd(i2c, 0x00); 
        oled_cmd(i2c, 0x10); 
        
        for (int col = 0; col < 128; col++) {
            oled_data(i2c, 0x00); // <-- FIXED: Changed to oled_data
        }
    }
}

void oled_set_cursor(char i2c, uint8_t page, uint8_t col) {
    if (page > 7)   page = 7;
    if (col > 127)  col = 127;

    oled_cmd(i2c, 0xB0 + page); 
    oled_cmd(i2c, 0x00 + (col & 0x0F));       
    oled_cmd(i2c, 0x10 + ((col >> 4) & 0x0F)); 
}

void oled_print_char(char i2c, char c) {
    if (c < 32 || c > 90) {
        c = ' '; 
    }

    uint32_t font_index = c - 32;

    for (int col = 0; col < 5; col++) {
        oled_data(i2c, SSD1306_FONT[font_index][col]); // <-- FIXED: Changed to oled_data
    }
    oled_data(i2c, 0x00); // <-- FIXED: Changed to oled_data
}

void oled_print_str(char i2c, const char* str) {
    while (*str != '\0') {
        char current_char = *str;
        if (current_char >= 'a' && current_char <= 'z') {
            current_char -= 32; 
        }
        oled_print_char(i2c, current_char);
        str++; 
    }
}