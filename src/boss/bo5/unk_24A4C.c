// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#include "../../st/set_cutscene_events.h"

#define STAGE_IS_DRE
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_scale_avatar.h"

INCLUDE_RODATA("boss/bo5/nonmatchings/unk_24A4C", D_us_8019F84C);

INCLUDE_RODATA("boss/bo5/nonmatchings/unk_24A4C", D_us_8019F854);

INCLUDE_ASM("boss/bo5/nonmatchings/unk_24A4C", EntityCutscene);
