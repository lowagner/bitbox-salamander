#include "miniz.h"

#define MAPS_IMPLEMENTATION
#define SPRITE_IMPLEMENTATION
#include "map_town.h"

// dialogs for town
int town_dialog() 
{
	window_set(0); // hide hud

	int ans;
	window_dialog (PNJ1, 
		"Hi boy, what are you doing ?\n I thought your father never let you go out alone ?",
		"ok");

	ans= window_dialog (PLAYER, "Well, ...",
		"mind your own business !\n" 
		"my father is not well !\n"
		"I'm waiting for him !"
	);

	switch (ans) {
		case 0 : 
			window_dialog (PNJ1, "Young boy, I'm your friend father,\n and I don't like the way you talk to me.\n"
				"And the fact that you're alone at this\n late hour is my business indeed.\n"
				"Come here, I'll bring you home.",0);
			ans= 1;
			break;
		case 1 : 
			window_dialog (PNJ1, 
				"Oh, your father is in bad shape ?\n Well, it's nice that you found me,"
				" I'll never let him down alone.\n We're going to your house at once !\n", 0);
			window_dialog (PLAYER, "Oh, well ...",0);
			ans= 0;
			break;
		case 2 : 
		default: 
			window_dialog (PNJ1,
				"No I haven't seen him just here.\n"
				"In fact I was ... returning from\n"
				"the Happy Boar tavern and returning\n"
				"home. See you !\n",0);
			window_dialog (PLAYER, "It seemed to work ...",0);
			ans= 2;
	}

	window_set(0);
	window_draw_hud();
	return ans;
}


const uint16_t entries[][2] = {{176,624}};
void town_enter(uint8_t entry)
{
	
	player.spr->x = entries[entry][0];
	player.spr->y = entries[entry][1];

}
void town_frame()
{
	
}
void town_exit()
{
	
}

uint8_t town_background_collide(uint8_t bgtype)
{
	return bgtype;
}


uint8_t town_object_collide(const struct ExtraObject *eo)
{
	return col_block;
}

