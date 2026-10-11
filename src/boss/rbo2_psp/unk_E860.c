// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019F4AC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", EntityBreakable);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_pspeu_09246448);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", polarPlacePartsWithAngvel);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_801CDD00);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_801CDD80);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_801CDF1C);

void func_801CE1E8(s32 step) {
    s32 i;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", polarPlacePartsList);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019A98C);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_pspeu_09246AE8);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_801CDC80);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019B52C);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C718);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C924);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D4CC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D950);
