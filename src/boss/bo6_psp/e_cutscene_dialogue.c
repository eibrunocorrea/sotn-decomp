// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"
#include <cutscene.h>

// helpers are called from raw-asm functions in sibling objects
#define CUTSCENE_GLOBAL

extern Dialogue g_Dialogue;

#include "../../get_lang.h"

#include "../../st/cutscene_actor_name.h"

// bo6 draws the dialogue on the right half of the framebuffer
#define CUTSCENE_UNK3_RECT_X 256
#include "../../st/cutscene_unk3.h"

#define CUTSCENE_UNK4_TPAGE 0x14
#include "../../st/cutscene_unk4.h"

#define CUTSCENE_UNK1_NEXT_X 256
#define CUTSCENE_UNK1_UNK17 2
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"
