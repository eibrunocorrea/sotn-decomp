// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo3.h"

#ifdef VERSION_PSP
// RBO3 on PSP links two EInitCommon copies: the shared framework one owns the
// g_EInitCommon name (symexport, 0x092666B8), while the red door uses the
// overlay-local copy (RBO3EInitCommon, 0x0924A620).
#define g_EInitCommon RBO3EInitCommon
#endif

#include "../../st/e_red_door.h"
