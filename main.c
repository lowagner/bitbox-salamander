// miniz main & rendering funcitons
#include <stdint.h>
#include "build/binaries.h"

uint8_t *bg_tilemap;


// - window functions 

#define WINDOW_W 30
#define WINDOW_H 14

uint8_t window[WINDOW_W * WINDOW_H];


// - room 
// ------------------------------------------------------------------
struct Object { 
	uint8_t x,y; //  position in tile
	uint8_t type,sprite; // type of element, graphical sprite 
};

struct Frame {
	 uint8_t idx;
	 uint8_t dx,dy; // offset wrt sprite corner
	 uint8_t height; // frame height 
};

struct Sprite {
	uint8_t w,nb_frames;
	struct Frame *frames; 
	uint8_t *palette;
	uint8_t *data;
};

struct {
	uint8_t lw, lh; // tilemap size
	uint8_t *tilemap;
	uint8_t *tileset;
	struct Object *objects; 
	struct Sprite sprites[16]; // maximum 16 sprites 
} room;


void enter_room(uint8_t *data)
{
 	// leave preceding room

 	// load room data
	room.lw = *data++;
	room.lh = *data++;
	uint8_t unique_indices = *data++;
	uint8_t nbobjects = *data++;

	room.tilemap = data;
	data += room.lw*room.lh;

	room.tileset = data;
	data += unique_indices;

	room.objects = data;
	data += sizeof(struct Object)*nbobjects;

	struct Sprite *spr=(struct Sprite *)room.sprites; // discards const
	while (*data) {
		// load a sprite
		spr->w = *data++;
		spr->nb_frames=*data++;
		uint16_t datasize = data[0]<<8 | data [1];
		data += 2;

		spr->palette=(uint8_t*) data; data +=16;

		spr->frames = (struct Frame*)data;
		data+=sizeof(struct Frame)*spr->nb_frames;

		spr->data = data;
		data += datasize;

		spr++; // next element
	}


}



void game_init (void) {
	enter_room(rooms_start_room);
}

void game_frame (void) {
	
}


