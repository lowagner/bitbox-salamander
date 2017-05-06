#include "miniz.h"

// -- updates

// wait till the end of animation and destroy sprite
void end_anim_destroy(struct ExtraObject *this);


uint8_t collide_hurt    (struct ExtraObject *this);
uint8_t collide_canpull (struct ExtraObject *this);
uint8_t collide_chest   (struct ExtraObject *this);
uint8_t collide_exit    (struct ExtraObject *this);

void rat_update(struct ExtraObject *this);
void rat_hit(struct ExtraObject *this);

