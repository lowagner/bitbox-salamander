#include "miniz.h"
#include "behaviours.h"

#define MAPS_IMPLEMENTATION

#include "map_start_underground.h"

static const uint16_t entries[][2] = {{120,580},{280,27}};


void start_underground_enter(uint8_t entry)
{

	player.x = entries[entry][0];
	player.y = entries[entry][1];

	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];
		switch(eo->type) {
			case type_start_underground_rat : 
				eo->update = rat_update;
				eo->collide = collide_hurt;
				eo->hit = rat_hit;
				break;

			case type_start_underground_exit1 : 
				eo->data.b[0]=room_start_labyrinth;
				eo->data.b[1]=0;
				eo->collide = collide_exit;
				break;

			// ladder exit to forest
			case type_start_underground_exit2 : 
				//eo->data.b[0]=room_forest;
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

