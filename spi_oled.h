#ifndef M_SPI_OLED
#define M_SPI_OLED

#include <avr/io.h>
#include <util/delay.h>
#include "font.h"

#define DDR_SPI     DDRB
#define PORT_SPI    PORTB
#define DD_SCK      PB5 // Clock
#define DD_MOSI     PB3 // Master Output
#define DD_SS       PB2 // Slave Select -- Set and keep high to prevent SPI malfunction

// PORTD For remaining Pins
#define LCD_DDR     DDRD
#define LCD_PORT    PORTD
#define LCD_CS      PD5 // Chip select ( Active LOW )
#define LCD_DC      PD6 // Data/Command Select | HIGH/LOW Correspondingly
#define LCD_RST     PD7 // Reset | Active Low
// PD3 - Chip Select ( Active LOW )
// PD4 - Data/Command Select ( Command = LOW, Data = HIGH )
// PD5 - Reset (Active low)

// LCD DISPLAY SETTINGS
#define LCD_WIDTH 128
#define LCD_HEIGHT 64


// Prints string to screen
void lcd_print(char *str);

// Prints character to screen
void lcd_putc(char c);

// Display buffer
uint8_t BUFFER[LCD_HEIGHT/8][LCD_WIDTH];

// LCD Cursor coordinates
uint8_t LCD_X, LCD_Y;

// Enables SPI hardware
void init_spi(void);

// Setup common registers and get LCD ready for display
void lcd_setup(void);

// Draws display buffer to screen
void blit(void);

void set_cursor(uint8_t x, uint8_t y);

// Clears buffer & blit to screen
void lcd_clear(void);

// Lcd initialization step requires LCD reset pin manip
void _lcd_init_reset(void);

// sends array of commands
void lcd_arcmd(uint8_t *command_set, uint16_t cmd_size);

// sends single command
void lcd_cmd(uint8_t command);

// Send array of bytes as data
void lcd_ardata(uint8_t *data_set, uint16_t data_size);

// Send single byte of data
void lcd_data(uint8_t data);

// Move LCD cursor to next `page` || aka line
void lcd_nextline(void);

// Does not function well yet
void put_pixel(uint8_t x, uint8_t y);

// Does not function well yet
void clear_pixel(uint8_t x, uint8_t y);

// Clear display buffer
void clear_buffer(void);

#endif
