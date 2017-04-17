#include "miniz.h"
#include "behaviours.h"

#define MAPS_IMPLEMENTATION

#include "map_start_underground.h"


void start_underground_enter(uint8_t entry)
{
	player.x = 120;
	player.y = 580;

	for (int i=0;i<room.nb_objects;i++) {
		if (room.objects[i].type == type_start_underground_rat) {
			room.objects[i].update = rat_update;
			room.objects[i].collide = collide_hurt;
		}
	}
}


void start_underground_frame()
{
	if (!status.start_sub_shown_msg) {
		status.start_sub_shown_msg = true;
		window_dialog (PLAYER, "An underground passageway in my room !",0);
		window_draw_hud();
	}
}

void start_underground_exit()
{
}

uint8_t start_underground_background_collide(uint8_t bgtype)
{
	return bgtype;
}

