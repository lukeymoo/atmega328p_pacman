/*
 * map.h
 *
 * Created: 4/14/2020 8:25:49 PM
 *  Author: lukem
 */ 


#ifndef MAP_H_
#define MAP_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "spi_oled.h"

#define POINT	0
#define WALL	1
#define ABILITY 2
#define EMPTY	3
#define E_SPAWN	4
#define GATE	5

// Define a game object(points, abilities)
typedef struct GAME_OBJECT {
	uint8_t x;
	uint8_t y;
	uint8_t type; // point or ability?
	uint8_t active; // should player interact?
} GAME_OBJECT;

// Game Collision MAP
extern const uint8_t COLLISION_MAP[8][16] PROGMEM;

// Game MAP
extern const uint8_t MAP[] PROGMEM;

// Objects MAP
GAME_OBJECT OBJECT_MAP[128];

void init_map(void);

GAME_OBJECT *find_object(uint8_t x, uint8_t y);

#endif /* MAP_H_ */