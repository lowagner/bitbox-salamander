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
#include "miniz.h"


#define START_ROOM  room_start
#define START_ENTRY 0

#define X(room) extern const struct MapDef map_##room;
	ALL_ROOMS 
#undef X



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

// un/load all objects when entering a room
void room_load(int room_id, int entry)
{
	// unload preceding room data if there was a previous room
	if (room.background) {	
		room_defs[room_id].exit();

		for (int i=0;i<room.nb_objects;i++) {
			blitter_remove(room.objects[i].spr);
			room.objects[i].update = (void *) 0;
			room.objects[i].collide = (void *) 0;
		}
		room.nb_objects=0;
		blitter_remove(room.background);
		resource_unload_all();
	}
	wait_vsync(1);	// let blitter flush old objects 

	// here we only keep window and hero WHICH MUST BE IN FLASH 

	// now load map 
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

	player_reset();
	
	// room callback (will position player)
	room_defs[room_id].start(entry);

	// now player has right position. position bg and thus sprites.
	bg_scroll();
}

// if possible put player in center of bg by moving room
void bg_scroll(void)
{

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



void game_init()
{
	resource_init();
	window_init(); // load window + hud

	player_init();

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
	uint8_t old_room=room.id;
	// --- update player
	player_control(); // reads gamepad, updates player vx, vy and control
	// now check potential collisions and handle them
	// reads terrain_id of player corners (after movement)
	Quad collide = object_bg_collide (&player); 


	// player can collide with objects onscreen. update status
	player_obj_collide(&collide); 

	// adjusts current speed according to blocking status of each corner
	object_block (&player, collide);

	if (old_room != room.id) 
		return; // if we changed room, skip frame

	// move player
	player.x += player.vx;
	player.y += player.vy;
	object_anim_frame(&player);
	object_transfer(&player);
	bg_scroll();

	// special for player : handle invincibility (after transfer !)
	if (room.invincibility_frames) {
		if (vga_frame%4==0) 
			player.spr->y = VGA_H_PIXELS;
		room.invincibility_frames--;
	}


	// update objects 
	// -- Update positions / animations
	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo=&room.objects[i];
		if (eo->update)
			eo->update(eo);

		eo->x += eo->vx;
		eo->y += eo->vy;
		object_anim_frame(eo);
		object_transfer(eo);
	}
	
	// move hold object 
	if (room.hold) {
		room.hold->x += player.vx;
		room.hold->y += player.vy;
	}


	// room callback
	room_defs[room.id].frame();

}