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
#include "lib/blitter/mapdefs.h"

#define TINYMALLOC_IMPLEMENTATION
#include "lib/resources/tinymalloc.h"
#undef TINYMALLOC_IMPLEMENTATION

#define TINYLZ4_IMPLEMENTATION
#include "lib/resources/tinylz4.h" 
#undef TINYLZ4_IMPLEMENTATION

#define RES_IMPLEMENTATION
#include "data.h"
#undef RES_IMPLEMENTATION

#include "sprite_hero.h"
#include "miniz.h"


// Runtime 
struct ExtraObject {
	const struct SpriteDef *def;
	object *spr;

	uint8_t frame; // within animation
	uint8_t state;
	int8_t vx,vy;	
};

struct Room {
	int id; // current room id
	int nb_objects;
	object *tilemap;
	const struct MapDef *def;

	struct ExtraObject objects[MAX_OBJECTS];
	uint8_t *tmap;
} room;


// room defs. call with room_defs[room.id].start()

#define X(room) extern const struct MapDef map_##room;
	ALL_ROOMS 
#undef X

struct RoomDef {
	const struct MapDef *def;
	void (*start)();
	void (*frame)();
	void (*collide)();
	void (*done)();
};

const struct RoomDef room_defs[] = {
#define X(room) {&map_##room, room##_enter ,room##_frame, room##_exit},
	ALL_ROOMS
#undef X
};


// const uint8_t * tile_properties; // ?? terrains ! 
struct ExtraObject player;


#define MEM_SIZE (80*1024)
char RAM[MEM_SIZE];


void room_load(int room_id)
{
	const struct MapDef *def = room_defs[room_id].def; // shortcut

	room.id = room_id;
	room.def = def;
	
	room.nb_objects = def->nb_objects;

	room.tilemap = tilemap_new(
		load_resource(def->tileset),
		0,0,
		TMAP_HEADER(def->tilemap_w,def->tilemap_h,def->tilesize,TMAP_U8) | TSET_8bit,
		load_resource(def->tilemap)
		);
	
	// load sprites from level

	void *spr_data[def->nb_sprites]; // define dyn array on stack
	for (int i=0; i<def->nb_sprites; i++) {
		spr_data[i]=load_resource(def->sprites[i]->sprfile); 
	}
	
	// create objects
	for (int i=0;i<def->nb_objects;i++) {

		const struct MapObjectDef *mo = &def->objects[i];
		// load sprite as needed, mor are 

		struct ExtraObject *o = &room.objects[i];

		o->def = def->sprites[i];
		o->vx=0; 
		o->vy=0;	
		o->frame=0;  
		o->state=mo->state_id;

		o->spr = sprite_new(spr_data[i],mo->x,mo->y,0);
	}

	// room callback
	room_defs[room_id].start();

}

void game_init()
{
	t_addchunk(&RAM, sizeof(RAM));

	window_init(); // load window + hud

	// load map -> structured map file ? set file 
	room_load(room_start);

	// player enters room at given position 
	player.spr = sprite_new(load_resource(res_hero_spr),0,0,-1); 
	player.spr->x = 150;
	player.spr->y = 60;
	player.def = &sprite_hero;
	
}


void object_set_state(struct ExtraObject *o, int state)
{
	if (o->state!=state) {
		o->state = state;
		o->frame=0;
	}
}

void object_anim_frame(struct ExtraObject *eo)
{
	const struct StateDef *std = &eo->def->states[eo->state];

	if (vga_frame%8==0) { // animate : 1 frame = 133ms
		// loop at end of animation.
		eo->frame += 1;
		if (eo->frame >= std->nb_frames+1)
			eo->frame=0;
	}	
	eo->spr->fr = std->frames[eo->frame]; 
}

void start_object_collide(struct ExtraObject *eo)
{
	message("colliding with object of type %x in state %x\n",eo->def,eo->state );
}

#if 0
// http://kishimotostudios.com/articles/aabb_collision/
void  objects_collisions( void ) 
{
	// position tha the player WOULD have considering its speed

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


// reject if bumps into blocking, depending on which part touches, or make it slide sideways
void start_background_collide(uint8_t a, uint8_t b,uint8_t c, uint8_t d)
{	
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

#endif 

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
		object_set_state(&player,state_hero_walk_l);
	} else if ( GAMEPAD_PRESSED(0,right) ) {
		player.vx=1;
		object_set_state(&player,state_hero_walk_r);
	} else {
		player.vx=0;
	}

	if ( GAMEPAD_PRESSED(0,up)) {
		player.vy =-1;
		object_set_state(&player,state_hero_walk_up);
	} else if ( GAMEPAD_PRESSED(0,down) ) {
		player.vy = 1;
		object_set_state(&player,state_hero_walk_dn);
	} else {
		player.vy=0;
	}

	// idle states after timer ?

	#if 0
	// -- Collision of objects with player
	objects_collisions();

	// tilemap 
	start_background_collide(  // depends on room
		bg_property_at (player.spr->x + window_hitbox[player.state][0],player.spr->y + window_hitbox[player.state][1]), 
		bg_property_at (player.spr->x + window_hitbox[player.state][2],player.spr->y + window_hitbox[player.state][1]), 
		bg_property_at (player.spr->x + window_hitbox[player.state][0],player.spr->y + window_hitbox[player.state][3]),
		bg_property_at (player.spr->x + window_hitbox[player.state][2],player.spr->y + window_hitbox[player.state][3])
		);
	#endif 

	// -- Update positions / animations
	for (int i=0;i<room.nb_objects;i++) {
		
		struct ExtraObject *eo=&room.objects[i];
		
		eo->spr->x += eo->vx;
		eo->spr->y += eo->vy;
		object_anim_frame(eo);
	}

	
	player.spr->x += player.vx;
	player.spr->y += player.vy;
	object_anim_frame(&player);

	// room callback
	room_defs[room.id].frame();
}