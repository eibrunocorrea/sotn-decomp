// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"
#include <scratchpad.h>

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", EntityFlyingZombie2);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", EntityFlyingZombie1);

extern s16 D_us_80182298[];

// nb. this is very similar to the stained glass recursive function in DAI
static Primitive* func_us_801B2C40_from_rare(
    SVECTOR* p0, SVECTOR* p1, SVECTOR* p2, SVECTOR* p3, Primitive* srcPrim,
    s32 iterations, Primitive* dstPrim, u8* dataPtr) {
    long sp4C;
    long sp48;
    long sp44;
    s32 index0;
    s32 index1;
    s32 index2;
    s32 index3;
    s16* indexPtr;
    uvPair* uvValues;
    Primitive* tempPrim;
    SVECTOR* points;
    s32 count;
    Primitive* dstPrimNext;

    if (dstPrim == NULL) {
        return NULL;
    }
    tempPrim = (Primitive*)dataPtr;
    dataPtr += sizeof(Primitive);
    points = (SVECTOR*)dataPtr;
    dataPtr += sizeof(SVECTOR) * 9;
    uvValues = (uvPair*)dataPtr;
    dataPtr += sizeof(uvPair) * 10;
    points[0] = *p0;
    points[2] = *p1;
    points[6] = *p2;
    points[8] = *p3;

    points[3].vx = ((points[0].vx + points[6].vx + 1) >> 1);
    points[3].vy = ((points[0].vy + points[6].vy + 1) >> 1);
    points[3].vz = ((points[0].vz + points[6].vz + 1) >> 1);
    points[5].vx = ((points[2].vx + points[8].vx + 1) >> 1);
    points[5].vy = ((points[2].vy + points[8].vy + 1) >> 1);
    points[5].vz = ((points[2].vz + points[8].vz + 1) >> 1);

    uvValues[0] = UV(srcPrim->u0);
    uvValues[2] = UV(srcPrim->u1);
    uvValues[6] = UV(srcPrim->u2);
    uvValues[8] = UV(srcPrim->u3);
    uvValues[3].u = (((uvValues[0].u) + (uvValues[6].u) + 1) >> 1);
    uvValues[3].v = (((uvValues[0].v) + (uvValues[6].v) + 1) >> 1);
    uvValues[5].u = (((uvValues[2].u) + (uvValues[8].u) + 1) >> 1);
    uvValues[5].v = (((uvValues[2].v) + (uvValues[8].v) + 1) >> 1);

    *tempPrim = *srcPrim;
    indexPtr = D_us_80182298;
    for (count = 0; count < 2; count++) {
        index0 = *indexPtr++;
        index1 = *indexPtr++;
        index2 = *indexPtr++;
        index3 = *indexPtr++;

        sp44 = RotTransPers4(
            &points[index0], &points[index1], &points[index2], &points[index3],
            (long*)&tempPrim->x0, (long*)&tempPrim->x1, (long*)&tempPrim->x2,
            (long*)&tempPrim->x3, &sp48, &sp4C);

        UV(tempPrim->u0) = uvValues[index0];
        UV(tempPrim->u1) = uvValues[index1];
        UV(tempPrim->u2) = uvValues[index2];
        UV(tempPrim->u3) = uvValues[index3];

        if (iterations == 1) {
            dstPrimNext = dstPrim->next;
            *dstPrim = *tempPrim;
            dstPrim->next = dstPrimNext;
            dstPrim = dstPrim->next;
            if (dstPrim == NULL) {
                return NULL;
            }
        } else {
            dstPrim = func_us_801B2C40_from_rare(
                &points[index0], &points[index1], &points[index2],
                &points[index3], tempPrim, iterations - 1, dstPrim, dataPtr);
        }
    }
    return dstPrim;
}

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", UpdateBodyDisplay);

extern SVECTOR D_us_80182034;
extern SVECTOR D_us_8018203C;
extern SVECTOR D_us_80182044;
extern SVECTOR D_us_8018204C;
extern SVECTOR D_us_80182054;
extern SVECTOR D_us_8018205C;
extern SVECTOR D_us_80182064;
extern SVECTOR D_us_8018206C;
extern SVECTOR D_us_80182074;

static void func_us_801B33F4_from_rare(void) {
    long flag;
    long p;
    SVECTOR sVec;
    VECTOR vector;
    MATRIX matrix;

    Primitive* prim;
    Primitive* primTwo;
    Entity* entity;
    s32 i;
    s32 swordHitboxOffsetX;
    s32 swordHitboxOffsetY;
    s32 posX;
    s32 posY;

    posX = g_CurrentEntity->posX.i.hi;
    posY = g_CurrentEntity->posY.i.hi;
    SetGeomScreen(0x100);
    SetGeomOffset(posX, posY);
    sVec.vx = g_CurrentEntity->ext.azaghal.pos.vx;
    sVec.vy = g_CurrentEntity->ext.azaghal.pos.vy;
    sVec.vz = g_CurrentEntity->ext.azaghal.pos.vz;
    RotMatrix(&sVec, &matrix);

    if (g_CurrentEntity->facingLeft) {
        RotMatrixY(0x800, &matrix);
    }

    vector.vx = 0;
    vector.vy = 0;
    vector.vz = 0x100;
    TransMatrix(&matrix, &vector);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);

    prim = g_CurrentEntity->ext.azaghal.prim;
    prim->drawMode = DRAW_UNK02;
    RotTransPers4(&D_us_80182034, &D_us_8018203C, &D_us_80182044,
                  &D_us_8018204C, (long*)&prim->x0, (long*)&prim->x1,
                  (long*)&prim->x2, (long*)&prim->x3, &p, &flag);
    primTwo = g_CurrentEntity->ext.azaghal.primThree;
    primTwo = func_us_801B2C40_from_rare(&D_us_80182034, &D_us_8018203C,
                               &D_us_80182044, &D_us_8018204C, prim, 3,
                               primTwo, (u8*)SPAD(0));
    prim->drawMode = DRAW_HIDE;
    prim = prim->next;

    prim->drawMode = DRAW_UNK02;
    RotTransPers4(&D_us_80182054, &D_us_8018205C, &D_us_80182064,
                  &D_us_8018206C, (long*)&prim->x0, (long*)&prim->x1,
                  (long*)&prim->x2, (long*)&prim->x3, &p, &flag);
    primTwo = func_us_801B2C40_from_rare(&D_us_80182054, &D_us_8018205C,
                               &D_us_80182064, &D_us_8018206C, prim, 3,
                               primTwo, (u8*)SPAD(0));
    prim->drawMode = DRAW_HIDE;

    if (g_CurrentEntity->flags & FLAG_DEAD) {
        while (primTwo != NULL) {
            primTwo->drawMode = DRAW_HIDE;
            primTwo = primTwo->next;
        }
    } else {
        RotTransPers(&D_us_80182074, (long*)SPAD(0), &p, &flag);
        swordHitboxOffsetX = *(s16*)SP(0);
        swordHitboxOffsetY = *(s16*)SP(2);
        swordHitboxOffsetX -= posX;
        swordHitboxOffsetY -= posY;

        g_CurrentEntity->hitboxOffX = swordHitboxOffsetX / 8;
        if (g_CurrentEntity->facingLeft) {
            g_CurrentEntity->hitboxOffX = (s16)-g_CurrentEntity->hitboxOffX;
        }

        g_CurrentEntity->hitboxOffY = swordHitboxOffsetY / 8;

        for (i = 2; i < 8; i++) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromCurrentEntity(E_AZAGHAL_SWORD_HITBOX, entity);
                entity->posX.i.hi = posX + ((swordHitboxOffsetX * i) / 8);
                entity->posY.i.hi = posY + ((swordHitboxOffsetY * i) / 8);
            }
        }

        prim = primTwo;
        for (i = 0; i < 6; i++) {
            prim->type = PRIM_G4;
            primTwo = prim->next;

            prim->r0 = i * 0x10;
            prim->g0 = i * 0x10;
            prim->b0 = i * 0x10;
            LOW(prim->r1) = LOW(prim->r0);
            prim->r2 = (i + 1) * 0x10;
            prim->g2 = (i + 1) * 0x10;
            prim->b2 = (i + 1) * 0x10;
            LOW(prim->r3) = LOW(prim->r2);
            LOW(prim->x0) = LOW(primTwo->x0);
            LOW(prim->x1) = LOW(primTwo->x1);
            LOW(prim->x2) = LOW(primTwo->x2);
            LOW(prim->x3) = LOW(primTwo->x3);
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            prim = primTwo;
        }

        prim->type = PRIM_G4;
        prim->r0 = i * 0x10;
        prim->g0 = i * 0x10;
        prim->b0 = i * 0x10;
        LOW(prim->r1) = LOW(prim->r0);
        prim->r2 = (i + 1) * 0x10;
        prim->g2 = (i + 1) * 0x10;
        prim->b2 = (i + 1) * 0x10;
        LOW(prim->r3) = LOW(prim->r2);
        LOW(prim->x0) = LOW(prim->x2);
        LOW(prim->x1) = LOW(prim->x3);

        prim->x2 = posX + (swordHitboxOffsetX / 4);
        prim->y2 = posY + (swordHitboxOffsetY / 4);
        prim->x3 = (posX + swordHitboxOffsetX) - (swordHitboxOffsetX / 8);
        prim->y3 = (posY + swordHitboxOffsetY) - (swordHitboxOffsetY / 8);
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
        prim = prim->next;

        swordHitboxOffsetX = 0;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            swordHitboxOffsetX++;
            prim = prim->next;
        }
        FntPrint("no_use %x\n", swordHitboxOffsetX);
    }
}

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", InitPositionLerp);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", ApplyPositionLerp);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", AnimateAzaghal);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", EntityAzaghal);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", EntityAzaghalSwordHitbox);

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", func_us_801CAB8C);
