/*
 * enemy.h
 *
 * Created: 4/16/2020 12:53:19 PM
 *  Author: lukem
 */ 


#ifndef ENEMY_H_
#define ENEMY_H_

#include "sprite.h"

// Going to have to keep enemies simple
// Not enough RAM :(
typedef struct ENEMY_STRUCT {
	uint8_t x, y, direction; // movement variables
	// path finding variables
	// used for look aheads
	uint8_t pixels_moved;
	uint8_t last_direction;
	uint8_t blocked;
} ENEMY_STRUCT;



#endif /* ENEMY_H_ */