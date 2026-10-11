// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_09258DE0);

s32 func_pspeu_09258EF0(s16 x, s16 y) {
    Collider collider;

    g_api.CheckCollision(x, y, &collider, 0);
    if (collider.effects & EFFECT_SOLID) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_us_801AD338);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925B1B0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925C178);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925C580);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925C938);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925CBE8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", EntityOlroxAfterImage);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_us_801B001C);
