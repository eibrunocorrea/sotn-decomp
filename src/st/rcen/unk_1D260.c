// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcen.h"

// This function does not appear to have a corresponding func on PSP - likely
// unused and stripped
static void SpawnEntityCluster(void) {
    s32 i;
    u8 variant;
    s16 angle;
    Entity* newEntity;

    variant = Random() & 0x3;
    angle = ((Random() & 0xF) << 8) - 0x800;

    for (i = 0; i < 6; i++) {
        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromEntity(E_UNK_20, g_CurrentEntity, newEntity);
            newEntity->ext.et_8019D330.unk89 = 6 - i;
            newEntity->ext.et_8019D330.unk88 = variant;
            newEntity->params = 2;
            newEntity->ext.et_8019D330.unk84 = angle;
            newEntity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

INCLUDE_ASM("st/rcen/nonmatchings/unk_1D260", func_us_8019D330);
