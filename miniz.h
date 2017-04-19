#pragma once
#include <stdbool.h>
#include "lib/blitter/blitter.h"
#include "lib/blitter/mapdefs.h"


#define ALL_ROOMS \
	X(beach) \
	X(start) \
	X(town) \
	X(bateau) \
	X(town_nuit) \
	X(start_underground) \
	X(start_ug_laby) \


// room defs
enum room_id_list {
#define X(rm) room_##rm,
	ALL_ROOMS
#undef X
};

#define MESSAGE_SPEED 1 // 1-4
#define HUD_HEIGHT 4

// faces : line<<4|col
#define PLAYER 0x44
#define PNJ1 1
#define FATHER 0x66
#define GUARD_ANGRY 0x52
#define GUARD_NEUTRAL 0x51
#define PNJ_OLD 0x76


// quad u32<->4u8
typedef union {
	uint32_t w;
	uint8_t b[4];
} Quad;

enum CollisionType 
{
	col_walk, 
	col_block, // player just blocked
	col_bump,  // player is rejected
	col_hit1,  // rejected + invuln frames + gets hit points
	col_hit2,
	col_fall,
	col_water,
	col_grass,
};


// Sprites
struct ExtraObject {
	const struct SpriteDef *def;
	object *spr;

	uint8_t frame; // within animation
	uint8_t state;
	
	uint8_t type;

	int8_t vx,vy;	
	int16_t x,y; // on-level position 0,0 is the topleft of the map

	// callbacks
	void    (*update)  (struct ExtraObject *this);  
	uint8_t (*collide) (struct ExtraObject *this);  // returns a col_type

	Quad data; // extra data 
};

extern struct ExtraObject player;


// room temporary status, in ram
struct Room {
	int id; // current room id

	object *background;
	uint8_t *tmap;

	int nb_objects; // can be larger / smaller than def
	struct ExtraObject objects[MAX_OBJECTS];
	
	struct ExtraObject *hold; // pulling / lifting
	uint8_t invincibility_frames; // invincibility frames
};

// room static definitions (def is read only)
struct RoomDef {
	const struct MapDef *def;
	void (*start)(uint8_t );
	void (*frame)(void);
	uint8_t (*bg_col)(uint8_t);
	void (*exit)(void);
};

// all objects
enum {
	obj_sword_none,
	obj_sword_stick,
	obj_sword_rusted,
	obj_sword_metal,
	obj_sword_gold,
	obj_sword_enchanted,
	obj_letter,
};

// global status (saved)
struct Status {
	unsigned life: 5; // shown as half-hearts
	unsigned life_max: 5; 
	unsigned mana: 3;
	
	// inventory
	unsigned gold: 7;
	unsigned bombs: 5; 
	unsigned arrows: 5;
	unsigned sword: 3; // different types, only one at a time. see sword enum

	unsigned keys:2;

	// switches
	unsigned town_nuit_guard_talked: 2;
	unsigned start_sub_shown_msg:1;
	unsigned start_laby_talked_old:1;
};

extern struct Room room;
extern struct Status status; 
extern const struct RoomDef room_defs[];

// Window
void window_init();
void window_set(int y_target);
void window_draw_hud();
int  window_dialog (const int face_id,const char *msg,const char *answers);

// Blocking wait for new joystick button pressed, send it.
int wait_joy_pressed() ;
// room functions
void object_set_state(struct ExtraObject *o, int state);
void object_anim_frame(struct ExtraObject *eo);
void object_transfer(const struct ExtraObject *eo);
Quad object_bg_collide(struct ExtraObject *eo);
void object_block (struct ExtraObject *eo, Quad collision);

// player.c
void player_obj_collide( Quad *q );
void player_control(void);
void player_init(void);
void player_reset(void); // reset for room
void player_fall(int room_id, int entry); 
void player_take_anim(int object_type);

// resource 
void resource_init(void);
void *load_resource(const void *data);
void resource_unload_all(void);

// in main
void room_load(int room_id, int entry);
void bg_scroll(void);
