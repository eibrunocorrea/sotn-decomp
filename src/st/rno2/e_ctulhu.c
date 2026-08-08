// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

#define STAGE_IS_RNZ0
#define DISABLE_TRIPLE_FIREBALL

// rno2's ctulhu data has no semantic symbols yet; alias the shared-header
// names to the local blob labels (same pattern as rnz0's e_ctulhu.c)
#define g_EInitCtulhuFireball D_us_801808E0
#define g_EInitCtulhuIceShockwave D_us_801808EC
#define ctulhu_shockwave_uvs D_us_801AF800

#include "../e_ctulhu.h"
