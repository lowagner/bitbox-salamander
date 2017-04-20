#include "bitbox.h"
#include "miniz.h"
#include "behaviours.h"

#define MAPS_IMPLEMENTATION
#include "map_start.h"

const char *window_msgs[4] = {
	_("Such a vast world and I'm here !"),
	_("I can't sleep ! I want to go out !"),
	_("The moon is so nice"),
	_("Why can't I go see what's outside"),
};

static void window_dlg()
{
	static int dlg;
	window_dialog(PLAYER,window_msgs[dlg++%4],"ok");
	window_draw_hud();
}

void start_enter(uint8_t entry)
{
	// only one entry 
	player.x = 150;
	player.y = 60;

	for (int i=0;i<room.nb_objects;i++) {
		switch (room.objects[i].type) {
		case type_start_bed : 
			room.objects[i].collide = collide_canpull;
			break;
		case type_start_chest : 
			room.objects[i].data.w = state_items16_letter; // not give, special in fact
			room.objects[i].collide = collide_chest;
			break;
		}

	}
}

uint8_t start_background_collide(uint8_t bgtype)
{
	static int dlg_frame;
	// once per frame


	switch (bgtype) {
		case terrain_start_stair : 
			wait_vsync(20);
			room_load(room_start_town,0);
			return col_block;

		case terrain_start_window : 
			if (vga_frame>dlg_frame) { // once max per frame
				window_dlg(); 
				dlg_frame=vga_frame;
			}
			return col_block;

		case terrain_start_hole : 
			// must have all feet there
			player_fall(room_start_underground,0);
			return col_walk;

		default: 
			return bgtype;
	}
}

void start_frame()
{
}

void start_exit()
{
}