#include "bitbox.h"
#include "miniz.h"

#define MAPS_IMPLEMENTATION
#define SPRITE_IMPLEMENTATION
#include "map_start.h"

const char *window_msgs[4] = {
	"Such a vast world and I'm here !",
	"I can't sleep ! I want to go out !",
	"The moon is so nice",
	"Why can't I go see what's outside",
};

static void window_dlg()
{
	static int dlg;
	window_dialog(PLAYER,window_msgs[dlg++%4],"ok");
	window_draw_hud();
}

void start_enter(uint8_t entry)
{
	message("entering room : start\n");
	player.x = 150;
	player.y = 60;
}

uint8_t start_background_collide(uint8_t bgtype)
{
	static int dlg_frame;
	// once per frame


	switch (bgtype) {
		case terrain_start_stair : 
		// sound stair ?
		wait_vsync(20);
		room_load(room_town_nuit,0);

		return col_block;

		case terrain_start_window : 
		if (vga_frame>dlg_frame) { // once max per frame
			window_dlg(); 
			dlg_frame=vga_frame;
		}
		return col_block;

		default: 
		return bgtype;
	}
}


uint8_t start_object_collide(const struct ExtraObject *eo)
{
	return col_block;
}

void start_frame()
{
}

void start_exit()
{
}