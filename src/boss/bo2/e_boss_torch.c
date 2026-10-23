// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

extern EInit D_us_80180524;
extern AnimateEntityFrame anim_boss_torch[];
// Torches are scaled slightly smaller based on params
// to appear further in the background.
extern s16 D_us_80180B6C[];

void EntityBossTorch(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180524);
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = D_us_80180B6C[self->params];
        // fallthrough
    case 1:
        AnimateEntity(anim_boss_torch, self);
        if (g_Timer & 4) {
            self->palette = (self->params * 2) + PAL_FLAG(0x5A);
        } else {
            self->palette = (self->params * 2) + PAL_FLAG(0x5B);
        }
        break;
    case 255:
#include "../../st/pad2_anim_debug.h"
    }
}
