// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

extern EInit g_EInitInteractable;
extern s32 D_pspeu_09265CC8;
extern u32 g_CutsceneFlags;
extern s32 E_ID(LIFE_UP_SPAWN);

void EntityBossFightManager(Entity* self) {
    Entity* entity;
    s32 posX;
    s32 posY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->flags |= FLAG_UNK_10000;
        // fallthrough

    case 1:
        entity = &PLAYER;
        posX = entity->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if ((posX > 0x40) && (posX < 0x1C0)) {
            D_pspeu_09265CC8 |= 1;
            self->step++;
        }
        break;

    case 2:
        if (g_CastleFlags[DEATH_FIGHT_CS] ||
            (g_PlayableCharacter != PLAYER_ALUCARD) ||
            (g_DemoMode != Demo_None)) {
            entity = &PLAYER;
            posX = entity->posX.i.hi + g_Tilemap.scrollX.i.hi;
            if ((posX <= 0x80) || (posX >= 0x180)) {
                break;
            }
        } else if (!(g_CutsceneFlags & 2)) {
            break;
        }
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_DEATH_DEFEAT, TIMEATTACK_SET_VISITED);
        D_pspeu_09265CC8 |= 2;
        stopMusicFlag = true;
        currentMusicId = MU_DEATH_BALLAD;
        self->step++;
        break;

    case 3:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        // fallthrough

    case 4:
        if (D_pspeu_09265CC8 & 0x10) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_DEATH_DEFEAT, TIMEATTACK_SET_RECORD);
            g_api.PlaySfx(SET_UNK_90);
            currentMusicId = MU_ABANDONED_PIT;
            self->step++;
        }
        break;

    case 5:
        if (D_pspeu_09265CC8 & 0x40) {
            self->step++;
        }
        break;

    case 6:
        posX = 0x100 - g_Tilemap.scrollX.i.hi;
        posY = 0x80 - g_Tilemap.scrollY.i.hi;
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_ID(LIFE_UP_SPAWN), self, entity);
            entity->posX.i.hi = posX;
            entity->posY.i.hi = posY;
            entity->params = 0x15;
            D_pspeu_09265CC8 |= 0x80;
            stopMusicFlag = true;
            currentMusicId = MU_ABANDONED_PIT;
            self->step++;
        }
        break;

    case 7:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    }

    FntPrint("set_step %x\n", self->step);
    FntPrint("boss_flag %x\n", D_pspeu_09265CC8);
}
