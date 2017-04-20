#include "miniz.h"
#include "behaviours.h"

#define MAPS_IMPLEMENTATION

#include "map_start_underground.h"


void start_underground_enter(uint8_t entry)
{
	player.x = 120;
	player.y = 580;

	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];
		switch(eo->type) {
			case type_start_underground_rat : 
				eo->update = rat_update;
				eo->collide = collide_hurt;
				break;

			case type_start_underground_rat_sleep : 
				eo->collide = collide_hurt;
				break;

			case type_start_underground_exit1 : 
				eo->data.b[0]=room_start_ug_laby;
				eo->data.b[1]=0;
				eo->collide = collide_exit;
				break;
		}
	}
}


void start_underground_frame()
{
	if (!status.start_sub_shown_msg) {
		status.start_sub_shown_msg = true;
		window_dialog (PLAYER, _("An underground passageway in my room !"),0);
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

