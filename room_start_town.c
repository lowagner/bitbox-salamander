#include "miniz.h"

#define MAPS_IMPLEMENTATION
#include "map_start_town.h"

static void guard_dialog2()
{
	window_dialog (GUARD_ANGRY, _("Get back to sleep immediately ! "),0);
}

// dialogs for town
static int guard_dialog() 
{
	window_set(0); // hide hud

	int ans;
	window_dialog (GUARD_NEUTRAL, 
		_("Hi boy, what are you doing ?\n")
		_("I thought your father never let you go out alone ?"),

		_("...")
		);

	ans= window_dialog (PLAYER, _("Well, ..."),
		_("Mind your own business !\n" )
		_("My father is not well !\n")
		_("I'm waiting for him !")
	);

	switch (ans) {
		case 0 : 
			window_dialog (GUARD_ANGRY,
				_("Young boy, I'm a friend of your father, and I don't like the way you talk to me. ")
				_("And the fact that you're alone at this late hour is my business indeed. ")
				_("Come here, I'll bring you home."),
				0);
			ans= 1;
			break;
		case 1 : 
			window_dialog (GUARD_NEUTRAL, 
				_("Oh, your father is in bad shape ?\n")
				_("Well, it's good that you told me, I'll make sure to check in on him.\n")
				_("Let's go back inside."),0);
			window_dialog (PLAYER, _("Oh, well ..."),0);
			ans= 0;
			break;
		case 2 : 
		default: 
			window_dialog (GUARD_NEUTRAL,
				_("Well I haven't seen him around.\n")
				_("I was returning from the Happy Boar tavern and heading home.")
				_("Let's have you wait inside in any case..."),0);
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
void start_town_enter(uint8_t entry)
{
	player.x = entries[entry][0];
	player.y = entries[entry][1];
	for (int i=0;i<room.nb_objects;i++) {
		switch(room.objects[i].type) {
			case type_start_town_guard : 
				room.objects[i].update = update_guard;
				room.objects[i].collide = guard_nuit_collide;
			break;
		}
	}
}

void start_town_frame() {}
void start_town_exit() {}
uint8_t start_town_background_collide(uint8_t bgtype)
{
	return bgtype;
}
