#ifndef ENGINE_H
#define ENGINE_H

#include "spi_oled.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 	0 // 0000 0000
#define RIGHT 	1 // 0000 0001
#define UP 		2 // 0000 0010
#define DOWN 	3 // 0000 0100
#define NEITHER 0xff // used in pathfinding to know if player is exactly above the player

// User input is monitored via PORTC
// We should use 0x0f to mask input pins
// Buttons are on these pins
#define KEY_UP		PC0
#define KEY_RIGHT	PC1
#define KEY_LEFT	PC2
#define KEY_DOWN	PC3

uint8_t retainer;

// SUCCESS SCREEN
extern const uint8_t SUCCESS_SCREEN [] PROGMEM;
void show_sucess();

// FAIL SCREEN
extern const uint8_t FAIL_SCREEN [] PROGMEM;
void show_fail();

void reset();

// Create enemies
ENEMY_STRUCT ghost;

// Create player
PLAYER_STRUCT player;

volatile uint8_t USER_INPUT;

// overlay debugging information
void display_debug(PLAYER_STRUCT *player);

// Checks for collisions in specified direction
uint8_t check_collision(PLAYER_STRUCT *player, uint8_t direction);
uint8_t is_wall(uint8_t x, uint8_t y);
GAME_OBJECT *is_object(PLAYER_STRUCT *player, uint8_t x, uint8_t y);

// Set initial values for game
void init_game();

// Initializes pins in port C for user input
void init_input(void);

// Checks state of buttons
void check_input(PLAYER_STRUCT *player);

// Draw specified map of given size ( single axis array of 1024 elements is recommend )
void draw_map(const uint8_t *map, uint16_t size);

// Draws points where collision map = 0
// Draw abilities where collision map = 2
void draw_objects();

// Draws sprite, uses global variable for animation state
//void draw_pacman(void);

// Draws pacman on a per pixel basis
void draw_sprite(PLAYER_STRUCT *player);

void draw_enemy(ENEMY_STRUCT *ghost);

// Moves player in given direction
void move(PLAYER_STRUCT *player);

// Controls enemy AI
// Just moves in a direction until unable to
// Then a new direction, repeat
void ai_logic(PLAYER_STRUCT *player, ENEMY_STRUCT *ghost);

// Draw score
void draw_score(uint8_t score);

// Draw Live's bar
void draw_lives(uint8_t lives);

// Draw box around player's tile
void debug_tile(PLAYER_STRUCT *player);


// getter functions, make things neater for game logic
// For PLAYER
//int playerTopLeftX(PLAYER_STRUCT *player);
//int playerTopLeftY(PLAYER_STRUCT *player);
//int playerTopRightX(PLAYER_STRUCT *player);
//int playerTopRightY(PLAYER_STRUCT *player);
//
//int playerBottomLeftX(PLAYER_STRUCT *player);
//int playerBottomLeftY(PLAYER_STRUCT *player);
//int playerBottomRightX(PLAYER_STRUCT *player);
//int playerBottomRightY(PLAYER_STRUCT *player);
//
//// For Collision map vertices
int cTopLeftX(PLAYER_STRUCT *player);
int cTopLeftY(PLAYER_STRUCT *player);
//int cTopRightX(PLAYER_STRUCT *player);
//int cTopRightY(PLAYER_STRUCT *player);
//
//int cBottomLeftX(PLAYER_STRUCT *player);
//int cBottomLeftY(PLAYER_STRUCT *player);
//int cBottomRightX(PLAYER_STRUCT *player);
//int cBottomRightY(PLAYER_STRUCT *player);

#endif
