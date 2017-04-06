#include "miniz.h"

#define MAPS_IMPLEMENTATION
#define SPRITE_IMPLEMENTATION
#include "map_town.h"



static const uint16_t entries[][2] = {{176,624}};
void town_enter(uint8_t entry)
{
	
	player.x = entries[entry][0];
	player.y = entries[entry][1];

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

