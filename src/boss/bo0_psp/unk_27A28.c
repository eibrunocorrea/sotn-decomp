// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern EInit D_pspeu_09275F90;

s16 func_pspeu_0925F0A8(OlroxPrim* prim) {
    s16 posY;
    u16 hitKind;

    posY = g_Tilemap.scrollY.i.hi + prim->unk20.i.hi;
    if (posY >= 0x1CC) {
        posY = 0x1CC - posY;
        hitKind = 1;
    } else {
        posY = 0;
        hitKind = 0;
    }
    prim->priority = hitKind;
    FntPrint("hit_kind %x\n", hitKind);
    return posY;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_0925F158);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_0925F530);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_0925F7A0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092608C0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092611E8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09264380);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09264460);

u8 func_pspeu_09265170(s16* values, s16* targets, s16 speed) {
    s32 i;
    u8 ret = true;

    for (i = 0; i < 8;) {
        if (abs(*values - *targets) < speed) {
            *values = *targets;
        } else {
            ret = false;
            if (*values > *targets) {
                *values -= speed;
            }
            if (*values < *targets) {
                *values += speed;
            }
        }
        values++;
        i++;
        values++;
        targets++;
    }
    return ret;
}

s32 func_us_801B163C(s16* value, s16 target, s16 speed);

u8 func_pspeu_092652C0(s16* values, s16* targets, s16 speed) {
    s32 i;
    u8 ret = true;

    for (i = 0; i < 8;) {
        if (!func_us_801B163C(values, *targets, speed)) {
            ret = false;
        }
        values++;
        i++;
        values++;
        targets++;
    }
    return ret;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09265368);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092657B8);

s32 func_pspeu_09265CA0(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 result;
    s16* value;

    result = 0;
    value = &g_CurrentEntity->ext.et_801B619C.unk88;
    result += func_us_801B163C(value, arg2.i.lo, arg2.i.hi);
    value += 2;
    result += func_us_801B163C(value, arg3.i.lo, arg3.i.hi);
    value = &g_CurrentEntity->ext.et_801B619C.unk80;
    result += func_us_801B163C(value, arg0.i.lo, arg0.i.hi);
    value += 2;
    result += func_us_801B163C(value, arg1.i.lo, arg1.i.hi);
    if (result == 4) {
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09265D78);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092667C8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09267730);

void func_pspeu_09267E48(Entity* self) {
    OlroxPrim* part;

    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    if (!self->step) {
        InitializeEntity(D_pspeu_09275F90);
        self->animCurFrame = 0;
        self->hitboxOffY = 4;
        self->hitboxWidth = 4;
        self->hitboxHeight = 0x10;
    }
    part = self->ext.et_801B5470.unk8C;
    self->posX.i.hi = part->unk28.i.hi;
    self->posY.i.hi = part->unk2C.i.hi;
}

Primitive* func_pspeu_09267EF8(Primitive* prim, u8 count) {
    s32 i;
    Primitive* current = prim;

    while (current != NULL) {
        if (current->drawMode & DRAW_HIDE) {
            prim = current;
            for (i = 1; i < count; i++) {
                current = current->next;
                if (!current) {
                    return NULL;
                }
                if (!(current->drawMode & DRAW_HIDE)) {
                    break;
                }
            }
            if (i == count) {
                return prim;
            }
        }
        current = current->next;
    }
    return NULL;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09267FC0);
