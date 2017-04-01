#define MAPS_IMPLEMENTATION
#define SPRITE_IMPLEMENTATION
#include "map_bateau.h"
#include "miniz.h"

void bateau_enter(uint8_t entry)
{
	
}

uint8_t bateau_background_collide(uint8_t bgtype)
{
	return bgtype;
}


uint8_t bateau_object_collide(const struct ExtraObject *eo)
{
	return col_block;
}



void bateau_frame()
{
}

void bateau_exit()
{
	
}