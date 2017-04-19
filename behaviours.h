#include "miniz.h"

void rat_update(struct ExtraObject *this);

uint8_t collide_hurt(struct ExtraObject *this);
uint8_t collide_canpull(struct ExtraObject *this);
uint8_t collide_chest(struct ExtraObject *this);
uint8_t collide_exit(struct ExtraObject *this);
