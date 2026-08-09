// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo4.h"

#ifdef VERSION_PSP
// BO4 on PSP links two EInitCommon copies: the shared framework one owns the
// g_EInitCommon name (symexport, 0x092666B8), while the red door uses the
// overlay-local copy (BO4EInitCommon, 0x09263388).
#define g_EInitCommon BO4EInitCommon
#endif

#include "../../st/e_red_door.h"
