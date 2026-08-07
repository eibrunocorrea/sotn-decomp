// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo7.h"

/*
 * BO7's breakable entity is stage-specific and roughly twice the size of the
 * shared candle implementation (0x290 versus 0x134 bytes).
 */
INCLUDE_ASM("boss/bo7/nonmatchings/e_breakable", EntityBreakable);

INCLUDE_ASM("boss/bo7/nonmatchings/e_breakable", EntityBreakableDebris);
