// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

extern EInit g_EInitEnvironment;
extern EInit g_EInitCommon;

void func_us_801AB9EC_from_bo0(Primitive* prim);
extern Primitive* FindFirstUnkPrim(Primitive* prim);
extern Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);
extern void UnkPolyFunc0(Primitive* prim);
extern void UnkPrimHelper(Primitive* prim);

INCLUDE_ASM("st/rno2/nonmatchings/unk_3459C", func_us_801AB9EC_from_bo0);

extern u16 D_us_80180DFC[];
extern u16 D_us_80180E10[];

void func_us_801B5FB8_from_no2(Entity* self) {
    Entity* tempEntity;
    Primitive* prim;
    s32 primIndex;
    s32 i;
    s32 tileIdx;

    FntPrint("timer %x\n", self->ext.breakableNo2.unk80);
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->animCurFrame = 0;
        if (g_CastleFlags[RNO2_SECRET_WALL_OPEN]) {
            for (i = 0; i < 10; i++) {
                tileIdx = D_us_80180DFC[i];
                g_Tilemap.fg[tileIdx] = D_us_80180E10[i];
            }
            DestroyEntity(self);
            return;
        }
        self->zPriority = 0xA8;
        self->hitboxState = 2;
        self->hitPoints = 0x7FFF;
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0x28;
        break;

    case 1:
        if (self->hitFlags) {
            PlaySfxPositional(SFX_WALL_DEBRIS_B);
            self->ext.breakableNo2.unk80 = 0x10;
            self->ext.breakableNo2.unk88++;
            self->step++;
        }
        if (self->ext.breakableNo2.unk88 == 3) {
            self->hitboxState = 0;
            self->step = 3;
        }
        break;

    case 2:
        if (self->ext.breakableNo2.unk88 == 1) {
            self->animCurFrame = 12;
        }
        if (self->ext.breakableNo2.unk88 == 2) {
            self->animCurFrame = 13;
        }
        if (!--self->ext.breakableNo2.unk80) {
            self->step--;
        }
        break;

    case 3:
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableNo2.unk7C = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        } else {
            g_CastleFlags[RNO2_SECRET_WALL_OPEN] |= 1;
            g_api.RevealSecretPassageAtPlayerPositionOnMap(
                RNO2_SECRET_WALL_OPEN);
            for (i = 0; i < 10; i++) {
                tileIdx = D_us_80180DFC[i];
                g_Tilemap.fg[tileIdx] = D_us_80180E10[i];
            }
            DestroyEntity(self);
            return;
        }
        prim = self->ext.breakableNo2.unk7C;
        for (i = 0; i < 2; i++) {
            prim->tpage = 0xF;
            prim->clut = 0x21;
            prim->u0 = prim->u2 = 0x80;
            prim->u1 = prim->u3 = 0x97;
            prim->v0 = prim->v1 = 0xF0;
            prim->v2 = prim->v3 = 0xFF;
            prim->x0 = prim->x2 = self->posX.i.hi + 0x11;
            prim->x1 = prim->x3 = prim->x0 - 0x18;
            if (i != 0) {
                prim->y0 = prim->y1 = self->posY.i.hi + 0x10008;
            } else {
                prim->y0 = prim->y1 = self->posY.i.hi + 0xFFE8;
            }
            prim->y2 = prim->y3 = prim->y0 + 0x10;
            prim->priority = 0xA8;
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        }
        self->ext.breakableNo2.unk80 = 0x20;
        self->step++;
        break;

    case 4:
        if (!--self->ext.breakableNo2.unk80) {
            self->step++;
        }
        break;

    case 5:
        self->animCurFrame = 0;
        g_CastleFlags[RNO2_SECRET_WALL_OPEN] |= 1;
        g_api.RevealSecretPassageAtPlayerPositionOnMap(RNO2_SECRET_WALL_OPEN);
        for (i = 0; i < 10; i++) {
            tileIdx = D_us_80180DFC[i];
            g_Tilemap.fg[tileIdx] = D_us_80180E10[i];
        }
        prim = self->ext.breakableNo2.unk7C;
        for (i = 0; i < 8; i++) {
            UnkPolyFunc2(prim);
            prim->next->x1 = self->posX.i.hi - 8 + ((i % 2) * 0x10);
            prim->next->y0 = self->posY.i.hi - 24 + ((i / 2) * 0x10);
            prim->next->r3 = i;
            prim = prim->next;
            prim = prim->next;
        }
        tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (tempEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
            tempEntity->posY.i.hi += 0x20;
            tempEntity->params = 0x13;
            tempEntity->params += 0xAA00;
        }
        for (i = 0; i < 8; i++) {
            tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, tempEntity);
                tempEntity->posX.i.hi += 0xF - (Random() & 0x1F);
                tempEntity->posY.i.hi += 0xF - (Random() & 0x1F);
                tempEntity->params = 0x10;
                tempEntity->params += 0xAA00;
            }
        }
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
        self->ext.breakableNo2.unk80 = 0x180;
        self->step++;
        break;

    case 6:
        prim = self->ext.breakableNo2.unk7C;
        while (prim != NULL) {
            if (prim->p3 & 8) {
                func_us_801AB9EC_from_bo0(prim);
            }
            prim = prim->next;
        }
        if (!--self->ext.breakableNo2.unk80) {
            g_CastleFlags[RNO2_SECRET_WALL_OPEN] |= 2;
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801AC54C_from_bo0(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->hitboxState = 0;
        self->animCurFrame = 0;
        break;

    case 1:
        if (g_CastleFlags[RNO2_SECRET_WALL_OPEN] & 2) {
            DestroyEntity(self);
            return;
        }
        if (g_CastleFlags[RNO2_SECRET_WALL_OPEN]) {
            g_CastleFlags[RNO2_SECRET_WALL_OPEN] |= 2;
        }
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableNo2.unk7C = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
            return;
        }
        prim = self->ext.breakableNo2.unk7C;
        for (i = 0; i < 4; i++) {
            UnkPolyFunc2(prim);
            prim->next->x1 = self->posX.i.hi;
            prim->next->y0 = self->posY.i.hi;
            prim->next->r3 = i + 8;
            prim = prim->next;
            prim = prim->next;
        }
        self->step++;
        break;

    case 2:
        i = 1;
        prim = self->ext.breakableNo2.unk7C;
        while (prim != NULL) {
            if (prim->p3 & 8) {
                i = 0;
                func_us_801AB9EC_from_bo0(prim);
            }
            prim = prim->next;
        }
        if (i != 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}

void func_us_801AC73C_from_bo0(Primitive* prim) {
    s32 x, y;

    if (!prim->g3) {
        prim->u0 = 1;
        prim->v0 = 1;
        prim->r0 = 0x80;
        prim->g0 = 0x80;
        prim->b0 = 0xC0;
        prim->drawMode = DRAW_UNK02;
        prim->x0 = g_CurrentEntity->posX.i.hi;
        prim->y0 = g_CurrentEntity->posY.i.hi + 8;
        prim->x1 = 0;
        prim->y1 = 0;
        LOW(prim->x2) = 0x7000 - ((Random() & 7) << 0xD);
        LOW(prim->x3) = 0x7000 - ((Random() & 7) << 0xD);
        prim->g3 = 1;
        prim->r3 = 0x20;
    }
#ifdef VERSION_US
    x = (prim->x0 << 0x10) + (u16)prim->x1;
#else
    x = (prim->x0 << 0x10) + prim->x1;
#endif
    x += LOW(prim->x2);
    prim->x0 = HIHU(x);
    prim->x1 = LOHU(x);
#ifdef VERSION_US
    y = (prim->y0 << 0x10) + (u16)prim->y1;
#else
    y = (prim->y0 << 0x10) + prim->y1;
#endif
    y += LOW(prim->x3);
    prim->y0 = HIH(y);
    prim->y1 = LOH(y);
    LOW(prim->x3) += 0x2000;
    prim->r3 -= 1;
    if (!prim->r3) {
        prim->g3 = 0;
        prim->drawMode = DRAW_HIDE;
        prim->p3 = 0;
    }
}

extern u16 D_us_80180E24[];
extern u16 D_us_80180E34[][8];

void func_us_801B68EC_from_no2(Entity* self) {
    Entity* tempEntity;
    Primitive* prim;
    s32 primIndex;
    s32 i;
    s32 tileIdx;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->animCurFrame = 0;
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate = 0xC00;
        if (g_CastleFlags[RNO2_SECRET_FLOOR_OPEN]) {
            for (i = 0; i < 8; i++) {
                tileIdx = D_us_80180E24[i];
                g_Tilemap.fg[tileIdx] = D_us_80180E34[1][i];
            }
            DestroyEntity(self);
            return;
        }
        for (i = 0; i < 8; i++) {
            tileIdx = D_us_80180E24[i];
            g_Tilemap.fg[tileIdx] = D_us_80180E34[0][i];
        }
        self->hitboxState = 2;
        self->hitPoints = 0x10;
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0x28;
        primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 0x1E);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableNo2.unk7C = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim->priority = 0x68;
                self->ext.breakableNo2.unk84 = prim;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
        }
        break;

    case 1:
        if (self->hitFlags) {
            for (i = 0; i < 0x10; i++) {
                prim = self->ext.breakableNo2.unk7C;
                prim = FindFirstUnkPrim(prim);
                if (prim != NULL) {
                    prim->p3 = 1;
                }
            }
        }
        prim = self->ext.breakableNo2.unk7C;
        while (prim != NULL) {
            if (prim->p3) {
                func_us_801AC73C_from_bo0(prim);
            }
            prim = prim->next;
        }
        prim = self->ext.breakableNo2.unk84;
        prim->x0 = prim->y0 = 0;
        prim->u0 = 0;
        prim->drawMode = DRAW_UNK02;
        if (self->flags & FLAG_DEAD) {
            self->animCurFrame = 0;
            self->step++;
        }
        break;

    case 2:
        primIndex = self->primIndex;
        g_api.FreePrimitives(primIndex);
        self->flags &= ~FLAG_HAS_PRIMS;
        g_CastleFlags[RNO2_SECRET_FLOOR_OPEN] = 1;
        g_api.RevealSecretPassageAtPlayerPositionOnMap(RNO2_SECRET_FLOOR_OPEN);
        for (i = 0; i < 8; i++) {
            tileIdx = D_us_80180E24[i];
            g_Tilemap.fg[tileIdx] = D_us_80180E34[1][i];
        }
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x18);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableNo2.unk7C = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
            return;
        }
        prim = self->ext.breakableNo2.unk7C;
        for (i = 0; i < 4; i++) {
            UnkPolyFunc2(prim);
            prim->next->x1 = self->posX.i.hi - 8 + ((i % 2) * 0x10);
            prim->next->y0 = self->posY.i.hi + ((i / 2) * 0x10);
            prim->next->r3 = i + 0xC;
            prim = prim->next;
            prim = prim->next;
        }
        tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (tempEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
            tempEntity->posY.i.hi += 0x20;
            tempEntity->params = 0x13;
            tempEntity->params += 0xAA00;
        }
        for (i = 0; i < 8; i++) {
            tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, tempEntity);
                tempEntity->posX.i.hi += 0xF - (Random() & 0x1F);
                tempEntity->posY.i.hi += (Random() & 0x1F);
                tempEntity->params = 0x10;
                tempEntity->params += 0xAA00;
            }
        }
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
        self->step++;
        break;

    case 3:
        i = 1;
        prim = self->ext.breakableNo2.unk7C;
        while (prim != NULL) {
            if (prim->p3 & 8) {
                i = 0;
                func_us_801AB9EC_from_bo0(prim);
            }
            prim = prim->next;
        }
        if (i != 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}
