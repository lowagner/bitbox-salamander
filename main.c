/* TODO : 

- collisions tiles / 
- object types / callbacsks (frame, collide player) + collisions player / tiles
- implement packbit sprites out (+8bit) , blit them on-screen 

- window display
- special cased blitters ? + nonclipped lines
- multiroom ! 
*/
#include "stdbool.h"

#include "bitbox.h"
#include "lib/blitter/blitter.h"

void *load_resource( int res_id ); // resources.c
void resource_init();

#define TILEMAPS_IMPLEMENTATION
#include "data.h"
#include "room_start.h"

#include "room_window.h" // special "room" for window + always-on data

#define MAX_OBJECTS 16


struct ExtraObject {
	object *spr;
	uint8_t frame; // within animation
	uint8_t state;
	int8_t vx,vy;	
};

// reloaded each time 
struct Room {
	uint8_t **anims;
	uint8_t *state2type;

	int nb_objects;
	struct ExtraObject objects[MAX_OBJECTS];
	uint8_t *tmap;
	unsigned int tmap_w;
	
	void (*room_start)(); // at room start
	void (*room_collide)(); // collides with tile in room ?
};

object *tilemap;
uint8_t *tmap; // to room
const uint8_t * tile_properties;
unsigned int tmap_w;

int nb_objects;
struct ExtraObject objects[start_objects_nb]; // in sync with preceding (should merge and put fr to line directly ?... )
struct ExtraObject player;

void game_init()
{
	resource_init(); // setup memory map


	// load window + hud
	player.spr = sprite_new(load_resource((uintptr_t)data_window_hero_spr),0,0,-1); // direct reference


	// load level	
	tmap=load_resource(data_start_tmap);
	const void *tset=load_resource(data_start_tset);
	tilemap = tilemap_new(tset,0,0,start_header,tmap);
	tmap_w=TMAP_WIDTH(start_header);
	tile_properties=tset+start_tset_attrs_offset;
	
	// load sprites from level
	for (int i=0;i<start_t_nb;i++)
		start_sprites[i]=load_resource((uintptr_t)start_sprites[i]); 

	// create objects/monsters
	// respawn ? create player separately ?
	nb_objects=0; 
	for (int i=0;i<start_objects_nb;i++) {
		const struct MapObjectRef *mo = &start_objects[i];

		struct ExtraObject *o = &objects[nb_objects++];

		o->vx=0; o->vy=0;	
		o->frame=1;  
		o->state=mo->state_id;

		o->spr = sprite_new(start_sprites[start_types[o->state]],mo->x,mo->y,0);
	}

	// player enters room in given position 
	player.spr->x = 150;
	player.spr->y = 60;


	
}


void object_set_state(struct ExtraObject *o, int state)
{
	if (o->state!=state) {
		o->state = state;
		o->frame=1;
	}
}

void anim_frame(struct ExtraObject *eo)
{
	uint8_t **anims = eo==&player?window_anims:start_anims;
	if (vga_frame%8==0) { // animate : 1 frame = 133ms
		// loop was end of animation.
		eo->frame += 1;
		if (eo->frame >= anims[eo->state][0]+1)
			eo->frame=1;
	}	
	eo->spr->fr = anims[eo->state][eo->frame]; 
}

void start_object_collide(struct ExtraObject *o)
{
	message("colliding with object state %x type %x\n",o->state, start_types[o->state] );
}


// http://kishimotostudios.com/articles/aabb_collision/
void  objects_collisions( void ) 
{
	// position tha the player WOULD have

	const int bx1=player.spr->x+player.vx+window_hitbox[player.state][0];
	const int by1=player.spr->y+player.vy+window_hitbox[player.state][1];
	const int bx2=player.spr->x+player.vx+window_hitbox[player.state][2];
	const int by2=player.spr->y+player.vy+window_hitbox[player.state][3];

	for (int i=0;i<nb_objects;i++) {

		const int st=objects[i].state;

		// get hit boxes
		const int ax1=objects[i].spr->x+start_hitbox[st][0];
		const int ay1=objects[i].spr->y+start_hitbox[st][1];
		const int ax2=objects[i].spr->x+start_hitbox[st][2];
		const int ay2=objects[i].spr->y+start_hitbox[st][3];


		if (ax1 >= bx2) continue; // A is ToTheRightOf B
		if (ax2 < bx1)  continue; // A is ToTheLeftOf B
	 	if (ay2 < by1)  continue; // A is Above B
	 	if (ay1 >= by2) continue; // A is Below B
  	
	 	// there has been a collision
	 	// block ? handle as tile 
	 	player.vx=0;
	 	player.vy=0;

	 	start_object_collide(&objects[i]);

	}

}

static inline uint8_t bg_property_at(int x, int y)
{
	// vram at pos ?
	uint8_t tileid = tmap[y/16*tmap_w+x/16]; // XXX offset
	return tile_properties[tileid];
}



void start_background_collide(uint8_t a, uint8_t b,uint8_t c, uint8_t d)
{	

	// reject if bumps into blocking, depending on which part touches, or make it slide sideways

	// react to specials (1-4)
	// water etc.

	if (a||b||c||d) 
		message ("collision %02x %02x %02x %02x\n",a,b,c,d);

	if (player.vy<0) {
		if (( a | b ) & start_prop_blocking)
			player.vy=0;		
		// shift left or right if only part blocks
		if (( a & ~b) & start_prop_blocking) {
			player.vx+=1;
		} else if ((~a & b) & start_prop_blocking) {
			player.vx-=1;
		}
	} else if (player.vy>0) {
		if (( c | d ) & start_prop_blocking)
			player.vy=0;		
		// shift left or right if only part blocks
		if (( c & ~d) & start_prop_blocking) {
			player.vx+=1;
		} else if ((~c & d) & start_prop_blocking) {
			player.vx-=1;
		}
	} 

	if (player.vx<0) {
		if (( a | c ) & start_prop_blocking)
			player.vx=0;		
		// shift left or right if only part blocks
		if (( a & ~c) & start_prop_blocking) {
			player.vy+=1;
		} else if ((~a & c) & start_prop_blocking) {
			player.vy-=1;
		}
	} else if (player.vx>0) {
		if (( b | d ) & start_prop_blocking)
			player.vx=0;		
		// shift left or right if only part blocks
		if (( b & ~d) & start_prop_blocking) {
			player.vy+=1;
		} else if ((~b & d) & start_prop_blocking) {
			player.vy-=1;
		}
	} 


}


void game_frame()
{
	// -- Inputs 
	/*
	static uint16_t prev_buttons;
	uint16_t pressed_buttons = gamepad_buttons[0] & ~prev_buttons;  // just pressed up
	uint16_t released_buttons = prev_buttons & ~gamepad_buttons[0]; // just released
	prev_buttons=gamepad_buttons[0];
	*/

	// update player from controls
	if ( GAMEPAD_PRESSED(0,left) ) {
		player.vx=-1;
		object_set_state(&player,window_st_hero_walk_l);
	} else if ( GAMEPAD_PRESSED(0,right) ) {
		player.vx=1;
		object_set_state(&player,window_st_hero_walk_r);
	} else {
		player.vx=0;
	}

	if ( GAMEPAD_PRESSED(0,up)) {
		player.vy =-1;
		object_set_state(&player,window_st_hero_walk_up);
	} else if ( GAMEPAD_PRESSED(0,down) ) {
		player.vy = 1;
		object_set_state(&player,window_st_hero_walk_d);
	} else {
		player.vy=0;
	}

	// idle states ?

	// -- Collisions 

	objects_collisions();

	// tilemap 
	start_background_collide(  // depends on room
		bg_property_at (player.spr->x + window_hitbox[player.state][0],player.spr->y + window_hitbox[player.state][1]), 
		bg_property_at (player.spr->x + window_hitbox[player.state][2],player.spr->y + window_hitbox[player.state][1]), 
		bg_property_at (player.spr->x + window_hitbox[player.state][0],player.spr->y + window_hitbox[player.state][3]),
		bg_property_at (player.spr->x + window_hitbox[player.state][2],player.spr->y + window_hitbox[player.state][3])
		);

	// -- Update positions / animations
	for (int i=0;i<nb_objects;i++) {
		
		struct ExtraObject *eo=&objects[i];
		
		eo->spr->x += eo->vx;
		eo->spr->y += eo->vy;

		//anim_frame(eo);
	}

	
	player.spr->x += player.vx;
	player.spr->y += player.vy;

	anim_frame(&player);

}