// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"
#include <cutscene.h>

// helpers are called from raw-asm functions in sibling objects
#define CUTSCENE_GLOBAL

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#include "../../st/cutscene_actor_name.h"
