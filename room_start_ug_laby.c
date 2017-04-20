#include "miniz.h"

#define MAPS_IMPLEMENTATION
#include "map_start_ug_laby.h"

static uint8_t dialog_old (struct ExtraObject *this) 
{
	if (!status.start_laby_talked_old) {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone. \nTake this."),0);
		// FIXME give wine
		player_take_anim(1);
		window_dialog (PLAYER, _("Well, ... thank you"),"...");
		window_dialog (PNJ_OLD,_("What ?"),0);
		window_dialog (PLAYER, _("It's a bottle of wine ! \nDon't you think I'm too young for this ?"),0);
		window_dialog (PNJ_OLD,_("Ah ! Of course. Give it back to me"),0);
		window_draw_hud();
		status.start_laby_talked_old=1;
	} else if (status.sword==0) {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone. \nTake this."),0);
		player_take_anim(2);
		window_dialog (PLAYER, _("Well, ... thank you"),"...");
		window_dialog (PNJ_OLD,_("What, *again* ?"),0);
		window_dialog (PLAYER, _("It's just a wood stick !"),0);
		int ans =  window_dialog (PNJ_OLD,_("Spoiled child !\n Do you want it or not ?"),_("No .. \nYes !"));
		if (ans) {
			status.sword=obj_sword_stick;
		}		
	} else {
		window_dialog (PNJ_OLD,_("It's dangerous to go alone.\nBut you have my stick now.\nLeave me alone."),0);
		window_dialog (PLAYER, _("But won't it be dangerous to be alone ?"),0);
		window_dialog (PNJ_OLD,_("uuh .."),0);
	}
	window_draw_hud();
	return col_block;
}


static const uint16_t entries[][2] = {{0,44}};
void start_ug_laby_enter(uint8_t entry)
{	
	player.x = entries[entry][0];
	player.y = entries[entry][1];


	for (int i=0;i<room.nb_objects;i++) {
		struct ExtraObject *eo = &room.objects[i];
		switch(eo->type) {
			case type_start_ug_laby_old : 
				eo->collide = dialog_old;
				break;
		}
	}

}
void start_ug_laby_frame()
{
	
}
void start_ug_laby_exit()
{
	
}

uint8_t start_ug_laby_background_collide(uint8_t bgtype)
{
	return bgtype;
}
