/*
 * player.h
 *
 * Created: 4/14/2020 8:31:39 PM
 *  Author: lukem
 */ 
#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>

/*
	Collision Mask
	---------------
	0000 0000
	
	Only lower nibbles used
*/

#define COL_LEFT	0x01 // bit 1
#define COL_RIGHT	0x02 // bit 2
#define COL_UP		0x04 // bit 3
#define COL_DOWN	0x08 // bit 4

#include "sprite.h"
#include "map.h"


typedef struct PLAYER_STRUCT {
	uint8_t x, y, direction; // movement variables
	uint8_t animation; // only 2 frames of animation 0, 1
	uint8_t score;
	uint8_t lives;
	uint8_t dead;
	// pointers to animation frame data
	const uint8_t *loaded_closed;
	const uint8_t *loaded_open;
} PLAYER_STRUCT;




#endif /* PLAYER_H_ */