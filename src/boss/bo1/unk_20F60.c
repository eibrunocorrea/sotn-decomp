// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo1.h"

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801BA164_from_cat);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801BA388_from_cat);

#include "../../st/approach_s16.h"

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A1878);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A2774);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A2BC4);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A2CC4);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A2D90);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A2F2C);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A3480);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A38EC);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A4394);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A45D0);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A493C);

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A4AF4);

static void SpawnEntityCluster_from_rcen(void) {
    s32 i;
    u8 variant;
    s16 angle;
    Entity* newEntity;

    variant = Random() & 0x3;
    angle = ((Random() & 0xF) << 8) - 0x800;

    for (i = 0; i < 6; i++) {
        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromEntity(E_UNK_22, g_CurrentEntity, newEntity);
            newEntity->ext.et_8019D330.unk89 = 6 - i;
            newEntity->ext.et_8019D330.unk88 = variant;
            newEntity->params = 2;
            newEntity->ext.et_8019D330.unk84 = angle;
            newEntity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

INCLUDE_ASM("boss/bo1/nonmatchings/unk_20F60", func_us_801A518C);
