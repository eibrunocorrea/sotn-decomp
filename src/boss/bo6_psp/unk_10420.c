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

void func_pspeu_09248220(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    Entity* richter;

    richter = &RIC;

    switch (self->step) {
    case 0:
        primIndex = (s16)g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            g_PauseAllowed = false;
            g_unkGraphicsStruct.pauseEnemies = true;
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            self->animSet = 0;
            prim = &g_PrimBuf[primIndex];
            prim->x0 = prim->x2 = 0;
            prim->x1 = prim->x3 = 0x100;
            prim->y0 = prim->y1 = 4;
            prim->y2 = prim->y3 = 0xE8;
            PCOL(prim) = 0;
            prim->priority = 0x1F8;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 1:
        g_Player.padSim = PAD_NONE;
        if (!func_pspeu_0924A168_from_rbo2() && !self->step_s &&
            (g_Player.vram_flag & TOUCHING_GROUND)) {
            if (self->posX.i.hi < richter->posX.i.hi) {
                g_Player.padSim = PAD_RIGHT;
            } else {
                g_Player.padSim = PAD_LEFT;
            }
            self->step_s++;
        }
        g_Player.demo_timer = 1;
        if (D_pspeu_0927C148 & 0x40) {
            self->step++;
        }
        break;
    case 2:
        g_Player.demo_timer = 1;
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) += 1;

        if (prim->r0 == 0xFF) {
            prim->drawMode = DRAW_COLORS;
            self->step++;
        }
        break;
    case 3:
        g_Player.demo_timer = 1;
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) -= 4;
        if (prim->r0 < 5) {
            g_PauseAllowed = true;
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = false;
            }
            D_800978B4 = 0;
            g_GameState = Game_Ending;
            g_GameStep = Play_Reset;
            self->step++;
        }
        break;
    case 4:
        break;
    }
}

void func_pspeu_09248660(Entity* self) {
    Entity* player = &PLAYER;
    Entity* richter = &RIC;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = true;
        self->animSet = 0;
        if (richter->posX.i.hi < 0x80) {
            if (player->posX.i.hi < 0xC0) {
                g_Player.padSim = PAD_RIGHT;
            } else {
                g_Player.padSim = PAD_LEFT;
            }
        } else {
            if (player->posX.i.hi > 0x40) {
                g_Player.padSim = PAD_LEFT;
            } else {
                g_Player.padSim = PAD_RIGHT;
            }
        }
        break;

    case 1:
        g_Player.padSim = PAD_NONE;
        if (!func_pspeu_0924A168_from_rbo2()) {
            if (richter->posX.i.hi < 0x80) {
                if (player->posX.i.hi < 0xC0) {
                    g_Player.padSim = PAD_RIGHT;
                } else {
                    if (!player->facingLeft) {
                        g_Player.padSim = PAD_LEFT;
                    }
                    self->step++;
                }
            } else if (player->posX.i.hi > 0x40) {
                g_Player.padSim = PAD_LEFT;
            } else {
                if (player->facingLeft) {
                    g_Player.padSim = PAD_RIGHT;
                }
                self->step++;
            }
        }
        g_Player.demo_timer = 1;
        break;

    case 2:
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        if (D_pspeu_0927C148 & 8) {
            g_PauseAllowed = true;
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = false;
            }
            DestroyEntity(self);
        }
        return;
    }
}
