// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_TILEMAP_SCROLL
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_skip.h"

#include "../../st/cutscene_scale_avatar.h"

INCLUDE_RODATA("boss/rbo6/nonmatchings/unk_2208C", D_us_8019CE94);

INCLUDE_RODATA("boss/rbo6/nonmatchings/unk_2208C", D_us_8019CEA0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", EntityCutscene);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A362C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A367C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A37B4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A399C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A3BE0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A4028);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A9208_from_bo6);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", DecreaseBrightness);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A4594);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_2208C", func_us_801A4F14);
