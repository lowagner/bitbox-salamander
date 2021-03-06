#include "miniz.h"

#include "sprite_hero.h"
#include "sprite_sword.h"
#include "sprite_items16.h" // for take

// check collision between player and objects
// room transforms the collision into collision type
void player_obj_collide( Quad *q ) 
{

	const struct StateDef* def = &player.def->states[player.state];
	// position that the sprite WOULD have considering its speed
	const int plx1=player.x+player.vx+def->x1;
	const int ply1=player.y+player.vy+def->y1;
	const int plx2=player.x+player.vx+def->x2;
	const int ply2=player.y+player.vy+def->y2;

	// position of sword if is attacking
	const struct StateDef* swdef = &sword.def->states[sword.state];
	const int swx1=sword.x+swdef->x1;
	const int swy1=sword.y+swdef->y1;
	const int swx2=sword.x+swdef->x2;
	const int swy2=sword.y+swdef->y2;

	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];

		// dont collide with hold item or sword
		if (eo == room.hold || eo == &sword) 
			continue;

		const struct StateDef *std = &(eo->def->states[eo->state]);
		// get hit boxes
		const int ax1=eo->x+eo->vx+std->x1;
		const int ay1=eo->y+eo->vx+std->y1;
		const int ax2=eo->x+eo->vy+std->x2;
		const int ay2=eo->y+eo->vy+std->y2;

		// check if collides with sword. if so, hit()
		if (!((ax1 > swx2) || (ax2 <  swx1) || (ay2 <  swy1) || (ay1 > swy2)) && eo->hit)
			eo->hit(eo);

		// first AABB collision with player
		if ((ax1 > plx2) || (ax2 <  plx1) || (ay2 <  ply1) || (ay1 > ply2))
			continue;

		// callback based/update
		// TODO send collision side ?
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
inline bool player_isattacking() { return player.state % 4 == 0; }
inline bool player_iswalking()   { return player.state % 4 == 1; }
inline bool player_ispulling()   { return player.state % 4 == 3; }

// keeps orientation
static void player_attack()  { 
	object_set_state(&player,(player.state&~3) | 0) ; 
	player.vx=0;
	player.vy=0; 
	// show & anim sword 
	sword.x = player.x;
	sword.y = player.y;
	object_set_state(&sword, player.state / 4); // FIXME depend on word type
}

static void player_setidle() { object_set_state(&player,(player.state&~3) | 2 ); }

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


	// start attacking
	if (GAMEPAD_PRESSED(0,A) && status.sword)
		player_attack();

	// rest 
	if (!gamepad_buttons[0]) {
		if (player_iswalking())
			player_setidle();
	}
}

// enter this state from object behaviour collide_canpull(..)
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

static void player_control_attack(void)
{
	// wait till the end and stop at the end.
	const struct StateDef *std = &sword.def->states[sword.state];

	if (vga_frame%8==7 && sword.frame == std->nb_frames-1 ) { 
		player_setidle();
		sword.y=1024; // hide weapon
	}
}

void player_control(void)
{
	if (player_ispulling()) // pulling ?
		player_control_pull();
	else if (player_isattacking())
		player_control_attack();
	else
		player_control_walk();
}


void player_fall(int room_id, int entry)
{
	object_set_state(&player, state_hero_falling);
	object_transfer(&player);
	wait_vsync(10);

	for (int i=0;i<30;i++) {
		object_anim_frame(&player);
		object_transfer(&player);
		wait_vsync(1);
	}
	room_load(room_id,entry);
}


void player_take_anim(int object_type)
{

	int old_state = player.state; // save player state
	object_set_state(&player, state_hero_receive);

	// force update of frames now
	object_transfer(&player);

	// display item as a raw sprite
	object *spr= sprite_new(
			sprite_items16.file,
			player.spr->x+15,
			player.spr->y+2,
			-1); // front of player
	spr->fr = sprite_items16.states[object_type].frames[0];

	for (int i=0;i<10;i++) {
		spr->y-=1;
		wait_vsync(6);
	}
	wait_vsync(20); // little pause

	blitter_remove(spr); 
	
	object_set_state(&player, old_state);
}

void player_update() 
{
	player_control(); // reads gamepad, updates player vx, vy and control. 

	// now check potential collisions and handle them
	// reads terrain_id of player corners (after movement)
	Quad collide = object_bg_collide (&player); 


	// player can collide with objects onscreen. update collide status
	player_obj_collide(&collide); 

	if (collide.w) message("collision : %x\n",collide.w);

	// adjusts current speed according to blocking status of each corner
	object_block (&player, collide);

	// move player
	player.x += player.vx;
	player.y += player.vy;

	object_anim_frame(&player);
	object_transfer(&player);

	if (status.sword) {	
		// check sword collision with objects

		object_anim_frame(&sword);
		object_transfer(&sword);
	}
	
	// move hold object 
	if (room.hold) {
		room.hold->x += player.vx;
		room.hold->y += player.vy;
	}

	bg_scroll();

	// special for player : handle invincibility (after transfer !)
	if (room.invincibility_frames) {
		if (vga_frame%4==0) 
			player.spr->y = VGA_H_PIXELS;
		room.invincibility_frames--;
	}
}

void player_init(void)
{
	player.spr = sprite_new(load_resource(data_hero_spr),0,0,-1); 
	player.def = &sprite_hero;

	sword.spr = sprite_new(load_resource(data_sword_spr),0,0,-2); // front of player
	sword.def = &sprite_sword;

}

void player_reset(void) // reset for room
{
	object_set_state(&player, state_hero_idle_dn); // reset player 
	player.vx = player.vy = 0;
	sword.y = 1024; // hide it.
}