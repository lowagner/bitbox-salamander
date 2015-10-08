enum { B_NONE, B_PANEL1, B_PANEL2, B_PANEL3 } Behaviours; // how the tiles react

// east : SCROLL_ENABLED NSEW (ou pas)
// objects divers + ce qu'on en fait (yc invisbibles / zones si defini sur la map) 
// par defaut, jump to NSEW piece (oui + simple) si touche le bord.


int room_id;

/* player related */
struct Player {
	object *sprite;
	int righthand=0; // objectid in left hand (A)
	int lefthand=0; // objectid in right hand (B)
	int armor=0; // which armor we have now

	int invincible_frames_left=0; // time left 

	uint8_t inventory[2][16]; // list of object_id, number of objects/ammo.
}


// local room data
// uint8_t *sprites[16]; // fast access ? as sprites only + special attributes on _tiles_
uint16_t *tilemap;
void (*on_walk(int));
void (*on_hit(int));
void (*on_action(int));

#define MAXROOM_DATA 60000
struct room {
	// variable len data
	char data[MAXROOM_DATA]; 

	/* 
	made of : 

	u8 width, height; // tilemap size in 16x16 tiles 
	u8 nbtiles
	u8 nbobjects; // sprite data len
	u8 nbsprites;

	
	u8 tilemap[width*height]
	u8 tileset[256][nbtiles]

	u8 object data[4][nbobjects]
		[x,y,type,sprite_id, (4 extra object data : life ?) ] // args for object_init callback. -> ou array tel quel 
	
	buffersize # max unzipped animation frame for this sprite
	u8 display_buffers[nbsprites][buffersize];

	u32 sprites_ptr[nbsprites] // variable data a reecrire au load
	[nbsprites fois] 
		u8 buffer[buffersize] (specific to max unzipped frame)
		nb_animations
		u16 anim_ofs[nbanimation]; // offset in anim_file
		u8  framesize[nbanimation][8]; // decompressed framesizes of animation

	Song song (avec free SFX ? non a part)	XXX sounds/music/sfx : a song with extra SFX !

	*/  
}; // loaded as is from zipped flash. rewrite pointers/sprites on load .

// flash
struct Sprite {
	uint8_t width, height;
	uint8_t nb_anims; // for each animation, number of frames

	int *anim_length; // pointer to array of filesizes for animations
	void **animations; // ptr to array of ptrs to zipped rom frame data
};



// frame callbacks
void frame_appear() {
	// set player at start of screen
	player->x = ;
	player->y = ;


	// blinks player
	if ((vga_frame/16)%2) 
		player->frame=0
	else 
		player->frame=PLAYER_DOWN0;

	if (vga_frame>3*60) {
		// change state to frame normal
		vga_frame=0;
		frame = frame_normal;
		player_invincible=0;
	}
}

void change_room(int next_room)
{
	/* Leave preceding room */


	/* enter new room */
	room_id = next_room;
	// load vram, map
	// instantiate initial objects


}

collide_tile(int pos)
{
	// according to globals room_id and player_pos
	switch(map[pos]) {
		case B_NONE : player->speed=4;
		case B_SLOW : player->speed=2;
		case B_PANEL1: ; break;
		case B_DIE: ; break; // appear in 
	}
}

action_tile(int pos)
{
	// also uses current used object
	switch(map[pos]) {
		case B_
	}
}



void line_background(void) 
{
	// globals bg_x, bg_y, vram, palette ...
	

}