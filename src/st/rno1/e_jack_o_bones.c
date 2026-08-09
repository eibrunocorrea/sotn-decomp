// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno1.h"

#define E_JACKO_DEATH_PARTS E_JACKO_BONES_DEATH_PARTS
#define E_JACKO_JACK E_JACKO_BONES_JACK

#ifdef VERSION_PSP
// psp symbols keep the D_us_* names used by sibling asm objects
#define g_EInitJackOBones2 D_us_8018070C
#define g_EInitJackOBones3 D_us_80180718
#endif

#include "../e_jack_o_bones.h"
