// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno0.h"

// No matched donor found anywhere in the tree for either function
// (RNO0-exclusive background prop); decompiled directly from the
// target instructions.
extern EInit g_EInitCommon;

void EntityPendulum(Entity* self) {
    s16 rotate;

    if (!self->step) {
        InitializeEntity(g_EInitCommon);
        self->animSet = -0x7FFE;
        self->animCurFrame = 0x21;
        self->zPriority = 0x50;
        self->drawFlags = 0xC;
        self->unk5A = 0;
        self->palette = 0;
        self->opacity = 0x60;
    }

    rotate = rsin(((g_Timer % 120) * ROT(360) + 60) / 120);
    if (!rotate) {
        g_api.PlaySfx(0x6B7);
    }
    self->rotate = (rotate >> 6) + (rotate >> 7);
}

extern EInit OVL_EXPORT(EInitSpawner);

void EntityClockTickSound(Entity* self) {
    s32 diff;
    s32 diff2;
    s16 volume;

    if (!self->step) {
        InitializeEntity(OVL_EXPORT(EInitSpawner));
    }

    if ((g_Timer % 60) != 0) {
        return;
    }

    switch (self->params) {
    case 0:
        g_api.PlaySfx(0x6B7);
        break;

    case 1:
        diff = 0x140 - (g_Tilemap.scrollX.i.hi + PLAYER.posX.i.hi);
        volume = (diff * 2) / 5;
        if (volume < 0) {
            volume = 0;
        } else if (volume > 0x7F) {
            volume = 0x7F;
        }
        g_api.PlaySfxVolPan(0x6B7, volume & 0xFF, -8);
        break;

    case 2:
        diff2 = PLAYER.posX.i.hi + 0x40;
        volume = (diff2 * 2) / 5;
        if (volume < 0) {
            volume = 0;
        } else if (volume > 0x7F) {
            volume = 0x7F;
        }
        g_api.PlaySfxVolPan(0x6B7, volume & 0xFF, 8);
        break;
    }
}

void RNO0_Unused801B70FC(void) {}
