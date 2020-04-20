#ifndef LCD_FONT_H
#define LCD_FONT_H

#include <avr/pgmspace.h>
#include <avr/io.h>


extern const uint8_t FONT[][6] PROGMEM;

// Prints string to screen
void lcd_print(char *str);

// Prints character to screen
void lcd_putc(char c);

// Prints to buffer
void buf_print(char *str);
void buf_putc(char c);

// Prints to buffer - black font
void overlay_print(char *str);
void overlay_putc(char c);


#endif
