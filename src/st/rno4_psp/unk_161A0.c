// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rno4/rno4.h"

// this overlay's nova skeleton reuses local ids and the blade soldier
// death parts; psp keeps the shared donor names via defines
#define E_NOVA_DEATH_PARTS E_BLADE_SOLDIER_DEATH_PARTS
#define NOVA_DEATH_PARTS_FROM_BLADE_SOLDIER
#define BLADE_DEATH_ANIM_FRAME_BASE 0x1D
#define BLADE_DEATH_FALL_DURATION_FIELD deathPartFallDurationAlt
#define BLADE_DEATH_PARTS_NO_ROTATION_TABLE
#define BLADE_DEATH_ROTATION_TABLE death_parts_rotspeeds
#define g_EInitBladeSoldier g_EInitNovaSkeleton

#include "../e_nova_skeleton.h"
