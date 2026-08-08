// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcen.h"

s16 func_us_8019A98C(s16 arg0, s16 arg1, s16 arg2) {
    s16 v_s1;
    s16 v_s0;

    arg1 &= 0xFFF;

    v_s1 = arg2 - arg1;
    v_s0 = v_s1;

    if (v_s1 > ROT(180)) {
        v_s0 = v_s1 - ROT(360);
    }
    if (v_s1 < ROT(-180)) {
        v_s0 = v_s1 + ROT(360);
    }

    if (abs(v_s0) > arg0) {
        if (v_s1 < 0) {
            v_s0 = arg1 - arg0;
        } else {
            v_s0 = arg1 + arg0;
        }
        return v_s0;
    }

    return arg2;
}

void func_us_8019AA04(s16 sfxId) {
    s32 posX, posY;
    s16 sfxPan;
    s16 sfxVol;

    posX = g_CurrentEntity->posX.i.hi - 128;
    sfxPan = (abs(posX) - 32) >> 5;
    if (sfxPan > 8) {
        sfxPan = 8;
    } else if (sfxPan < 0) {
        sfxPan = 0;
    }
    if (posX < 0) {
        sfxPan = -sfxPan;
    }
    sfxVol = abs(posX) - 96;
    posY = abs(g_CurrentEntity->posY.i.hi - 128) - 112;
    if (posY > 0) {
        sfxVol += posY;
    }
    if (sfxVol < 0) {
        sfxVol = 0;
    }
    sfxVol = 64 - (sfxVol >> 1);
    if (sfxVol > 0) {
        g_api.PlaySfxVolPan(sfxId, sfxVol, sfxPan);
    }
}

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", EntityShaft);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019B5A4);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019B6D4);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019B8A8);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019C4EC);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019C610);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019C7B8);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019CDA0);

INCLUDE_ASM("st/rcen/nonmatchings/e_shaft", func_us_8019CDF8);
