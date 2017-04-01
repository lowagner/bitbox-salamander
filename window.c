// window : 2 tilesets left & right, 2ptrs + 1 limit left/right, 1 unique vram 
//#include <bitbox.h>

#include "string.h"

#include "lib/blitter/blitter.h"

#define MAPS_IMPLEMENTATION
#include "map_window.h" 

#include "miniz.h"

static uint8_t vram[16][40];
static object *window;

#define HUD_HEIGHT 4

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

	window_draw_hud();
}

int wait_joy_pressed() // wait for new joystick button pressed, send it.
{
	// wait for release first 
	while (gamepad_buttons[0]);

	message("%d %x\n",vga_frame, gamepad_buttons[0]);

	uint16_t prev_buttons=gamepad_buttons[0];
	while (1) {
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

void window_draw_hud() // (re)draw hud
{
	memset(vram, 0,40*16); 
	tmap_blitlayer(window, 0,12,window->b,data_window_tmap,layer_window_hud);
	window_set(4*8);
}

// XXX faces as left window blocks : load tileset, display it, unload at end. 

// face_id : 0 if none
// msg : \n separated multiline message.
// answers: \n separated different answers, NULL : this is a message. 
int window_dialog (int face_id, char *msg, char *answers)
{

	message("Dialog: \n");
	message("message=%s\n",msg);
	message("answers=%s\n",answers);

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
		for (char *p=msg; *p; p++) {
			if (*p=='\n' || pos==39) {
				nlines++; pos=3;
			} else {
				vram[8+nlines][pos++] = *p+1;
			}
			wait_vsync(4); // faster if button pressed
		}
		nlines += 2;
	}



	if (answers) { 
		// display answers
		int nchoices=0;
		pos=4;
		for (char *p=answers; *p; p++) {
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
