// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcen.h"

extern EInit g_EInitCommon;
extern u16 D_us_801809FC[];

static void func_8018F8EC(u16 index) {
    Tilemap* tilemap = &g_Tilemap;
    u16* tileLayoutPtr;
    u16 tilePos = 0x316;
    s32 i, j;

    tileLayoutPtr = &D_us_801809FC[index * 8];

    for (i = 0; i < 2; tilePos += 0x2C, i++) {
        for (j = 0; j < 4; tilePos++, j++) {
            tilemap->fg[tilePos] = *tileLayoutPtr++;
        }
    }
}

INCLUDE_ASM("st/rcen/nonmatchings/unk_1F0D8", func_us_8019F148);

INCLUDE_ASM("st/rcen/nonmatchings/unk_1F0D8", func_us_8019F5F0);

INCLUDE_ASM("st/rcen/nonmatchings/unk_1F0D8", func_us_8019F9C0);

INCLUDE_ASM("st/rcen/nonmatchings/unk_1F0D8", func_us_801B4148_from_bo0);

void func_us_801C123C_from_no4(Entity* self) {
    if (self->step == 0) {
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(1);
        self->animCurFrame = 7;
        self->drawFlags = 4;
        self->rotate = 0x800;
        self->zPriority++;
    }
}
