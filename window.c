// window : 2 tilesets left & right, 2ptrs + 1 limit left/right, 1 unique vram 
//#include <bitbox.h>

#include "lib/blitter/blitter.h"

#define MAPS_IMPLEMENTATION
#include "map_window.h" // special "room" for window + always-on data

#include "data.h"

#include "string.h"

static uint8_t vram[32][16];
static object *window;
static int window_y;

#define HUD_HEIGHT 4

void window_init() 
{
	window = tilemap_new(
		load_resource(map_window.tileset), 
		0,0,
		TMAP_HEADER(map_window.tilemap_w,map_window.tilemap_h,map_window.tilesize,TMAP_U8),
		vram
	);
}

int wait_joy_pressed() // wait for new joystick button pressed, send it.
{
	uint16_t prev_buttons=gamepad_buttons[0];

	while (1) {
		uint16_t but = gamepad_buttons[0];
		int pressed = (but & ~prev_buttons); // 1+lsb pos or zero
		if (pressed) 
			return __builtin_ffs(pressed-1);
		prev_buttons = but;

		vsync_wait(1);
	}
}

// linear movement to target
void window_set(int y_target) 
{
	while (window_y != y_target) {
		window_y += window_y>y_target ? -1 : 1;
		vsync_wait(1);
	}
}

void window_draw_hud() // (re)draw hud
{
}

// XXX faces as left window blocks : load tileset, display it, unload at end. 

// face_id : 0 if none
// msg : \n separated multiline message.
// answers: \n separated different answers, NULL : this is a message. 
int window_dialog (int face_id, char *msg, char *answers)
{
	int nlines=0;
	int pos=0; // horizontal position
	int choice=0;

	window_set(0); // hide window

	// display message
	memset(vram, sizeof(vram), ' '); 
	for (char *p=msg; *p; p++) {
		if (*p=='\n') {
			nlines++; pos=0;
		} else {
			vram[nlines][pos++] = *p;
		}
	}

	if (answers) { 
		// display answers
		int nchoices=0;
		pos=2;
		for (char *p=answers; *p; p++) {
			if (*p=='\n') {
				nchoices++;
				pos=2;
			} else {
				vram[nlines+nchoices][pos++] = *p;
			}
		}

		window_set((nlines+nchoices)*8);

		// select, display cursor
		while(1) {
			int btn = wait_joy_pressed();
			vram[nlines+choice][0] = ' ';
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
			vram[nlines+choice][0] = '*';
		}
	} else { // !answers
		window_set(nlines*8);
		while (wait_joy_pressed() != gamepad_A);
		goto end;
	}

end: 
	window_set(0);
	window_set(HUD_HEIGHT);
	window_draw_hud();

	return choice;
}
