// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo1/rbo1.h"

INCLUDE_ASM("boss/rbo1_psp/nonmatchings/rbo1_psp/unk_11178", EntityBossFightManager);

// rbo1 uses its own boss-flag bits for the room blocks
#define BOSS_ROOM_BLOCK_FLAG_CLOSE 8
#define BOSS_ROOM_BLOCK_FLAG_OPEN 0x10
#include "../../st/e_boss_room_block.h"

INCLUDE_ASM("boss/rbo1_psp/nonmatchings/rbo1_psp/unk_11178", EntityBreakable);

INCLUDE_ASM("boss/rbo1_psp/nonmatchings/rbo1_psp/unk_11178", EntityBackgroundBlock);
