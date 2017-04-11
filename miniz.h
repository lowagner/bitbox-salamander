#pragma once
#include <stdbool.h>
#include "lib/blitter/blitter.h"

#define NULL ((void *)0)

#define ALL_ROOMS \
	X(beach) \
	X(start) \
	X(town) \
	X(bateau) \
	X(town_nuit) \
	X(start_underground) \


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

// Window
void window_init();
void window_set(int y_target);
void window_draw_hud();
int  window_dialog (const int face_id,const char *msg,const char *answers);

// Blocking wait for new joystick button pressed, send it.
int wait_joy_pressed() ;

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


// room status, in ram
struct Room {
	int id; // current room id

	object *background;
	uint8_t *tmap;

	int nb_objects; // can be larger / smaller than def
	struct ExtraObject objects[MAX_OBJECTS];
	
	struct ExtraObject *hold; // pulling / lifting
	
};

// global status (saved)
struct Status {
	unsigned life: 5; // shown as half-hearts
	unsigned avail_life: 5; 
	unsigned mana: 4;
	
	// inventory
	unsigned blue_potion: 3;
	unsigned bombs: 5;
	unsigned arrows: 5;
	unsigned gold: 7;
	
	// switches
	unsigned town_nuit_guard_talked: 2;
	unsigned start_sub_shown_msg:1;
};

extern struct Room room;
extern struct Status status; 

// room functions
void object_set_state(struct ExtraObject *o, int state);
void objects_collisions( void (*f)(struct ExtraObject *) );

void player_canpull(const struct ExtraObject *eo); // the player can pull this in this frame

void resource_init(void);
void *load_resource(const void *data);
void resource_unload_all();

void room_load(int room_id, int entry);
void bg_scroll(void);
Quad bg_collide(struct ExtraObject *obj);