// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#include "../../get_lang.h"

#include "../../st/cutscene_actor_name.h"

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"

#define CUTSCENE_UNK1_NEXT_X 0
#define CUTSCENE_UNK1_UNK17 2
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"

#include "../../st/set_cutscene_events.h"

extern u32 g_CutsceneFlags;
extern PfnEntityUpdate EntityUpdates[];

void RunCutsceneEvents(void) {
    Entity* entity;
    u16 startTimer;

    g_Dialogue.timer++;
    // protect from overflows
    if (g_Dialogue.timer >= 0xFFFF) {
        g_Dialogue.hasEvents = 0;
        return;
    }
    while (true) {
        // Run the next event only once its start timer has elapsed
        startTimer = *g_Dialogue.eventCur++ << 8;
        startTimer |= *g_Dialogue.eventCur++;
        if (g_Dialogue.timer < startTimer) {
            // Re-evaluate the condition at the next frame
            g_Dialogue.eventCur -= 2;
            return;
        }
        switch (*g_Dialogue.eventCur++) {
        case CSEV_SPAWN:
            entity =
                &g_Entities[*g_Dialogue.eventCur++ & 0xFF] + STAGE_ENTITY_START;
            DestroyEntity(entity);
            entity->entityId = *g_Dialogue.eventCur++;
            entity->pfnUpdate = EntityUpdates[entity->entityId - 1];
            entity->posX.i.hi = *g_Dialogue.eventCur++ * 0x10;
            entity->posX.i.hi |= *g_Dialogue.eventCur++;
            entity->posY.i.hi = *g_Dialogue.eventCur++ * 0x10;
            entity->posY.i.hi |= *g_Dialogue.eventCur++;
            entity->posX.i.hi -= g_Tilemap.scrollX.i.hi;
            entity->posY.i.hi -= g_Tilemap.scrollY.i.hi;
            break;
        case CSEV_DESTROY:
            entity =
                &g_Entities[*g_Dialogue.eventCur++ & 0xFF] + STAGE_ENTITY_START;
            DestroyEntity(entity);
            break;
        case CSEV_WAIT_FOR_FLAG:
            if (!((g_CutsceneFlags >> *g_Dialogue.eventCur) & 1)) {
                g_Dialogue.eventCur--;
                return;
            }
            g_CutsceneFlags &= ~(1 << *g_Dialogue.eventCur++);
            break;
        case CSEV_SET_FLAG:
            g_CutsceneFlags |= 1 << *g_Dialogue.eventCur++;
            break;
        }
    }
}

#include "../../st/cutscene_skip.h"

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/e_cutscene_dialogue", EntityCutscene);
