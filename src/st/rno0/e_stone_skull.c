// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno0.h"

// NOT a verbatim transplant of src/st/rare/e_stone_skull.c: the bobbing
// physics (velocityY clamped between FIX(-1.0) and FIX(1.0), stepping
// by FIX(0.125)) match exactly, but RNO0's version uses a different
// opacity (0xD0 vs ARE's 0xFF) and has NO trailing hitbox_config /
// hitbox_indices lookup at all -- confirmed by reading the raw target
// instructions end-to-end (the function just returns after the
// step 0/1 physics and the step 255 debug block).
extern EInit g_EInitStoneSkull;
extern u8 D_us_80181E8C[]; // pose animation table

void EntityStoneSkull(Entity* self) {
    s32 posY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitStoneSkull);
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0xD0;
        self->ext.stoneSkull.startingPosY =
            g_Tilemap.scrollY.i.hi + self->posY.i.hi;
        self->velocityY = FIX(1.0);
        /* fallthrough */
    case 1:
        AnimateEntity(D_us_80181E8C, self);
        MoveEntity();
        posY = (self->posY.i.hi + g_Tilemap.scrollY.i.hi);
        posY = self->ext.stoneSkull.startingPosY - posY;
        if (self->velocityY > 0) {
            posY += self->params;
        } else {
            posY -= self->params;
        }

        if (posY < 0) {
            self->velocityY -= FIX(0.125);
            if (self->velocityY < FIX(-1.0)) {
                self->velocityY = FIX(-1.0);
            }
        } else {
            self->velocityY += FIX(0.125);
            if (self->velocityY > FIX(1.0)) {
                self->velocityY = FIX(1.0);
            }
        }
        break;
    case 255:
#include "../pad2_anim_debug.h"
    }
}
