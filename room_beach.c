#define MAPS_IMPLEMENTATION
#include "map_beach.h"

#include "miniz.h"

void beach_enter( uint8_t entry ) 
{
}

void beach_frame( void )
{
}

void beach_exit() 
{
}

uint8_t beach_background_collide(uint8_t bgtype)
{
	return bgtype;
}


uint8_t beach_object_collide(const struct ExtraObject *eo)
{
	return col_block;
}

