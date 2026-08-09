// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo2.h"
#include <cutscene.h>

// helpers are called from raw-asm functions in sibling objects
#define CUTSCENE_GLOBAL

// e_cutscene/cutscene_script_text
#ifdef VERSION_PSP
#define CUTSCENE_UNK1_NEXT_X 0
#define CUTSCENE_UNK1_UNK17 2
#endif

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#ifdef VERSION_PSP
// psp emits the helpers in a different order than psx
#include "../../get_lang.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"
#else
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"
#endif
