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

extern u32 g_CutsceneFlags;
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
            g_CutsceneFlags = 1;
        }
        g_Player.demo_timer = 1;
        break;

    case 2:
        if (g_CutsceneFlags & 2) {
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

void func_pspeu_09247D10(Entity* self) {
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
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

            PCOL(prim) = 0x80;

            prim->priority = 0x1F8;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            self->ext.utimer.t = 0;
            g_api.PlaySfx(SFX_WEAPON_APPEAR);
        }
        self->flags |= FLAG_UNK_10000;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        self->ext.utimer.t++;
        if (self->ext.utimer.t > 8) {
            self->step++;
            prim->drawMode = DRAW_HIDE;
        } else if (self->ext.utimer.t & 1) {
            prim->drawMode = DRAW_HIDE;
        } else {
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 2:
        if (g_CutsceneFlags & 0x100) {
            g_unkGraphicsStruct.unk28 = 0;
            self->step++;
            self->ext.utimer.t = 0;
            prim = &g_PrimBuf[self->primIndex];
            PCOL(prim) = 0;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 3:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) += 1;

        if (prim->r0 == 0xFF) {
            prim->drawMode = DRAW_COLORS;
            self->step++;
        }
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) -= 4;

        if (prim->r0 < 5) {
            g_PauseAllowed = true;
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = false;
            }
            if (g_CastleFlags[MET_MARIA_IN_CEN]) {
                D_800978B4 = 2;
            } else {
                D_800978B4 = 1;
            }
            g_GameState = Game_Ending;
            g_GameStep = Play_Reset;
            self->step++;
        }
        break;
    }
}

void func_pspeu_09248220(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    Entity* richter;

    richter = &RIC;

    switch (self->step) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_G4, 1);
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
        if (g_CutsceneFlags & 0x40) {
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
        if (g_CutsceneFlags & 8) {
            g_PauseAllowed = true;
            if (g_unkGraphicsStruct.pauseEnemies) {
                g_unkGraphicsStruct.pauseEnemies = false;
            }
            DestroyEntity(self);
        }
        return;
    }
}
