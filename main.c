//si touche & pullable OU portable (comme block mais plus haute prio, besoin des deux passe en mode pull)
/* TODO : 

- window display : dialogs a revoir (progressif, borders collide, button skip)
- hud
- unify room / state ?
- porter 
- shared tilesets
- special cased blitters ? + nonclipped lines : custom blitter lib

*/
#include "stdbool.h"

#include "bitbox.h"
#include "lib/blitter/blitter.h"
#include "lib/blitter/mapdefs.h"

#include "sprite_hero.h"
#include "miniz.h"

#define START_ROOM  room_start
#define START_ENTRY 0

#define X(room) extern const struct MapDef map_##room;
	ALL_ROOMS 
#undef X

// room
struct RoomDef {
	const struct MapDef *def;
	void (*start)(uint8_t );
	void (*frame)();
	uint8_t (*bg_col)(uint8_t);
	void (*exit)();
};

// declare callbacks
#define X(room) \
  void room##_enter(uint8_t);\
  void room##_frame();\
  uint8_t room##_background_collide(uint8_t);\
  void room##_exit();

  ALL_ROOMS
#undef X

const struct RoomDef room_defs[] = {
#define X(room) { \
	&map_##room,\
	room##_enter,\
	room##_frame,\
	room##_background_collide,\
	room##_exit,\
	},
	ALL_ROOMS
#undef X
};

struct Room room;
struct Status status;


// const uint8_t * tile_properties; // ?? terrains ! 
struct ExtraObject player;


void room_load(int room_id, int entry)
{
	// unload preceding room data
	if (room.background) {	// there was a previous room
		room_defs[room_id].exit();

		for (int i=0;i<room.nb_objects;i++) {
			blitter_remove(room.objects[i].spr);
			room.objects[i].update = NULL;
			room.objects[i].collide = NULL;
		}
		room.nb_objects=0;
		blitter_remove(room.background);
		resource_unload_all();
	}

	// now load it
	const struct MapDef *def = room_defs[room_id].def; // shortcut

	room.id = room_id;
	
	room.nb_objects = def->nb_objects;
	room.tmap = load_resource(def->tilemap);
	room.background = tilemap_new(
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
		eo->x = mo->x;
		eo->y = mo->y;
		eo->frame=0;  
		eo->state=mo->state_id;
		eo->def = mo->sprite;
		eo->type = mo->type;
		void *spr_data = load_resource(mo->sprite->file); // load *or reference it if already loaded*
		eo->spr = sprite_new(spr_data,0,0,0);
		// XXX callbacks
	}
	object_set_state(&player, state_hero_idle_dn);
	
	message("RAM left after load : %d bytes\n",t_available());

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

	// transfer coordinates
	eo->spr->x = eo->x+room.background->x;
	eo->spr->y = eo->y+room.background->y;

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

void bg_scroll(void)
{
	// if possible put player in center of bg

	// move viewport to entry position so that player.x = 160 player.y=120
	room.background->x = -player.x+VGA_H_PIXELS/2;
	room.background->y = -player.y+VGA_V_PIXELS/2;

	// hit borders
	if (room.background->x>0)
		room.background->x=0;

	if (room.background->y>0)
		room.background->y=0;

	if (room.background->x+room.background->w < VGA_H_PIXELS)
		room.background->x=VGA_H_PIXELS-room.background->w;

	if (room.background->y+room.background->h < VGA_V_PIXELS)
		room.background->y=VGA_V_PIXELS-room.background->h;

}

// update object collision state for each corner of the sprite
Quad bg_collide(struct ExtraObject *eo)
{
	const struct StateDef* def = &eo->def->states[eo->state];
	// position that the sprite WOULD have considering its speed
	const int bx1=eo->x+eo->vx+def->x1;
	const int by1=eo->y+eo->vy+def->y1;
	const int bx2=eo->x+eo->vx+def->x2;
	const int by2=eo->y+eo->vy+def->y2;

	uint8_t (*f)(uint8_t) = room_defs[room.id].bg_col;

	uint8_t a = f(bg_property_at (bx1,by1));
	uint8_t b = f(bg_property_at (bx2,by1));
	uint8_t c = f(bg_property_at (bx1,by2));
	uint8_t d = f(bg_property_at (bx2,by2));

	return (Quad){.b={a,b,c,d}};
}


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

void player_control_walk(void)
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


void player_control_pull(void)
{
	// still pulling ?
	if (!GAMEPAD_PRESSED(0,B)) {
		room.hold = NULL;
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

	status.life_max = 6;
	status.life = 3;
	status.mana = 1;

	status.gold = 17;
	status.bombs = 8;
	status.arrows = 16;


	room_load(START_ROOM,START_ENTRY);
	window_draw_hud();

}

void game_frame()
{
	// -- Inputs 

	player_control(); // reads gamepad, updates player vx, vy and control

	// now check potential collisions and handle them

	// reads terrain_id of player corners (after movement)
	uint8_t old_room=room.id;
	Quad collide = bg_collide (&player); 
	if (old_room != room.id) return; // if we changed room, skip frame


	// player can collide with objects onscreen. update status
	player_obj_collide(&collide); 

	// adjusts current speed according to blocking status of each corner
	block_object (&player, collide);

	// update objects 


	// -- Update positions / animations
	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo=&room.objects[i];
		if (eo->update)
			eo->update(eo);

		eo->x += eo->vx;
		eo->y += eo->vy;
		object_anim_frame(eo);
	}
	
	// move hold object 
	if (room.hold) {
		room.hold->x += player.vx;
		room.hold->y += player.vy;
	}

	// move player
	player.x += player.vx;
	player.y += player.vy;
	object_anim_frame(&player);

	bg_scroll();

	// room callback
	room_defs[room.id].frame();

}