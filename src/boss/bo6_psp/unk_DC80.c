// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#include "../../st/set_cutscene_events.h"

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_DC80", RunCutsceneEvents);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_DC80", CutsceneSkip);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_DC80", EntityCutsceneDialogue);
