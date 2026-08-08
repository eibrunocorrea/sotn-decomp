// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern EInit g_EInitEnvironment;

void func_us_801AB9EC(Primitive* prim);

INCLUDE_ASM("boss/bo0/nonmatchings/2B9EC", func_us_801AB9EC);

INCLUDE_ASM("boss/bo0/nonmatchings/2B9EC", func_us_801ABFE0);

void func_us_801AC54C(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->hitboxState = 0;
        self->animCurFrame = 0;
        break;

    case 1:
        if (g_CastleFlags[NO2_SECRET_WALL_OPEN] & 2) {
            DestroyEntity(self);
            return;
        }
        if (g_CastleFlags[NO2_SECRET_WALL_OPEN]) {
            g_CastleFlags[NO2_SECRET_WALL_OPEN] |= 2;
        }
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableNo2.unk7C = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
            return;
        }
        prim = self->ext.breakableNo2.unk7C;
        for (i = 0; i < 4; i++) {
            UnkPolyFunc2(prim);
            prim->next->x1 = self->posX.i.hi;
            prim->next->y0 = self->posY.i.hi;
            prim->next->r3 = i + 8;
            prim = prim->next;
            prim = prim->next;
        }
        self->step++;
        break;

    case 2:
        i = 1;
        prim = self->ext.breakableNo2.unk7C;
        while (prim != NULL) {
            if (prim->p3 & 8) {
                i = 0;
                func_us_801AB9EC(prim);
            }
            prim = prim->next;
        }
        if (i != 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}

void func_us_801AC73C(Primitive* prim) {
    s32 x, y;

    if (!prim->g3) {
        prim->u0 = 1;
        prim->v0 = 1;
        prim->r0 = 0x80;
        prim->g0 = 0x80;
        prim->b0 = 0xC0;
        prim->drawMode = DRAW_UNK02;
        prim->x0 = g_CurrentEntity->posX.i.hi;
        prim->y0 = g_CurrentEntity->posY.i.hi + 8;
        prim->x1 = 0;
        prim->y1 = 0;
        LOW(prim->x2) = 0x7000 - ((Random() & 7) << 0xD);
        LOW(prim->x3) = 0x7000 - ((Random() & 7) << 0xD);
        prim->g3 = 1;
        prim->r3 = 0x20;
    }
#ifdef VERSION_US
    x = (prim->x0 << 0x10) + (u16)prim->x1;
#else
    x = (prim->x0 << 0x10) + prim->x1;
#endif
    x += LOW(prim->x2);
    prim->x0 = HIHU(x);
    prim->x1 = LOHU(x);
#ifdef VERSION_US
    y = (prim->y0 << 0x10) + (u16)prim->y1;
#else
    y = (prim->y0 << 0x10) + prim->y1;
#endif
    y += LOW(prim->x3);
    prim->y0 = HIH(y);
    prim->y1 = LOH(y);
    LOW(prim->x3) += 0x2000;
    prim->r3 -= 1;
    if (!prim->r3) {
        prim->g3 = 0;
        prim->drawMode = DRAW_HIDE;
        prim->p3 = 0;
    }
}

INCLUDE_ASM("boss/bo0/nonmatchings/2B9EC", func_us_801AC894);
