// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

extern EInit g_EInitInteractable;
extern s32 D_pspeu_0926CCD0;
extern s32 D_pspeu_0926CCD8;
extern s32 D_pspeu_0926CD00;
extern s32 D_pspeu_0926CCF8;
extern s32 E_ID(UNK_17);
extern s32 E_ID(UNK_19);
extern s32 E_ID(UNK_1A);
extern s32 E_ID(UNK_1C);
extern s32 E_ID(UNK_1D);

// hit during enter sequence
void func_pspeu_0925CA08(Entity* self) {
    s32 i;
    Entity* dop;
    Entity* entity;
    s32 offsetX;
    s32 offsetY;
    s32 tangent;
    s16 angle;

    FntPrint("pl_demo_timer:%02x\n", g_Player.demo_timer);
    FntPrint("step:%02x\n", self->step);

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);

        self->animSet = 0;
        self->animCurFrame = 0;
        entity = self + 1;

        for (i = 1; i < 6; i++) {
            CreateEntityFromEntity(E_ID(UNK_17), self, entity);
            entity->params = i + 0x100;
            entity++;
            CreateEntityFromEntity(E_ID(UNK_17), self, entity);
            entity->params = i;
            entity++;
        };
        CreateEntityFromEntity(E_ID(UNK_19), self, entity);
        entity++;
        CreateEntityFromEntity(E_ID(UNK_1A), self, entity);
        break;

    case 1:
        if (GetDistanceToPlayerX() < 64) {
            g_Player.demo_timer = 2;
            if (g_Player.status & PLAYER_STATUS_MIST_FORM) {
                g_Player.padSim = PAD_NONE;
            } else if (g_Player.status & PLAYER_STATUS_WOLF_FORM) {
                g_Player.padSim = PAD_L1;
            } else if (g_Player.status & PLAYER_STATUS_BAT_FORM) {
                g_Player.padSim = PAD_R1;
            } else {
                g_Player.demo_timer = 0x240;
                g_Player.padSim = 0;
                self->step = 3;
            }
        }
        break;

    case 2:
        if (!--self->ext.unkDoor.unk9C) {
            self->step++;
        }
        break;

    case 3:
        entity = self + 1;
        entity->ext.unkDoor.unk84 = 1;
        entity++;
        entity->ext.unkDoor.unk84 = 1;
        self->step++;
        break;

    case 4:
        if (D_pspeu_0926CCD0 & 2) {
            // BUG! Sets Doppleganger 10 defeat flag instead of Doppleganger 40
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_DOPPLEGANGER_10_DEFEAT, TIMEATTACK_SET_RECORD);
            self->step++;
        }
        break;

    case 5:
        if (!D_pspeu_0926CCD8) {
            dop = &g_Entities[STAGE_ENTITY_START];
            offsetX = self->posX.i.hi - dop->posX.i.hi;
            offsetY = self->posY.i.hi - dop->posY.i.hi;
            angle = ratan2(-offsetY, offsetX);
            dop->posX.val += rcos(angle) << 4;
            dop->posY.val -= rsin(angle) << 4;

            tangent = (offsetX * offsetX) + (offsetY * offsetY);
            tangent = SquareRoot0(tangent);
            if (tangent < 8) {
                self->step++;
            }
        } else if (D_pspeu_0926CCD8 == 2) {
            self->step++;
        }
        break;

    case 6:
        dop = &g_Entities[STAGE_ENTITY_START];
        dop->zPriority = 0x56;
        if (D_pspeu_0926CCD0 & 4) {
            g_api.PlaySfx(SET_UNK_92);
            self->step++;
        }
        break;

    case 7:
        offsetX = 0x100 - g_Tilemap.scrollX.i.hi;
        offsetY = 0x80 - g_Tilemap.scrollY.i.hi;
        for (i = 0; i < 2; i++) {
            dop = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (dop) {
                CreateEntityFromCurrentEntity(E_ID(UNK_1C), dop);
                dop->posX.i.hi = offsetX - 64 + (i * 128);
                dop->posY.i.hi = offsetY;
                dop->params = i;
            }
        }
        self->ext.unkDoor.unk9C = 0xC0;
        self->step++;
        break;

    case 8:
        if (!--self->ext.unkDoor.unk9C) {
            self->step++;
        }
        break;

    case 9:
        dop = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (dop) {
            CreateEntityFromEntity(E_ID(UNK_1D), self, dop);
            dop->posX.i.hi = 0x100 - g_Tilemap.scrollX.i.hi;
            dop->posY.i.hi = 0x80 - g_Tilemap.scrollY.i.hi;
            dop->params = 4;
            self->step++;
        }
        break;

    case 10:
        break;
    }

    FntPrint("stone_flag:%02x\n", D_pspeu_0926CCD8);
    D_pspeu_0926CCD8 = 0;
    D_pspeu_0926CD00 = self->posX.i.hi;
    D_pspeu_0926CCF8 = self->posY.i.hi;
}
