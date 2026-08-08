// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rcen.h"

extern EInit D_us_801805A0;
u8 GetPlayerCollisionWith(Entity* self, u16 w, u16 h, u16 flags);

// Unused on PSP, see UnusedPrimFunction in CEN
static s16 func_us_8019FD4C(Primitive* prim, s16 dx) {
    prim->drawMode = DRAW_UNK02;
    prim->v0 = prim->v1 = 0x50;
    prim->v2 = prim->v3 = 0x60;
    prim->y0 = prim->y1 = 0xBC;
    prim->y2 = prim->y3 = 0xCC;
    prim->u0 = prim->u2 = 2;
    prim->x0 = prim->x2 = dx;
    dx -= 0x20;

    if (dx < 0x50) {
        dx = 0x50 - dx;
        prim->u1 = prim->u3 = 0x22 - dx;
        prim->x1 = prim->x3 = 0x50;
        dx = 0;
    } else {
        prim->u1 = prim->u3 = 0x22;
        prim->x1 = prim->x3 = dx;
    }
    return dx;
}

static s16 func_801904B8(Primitive* prim, s16 dy) {
    prim->drawMode = DRAW_UNK02;
    prim->u0 = prim->u2 = 0x50;
    prim->u1 = prim->u3 = 0x60;
    prim->x0 = prim->x2 = g_CurrentEntity->posX.i.hi - 8;
    prim->x1 = prim->x3 = g_CurrentEntity->posX.i.hi + 8;
    prim->v2 = prim->v3 = 0x26;
    prim->y2 = prim->y3 = dy;
    dy += 0x20;
    prim->v0 = prim->v1 = 6;
    prim->y0 = prim->y1 = dy;
    if (dy >= 0x101) {
        dy = 0;
    }
    return dy;
}

INCLUDE_ASM("st/rcen/nonmatchings/e_elevator", func_us_8019FE9C);

void EntityUnkId1B(Entity* self) {
    Entity* entity = self + self->params;
    u8 isTouchingPlayer;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801805A0);
        if (self->params & 0x10) {
            self->animCurFrame = self->params & 15;
            self->zPriority = 0x6A;
            self->step = 2;
            return;
        }
        self->animCurFrame = 0;
        break;

    case 1:
        self->posX.i.hi = entity->posX.i.hi;
        if (self->params == 1) {
            self->posY.i.hi = entity->posY.i.hi + 27;
            isTouchingPlayer = GetPlayerCollisionWith(self, 12, 8, 4);
        } else {
            self->posY.i.hi = entity->posY.i.hi - 32;
            isTouchingPlayer = GetPlayerCollisionWith(self, 12, 8, 6);
        }
        self->ext.cenElevator.playerCollision = isTouchingPlayer;
        break;
    }
}
