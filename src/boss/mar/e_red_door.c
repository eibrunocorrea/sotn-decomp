// SPDX-License-Identifier: AGPL-3.0-or-later
#include "mar.h"

#ifdef VERSION_PSP
// MAR on PSP links two EInitCommon copies: the shared framework one owns the
// g_EInitCommon name (symexport, 0x092666B8), while the red door uses the
// overlay-local copy (labeled RBO3EInitCommon in symbols.pspeu.bomar.txt,
// 0x0924BF20; the RBO3 prefix is an upstream naming relic).
#define g_EInitCommon RBO3EInitCommon
#endif

#include "../../st/e_red_door.h"
