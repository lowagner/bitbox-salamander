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

#include "sprite_hero.h"
#include "miniz.h"

#define X(room) extern const struct MapDef map_##room;
	ALL_ROOMS 
#undef X

// rom
struct RoomDef {
	const struct MapDef *def;
	void (*start)(uint8_t );
	void (*frame)();
	uint8_t (*obj_col)(const struct ExtraObject *eo);
	uint8_t (*bg_col)(uint8_t);
	void (*done)();
};

const struct RoomDef room_defs[] = {
#define X(room) { \
	&map_##room,\
	room##_enter,\
	room##_frame,\
	room##_object_collide,\
	room##_background_collide,\
	room##_exit,\
	},
	ALL_ROOMS
#undef X
};

// current room, in ram
struct Room {
	int id; // current room id

	object *tilemap;
	uint8_t *tmap;

	int nb_objects; // can be larger / smaller than def
	struct ExtraObject objects[MAX_OBJECTS];

} room;


// const uint8_t * tile_properties; // ?? terrains ! 
struct ExtraObject player;


void room_load(int room_id, int entry)
{
	const struct MapDef *def = room_defs[room_id].def; // shortcut

	room.id = room_id;
	
	room.nb_objects = def->nb_objects;
	room.tmap = load_resource(def->tilemap);
	room.tilemap = tilemap_new(
		load_resource(def->tileset),
		0,0,
		TMAP_HEADER(def->tilemap_w,def->tilemap_h,def->tilesize,TMAP_U8) | TSET_8bit,
		room.tmap
		);

	
	// create objects / allocate sprites 
	for (int i=0;i<def->nb_objects;i++) {

		const struct MapObjectDef *mo = &def->objects[i];

		// load sprite as needed
		struct ExtraObject *eo = &room.objects[i];

		eo->vx=0; 
		eo->vy=0;	
		eo->frame=0;  
		eo->state=mo->state_id;
		eo->def = mo->sprite;
		void *spr_data = load_resource(mo->sprite->file); // load *or reference it if already loaded*
		eo->spr = sprite_new(spr_data,mo->x,mo->y,0);
	}

	// room callback
	room_defs[room_id].start(entry);
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
		if (eo->frame >= std->nb_frames)
			eo->frame=0;
	}	
	eo->spr->fr = std->frames[eo->frame]; 
}

// updates return 


static inline uint8_t bg_property_at(int x, int y)
{
	// XXX scroll offset
	uint8_t tileid = room.tmap[y/16*room_defs[room.id].def->tilemap_w+x/16]; 
	// 4 corners in a tile
	uint16_t ter_tile = room_defs[room.id].def->terrains[tileid-1]; // 1-based
	if (x%16<8) { 	// left
		return y%16<8 ? (ter_tile>>12)&0xf : (ter_tile>>4)&0xf;
	} else { 		// right
		return y%16<8 ? (ter_tile>> 8)&0xf : (ter_tile>>0)&0xf;
	}
}


// update object collision state for each corner of the sprite
Quad bg_collide(struct ExtraObject *eo)
{
	const struct StateDef* def = &eo->def->states[eo->state];
	// position that the sprite WOULD have considering its speed
	const int bx1=eo->spr->x+eo->vx+def->x1;
	const int by1=eo->spr->y+eo->vy+def->y1;
	const int bx2=eo->spr->x+eo->vx+def->x2;
	const int by2=eo->spr->y+eo->vy+def->y2;

	uint8_t (*f)(uint8_t) = room_defs[room.id].bg_col;

	uint8_t a = f(bg_property_at (bx1,by1));
	uint8_t b = f(bg_property_at (bx2,by1));
	uint8_t c = f(bg_property_at (bx1,by2));
	uint8_t d = f(bg_property_at (bx2,by2));

	return (Quad){.b={a,b,c,d}};
}



inline uint32_t collide (
	int plx1, int ply1, int plx2, int ply2,
	int x1,   int y1,   int x2,   int y2
	)
{
	// collision or not
	if (plx1>x2 || plx2<x1 || ply2<y1 || ply1>y2)       
		return 0;
	
	// checks if ABCD. only 2 checks, others are done
	const int plxm = (plx1+plx2)/2;
	const int plym = (ply1+ply2)/2;

	uint32_t res=0;
	
	if (!(x1>plxm || y1>plym))  
		res |= 1<<24;
	if (!(x2<plxm || y1>plym)) 
		res |= 1<<16;
	if (!(x1>plxm || y2<plym))
		res |= 1<<8;
	if (!(x2<plxm || y2<plym))
		res |= 1;
		
	return res;
}


// room transforms the collision type into collision type
void obj_collide( Quad *q ) 
{
	const struct StateDef* def = &player.def->states[player.state];
	// position that the sprite WOULD have considering its speed
	const int plx1=player.spr->x+player.vx+def->x1;
	const int ply1=player.spr->y+player.vy+def->y1;
	const int plx2=player.spr->x+player.vx+def->x2;
	const int ply2=player.spr->y+player.vy+def->y2;

	// collide with each object on map, also updating it
	uint8_t (*f)(const struct ExtraObject*) = room_defs[room.id].obj_col; // room collision callback

	for (int i=0;i<room.nb_objects;i++) {
		const struct ExtraObject *eo = &room.objects[i];

		const struct StateDef *std = &(eo->def->states[eo->state]);
		// get hit boxes
		const int ax1=eo->spr->x+eo->vx+std->x1;
		const int ay1=eo->spr->y+eo->vx+std->y1;
		const int ax2=eo->spr->x+eo->vy+std->x2;
		const int ay2=eo->spr->y+eo->vy+std->y2;

		// first AABB collision
		if ((ax1 > plx2) || (ax2 <  plx1) || (ay2 <  ply1) || (ay1 > ply2))
			continue;

		// do collide
		uint8_t coltype = f(eo);

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

void player_control(struct ExtraObject *player)
{
	// update player from controls
	if ( GAMEPAD_PRESSED(0,left) ) {
		player->vx=-1;
		object_set_state(player,state_hero_walk_l);
	} else if ( GAMEPAD_PRESSED(0,right) ) {
		player->vx=1;
		object_set_state(player,state_hero_walk_r);
	} else {
		player->vx=0;
	}

	if ( GAMEPAD_PRESSED(0,up)) {
		player->vy =-1;
		object_set_state(player,state_hero_walk_up);
	} else if ( GAMEPAD_PRESSED(0,down) ) {
		player->vy = 1;
		object_set_state(player,state_hero_walk_dn);
	} else {
		player->vy=0;
	}

	// idle states after timer ?

}

// reject if bumps into blocking, depending on which part touches, or make it slide sideways
// a = topleft ... d=bottom right
void block_object(struct ExtraObject *eo, Quad collision)
{	
	uint8_t a = collision.b[0];
	uint8_t b = collision.b[1];
	uint8_t c = collision.b[2];
	uint8_t d = collision.b[3];

	if (eo->vy<0 && (a||b)) {
		// shift left or right if only part blocks
		if (!b) {
			eo->vx+=1;
		} else if (!a) {
			eo->vx-=1;
		}
		eo->vy=0;
	} else if (eo->vy>0 && (c||d)) {
		// shift left or right if only part blocks
		if (!d) {
			eo->vx+=1;
		} else if (!c) {
			eo->vx-=1;
		}
		eo->vy=0;		
	} 

	if (eo->vx<0 && (a||c)) {
		// shift up/dn if only part blocks
		if (!c) {
			eo->vy+=1;
		} else if (!a) {
			eo->vy-=1;
		}
		eo->vx=0;		
	} else if (eo->vx>0 && (b||d)) {
		if (!d) {
			eo->vy+=1;
		} else if (!b) {
			eo->vy-=1;
		}
		eo->vx=0;		
	} 
}



void game_init()
{
	resource_init();
	window_init(); // load window + hud

	player.spr = sprite_new(load_resource(data_hero_spr),0,0,-1); 
	player.def = &sprite_hero;

	room_load(room_start,0);
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

	player_control(&player); // reads gamepad, updates player vx, vy and control
	// objects control ?

	// now check potential collisions and handle them

	// reads terrain_id of player corners (after movement)
	Quad collide = bg_collide (&player); 

	// player can collide with objects onscreen. update status
	obj_collide(&collide); 

	// adjusts current speed according to blocking status of each corner
	block_object (&player, collide);

	// update objects 

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