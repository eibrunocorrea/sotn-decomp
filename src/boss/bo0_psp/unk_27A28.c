// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

s16 func_pspeu_0925F0A8(Primitive* prim) {
    s16 posY;
    u16 hitKind;

    posY = g_Tilemap.scrollY.i.hi + prim->y2;
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

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09265CA0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09265D78);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092667C8);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09267730);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_09267E48);

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
