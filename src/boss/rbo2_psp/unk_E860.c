// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

extern s32 E_ID(UNK_17);

typedef struct {
    u8 pad[0xAC];
    s16 unkAC;
} unkStr_09246448;

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019F4AC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", EntityBreakable);

void func_pspeu_09246448(Entity* self) {
    s16 angle = self->ext.GH_Props.rotate;
    Entity* src;

    angle += ((unkStr_09246448*)self)->unkAC;
    if (g_CurrentEntity->facingLeft) {
        angle = -angle;
    }
    src = self->ext.GH_Props.parent;
    self->posX.val = src->posX.val;
    self->posY.val = src->posY.val;
    self->posX.val -= self->ext.GH_Props.length * rsin(angle) * 16;
    self->posY.val += self->ext.GH_Props.length * rcos(angle) * 16;
    self->ext.GH_Props.unkA8 |= 1;
}

void polarPlacePartsWithAngvel(s16* entOffsets) {
    Entity* ent;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            ent = g_CurrentEntity + *entOffsets;
            ent->ext.GH_Props.rotate += ent->ext.GH_Props.rotVel;
            func_pspeu_09246448(ent);
        }
        entOffsets++;
    }
}

void func_801CDD00(Entity* entity, s16 arg1, s16 arg2) {
    s16 temp_t0 = arg1 - entity->ext.GH_Props.rotate;

    if (temp_t0 > ROT(180)) {
        temp_t0 = temp_t0 - ROT(360);
    }

    if (temp_t0 < ROT(-180)) {
        temp_t0 = temp_t0 + ROT(360);
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

void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!entity->ext.GH_Props.unkA8) {
            func_pspeu_09246448(entity);
        }
        offsets++;
    }
}

s16 func_us_8019A98C(s16 arg0, s16 arg1, s16 arg2) {
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

void func_pspeu_09246AE8(void) {
    Entity* entity;
    s32 posX;
    s32 posY;

    if (!(g_Timer & 0xF)) {
        entity = AllocEntity(&g_Entities[160], &g_Entities[168]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_ID(UNK_17), entity);
            posX = rand() & 0xFF;
            posY = (rand() & 0x3F) + 0x20;
            entity->posX.i.hi = posX;
            entity->posY.i.hi = posY;
        }
    }
}

#include "../../st/approach_s16.h"

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019B52C);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C718);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C924);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D4CC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D950);
