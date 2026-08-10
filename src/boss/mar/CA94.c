// SPDX-License-Identifier: AGPL-3.0-or-later
#include "mar.h"

extern EInit g_EInitSpawner;
extern u32 g_CutsceneFlags;
extern s32 g_SkipCutscene;

static u8 D_us_80180688[] = {
    0x01, 0x01, 0xFF, 0x00, 0x01, 0x02, 0xFF, 0x00,
};
static u8 D_us_80180690[] = {
    0x01, 0x02, 0x07, 0x03, 0x07, 0x04, 0x07, 0x05, 0x07, 0x06,
    0x07, 0x07, 0x07, 0x08, 0x07, 0x09, 0x10, 0x0A, 0xFF, 0x00,
};
static u8 D_us_801806A4[] = {
    0x01, 0x0A, 0x06, 0x0B, 0x06, 0x0C, 0x06, 0x0D, 0x10, 0x0E,
    0x0A, 0x0F, 0x0A, 0x0E, 0x0A, 0x0F, 0x20, 0x0E, 0xFF, 0x00,
};
static u8 D_us_801806B8[] = {
    0x01, 0x0E, 0x04, 0x10, 0x04, 0x11, 0x0B, 0x12, 0xFF, 0x00,
    0x00, 0x00, 0x01, 0x13, 0xFF, 0x00, 0x01, 0x12, 0xFF, 0x00,
};
static u8 D_us_801806CC[] = {
    0x20, 0x12, 0x03, 0x11, 0x03, 0x10, 0x03, 0x14, 0x03, 0x15, 0x40, 0x16,
    0x06, 0x15, 0x05, 0x0D, 0x05, 0x0C, 0x05, 0x0B, 0x20, 0x0A, 0xFF, 0x00,
};
static u8 D_us_801806E4[] = {
    0x10, 0x0A, 0x10, 0x17, 0x08, 0x0A, 0xFF, 0x00,
};
static u8 D_us_801806EC[] = {
    0x01, 0x0A, 0x02, 0x0B, 0x02, 0x0C, 0x06, 0x18, 0x06, 0x19,
    0x50, 0x18, 0x03, 0x0C, 0x03, 0x0B, 0x08, 0x0A, 0xFF, 0x00,
};
static u8 D_us_80180700[] = {
    0x01, 0x0A, 0x03, 0x0B, 0x03, 0x0C, 0x03, 0x0D, 0x03, 0x0E, 0x06, 0x1B,
    0x10, 0x1A, 0x20, 0x1B, 0x03, 0x0E, 0x03, 0x0D, 0x03, 0x0C, 0xFF, 0x00,
};
static u8 D_us_80180718[] = {
    0x04, 0x1C, 0x04, 0x1D, 0x04, 0x1E, 0x04, 0x1F, 0xFF, 0x00, 0x00, 0x00,
};
static u8 D_us_80180724[] = {
    0x02, 0x20, 0x04, 0x21, 0x04, 0x22, 0x04, 0x23, 0x04, 0x24,
    0x04, 0x25, 0x04, 0x26, 0x04, 0x27, 0x02, 0x20, 0x00, 0x00,
};
static u8 D_us_80180738[] = {
    0x08, 0x28, 0x08, 0x29, 0x60, 0x2A, 0xFF, 0x00, 0x01, 0x0C, 0xFF, 0x00,
};

void func_us_8018CA94(Entity* self) {
    Entity* player;
    Tilemap* tilemap;
    s32 pan;

    // n.b.! unused, required for PSP
    tilemap = &g_Tilemap;
    player = &PLAYER;

    if (g_SkipCutscene && self->step < 12) {
        SetStep(12);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitSpawner);
        self->animSet = ANIMSET_OVL(2);
        self->animCurFrame = 1;
        self->unk5A = 0x48;
        self->palette = 0x210;
        break;

    case 1:
        if (g_CutsceneFlags & 0x4) {
            self->animCurFrame = 2;
            self->step++;
        }
        break;

    case 2:
        if (g_CutsceneFlags & 0x8) {
            SetStep(3);
        }
        break;

    case 3:
        AnimateEntity(D_us_80180690, self);
        if (g_CutsceneFlags & 0x10) {
            SetStep(4);
        }
        break;

    case 4:
        AnimateEntity(D_us_801806A4, self);
        if (g_CutsceneFlags & 0x20) {
            SetStep(5);
        }
        break;

    case 5:
        AnimateEntity(D_us_801806B8, self);
        if (g_CutsceneFlags & 0x40) {
            SetStep(6);
        }
        break;

    case 6:
        self->animCurFrame = 19;
        if (g_CutsceneFlags & 0x80) {
            self->step++;
        }
        break;

    case 7:
        self->animCurFrame = 0x12;
        if (g_CutsceneFlags & 0x100) {
            self->step++;
        }
        break;

    case 8:
        AnimateEntity(D_us_801806CC, self);
        if (g_CutsceneFlags & 0x200) {
            SetStep(9);
        }
        break;

    case 9:
        AnimateEntity(D_us_801806E4, self);
        if (g_CutsceneFlags & 0x400) {
            SetStep(10);
        }
        break;

    case 10:
        AnimateEntity(D_us_801806EC, self);
        if (g_CutsceneFlags & 0x800) {
            SetStep(11);
        }
        break;

    case 11:
        AnimateEntity(D_us_80180700, self);
        if (g_CutsceneFlags & 0x1000) {
            SetStep(12);
        }
        break;

    case 12:
        if (!AnimateEntity(D_us_80180718, self)) {
            SetStep(13);
            self->velocityX = FIX(1.5);
        }
        break;

    case 13:
        pan = AnimateEntity(D_us_80180724, self);
        if (pan & 0x80 && (self->pose == 3 || self->pose == 7)) {

            pan = (self->posX.i.hi - 120) / 16;

            if (pan < -8) {
                pan = -8;
            }
            if (pan > 8) {
                pan = 8;
            }

            g_api.PlaySfxVolPan(SFX_STOMP_SOFT_B, 80, pan);
        }

        MoveEntity();

        if (self->posX.i.hi > 184) {
            SetStep(14);
            self->velocityY = FIX(-4.0);
        }

        break;

    case 14:
        AnimateEntity(D_us_80180738, self);
        self->velocityY += FIX(0.1875);
        MoveEntity();

        if (self->velocityY > 0 && self->posY.i.hi > 0x83) {
            self->velocityY = 0;
            SetStep(15);
        }

        break;

    case 15:
        pan = AnimateEntity(D_us_80180724, self);
        if (pan & 0x80 && (self->pose == 3 || self->pose == 7)) {

            pan = (self->posX.i.hi - 120) / 16;

            if (pan < -8) {
                pan = -8;
            }
            if (pan > 8) {
                pan = 8;
            }

            g_api.PlaySfxVolPan(SFX_STOMP_SOFT_B, 80, pan);
        }

        MoveEntity();

        if (self->posX.i.hi > 256) {
            g_CutsceneFlags |= 0x2000;
        }

        if (self->posX.i.hi > 272) {
            DestroyEntity(self);
        }

        break;
    }
}
