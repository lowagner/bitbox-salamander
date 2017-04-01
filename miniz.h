#pragma once
#include "lib/blitter/blitter.h"

#define ALL_ROOMS \
	X(beach) \
	X(start) \
	X(town) \
	X(bateau) \


// room defs
enum room_id_list {
#define X(rm) room_##rm,
	ALL_ROOMS
#undef X
};


// faces : line<<4|col
#define PLAYER 0x44
#define PNJ1 1
#define FATHER 0x66


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
int  window_dialog (int face_id, char *msg, char *answers);

// Blocking wait for new joystick button pressed, send it.
int wait_joy_pressed() ;

// Sprites
struct ExtraObject {
	const struct SpriteDef *def;
	object *spr;

	uint8_t frame; // within animation
	uint8_t state;
	int8_t vx,vy;	
};

extern struct ExtraObject player;

// declare callbacks
#define X(room) \
  void room##_enter(uint8_t);\
  void room##_frame();\
  uint8_t room##_object_collide(const struct ExtraObject *eo);\
  uint8_t room##_background_collide(uint8_t);\
  void room##_exit();

  ALL_ROOMS
#undef X

void resource_init(void);
void *load_resource(const void *data);
void objects_collisions( void (*f)(struct ExtraObject *) );
