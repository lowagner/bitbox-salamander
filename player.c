#include "miniz.h"

#include "sprite_hero.h"

// collision between player and objects
// room transforms the collision type into collision type
void player_obj_collide( Quad *q ) 
{

	const struct StateDef* def = &player.def->states[player.state];
	// position that the sprite WOULD have considering its speed
	const int plx1=player.x+player.vx+def->x1;
	const int ply1=player.y+player.vy+def->y1;
	const int plx2=player.x+player.vx+def->x2;
	const int ply2=player.y+player.vy+def->y2;

	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];

		// dont collide with hold item
		if (eo == room.hold) 
			continue;

		const struct StateDef *std = &(eo->def->states[eo->state]);
		// get hit boxes
		const int ax1=eo->x+eo->vx+std->x1;
		const int ay1=eo->y+eo->vx+std->y1;
		const int ax2=eo->x+eo->vy+std->x2;
		const int ay2=eo->y+eo->vy+std->y2;

		// first AABB collision
		if ((ax1 > plx2) || (ax2 <  plx1) || (ay2 <  ply1) || (ay1 > ply2))
			continue;

		// callback based/update
		// TODO collision side ?
		uint8_t coltype = eo->collide ? eo->collide(eo) : col_block;

		// checks if ABCD. only 2 checks, others are done
		const int plxm = (plx1+plx2)/2;
		const int plym = (ply1+ply2)/2;

		// priority update of q
		if (!(ax1>plxm || ay1>plym))
			if (q->b[0]<coltype) q->b[0]=coltype;
		if (!(ax2<plxm || ay1>plym))
			if (q->b[1]<coltype) q->b[1]=coltype;
		if (!(ax1>plxm || ay2<plym))
			if (q->b[2]<coltype) q->b[2]=coltype;
		if (!(ax2<plxm || ay2<plym))
			if (q->b[3]<coltype) q->b[3]=coltype;
	}

}

// player states : RUDL x AWIP
inline bool player_iswalking() { return player.state % 4 == 1;  }
inline bool player_ispulling() { return player.state % 4 == 3; }
// keeps orientation
inline void player_setidle()   { object_set_state(&player,(player.state&~3) | 2 ); }

static void player_control_walk(void)
{
	// update player from controls
	if ( GAMEPAD_PRESSED(0,up) || gamepad_y[0] < -50 ) {
		player.vy =-1;
		if (!player.vx) 
			object_set_state(&player,state_hero_walk_up);
	} else if ( GAMEPAD_PRESSED(0,down) || gamepad_y[0] > 50 ) {
		player.vy = 1;
		if (!player.vx) 
			object_set_state(&player,state_hero_walk_dn);
	} else {
		player.vy=0;
	}

	if ( GAMEPAD_PRESSED(0,left) || gamepad_x[0] < -50 ) {
		player.vx=-1;
		if (!player.vy) 
			object_set_state(&player,state_hero_walk_l);

	} else if ( GAMEPAD_PRESSED(0,right) || gamepad_x[0] > 50) {
		player.vx=1;
		if (!player.vy) 
			object_set_state(&player,state_hero_walk_r);
	} else {
		player.vx=0;
	}


	// rest 
	if (!gamepad_buttons[0]) {
		if (player_iswalking())
			player_setidle();
	}
}


static void player_control_pull(void)
{
	// still pulling ?
	if (!GAMEPAD_PRESSED(0,B)) {
		room.hold = (void *)0;
		object_set_state(&player, (player.state & ~3) | 2); // idle
		return;
	}

	switch(player.state) {
		case state_hero_pull_r : 
			if (GAMEPAD_PRESSED(0,left)  || gamepad_x[0] < -50) {
				if (vga_frame % 8==0) 
					player.vx = -1; // decimal speed
			} else {
				player.vx = 0;
			}
		break;
			
		case state_hero_pull_l : 
			if (GAMEPAD_PRESSED(0,right) || gamepad_x[0] > 50) {
				if (vga_frame % 8==0) 
					player.vx = 1;
			} else {
				player.vx = 0;
			}
		break;
	}
}

void player_control(void)
{
	if (player_ispulling()) // pulling ?
		player_control_pull();
	else 
		player_control_walk();
}

void player_init(void)
{
	player.spr = sprite_new(load_resource(data_hero_spr),0,0,-1); 
	player.def = &sprite_hero;
}


void player_reset(void) // reset for room
{
	object_set_state(&player, state_hero_idle_dn); // reset player 
	player.vx = player.vy = 0;
}