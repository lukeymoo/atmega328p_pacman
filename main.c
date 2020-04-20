/*
 * Pacman.c
 *
 * Created: 4/13/2020 3:26:17 AM
 * Author : lukem
 */ 
#include "game.h"
#include <stdlib.h>

// overflow counter -- 163 overflows for 60 fps
// b/c we use 8 bit timer || only 1 16-bit available
volatile uint8_t T0_OVF_C = 0;
volatile uint8_t can_update = 0;

// game FPS timer
ISR(TIMER0_OVF_vect)
{
	if(T0_OVF_C >= 60) {
		can_update = 1;
		T0_OVF_C = 0;
		TCNT0 = 0;
	}
	T0_OVF_C++;
	return;
}

// Animation timer
ISR(TIMER1_OVF_vect)
{
	draw_sprite(&player);
	TCNT1 = 40000; // 41666
	return;
}

void init_timers(void);

int main(void)
{
	DDRB |= (1 << PB0);
	PORTB |= (1 << PB0);
	
	retainer = 3;
	
	// enable SPI
	init_spi();
	
	// Setup common registers
	lcd_setup();
	
	// enable user input pins
	init_input();
	
	init_game();
	init_timers();
	
	init_map();
	
	while(1) {		
		// We check input according to game timer
		if(can_update == 1) {
			//display_debug(&player);
			//draw_map(MAP, 1024);
			draw_objects();
			draw_sprite(&player);
			draw_enemy(&ghost);
			draw_score(player.score);
			draw_lives(player.lives);
			//debug_tile(&player);
			blit();
			clear_buffer();
			check_input(&player);
			move(&player);
			ai_logic(&player, &ghost);
			
			can_update = 0;
		}
		if(player.score == 64) {
			show_sucess();
			retainer = 3;
			reset();
		}
		if(player.dead && retainer > 1) {
			player.dead = 0;
			retainer --;
			reset();
			player.lives = retainer;
		}
		if(player.dead && retainer <= 1) {
			show_fail();
			retainer = 3;
			reset();
		}
	}
	
	return 0;
}

void init_timers(void)
{
	// we count to 31250 using 8 bit timers
	// because number is too big we utilize overflows ( 31250 / 255 ) -- this timer is for updating the screen
	// 31250 fits into 8 bit integer ~120 times -- we use less overflows for slightly faster FPS
	TCCR0B |= (1 << CS01) | (1 << CS00); // 64 pre-scaler
	TCNT0 = 0; // count up starting here ; we need to overflow 100 times to get ~ < 100 ms updates
	TIMSK0 = (1 << TOIE0); // enable overflow interrupt
	
	
	// Load 41666 into TCNT1 -- this counter is for updating pac mans animation
	// this setup achieves ~150 ms counters
	TCCR1B |= (1 << CS11); // 8 pre-scaler
	TCNT1 = 41666; // count up starting here
	TIMSK1 = (1 << TOIE0); // enable overflow interrupt
	
	sei(); // enable global interrupt
	return;
}
