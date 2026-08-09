// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo3/rbo3.h"

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", EntityBreakable);

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", func_801A8620);

// reverse castle draws the sky band at the top of the screen
#define SKY_LAND_Y_TOP 64
#define SKY_LAND_Y_BOTTOM 18
#define PAL_BACKGROUND_SKY_LAND 0xC5
#include "../../st/e_background_sky_land.h"

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", func_pspeu_092460E0);

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", func_pspeu_09246CE0);

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", func_pspeu_09247080);

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", EntityHolyLightning);

INCLUDE_ASM("boss/rbo3_psp/nonmatchings/rbo3_psp/unk_E1F8", EntityBackgroundBlock);
