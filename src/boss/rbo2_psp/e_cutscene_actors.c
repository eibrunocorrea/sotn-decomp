// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

void CutsceneCameraPan(s16 target) {
    s16 delta = target - g_unkGraphicsStruct.unk14;

    if (delta > 1) {
        g_unkGraphicsStruct.unk14++;
    } else if (delta < -1) {
        g_unkGraphicsStruct.unk14--;
    } else {
        g_unkGraphicsStruct.unk14 = target;
    }
}

bool func_us_801A8FC0_from_bo6(void) {
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
