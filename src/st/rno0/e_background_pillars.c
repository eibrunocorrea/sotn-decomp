// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno0.h"

#define g_EInitSpawner OVL_EXPORT(EInitSpawner)
#define E_ID_16 E_UNK_16
#define func_us_801CC8F8 func_us_801CC8F8_from_no0
#define func_us_801CC9B4 func_us_801CC9B4_from_no0

#define BG_PILLARS_U_LEFT 0xE0
#define BG_PILLARS_U_RIGHT 0xC0
#define BG_PILLARS_Y_TOP 0x5F
#define BG_PILLARS_Y_BOTTOM 0x3F
#define BG_PILLARS_SPAWN_Y 0x6A
#define BG_PILLARS_ANIMSET ANIMSET_OVL(2)

extern EInit g_EInitSpawner;

#include "../e_background_pillars.h"
