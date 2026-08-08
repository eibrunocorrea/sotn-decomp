// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"
#include <cutscene.h>

// helpers are called from raw-asm functions in sibling objects
#define CUTSCENE_GLOBAL

extern Dialogue g_Dialogue;
extern const char* actor_names[];

#include "../../st/cutscene_actor_name.h"
