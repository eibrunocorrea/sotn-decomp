// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo2.h"

s16 func_us_8019A98C_from_rcen(s16 arg0, s16 arg1, s16 arg2) {
    s16 v_s1;
    s16 v_s0;

    arg1 &= 0xFFF;

    v_s1 = arg2 - arg1;
    v_s0 = v_s1;

    if (v_s1 > ROT(180)) {
        v_s0 = v_s1 - ROT(360);
    }
    if (v_s1 < ROT(-180)) {
        v_s0 = v_s1 + ROT(360);
    }

    if (abs(v_s0) > arg0) {
        if (v_s1 < 0) {
            v_s0 = arg1 - arg0;
        } else {
            v_s0 = arg1 + arg0;
        }
        return v_s0;
    }

    return arg2;
}

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019B430);

#include "../../st/approach_s16.h"

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019B52C);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019C718);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019C924);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019D4CC);

extern s32 D_us_80180B5C;
extern EInit D_us_801804C4;

void func_us_8019D950(Entity* self) {
    s32 params;
    s32 rotate;

    if (D_us_80180B5C & 8) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804C4);
        params = self->params;
        rotate = self->rotate;
        self->drawFlags = 8;
        self->blendMode = 0x30;
        self->opacity = 0x60;
        self->animCurFrame = params;
        if (rotate) {
            self->drawFlags |= 4;
        }
    }
    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019DA04);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019E558);

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019E920);

extern EInit D_us_801804DC;

void func_us_8019ECCC(Entity* self) {
    s32 params;
    s32 rotate;

    if (D_us_80180B5C & 0x10) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804DC);
        params = self->params;
        rotate = self->rotate;
        self->drawFlags = 8;
        self->blendMode = 0x30;
        self->opacity = 0x60;
        self->animCurFrame = params;
        if (rotate) {
            self->drawFlags |= 4;
        }
    }
    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/rbo2/nonmatchings/unk_1B284", func_us_8019ED80);

// twin of polarPlacePart (src/st/giantbro_helpers.h); this overlay never
// resolved the polarPlacePart twin above, so it keeps calling it by its
// still-unmatched local name.
void func_us_8019ED80(Entity* self);

void polarPlacePartsWithAngvel(s16* entOffsets) {
    Entity* ent;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            ent = g_CurrentEntity + *entOffsets;
            ent->ext.GH_Props.rotate += ent->ext.GH_Props.rotVel;
            func_us_8019ED80(ent);
        }
        entOffsets++;
    }
}

void func_801CDD00(Entity* entity, s16 arg1, s16 arg2) {
    s16 temp_t0 = arg1 - entity->ext.GH_Props.rotate;

    if (temp_t0 > 0x800) {
        temp_t0 = temp_t0 - 0x1000;
    }

    if (temp_t0 < -0x800) {
        temp_t0 = temp_t0 + 0x1000;
    }

    temp_t0 = temp_t0 / arg2;
    entity->ext.GH_Props.rotVel = temp_t0;
    entity->ext.GH_Props.unkA4 = arg1;
}

void func_801CDD80(s16* entOffsets, unkStr_801CDD80* arg1) {
    Entity* var_s1;
    s16* ptr = arg1->unk4;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            var_s1 = g_CurrentEntity + *entOffsets;
            func_801CDD00(var_s1, *ptr, arg1->unk0);
        }
        ptr++;
        entOffsets++;
    }
}

void func_801CDF1C(s16 entIndices[], unkStr_801CDD80* arg1, s32 arg2) {

    arg1 += (u16)g_CurrentEntity->ext.GH_Props.unkB0[arg2];

    if (!g_CurrentEntity->ext.GH_Props.unkB4[arg2]) {
        func_801CDD80(entIndices, arg1);
        g_CurrentEntity->ext.GH_Props.unkB4[arg2] = arg1->unk0;
    }
    if (!--g_CurrentEntity->ext.GH_Props.unkB4[arg2]) {
        arg1++;
        if (!arg1->unk0) {
            g_CurrentEntity->ext.GH_Props.unkB0[arg2] = 0;
        } else {
            ++g_CurrentEntity->ext.GH_Props.unkB0[arg2];
        }
    }
}

void func_801CE1E8(s32 step) {
    s32 i;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

void func_801CE228() {
    s32 i;

    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!entity->ext.GH_Props.unkA8) {
            func_us_8019ED80(entity);
        }
        offsets++;
    }
}

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
