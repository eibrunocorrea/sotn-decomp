// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

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
            CreateEntityFromEntity(E_UNK_16, g_CurrentEntity, newEntity);
            newEntity->ext.et_8019D330.unk89 = 6 - i;
            newEntity->ext.et_8019D330.unk88 = variant;
            newEntity->params = 2;
            newEntity->ext.et_8019D330.unk84 = angle;
            newEntity->zPriority = g_CurrentEntity->zPriority + 1;
        }
    }
}

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019D330_from_rcen);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019DB9C);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019EADC);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019EE30);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019F1CC);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FA78);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FB04);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FBC0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_8019FCB4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A01A4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0710);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0860);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0AB4);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A0DC0);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A1150);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A11DC);

INCLUDE_ASM("boss/rbo6/nonmatchings/unk_1D690", func_us_801A1B38);

#include <cutscene.h>

// helpers are called from raw-asm functions in sibling objects
#define CUTSCENE_GLOBAL

extern Dialogue g_Dialogue;

// matches CutsceneUnk1 (see ../../st/cutscene_unk1.h), but this overlay never
// named it that -- func_us_801A1BE0 is a raw asm label referenced directly by
// EntityCutscene (out of scope), so it keeps its original name here.
#define CutsceneUnk1 func_us_801A1BE0
#define CUTSCENE_UNK1_NEXT_X 0
#include "../../st/cutscene_unk1.h"

#include "../../st/set_cutscene_script.h"
#undef CutsceneUnk1

#include "../../st/cutscene_unk3.h"

#include "../../st/cutscene_unk4.h"
