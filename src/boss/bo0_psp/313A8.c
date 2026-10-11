// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925DBD0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E070);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_psp_09239568);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_us_801B163C);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E340);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E488);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E578);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E688);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E778);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925E8C8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EA30);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EAD8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EC00);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925ECA8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925ED50);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EDF8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/313A8", func_pspeu_0925EF08);

s32 func_pspeu_0925E340(Primitive* prim, s16 x, s16 y, s16 speed);

void func_pspeu_0925EFF8(Primitive* prim) {
    switch (prim->u2) {
    case 0:
        if (func_pspeu_0925E340(prim, -0x40, 0x40, 8)) {
            prim->u2++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, -0x100, 0x380, 0x14)) {
            prim->u2++;
        }
        break;
    }
}
