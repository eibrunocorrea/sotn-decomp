// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

extern s16 D_pspeu_09264038[];
extern s16 D_pspeu_09264048[];
extern s16 D_pspeu_09264058[];
extern EInit D_pspeu_09266738;
extern u8 D_pspeu_092640A8[];
extern u8 D_pspeu_092640B0[];
extern u8 D_pspeu_092640B8[];
extern u8 D_pspeu_092640C0[];
// local copy of func_us_801BDA0C (see no1/unk_3D8AC.c), with
// the rbo5 timer trigger from EntityUnkId17 (see rbo5/doors.c).
void func_us_801BDA0C(Entity* self) {
    s32 flag;
    s32 primIndex;
    s16 xOffset, yOffset;
    s16 magnitude;
    Entity* tempEntity;
    Primitive* prim;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09266738);
        self->drawFlags |=
            ENTITY_OPACITY | ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
        if ((self->params & 0xF) > 1) {
            self->opacity = 0x20;
        } else {
            self->opacity = 0x80;
        }
        self->scaleX = D_pspeu_09264058[self->params & 0xF];
        self->scaleY = self->scaleX;
        if ((self->params & 0xF) % 2) {
            self->rotate = ROT(-90);
        } else {
            self->rotate = ROT(0);
        }
        self->zPriority = (0x40 - self->params) & 0xF;
        if (self->params & 0x100) {
            self->animCurFrame = 100;
        } else {
            self->animCurFrame = 98;
        }
        primIndex = g_api.AllocPrimitives(PRIM_TILE, 1);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.et_801BDA0C.unk7C = prim;
            prim->x0 = self->posX.i.hi;
            prim->y0 = self->posY.i.hi;
            prim->priority = self->zPriority;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        } else {
            DestroyEntity(self);
            return;
        }
        self->ext.et_801BDA0C.unk84 = 0;
        self->ext.et_801BDA0C.unk80 = 0;
        break;
    case 1:
        self->palette = PAL_FLAG(0x4F);
        if (self->ext.et_801BDA0C.unk84) {
            self->ext.et_801BDA0C.unk9C = 64;
            self->step++;
        }
        break;

    case 2:
        flag = self->params & 0xFF;
        if (flag != 1) {
            self->step++;
        } else {
            if (self->params & 0x100) {
                AnimateEntity(D_pspeu_092640B0, self);
            } else {
                AnimateEntity(D_pspeu_092640A8, self);
            }
            if ((g_Timer & 0xF) == 0) {
                g_api.PlaySfx(SFX_STONE_MOVE_B);
            }
            if (--self->ext.et_801BDA0C.unk9C == 0) {
                self->step++;
            }
        }
        break;

    case 3:
        if (g_Timer % 2 == 0) {
            self->palette++;
            if (self->palette > PAL_FLAG(0x58)) {
                g_api.PlaySfx(SFX_ELECTRICITY);
                self->palette = PAL_FLAG(0x4F);
                self->step++;
                if (self->params == 5) {
                    (self + 1)->ext.et_801BDA0C.unk84 = 1;
                }
            }
        }
        break;

    case 4:
        magnitude = self->ext.et_801BDA0C.unk80 / FIX(1.0);
        xOffset = FLT_TO_I(rcos(self->rotate - ROT(90)) * magnitude);
        yOffset = FLT_TO_I(rsin(self->rotate - ROT(90)) * magnitude);
        prim = self->ext.et_801BDA0C.unk7C;
        self->posX.i.hi = prim->x0 + xOffset;
        self->posY.i.hi = prim->y0 + yOffset;
        if (self->params & 0x100) {
            AnimateEntity(D_pspeu_092640B0, self);
        } else {
            AnimateEntity(D_pspeu_092640A8, self);
        }
        if ((g_Timer & 0xF) == 0) {
            g_api.PlaySfx(SFX_STONE_MOVE_B);
        }
        if (self->params & 0x100) {
            self->ext.et_801BDA0C.unk88 -= D_pspeu_09264038[self->params & 0xF];
        } else {
            self->ext.et_801BDA0C.unk88 += D_pspeu_09264038[self->params & 0xF];
        }
        self->ext.et_801BDA0C.unk80 += self->ext.et_801BDA0C.unk88;
        if (abs(self->ext.et_801BDA0C.unk80) > FIX(12)) {
            (self + 2)->ext.et_801BDA0C.unk84 = 1;
            self->step++;
            if (self->params == 5) {
                (self + 1)->ext.et_801BDA0C.unk84 = 1;
            }
        }
        tempEntity = self + 2;
        if (tempEntity->opacity < 0x80) {
            tempEntity->opacity++;
        }
        break;

    case 5:
        tempEntity = self + 2;
        if (tempEntity->opacity < 0x80) {
            tempEntity->opacity += 2;
        }
        magnitude = self->ext.et_801BDA0C.unk80 / FIX(1.0);
        xOffset = FLT_TO_I(rcos(self->rotate - ROT(90)) * magnitude);
        yOffset = FLT_TO_I(rsin(self->rotate - ROT(90)) * magnitude);
        prim = self->ext.et_801BDA0C.unk7C;
        self->posX.i.hi = prim->x0 + xOffset;
        self->posY.i.hi = prim->y0 + yOffset;
        if (self->params & 0x100) {
            AnimateEntity(D_pspeu_092640C0, self);
        } else {
            AnimateEntity(D_pspeu_092640B8, self);
        }
        self->rotate += D_pspeu_09264048[self->params & 0xF];
        if (self->params & 0x100) {
            self->ext.et_801BDA0C.unk88 -= D_pspeu_09264038[self->params & 0xF];
        } else {
            self->ext.et_801BDA0C.unk88 += D_pspeu_09264038[self->params & 0xF];
        }
        self->ext.et_801BDA0C.unk80 += self->ext.et_801BDA0C.unk88;
        if (self->ext.et_801BDA0C.unk80 > FIX(64)) {
            DestroyEntity(self);
        }
        break;
    }
}

// local copy of EntityUnkId1C (see rbo5/doors.c)
void func_pspeu_0925D968(Entity* self) {
    s32 offsetX;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09266738);
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate = ROT(-90);
        if (self->params & 1) {
            self->animCurFrame = 0x64;
            self->velocityX = FIX(-0.5);
        } else {
            self->animCurFrame = 0x62;
            self->velocityX = FIX(0.5);
        }
        self->zPriority = 0x5E;
        // fallthrough

    case 1:
        MoveEntity();
        if (self->params & 1) {
            AnimateEntity(D_pspeu_092640B0, self);
        } else {
            AnimateEntity(D_pspeu_092640A8, self);
        }
        offsetX = self->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (self->velocityX > 0) {
            if (offsetX > DISP_STAGE_W) {
                self->step++;
            }
        } else if (offsetX < DISP_STAGE_W) {
            self->step++;
        }
        break;

    case 2:
        self->posX.i.hi = DISP_STAGE_W - g_Tilemap.scrollX.i.hi;
        if (self->params & 1) {
            self->animCurFrame = 0x64;
        } else {
            self->animCurFrame = 0x62;
        }
        break;
    }
}

extern EInit g_EInitInteractable;
extern s32 E_ID(UNK_16);
extern s32 D_pspeu_0926CCF0;
extern s32 D_pspeu_0926CCE8;
extern s32 D_pspeu_0926CCE0;
extern s32 D_pspeu_092640C8[];
// local copy of EntityUnkId19 (see rbo5/doors.c)
void func_pspeu_0925DB68(Entity* self) {
    Primitive* prim;
    Primitive* lastPrim;
    s32 primIndex;
    s32 i;
    s32 angle;
    s32 moveX;
    s32 moveY;

    s16 xOffset;
    s16 posX;
    s16 posY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        break;

    case 1:
        if (self->ext.et_801BE2C8.unk84) {
            self->step++;
            self->ext.et_801BE2C8.unk84 = 0;
        }
        break;

    case 2:
        primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 0x30);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.et_801BE2C8.unk7C = prim;
            while (prim != NULL) {
                prim->r0 = prim->g0 = prim->b0 = 8;
                prim->priority = 0xD0;
                prim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                lastPrim = prim;
                prim = prim->next;
            }
            self->ext.et_801BE2C8.unk80 = lastPrim;
        } else {
            DestroyEntity(self);
            return;
        }
        self->step++;
        break;

    case 3:

        xOffset = (self - 1)->ext.et_801BDA0C.unk80 / FIX(1.0);
        if (xOffset < 0) {
            xOffset = -xOffset;
        }

        posX = self->posX.i.hi;
        posY = self->posY.i.hi - 0x28;
        prim = self->ext.et_801BE2C8.unk7C;
        for (i = 0; i < 32; i++) {
            prim->x0 = posX - xOffset - 0x20 + i;
            prim->y0 = posY - 0x20 + i;
            prim->u0 = xOffset * 2 + 0x40 - i * 2;
            prim->v0 = 0x94 - i * 2;
            prim = prim->next;
        }

        while (prim != NULL) {
            prim->x0 = posX - xOffset;
            prim->y0 = posY;
            prim->u0 = xOffset * 2;
            prim->v0 = 0x54;
            prim = prim->next;
        }

        lastPrim = self->ext.et_801BE2C8.unk80;
        if (self->ext.et_801BE2C8.unk84) {
            lastPrim->x0 = 0;
            lastPrim->y0 = 0;
            lastPrim->u0 = 0xFF;
            lastPrim->v0 = 0xFF;
            if (lastPrim->r0 < 248) {
                lastPrim->r0 += 8;
                lastPrim->g0 = lastPrim->b0 = lastPrim->r0;
            } else {
                (self + 1)->ext.et_801BE2C8.unk84 = 1;
                self->step++;
            }
        }
        break;

    case 4:
        prim = self->ext.et_801BE2C8.unk7C;
        prim->type = PRIM_TILE;
        prim->r0 = prim->b0 = prim->g0 = 0xFF;
        prim->priority = 0x40;
        prim = prim->next;

        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }

        prim = self->ext.et_801BE2C8.unk80;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        CreateEntityFromEntity(
            E_ID(UNK_16), self, &g_Entities[STAGE_ENTITY_START]);
        // BUG! Sets Doppleganger 10 defeat flag instead of Doppleganger 40
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_DOPPLEGANGER_10_DEFEAT, TIMEATTACK_SET_VISITED);
        (self + 1)->ext.et_801BE2C8.unk84 = 1;
        self->step++;
        break;

    case 5:
        lastPrim = self->ext.et_801BE2C8.unk80;

        if (lastPrim->r0 > 8) {
            lastPrim->r0 -= 8;
            lastPrim->g0 = lastPrim->b0 = lastPrim->r0;
            break;
        }
        g_api.FreePrimitives(self->primIndex);
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x10);
        if (primIndex == -1) {
            return;
        }

        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801BE2C8.unk7C = prim;
        moveY = self->posY.i.hi;
        moveX = self->posX.i.hi;

        for (i = 0; i < 16; i++) {
            angle = i << 8;
            prim->x0 = moveX + (((rcos(angle) >> 4) * 64) >> 8);
            prim->y0 = moveY - (((rsin(angle) >> 4) * 64) >> 8);
            prim->u0 = 32 + (((rcos(angle) >> 4) * 0x1E) >> 8);
            prim->v0 = 0xE0 - (((rsin(angle) >> 4) * 0x1E) >> 8);

            angle = (i + 1) << 8;
            prim->x1 = moveX + (((rcos(angle) >> 4) * 64) >> 8);
            prim->y1 = moveY - (((rsin(angle) >> 4) * 64) >> 8);
            prim->u1 = 32 + (((rcos(angle) >> 4) * 0x1E) >> 8);
            prim->v1 = 0xE0 - (((rsin(angle) >> 4) * 0x1E) >> 8);

            prim->y2 = prim->y3 = moveY;
            prim->x2 = prim->x3 = moveX;
            prim->u2 = prim->u3 = 0x20;
            prim->v2 = prim->v3 = 0xE0;

            prim->tpage = 0x1A;
            prim->clut = PAL_FILL_WHITE;
            prim->priority = 0x40;
            prim->drawMode = DRAW_DITHERING | DRAW_COLORS | DRAW_UNK02;
            prim = prim->next;
        }
        D_pspeu_0926CCF0 = 0x100;
        self->step++;
    case 6:
        // same as the warp room background
        D_pspeu_0926CCE8 += 16;
        D_pspeu_0926CCE0 = (rsin(D_pspeu_0926CCE8) >> 8) + 0xD0;
        if (D_pspeu_0926CCE0 < 0) {
            D_pspeu_0926CCE0 = 0;
        }
        if (D_pspeu_0926CCE0 > 0xFF) {
            D_pspeu_0926CCE0 = 0xFF;
        }

        D_pspeu_0926CCF0 = (rcos(D_pspeu_0926CCE8) >> 8) + 0xD0;

        prim = self->ext.et_801BE2C8.unk7C;

        for (i = 0; i < 16; i++) {
            angle = D_pspeu_092640C8[(i + 0) % 16];
            prim->r0 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;
            angle = D_pspeu_092640C8[(i + 5) % 16];
            prim->g0 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;
            angle = D_pspeu_092640C8[(i + 10) % 16];
            prim->b0 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;
            angle = D_pspeu_092640C8[(i + 1) % 16];
            prim->r1 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;
            angle = D_pspeu_092640C8[(i + 6) % 16];
            prim->g1 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;
            angle = D_pspeu_092640C8[(i + 11) % 16];
            prim->b1 =
                ((rsin(angle) + FIX(1.0 / 16)) >> 6) * D_pspeu_0926CCF0 / 256;

            prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 = prim->b3 =
                D_pspeu_0926CCE0;

            D_pspeu_092640C8[i] += 32;
            prim = prim->next;
        }
        break;
    }
}

extern s16 D_pspeu_09264108[];
extern s16 D_pspeu_09264120[];
extern s16 D_pspeu_09264138[];
// local copy of func_us_801BE2C8 (see no1/unk_3D8AC.c), with
// the rbo5 demo-timer lifetime check (see rbo5/doors.c).
void func_us_801BE2C8(Entity* self) {
    s32 primIndex;
    s16 rotate;
    s16 scaleY;
    long p, flag;
    SVECTOR p0, p1, p2, p3;
    u8 pad[4];
    VECTOR trans;
    MATRIX m;
    Primitive* lastPrim;
    Primitive* prim;
    s16 t;
    s16 iter;
    s32 i, j;
    s16* zPointer;
    s16 posX, posY; // unused

    SVECTOR rot = {0};

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        break;

    case 1:
        if (self->ext.et_801BE2C8.unk84) {
            self->step++;
        }
        break;

    case 2:
        self->ext.et_801BE2C8.unk84 = 0;
        primIndex = g_api.func_800EDB58(PRIM_GT4, 0x180);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.et_801BE2C8.unk7C = prim;
            while (prim != NULL) {
                prim->tpage = 0x1A;
                prim->clut = PAL_FILL_WHITE;
                prim->u0 = prim->u2 = 0x20;
                prim->u1 = prim->u3 = 0x28;
                prim->v0 = prim->v1 = 0xA0;
                prim->v2 = prim->v3 = 0xA8;
                prim->r0 = prim->g0 = prim->b0 = 0x10;
                LOW(prim->r1) = LOW(prim->r0);
                LOW(prim->r2) = LOW(prim->r0);
                LOW(prim->r3) = LOW(prim->r0);
                prim->priority = 0xD0;
                prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
                lastPrim = prim;
                prim = prim->next;
            }
            self->ext.et_801BE2C8.unk80 = lastPrim;
            lastPrim->x0 = lastPrim->x2 = 0;
            lastPrim->x1 = lastPrim->x3 = 0;
            lastPrim->y0 = lastPrim->y1 = 0;
            lastPrim->y2 = lastPrim->y3 = 0;
            lastPrim->r0 = lastPrim->g0 = lastPrim->b0 = 0xFF;
            LOW(lastPrim->r1) = LOW(lastPrim->r0);
            LOW(lastPrim->r2) = LOW(lastPrim->r0);
            LOW(lastPrim->r3) = LOW(lastPrim->r0);
            lastPrim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
        } else {
            DestroyEntity(self);
            return;
        }
        zPointer = self->ext.et_801BE2C8.unk88;
        for (i = 0; i < 8; i++) {
            *zPointer = D_pspeu_09264108[i];
            zPointer++;
        }
        self->ext.et_801BE2C8.unkA0 = 0;
        g_api.PlaySfx(SFX_DOP_DOOR_OPEN);
        self->step++;
        break;

    case 3:
        iter = 0;
        zPointer = self->ext.et_801BE2C8.unk88;
        prim = self->ext.et_801BE2C8.unk7C;
        SetGeomScreen(0x100);
        SetGeomOffset(self->posX.i.hi, self->posY.i.hi);
        for (i = 0; i < 8; i++) {
            scaleY = 0x280;
            rotate = *zPointer;
            RotMatrix(&rot, &m);
            RotMatrixY(scaleY, &m);
            RotMatrixZ(rotate, &m);
            trans.vx = 0;
            trans.vy = 0;
            trans.vz = 0x140;
            TransMatrix(&m, &trans);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            t = D_pspeu_09264138[i] - self->ext.et_801BE2C8.unkA0 / 16;
            if (t < 0) {
                t = 0;
            }
            p0.vx = 0;
            p0.vy = 0 - t;
            p0.vz = 0;

            p1.vx = t - 0;
            p1.vy = 0;
            p1.vz = 0;

            p2.vx = 0 - t;
            p2.vy = 0;
            p2.vz = 0;

            p3.vx = 0;
            p3.vy = t - 0;
            p3.vz = 0;
            for (j = 0; p0.vx < 0xC0; j++, prim = prim->next) {
                if (iter > 0x170) {
                    break;
                }
                RotAverage4(
                    &p0, &p1, &p2, &p3, (long*)(&prim->x0), (long*)(&prim->x1),
                    (long*)(&prim->x2), (long*)(&prim->x3), &p, &flag);
                prim->r0 = 0x20 - (i % 3) * 8 - j / 4;
                if (prim->r0 > 0x80) {
                    prim->r0 = 0;
                }
                prim->g0 = 0x20 - (i % 2) * 8 - j / 4;
                if (prim->g0 > 0x80) {
                    prim->g0 = 0;
                }
                prim->b0 = 0x20 - (i % 4) * 8 - j / 4;
                if (prim->b0 > 0x80) {
                    prim->b0 = 0;
                }
                LOW(prim->r1) = LOW(prim->r0);
                LOW(prim->r2) = LOW(prim->r0);
                LOW(prim->r3) = LOW(prim->r0);
                iter++;
                posX = prim->x0;
                posY = prim->y0;
                p0.vx += 3;
                p1.vx += 3;
                p2.vx += 3;
                p3.vx += 3;
            }
            *zPointer += D_pspeu_09264120[i];
            zPointer++;
        }
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        prim = self->ext.et_801BE2C8.unk80;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
        if (self->ext.et_801BE2C8.unk84) {
            self->ext.et_801BE2C8.unkA0 += 4;
        }
        if (self->ext.et_801BE2C8.unkA0 > 0x200) {
            posX = self->posX.i.hi;
            posY = self->posY.i.hi;
            DestroyEntity(self);
        }
        if (g_Player.demo_timer == 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern s16 D_pspeu_09264150[];
extern u16 D_pspeu_09264168[];
// local copy of func_us_801B5004 (see rbo5/doors.c)
void func_pspeu_092481F0_from_bo3(Tilemap* map, s32 arg1) {
    Tilemap* tmap;
    s16 tilePos;
    u16* tileData;
    s32 i;

    tmap = &g_Tilemap;
    tilePos = D_pspeu_09264150[arg1 >> 1];
    tileData = &D_pspeu_09264168[arg1 << 2];

    for (i = 0; i < 4; i++) {
        tmap->fg[tilePos] = *tileData++;
        tilePos += tmap->hSize << 4;
    }
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_259A0", func_pspeu_0925EF30);
