// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern s16* D_pspeu_092723C8[];

void func_pspeu_0925DBD0(Primitive* prim) {
    s16* data;
    s16 x;
    s16 y;
    s16 u;
    s16 v;
    s16 width;
    s16 height;

    data = D_pspeu_092723C8[g_CurrentEntity->animCurFrame];
    data += 2;
    u = g_CurrentEntity->unk5A & 1;
    u = u * 0x7F;
    v = (g_CurrentEntity->unk5A & 2) >> 1;
    v = v * 0x7F;
    x = g_CurrentEntity->posX.i.hi;
    if (g_CurrentEntity->facingLeft) {
        x -= *data++;
    } else {
        x += *data++;
    }
    y = g_CurrentEntity->posY.i.hi + *data++;
    width = *data++;
    height = *data++;
    if (g_CurrentEntity->facingLeft) {
        prim->x0 = x - width + 1;
        prim->y0 = y;
        prim->x1 = x + 1;
        prim->y1 = y;
        prim->x2 = x - width + 1;
        prim->y2 = y + height;
        prim->x3 = x + 1;
        prim->y3 = y + height;
    } else {
        prim->x0 = x;
        prim->y0 = y;
        prim->x1 = x + width;
        prim->y1 = y;
        prim->x2 = x;
        prim->y2 = y + height;
        prim->x3 = x + width;
        prim->y3 = y + height;
    }
    prim->clut = *data++;
    if (*data & 1) {
        u += 0x7F;
    }
    if (*data & 2) {
        v += 0x7F;
    }
    data++;
    if (g_CurrentEntity->facingLeft) {
        prim->u1 = prim->u3 = u + *data++;
        prim->v0 = prim->v1 = v + *data++;
        prim->u0 = prim->u2 = u + *data++;
        prim->v2 = prim->v3 = v + *data++;
    } else {
        prim->u0 = prim->u2 = u + *data++;
        prim->v0 = prim->v1 = v + *data++;
        prim->u1 = prim->u3 = u + *data++;
        prim->v2 = prim->v3 = v + *data++;
    }
}

void func_pspeu_0925E070(u8 step) {
    OlroxPrim* part;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    part = g_CurrentEntity->ext.et_801B365C.unk80;
    part->step = 0;
    part = part->unk18;
    part->step = 0;
}

#include "../../st/approach_s16.h"

s32 func_us_801B163C(s16* value, s16 target, s16 speed) {
    s16* velocity;
    s16 delta;
    s16 difference;
    s32 acceleration;

    velocity = value + 1;
    acceleration = speed / 16;
    if (!acceleration) {
        acceleration = 1;
    }
    difference = target - *value;
    delta = *velocity;
    if (difference > 0) {
        if (delta < speed) {
            delta += acceleration;
        } else {
            delta -= acceleration;
        }
    } else {
        if (delta > -speed) {
            delta -= acceleration;
        } else {
            delta += acceleration;
        }
    }
    if (abs(difference) < abs(delta)) {
        *value = target;
        *velocity = difference;
        return 1;
    }
    *value += delta;
    *velocity = delta;
    return 0;
}

s32 func_pspeu_0925E340(OlroxPrim* prim, s16 x, s16 y, s16 speed) {
    s16* value;
    s16* delta;
    s16 difference;
    s16 target;
    s32 i;
    s32 moved;

    moved = 0;
    value = &prim->unk0C;
    delta = &prim->unk0E;
    target = x;
    for (i = 0; i < 2; i++) {
        difference = target - *value;
        if (abs(difference) < speed) {
            *value = target;
            *delta = difference;
        } else {
            if (difference > 0) {
                *value += speed;
                *delta = speed;
            } else {
                *value -= speed;
                *delta = -speed;
            }
            moved |= 1;
        }
        value = &prim->unk10;
        delta = &prim->unk12;
        target = y;
    }
    return moved ^ 1;
}

void func_pspeu_0925E488(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(45), 0x18)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(-5.625), 0x20)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(-67.5), ROT(-5.625), 0x20)) {
            prim->step++;
        }
        break;
    }
}

void func_pspeu_0925E578(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-5.625), ROT(5.625), 0x1C)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 > -0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 -= 0xC;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-22.5), ROT(67.5), 0x18)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 < 0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 += 8;
        }
        break;
    }
}

void func_pspeu_0925E688(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-45), ROT(67.5), 0x18)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(0), ROT(56.25), 0x1C)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(11.25), ROT(33.75), 0x18)) {
            prim->step++;
        }
        break;
    }
}

void func_pspeu_0925E778(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-11.25), ROT(11.25), 0x1C)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 > -0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 -= 0x10;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-22.5), ROT(0), 0x18)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(-56.25), ROT(-16.875), 0x14)) {
            prim->step++;
        }
        if (g_CurrentEntity->ext.et_801B365C.unk90 < 0x100) {
            g_CurrentEntity->ext.et_801B365C.unk90 += 0x10;
        }
        break;
    }
}

u8 func_pspeu_0925E8C8(OlroxPrim* prim) {
    s32 result;
    OlroxPrim* other;

    other = prim->unk18;
    switch (prim->step) {
    case 0:
        result = func_pspeu_0925E340(prim, ROT(-33.75), ROT(0), 0x20);
        result += func_pspeu_0925E340(other, ROT(11.25), ROT(56.25), 0x20);
        if (result == 2) {
            prim->step++;
        }
        break;
    case 1:
        result = func_pspeu_0925E340(prim, ROT(-45), ROT(-22.5), 0x40);
        func_pspeu_0925E340(other, ROT(-22.5), ROT(56.25), 0x40);
        if (result) {
            prim->step++;
        }
        break;
    case 2:
        result = func_pspeu_0925E340(prim, ROT(-56.25), ROT(-45), 0x60);
        func_pspeu_0925E340(other, ROT(-33.75), ROT(67.5), 0x40);
        if (result) {
            return true;
        }
        break;
    }
    return false;
}

s32 func_pspeu_0925EA30(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, -0x100, 0xC);
    result = func_pspeu_0925E340(part, ROT(-39.375), ROT(56.25), 0x10);
    result += func_pspeu_0925E340(other, ROT(-78.75), ROT(28.125), 0x10);
    if (result == 2) {
        return true;
    }
    return false;
}

u8 func_pspeu_0925EAD8(OlroxPrim* prim) {
    s32 result;
    OlroxPrim* other;

    other = prim->unk18;
    result =
        func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0x180, 0x40);
    switch (prim->step) {
    case 0:
        result += func_pspeu_0925E340(prim, ROT(-8.4375), ROT(73.125), 0x18);
        if (func_pspeu_0925E340(other, ROT(-112.5), ROT(11.25), 0x20)) {
            prim->step++;
        }
        break;
    case 1:
        result += func_pspeu_0925E340(prim, ROT(-5.625), ROT(73.125), 0x18);
        result += func_pspeu_0925E340(other, ROT(-84.375), ROT(19.6875), 0x20);
        if (result == 3) {
            return true;
        }
        break;
    }
    return false;
}

s32 func_pspeu_0925EC00(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0, 0xC);
    result = func_pspeu_0925E340(part, ROT(-61.875), ROT(61.875), 0x28);
    result += func_pspeu_0925E340(other, ROT(-22.5), ROT(84.375), 0x38);
    if (result == 2) {
        return true;
    }
    return false;
}

s32 func_pspeu_0925ECA8(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0, 0xC);
    result = func_pspeu_0925E340(part, ROT(-5.625), ROT(5.625), 0x60);
    result += func_pspeu_0925E340(other, ROT(-45), ROT(56.25), 0x50);
    if (result == 2) {
        return true;
    }
    return false;
}

s32 func_pspeu_0925ED50(OlroxPrim* part) {
    s32 result;
    OlroxPrim* other;

    other = part->unk18;
    func_us_801B163C(&g_CurrentEntity->ext.et_801B365C.unk90, 0, 0x10);
    result = func_pspeu_0925E340(part, ROT(-45), ROT(45), 0x60);
    result += func_pspeu_0925E340(other, ROT(-45), ROT(33.75), 0x40);
    if (result == 2) {
        return true;
    }
    return false;
}

u8 func_pspeu_0925EDF8(OlroxPrim* prim) {
    s32 result;
    OlroxPrim* other;

    other = prim->unk18;
    switch (prim->step) {
    case 0:
        result = func_pspeu_0925E340(prim, ROT(-11.25), ROT(11.25), 0x20);
        result += func_pspeu_0925E340(other, ROT(0), ROT(33.75), 0xC);
        if (result == 2) {
            prim->step++;
        }
        break;
    case 1:
        result = func_pspeu_0925E340(prim, ROT(-67.5), ROT(0), 0x28);
        result += func_pspeu_0925E340(other, ROT(11.25), ROT(22.5), 0xC);
        if (result == 2) {
            return true;
        }
        break;
    }
    return false;
}

void func_pspeu_0925EF08(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(45), 8)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-90), ROT(-5.625), 8)) {
            prim->step++;
        }
        break;
    case 2:
        if (func_pspeu_0925E340(prim, ROT(-67.5), ROT(-5.625), 0xC)) {
            prim->step++;
        }
        break;
    }
}

void func_pspeu_0925EFF8(OlroxPrim* prim) {
    switch (prim->step) {
    case 0:
        if (func_pspeu_0925E340(prim, ROT(-5.625), ROT(5.625), 8)) {
            prim->step++;
        }
        break;
    case 1:
        if (func_pspeu_0925E340(prim, ROT(-22.5), ROT(78.75), 0x14)) {
            prim->step++;
        }
        break;
    }
}
