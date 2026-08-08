// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

#define SPIKES_TILE_WIDTH 96
#define DAMAGE_ENT_ON_HIT
#define DAMAGE_ENT_START g_Entities[160]
#define DAMAGE_ENT_END g_Entities[192]
#define SPIKES_PARTS_VELOCITY_Y FIX(-2.5)

#include "../e_spikes.h"

INCLUDE_RODATA("st/rno2/nonmatchings/e_spikes", D_us_801B1C4C);
