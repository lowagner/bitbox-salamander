// objects behaviours

#include <stdlib.h> // rand
#include "behaviours.h"

// get all useful sprites/maps defs. not sure you can use X macros here 
#include "sprite_rat.h"

// set speed according to state
static void rat_setspeed(struct ExtraObject *this)
{
	switch(this->state)
	{
		case state_rat_walk_u : 
			this->vx=0;
			this->vy=-2;
			break;
		case state_rat_walk_d : 
			this->vx=0;
			this->vy=2;
			break;
		case state_rat_walk_l : 
			this->vx=-2;
			this->vy=0;
			break;
		case state_rat_walk_r : 
			this->vx=2;
			this->vy=0;
			break;
		// pauses	
		default:
			this->vx=0;
			this->vy=0;
			break;
	}

}

void rat_update(struct ExtraObject *this) {
	// if collide or stop : pause a bit, turn left or right
	if (this->state >= state_rat_pause_r ) { // pause 
		if (this->data.w) {
			this->data.w--;
		} else {
			// un pause and turn
			if (this->state == state_rat_pause_l || this->state == state_rat_pause_r)			{
				this->state = rand()%2 ? state_rat_walk_d : state_rat_walk_u;
			} else {
				this->state = rand()%2 ? state_rat_walk_l : state_rat_walk_r;
			}
			rat_setspeed(this);
			this->data.w = rand() % 128; // till stop 
			return;
		}
	} else { // walking: if collide, corresponding pause 
		if ( bg_collide(this).w || !this->data.w) { 
			this->state += 4;
			rat_setspeed(this);
			this->data.w = rand() % 64; // pause before re start
		} else {
			this->data.w--;
		}
	}
}	

uint8_t collide_hurt(struct ExtraObject *this) {
	return col_hit1;
}

uint8_t collide_canpull(struct ExtraObject *this)
{
	// can lift : empty hands and use button
	if (GAMEPAD_PRESSED(0,B) && room.hold == NULL) {
		// to pull state
		object_set_state(&player, player.state | 3 );
		room.hold = this;
	}
	return col_block;
}
