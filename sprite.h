/*
 * sprite.h
 *
 * Created: 4/14/2020 8:26:16 PM
 *  Author: lukem
 */ 
#ifndef SPRITE_H_
#define SPRITE_H_

#include <avr/pgmspace.h>

// 'pacman-closed-left', 8x8px
extern const uint8_t PACMAN_CLOSED_LEFT [] PROGMEM;

// 'pacman-closed-right', 8x8px
extern const uint8_t PACMAN_CLOSED_RIGHT [] PROGMEM;

// 'pacman-closed-up', 8x8px
extern const uint8_t PACMAN_CLOSED_UP [] PROGMEM;

// 'pacman-closed-down', 8x8px
extern const uint8_t PACMAN_CLOSED_DOWN [] PROGMEM;


// 'pacman-open-left', 8x8px
extern const uint8_t PACMAN_OPEN_LEFT [] PROGMEM;

// 'pacman-open-right', 8x8px
extern const uint8_t PACMAN_OPEN_RIGHT [] PROGMEM;

// 'pacman-open-up', 8x8px
extern const uint8_t PACMAN_OPEN_UP [] PROGMEM;

// 'pacman-open-down', 8x8px
extern const uint8_t PACMAN_OPEN_DOWN [] PROGMEM;

// 'ghost', 8x8px
extern const uint8_t GHOST [] PROGMEM;



#endif /* SPRITE_H_ */