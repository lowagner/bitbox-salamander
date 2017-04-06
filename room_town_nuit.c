#include "miniz.h"

#define MAPS_IMPLEMENTATION
#include "map_town_nuit.h"



// dialogs for town
static int guard_dialog() 
{
	window_set(0); // hide hud

	int ans;
	window_dialog (GUARD_NEUTRAL, 
		"Hi boy, what are you doing ?\n"
		"I thought your father never let you go out alone ?",
		"ok");

	ans= window_dialog (PLAYER, "Well, ...",
		"mind your own business !\n" 
		"my father is not well !\n"
		"I'm waiting for him !"
	);

	switch (ans) {
		case 0 : 
			window_dialog (GUARD_ANGRY, "Young boy, I'm your friend father,\n"
				"and I don't like the way you talk to me.\n"
				"And the fact that you're alone at this\n"
				"late hour is my business indeed.\n"
				"Come here, I'll bring you home.",0);
			ans= 1;
			break;
		case 1 : 
			window_dialog (GUARD_NEUTRAL, 
				"Oh, your father is in bad shape ?\n"
				"Well, it's nice that you found me,\n"
				"I'll never let him down alone.\n"
				"We're going to your house at once !\n", 0);
			window_dialog (PLAYER, "Oh, well ...",0);
			ans= 0;
			break;
		case 2 : 
		default: 
			window_dialog (GUARD_NEUTRAL,
				"No I haven't seen him just here.\n"
				"In fact I was ... returning from\n"
				"the Happy Boar tavern and returning\n"
				"home. Let me escort you to your house anyway...\n",0);
			window_dialog (PLAYER, "...",0);
			ans= 2;
	}

	window_set(0);
	window_draw_hud();
	return ans;
}


static const uint16_t entries[][2] = {{124,330}};
void town_nuit_enter(uint8_t entry)
{
	player.x = entries[entry][0];
	player.y = entries[entry][1];
}

inline static int sign(int a) { return (a>0)-(a<0); }

void town_nuit_frame()
{
	// update guard
	for (int i=0;i<room.nb_objects;i++)
	{
		struct ExtraObject *ob = &room.objects[i];
		if (room.objects[i].type==type_town_nuit_guard) {
			ob->vx = 2*sign( player.x - ob->x);
			ob->vy = 2*sign( player.y - ob->y);
			break;
		}
	}

}
void town_nuit_exit()
{
	
}

uint8_t town_nuit_background_collide(uint8_t bgtype)
{
	return bgtype;
}


uint8_t town_nuit_object_collide(const struct ExtraObject *eo)
{
	switch(eo->type) {
		case type_town_nuit_guard : 
			guard_dialog();
			wait_vsync(20);
			room_load(room_start,0);

		break;
	}
	return col_block;
}

