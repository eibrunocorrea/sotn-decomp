// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo7.h"

// Donor: src/boss/rbo0/e_boss.c func_us_80194338 (matched).
bool func_us_80194338_from_rbo0(s16* offsets) {
    s32 posY;

    offsets++;
    posY = g_CurrentEntity->posY.i.hi + *offsets + g_Tilemap.scrollY.i.hi;
    posY = 0xD0 - posY;
    if (posY <= 0) {
        g_CurrentEntity->posY.i.hi += posY;
        g_CurrentEntity->velocityX = 0;
        g_CurrentEntity->velocityY = 0;
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80194D3C);

INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_801959E0);

INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80195AF0);

INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_80195C50);

INCLUDE_ASM("boss/bo7/nonmatchings/unk_14CE0", func_us_801963D8);
