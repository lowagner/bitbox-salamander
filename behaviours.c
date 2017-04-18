// objects behaviours

#include <stdlib.h> // rand
#include "behaviours.h"

// get all useful sprites/maps defs. not sure you can use X macros here 
#include "sprite_rat.h"
#include "sprite_items16.h"
#include "sprite_hero.h"

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
		if ( object_bg_collide(this).w || !this->data.w) { 
			this->state += 4;
			rat_setspeed(this);
			this->data.w = rand() % 64; // pause before re start
		} else {
			this->data.w--;
		}
	}
}	

uint8_t collide_hurt(struct ExtraObject *this) 
{
	if (room.invincibility_frames) 
		return col_walk;

	message ("ouch ! \n");
	// TODO play sound hurt
	if (status.life) 
		status.life--; 
	// TODO check game over
	room.invincibility_frames = 60;
	window_draw_hud();
	return col_block; // bump ?
}

uint8_t collide_canpull(struct ExtraObject *this)
{
	// can lift : empty hands and use button
	if (GAMEPAD_PRESSED(0,B) && room.hold == 0) {
		// to pull state
		object_set_state(&player, player.state | 3 );
		room.hold = this;
	}
	return col_block;
}


// coffer. can react to being opened by giving object ref (data.w) - if not already opened !
uint8_t collide_chest(struct ExtraObject *this)
{
	if (GAMEPAD_PRESSED(0,B)) {
		object_set_state(this,state_items16_chest_open); // open it
		int old_state = player.state; // save player state
		object_set_state(&player, state_hero_receive);

		// force update of frames now
		object_transfer(&player);
		object_transfer(this);

		// display item as a raw sprite
		object *spr= sprite_new(
				sprite_items16.file,
				player.spr->x+15,
				player.spr->y+2,
				-1); // front of player
		// TODO frame from obj_type !

		for (int i=0;i<10;i++) {
			spr->y-=1;
			wait_vsync(6);
		}

		blitter_remove(spr); // FIXME crash on exit room ?
		
		object_set_state(&player, old_state);
		// TODO inventory update

		// will not give anymore
		this->collide = 0;


	}
	return col_block;
}