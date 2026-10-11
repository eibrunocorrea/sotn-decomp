// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

bool func_pspeu_0924A168_from_rbo2(void) {
    if (g_Player.status & PLAYER_STATUS_TRANSFORM) {
        g_Player.padSim = PAD_NONE;
        if (g_Timer & 1) {
            if (g_Player.status & PLAYER_STATUS_BAT_FORM) {
                g_Player.padSim = PAD_BAT;
            } else if (g_Player.status & PLAYER_STATUS_MIST_FORM) {
                g_Player.padSim = PAD_NONE;
            } else if (g_Player.status & PLAYER_STATUS_WOLF_FORM) {
                g_Player.padSim = PAD_WOLF;
            }
        }
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09247B58);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09247D10);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09248220);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09248660);
