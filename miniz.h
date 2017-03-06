#define ALL_ROOMS \
	X(beach) \
	X(start) \
	X(town) \


// room defs
enum room_id_list {
#define X(rm) room_##rm,
	ALL_ROOMS
#undef X
};



// declare callbacks
#define X(room) \
  void room##_enter();\
  void room##_frame();\
  void room##_exit();

	ALL_ROOMS
#undef X

// faces
#define ME 0
#define PNJ1 1


// Window

void window_init();
void window_set(int y_target);
void window_draw_hud();
int  window_dialog (int face_id, char *msg, char *answers);

// Blocking wait for new joystick button pressed, send it.
int wait_joy_pressed() ;

