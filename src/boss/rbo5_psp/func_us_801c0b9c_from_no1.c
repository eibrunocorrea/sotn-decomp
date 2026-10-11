// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

extern EInit D_pspeu_09266738;
extern s32 E_ID(UNK_1B);
extern s32 D_pspeu_0926CCD0;
extern u8 D_pspeu_0926B490[];
extern u16 D_pspeu_0926B498[][2];

// local copy of func_us_801C0B9C (see no1/unk_3FA34.c), with
// the PSP boss room player-control handshake.
void func_us_801C0B9C_from_no1(Entity* self) {
    Entity* nextEntity;
    s32 i;
    s32 params;
    s32 fgIndex;
    s32 posX;
    s32 posY;
    s32 nearParams;
    s32 farParams;

    switch (self->step) {
    case 0:
        if (PLAYER.posX.i.hi < 0x80) {
            g_Player.padSim = PAD_RIGHT;
            nearParams = 0;
            farParams = 0x100;
        } else {
            g_Player.padSim = PAD_LEFT;
            nearParams = 0x100;
            farParams = 0;
        }
        g_Player.demo_timer = 0x18;
        InitializeEntity(D_pspeu_09266738);
        self->animCurFrame = 0;
        self->ext.et_801C0B9C.unk84 = 0;
        if (!self->params) {
            self->params += nearParams;
            nextEntity = self + 1;
            for (i = 1; i < 4; i++, nextEntity++) {
                CreateEntityFromCurrentEntity(E_ID(UNK_1B), nextEntity);
                nextEntity->params = i + nearParams;
            }

            for (i = 0; i < 4; i++, nextEntity++) {
                CreateEntityFromCurrentEntity(E_ID(UNK_1B), nextEntity);
                nextEntity->params = i + farParams;
            }
        }
        self->posY.i.hi = 0x68 - g_Tilemap.scrollY.i.hi;
        if (self->params & 0x100) {
            self->posX.i.hi = 0x1F0 - g_Tilemap.scrollX.i.hi;
        } else {
            self->posX.i.hi = 0x10 - g_Tilemap.scrollX.i.hi;
        }
        break;
    case 1:
        if (!self->params && g_Player.demo_timer < 9) {
            g_Player.padSim = PAD_NONE;
            g_Player.demo_timer = 4;
            self->ext.et_801C0B9C.unk84 = 1;
        }
        if (self->ext.et_801C0B9C.unk84) {
            self->step = 2;
        }
        break;
    case 2:
        if (!self->params) {
            g_Player.padSim = PAD_NONE;
            g_Player.demo_timer = 4;
        }
        if (!AnimateEntity(D_pspeu_0926B490, self)) {
            self->velocityY = FIX(4);
            self->step++;
        }
        break;
    case 3:
        if (!self->params) {
            g_Player.padSim = PAD_NONE;
            g_Player.demo_timer = 0x18;
        }
        MoveEntity();
        self->velocityY += FIX(0.125);

        params = self->params & 0xFF;
        posY = (((3 - params) * 0x10) + 0x60) + g_Tilemap.scrollY.i.hi;
        if (posY < self->posY.i.hi) {
            if (params != 4) {
                nextEntity = self + 1;
                nextEntity->ext.et_801C0B9C.unk84 = 1;
            }
            self->posY.i.hi = posY;
            fgIndex = 0xDE;
            if (!(self->params & 0x100)) {
                g_api.PlaySfx(SFX_EXPLODE_FAST_A);
                fgIndex = 0xC0;
            }
            fgIndex += ((3 - params) << 5);
            (&g_Tilemap.fg[fgIndex])[0] = D_pspeu_0926B498[7 - params][0];
            (&g_Tilemap.fg[fgIndex])[1] = D_pspeu_0926B498[7 - params][1];
            self->velocityY = 0;
            self->step++;
        }
        break;
    case 4:
        if (D_pspeu_0926CCD0 & 4) {
            self->step++;
        }
        break;
    case 5:
        fgIndex = 0xC0;
        params = self->params & 0xFF;
        if (self->params & 0x100) {
            fgIndex = 0xDE;
        }

        fgIndex += ((3 - params) << 5);
        (&g_Tilemap.fg[fgIndex])[0] = D_pspeu_0926B498[3 - params][0];
        (&g_Tilemap.fg[fgIndex])[1] = D_pspeu_0926B498[3 - params][1];
        self->step++;
    case 7:
        break;
    case 6:
        if (self->params & 0x100) {
            self->velocityX = FIX(0.5);
        } else {
            self->velocityX = FIX(-0.5);
        }
        MoveEntity();
        posX = g_Tilemap.scrollX.i.hi + self->posX.i.hi;
        if (posX < -0x20 || posX > 0x220) {
            self->step++;
        }
        break;
    }
}
