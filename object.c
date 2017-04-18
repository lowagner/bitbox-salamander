#include "miniz.h"

void object_set_state(struct ExtraObject *o, int state)
{
	if (o->state!=state) {
		o->state = state;
		o->frame=0;
	}
}


inline void object_transfer(const struct ExtraObject *eo)
{
	// transfer to sprite
	eo->spr->fr = eo->def->states[eo->state].frames[eo->frame];
	eo->spr->x = eo->x+room.background->x;
	eo->spr->y = eo->y+room.background->y;
}

void object_anim_frame(struct ExtraObject *eo)
{
	const struct StateDef *std = &eo->def->states[eo->state];

	if (vga_frame%8==0) { // animate : 1 frame = 133ms
		// loop at end of animation.
		eo->frame += 1;
		if (eo->frame >= std->nb_frames)
			eo->frame=0;
	}	
}


// reject if bumps into blocking, depending on which part touches, or make it slide sideways
// a = topleft ... d=bottom right
void object_block (struct ExtraObject *eo, Quad collision)
{	
	uint8_t a = collision.b[0];
	uint8_t b = collision.b[1];
	uint8_t c = collision.b[2];
	uint8_t d = collision.b[3];

	if (eo->vy<0 && (a||b)) {
		// shift left or right if only part blocks
		if (!b) {
			eo->vx+=1;
		} else if (!a) {
			eo->vx-=1;
		}
		eo->vy=0;
	} else if (eo->vy>0 && (c||d)) {
		// shift left or right if only part blocks
		if (!d) {
			eo->vx+=1;
		} else if (!c) {
			eo->vx-=1;
		}
		eo->vy=0;		
	} 

	if (eo->vx<0 && (a||c)) {
		// shift up/dn if only part blocks
		if (!c) {
			eo->vy+=1;
		} else if (!a) {
			eo->vy-=1;
		}
		eo->vx=0;		
	} else if (eo->vx>0 && (b||d)) {
		if (!d) {
			eo->vy+=1;
		} else if (!b) {
			eo->vy-=1;
		}
		eo->vx=0;		
	} 
}



static inline uint8_t bg_property_at(int x, int y)
{
	// XXX scroll offset
	uint8_t tileid = room.tmap[y/16*room_defs[room.id].def->tilemap_w+x/16]; 
	// 4 corners in a tile
	uint16_t ter_tile = room_defs[room.id].def->terrains[tileid-1]; // 1-based
	if (x%16<8) { 	// left
		return y%16<8 ? (ter_tile>>12)&0xf : (ter_tile>>4)&0xf;
	} else { 		// right
		return y%16<8 ? (ter_tile>> 8)&0xf : (ter_tile>>0)&0xf;
	}
}

// update object collision state for each corner of the sprite
Quad object_bg_collide(struct ExtraObject *eo)
{
	const struct StateDef* def = &eo->def->states[eo->state];
	// position that the sprite WOULD have considering its speed
	const int bx1=eo->x+eo->vx+def->x1;
	const int by1=eo->y+eo->vy+def->y1;
	const int bx2=eo->x+eo->vx+def->x2;
	const int by2=eo->y+eo->vy+def->y2;

	uint8_t (*f)(uint8_t) = room_defs[room.id].bg_col;

	uint8_t a = f(bg_property_at (bx1,by1));
	uint8_t b = f(bg_property_at (bx2,by1));
	uint8_t c = f(bg_property_at (bx1,by2));
	uint8_t d = f(bg_property_at (bx2,by2));

	return (Quad){.b={a,b,c,d}};
}

