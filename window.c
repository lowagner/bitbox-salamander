// window : 2 tilesets left & right, 2ptrs + 1 limit left/right, 1 unique vram 
//#include <bitbox.h>

#include "string.h"

#include "lib/blitter/blitter.h"

#define MAPS_IMPLEMENTATION
#include "map_window.h" 

#include "miniz.h"

static uint8_t vram[16][40];
static object *window;


void window_init() 
{
	window = tilemap_new(
		load_resource(map_window.tileset), 
		0,0,
		TMAP_HEADER(40,16,map_window.tilesize==8?TSET_8:TSET_16,TMAP_U8) | TSET_8bit,
		vram
	);
	window->x = (VGA_H_PIXELS-window->w)/2;
	window->z = -10; // set front 
	window->y=-window->h; // hide up

}

int wait_joy_pressed() // wait for new joystick button pressed, send it.
{
	// wait for release first 
	while (gamepad_buttons[0]);

	uint16_t prev_buttons=gamepad_buttons[0];
	while (1) {
		if ( gamepad_y[0] < -50 ) gamepad_buttons[0] |= gamepad_up;
		if ( gamepad_y[0] >  50 ) gamepad_buttons[0] |= gamepad_down;
		if ( gamepad_x[0] < -50 ) gamepad_buttons[0] |= gamepad_left;
		if ( gamepad_x[0] >  50 ) gamepad_buttons[0] |= gamepad_right;

		uint16_t but = gamepad_buttons[0];
		uint16_t pressed = (but & ~prev_buttons); // 1+lsb pos or zero

		if (pressed) 
			return pressed;

		prev_buttons = but;
		wait_vsync(1);
	}
}

// linear movement to target
void window_set(int y_target) 
{
	while (window->h+window->y != y_target) {
		if (window->h+window->y < y_target)
			window->y += 2;
		else 
			window->y -= 2;
		wait_vsync(1);
	}
}

static void draw_window(int x1, int y1, int x2, int y2)
{
	for (int i=x1;i<x2;i++) {
		vram[y1][i]=162;
		vram[y2][i]=178;		
	}
	for (int i=y1;i<y2;i++) {
		vram[i][x1] =164;
		vram[i][x2] =165;
	}
	for (int i=y1+1;i<y2;i++)
		for (int j=x1+1; j<x2;j++)
			vram[i][j] = ' '+1;

	// singles at corners
	vram[y1][x1] = 161;
	vram[y2][x1] = 177; 
	vram[y1][x2] = 163;
	vram[y2][x2] = 179;
}

static void draw_dialog(int x1,int y1,int x2,int y2)
{
	draw_window(x1+2,y1,x2,y2);

	// black on left
	for (int i=y1;i<y2;i++)
		for (int j=0;j<x1;j++)
			vram[i][j]=208 ; 
	
	vram[y1][x1] = 182; // speak
	vram[y1][x1+1] = 33; 
}

#define HUD_W 25
#define HUD_H 3


#define HUD_X ((40-HUD_W)/2)
#define HUD_Y (15-HUD_H)

void window_draw_hud() // (re)draw hud
{
	memset(vram, 0,40*16);
	// at end
	draw_window(HUD_X,HUD_Y,HUD_X+HUD_W,HUD_Y+HUD_H);

	// lives 
	// XXX case > 10 ! 
	int i;
	for (i=0;i<status.life/2;i++)
		vram[HUD_Y+1][HUD_X+14+i] = 146;
	if (status.life%2) {
		vram[HUD_Y+1][HUD_X+14+i] = 153;
		i++;
	}
	for (;i<status.life_max/2;i++)
		vram[HUD_Y+1][HUD_X+14+i] = 156;

	// magic
	static const uint8_t mana_chars[7][2] = {
		{206,206},	{206,207},	{206,222},	{206,223},
		{207,223},	{222,223},	{222,223},
	};

	vram[HUD_Y+0][HUD_X+21] = 236;
	vram[HUD_Y+1][HUD_X+21] = mana_chars[status.mana][0];
	vram[HUD_Y+2][HUD_X+21] = mana_chars[status.mana][1];
	vram[HUD_Y+3][HUD_X+21] = 205;

	// hud inventory
	vram[HUD_Y+1][HUD_X+1] = 37; 
	vram[HUD_Y+2][HUD_X+1] = status.gold >= 10 ? status.gold/10 + 49 : 33; 
	vram[HUD_Y+2][HUD_X+2] = status.gold%10 + 49; 

	vram[HUD_Y+1][HUD_X+4] = 220; 
	vram[HUD_Y+2][HUD_X+4] = status.bombs + 49; 

	vram[HUD_Y+1][HUD_X+6] = 204; 
	vram[HUD_Y+2][HUD_X+6] = status.arrows >= 10 ? status.arrows/10 + 49 : 33; 
	vram[HUD_Y+2][HUD_X+7] = status.arrows%10 + 49; 


	window_set(HUD_H*8+2);
}

static void draw_item(int x,int y,int line, int item_id)
{
	for (int i=0;i<2;i++)
		for (int j=0;j<2;j++)
			vram[y+i][x+j]=data_window_tmap[
				(layer_window_items*16+line*2+i)*40+\
				item_id*2+j
				];
}

void window_inventory()
{
	window_draw_hud();
	draw_window(HUD_X      ,0,HUD_X+2*6  ,HUD_Y-2);
	draw_window(HUD_X+2*8+1,0,HUD_X+HUD_W,HUD_Y-2);

	draw_item(26,HUD_Y-4,1,status.shield);
	draw_item(29,HUD_Y-4,2,status.sword);

	for (int i=0;i<NB_OBJECTS;i++) {
		if (status.objects & (1<<i))
			draw_item(
				HUD_X+1+(i%4)*3,
				1+(i/4)*3,
				0,i);
	}


	window_set(window->h); // show all

	// wait for re-press ENTER to leave
	while (wait_joy_pressed() != gamepad_start);

	window_set(HUD_H*8+2);
	
}


// face_id : 0 if none
// msg : \n separated multiline message.
// answers: \n separated different answers, NULL : this is a message. 
int window_dialog (const int face_id,const char *msg,const char *answers)
{

	int pos=0; // horizontal position
	int choice=0; // current choice

	// draw window
	memset(vram, ' '+1,40*16); // one-based 
	for (int i=2;i<40;i++) {
		vram[7] [i]=162;
		vram[15][i]=178;		
	}
	for (int i=7;i<15;i++) {
		vram[i][2] =164;
		vram[i][39]=165;	
		vram[i+1][0]=vram[i+1][1]=208 ;// black;	
	}
	// singles
	vram[ 7][ 2] = 161; // corners 
	vram[15][ 2] = 177; 
	vram[ 7][39] = 163;
	vram[15][39] = 179;
	vram[12][ 1] = 182; // speak
	vram[12][ 2] = 33; 

	// display face
	for (int i=0;i<2;i++)
		for (int j=0;j<2;j++)
			vram[7+i][j] = data_window_tmap[(layer_window_faces*16+(face_id>>4)*2+i)*40+j+(face_id&0xf)*2];

	window_set(9*8);

	int nlines=0;
	// display message
	if (msg) {
		pos=3;
		for (const char *p=msg; *p; p++) {
			if (*p=='\n' || pos==39) {
				nlines++; pos=3;
			} else {
				vram[8+nlines][pos++] = *p+1;
			}
			wait_vsync(MESSAGE_SPEED); // faster if button pressed
		}
		nlines += 2;
	}



	if (answers) { 
		// display answers
		int nchoices=0;
		pos=4;
		for (const char *p=answers; *p; p++) {
			if (*p=='\n') {
				nchoices++;
				pos=4;
			} else {
				vram[8+nlines+nchoices][pos++] = *p+1;
			}
		}


		// select, display cursor
		vram[8+nlines+choice][3] = 32; // right arrow (XXX blink?)
		while(1) {
			uint16_t btn = wait_joy_pressed();
			vram[8+nlines+choice][3] = ' '+1;
			switch (btn) {
				case gamepad_down : 
					if (choice >= nchoices) 
						choice = 0;
					else 
						choice +=1;
					break;
				case gamepad_up : 
					if (choice == 0) 
						choice = nchoices -1;
					else 
						choice -=1;
					break;
				case gamepad_A : 
					goto end;
					break;
			}
			vram[8+nlines+choice][3] = 32; // right arrow (XXX blink?)
		}
	} else { // !answers
		while (wait_joy_pressed() != gamepad_A);
		goto end;
	}

end: 
	return choice;
}
