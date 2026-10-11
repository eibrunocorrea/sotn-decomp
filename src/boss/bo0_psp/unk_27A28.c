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

void func_pspeu_0925F158(void) {
    OlroxPrim* part;
    s16 angle2;
    s16 angle1;
    s32 midX;
    s32 midY;
    s32 endX;
    s32 baseX;
    s32 baseY;
    s32 endY;
    Entity* child1;
    Entity* child2;

    child1 = g_CurrentEntity + 1;
    child2 = g_CurrentEntity + 2;
    part = g_CurrentEntity->ext.et_801B365C.unk80;
    part->unk1C.val = child1->posX.val;
    part->unk20.val = child1->posY.val;
    part = g_CurrentEntity->ext.et_801B365C.unk84;
    part->unk1C.val = child2->posX.val;
    part->unk20.val = child2->posY.val;
    if (g_CurrentEntity->ext.et_801B365C.unk88 == 0) {
        part = g_CurrentEntity->ext.et_801B365C.unk80;
    } else {
        part = g_CurrentEntity->ext.et_801B365C.unk84;
    }
    endX = part->unk1C.val;
    endY = part->unk20.val;
    if (g_CurrentEntity->facingLeft) {
        angle1 = ROT(270) - part->unk10;
    } else {
        angle1 = part->unk10 + ROT(270);
    }
    midX = endX - rcos(angle1) * 42 * 16;
    midY = endY + rsin(angle1) * 42 * 16;
    part->unk28.val = midX;
    part->unk2C.val = midY;
    if (g_CurrentEntity->facingLeft) {
        angle2 = ROT(270) - part->unk0C;
    } else {
        angle2 = part->unk0C + ROT(270);
    }
    baseX = midX - rcos(angle2) * 28 * 16;
    baseY = midY + rsin(angle2) * 28 * 16;
    part->unk14 = endX - baseX;
    if (!g_CurrentEntity->facingLeft) {
        part->unk14 = -part->unk14;
    }
    g_CurrentEntity->posX.val = baseX;
    g_CurrentEntity->posY.val = baseY - FIX(4);
    part = part->unk18;
    if (g_CurrentEntity->facingLeft) {
        angle2 = ROT(270) - part->unk0C;
    } else {
        angle2 = part->unk0C + ROT(270);
    }
    midX = baseX + rcos(angle2) * 28 * 16;
    midY = baseY - rsin(angle2) * 28 * 16;
    part->unk28.val = midX;
    part->unk2C.val = midY;
    if (g_CurrentEntity->facingLeft) {
        angle1 = ROT(270) - part->unk10;
    } else {
        angle1 = part->unk10 + ROT(270);
    }
    endX = midX + rcos(angle1) * 42 * 16;
    endY = midY - rsin(angle1) * 42 * 16;
    part->unk1C.val = endX;
    part->unk20.val = endY;
    part->unk14 = baseX - endX;
    if (g_CurrentEntity->facingLeft) {
        part->unk14 = -part->unk14;
    }
}

void func_pspeu_0925F530(void) {
    OlroxPrim* part;
    s16 angle1;
    s16 angle2;
    s32 baseX;
    s32 midX;
    s32 midY;
    s32 endX;
    s32 i;
    s32 baseY;
    s32 endY;
    Entity* child1;
    Entity* child2;

    child1 = g_CurrentEntity + 1;
    child2 = g_CurrentEntity + 2;
    part = g_CurrentEntity->ext.et_801B365C.unk80;
    part->unk1C.val = child1->posX.val;
    part->unk20.val = child1->posY.val;
    part = g_CurrentEntity->ext.et_801B365C.unk84;
    part->unk1C.val = child2->posX.val;
    part->unk20.val = child2->posY.val;
    part = g_CurrentEntity->ext.et_801B365C.unk80;
    for (i = 0; i < 2; i++) {
        baseX = g_CurrentEntity->posX.val;
        baseY = g_CurrentEntity->posY.val;
        if (g_CurrentEntity->facingLeft) {
            angle1 = ROT(270) - part->unk0C;
        } else {
            angle1 = part->unk0C + ROT(270);
        }
        midX = baseX + rcos(angle1) * 28 * 16;
        midY = baseY - rsin(angle1) * 28 * 16;
        part->unk28.val = midX;
        part->unk2C.val = midY;
        if (g_CurrentEntity->facingLeft) {
            angle2 = ROT(270) - part->unk10;
        } else {
            angle2 = part->unk10 + ROT(270);
        }
        endX = midX + rcos(angle2) * 42 * 16;
        endY = midY - rsin(angle2) * 42 * 16;
        part->unk1C.val = endX;
        part->unk20.val = endY;
        part->unk14 = endX - baseX;
        if (g_CurrentEntity->facingLeft) {
            part->unk14 = -part->unk14;
        }
        part = part->unk18;
    }
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_0925F7A0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092608C0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/unk_27A28", func_pspeu_092611E8);

void func_pspeu_09264380(Entity* self) {
    OlroxPrim* part;

    self->facingLeft = (self - 1)->facingLeft;
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F90);
        self->hitboxWidth = 12;
        self->hitboxHeight = 4;
        self->hitboxOffX = -12;
        self->animCurFrame = 0;
        break;
    case 1:
        part = self->ext.et_801B5470.unk8C;
        self->posX.val = part->unk1C.val;
        self->posY.val = part->unk20.val;
        break;
    }
}

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

void func_pspeu_09265368(void) {
    Primitive* prim;
    s16 angle;
    s16* angles;
    s16 lower;
    s16 upper;
    s16 length;
    s32 i;
    s16 baseX;
    s16 oldX0;
    s16 x0;
    s16 oldX1;
    s16 x1;
    s16 baseY;
    s16 oldY0;
    s16 oldY1;
    s16 delta;
    s16 y0;
    s16 y1;

    baseX = g_CurrentEntity->posX.i.hi;
    baseY = g_CurrentEntity->posY.i.hi;
    angles = &g_CurrentEntity->ext.et_801B619C.unk88;
    prim = g_CurrentEntity->ext.et_801B619C.prim;
    angle = ROT(-33.75);
    for (i = 0; i < 9;) {
        oldX0 = x0;
        oldY0 = y0;
        oldX1 = x1;
        oldY1 = y1;
        angle = (angle + *angles) / 2 + ROT(90);
        upper = 8;
        lower = 8;
        delta = angle - *angles;
        upper += rcos(delta) * 6 >> 12;
        lower -= rcos(delta) * 6 >> 12;
        if (!i) {
            lower = 12;
            angle = ROT(53.4375);
        }
        upper = upper * (g_CurrentEntity->ext.et_801B619C.unk84 / 2 + 0x800) >> 12;
        lower = lower * (g_CurrentEntity->ext.et_801B619C.unk84 / 2 + 0x800) >> 12;
        x0 = x1 = baseX;
        y0 = y1 = baseY;
        if (g_CurrentEntity->facingLeft) {
            angle = ROT(180) - angle;
        }
        x0 += (s16)(upper * rcos(angle) >> 12);
        y0 -= (s16)(upper * rsin(angle) >> 12);
        x1 -= (s16)(lower * rcos(angle) >> 12);
        y1 += (s16)(lower * rsin(angle) >> 12);
        length = 11;
        length = length * g_CurrentEntity->ext.et_801B619C.unk84 >> 12;
        if (!g_CurrentEntity->facingLeft) {
            baseX += (s16)(length * rcos(*angles) >> 12);
        } else {
            baseX -= (s16)(length * rcos(*angles) >> 12);
        }
        baseY -= (s16)(length * rsin(*angles) >> 12);
        angle = *angles;
        angles++;
        if (i) {
            prim->tpage = 0x14;
            prim->clut = 0x20B;
            prim->x0 = oldX0;
            prim->x2 = oldX1;
            prim->y0 = oldY0;
            prim->y2 = oldY1;
            prim->x1 = x0;
            prim->x3 = x1;
            prim->y1 = y0;
            prim->y3 = y1;
            prim = prim->next;
        }
        i++;
        angles++;
    }
}

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
