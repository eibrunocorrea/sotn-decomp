// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

extern EInit g_EInitInteractable;

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", EntityRbo3Door);

void EntityBackgroundSkyLand(Entity* self) {
    Primitive* prim;
    s16 primIndex;
    s32 x;

    if (self->step != 0) {
        return;
    }

    InitializeEntity(g_EInitInteractable);
    primIndex = g_api.AllocPrimitives(PRIM_GT4, 5);

    if (primIndex == -1) {
        DestroyEntity(self);
        return;
    }

    prim = &g_PrimBuf[primIndex];
    self->primIndex = primIndex;
    self->flags |= FLAG_HAS_PRIMS;
    x = 0;

    while (prim != NULL) {
        prim->x0 = prim->x2 = x;
        x += 62;
        prim->x1 = prim->x3 = x;
        prim->tpage = 0xF;
        prim->clut = 0xC5;
        prim->u0 = prim->u2 = 65;
        prim->u1 = prim->u3 = 127;
        prim->v0 = prim->v1 = 169;
        prim->v2 = prim->v3 = 198;
        prim->y0 = prim->y1 = 0xC0;
        prim->y2 = prim->y3 = 0xE4;
        prim->priority = 0x10;
        prim->drawMode = DRAW_DEFAULT;

        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A19CC);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A19FC);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A1BA0);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A1C14);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A3B88);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A3E78);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A3FD4);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A425C);

extern EInit D_us_801804F0;

void func_us_801A4430(Entity* self) {
    if (self->step == 0) {
        InitializeEntity(D_us_801804F0);
        self->hitboxWidth = 0x18;
        self->hitboxHeight = 4;
        self->hitboxOffX = -0x30;
        self->hitboxOffY = -2;
    }
}

INCLUDE_ASM("boss/bo5/nonmatchings/unk_2159C", func_us_801A4494);
