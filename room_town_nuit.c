#include "miniz.h"

#define MAPS_IMPLEMENTATION
#include "map_town_nuit.h"

static void guard_dialog2()
{
	window_dialog (GUARD_ANGRY, "Get back to sleep immediately ! ",0);
}

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
	return ans;
}

static char has_met_guard=0;
static uint8_t guard_nuit_collide(struct ExtraObject *eo)
{
	if (has_met_guard<3) {
		has_met_guard++;
		guard_dialog();
	} else {
		guard_dialog2();
	}
	window_draw_hud();
	wait_vsync(20);
	room_load(room_start,0);

	return col_block;
}

inline static int sign(int a) { return (a>0)-(a<0); }
static void update_guard (struct ExtraObject *this)
{
	this->vx = 2*sign( player.x - this->x);
	this->vy = 2*sign( player.y - this->y);
}


static const uint16_t entries[][2] = {{124,330}};
void town_nuit_enter(uint8_t entry)
{
	player.x = entries[entry][0];
	player.y = entries[entry][1];
	for (int i=0;i<room.nb_objects;i++) {
		switch(room.objects[i].type) {
			case type_town_nuit_guard : 
				room.objects[i].update = update_guard;
				room.objects[i].collide = guard_nuit_collide;
			break;
		}
	}
}

void town_nuit_frame() {}
void town_nuit_exit() {}
uint8_t town_nuit_background_collide(uint8_t bgtype)
{
	return bgtype;
}