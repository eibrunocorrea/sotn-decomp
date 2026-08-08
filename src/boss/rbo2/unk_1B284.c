// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo2.h"

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019A98C_from_rcen);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019B430);

#include "../../st/approach_s16.h"

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019B52C);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019C718);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019C924);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019D4CC);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019D950);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019DA04);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019E558);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019E920);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019ECCC);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019ED80);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", polarPlacePartsWithAngvel);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_801CDD00);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_801CDD80);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_801CDF1C);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_801CE1E8);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_801CE228);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", polarPlacePartsList);

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
            CreateEntityFromEntity(E_UNK_1F, g_CurrentEntity, newEntity);
            newEntity->ext.et_8019D330.unk89 = 6 - i;
            newEntity->ext.et_8019D330.unk88 = variant;
            newEntity->params = 2;
            newEntity->ext.et_8019D330.unk84 = angle;
            newEntity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019F260);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019F4AC);
