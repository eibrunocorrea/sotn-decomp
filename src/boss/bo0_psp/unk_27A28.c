// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"
#include <scratchpad.h>

extern s16 D_pspeu_0928FC88[];
extern s32 D_us_80181190;
extern s32 E_ID(UNK_3E);
extern s32 E_ID(LIFE_UP_SPAWN);
Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);

extern EInit D_pspeu_09275F90;
extern s32 D_pspeu_092907A8;
extern s32 D_us_801CE5B0;
extern s16 D_pspeu_0928FC40[];
extern s16 D_pspeu_0928FC50[];
extern s16 D_pspeu_0928FC60[];
extern s16 D_pspeu_0928FC70[];
extern s16* D_pspeu_0928FC80[];

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
        upper =
            upper * (g_CurrentEntity->ext.et_801B619C.unk84 / 2 + 0x800) >> 12;
        lower =
            lower * (g_CurrentEntity->ext.et_801B619C.unk84 / 2 + 0x800) >> 12;
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

void func_pspeu_092657B8(Entity* self) {
    Primitive* prim;
    s32 i;
    s16* angles;
    Entity* parent;
    s32 primIndex;
    u8 result;

    parent = self - 4;
    self->facingLeft = parent->facingLeft;
    self->palette = parent->palette;
    if (!D_us_801CE5B0) {
        self->posX.val = parent->posX.val;
        self->posY.val = parent->posY.val;
        if (!self->facingLeft) {
            self->posX.i.hi += 4;
        } else {
            self->posX.i.hi -= 4;
        }
    }
    if (self->flags & FLAG_DEAD) {
        self->hitboxState = 0;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F90);
        self->hitboxState = 0;
        self->animCurFrame = 0;
        self->flags |= FLAG_POS_CAMERA_LOCKED;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (primIndex == -1) {
            self->step = 0;
            break;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801B619C.prim = prim;
        for (i = 0; i < 8; i++) {
            prim->tpage = 0x14;
            prim->clut = 0x20B;
            prim->u0 = prim->u2 = i * 11 + 0x80;
            prim->u1 = prim->u3 = prim->u0 + 11;
            prim->v0 = prim->v1 = 0x50;
            prim->v2 = prim->v3 = 0x60;
            prim->priority = 0xA1;
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        }
        angles = &self->ext.et_801B619C.unk88;
        for (i = 0; i < 8;) {
            *angles = ROT(-33.75);
            angles++;
            i++;
            angles++;
        }
        self->ext.et_801B619C.unk84 = 0;
        // fallthrough
    case 1:
        if (!D_us_801CE5B0) {
            self->step++;
        }
        break;
    case 2:
        if (self->ext.et_801B619C.unk84 < 0x1000) {
            self->ext.et_801B619C.unk84 += 0x200;
        } else {
            self->ext.et_801B619C.unk84 = 0x1000;
            self->step++;
        }
        break;
    case 3:
        result = func_pspeu_092652C0(
            &self->ext.et_801B619C.unk88,
            D_pspeu_0928FC80[LOBU(self->ext.et_801B619C.unk80)], 0x20);
        if (result) {
            LOBU(self->ext.et_801B619C.unk80) ^= 1;
        }
        break;
    case 4:
        func_pspeu_092652C0(
            &self->ext.et_801B619C.unk88, D_pspeu_0928FC40, 0x20);
        break;
    case 6:
        func_pspeu_092652C0(
            &self->ext.et_801B619C.unk88, D_pspeu_0928FC50, 0x60);
        break;
    case 7:
        func_pspeu_092652C0(
            &self->ext.et_801B619C.unk88, D_pspeu_0928FC60, 0x80);
        break;
    case 8:
        func_pspeu_092652C0(
            &self->ext.et_801B619C.unk88, D_pspeu_0928FC70, 0x60);
        break;
    case 9:
        result = func_pspeu_09265170(
            &self->ext.et_801B619C.unk88,
            D_pspeu_0928FC80[LOBU(self->ext.et_801B619C.unk80)], 0x40);
        if (result) {
            LOBU(self->ext.et_801B619C.unk80) ^= 1;
        }
        break;
    }
    func_pspeu_09265368();
    if (self->palette & PAL_FLAG(0)) {
        prim = self->ext.et_801B619C.prim;
        while (prim) {
            prim->clut = self->palette & 0xFFF;
            prim = prim->next;
        }
    }
    parent = self - 4;
    if (parent->ext.et_801B365C.unk9B) {
        prim = self->ext.et_801B619C.prim;
        while (prim) {
            prim->tpage = 0x15;
            prim->clut = PAL_BREAKABLE;
            prim = prim->next;
        }
    }
}

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

Primitive* func_pspeu_09265D78(f32 pos, s16* angles, Primitive* prim, u8 side) {
    s16 baseX;
    s16 baseY;
    s16 angle;
    s16 length;
    s16 tipX0;
    s16 midX0;
    s16 midX2;
    s16 midY0;
    u8* value;
    Entity* child;
    s16 midY2;
    s16 rootX1;
    s16 tipX2;
    s16 rootX3;
    s16 tipY0;
    s16 rootY1;
    s16 tipY2;
    s16 rootY3;
    u16 clut;

    if (side) {
        clut = 0x20B;
        value = &g_CurrentEntity->ext.et_801B6CA4.unk93;
    } else {
        clut = 0x218;
        value = &g_CurrentEntity->ext.et_801B6CA4.unk92;
    }
    side *= 6;
    baseX = pos.i.lo;
    baseY = pos.i.hi;
    angle = *angles;
    if (g_CurrentEntity->facingLeft) {
        angle = ROT(180) - angle;
    }
    rootX1 = rootX3 = baseX;
    rootY1 = rootY3 = baseY;
    rootX1 += (-(rcos(angle) * 8) >> 12);
    rootX3 += (rcos(angle) * 8 >> 12);
    rootY1 += (rsin(angle) * 8 >> 12);
    rootY3 += (-(rsin(angle) * 8) >> 12);
    length = (*value + 0x100) * 32 / 0x100;
    if (!g_CurrentEntity->facingLeft) {
        baseX += (length * rcos(*angles - ROT(90)) >> 12);
    } else {
        baseX -= (length * rcos(*angles - ROT(90)) >> 12);
    }
    baseY += (-length * rsin(*angles - ROT(90)) >> 12);
    midX0 = midX2 = baseX;
    midY0 = midY2 = baseY;
    angle = *angles;
    angles += 2;
    if (g_CurrentEntity->facingLeft) {
        angle = ROT(180) - angle;
    }
    midX0 += (-(rcos(angle) * 6) >> 12);
    midY0 += (rsin(angle) * 6 >> 12);
    midX2 += (rcos(angle) * 6 >> 12);
    midY2 += (-(rsin(angle) * 6) >> 12);
    prim->tpage = 0x14;
    prim->clut = clut;
    prim->u0 = 0xC8;
    prim->u1 = 0xC8;
    prim->u2 = 0xD4;
    prim->u3 = 0xDA;
    prim->v0 = 0x1E;
    prim->v1 = 4;
    prim->v2 = 0x20;
    prim->v3 = 4;
    prim->x0 = midX0;
    prim->x1 = rootX1;
    prim->x2 = midX2;
    prim->x3 = rootX3;
    prim->y0 = midY0;
    prim->y1 = rootY1;
    prim->y2 = midY2;
    prim->y3 = rootY3;
    prim->priority = D_pspeu_092907A8 - 3 + side;
    prim->drawMode = DRAW_UNK02;
    prim = prim->next;
    if (!g_CurrentEntity->facingLeft) {
        baseX += (rcos(*angles + ROT(90)) * 6 >> 12);
    } else {
        baseX -= (rcos(*angles + ROT(90)) * 6 >> 12);
    }
    baseY += (-(rsin(*angles + ROT(90)) * 6) >> 12);
    midX0 = midX2 = baseX;
    midY0 = midY2 = baseY;
    angle = *angles;
    if (g_CurrentEntity->facingLeft) {
        angle = ROT(180) - angle;
    }
    midX0 += (-(rcos(angle) * 6) >> 12);
    midY0 += (rsin(angle) * 6 >> 12);
    midX2 += (rcos(angle) * 6 >> 12);
    midY2 += (-(rsin(angle) * 6) >> 12);
    length = (*value + 0x100) * 24 / 0x100;
    if (!g_CurrentEntity->facingLeft) {
        baseX += (length * rcos(*angles - ROT(90)) >> 12);
    } else {
        baseX -= (length * rcos(*angles - ROT(90)) >> 12);
    }
    baseY += (-length * rsin(*angles - ROT(90)) >> 12);
    angle = *angles;
    if (g_CurrentEntity->facingLeft) {
        angle = ROT(180) - angle;
    }
    tipX0 = tipX2 = baseX;
    tipY0 = tipY2 = baseY;
    tipX0 += (-(rcos(angle) * 6) >> 12);
    tipY0 += (rsin(angle) * 6 >> 12);
    tipX2 += (rcos(angle) * 6 >> 12);
    tipY2 += (-(rsin(angle) * 6) >> 12);
    if (side) {
        child = g_CurrentEntity + 2;
        value = &g_CurrentEntity->ext.et_801B6CA4.unk91;
    } else {
        child = g_CurrentEntity + 1;
        value = &g_CurrentEntity->ext.et_801B6CA4.unk90;
    }
    child->posX.i.hi = baseX;
    child->posY.i.hi = baseY;
    child->animCurFrame = *value;
    angle = *angles;
    if (g_CurrentEntity->ext.et_801B6CA4.unk94) {
        child->rotate = -(angle + ROT(67.5));
    } else {
        child->rotate = 0;
    }
    prim->tpage = 0x14;
    prim->clut = clut;
    prim->u0 = 0x88;
    prim->u1 = 0x9C;
    prim->u2 = 0x8C;
    prim->u3 = 0xA2;
    prim->v0 = 0x74;
    prim->v1 = 0x6D;
    prim->v2 = 0x80;
    prim->v3 = 0x77;
    prim->x0 = tipX0;
    prim->x1 = midX0;
    prim->x2 = tipX2;
    prim->x3 = midX2;
    prim->y0 = tipY0;
    prim->y1 = midY0;
    prim->y2 = tipY2;
    prim->y3 = midY2;
    prim->priority = D_pspeu_092907A8 - 3 + side;
    prim->drawMode = DRAW_UNK02;
    prim = prim->next;
    return prim;
}

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

void func_pspeu_09267FC0(Entity* self) {
    Primitive* prim;
    Primitive* src;
    Primitive* next;
    s32 i;
    Entity* entity;
    s32 count;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F90);
        self->zPriority = D_pspeu_092907A8 + 8;
        self->animCurFrame = 0;
        self->flags |= FLAG_POS_CAMERA_LOCKED;
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x108);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801B7CC8.prim = prim;
        entity = self;
        entity -= 10;
        for (i = 0; i < 10; i++, entity++) {
            src = entity->ext.prim;
            while (src) {
                next = prim->next;
                *prim = *src;
                prim->next = next;
                prim->clut = 0x20B;
                prim->priority += 8;
                PRED(prim) = 0x80;
                src = src->next;
                prim = prim->next;
            }
        }
        self->ext.et_801B7CC8.unk9C = prim;
        while (prim) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        break;
    case 1:
        count = 0;
        src = self->ext.et_801B7CC8.prim;
        while (src != self->ext.et_801B7CC8.unk9C) {
            if (!(src->drawMode & DRAW_HIDE)) {
                count++;
                if (!(Random() & 0xF)) {
                    prim = self->ext.et_801B7CC8.unk9C;
                    prim = func_pspeu_09267EF8(prim, 4);
                    if (prim) {
                        src->r0 = 0xB0;
                        src->r1 = 0x98;
                        src->r2 = 0xA0;
                        src->r3 = 0x88;
                        UnkRecursivePrimFunc2(src, 1, prim, SP(0));
                        src->drawMode = DRAW_HIDE;
                    }
                }
            }
            src = src->next;
        }
        src = self->ext.et_801B7CC8.unk9C;
        while (src) {
            if (!(src->drawMode & DRAW_HIDE)) {
                count++;
                if (src->r0 > 0x80) {
                    src->r0--;
                    if (src->r0 == 0x80) {
                        prim = self->ext.et_801B7CC8.unk9C;
                        prim = func_pspeu_09267EF8(prim, 4);
                        if (prim) {
                            src->r0 = 0x20;
                            src->r1 = 0x14;
                            src->r2 = 0x18;
                            src->r3 = 0x10;
                            UnkRecursivePrimFunc2(src, 1, prim, SP(0));
                            src->drawMode = DRAW_HIDE;
                            src->p3 = 0;
                        } else {
                            src->r0 += (Random() & 0x1F) + 2;
                        }
                    }
                } else {
                    if (!--src->r0) {
                        src->p3 = 0;
                        src->drawMode = DRAW_HIDE;
                    } else if (src->r0 < 0x10) {
                        src->y0 += (8 - src->r0 / 2);
                        src->y1 += (8 - src->r0 / 2);
                        src->y2 += (8 - src->r0 / 2);
                        src->y3 += (8 - src->r0 / 2);
                    }
                }
            }
            src = src->next;
        }
        if (!count) {
            entity = self - 10;
            entity->ext.et_801B365C.unk9B = 2;
            self->step++;
        }
        break;
    case 2:
        entity = self - 10;
        if (entity->ext.et_801B365C.unk9B == 3) {
            prim = self->ext.et_801B7CC8.prim;
            self->ext.et_801B7CC8.unk80 = prim;
            prim->type = PRIM_G4;
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0xF0;
            PGREY(prim, 0) = 0;
            LOW(prim->r1) = LOW(prim->r0);
            prim->r2 = 0xF0;
            prim->g2 = 0xF0;
            prim->b2 = 0xF0;
            LOW(prim->r3) = LOW(prim->r2);
            prim->priority = D_pspeu_092907A8 + 0x10;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            prim = prim->next;
            self->ext.et_801B7CC8.prim = prim;
            for (i = 0; i < 6; i++, entity++) {
                entity = self - 10;
                entity += D_pspeu_0928FC88[i];
                src = entity->ext.prim;
                while (src) {
                    next = prim->next;
                    *prim = *src;
                    prim->next = next;
                    prim->clut = PAL_BREAKABLE;
                    src = src->next;
                    prim = prim->next;
                }
            }
            self->ext.et_801B7CC8.unk9C = prim;
            while (prim) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
            self->ext.et_801B7CC8.unk84 = 0x28;
            self->step++;
        }
        break;
    case 3:
        prim = self->ext.et_801B7CC8.unk80;
        prim->x2 = prim->x0 -= 6;
        prim->x3 = prim->x1 += 6;
        prim->y0 = prim->y1 = 0;
        prim->y2 = prim->y3 = 0xF0;
        if ((prim->x0 & 0xF000) && (prim->x3 & 0xF00)) {
            prim->x0 = prim->x2 = 0;
            prim->x1 = prim->x3 = 0x100;
            prim->drawMode &= ~DRAW_UNK02;
            self->ext.et_801B7CC8.unk94 = 1;
            self->step++;
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity) {
                CreateEntityFromEntity(E_ID(UNK_3E), self, entity);
                entity->facingLeft = self->facingLeft;
                self->ext.et_801B7CC8.unkA4 = entity;
            }
        }
        break;
    case 4:
        if (!--self->ext.et_801B7CC8.unk94) {
            g_api.PlaySfx(SFX_METAL_CLANG_A);
            self->ext.et_801B7CC8.unk94 = 4;
            src = self->ext.et_801B7CC8.prim;
            for (i = 0; i < self->ext.et_801B7CC8.unk84; i++) {
                src = src->next;
            }
            prim = self->ext.et_801B7CC8.unk9C;
            prim = FindFirstUnkPrim2(prim, 4);
            if (prim) {
                src->p3 = 1;
                src->r0 = 0x24;
                src->r1 = 0x28;
                src->r2 = 0x2C;
                src->r3 = 0x21;
                UnkRecursivePrimFunc2(src, 1, prim, SP(0));
                src->drawMode |= DRAW_HIDE;
                self->ext.et_801B7CC8.unk84 -= 1;
                if (self->ext.et_801B7CC8.unk84 < 0) {
                    g_api.PlaySfx(SFX_TELEPORT_BANG_B);
                    self->step++;
                }
            }
        }
    case 5:
        prim = self->ext.et_801B7CC8.unk80;
        if (g_Timer % 2) {
            prim->drawMode =
                DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        } else {
            prim->drawMode = DRAW_HIDE;
        }
        count = self->ext.et_801B7CC8.unk84;
        src = self->ext.et_801B7CC8.unk9C;
        self->ext.et_801B7CC8.unk88++;
        while (src) {
            if (!(src->drawMode & DRAW_HIDE)) {
                count++;
                src->r0--;
                switch ((src->r0 & 0xF0) >> 4) {
                case 3:
                case 2:
                    if (self->ext.et_801B7CC8.unk88 & 1) {
                        src->x0++;
                        src->x1++;
                        src->x2++;
                        src->x3++;
                    } else {
                        src->x0--;
                        src->x1--;
                        src->x2--;
                        src->x3--;
                    }
                    break;
                case 1:
                    if (src->r0 == 0x10) {
                        prim = self->ext.et_801B7CC8.unk9C;
                        prim = FindFirstUnkPrim2(prim, 4);
                        if (prim) {
                            src->p3 = 3;
                            src->r0 = 0xD;
                            src->r1 = 6;
                            src->r2 = 0xE;
                            src->r3 = 0x14;
                            UnkRecursivePrimFunc2(src, 1, prim, SP(0));
                            src->drawMode = DRAW_HIDE;
                            src->p3 = 0;
                        } else {
                            src->r0 += Random() & 0xF;
                        }
                    }
                    src->y0--;
                    src->y1--;
                    src->y2--;
                    src->y3--;
                    break;
                case 0:
                    src->y0 -= (8 - src->r0 / 2);
                    src->y1 -= (8 - src->r0 / 2);
                    src->y2 -= (8 - src->r0 / 2);
                    src->y3 -= (8 - src->r0 / 2);
                    if (!src->r0) {
                        src->drawMode = DRAW_HIDE;
                        src->p3 = 0;
                    }
                    break;
                }
            }
            src = src->next;
        }
        if (count < 0) {
            entity = self - 10;
            entity->ext.et_801B365C.unk9B = 2;
            entity = self->ext.et_801B7CC8.unkA4;
            entity->step++;
            self->step++;
        }
        break;
    case 6:
        prim = self->ext.et_801B7CC8.unk80;
        prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        prim->y0 = prim->y1 = 0;
        prim->y2 = prim->y3 = 0xF0;
        prim->x2 = prim->x0 += 2;
        prim->x3 = prim->x1 -= 2;
        if (prim->x0 > prim->x1) {
            prim->drawMode = DRAW_HIDE;
            self->ext.et_801B7CC8.unk94 = 1;
            self->step++;
        }
        break;
    case 7:
        entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (entity) {
            CreateEntityFromEntity(E_ID(LIFE_UP_SPAWN), self, entity);
            entity->posY.i.hi -= 0x28;
            entity->params = 0;
            D_us_80181190 = 1;
            stopMusicFlag = 1;
            currentMusicId = MU_DANCE_OF_PALES;
            self->step++;
        }
        break;
    case 8:
        if (!g_api.func_80131F68()) {
            stopMusicFlag = 0;
            g_api.PlaySfx(currentMusicId);
            DestroyEntity(self);
        }
        break;
    }
}
