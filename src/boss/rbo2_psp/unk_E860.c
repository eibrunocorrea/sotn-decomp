// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

void func_pspeu_09246448(Entity* self);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019F4AC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", EntityBreakable);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_pspeu_09246448);

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

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019A98C);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_pspeu_09246AE8);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_801CDC80);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019B52C);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C718);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019C924);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D4CC);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_E860", func_us_8019D950);
