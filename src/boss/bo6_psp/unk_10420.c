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

extern u32 D_pspeu_0927C148;
extern EInit g_EInitInteractable;

void func_pspeu_09247B58(Entity* self) {
    Entity* player = &PLAYER;
    s16 playerGlobalX = player->posX.i.hi + g_Tilemap.scrollX.i.hi;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = true;
        g_Player.padSim = PAD_LEFT;
        g_Entities[E_AFTERIMAGE_1].ext.afterImage.disableFlag = 0;
        g_Player.demo_timer = 1;
        break;

    case 1:
        if (playerGlobalX > 0xC0) {
            g_Player.padSim = PAD_LEFT;
        } else {
            g_Player.padSim = PAD_NONE;
            self->step++;
            D_pspeu_0927C148 = 1;
        }
        g_Player.demo_timer = 1;
        break;

    case 2:
        if (D_pspeu_0927C148 & 2) {
            self->step++;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;

    case 3:
        g_PauseAllowed = true;
        if (g_unkGraphicsStruct.pauseEnemies) {
            g_unkGraphicsStruct.pauseEnemies = false;
        }
        g_Entities[E_AFTERIMAGE_1].ext.afterImage.disableFlag = 1;
        DestroyEntity(self);
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09247D10);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09248220);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_10420", func_pspeu_09248660);
