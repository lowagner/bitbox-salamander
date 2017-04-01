#include "bitbox.h"
#include "miniz.h"

#define MAPS_IMPLEMENTATION
#define SPRITE_IMPLEMENTATION
#include "map_start.h"

static void stair_dlg()
{
	window_dialog (FATHER, 
		"Hi boy, what are you doing ?\n"
		"No, you can't go outside now,\n"
		"it's night ! Please stay in your\n"
		"room and sleep !",
		"ok");
	window_draw_hud();
}


void start_enter(uint8_t entry)
{
	player.spr->x = 150;
	player.spr->y = 60;	
}

uint8_t start_background_collide(uint8_t bgtype)
{
	static int dlg_frame;

	switch (bgtype) {
		case terrain_start_stair : 
		if (vga_frame>dlg_frame) { // once max per frame
			stair_dlg(); 
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