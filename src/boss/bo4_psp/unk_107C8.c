// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo4/bo4.h"

// The Doppleganger boss reuses the shared player-anim block (dop_anim.h);
// splat's pre-split names (Mar*/func_pspeu_*) are kept until a rename pass
#define MARIA g_Entities[STAGE_ENTITY_START]
#define g_Dop g_Maria
#define SetDopplegangerStep MarSetStep
#define InitPlayerAfterImage InitMariaAfterImage
#define DrawPlayerAfterImage DrawMariaAfterImage
#define SetDopplegangerAnim func_pspeu_0924DCB8_from_rbo5
#define UpdateUnarmedAnim func_pspeu_0924DE50_from_rbo5
#define PlayAnimation func_pspeu_0924DFB0_from_rbo5
#define UpdateAnim func_pspeu_0924E260_from_rbo5
#define DOP_ANIM_NO_NULL_CHECK

extern PlayerState g_Maria;

// collision sensor tables (.rodata block shared with rbo5, different VMAs)
extern Point16 g_MarSensorsCeiling[];
extern Point16 g_MarSensorsFloor[];
extern Point16 g_MarSensorsWall[];

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
// crouch pose/anim pairs and stand anim table (.rodata shared with rbo5)
extern u8 D_pspeu_0926B1B8[];
extern u8 D_pspeu_0926B1C0[];

void SetDopplegangerStep(s16 step);
void SetDopplegangerAnim();
s32 func_pspeu_0924EA98_from_rbo5(s32 arg0);
s32 func_pspeu_0924CF50_from_rbo5(void);
void func_pspeu_0924C9C8_from_rbo5(s32 arg0, s32 arg1);
void func_pspeu_0924CA58_from_rbo5(s32 arg0);
void func_pspeu_0924CBF0_from_rbo5(s32 arg0);
void func_pspeu_0924CD20_from_rbo5(void);
void func_pspeu_0924CDE0_from_rbo5(void);
void func_pspeu_0924CE30_from_rbo5(void);
s32 func_pspeu_0924D108_from_rbo5(void);
void func_pspeu_0924D4E8_from_rbo5(void);
void func_pspeu_0924D528_from_rbo5(void);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_us_801C0B9C_from_no1);

// local copy of RicGetFreeEntity (see bo6/us_39144.c); byte-identical
// across the three boss walls
Entity* MarGetFreeEntity(s16 start, s16 end) {
    Entity* entity = &g_Entities[start];
    s16 i;

    for (i = start; i < end; i++, entity++) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

// local copy of RicGetFreeEntityReverse (see bo6/us_39144.c)
Entity* MarGetFreeEntityReverse(s16 start, s16 end) {
    Entity* entity = &g_Entities[end - 1];
    s16 i;
    for (i = end - 1; i >= start; i--, entity--) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

extern u8 D_pspeu_09268188[][4];
extern u8 D_pspeu_0926C918;
extern u8 D_pspeu_0926C910;
extern u8 D_pspeu_0926C908;
extern u8 D_pspeu_0926C900;
// local copy of func_us_801BB314 (see bo6/us_39144.c)
void func_pspeu_09252480_from_rbo5(s32 arg0) {
    D_pspeu_0926C918 = D_pspeu_09268188[arg0][0];
    D_pspeu_0926C910 = D_pspeu_09268188[arg0][1];
    D_pspeu_0926C908 = D_pspeu_09268188[arg0][2];
    D_pspeu_0926C900 = D_pspeu_09268188[arg0][3];
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_8015FDB0);

s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY);
extern Point16 D_pspeu_0926C8C0[16];
// local copy of RicEntityHitByHoly (see ric/pl_blueprints.c); the boss
// walls track the doppleganger in slot 64 and camera-lock the effect
void func_pspeu_09252768_from_rbo5(Entity* entity) {
    Primitive* prim;
    s32 i;
    s32 temp;
    s16 hitboxX;
    s16 hitboxY;
    s16 temp_xRand;
    s16 temp_yRand;

    switch (entity->step) {
    case 0:
        entity->primIndex =
            (s16)g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_0926C8C0));
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        hitboxX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        hitboxY = MARIA.posY.i.hi + MARIA.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < LEN(D_pspeu_0926C8C0); i++) {
            temp_xRand = hitboxX + rand() % 24 - 12;
            temp_yRand = hitboxY + rand() % 48 - 24;
            D_pspeu_0926C8C0[i].x = temp_xRand;
            D_pspeu_0926C8C0[i].y = temp_yRand;
            prim->clut = PAL_UNK_1B2;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->b1 = 0;
            prim->g0 = 0;
            prim->g1 = (rand() & 7) + 1;
            prim->g2 = 0;
            prim->priority = MARIA.zPriority + 4;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE | DRAW_HIDE |
                             DRAW_UNK02 | DRAW_TRANSP;
            if (rand() & 1) {
                prim->drawMode =
                    DRAW_UNK_100 | DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                    DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }
        entity->step++;
        break;
    case 1:
        if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
            DestroyEntity(entity);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[entity->primIndex];
    for (i = 0; i < 16; i++) {
        switch (prim->g0) {
        case 0:
            if (--prim->g1 == 0) {
                prim->g0++;
            }
            break;
        case 1:
            hitboxX = D_pspeu_0926C8C0[i].x;
            hitboxY = D_pspeu_0926C8C0[i].y;
            temp = func_8015FDB0(prim, hitboxX, hitboxY);
            D_pspeu_0926C8C0[i].y--;
            if (temp < 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->g0++;
            } else {
                prim->drawMode &= ~DRAW_HIDE;
            }
            break;
        }
        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252B20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252B48_from_rbo5);

// local copy of RicCreateEntFactoryFromEntity (see bo6/us_39144.c);
// same (68, 80) slot range as bo6
Entity* MarCreateEntFactoryFromEntity(
    Entity* source, u32 factoryParams, s32 arg2) {
    Entity* entity = MarGetFreeEntity(68, 80);
    if (!entity) {
        return NULL;
    }
    DestroyEntity(entity);
    entity->entityId = E_FACTORY;
    // the parent pointer must align for anything the factory creates
    entity->ext.factory.parent = source;
    entity->posX.val = source->posX.val;
    entity->posY.val = source->posY.val;
    entity->facingLeft = source->facingLeft;
    entity->zPriority = source->zPriority;
    entity->params = factoryParams & 0xFFF;
    entity->ext.factory.paramsBase = (factoryParams & 0xFF0000) >> 8;
    return entity;
}

extern FactoryBlueprint D_pspeu_092682F0[];
extern u8 D_pspeu_092685A8[][2];
// local copy of the doppleganger entity factory (see func_us_801CA2AC in
// bo4/unk_46E7C.c); aka MarEntityFactory in the bo6 wall
void func_pspeu_09252E78_from_rbo5(Entity* self) {
    Entity* newEntity;
    s16 i;
    s16 n;
    s16 endIndex;
    s16 startIndex;
    u8* data;

    if (self->step == 0) {
        data = (u8*)&D_pspeu_092682F0[self->params];
        self->ext.factory.newEntityId = *data++;
        self->ext.factory.amount = *data++;
        self->ext.factory.nPerCycle = *data & 0x3F;
        self->ext.factory.isNonCritical = (s16)(*data >> 7) & 1;
        self->ext.factory.incParamsKind = (s16)(*data++ >> 6) & 1;
        self->ext.factory.tCycle = *data++;
        self->ext.factory.kind = *data & 0xF;
        self->ext.factory.origin = (s16)(*data++ >> 4) & 0xF;
        self->ext.factory.delay = *data;
        self->flags |= FLAG_UNK_10000000;

        self->step++;

        switch (self->ext.factory.origin) {
        case B_ORIGIN_DEFAULT:
        case B_ORIGIN_6:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        case B_ORIGIN_2:
        case B_ORIGIN_4:
        case B_ORIGIN_5:
        case B_ORIGIN_7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        }
    } else {
        switch (self->ext.factory.origin) {
        case B_ORIGIN_DEFAULT:
        case B_ORIGIN_1:
        case B_ORIGIN_3:
        case B_ORIGIN_5:
        case B_ORIGIN_6:
            break;
        case B_ORIGIN_2:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case B_ORIGIN_4:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != Dop_Walk) {
                self->entityId = 0;
                return;
            }
            break;
        case B_ORIGIN_7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != Dop_Hit) {
                self->entityId = 0;
                return;
            }
            break;
        }
    }

    if (self->ext.factory.delay) {
        if (--self->ext.factory.delay) {
            return;
        }
        self->ext.factory.delay = self->ext.factory.tCycle;
    }

    // Save this value so we don't have to re-fetch on every for-loop cycle
    n = self->ext.factory.nPerCycle;
    for (i = 0; i < n; i++) {
        // !FAKE, this should probably be &entity_ranges[unk9C] or similar,
        // instead of doing &entity_ranges followed by +=
        data = (u8*)&D_pspeu_092685A8[0];
        data += self->ext.factory.kind * 2;

        startIndex = *data++;
        endIndex = *data;

        if (self->ext.factory.kind == 3) {
            DestroyEntity(&g_Entities[startIndex]);
            newEntity = &g_Entities[startIndex];
            g_Maria.unk48 = 0;
        } else if (self->ext.factory.kind == 0) {
            newEntity = MarGetFreeEntityReverse(startIndex, endIndex + 1);
        } else if (self->ext.factory.kind == 8) {
            if ((self->ext.factory.spawnIndex % 3) == 0) {
                newEntity = MarGetFreeEntity(81, 96);
            }
            if ((self->ext.factory.spawnIndex % 3) == 1) {
                newEntity = MarGetFreeEntity(96, 112);
            }
            if ((self->ext.factory.spawnIndex % 3) == 2) {
                newEntity = MarGetFreeEntity(112, 128);
            }
        } else {
            newEntity = MarGetFreeEntity(startIndex, endIndex + 1);
        }

        if (newEntity == NULL) {
            if (self->ext.factory.isNonCritical == 1) {
                self->entityId = 0;
            } else {
                self->ext.factory.delay = self->ext.factory.tCycle;
            }
            return;
        }

        DestroyEntity(newEntity);
        newEntity->entityId = self->ext.factory.newEntityId;
        newEntity->params = self->ext.factory.paramsBase;
        // The child (newEntity) is not an ent factory, but because the
        // factory creates many entities, we can't pick a particular
        // extension. But we're not allowed to use generic, so i'll just
        // reuse entFactory.
        newEntity->ext.factory.parent = self->ext.factory.parent;
        newEntity->posX.val = self->posX.val;
        newEntity->posY.val = self->posY.val;
        newEntity->facingLeft = self->facingLeft;
        newEntity->zPriority = self->zPriority;

        if (self->ext.factory.incParamsKind) {
            newEntity->params += self->ext.factory.spawnIndex;
        } else {
            newEntity->params += i;
        }

        self->ext.factory.spawnIndex++;

        if (self->ext.factory.spawnIndex == self->ext.factory.amount) {
            self->entityId = 0;
            return;
        }
    }
    self->ext.factory.delay = self->ext.factory.tCycle;
}

extern u16 D_pspeu_092633D8[];
// local copy of func_us_801CA748 (see bo4/unk_46E7C.c); contact damage
// entity that tracks the doppleganger during the bat-form slide
void func_pspeu_09253500_from_rbo5(Entity* self) {
    if (MARIA.step_s != Dop_Crouch || MARIA.step != Dop_MorphBat) {
        DestroyEntity(self);
        return;
    }

    if (self->step == 0) {
        InitializeEntity(D_pspeu_092633D8);
        if (g_Maria.status & PLAYER_STATUS_POISON) {
            self->attack = self->attack / 2;
        }
        self->hitboxOffX = 4;
        self->step++;
    }

    self->flags =
        FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_NOT_AN_ENEMY;
    self->facingLeft = MARIA.facingLeft;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->posX.i.hi = MARIA.posX.i.hi;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09253620_from_rbo5);

extern AnimationFrame D_pspeu_09268680[];
extern s32 D_pspeu_0926C8B8;
// local copy of RicEntityHitByDark (see ric/pl_blueprints.c); the boss
// walls use different flags and track the doppleganger's z-priority
void func_pspeu_09253EA0_from_rbo5(Entity* entity) {
    s16 x, y;

    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        entity->unk5A = 0x79;
        entity->animSet = ANIMSET_DRA(14);
        entity->zPriority = MARIA.zPriority + 2;
        entity->palette = PAL_FLAG(PAL_UNK_19F);
        if (D_pspeu_0926C8B8 & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        D_pspeu_0926C8B8++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_pspeu_09268680;
        entity->posY.i.hi += (rand() % 35) - 15;
        entity->posX.i.hi += (rand() % 20) - 10;
        entity->velocityY = -0x6000 - (rand() & 0x3FFF);
        entity->step++;
        break;
    case 1:
        if (entity->opacity > 16) {
            entity->opacity -= 8;
        }
        entity->posY.val += entity->velocityY;
        entity->scaleX += 8;
        entity->scaleY += 8;
        if (entity->poseTimer < 0) {
            DestroyEntity(entity);
        }
        break;
    }
}

// local copy of func_us_801BD47C (see bo6/us_39144.c); true when
// another live entity shares this id and params
bool func_maria_80162E9C(Entity* entity) {
    Entity* e;
    s32 i;
    s16 objId;
    s16 params;

    objId = entity->entityId;
    params = entity->params;
    for (e = &g_Entities[0x50], i = 0x50; i < 0x90; e++, i++) {
        if (objId == e->entityId && params == e->params && e != entity) {
            return true;
        }
    }
    return false;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09254180_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09256258_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09256D30_from_rbo5);

// local copy of EntityWingSmashTrail (see bo4/unk_46E7C.c)
void func_pspeu_092570B8_from_rbo5(Entity* self) {
    if (!(MARIA.step_s == 3 && MARIA.step == Dop_MorphBat)) {
        DestroyEntity(self);
        return;
    }

    if (self->step == 0) {
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->animSet = MARIA.animSet;
        self->animCurFrame = MARIA.animCurFrame | ANIM_FRAME_LOAD;
        self->unk5A = 8;
        self->zPriority = MARIA.zPriority - 2;
        self->drawFlags =
            MARIA.drawFlags | (ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX);
        self->opacity = 0x80;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->rotate = MARIA.rotate;
        self->facingLeft = MARIA.facingLeft;
        self->palette = PAL_FLAG(0x202);
        self->scaleX = self->scaleY = 0x100;
        self->step++;
        return;
    }
    // This actually makes the wing smashes shrink over time, not rotate.
    self->scaleX -= 8;
    self->scaleY -= 8;
    self->animCurFrame = MARIA.animCurFrame | ANIM_FRAME_LOAD;
    if (self->opacity >= 5) {
        self->opacity -= 5;
    } else {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09257298_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_maria_80161C2C);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarEntityHitByLightning);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09258A50_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09259480_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09259A20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0925BA80_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092457C8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09245AD8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09246210_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09254448);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09255070);

void func_pspeu_09248828_from_rbo5(void) {
    if (g_Maria.timers[1]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x17), 0);
    }
    if (g_Maria.timers[0]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x16), 0);
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248898_from_rbo5);

void func_pspeu_09248B88_from_rbo5(void) {
    if (func_pspeu_0924EA98_from_rbo5(0x4301C) == 0) {
        MarSetSpeedX(FIX(1.5));
        if (MarCheckFacing() == 0) {
            func_pspeu_0924CA58_from_rbo5(0);
        }
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248BE8_from_rbo5);

void MarDecelerateX_0925B090(s32 speed);
// local copy of DopplegangerStepFall (see bo4/unk_45354.c)
void func_pspeu_09248F50_from_rbo5(void) {
    if (func_pspeu_0924EA98_from_rbo5(0x9029) == 0) {
        MarDecelerateX_0925B090(FIX(1.0 / 16.0));
        if (MarCheckFacing() != 0) {
            MarSetSpeedX(FIX(3.0 / 4.0));
        }
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248FA8_from_rbo5);

// cen's func_80158B04 with the Doppleganger's taller ember origin (22 vs 16)
void func_80158B04(u16 arg0) {
    s16 xMod = 3;
    if (MARIA.facingLeft) {
        xMod = -xMod;
    }

    MARIA.posY.i.hi -= 22;
    MARIA.posX.i.hi += xMod;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_4, 1), 0);
    MARIA.posY.i.hi += 22;
    MARIA.posX.i.hi -= xMod;

    if (arg0 & 1) {
        g_api.func_80102CD8(3);
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
    }

    if (arg0 & 2) {
        MARIA.velocityX = 0;
        MARIA.velocityY = 0;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249460_from_rbo5);

s32 func_pspeu_092497C0_from_rbo5(void) {
    s16 rnd = rand() & PSP_RANDMASK;
    MARIA.ext.player.anim = 0x2E + (rnd % 3);
    return rnd % 16;
}

// local copy of func_us_801C72BC (see bo4/unk_46E7C.c): reset the boss
// visual state; the PSP build reorders the stores and chains pose/poseTimer
void func_pspeu_09249838_from_rbo5(void) {
    MARIA.pose = MARIA.poseTimer = 0;
    MARIA.animSet = ANIMSET_OVL(1);
    MARIA.blendMode = BLEND_NO;
    g_Maria.unk44 = 0;
    g_Maria.unk46 = 0;
    MARIA.drawFlags &= ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G |
                       ENTITY_MASK_R | ENTITY_SCALEY | ENTITY_SCALEX;
    MARIA.rotate = 0;
    if (g_Entities[80].entityId == E_MIST) {
        func_pspeu_0924D4E8_from_rbo5();
    }
}

void func_80159C04(void) {
    if (MARIA.posX.i.hi <= PLAYER.posX.i.hi) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249910_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249FE8_from_rbo5);

extern u16 D_pspeu_0926AB12;
// local copy of BatFormFinished (see bo4/unk_46E7C.c); the u16 is the
// bat morph animation's [0].pose field
s32 func_pspeu_0924A948_from_rbo5(void) {
    if (MARIA.step_s == 0) {
        return 0;
    }
    if (g_Maria.padTapped & PAD_R1) {
        MarSetStep(Dop_UnmorphBat);
        SetDopplegangerAnim(202);
        D_pspeu_0926AB12 = 6;
        MARIA.palette = PAL_FLAG(0x20D);
        g_Maria.unk66 = 0;
        g_Maria.unk68 = 0;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x21), 0);
        MARIA.velocityY >>= 1;
        return 1;
    }
    return 0;
}

void func_pspeu_0924AA18_from_rbo5(s16 arg0) {
    if (MARIA.rotate < arg0) {
        MARIA.rotate += 16;
        if (MARIA.rotate > arg0) {
            MARIA.rotate = arg0;
        }
    }
    if (MARIA.rotate > arg0) {
        MARIA.rotate -= 16;
        if (MARIA.rotate < arg0) {
            MARIA.rotate = arg0;
        }
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924AAF0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924AB20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924B918_from_rbo5);

// local copy of func_us_801C8EE4 (see bo4/unk_46E7C.c): leave mist form
s32 func_pspeu_0924BB78_from_rbo5(void) {
    if (MARIA.step_s == 0) {
        return 0;
    }
    if (g_Maria.padTapped & PAD_R2) {
        MarCheckFacing();
        MarSetStep(Dop_UnmorphMist);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924BBE8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C200_from_rbo5);

extern s32 D_pspeu_0926CE18;
// local copy of DopplegangerStepSwordWarp (see bo4/unk_46E7C.c)
void func_pspeu_0924C498_from_rbo5(void) {
    if (MARIA.step_s == 0) {
        if (g_Entities[E_BOSS_WEAPON].entityId == E_NONE) {
            D_pspeu_0926CE18 = 0x10;
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(61, 0x15), 0);
            MARIA.step_s++;
        }
    } else if (--D_pspeu_0926CE18 == 0) {
        MARIA.palette = PAL_FLAG(0x200);
        func_pspeu_0924CA58_from_rbo5(0);
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C550_from_rbo5);

// local variant of the crouch setter (see bo4/unk_45354.c func_8010E470)
void func_pspeu_0924C9C8_from_rbo5(s32 arg0, s32 velocityX) {
    s32 unused_stack[2];

    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    MARIA.step = Dop_Crouch;
    MARIA.step_s = (s32)D_pspeu_0926B1B8[arg0 * 2];
    SetDopplegangerAnim(D_pspeu_0926B1B8[arg0 * 2 + 1]);
}

// local variant of the stand setter (see bo4/unk_45354.c func_8010E570)
void func_pspeu_0924CA58_from_rbo5(s32 arg0) {
    s32 anim = 0;
    bool atLedge = false;

    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        atLedge = true;
    }
    MARIA.velocityX = arg0;
    MARIA.velocityY = 0;
    SetDopplegangerStep(Dop_Stand);
    if (g_Maria.unk48) {
        MARIA.step_s = 2;
        atLedge = false;
    }

    switch (g_Maria.prev_step) {
    case Dop_UnmorphBat:
        anim = 4;
        break;
    case Dop_Walk:
        anim = 4;
        if (MARIA.ext.player.anim == 9) {
            MARIA.ext.player.anim = D_pspeu_0926B1C0[2 + atLedge];
            return;
        }
        if (MARIA.ext.player.anim == 7) {
            anim = 0;
        }
        break;
    case Dop_Jump:
    case Dop_Fall:
        anim = 6;
        if (abs(MARIA.velocityX) > FIX(2.5)) {
            anim = 4;
        }
        break;
    default:
        anim = 8;
        break;
    }
    anim += atLedge;
    SetDopplegangerAnim(D_pspeu_0926B1C0[anim]);
}

// local variant of the walk setter (see bo4/unk_45354.c func_8010E6AC)
void func_pspeu_0924CBF0_from_rbo5(s32 forceAnim13) {
    bool atLedge;

    atLedge = false;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        atLedge = true;
    }

    MarSetSpeedX(FIX(1.5));
    MARIA.velocityY = 0;
    SetDopplegangerStep(Dop_Walk);

    if (forceAnim13) {
        if (MARIA.ext.player.anim != 13) {
            SetDopplegangerAnim(13);
        }
    } else {
        SetDopplegangerAnim(7);
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(1, 5), 0);
    }

    if (g_Maria.unk4C) {
        MARIA.ext.player.anim = 9;
    }

    if (MARIA.ext.player.anim == 7 && atLedge) {
        MARIA.pose = 1;
    }

    if (g_Maria.prev_step == Dop_Crouch) {
        MARIA.pose = 4;
    }
}

// local variant of the jump setter (see bo4/unk_45354.c func_us_801C58E4)
void func_pspeu_0924CD20_from_rbo5(void) {
    if (MarCheckFacing() != 0) {
        SetDopplegangerAnim(0x1A);
        MarSetSpeedX(FIX(3.0 / 2.0));
        g_Maria.unk44 = 0;
    } else {
        SetDopplegangerAnim(0x16);
        MARIA.velocityX = 0;
        g_Maria.unk44 = 4;
    }
    MARIA.velocityY = FIX(-4.875);
    SetDopplegangerStep(Dop_Jump);
    if (g_Maria.prev_step == Dop_Walk) {
        g_Maria.unk44 |= 0x10;
    }
}

// local variant of the double jump setter (see bo4/unk_45354.c
// func_us_801C5990)
void func_pspeu_0924CDE0_from_rbo5(void) {
    g_Maria.unk44 |= 0x21;
    MARIA.velocityY = FIX(-4.25);
    SetDopplegangerAnim(0x20);
    MARIA.step_s = 0;
}

// local variant of the fall setter (see bo4/unk_45354.c func_us_801C59DC)
void func_pspeu_0924CE30_from_rbo5(void) {
    SetDopplegangerStep(Dop_Fall);
    if (g_Maria.prev_step != Dop_Walk) {
        SetDopplegangerAnim(0x1C);
    }
    MARIA.velocityX = 0;
    MARIA.velocityY = FIX(2.0);
    g_Maria.timers[ALU_T_5] = 8;
    g_Maria.timers[ALU_T_6] = 8;
    g_Maria.unk44 = 0x10;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CF50_from_rbo5);

// local variant of the subweapon handler (see bo4/unk_45354.c
// func_us_801C5B68); the ammo counter returns the base anim
s32 func_pspeu_0924D108_from_rbo5(void) {
    s16 var_s4;
    s16 animBase;
    s32 playerAnimOffset;
    s32 tapped;

    playerAnimOffset = 0;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        playerAnimOffset = 1;
    }
    tapped = g_Maria.padTapped & (PAD_SQUARE | PAD_CIRCLE);
    animBase = (s16)func_pspeu_0924CF50_from_rbo5();
    if (!animBase) {
        return 1;
    }
    if (animBase < 0) {
        return 0;
    }
    if (g_Maria.unk46 & 0x8000) {
        return 0;
    }
    if (g_Maria.timers[ALU_T_CURSE]) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(57, 1), 0);
        switch (MARIA.step) {
        case Dop_Stand:
        case Dop_Walk:
            SetDopplegangerAnim(0xB5);
            MARIA.step = Dop_Stand;
            break;
        case Dop_Crouch:
            SetDopplegangerAnim(0xB6);
            MARIA.step = Dop_Crouch;
            break;
        case Dop_Fall:
        case Dop_Jump:
            SetDopplegangerAnim(0xB7);
            MARIA.step = Dop_Jump;
            break;
        }
        g_Maria.unk46 = 0x8012;
        g_Maria.unk54 = 0xFF;
        MARIA.step_s = 0x51;
        g_api.PlaySfx(0x7D7);
        return 1;
    }
    if (tapped == PAD_SQUARE) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(48, 0), 0);
        MARIA.step_s = 0x41;
        g_Maria.unk46 = 0x8002;
        g_Maria.unk54 = 0xD;
        animBase = 0x41;
    } else {
        g_Maria.unk46 = 0x8003;
        MARIA.step_s = 0x42;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(48, 1), 0);
        g_Maria.unk54 = 8;
        animBase = 0xA7;
    }
    switch (MARIA.step) {
    case Dop_Stand:
    case Dop_Walk:
        g_CurrentEntity->velocityX = g_CurrentEntity->velocityX >> 1;
        MARIA.step = Dop_Stand;
        var_s4 = playerAnimOffset;
        break;
    case Dop_Crouch:
        var_s4 = 2;
        if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
            var_s4++;
        }
        if (MARIA.step_s == 2) {
            var_s4 = playerAnimOffset;
            MARIA.step = Dop_Stand;
        }
        break;
    case Dop_Fall:
    case Dop_Jump:
        var_s4 = 4;
        if (MARIA.velocityY > 0) {
            var_s4++;
            if (g_Maria.padPressed & PAD_DOWN) {
                var_s4++;
            }
        }
        break;
    }
    SetDopplegangerAnim(animBase + var_s4);
    g_Maria.timers[ALU_T_9] = 4;
    return 1;
}

// destroys the subweapon crash entity and clears the attack state
void func_pspeu_0924D4E8_from_rbo5(void) {
    Entity* entity = &g_Entities[80];

    DestroyEntity(entity);
    g_Maria.unk46 = 0;
}

// backdash; anim 0xDB is the gate the input router checks
void func_pspeu_0924D528_from_rbo5(void) {
    MARIA.step = Dop_Stand;
    MARIA.step_s = 3;
    MarSetSpeedX(FIX(-3.5));
    g_CurrentEntity->velocityY = 0;
    SetDopplegangerAnim(0xDB);
    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
}

#include "../dop_anim.h"

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarDisableAfterImage);

void func_pspeu_0924E770_from_rbo5(void) {
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.resetFlag =
        0;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.disableFlag =
        0;
}

// reduced variant of func_us_801C5430 (see bo4/unk_45354.c): a0 is ignored
void func_pspeu_0924E788_from_rbo5(s16 a0, s16 minTime) {
    if (g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] <= minTime) {
        g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] = minTime;
    }
}

// local copy of DecelerateX (see decelerate.h)
void MarDecelerateX_0925B090(s32 speed) {
    if (g_CurrentEntity->velocityX < 0) {
        g_CurrentEntity->velocityX += speed;
        if (g_CurrentEntity->velocityX > 0) {
            g_CurrentEntity->velocityX = 0;
        }
    } else {
        g_CurrentEntity->velocityX -= speed;
        if (g_CurrentEntity->velocityX < 0) {
            g_CurrentEntity->velocityX = 0;
        }
    }
}

// local copy of DecelerateY (see decelerate.h)
void MarDecelerateX_0925B130(s32 speed) {
    if (g_CurrentEntity->velocityY < 0) {
        g_CurrentEntity->velocityY += speed;
        if (g_CurrentEntity->velocityY > 0) {
            g_CurrentEntity->velocityY = 0;
        }
    } else {
        g_CurrentEntity->velocityY -= speed;
        if (g_CurrentEntity->velocityY < 0) {
            g_CurrentEntity->velocityY = 0;
        }
    }
}

#include "../../mar_check_facing.h"

#include "../../mar_set_speed_x.h"

void func_pspeu_0924EA50_from_rbo5(s32 velocityX) {
    if (MARIA.entityRoomIndex == 1) {
        velocityX = -velocityX;
    }
    MARIA.velocityX = velocityX;
}

// local variant of MarCheckInput (see maria/pl_collision.c): same CHECK_*
// bit layout, but each gravity block writes velocityY directly (no
// accumulator) and the boss falls at 22/128 per frame instead of 28/128
s32 func_pspeu_0924EA98_from_rbo5(s32 checks) {
    if ((checks & 8) && g_Maria.unk46 == 0) {
        MarCheckFacing();
    }
    if (checks & 0x8000) {
        MARIA.velocityY += FIX(22.0 / 128);
        if (MARIA.velocityY > FIX(7)) {
            MARIA.velocityY = FIX(7);
        }
    }
    if (checks & 0x10000) {
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8) && !(g_Maria.unk44 & 0x20) &&
            (g_Maria.padPressed & PAD_CROSS)) {
            // 4.4 is precisely 1/5 of 22, like maria's 5.6 is of 28
            MARIA.velocityY += FIX(4.4 / 128);
        } else {
            MARIA.velocityY += FIX(22.0 / 128);
            if (MARIA.velocityY > FIX(7)) {
                MARIA.velocityY = FIX(7);
            }
        }
    }
    if ((checks & 0x80) && (g_Maria.vram_flag & TOUCHING_CEILING) &&
        (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-1);
    }
    if (checks & 0x200) {
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8)) {
            MARIA.velocityY += FIX(11.0 / 128);
        } else {
            MARIA.velocityY += FIX(22.0 / 128);
            if (MARIA.velocityY > FIX(7)) {
                MARIA.velocityY = FIX(7);
            }
        }
    }
    if (MARIA.velocityY >= 0) {
        if ((checks & 1) && (g_Maria.vram_flag & TOUCHING_GROUND)) {
            if (g_Maria.unk46) {
                if ((g_Maria.unk46 & 0x7FFF) == 0xFF) {
                    func_pspeu_0924CA58_from_rbo5(0);
                    func_pspeu_0924D4E8_from_rbo5();
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                    return 1;
                }
                if (MARIA.velocityY > FIX(6.875)) {
                    func_pspeu_0924C9C8_from_rbo5(1, 0);
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                } else {
                    if (g_Maria.unk44 & 0x10) {
                        func_pspeu_0924CBF0_from_rbo5(1);
                    } else {
                        func_pspeu_0924CA58_from_rbo5(0);
                    }
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                }
                func_pspeu_0924D4E8_from_rbo5();
                return 1;
            } else {
                if (MARIA.velocityY > FIX(6.875)) {
                    if (MARIA.step_s == 0x70 || MARIA.step == 5) {
                        func_pspeu_0924C9C8_from_rbo5(3, MARIA.velocityX / 2);
                    } else {
                        func_pspeu_0924C9C8_from_rbo5(1, 0);
                    }
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                } else if (g_Maria.unk44 & 0x10) {
                    func_pspeu_0924CBF0_from_rbo5(1);
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                } else if (abs(MARIA.velocityX) > FIX(2)) {
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                    func_pspeu_0924CA58_from_rbo5(MARIA.velocityX);
                } else {
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                    func_pspeu_0924CA58_from_rbo5(0);
                }
                return 1;
            }
        } else if ((checks & 0x20000) &&
                   (g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_0924C9C8_from_rbo5(3, MARIA.velocityX);
            g_api.PlaySfx(SFX_STOMP_HARD_B);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
            return 1;
        }
    }
    if ((checks & 4) && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        func_pspeu_0924CE30_from_rbo5();
        return 1;
    }
    if ((checks & 0x1000) &&
        (g_Maria.padTapped & (PAD_SQUARE | PAD_CIRCLE))) {
        if (func_pspeu_0924D108_from_rbo5()) {
            return 1;
        }
    }
    if (!(g_Maria.unk46 & 0x8000)) {
        if ((checks & 0x10) && (g_Maria.padTapped & PAD_CROSS)) {
            func_pspeu_0924CD20_from_rbo5();
            return 1;
        }
        if ((checks & 0x20) && (g_Maria.padTapped & PAD_CROSS) &&
            !(g_Maria.unk44 & 1)) {
            func_pspeu_0924CDE0_from_rbo5();
            return 1;
        }
        if ((checks & 0x2000) && (g_Maria.padPressed & PAD_DOWN)) {
            func_pspeu_0924C9C8_from_rbo5(2, 0);
            return 1;
        }
        if ((checks & 0x40000) && (g_Maria.padTapped & PAD_TRIANGLE) &&
            (MARIA.ext.player.anim != 0xDB)) {
            func_pspeu_0924D528_from_rbo5();
            return 1;
        }
    }
    return 0;
}

// local variant of MarCheckFloor (see maria/pl_collision.c); the boss
// walls drop the g_unkGraphicsStruct gate and the quicksand/water tail
// blocks and add g_Maria.status guards around the sensor probes
void MarCheckFloor(void) {
    Collider col;
    s32 i;
    s32 effects2;
    s32 effects;
    s16 yCheck;
    s16 xCheck;
    s16* x;
    s16* y;
    s32* vramFlag;
    s32 vramApply;
    s16 xMod;
    s32 xMax;
    s16 yMod;

    yMod = 0;
    i = 0;
    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    if (MARIA.velocityY == 0 && g_Maria.unk04 & 1 &&
        !(g_Maria.status & 0x40083)) {
        yMod = 4;
    } else {
        i = 1;
    }
    for (; i < NUM_HORIZONTAL_SENSORS; i++, yMod = 0) {
        if (g_Maria.colFloor[i].effects & EFFECT_SOLID_FROM_BELOW) {
            continue;
        }
        if (!(g_Maria.colFloor[i].effects & EFFECT_UNK_0002 ||
              MARIA.velocityY >= 0 || g_Maria.status & 3 ||
              abs(MARIA.velocityX) >= FIX(2))) {
            continue;
        }
        effects2 = g_Maria.colFloor[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_0800)) {
            if (g_Maria.status & 3 && !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsFloor[i].x;
            yCheck = *y + g_MarSensorsFloor[i].y;
            yCheck += g_Maria.colFloor[i].unk18 - 1;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
            }
            effects = col.effects;
            if (!(effects & EFFECT_SOLID)) {
                if (g_Maria.colFloor[i].effects != EFFECT_SOLID ||
                    MARIA.velocityY >= 0) {
                    if (effects2 & EFFECT_UNK_0800) {
                        *y += yMod + g_Maria.colFloor[i].unk8;
                    } else {
                        *y += yMod + g_Maria.colFloor[i].unk18;
                    }
                    *vramFlag |= 1;
                    return;
                }
                continue;
            }
            if ((effects &
                 (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ==
                (EFFECT_UNK_8000 | EFFECT_SOLID)) {
                if (i < 2) {
                    *vramFlag |= (effects & (EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                                             EFFECT_UNK_1000)) +
                                 (EFFECT_UNK_8000 | EFFECT_SOLID);
                    *y += g_Maria.colFloor[i].unk8 + col.unk18 - 1 + yMod;
                    return;
                }
                if (i == 2 && (effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 |
                                          EFFECT_SOLID)) ==
                                  (EFFECT_UNK_8000 | EFFECT_SOLID)) {
                    g_Maria.colFloor[2].effects = effects;
                    g_Maria.colFloor[2].unk10 = g_Maria.colFloor[2].unk8;
                }
                if (i == 3 &&
                    (effects &
                     (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID)) ==
                        (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID)) {
                    g_Maria.colFloor[3].effects = effects;
                    g_Maria.colFloor[3].unk10 = g_Maria.colFloor[3].unk8;
                }
            }
        }
        if (effects2 == (EFFECT_UNK_8000 | EFFECT_SOLID) && i < 2) {
            *vramFlag |= g_Maria.colFloor[i].effects &
                         (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                          EFFECT_UNK_1000 | EFFECT_SOLID);
            *y += yMod + g_Maria.colFloor[i].unk18;
            return;
        }
    }
    if (MARIA.velocityY < 0) {
        return;
    }
    xCheck = *x + g_MarSensorsFloor[0].x;
    yCheck = *y + g_MarSensorsFloor[0].y + 10;
    g_api.CheckCollision(xCheck, yCheck, &col, 0);
    if ((col.effects & (EFFECT_UNK_8000 | EFFECT_SOLID)) != EFFECT_NONE) {
        return;
    }

    for (i = 2; i < NUM_HORIZONTAL_SENSORS; i++) {
        if ((g_Maria.colFloor[3].effects & EFFECT_UNK_8000) &&
            (g_Maria.colFloor[2].effects & EFFECT_UNK_8000)) {
            return;
        }
        effects2 = g_Maria.colFloor[i].effects;
        vramApply = ((effects2 &
                      (EFFECT_UNK_4000 | EFFECT_UNK_2000 | EFFECT_UNK_1000)) +
                     (EFFECT_UNK_8000 | EFFECT_SOLID));
        if (!(effects2 & EFFECT_UNK_8000)) {
            continue;
        }
        if (i == 2) {
            effects = EFFECT_UNK_4000;
            xMod = g_Maria.colFloor[2].unk4;
            xMax = xMod + 8;
        } else {
            effects = 0;
            xMod = g_Maria.colFloor[3].unkC;
            xMax = 8 - xMod;
        }
        if ((effects2 & EFFECT_UNK_4000) == effects) {
            xCheck = xMod + (*x + g_MarSensorsFloor[i].x);
            yCheck = *y + g_MarSensorsFloor[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (col.effects & EFFECT_SOLID) {
                *y += col.unk18;
                *vramFlag |= vramApply;
                return;
            }
            continue;
        }
        if (xMax <= 0) {
            continue;
        }
        if (!(effects2 & 1)) {
            continue;
        }
        xCheck = *x + g_MarSensorsFloor[i].x + xMod;
        yCheck = *y + g_MarSensorsFloor[i].y + g_Maria.colFloor[i].unk10;
        g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
        if (col.effects & EFFECT_SOLID) {
            *y += (col.unk18 + g_Maria.colFloor[i].unk10);
            *vramFlag |= vramApply;
            return;
        }
    }
}

// local variant of MarCheckCeiling (see maria/pl_collision.c); same
// boss-wall edits as MarCheckFloor, including a status adjust on the
// stale collider at the top of the loop
void MarCheckCeiling(void) {
    Collider col;
    s32 i;
    u32 effects;
    u32 effects2;
    s16 xCheck;
    s16 yCheck;
    s16* x;
    s16* y;
    s32* vramFlag;
    s32 vramApply;
    s16 xMod;
    s32 xMax;

    i = 1;
    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    for (; i < NUM_HORIZONTAL_SENSORS; i++) {
        if (g_Maria.colCeiling[i].effects & EFFECT_SOLID_FROM_ABOVE) {
            continue;
        }
        if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
            col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
        }
        effects2 = g_Maria.colCeiling[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_8000)) {
            if (g_Maria.status & 3 && !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsCeiling[i].x;
            yCheck = *y + g_MarSensorsCeiling[i].y +
                     g_Maria.colCeiling[i].unk10 + 1;
            g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
            if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
            }
            effects = col.effects;
            if (!(effects & EFFECT_SOLID)) {
                if (g_Maria.colCeiling[i].effects != EFFECT_SOLID ||
                    MARIA.velocityY <= 0) {
                    *vramFlag |= 2;
                    if (!(*vramFlag & 1)) {
                        if (g_Maria.colCeiling[i].effects & EFFECT_UNK_8000) {
                            *y += g_Maria.colCeiling[i].unk10;
                        } else {
                            *y += g_Maria.colCeiling[i].unk20;
                        }
                    }
                    return;
                }
                continue;
            }
            if ((effects &
                 (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ==
                (EFFECT_UNK_0800 | EFFECT_SOLID)) {
                if (i < 2) {
                    *vramFlag |=
                        (EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                         ((effects >> 4) & (EFFECT_UNK_0400 | EFFECT_UNK_0200 |
                                            EFFECT_UNK_0100)));
                    if (!(*vramFlag & 1)) {
                        *y += 1 + (g_Maria.colCeiling[i].unk10 + col.unk20);
                    }
                    return;
                }
                if (i == 2 && (effects & (EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                                          EFFECT_SOLID)) ==
                                  (EFFECT_UNK_0800 | EFFECT_SOLID)) {
                    g_Maria.colFloor[2].effects = effects;
                    g_Maria.colFloor[2].unk8 = g_Maria.colFloor[2].unk10;
                }
                if (i == 3 &&
                    (effects &
                     (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
                        (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID)) {
                    g_Maria.colFloor[3].effects = effects;
                    g_Maria.colFloor[3].unk8 = g_Maria.colFloor[3].unk10;
                }
            }
        }
        if ((effects2 == (EFFECT_UNK_0800 | EFFECT_SOLID)) && i < 2) {
            *vramFlag |=
                (EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                 ((g_Maria.colCeiling[i].effects >> 4) &
                  (EFFECT_UNK_0400 | EFFECT_UNK_0200 | EFFECT_UNK_0100)));
            if (!(*vramFlag & 1)) {
                *y += g_Maria.colCeiling[i].unk20;
            }
            return;
        }
    }
    if (MARIA.velocityY > 0) {
        return;
    }
    xCheck = *x + g_MarSensorsCeiling[0].x;
    yCheck = (*y + g_MarSensorsCeiling[0].y) - 10;
    g_api.CheckCollision(xCheck, yCheck, &col, 0);
    if ((col.effects & EFFECT_SOLID) != EFFECT_NONE) {
        return;
    }
    for (i = 2; i < NUM_HORIZONTAL_SENSORS; i++) {
        if ((g_Maria.colCeiling[3].effects & EFFECT_UNK_0800) &&
            (g_Maria.colCeiling[2].effects & EFFECT_UNK_0800)) {
            return;
        }
        effects2 = g_Maria.colCeiling[i].effects;
        vramApply = ((effects2 >> 4) &
                     (EFFECT_UNK_0400 | EFFECT_UNK_0200 | EFFECT_UNK_0100)) +
                    (EFFECT_UNK_0800 | EFFECT_UNK_0002);
        if (!(effects2 & EFFECT_UNK_0800)) {
            continue;
        }
        if (i == 2) {
            effects = EFFECT_UNK_4000;
            xMod = g_Maria.colCeiling[2].unk4;
            xMax = xMod + 8;
        } else {
            effects = 0;
            xMod = g_Maria.colCeiling[3].unkC;
            xMax = 8 - xMod;
        }
        if ((effects2 & EFFECT_UNK_4000) == effects) {
            xCheck = xMod + (*x + g_MarSensorsCeiling[i].x);
            yCheck = *y + g_MarSensorsCeiling[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (col.effects & EFFECT_SOLID) {
                *vramFlag |= vramApply;
                if (!(*vramFlag & 1)) {
                    *y += col.unk20;
                }
                return;
            }
            continue;
        }
        if (xMax <= 0) {
            continue;
        }
        if (!(effects2 & 1)) {
            continue;
        }
        xCheck = *x + g_MarSensorsCeiling[i].x + xMod;
        yCheck = *y + g_MarSensorsCeiling[i].y + g_Maria.colCeiling[i].unk8;
        g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
        if (col.effects & EFFECT_SOLID) {
            if (!(*vramFlag & 1)) {
                *y += col.unk20 + g_Maria.colCeiling[i].unk8;
            }
            *vramFlag |= vramApply;
            return;
        }
    }
}

// local copy of MarCheckWallRight (see maria/pl_collision.c) minus the
// g_unkGraphicsStruct gate
void func_pspeu_09250260_from_rbo5(void) {
    Collider col;
    s32 i;
    s32 effects;
    s16 xCheck;
    s16 yCheck;
    s16* y;
    s16* x;
    s32* vramFlag;

    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    effects =
        g_Maria.unk04 & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                         EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID);
    if ((effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                     EFFECT_SOLID))) {
        *vramFlag |= 4;
        return;
    }

    for (i = 0; i < NUM_VERTICAL_SENSORS; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_UNK_0002 | EFFECT_SOLID);
        if (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_0002 | EFFECT_SOLID)) {
            xCheck = *x + g_MarSensorsWall[i].x + g_Maria.colWall[i].unk4 - 1;
            yCheck = *y + g_MarSensorsWall[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if ((col.effects & EFFECT_SOLID) == EFFECT_NONE) {
                *vramFlag |= 4;
                *x += g_Maria.colWall[i].unk4;
                return;
            }
        }
        if ((effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
                EFFECT_UNK_8000 &&
            (i != 0) &&
            ((g_Maria.colWall[0].effects & EFFECT_UNK_0800) ||
             !(g_Maria.colWall[0].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
            *vramFlag |= 4;
            *x += g_Maria.colWall[i].unk4;
            return;
        }
        if ((effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
                EFFECT_UNK_0800 &&
            (i != 6) &&
            ((g_Maria.colWall[6].effects & EFFECT_UNK_8000) ||
             !(g_Maria.colWall[6].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
            *vramFlag |= 4;
            *x += g_Maria.colWall[i].unk4;
            return;
        }
    }
}

// local copy of MarCheckWallLeft (see maria/pl_collision.c) minus the
// g_unkGraphicsStruct gate
void MarCheckWallLeft(void) {
    Collider col;
    s32 i;
    s32 effects;
    s16 xCheck;
    s16 yCheck;
    s16* y;
    s16* x;
    s32* vramFlag;

    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    effects =
        g_Maria.unk04 & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                         EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID);
    if ((effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                     EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0400 | EFFECT_UNK_0002 |
                     EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                     EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID))) {
        *vramFlag |= 8;
        return;
    }
    for (i = NUM_VERTICAL_SENSORS; i < NUM_VERTICAL_SENSORS * 2; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_UNK_0002 | EFFECT_SOLID);
        if ((effects == (EFFECT_UNK_8000 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0002 | EFFECT_SOLID))) {
            xCheck = *x + g_MarSensorsWall[i].x + g_Maria.colWall[i].unkC + 1;
            yCheck = *y + g_MarSensorsWall[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if ((col.effects & EFFECT_SOLID) == EFFECT_NONE) {
                *vramFlag |= 8;
                *x += g_Maria.colWall[i].unkC;
                return;
            }
        }
        if (((effects &
              (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
             (EFFECT_UNK_8000 | EFFECT_UNK_4000)) &&
            (i != 7) &&
            ((g_Maria.colWall[7].effects & EFFECT_UNK_0800) ||
             !(g_Maria.colWall[7].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
            *vramFlag |= 8;
            *x += g_Maria.colWall[i].unkC;
            return;
        }
        if (((effects &
              (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
             (EFFECT_UNK_4000 | EFFECT_UNK_0800)) &&
            (i != 13) &&
            ((g_Maria.colWall[13].effects & EFFECT_UNK_8000) ||
             !(g_Maria.colWall[13].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
            *vramFlag |= 8;
            *x += g_Maria.colWall[i].unkC;
            return;
        }
    }
}

// local copy of ReboundStoneBounce1 (see rebound_stone.h)
void func_pspeu_092508C0_from_rbo5(s16 bounceAngle) {
    g_CurrentEntity->ext.reboundStone.stoneAngle =
        (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

// local copy of ReboundStoneBounce2 (see rebound_stone.h)
void func_pspeu_09250948_from_rbo5(s16 bounceAngle) {
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.stoneAngle =
            (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092509D0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092514F0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09251748_from_rbo5);
