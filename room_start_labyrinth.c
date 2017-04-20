#include "miniz.h"
#include "behaviours.h"

#define MAPS_IMPLEMENTATION
#include "map_start_labyrinth.h"

static uint8_t dialog_old (struct ExtraObject *this) 
{
	if (!status.start_laby_talked_old) {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone. \nTake this."),0);
		player_take_anim(1); // FIXME give wine
		window_dialog (PLAYER, _("Well, ... thank you"),"...");
		window_dialog (PNJ_OLD,_("What ?"),0);
		window_dialog (PLAYER, _("It's a bottle of wine ! \nDon't you think I'm too young for this ?"),0);
		window_dialog (PNJ_OLD,_("Ah ! Of course. Give it back to me"),0);
		window_draw_hud();
		status.start_laby_talked_old=1;
	} else if (status.sword==0) {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone. \nTake this."),0);
		player_take_anim(state_items16_stick); 
		window_dialog (PLAYER, _("Well, ... thank you"),"...");
		window_dialog (PNJ_OLD,_("What, *again* ?"),0);
		window_dialog (PLAYER, _("It's just a wood stick !"),0);
		int ans =  window_dialog (PNJ_OLD,_("Spoiled child !\n Do you want it or not ?"),_("No .. \nYes !"));
		if (ans) {
			// say something for usage
			status.sword=sword_stick;
		}		
	} else {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone.\nBut you have my stick now.\nLeave me alone."),0);
		window_dialog (PLAYER, _("But won't it be dangerous to be alone ?"),0);
		window_dialog (PNJ_OLD,_("uuh .. Anyway, let me heal you."),0);
		status.life = status.life_max;
	}
	window_draw_hud();
	return col_block;
}


static const uint16_t entries[][2] = {{20,44}};
void start_labyrinth_enter(uint8_t entry)
{	
	player.x = entries[entry][0];
	player.y = entries[entry][1];


	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];
		switch(eo->type) {
			case type_start_labyrinth_old : 
				eo->collide = dialog_old;
				break;
			case type_start_labyrinth_exit0 : 
				eo->data.b[0]=room_start_underground;
				eo->data.b[1]=1;
				eo->collide = collide_exit;
				break;
		}
	}

}
void start_labyrinth_frame()
{
	
}
void start_labyrinth_exit()
{
	
}

uint8_t start_labyrinth_background_collide(uint8_t bgtype)
{
	return bgtype;
}

