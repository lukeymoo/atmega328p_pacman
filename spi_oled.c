#include "spi_oled.h"

// Enables SPI hardware
void init_spi(void)
{
    // Initialize SPI port
    DDR_SPI |= (1 << DD_SCK) | (1 << DD_MOSI) | (1 << DD_SS);
    PORT_SPI |= (1 << DD_SS); // set high/output to prevent enable as slave

    // Initialize LCD pins
    LCD_DDR |= (1 << LCD_CS) | (1 << LCD_DC) | (1 << LCD_RST);
    LCD_PORT |= (1 << LCD_CS) | (1 << LCD_DC) | (1 << LCD_RST);

    // Enable SPI
    // Data order -> MSB
    // Master Mode
    // Clock Speed 20,000,000 MHz / 4 -> 5 MHz
    SPCR = (1 << SPE) | (1 << MSTR);
    return;
}

// LCD reset initialization
// Called privately by lcd_setup
void _lcd_init_reset(void)
{
    LCD_PORT &= ~(1 << LCD_RST);
    _delay_us(5);
    LCD_PORT |= (1 << LCD_RST);
    _delay_us(5);
    LCD_PORT &= ~(1 << LCD_RST);
    _delay_us(5);
    LCD_PORT |= (1 << LCD_RST);
    _delay_ms(50);
    return;
}

// Set common registers for LCD
void lcd_setup(void)
{
    uint8_t cmd[4] = {0, 0, 0, 0};
    
    _lcd_init_reset();
	
    LCD_X = 0;
    LCD_Y = 0;

    // display off
    cmd[0] = 0xAE;
    lcd_arcmd(cmd, 1);

    // memory addressing mode
    // 00 -> horizontal || 0x02 -> vertical
    cmd[0] = 0x20;
    cmd[1] = 0x00;
    lcd_arcmd(cmd, 2);

    // set page start address, 0-7
    cmd[0] = 0xB0;
    lcd_arcmd(cmd, 1);

    // set COM output scan direction .. low, high, start line address
    cmd[0] = 0xC8;
    cmd[1] = 0x00;
    cmd[2] = 0x10;
    cmd[3] = 0x40;
    lcd_arcmd(cmd, 4);

    // set contrast control register
    cmd[0] = 0x81;
    cmd[1] = 0x3F;
    lcd_arcmd(cmd, 2);

    // set segment remap.
    // A0=column 0's address mapped to SEG0 || A1=column 127's address mapped to SEG0
    cmd[0] = 0xA1;
    lcd_arcmd(cmd, 1);

    // display mode, A6= normal || A7=inverse
    cmd[0] = 0xA6;
    lcd_arcmd(cmd, 1);

    // multiplex ratio
    // DISPLAY_HEIGHT - 1
    cmd[0] = 0xA8;
    cmd[1] = (LCD_HEIGHT - 1);
    lcd_arcmd(cmd, 2);

    // output RAM to display, A4=output follows RAM || A5=output ignores RAM
    cmd[0] = 0xA4;
    lcd_arcmd(cmd, 1);

    // display offset
    // no offset
    cmd[0] = 0xD3;
    cmd[1] = 0x00;
    lcd_arcmd(cmd, 2);

    // set display clock ration/osc freq
    // 1111 0000 high nibble is for osc freq || low nibble for divide ratio
    cmd[0] = 0xD5;
    cmd[1] = 0xF0;
    lcd_arcmd(cmd, 2);

    // pre charge period
    cmd[0] = 0xD9;
    cmd[1] = 0x22;
    lcd_arcmd(cmd, 2);

    // com pins hardware config
    cmd[0] = 0xDA;
    cmd[1] = 0x12;
    lcd_arcmd(cmd, 2);

    // vcomh
    // .77xVcc
    cmd[0] = 0xDB;
    cmd[1] = 0x20;
    lcd_arcmd(cmd, 2);

    // charge pump 
    // enable during display ON
    cmd[0] = 0x8D;
    cmd[1] = 0x14;
    lcd_arcmd(cmd, 2);
	
	// Send power on command
    lcd_cmd(0xAF);
    
    // Clear buffer
    clear_buffer();
    
    // Blit blank buffer
    blit();
    return;
}

// Draws buffer to screen
void blit(void)
{
    set_cursor(0, 0);
    // iterate buffer and draw
    for(int y = 0; y < LCD_HEIGHT/8; y++) {
        for(int x = 0; x < LCD_WIDTH; x++) {
            lcd_ardata(BUFFER[y]+x, 1);
        }
    }
    return;
}

// Set X/Y -- Y is 8 pixels tall per `page`
// Range is B0-B7
// While X is 0-128
void set_cursor(uint8_t x, uint8_t y)
{
	uint8_t cmd[4] = { 0, 0, 0, 0 };
    cmd[0] = 0xB0 + y;
    cmd[1] = 0x21;
    cmd[2] = x;
    cmd[3] = 0x7f;
    LCD_X = x;
    LCD_Y = y;
    lcd_arcmd(cmd, 4);
    return;
}

// Clear screen
void lcd_clear(void)
{
    uint8_t data = 0x00;
	
    for(uint8_t y = 0; y < LCD_HEIGHT/8; y++) {
		for(uint8_t x = 0; x < LCD_WIDTH; x++) {
			set_cursor(x, y);
			lcd_data(data);
		}
    }
    
    set_cursor(0, 0);
    return;
}

void lcd_arcmd(uint8_t *command_set, uint16_t cmd_size)
{
    // Set LCD_DC low for command
    LCD_PORT &= ~(1 << LCD_DC);
    
    // Select LCD
    LCD_PORT &= ~(1 << LCD_CS);

    // Go through command_set and set bytes as command
    for(uint8_t i = 0; i < cmd_size; i++) {
        SPDR = command_set[i]; // send data
        while(!(SPSR & (1 << SPIF))); // Wait for completion
    }

    // Unselect LCD
    LCD_PORT |= (1 << LCD_CS);
    return;
}

// Sends single byte of data as command
void lcd_cmd(uint8_t command)
{
    // Low for command
    LCD_PORT &= ~(1 << LCD_DC);
    // Select LCD 
    LCD_PORT &= ~(1 << LCD_CS);
    
    // Send command
    SPDR = command;
    while(!(SPSR & (1 << SPIF))); // wait for transmission
    
    // Release LCD 
    LCD_PORT |= (1 << LCD_CS);
}

// Send array of bytes as data
void lcd_ardata(uint8_t *data_set, uint16_t data_size)
{
    // Set LCD_DC HIGH for data
    LCD_PORT |= (1 << LCD_DC);
 
    // Select LCD
    LCD_PORT &= ~(1 << LCD_CS);

    for(uint8_t i = 0; i < data_size; i++) {
        SPDR = data_set[i]; // send data
        while(!(SPSR & (1 << SPIF))); // wait for completion
    }

    // Unselect LCD
    LCD_PORT |= (1 << LCD_CS);
    return;
}

// Send single byte of data
void lcd_data(uint8_t data)
{
    // DC pin high for DATA
    LCD_PORT |= (1 << LCD_DC);
    // Select LCD, active low
    LCD_PORT &= ~(1 << LCD_CS);
    
    // Send data
    SPDR = data;
    while(!(SPSR & (1 << SPIF))); // wait for transmission
    
    // Release LCD
    LCD_PORT |= (1 << LCD_CS);
    return;
}

void lcd_nextline(void)
{
    if(LCD_Y < 7) {
        set_cursor(0, LCD_Y + 1);
    }
    return;
}

// Does not function well yet
void put_pixel(uint8_t x, uint8_t y)
{	
    if(x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1) {
        return;
    }
    
    BUFFER[y/8][x] |= (1 << (y % 8));
    return;
}

// Does not function well yet
void clear_pixel(uint8_t x, uint8_t y)
{	
    if(x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1) {
        return;
    }
    
    BUFFER[y/8][x] &= ~(1 << (y % 8));
    return;
}

void clear_buffer(void)
{
    for(int y = 0; y < LCD_HEIGHT/8; y++) {
        for(int x = 0; x < LCD_WIDTH; x++) {
            BUFFER[y][x] = 0x00;
        }
    }
    return;
}

