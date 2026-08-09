// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

// Boss Richter reuses his subweapon-crash entities; splat's pre-split
// func_pspeu_* names are kept until a rename pass
extern s16 D_pspeu_0927AF38[4][6]; // glass shatter offsets (stage data blob)

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925D310);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925D980);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925E330);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925E578);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925EA20);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925EAD8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925F210);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925F728);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925F8A8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09260020);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09260AE8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_092612E8);

// local copy of GetAguneaLightningAngle (see ric/319C4.c)
s16 func_pspeu_09261328(s16* arg0, s16 arg1, s16 arg2, s16* arg3) {
    arg1 += rand() % 256 - 0x80;
    *arg3 = (rand() % 48) + 0x10;
    arg0[0] = arg0[1];
    arg0[2] = arg0[3];
    if (arg2) {
        arg0[1] += (rcos(arg1) * *arg3) >> 0xC;
        arg0[3] += (rsin(arg1) * *arg3) >> 0xC;
        if (arg2 % 2) {
            return func_pspeu_09261328(arg0, arg1 - 0x140, arg2 / 2, arg3);
        } else {
            rand();
            rand();
            return func_pspeu_09261328(
                arg0, arg1 + 0x140, (arg2 - 1) / 2, arg3);
        }
    }
    return arg1;
}

// local copy of AguneaShuffleParams (see ric/319C4.c)
void func_pspeu_09261530(s32 bufSize, s32* buf) {
    s32 i, idx, swapTemp;

    for (i = bufSize - 1; i > 0; i--) {
        // Pick random index
        idx = rand() % bufSize;

        // Hold swap value temporarily
        swapTemp = buf[i];
        buf[i] = buf[idx];
        buf[idx] = swapTemp;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_092615E8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09261ED8);

// local copy of RicEntitySubwpnStopwatchCircle (see ric/319C4.c)
void func_pspeu_09262838(Entity* self) {
    s16 temp_s0_4;
    s16 psp_s4;
    s32 sine;
    s32 cosine;
    s16 xCoord;
    s16 yCoord;
    Primitive* prim;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x10);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_FILL_WHITE;
            prim->priority = self->zPriority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            psp_s4 = i * 0x100;
            prim->u0 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v0 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            psp_s4 = (i + 1) * 0x100;
            prim->u1 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v1 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            prim->u2 = prim->u3 = 0x20;
            prim->v2 = prim->v3 = 0xE0;
            prim->r0 = prim->r1 = prim->g0 = prim->g1 = prim->b0 = prim->b1 =
                0x40;
            prim->r2 = prim->r3 = prim->g2 = prim->g3 = 0;
            prim->b2 = prim->b3 = 0x20;
            prim = prim->next;
        }
        self->ext.et_stopwatchCircle.size = 0x20;
        self->step++;
        break;
    case 1:
        self->ext.et_stopwatchCircle.size += 0x18;
        self->ext.et_stopwatchCircle.timer++;
        if (self->ext.et_stopwatchCircle.timer > 0x1E) {
            DestroyEntity(self);
            return;
        }
    }
    xCoord = self->posX.i.hi;
    yCoord = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        sine = rsin(i * 0x100);
        cosine = rcos(i * 0x100);
        temp_s0_4 = self->ext.et_stopwatchCircle.size - 0x20;
        prim->x0 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y0 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x2 = xCoord + ((sine * (temp_s0_4)) >> 0xC);
        prim->y2 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        sine = rsin((i + 1) * 0x100);
        cosine = rcos((i + 1) * 0x100);
        prim->x1 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y1 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x3 = xCoord + ((sine * temp_s0_4) >> 0xC);
        prim->y3 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09262CE8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09264730);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_092649F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09265100);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09266AB0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09266CE8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", Unused09266E30);

// local copy of RicEntitySubwpnHolyWaterBreakGlass (see
// ric/pl_subweapon_holywater.c); prim priority tracks the boss entity
#define FAKEPRIM ((FakePrim*)prim)
void func_pspeu_09266E40(Entity* self) {
    Point16 sp10[8];
    Primitive* prim;
    s16 posX;
    s16 posY;
    s16 arrIndex;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                sp10[i].x = FAKEPRIM->posX.i.hi = FAKEPRIM->x0 = posX;
                sp10[i].y = FAKEPRIM->posY.i.hi = FAKEPRIM->y0 = posY;
                // Random velocity from 0.25 to 0.5
                FAKEPRIM->velocityX.val = (rand() & 0x3FFF) + FIX(0.25);
                if (i & 1) {
                    FAKEPRIM->velocityX.val = -FAKEPRIM->velocityX.val;
                }
                FAKEPRIM->velocityY.val =
                    -(((rand() & PSP_RANDMASK) * 2) + FIX(2.5));
                FAKEPRIM->drawMode = DRAW_HIDE | DRAW_UNK02;
                FAKEPRIM->type = PRIM_TILE;
            } else {
                prim->r0 = prim->r1 = prim->r2 = prim->r3 =
                    (rand() & 0xF) + 0x30;
                prim->b0 = prim->b1 = prim->b2 = prim->b3 =
                    (rand() & 0x7F) + 0x80;
                prim->g0 = prim->g1 = prim->g2 = prim->g3 =
                    (rand() & 0x1F) + 0x30;
                if (rand() & 1) {
                    prim->drawMode = (DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                      DRAW_UNK02 | DRAW_TRANSP);
                } else {
                    prim->drawMode = (DRAW_COLORS | DRAW_UNK02);
                }
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                prim->u0 = arrIndex = i & 3;
                prim->x0 = posX + D_pspeu_0927AF38[arrIndex][0];
                prim->y0 = posY + D_pspeu_0927AF38[arrIndex][1];
                prim->x1 = posX + D_pspeu_0927AF38[arrIndex][2];
                prim->y1 = posY + D_pspeu_0927AF38[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_pspeu_0927AF38[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_pspeu_0927AF38[arrIndex][5];
                prim->type = PRIM_G4;
                prim->priority = RIC.zPriority + 2;
            }
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.timer.t = 20;
        self->step++;
        break;
    case 1:
        if (--self->ext.timer.t == 0) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                FAKEPRIM->posX.i.hi = FAKEPRIM->x0;
                FAKEPRIM->posY.i.hi = FAKEPRIM->y0;
                FAKEPRIM->posX.val += FAKEPRIM->velocityX.val;
                FAKEPRIM->posY.val += FAKEPRIM->velocityY.val;
                FAKEPRIM->velocityY.val += FIX(36.0 / 128);
                sp10[i].x = FAKEPRIM->posX.i.hi;
                sp10[i].y = FAKEPRIM->posY.i.hi;
                FAKEPRIM->x0 = FAKEPRIM->posX.i.hi;
                FAKEPRIM->y0 = FAKEPRIM->posY.i.hi;
            } else {
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                arrIndex = prim->u0;
                prim->x0 = posX + D_pspeu_0927AF38[arrIndex][0];
                prim->y0 = posY + D_pspeu_0927AF38[arrIndex][1];
                prim->x1 = posX + D_pspeu_0927AF38[arrIndex][2];
                prim->y1 = posY + D_pspeu_0927AF38[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_pspeu_0927AF38[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_pspeu_0927AF38[arrIndex][5];
            }
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09267520);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09267A60);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_801B2E5C);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09267D20);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09268448);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09268D98);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09269428);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09269E60);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0926A048);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0926A2B8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0926A738);
