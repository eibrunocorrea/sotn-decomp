// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

s32 func_pspeu_0925E340(OlroxPrim* prim, s16 x, s16 y, s16 speed);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925DBD0);

void func_pspeu_0925E070(u8 step) {
    OlroxPrim* part;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    part = g_CurrentEntity->ext.et_801B365C.unk80;
    part->step = 0;
    part = part->unk18;
    part->step = 0;
}

// twin of func_801CDC80 (src/st/giantbro_helpers.h)
int func_psp_09239568(s16* arg0, s16 arg1, s16 arg2) {
    if (abs(*arg0 - arg1) < arg2) {
        *arg0 = arg1;
        return true;
    }

    if (*arg0 > arg1) {
        *arg0 -= arg2;
    }

    if (*arg0 < arg1) {
        *arg0 += arg2;
    }

    return false;
}

s32 func_us_801B163C(s16* value, s16 target, s16 speed) {
    s16* velocity;
    s16 delta;
    s16 difference;
    s32 acceleration;

    velocity = value + 1;
    acceleration = speed / 16;
    if (!acceleration) {
        acceleration = 1;
    }
    difference = target - *value;
    delta = *velocity;
    if (difference > 0) {
        if (delta < speed) {
            delta += (s16)acceleration;
        } else {
            delta -= (s16)acceleration;
        }
    } else {
        if (delta > -speed) {
            delta -= (s16)acceleration;
        } else {
            delta += (s16)acceleration;
        }
    }
    if (abs(difference) < abs(delta)) {
        *value = target;
        *velocity = difference;
        return 1;
    }
    *value += delta;
    *velocity = delta;
    return 0;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E340);

void func_pspeu_0925E488(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(45), 0x18)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(-5.625), 0x20)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(-67.5), ROT(-5.625), 0x20)) {
            prim->step++;
        }
        break;
    }
}

void func_pspeu_0925E578(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-5.625), ROT(5.625), 0x1C)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 > -0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 -= 0xC;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-22.5), ROT(67.5), 0x18)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 < 0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 += 8;
        }
        break;
    }
}

void func_pspeu_0925E688(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-45), ROT(67.5), 0x18)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(0), ROT(56.25), 0x1C)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(11.25), ROT(33.75), 0x18)) {
            prim->step++;
        }
        break;
    }
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E778);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E8C8);

s32 func_pspeu_0925EA30(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, -0x100, 0xC);
    result = func_pspeu_0925E340(part, ROT(-39.375), ROT(56.25), 0x10);
    result += func_pspeu_0925E340(other, ROT(-78.75), ROT(28.125), 0x10);
    if (result == 2) {
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EAD8);

s32 func_pspeu_0925EC00(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0x0, 0xC);
    result = func_pspeu_0925E340(part, ROT(-61.875), ROT(61.875), 0x28);
    result += func_pspeu_0925E340(other, ROT(-22.5), ROT(84.375), 0x38);
    if (result == 2) {
        return true;
    }
    return false;
}

s32 func_pspeu_0925ECA8(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0x0, 0xC);
    result = func_pspeu_0925E340(part, ROT(-5.625), ROT(5.625), 0x60);
    result += func_pspeu_0925E340(other, ROT(-45), ROT(56.25), 0x50);
    if (result == 2) {
        return true;
    }
    return false;
}

s32 func_pspeu_0925ED50(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0x0, 0x10);
    result = func_pspeu_0925E340(part, ROT(-45), ROT(45), 0x60);
    result += func_pspeu_0925E340(other, ROT(-45), ROT(33.75), 0x40);
    if (result == 2) {
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EDF8);

void func_pspeu_0925EF08(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(45), 8)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(-5.625), 8)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(-67.5), ROT(-5.625), 0xC)) {
            prim->step++;
        }
        break;
    }
}

void func_pspeu_0925EFF8(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-5.625), ROT(5.625), 8)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-22.5), ROT(78.75), 0x14)) {
            prim->step++;
        }
        break;
    }
}
