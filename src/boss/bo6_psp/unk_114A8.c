// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

// Maria fights alongside the player in this overlay; her state mirrors cen.h
#define MARIA g_Entities[STAGE_ENTITY_START]
#define MARIA_AFTERIMAGE_1 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
#define MARIA_AFTERIMAGE_2 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_2]
#define MARIA_AFTERIMAGE_3 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_3]
#define MAR_DRAW_AFTER_IMAGE_SLOT 9

extern PlayerState g_Maria;

extern EInit D_pspeu_0926EB98;            // slide kick hitbox entity init
extern AnimationFrame D_pspeu_0927A418[]; // rising sparkle anim
extern AnimationFrame D_pspeu_0927A5C0[]; // Maria's stop-run anim

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetBladeDash(void);
void MarSetAnimation(AnimationFrame* anim);
void MarDecelerateX(s32 speed);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
bool func_pspeu_0925B680(s32 checks);
void func_pspeu_09259AD8(s32 velocityX);
void func_pspeu_09259D48(void);
void func_pspeu_09259C58(void);
void func_pspeu_0925AA50(void);
void MarSetStep(s16 step);
void MarSetCrouch(s32 kind, s32 velocityX);
void func_maria_8015CC28(void);
void func_pspeu_0924EA50_from_rbo5(s32 velocityX);
s32 func_pspeu_09248D20(
    SubweaponDef* subweapon, s32 isItemCrash, s32 useHearts);
s32 MarCheckSubwpnChainLimit(s16 subwpnId, s16 limit);

// Maria's anim tables live in the raw data blobs of the overlay
extern AnimationFrame D_pspeu_0927A5E0[]; // stand
extern AnimationFrame D_pspeu_0927A6B0[]; // walk
extern AnimationFrame D_pspeu_0927A6D8[]; // crouch from stand 2
extern AnimationFrame D_pspeu_0927A6E8[]; // crouch
extern AnimationFrame D_pspeu_0927A6F0[]; // land from air run
extern AnimationFrame D_pspeu_0927A6F8[]; // crouch from stand
extern AnimationFrame D_pspeu_0927A718[]; // jump neutral
extern AnimationFrame D_pspeu_0927A738[]; // jump moving
extern AnimationFrame D_pspeu_0927A768[]; // fall
extern AnimationFrame D_pspeu_0927A790[]; // slide kick escape jump
extern AnimationFrame D_pspeu_0927A820[]; // subweapon stand
extern AnimationFrame D_pspeu_0927A870[]; // subweapon jump
extern AnimationFrame D_pspeu_0927A8A8[]; // run
extern AnimationFrame D_pspeu_0927A8D0[]; // death
extern AnimationFrame D_pspeu_0927A988[]; // slide
extern AnimationFrame D_pspeu_0927A9B8[]; // slide kick
extern AnimationFrame D_pspeu_0927A9C8[]; // slide kick bounce
extern AnimationFrame D_pspeu_0927A9E0[]; // high jump
extern AnimationFrame D_pspeu_0927AA20[]; // blade dash
extern s32 D_pspeu_0927C760;              // throw dagger timer (raw bss)

// local copy of RicGetFreeEntity (see us_39144.c)
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

// local copy of RicGetFreeEntityReverse (see us_39144.c)
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

extern u8 D_pspeu_092797D0[][4];
extern u8 D_pspeu_0927C230;
extern u8 D_pspeu_0927C228;
extern u8 D_pspeu_0927C220;
extern u8 D_pspeu_0927C218;
// local copy of func_us_801BB314 (see bo6/us_39144.c)
void func_pspeu_09252480_from_rbo5(s32 arg0) {
    D_pspeu_0927C230 = D_pspeu_092797D0[arg0][0];
    D_pspeu_0927C228 = D_pspeu_092797D0[arg0][1];
    D_pspeu_0927C220 = D_pspeu_092797D0[arg0][2];
    D_pspeu_0927C218 = D_pspeu_092797D0[arg0][3];
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09248D20);

extern u8 D_pspeu_09279A40[][8];
s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY) {
    s16 offset;
    s32 ret;
    u8* uvAnim;

    ret = 0;
    uvAnim = D_pspeu_09279A40[0];
    if (prim->b0 >= 6) {
        prim->b0 = 0;
        ret = -1;
    }

    uvAnim = &uvAnim[prim->b0 * 8];

    if (prim->b0 >= 3) {
        offset = 4;
    } else {
        offset = 6;
    }

    prim->x0 = posX - offset;
    prim->y0 = posY - offset;
    prim->x1 = posX + offset;
    prim->y1 = posY - offset;
    prim->x2 = posX - offset;
    prim->y2 = posY + offset;
    prim->x3 = posX + offset;
    prim->y3 = posY + offset;
    prim->u0 = *uvAnim++;
    prim->v0 = *uvAnim++;
    prim->u1 = *uvAnim++;
    prim->v1 = *uvAnim++;
    prim->u2 = *uvAnim++;
    prim->v2 = *uvAnim++;
    prim->u3 = *uvAnim++;
    prim->v3 = *uvAnim;

    prim->b1++;
    if (!(prim->b1 & 1)) {
        prim->b0++;
    }
    return ret;
}

s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY);
extern Point16 D_pspeu_0927C1D8[16];
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
            (s16)g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_0927C1D8));
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        hitboxX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        hitboxY = MARIA.posY.i.hi + MARIA.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < LEN(D_pspeu_0927C1D8); i++) {
            temp_xRand = hitboxX + rand() % 24 - 12;
            temp_yRand = hitboxY + rand() % 48 - 24;
            D_pspeu_0927C1D8[i].x = temp_xRand;
            D_pspeu_0927C1D8[i].y = temp_yRand;
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
            hitboxX = D_pspeu_0927C1D8[i].x;
            hitboxY = D_pspeu_0927C1D8[i].y;
            temp = func_8015FDB0(prim, hitboxX, hitboxY);
            D_pspeu_0927C1D8[i].y--;
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

extern AnimationFrame D_pspeu_09279A70[];
extern s32 D_pspeu_0927C1D0;
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
        if (D_pspeu_0927C1D0 & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        D_pspeu_0927C1D0++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_pspeu_09279A70;
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

// declaring the ignored arg reproduces mwcc's 0x10 stack frame
void Unused09249778(s32 arg0) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09252B48_from_rbo5);

// local copy of RicCreateEntFactoryFromEntity (see us_39144.c); note the
// (68, 80) slot range - cen's Maria uses (72, 80)
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

extern FactoryBlueprint D_pspeu_09279BD0[];
extern u8 D_pspeu_09279D98[][2];
void MarEntityFactory(Entity* self) {
    Entity* newEntity;
    s16 nPerCycle;
    s16 i;
    s16 startIndex;
    s16 endIndex;
    u8* data;

    if (self->step == 0) {
        data = (u8*)&D_pspeu_09279BD0[self->params];
        self->ext.factory.newEntityId = *data++;
        self->ext.factory.amount = *data++;
        self->ext.factory.nPerCycle = *data & 0x3F;
        self->ext.factory.isNonCritical = (s16)(*data >> 7) & 1;
        self->ext.factory.incParamsKind = (s16)(*data++ >> 6) & 1;
        self->ext.factory.tCycle = *data++;
        self->ext.factory.kind = *data & 0x7;
        self->ext.factory.origin = (s16)(*data++ >> 3) & 0x1F;
        self->ext.factory.delay = *data;
        self->flags |= FLAG_UNK_10000000;
        self->step++;
        switch (self->ext.factory.origin) {
        case 0:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        case 4:
            self->flags |= 0;
        case 2:
        case 9:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
        case 3:
        case 7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case 8:
            if (self->ext.factory.parent) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
            }
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        }
    } else {
        switch (self->ext.factory.origin) {
        case 0:
            break;
        case 9:
            if (g_Maria.unk4E) {
                DestroyEntity(self);
                return;
            }
        case 2:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case 4:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != PL_S_RUN) {
                self->entityId = 0;
                return;
            }
            break;
        case 3:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step == PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 8:
            if (self->ext.factory.parent) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
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
    nPerCycle = self->ext.factory.nPerCycle;
    for (i = 0; i < nPerCycle; i++) {
        data = D_pspeu_09279D98[0];
        data += self->ext.factory.kind * 2;
        startIndex = *data++;
        endIndex = *data;
        if (self->ext.factory.kind == 0) {
            newEntity = MarGetFreeEntityReverse(startIndex, endIndex + 1);
        } else if (self->ext.factory.kind == 4) {
            newEntity = &g_Entities[95];
        } else if (self->ext.factory.kind == 5) {
            newEntity = &g_Entities[112];
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
        newEntity->entityId =
            self->ext.factory.newEntityId + self->ext.factory.entityIdMod;
        newEntity->params = self->ext.factory.paramsBase;
        // The child  (newEntity) is not an ent factory, but because the
        // factory creates many entities, we can't pick a particular extension.
        // But we're not allowed to use generic, so i'll just reuse entFactory.
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

extern EInit D_pspeu_0926EB88;
void MarEntitySlideKick(Entity* entity) {
    if (MARIA.step != PL_S_SLIDE) {
        DestroyEntity(entity);
    } else {
        entity->posX.i.hi = MARIA.posX.i.hi;
        entity->posY.i.hi = MARIA.posY.i.hi;
        entity->facingLeft = MARIA.facingLeft;
        if (entity->step == 0) {
            InitializeEntity(D_pspeu_0926EB88);
            entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
            entity->hitboxOffX = 0x14;
            entity->hitboxOffY = 0xC;
            entity->hitboxWidth = entity->hitboxHeight = 9;
            entity->ext.subweapon.timer = entity->hitboxState;
            entity->step = 1;
        }
        entity->hitboxState = entity->ext.subweapon.timer;
        if (MARIA.pose < 2) {
            entity->hitboxState = 0;
        }
        if (MARIA.pose > 7) {
            DestroyEntity(entity);
        }
    }
}

// local copy of func_us_801BC3E0 (see us_39144.c) - slide kick hitbox
void func_pspeu_0924A2E0(Entity* self) {
    if (MARIA.step != PL_S_SLIDE_KICK) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->facingLeft = MARIA.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_pspeu_0926EB98);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0x14;
        self->hitboxWidth = self->hitboxHeight = 9;
        self->step = 1;
    }

    if (MARIA.animCurFrame == 140) {
        self->hitboxOffY = 0;
    }

    if (MARIA.animCurFrame == 141) {
        self->hitboxOffY = 12;
    }

    if (self->hitFlags) {
        g_Maria.unk44 |= 0x80;
    } else {
        g_Maria.unk44 &= ~0x80;
    }
    self->hitFlags = 0;
}

extern EInit D_pspeu_0926EB68;
void func_pspeu_0924A448(Entity* self) {
    if (MARIA.step != PL_S_BLADEDASH) {
        DestroyEntity(self);
    } else {
        self->posX.i.hi = MARIA.posX.i.hi;
        self->posY.i.hi = MARIA.posY.i.hi;
        self->facingLeft = MARIA.facingLeft;
        if (self->step == 0) {
            InitializeEntity(D_pspeu_0926EB68);
            self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
            self->hitboxOffX = self->hitboxOffY = 0;
            self->hitboxWidth = self->hitboxHeight = 20;
            self->hitboxWidth = self->hitboxHeight = 12;
            self->ext.subweapon.subweaponId = 0x11;
            self->step++;
        }
        if (MARIA.pose > 0x12) {
            DestroyEntity(self);
        }
    }
}

extern EInit D_pspeu_0926EB78;
void func_pspeu_0924A578(Entity* self) {
    if (MARIA.step != PL_S_HIGHJUMP) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->facingLeft = MARIA.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_pspeu_0926EB78);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0xC;
        self->hitboxOffY = -0x1A;
        self->hitboxWidth = 12;
        self->hitboxHeight = 12;
        self->step = 1;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A660);

extern unkStr_8011E4BC* D_pspeu_09279ED8[];
extern s32 D_pspeu_0927C308[];
extern s32 D_pspeu_0927C288[];
void MarEntityHitByCutBlood(Entity* self) {
    byte stackpad[0x28];
    u8 thickness;
    s16 variant;
    s16 playerX;
    s16 playerY;
    s16 velocityScale;
    s16 velocityRnd;
    s32 timer;
    s16 x;
    s16 y;
    s16 rnd;
    s32 i;
    unkStr_8011E4BC* props;
    FakePrim* tilePrim;

    variant = (self->params) >> 8;
    props = D_pspeu_09279ED8[variant];
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    playerX = MARIA.posX.i.hi;
    playerY = MARIA.posY.i.hi;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, props->count + 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = props->flags;
        switch (props->unkA) {
        case 8:
            self->ext.et_8011E4BC.unk7C = 0x100;
            break;
        case 7:
            self->ext.et_8011E4BC.unk7C = 0x3F;
            break;
        case 4:
            self->ext.et_8011E4BC.unk7C = 0x2F;
            break;
        case 3:
        case 6:
            self->ext.et_8011E4BC.unk7C = 0x1F;
            break;
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (1) {
            tilePrim->drawMode = props->drawMode;
            tilePrim->priority = MARIA.zPriority + props->priority;
            if (tilePrim->next == NULL) {
                tilePrim->drawMode &= ~DRAW_HIDE;
                tilePrim->y0 = tilePrim->x0 = tilePrim->w = 0;
                break;
            }
            tilePrim->posX.i.hi = x;
            tilePrim->posY.i.hi = y;
            tilePrim->posX.i.lo = tilePrim->posY.i.lo = 0;
            switch (props->unkA) {
            case 0:
                velocityRnd = rand() & PSP_RANDMASK;
                velocityScale = (velocityRnd & 1) + 2;
                tilePrim->velocityX.val = (rcos(velocityRnd) << velocityScale);
                tilePrim->velocityY.val = -(rsin(velocityRnd) << velocityScale);
                break;
            case 1:
                tilePrim->velocityX.val = ((rand() & 0x1FF) - 0x100) << 8;
                tilePrim->velocityY.val = ((rand() & 0x1FF) - 0x100) << 8;
                break;
            case 3:
                tilePrim->posX.i.hi = x + (rand() & 0xF) - 7;
                tilePrim->posY.i.hi = y - (rand() & 0xF);
                tilePrim->velocityY.val = FIX(-0.75) - (rand() & 0x7FFF);
                if (self->ext.et_8011E4BC.parent != NULL) {
                    tilePrim->velocityX.val = self->ext.et_8011E4BC.parent->velocityX;
                }
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 4:
                rnd = rand() & 0x1F;
                tilePrim->posX.i.hi = x + rnd - 0x10;
                rnd = rand() & 0x1F;
                tilePrim->posY.i.hi = y + rnd - 0x14;
                rnd = rand() & 0x1F;
                tilePrim->velocityX.val = D_pspeu_0927C308[rnd];
                tilePrim->velocityY.val = D_pspeu_0927C288[rnd];
                break;
            case 6:
                tilePrim->posX.i.hi = self->posX.i.hi + (rand() & 0xF) - 8;
                tilePrim->posY.i.hi = self->posY.i.hi + (rand() & 0xF) - 4;
                tilePrim->velocityY.val = (rand() & PSP_RANDMASK) + FIX(0.5);
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 7:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                if (i < 10) {
                    tilePrim->velocityY.val =
                        -((i * i * FIX(0.09375)) + FIX(0.125));
                } else {
                    tilePrim->velocityY.val =
                        (i - 10) * (i - 10) * FIX(0.09375) + FIX(0.125);
                }
                tilePrim->delay = 0x3F;
                break;
            case 8:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                tilePrim->velocityX.val = ((rand() & 0x7FFF) - FIX(0.25)) >> 1;

                tilePrim->velocityY.val = -((rand() & 0x1FFF) + FIX(0.375));
                tilePrim->timer = (i * 4);
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            tilePrim->r0 = props->r;
            tilePrim->g0 = props->g;
            tilePrim->b0 = props->b;
            tilePrim->w = props->w;
            tilePrim->h = props->h;
            i++;
            tilePrim = tilePrim->next;
        }
        self->step++;
        break;
    case 1:
        switch (props->unkA) {
        case 3:
        case 6:
        case 7:
        case 8:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
        case 4:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
            thickness = 3;
            timer = self->ext.et_8011E4BC.unk7C;
            if (timer < 9) {
                thickness--;
            }
            if (timer < 4) {
                thickness--;
            }
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (1) {
            if (tilePrim->next == NULL) {
                tilePrim->drawMode &= ~DRAW_HIDE;
                tilePrim->y0 = tilePrim->x0 = tilePrim->w = 0;
                return;
            }
            tilePrim->posX.i.hi = tilePrim->x0;
            tilePrim->posY.i.hi = tilePrim->y0;
            switch (props->unkA) {
            case 0:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->r0 -= 6;
                tilePrim->g0 -= 6;
                tilePrim->b0 -= 6;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 1:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->velocityY.val =
                    (tilePrim->velocityY.val + FIX(0.15625));
                tilePrim->r0 -= 3;
                tilePrim->g0 -= 3;
                tilePrim->b0 -= 3;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 3:
            case 6:
            case 7:
                tilePrim->posY.val += tilePrim->velocityY.val;
                if (--tilePrim->delay < 0) {
                    tilePrim->drawMode |= DRAW_HIDE;
                }
                break;
            case 4:
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->posY.val += tilePrim->velocityY.val;
                // There is probably a clever way to write this
                tilePrim->velocityY.val -= tilePrim->velocityY.val >> 5;
                if (!(timer & 7)) {
                    tilePrim->velocityX.val >>= 1;
                    tilePrim->velocityY.val >>= 1;
                    if (timer & 0x20) {
                        tilePrim->velocityY.val >>= 1;
                    }
                    if (timer == 0x18) {
                        tilePrim->drawMode = DRAW_UNK02;
                    }
                }
                tilePrim->w = tilePrim->h = thickness;
                break;
            case 8:
                if (tilePrim->timer == 0) {
                    tilePrim->drawMode &= ~DRAW_HIDE;
                    tilePrim->r0 -= 1;
                    tilePrim->g0 -= 1;
                    tilePrim->b0 -= 1;
                    tilePrim->posY.val += tilePrim->velocityY.val;
                    tilePrim->posX.val += tilePrim->velocityX.val;
                    if (g_unkGraphicsStruct.D_80097448 == 0 ||
                        !(tilePrim->posY.i.hi >
                          (MARIA.posY.i.hi - g_unkGraphicsStruct.D_80097448 +
                           0x19))) {
                        tilePrim->drawMode |= DRAW_HIDE;
                    }
                } else {
                    tilePrim->timer--;
                }
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            i++;
            tilePrim = tilePrim->next;
        }
    }
}

extern AnimationFrame D_pspeu_09279F40[];
extern AnimationFrame D_pspeu_09279F00[];
extern AnimationFrame D_pspeu_09279DE0[];
void func_maria_80161C2C(Entity* self) {
    s16 paramsHi;
    s16 paramsLo;

    paramsLo = self->params & 0xFF;
    paramsHi = (self->params >> 8) & 0xFF;
    switch (self->step) {
    case 0:
        if (paramsHi == 1) {
            self->scaleX = 0xC0;
            self->scaleY = 0xC0;
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->animSet = ANIMSET_DRA(2);
            self->anim = D_pspeu_09279F40;
        }
        if (paramsHi == 0 || paramsHi == 2) {
            if (paramsLo & 3) {
                self->anim = D_pspeu_09279F00;
                self->scaleX = 0x120;
                self->scaleY = 0x120;
                self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
                self->animSet = ANIMSET_DRA(2);
            } else {
                self->animSet = ANIMSET_DRA(5);
                self->anim = D_pspeu_09279DE0;
                self->palette = PAL_FLAG(PAL_UNK_170);
            }
        }
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        if (rand() & 3) {
            self->zPriority = MARIA.zPriority + 2;
        } else {
            self->zPriority = MARIA.zPriority - 2;
        }
        if (paramsHi == 2) {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() % 44) - 22;
        } else {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() & 15) - 8;
        }
        self->posY.i.hi =
            MARIA.posY.i.hi + MARIA.hitboxOffY + (rand() & 31) - 16;
        self->velocityY = FIX(-0.5);
        self->velocityX = MARIA.velocityX >> 2;
        self->step++;
        break;
    case 1:
        self->scaleX -= 4;
        self->scaleY -= 4;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if ((self->pose == 8) && (self->anim != D_pspeu_09279DE0)) {
            self->blendMode = BLEND_TRANSP;
            if (!(paramsLo & 1) && self->poseTimer == 1) {
                MarCreateEntFactoryFromEntity(self, FACTORY(4, 4), 0);
            }
        }
        if (self->pose == 16 && self->anim == D_pspeu_09279DE0) {
            self->blendMode = BLEND_TRANSP;
        }
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern AnimationFrame D_pspeu_09279F78[];
void func_pspeu_0924B8D0(Entity* self) {
    switch (self->step) {
    case 0:
        self->animSet = ANIMSET_DRA(2);
        self->anim = D_pspeu_09279F78;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->zPriority = MARIA.zPriority + 4;
        self->velocityY = (rand() & 0x3FFF) - 0x10000;
        self->step++;
        break;
    case 1:
        if (self->pose == 6) {
            if (self->poseTimer == 1 && (rand() & 1)) {
                MarCreateEntFactoryFromEntity(self, 4, 0);
            }
        }
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

// local copy of func_us_801BD47C (see us_39144.c) - true when another
// live entity shares this id and params
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

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityPlayerBlinkWhite);

void func_pspeu_0924D7F0(Entity* self) {
    Primitive* prim;
    s16 primIndex;

    self->posX.i.hi = MARIA.posX.i.hi - 0;
    self->posY.i.hi = MARIA.posY.i.hi - 8;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.circleExpand.width = 16;
        self->ext.circleExpand.height = 12;
        prim = &g_PrimBuf[self->primIndex];
        prim->u0 = prim->u2 = 64;
        prim->v0 = prim->v1 = 192;
        prim->u1 = prim->u3 = 127;
        prim->v2 = prim->v3 = 255;
        PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) = 128;
        prim->tpage = 0x1A;
        prim->clut = PAL_CC_FIRE_EFFECT;
        prim->priority = MARIA.zPriority + 8;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->step++;
        break;

    case 1:
        self->ext.circleExpand.width += 2;
        self->ext.circleExpand.height += 2;
        if (self->ext.circleExpand.width > 56) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y0 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x1 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y1 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x2 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y2 = self->posY.i.hi + self->ext.circleExpand.height;
    prim->x3 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y3 = self->posY.i.hi + self->ext.circleExpand.height;
    if (prim->b3 >= 12) {
        prim->b3 -= 12;
    }
    // remember last element of PGREY(prim,3) is prim->b3
    PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3);
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityShrinkingPowerUpRing);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByIce);

extern u16 D_pspeu_0927A3B8[];
void MarEntityHitByLightning(Entity* self) {
    s16 x;
    s16 y;
    s16 mul2;
    s32 i;
    s32 xMod;
    s32 yMod;
    bool terminate;
    s32 mul;
    s16 angle;
    Primitive* primPrev;
    Primitive* prim;

    terminate = false;
    if ((self->params & 0xFF00) == 0x100) {
        if ((++self->ext.hitbylightning.unk9C) > 0xA8) {
            terminate = true;
        }
    } else if ((self->params & 0xFF00) == 0x200) {
        if (++self->ext.hitbylightning.unk9C > 0x90) {
            terminate = true;
        }
    } else if (MARIA.step != PL_S_HIT) {
        terminate = true;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }

        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x3F);
        self->ext.hitbylightning.unk80 = (rand() & PSP_RANDMASK);
        self->ext.hitbylightning.unk82 = (rand() & 0x1FF) + 0x100;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi;
            prim->tpage = 0x1A;
            prim->clut = D_pspeu_0927A3B8[rand() & 1];
            prim->priority = MARIA.zPriority - 2;
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim = prim->next;
        }
        if ((MARIA.velocityY != 0) && (MARIA.step != PL_S_DEAD)) {
            self->ext.hitbylightning.unk92 = 1;
        }
        self->ext.hitbylightning.unk94 = 0x10;
        g_api.PlaySfx(SFX_PENTAGRAM_ATTACK);
        self->step++;
        break;
    case 1:
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x1FF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) * 12;
        yMod = (-((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) * 7) << 1;
        self->posX.val = xMod + MARIA.posX.val;
        self->posY.val = yMod + MARIA.posY.val;
        if (self->ext.hitbylightning.unk92 &&
            g_Maria.vram_flag &
                (TOUCHING_L_WALL | TOUCHING_R_WALL | TOUCHING_CEILING)) {
            terminate = true;
        }
        if (terminate) {
            self->ext.hitbylightning.unk90 = (rand() & 0xF) + 0x10;
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbylightning.unk90 == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.hitbylightning.unk94 > 0) {
            self->ext.hitbylightning.unk94--;
        }
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0xFF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) *
               ((rand() % 8) + 8);
        yMod = -((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) *
                   ((rand() % 8) + 10) +
               self->ext.hitbylightning.unk98;
        self->posX.val = MARIA.posX.val + xMod;
        self->posY.val = MARIA.posY.val + yMod;
        self->ext.hitbylightning.unk98 -= 0x8000;
        prim = &g_PrimBuf[self->primIndex];
        break;
    }

    x = (self->posX.i.hi + (rand() & 7)) - 4;
    y = (self->posY.i.hi + (rand() & 0x1F)) - 0x18;
    mul2 = self->ext.hitbylightning.unk94;
    mul2 = (mul2 * rsin(self->ext.hitbylightning.unk80)) >> 0xC;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 5; i++) {
        primPrev = prim;
        prim = prim->next;
        *primPrev = *prim;
        primPrev->next = prim;
        primPrev->u0 = primPrev->u2 = (i * 0x10) + 0x90;
        primPrev->u1 = primPrev->u3 = ((i + 1) * 0x10) + 0x90;
        primPrev->v0 = primPrev->v1 = 0xC0;
        primPrev->v2 = primPrev->v3 = 0xCF;
    }
    prim->x0 = prim->x1;
    prim->y0 = prim->y1;
    prim->x2 = prim->x3;
    prim->y2 = prim->y3;
    angle = self->ext.hitbylightning.unk7C + 0x400;
    prim->x1 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y1 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk7C - 0x400;
    prim->x3 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y3 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk80 & 0xFFF;
    if (angle < 0x400) {
        prim->priority = MARIA.zPriority + 2;
    } else if (angle < 0xC00) {
        prim->priority = MARIA.zPriority - 2;
    } else {
        prim->priority = MARIA.zPriority + 2;
    }
    prim->u0 = prim->u2 = (i << 4) + 0x90;
    prim->u1 = prim->u3 = ((i + 1) << 4) + 0x90;
    prim->v0 = prim->v1 = 0xC0;
    prim->v2 = prim->v3 = 0xCF;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924FFB8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09250DA8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251100);

// local copy of func_us_801C13A8 (see us_3E79C.c) - rising sparkle
void func_pspeu_092516D0(Entity* self) {
    s16 params = self->params & 0x7F00;
    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->unk5A = 0x79;
        self->animSet = ANIMSET_DRA(14);
        self->zPriority = MARIA.zPriority + 6;
        self->palette = PAL_FLAG(0x25E);
        self->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0xC0;
        self->anim = D_pspeu_0927A418;
        if (params) {
            self->scaleX = self->scaleY = 0x80;
            self->anim = D_pspeu_0927A418;
        }
        self->velocityY = -FIX(0.25);
        self->step++;
        break;

    case 1:
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern u8 D_pspeu_0927A450[];
extern EInit D_pspeu_0926EBD8;
void func_pspeu_09251850(Entity* self) {
    u8 rVal;
    u8 gVal;
    u8 bVal;
    s16 xVar;
    s16 yVar;
    s32 rgbIdx;
    s16 angle0;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angleOffset;
    s32 twentyone;
    Primitive* prevPrim;
    s32 i;
    Primitive* prim;
    s32 graphicsTemp;

    twentyone = 21;

    switch (self->step) {
    case 0:
        self->ext.subwpnAxe.subweaponId = 2;
        InitializeEntity(D_pspeu_0926EBD8);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 5);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_UNK_10000000 |
                      FLAG_HAS_PRIMS;
        self->facingLeft = (MARIA.facingLeft + 1) & 1;
        MarSetSpeedX(FIX(-2));
        self->velocityY = FIX(-6);
        if (self->facingLeft) {
            self->ext.subwpnAxe.unk7C = 0x400;
        } else {
            self->ext.subwpnAxe.unk7C = 0xC00;
        }

        self->posY.i.hi -= 12;
        for (prim = &g_PrimBuf[self->primIndex], i = 0; prim != NULL; i++,
            prim = prim->next) {
            prim->tpage = 0x1C;
            prim->u0 = 0;
            prim->v0 = 0;
            prim->u1 = 0x18;
            prim->v1 = 0;
            prim->u2 = 0;
            prim->v2 = 0x28;
            prim->u3 = 0x18;
            prim->v3 = 0x28;
            prim->priority = MARIA.zPriority - 2;
            if (i != 0) {
                prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                                 DRAW_HIDE | DRAW_COLORS | DRAW_TRANSP;
                self->ext.subwpnAxe.unk8C[i - 1] = 0;
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                self->ext.subwpnAxe.unk94[i - 1] = 0;
            } else {
                prim->drawMode = DRAW_UNK_100 | DRAW_HIDE;
            }
        }
        self->hitboxWidth = 12;
        self->hitboxHeight = 12;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->ext.subwpnAxe.unk98 = 0x7F;
        self->step = 1;
        break;
    case 1:
        if (self->facingLeft) {
            angleOffset = -0x80;
        } else {
            angleOffset = 0x80;
        }
        self->ext.subwpnAxe.unk7C += angleOffset;
        if (!(self->ext.subwpnAxe.unk7C & 0x3FF)) {
            g_api.PlaySfxVolPan(
                SFX_WEAPON_SWISH_C, self->ext.subwpnAxe.unk98, 0);
            self->ext.subwpnAxe.unk98 -= 8;
            if (self->ext.subwpnAxe.unk98 < 0) {
                self->ext.subwpnAxe.unk98 = 0;
            }
        }
        self->velocityY += FIX(34.0 / 128);
        if (self->velocityY > FIX(8)) {
            self->velocityY = FIX(8);
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        if (self->posY.i.hi > 0x100) {
            DestroyEntity(self);
            return;
        }
        if (self->hitFlags == 2 || self->flags & FLAG_DEAD) {
            self->hitboxState = 0;
            self->velocityX = -(self->velocityX / 2);
            self->velocityY = FIX(-3);
            self->step = 2;
        }
        break;
    case 2:
        if (self->facingLeft) {
            angleOffset = 0xC0;
        } else {
            angleOffset = -0xC0;
        }
        self->ext.subwpnAxe.unk7C += angleOffset;
        self->velocityY += FIX(18.0 / 128);
        if (self->velocityY > FIX(8)) {
            self->velocityY = FIX(8);
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        if (self->posY.i.hi > 0x100) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    if (self->poseTimer == 0) {
        graphicsTemp = self->pose;
        self->ext.subwpnAxe.unk8C[graphicsTemp] = 0;
        self->ext.subwpnAxe.unk90[graphicsTemp] = 1;
        self->ext.subwpnAxe.unk94[graphicsTemp] = 1;
        graphicsTemp++;
        graphicsTemp &= 3;
        self->pose = graphicsTemp;
        self->poseTimer = 2;
    } else {
        self->poseTimer--;
    }
    for (prim = &g_PrimBuf[self->primIndex], prevPrim = prim,
        graphicsTemp = ((g_GameTimer >> 1) & 1) + 0x1AB, i = 0;
         prim != NULL; i++, prim = prim->next) {
        prim->clut = graphicsTemp;
        if (i == 0) {
            if (self->facingLeft) {
                angle0 = 0x560;
                angle1 = 0x2A0;
                angle2 = 0xAA0;
                angle3 = 0xD60;
            } else {
                angle1 = 0x560;
                angle0 = 0x2A0;
                angle3 = 0xAA0;
                angle2 = 0xD60;
            }

            xVar = self->posX.i.hi;
            yVar = self->posY.i.hi;
            angleOffset = self->ext.subwpnAxe.unk7C;
            angle0 += angleOffset;
            angle1 += angleOffset;
            angle2 += angleOffset;
            angle3 += angleOffset;
            prim->x0 = xVar + (((rcos(angle0) << 4) * twentyone) >> 0x10);
            prim->y0 = yVar + -(((rsin(angle0) << 4) * twentyone) >> 0x10);
            prim->x1 = xVar + (((rcos(angle1) << 4) * twentyone) >> 0x10);
            prim->y1 = yVar + -(((rsin(angle1) << 4) * twentyone) >> 0x10);
            prim->x2 = xVar + (((rcos(angle2) << 4) * twentyone) >> 0x10);
            prim->y2 = yVar + -(((rsin(angle2) << 4) * twentyone) >> 0x10);
            prim->x3 = xVar + (((rcos(angle3) << 4) * twentyone) >> 0x10);
            prim->y3 = yVar + -(((rsin(angle3) << 4) * twentyone) >> 0x10);
            prim->drawMode &= ~DRAW_HIDE;
        } else if (self->ext.subwpnAxe.unk90[i - 1]) {
            if (self->ext.subwpnAxe.unk94[i - 1]) {
                self->ext.subwpnAxe.unk94[i - 1] = 0;
                prim->x0 = prevPrim->x0;
                prim->y0 = prevPrim->y0;
                prim->x1 = prevPrim->x1;
                prim->y1 = prevPrim->y1;
                prim->x2 = prevPrim->x2;
                prim->y2 = prevPrim->y2;
                prim->x3 = prevPrim->x3;
                prim->y3 = prevPrim->y3;
            }
            rgbIdx = self->ext.subwpnAxe.unk8C[i - 1]++;
            if (rgbIdx < 10) {
                rVal = D_pspeu_0927A450[rgbIdx * 4 + 0];
                gVal = D_pspeu_0927A450[rgbIdx * 4 + 1];
                bVal = D_pspeu_0927A450[rgbIdx * 4 + 2];
                prim->r0 = rVal;
                prim->g0 = gVal;
                prim->b0 = bVal;
                prim->r1 = rVal;
                prim->g1 = gVal;
                prim->b1 = bVal;
                prim->r2 = rVal;
                prim->g2 = gVal;
                prim->b2 = bVal;
                prim->r3 = rVal;
                prim->g3 = gVal;
                prim->b3 = bVal;
                prim->drawMode &= ~DRAW_HIDE;
            } else {
                self->ext.subwpnAxe.unk90[i - 1] = 0;
                prim->drawMode |= DRAW_HIDE;
            }
        }
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092522A8);

extern s16 D_pspeu_0927A478[];
extern s32 D_pspeu_0927C240;
void func_pspeu_09251748_from_rbo5(Entity* self) {
    Collider collider;
    Primitive* prim;
    s16 offsetX;
    s16 offsetY;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angle4;
    s16 x;
    s16 y;
    s16 xCol;
    s32 modX;
    s32 modY;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->facingLeft = MARIA.facingLeft;
        // Not sure what this unkB0 does, but it seems to be
        // a standard part of the Ext, and may not be entity specific.
        // This line is not in the DRA version of dagger.

        self->hitboxWidth = 4;
        self->hitboxHeight = 2;
        self->hitboxOffX = 4;
        self->hitboxOffY = 0;
        // This conditional block is very different from DRA
        if (self->params & 0xFF00) {
            self->posY.i.hi += D_pspeu_0927A478[D_pspeu_0927C240 & 7];
            D_pspeu_0927C240++;
        } else {
            self->posY.i.hi -= 9;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1C;
        prim->clut = PAL_UNK_1AB;
        prim->u0 = prim->u1 = 0x18;
        prim->v0 = prim->v2 = 0x18;
        prim->u2 = prim->u3 = 0x20;
        prim->v1 = prim->v3 = 0;
        prim->priority = MARIA.zPriority + 2;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;

        prim = prim->next;
        prim->type = PRIM_LINE_G2;
        prim->priority = MARIA.zPriority + 2;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
        prim->r0 = 0x7F;
        prim->g0 = 0x3F;
        prim->b0 = 0;
        MarSetSpeedX(FIX(8));
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        self->ext.subweapon.timer++;
        if (self->velocityX > 0) {
            xCol = 8;
        }
        if (self->velocityX < 0) {
            xCol = -8;
        }
        if (self->hitFlags == 1) {
            self->ext.subweapon.timer = 4;
            self->step = 3;
            self->hitboxState = 0;
            return;
        }
        for (i = 0; i < 8; i++) {
            if (self->velocityX > 0) {
                self->posX.i.hi++;
            }
            if (self->velocityX < 0) {
                self->posX.i.hi--;
            }
            g_api.CheckCollision(
                self->posX.i.hi + xCol, self->posY.i.hi, &collider, 0);
            if ((self->hitFlags == 2) || (collider.effects & EFFECT_BLOCK)) {
                self->ext.subweapon.timer = 64;
                self->velocityX = -(self->velocityX >> 3);
                self->velocityY = FIX(-2.5);
                self->hitboxState = 0;
                self->posX.i.hi += xCol;
                MarCreateEntFactoryFromEntity(self, FACTORY(42, 2), 0);
                self->posX.i.hi -= xCol;
                g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
                self->step++;
                return;
            }
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        offsetX = 12;
        offsetY = 8;
        if (self->facingLeft) {
            offsetX = -offsetX;
            offsetY = -offsetY;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = x - offsetX;
        prim->y0 = y - 4;
        prim->x1 = x + offsetX;
        prim->y1 = y - 4;
        prim->x2 = x - offsetX;
        prim->y2 = y + 4;
        prim->x3 = x + offsetX;
        prim->y3 = y + 4;
        // Difference here vs DRA in how g_GameTimer works with clut
        prim->clut = ((g_GameTimer >> 1) & 1) + 0x1AB;
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->x0 = x - offsetY;
        prim->y0 = y - 1;
        prim->x1 = x - (offsetX * (self->ext.subweapon.timer / 2));
        prim->y1 = y - 1;
        prim->drawMode &= ~DRAW_HIDE;
        if (self->step != 1) {
            prim->drawMode |= DRAW_HIDE;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.subweapon.timer == 0x20) {
            prim->drawMode |=
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) =
                0x60;
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY += FIX(0.125);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        offsetX = 12;
        if (self->facingLeft == 0) {
            angle1 = 0x800 - 0xD2;
            angle2 = 0xD2;
            angle3 = 0x800 + 0xD2;
            angle4 = -0xD2;
            self->rotate -= ROT(11.25);
        } else {
            angle2 = 0x800 - 0xD2;
            angle1 = 0xD2;
            angle4 = 0x800 + 0xD2;
            angle3 = -0xD2;
            self->rotate += ROT(11.25);
        }
        angle1 += self->rotate;
        angle2 += self->rotate;
        angle3 += self->rotate;
        angle4 += self->rotate;
        // offsetX is not used at all down here, but this block is needed.
        // It doesn't show up in the asm at all, but moves everything else into
        // alignment. Yet another lesson from PSP.
        if (self->facingLeft) {
            offsetX = -offsetX;
        }
        prim = &g_PrimBuf[self->primIndex];
        modX = (rcos(angle1) * 0xCA0) >> 0x14;
        modY = -(rsin(angle1) * 0xCA0) >> 0x14;
        prim->x0 = x + (s16)modX;
        prim->y0 = y - (s16)modY;
        modX = (rcos(angle2) * 0xCA0) >> 0x14;
        modY = -(rsin(angle2) * 0xCA0) >> 0x14;
        prim->x1 = x + (s16)modX;
        prim->y1 = y - (s16)modY;
        modX = (rcos(angle3) * 0xCA0) >> 0x14;
        modY = -(rsin(angle3) * 0xCA0) >> 0x14;
        prim->x2 = x + (s16)modX;
        prim->y2 = y - (s16)modY;
        modX = (rcos(angle4) * 0xCA0) >> 0x14;
        modY = -(rsin(angle4) * 0xCA0) >> 0x14;
        prim->x3 = x + (s16)modX;
        prim->y3 = y - (s16)modY;
        // same deal here with g_GameTimer
        prim->clut = ((g_GameTimer >> 1) & 1) + 0x1AB;
        if (self->ext.subweapon.timer < 0x21) {
            prim->r0 -= 2;
            prim->g0 = prim->b0 = PGREY(prim, 1) = PGREY(prim, 2) =
                PGREY(prim, 3) = prim->r0;
        }
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->drawMode |= DRAW_HIDE;
        break;
    case 3:
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253AA8);

// local copy of func_us_801B4EAC (see richter.c) - integrate velocity
// and clamp to the arena walls
void func_pspeu_09253E08(void) {
    g_Maria.unk04 = g_Maria.vram_flag;
    g_Maria.vram_flag = 0;
    MARIA.posY.val += MARIA.velocityY;
    MARIA.posX.val += MARIA.velocityX;

    if (MARIA.posY.val >= 0xB30000) {
        MARIA.posY.val = 0xB30000;
        g_Maria.vram_flag |= TOUCHING_GROUND;
    }
    if (MARIA.posY.val <= 0x280000) {
        MARIA.posY.val = 0x280000;
        g_Maria.vram_flag |= TOUCHING_CEILING;
    }
    if (MARIA.posX.val >= 0xF80000) {
        MARIA.posX.val = 0xF80000;
        g_Maria.vram_flag |= TOUCHING_R_WALL;
    }
    if (MARIA.posX.val <= 0x80000) {
        MARIA.posX.val = 0x80000;
        g_Maria.vram_flag |= TOUCHING_L_WALL;
    }
}

// local copy of static CheckBladeDashInput (see richter.c); non-static
// so the remaining INCLUDE_ASM stubs can reference the symbol
void func_pspeu_09253F48(void) {
    if ((MARIA.step == 1 || MARIA.step == 2 || MARIA.step == 3 ||
         MARIA.step == 5 || MARIA.step == 4) &&
        !g_Maria.unk46 && (g_Maria.padTapped & PAD_R1)) {
        MarSetBladeDash();
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09254008);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092540C8);

extern s32 D_pspeu_0927C278;
extern s32 D_pspeu_0927C280;
// local copy of func_us_801B5A14 (see bo6/richter.c): set the boss
// think-step and reset its timer
void func_pspeu_09247190_from_rbo5(s32 step) {
    D_pspeu_0927C280 = step;
    D_pspeu_0927C278 = 0;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09254F38);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09256610);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09256778);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092568D0);

// local copy of RicStepWalk (see ric/pl_steps.c) driving Maria
void func_pspeu_09256BA0(void) {
    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP |
                             CHECK_CRASH | CHECK_ATTACK | CHECK_CROUCH)) {
        MarDecelerateX(FIX(0.125));
        if (MarCheckFacing() == 0) {
            func_pspeu_09259AD8(0);
            return;
        }
        if (MARIA.step_s != 0) {
            if (MARIA.step_s) {
            }
        } else {
            MarSetSpeedX(FIX(1.25));
        }
    }
}

// local copy of RicStepRun (see ric/pl_steps.c) driving Maria
void func_pspeu_09256C28(void) {
    g_Maria.timers[PL_T_8] = 8;
    g_Maria.timers[PL_T_CURSE] = 8;
    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP |
                             CHECK_CRASH | CHECK_ATTACK | CHECK_CROUCH)) {
        MarDecelerateX(FIX(0.125));
        if (MarCheckFacing() == 0) {
            func_pspeu_09259AD8(0);
            if (g_Maria.timers[PL_T_RUN] == 0) {
                if (!(g_Maria.vram_flag &
                      (TOUCHING_L_WALL | TOUCHING_R_WALL))) {
                    MarSetAnimation(D_pspeu_0927A5C0);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                }
            } else {
                MARIA.velocityX = 0;
            }
            return;
        }
        if (MARIA.step_s != 0) {
            if (MARIA.step_s) {
            }
        } else {
            MarSetSpeedX(FIX(2.25));
        }
    }
}

extern AnimationFrame D_pspeu_0927A980[];
extern AnimationFrame D_pspeu_0927A758[];
void MarDisableAfterImage(s32 resetAnims, s32 arg1);
void func_pspeu_09256D28(void) {
    s32 facing;

    if ((g_Maria.vram_flag & TOUCHING_CEILING) &&
        (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-0.25);
        g_Maria.unk44 |= 0x20;
    }
    if (func_pspeu_0925B680(
            CHECK_GROUND | CHECK_FACING | CHECK_ATTACK | CHECK_GRAVITY_JUMP)) {
        return;
    }
    switch (MARIA.step_s) {
    case 0:
        MarDecelerateX(FIX(0.0625));
        facing = MarCheckFacing();
        if (facing) {
            if (g_Maria.unk44 & 0x10) {
                MarSetSpeedX(FIX(2.25));
            } else {
                MarSetSpeedX(FIX(1.25));
            }
            g_Maria.unk44 &= ~4;
        } else {
            g_Maria.unk44 &= ~0x10;
            if ((MARIA.pose < 2) && !(g_Maria.unk44 & 8) &&
                (g_Maria.unk44 & 4) && (g_Maria.padTapped & PAD_CROSS)) {
                MarSetAnimation(D_pspeu_0927A790);
                MarSetSpeedX(FIX(-1.5));
                MARIA.velocityY = FIX(-2.625);
                g_Maria.unk44 |= 0xA;
                g_Maria.unk44 &= ~4;
                MARIA.step_s = 2;
            }
        }
        break;
    case 2:
        break;
    case 0x40:
        MarDisableAfterImage(1, 1);
        if (MARIA.pose < 3) {
            facing = MarCheckFacing();
            if (facing) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        } else {
            if (((g_Maria.padPressed & PAD_RIGHT) && MARIA.facingLeft == 0) ||
                ((g_Maria.padPressed & PAD_LEFT) && MARIA.facingLeft)) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                MarSetSpeedX(FIX(1.25));
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        }
        if (MARIA.poseTimer < 0) {
            if (g_Maria.padPressed & PAD_SQUARE) {
                MARIA.step_s += 1;
                g_Maria.unk46 = 2;
                MarSetAnimation(D_pspeu_0927A980);
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, BP_ARM_BRANDISH_WHIP, 0);
            } else {
                g_Maria.unk46 = 0;
                MARIA.step_s = 0;
                MarSetAnimation(D_pspeu_0927A758);
            }
        }
        break;
    case 0x41:
        MarDisableAfterImage(1, 1);
        if (!(g_Maria.padPressed & PAD_SQUARE)) {
            g_Maria.unk46 = 0;
            MARIA.step_s = 0;
            MarSetAnimation(D_pspeu_0927A758);
        }
        break;
    case 0x42:
        MarDisableAfterImage(1, 1);
        if (MARIA.pose < 3) {
            facing = MarCheckFacing();
            if (facing) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        } else {
            if (((g_Maria.padPressed & PAD_RIGHT) && MARIA.facingLeft == 0) ||
                ((g_Maria.padPressed & PAD_LEFT) && MARIA.facingLeft)) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                MarSetSpeedX(FIX(1.25));
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        }
        if (MARIA.poseTimer < 0) {
            g_Maria.unk46 = 0;
            MARIA.step_s = 0;
            MarSetAnimation(D_pspeu_0927A758);
        }
        break;
    }
}

// local copy of RicStepFall (see boss/bo6/richter.c)
void MarStepFall(void) {
    if (func_pspeu_0925B680(
            CHECK_GROUND | CHECK_FACING | CHECK_ATTACK | CHECK_GRAVITY_FALL)) {
        return;
    }
    MarDecelerateX(FIX(1. / 16));
    switch (MARIA.step_s) {
    case 0:
        if (g_Maria.timers[PL_T_5] && g_Maria.padTapped & PAD_CROSS) {
            func_pspeu_09259C58();
        } else if (MarCheckFacing()) {
            MarSetSpeedX(FIX(0.75));
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257418);

void MarResetPose(void) {
    MARIA.pose = MARIA.poseTimer = 0;
    g_Maria.unk44 = 0;
    g_Maria.unk46 = 0;
    MARIA.drawFlags &= ~ENTITY_ROTATE;
}

// local copy of func_us_801B77D8 (see boss/bo6/richter.c)
void func_pspeu_092577D8(void) {
    if ((MARIA.posX.i.hi - PLAYER.posX.i.hi) <= 0) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257820);

extern AnimationFrame D_pspeu_0927A778[];
extern RECT D_pspeu_0927A518;
extern s32 D_pspeu_0927C768;
extern s32 D_pspeu_0927C770;
extern s32 D_pspeu_0927C248;
extern s16 D_pspeu_0927C778;
extern s16 D_pspeu_0927C780;
extern u8 D_pspeu_0927C788[];
void func_pspeu_09258010(
    s32 damageEffects, s32 damageKind, s32 prevStep, s32 prevStepS) {
    s32 j;
    s32 i;
    u8* s2;
    u8* imgPtr;
    s32 disableColorChange = 0;
    PlayerDraw* playerDraw = &g_PlayerDraw[8];
    Entity* entity;

    switch (MARIA.step_s) {
    case 0:
        MarResetPose();
        func_pspeu_092577D8();
        func_pspeu_0924EA50_from_rbo5(FIX(-0.75));
        MarSetAnimation(D_pspeu_0927A778);
        MARIA.velocityY = FIX(-1.625);
        g_api.PlaySfx(SFX_BOSS_RIC_DEATH);
        D_pspeu_0927C768 = damageEffects;
        MARIA.step_s++;
        if (!g_CastleFlags[SHAFT_ORB_DEFEATED] && !g_DemoMode) {
            entity = &g_Entities[200];
            CreateEntityFromCurrentEntity(E_ID(ID_17), entity);
            entity->params = 2;
            entity->flags = FLAG_UNK_10000;
        }
        D_pspeu_0927C248 = 10;
        MARIA.palette = 0x8220;
        return;
    case 1:
        if (D_pspeu_0927C768 & ELEMENT_FIRE) {
            func_pspeu_09252480_from_rbo5(0);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x48), 0);
            // RIC blueprint 53 has child 9, func_80161C2C
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_DEATH_BY_FIRE, 2), 0);
            D_pspeu_0927C770 = 1;
        } else if (D_pspeu_0927C768 & ELEMENT_THUNDER) {
            func_pspeu_09252480_from_rbo5(2);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4C), 0);
            // RIC blueprint 48 has child 41, EntityHitByLightning
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_THUNDER, 1), 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_THUNDER, 2), 0);
            D_pspeu_0927C770 = 2;
        } else if (D_pspeu_0927C768 & ELEMENT_ICE) {
            func_pspeu_09252480_from_rbo5(3);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4D), 0);
            // RIC blueprint 47 has child 40, EntityHitByIce
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 47, 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_ICE, 1), 0);
            D_pspeu_0927C770 = 3;
            MARIA.blendMode = BLEND_TRANSP | BLEND_ADD;
        } else {
            func_pspeu_09252480_from_rbo5(1);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4A), 0);
            // RIC blueprint 11 has child 5, RicEntityHitByCutBlood
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_MULTIPLE_EMBERS, 5), 0);
            D_pspeu_0927C770 = 0;
        }
        playerDraw->r0 = playerDraw->g0 = playerDraw->b0 = playerDraw->r1 =
            playerDraw->g1 = playerDraw->b1 = playerDraw->r2 = playerDraw->g2 =
                playerDraw->b2 = playerDraw->r3 = playerDraw->g3 =
                    playerDraw->b3 = 0x80;
        playerDraw->enableColorBlend = 1;
        MARIA.step_s++;
        break;
    case 2:
        if (MARIA.pose != 4) {
            break;
        }
        MARIA.step_s++;
        break;
    case 3:
        // Unusual, FIX usually has denominator of 128
        MARIA.velocityY += FIX(11.0 / 256);
        if (MARIA.velocityY > FIX(1.0 / 16)) {
            MARIA.velocityY >>= 2;
            MARIA.velocityX >>= 3;
            StoreImage(&D_pspeu_0927A518, (u_long*)D_pspeu_0927C788);
            D_pspeu_0927C778 = 0;
            D_pspeu_0927C780 = 0x40;
            MARIA.step_s++;
        }
        break;
    case 4:
        if (g_Timer & 1) {
            break;
        }
        if (D_pspeu_0927C780 > 16) {
            // Another really weird velocityY value.
            // It's 0x233.
            MARIA.velocityY += FIX(0.0086);
        } else {
            MARIA.velocityX = 0;
            MARIA.velocityY = 0;
        }
        for (i = 0; i < 4; i++) {
            s2 = imgPtr = D_pspeu_0927C788;
            s2 += ((D_pspeu_0927C778 >> 1) & 7);
            s2 += ((D_pspeu_0927C778 & 0xFF) >> 4) << 6;
            for (j = 0; j < 0x28; j++) {
                if (D_pspeu_0927C778 & 1) {
                    *(s2 + ((j & 7) * 8) + ((j >> 3) * 0x400)) &= 0xF0;
                } else {
                    *(s2 + ((j & 7) * 8) + ((j >> 3) * 0x400)) &= 0x0F;
                }
            }
            D_pspeu_0927C778 += 0x23;
            D_pspeu_0927C778 &= 0xFF;
        }
        LoadImage(&D_pspeu_0927A518, (u_long*)imgPtr);
        if (--D_pspeu_0927C780 == 0) {
            MARIA.velocityY = 0;
            playerDraw->enableColorBlend = 0;
            MARIA.step_s = 0x80;
        }
        break;
    case 0x80:
        break;
    }
    // Always happens, kind of weird
    if (!disableColorChange) {
        if (D_pspeu_0927C770 == 0) {
            if (playerDraw->r0 < 0xF8) {
                playerDraw->r0 += 2;
            }
            if (playerDraw->b0 > 8) {
                playerDraw->b0 -= 2;
            }

            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->r0;
            playerDraw->g0 = playerDraw->g1 = playerDraw->b1 = playerDraw->g2 =
                playerDraw->b2 = playerDraw->g3 = playerDraw->b3 =
                    playerDraw->b0;
        }
        if (D_pspeu_0927C770 == 1 || D_pspeu_0927C770 == 2) {
            if (playerDraw->b0 > 8) {
                playerDraw->b0 -= 2;
            }
            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->r0 =
                playerDraw->g0 = playerDraw->g1 = playerDraw->b1 =
                    playerDraw->g2 = playerDraw->b2 = playerDraw->g3 =
                        playerDraw->b3 = playerDraw->b0;
        }
        if (D_pspeu_0927C770 == 3) {
            if ((playerDraw->r0 > 8) && (g_Timer & 1)) {
                playerDraw->r0 -= 1;
            }
            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->g3 =
                playerDraw->g2 = playerDraw->g1 = playerDraw->g0 =
                    playerDraw->r0;
        }
    }
}

extern AnimationFrame D_pspeu_0927A758[];
// Local copy of RicStepStandInAir; the boss copy restores Maria's palette.
void func_pspeu_09258830(void) {
    if (MARIA.step_s == 0) {
        MARIA.velocityY += 0x3800;
        if (MARIA.velocityY > 0) {
            MARIA.velocityY = 0;
            MARIA.step_s = 1;
        }
    } else if (g_Maria.unk4E) {
        g_Maria.unk46 = 0;
        MarSetStep(PL_S_JUMP);
        MarSetAnimation(D_pspeu_0927A758);
        g_Maria.unk44 = 0;
        MARIA.palette = 0x8220;
    }
}

// local copy of RicStepEnableFlameWhip (see ric/pl_steps.c); the bo6
// variant drops the trailing fall check
void func_pspeu_092588E8(void) {
    if (MARIA.animCurFrame == 181 && MARIA.poseTimer == 1) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_35, 0);
        g_api.PlaySfx(SFX_WEAPON_APPEAR);
    }

    if (MARIA.poseTimer < 0) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x45), 0);
        g_Maria.timers[PL_T_POISON] = 0x800;
    }
}

// local copy of the bo6 RicStepHydrostorm (see boss/bo6/us_39144.c)
void func_pspeu_092589B0(void) {
    if (MARIA.poseTimer < 0) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
    }
}

// local copy of RicStepGenericSubwpnCrash minus the fall check (bo6 variant)
void func_pspeu_092589F0(void) {
    if (g_Maria.unk4E) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
    }
}

// local copy of RicStepThrowDaggers (see ric/pl_steps.c) minus the fall
// check; the timer lives in the wall's raw bss
void func_pspeu_09258A30(void) {
    if (MARIA.step_s == 0) {
        D_pspeu_0927C760 = 0x200;
        MARIA.step_s++;
    } else {
        MarCheckFacing();
        if (!--D_pspeu_0927C760) {
            g_Maria.unk46 = 0;
            func_pspeu_09259AD8(0);
            g_Maria.unk4E = 1;
        }
    }
    if (g_Maria.padTapped & PAD_CROSS) {
        func_pspeu_09259C58();
        g_Maria.unk46 = 0;
        g_Maria.unk4E = 1;
        D_pspeu_0927C760 = 0;
    }
}

// local copy of RicStepSlide (see boss/bo6/richter.c); reads the real
// player's floor sensors via g_Player
void MarStepSlide(void) {
    Collider collider;
    s32 isTouchingGround = 0;

    if (MARIA.facingLeft == 0 && g_Maria.vram_flag & TOUCHING_R_WALL) {
        isTouchingGround = 1;
    }
    if (MARIA.facingLeft && g_Maria.vram_flag & TOUCHING_L_WALL) {
        isTouchingGround = 1;
    }
    if (MARIA.posX.i.hi >= STAGE_WIDTH - 4 && MARIA.facingLeft == 0) {
        isTouchingGround = 1;
    }
    if (MARIA.posX.i.hi <= 4 && MARIA.facingLeft) {
        isTouchingGround = 1;
    }
    if ((MARIA.facingLeft == 0 &&
         g_Player.colFloor[2].effects & EFFECT_UNK_8000) ||
        (MARIA.facingLeft && g_Player.colFloor[3].effects & EFFECT_UNK_8000)) {
        isTouchingGround = 1;
    }
    if (isTouchingGround && MARIA.pose < 6) {
        MARIA.pose = 6;
        if (MARIA.velocityX > FIX(1)) {
            MARIA.velocityX = FIX(2);
        }
        if (MARIA.velocityX < FIX(-1)) {
            MARIA.velocityX = FIX(-2);
        }
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
    }
    if (MARIA.pose < 5) {
        if (func_pspeu_0925B680(CHECK_FALL | CHECK_CRASH)) {
            return;
        }
        if (g_Maria.padTapped & PAD_CROSS) {
            MARIA.posY.i.hi -= 4;
            func_pspeu_0925AA50();
            return;
        }
    } else if (MARIA.pose < 7) {
        if (func_pspeu_0925B680(CHECK_FALL | CHECK_CRASH | CHECK_SLIDE)) {
            return;
        }
    } else if (func_pspeu_0925B680(
                   CHECK_FALL | CHECK_FACING | CHECK_CRASH | CHECK_SLIDE)) {
        return;
    }

    MarDecelerateX(FIX(0.125));
    switch (MARIA.step_s) {
    case 0:
        if (!(g_GameTimer & 3) && MARIA.pose < 6 && MARIA.pose > 2) {
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_SLIDE, 2), 0);
        }
        if (MARIA.pose == 6 && MARIA.poseTimer == 1) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
        }
        if (MARIA.poseTimer < 0) {
            MarSetCrouch(0, MARIA.velocityX);
        }
        break;
    }
}

// local copy of RicStepSlideKick (see boss/bo6/richter.c)
void func_pspeu_09258E60(void) {
    if (g_Maria.padPressed & PAD_SQUARE && g_Maria.unk44 & 0x80) {
        MARIA.step = PL_S_JUMP;
        MarSetAnimation(D_pspeu_0927A790);
        MarSetSpeedX(FIX(-1.5));
        MARIA.velocityY = FIX(-3.5);
        g_Maria.unk44 |= (8 + 2);
        g_Maria.unk44 &= ~4;
        MARIA.step_s = 2;
        return;
    }
    MarDecelerateX(FIX(0.0625));
    MARIA.velocityY += 0x1000;

    if (g_Maria.vram_flag & TOUCHING_GROUND) {
        g_CurrentEntity->velocityX /= 2;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
        MARIA.facingLeft++;
        MARIA.facingLeft &= 1;
        MarSetCrouch(3, MARIA.velocityX);
        g_api.PlaySfx(SFX_STOMP_SOFT_A);
        return;
    }
    if (MARIA.velocityX < 0) {
        if (g_Maria.padPressed & PAD_RIGHT) {
            MarDecelerateX(FIX(0.125));
        }
        if (MARIA.velocityX > FIX(-3) ||
            (g_Maria.vram_flag & TOUCHING_L_WALL)) {
            MARIA.facingLeft++;
            MARIA.facingLeft &= 1;
            MARIA.velocityX /= 2;
            MarSetAnimation(D_pspeu_0927A9C8);
            g_Maria.unk44 = 0xA;
            MARIA.step_s = 2;
            MARIA.step = PL_S_JUMP;
        }
    }
    if (MARIA.velocityX > 0) {
        if (g_Maria.padPressed & PAD_LEFT) {
            MarDecelerateX(FIX(0.125));
        }
        if (MARIA.velocityX < FIX(3) || (g_Maria.vram_flag & TOUCHING_R_WALL)) {
            MARIA.facingLeft++;
            MARIA.facingLeft &= 1;
            MARIA.velocityX /= 2;
            MarSetAnimation(D_pspeu_0927A9C8);
            g_Maria.unk44 = 0xA;
            MARIA.step_s = 2;
            MARIA.step = PL_S_JUMP;
        }
    }
}

void MarStepBladeDash(void) {
    MarDecelerateX(FIX(7.0 / 64));
    if (MARIA.poseTimer < 0) {
        g_Maria.unk46 = 0;
        func_pspeu_09259AD8(0);
        return;
    }

    if (MARIA.pose >= 0x12 && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        g_Maria.unk46 = 0;
        func_pspeu_09259D48();
        return;
    }

    if (!(g_GameTimer & 3) && MARIA.pose < 0x12 &&
        (g_Maria.vram_flag & TOUCHING_GROUND)) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_SLIDE, 2), 0);
    }

    if (MARIA.pose == 18 && MARIA.poseTimer == 1 &&
        (g_Maria.vram_flag & TOUCHING_GROUND)) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
    }
}

// matches cen's func_80158B04 (see st/cen_psp/e_maria_step.c)
void func_80158B04(u16 arg0) {
    s16 xMod = 3;
    if (MARIA.facingLeft) {
        xMod = -xMod;
    }

    MARIA.posY.i.hi -= 16;
    MARIA.posX.i.hi += xMod;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_EMBERS, 1), 0);
    MARIA.posY.i.hi += 16;
    MARIA.posX.i.hi -= xMod;

    if (arg0 & 1) {
        g_api.ShakeCamera(3);
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
    }

    if (arg0 & 2) {
        MARIA.velocityX = 0;
        MARIA.velocityY = 0;
    }
}

// local copy of RicStepHighJump (see ric/pl_steps.c) minus the US debug
// prints
void func_pspeu_09259418(void) {
    bool loadAnim;

    loadAnim = false;
    g_Maria.high_jump_timer++;
    switch (MARIA.step_s) {
    case 0:
        if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
            if (MARIA.facingLeft) {
                if (!(g_Maria.padPressed & PAD_LEFT)) {
                    MarDecelerateX(FIX(0.0625));
                }
            } else {
                if (!(g_Maria.padPressed & PAD_RIGHT)) {
                    MarDecelerateX(FIX(0.0625));
                }
            }
        } else {
            MarDecelerateX(FIX(0.0625));
        }

        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            func_80158B04(3);
            g_Maria.high_jump_timer = 0;
            MARIA.step_s = 2;
        } else if (g_Maria.high_jump_timer > 0x1C) {
            MARIA.step_s = 1;
            MARIA.velocityY = -0x60000;
        }
        break;
    case 1:
        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            MARIA.step_s = 2;
            func_80158B04(3);
            g_Maria.high_jump_timer = 0;
        } else {
            MARIA.velocityY += 0x6000;
            if (MARIA.velocityY > 0x8000) {
                loadAnim = true;
            }
        }
        break;
    case 2:
        if (g_Maria.high_jump_timer > 4) {
            loadAnim = true;
        }
        break;
    }

    if (loadAnim) {
        MarSetAnimation(D_pspeu_0927A768);
        MarSetStep(PL_S_JUMP);
    }
}

void MarLoadGraphics_09259630(void) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259638);

void MarLoadGraphics_09259858(void) {}

// local copy of func_us_801B9340 (see boss/bo6/us_39144.c)
void func_pspeu_09259860(void) {
    switch (MARIA.step_s) {
    case 0:
        MarResetPose();
        MARIA.velocityY = FIX(-5);
        func_pspeu_0924EA50_from_rbo5(0xFFFF1000);
        MARIA.anim = D_pspeu_0927A8D0;
        g_api.PlaySfx(SFX_BOSS_RIC_DEATH);
        g_Maria.damagePalette = PAL_FLAG(PAL_CC_RED_EFFECT_A);
        g_Maria.timers[2] = 8;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(E_ID_21, 0x58), 0);
        MARIA.step_s += 1;
        return;
    case 1:
        if ((g_Maria.vram_flag & TOUCHING_CEILING) &&
            (FIX(-1) > MARIA.velocityY)) {
            MARIA.velocityY = FIX(-1);
        }
        if (func_pspeu_0925B680(0x20280) != 0) {
            MARIA.step = 0x70;
            MARIA.step_s = 2;
        }
        break;
    case 2:
        MarDecelerateX(FIX(0.125));
        if ((PLAYER.posX.i.hi - MARIA.posX.i.hi) > 0) {
            MARIA.facingLeft = 0;
            return;
        }
        MARIA.facingLeft = 1;
        break;
    }
}

// local copy of RicSetCrouch (see ric/pl_setstep.c)
void MarSetCrouch(s32 kind, s32 velocityX) {
    MarSetStep(PL_S_CROUCH);
    MarSetAnimation(D_pspeu_0927A6E8);
    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    if (kind == 1) {
        MARIA.anim = D_pspeu_0927A6D8;
        MARIA.step_s = 4;
    }
    if (kind == 2) {
        MARIA.anim = D_pspeu_0927A6F8;
        MARIA.step_s = 1;
    }
    if (kind == 3) {
        MARIA.anim = D_pspeu_0927A6F0;
        MARIA.step_s = 4;
    }
}

// local copy of RicSetStand (see boss/bo6/us_39144.c)
void func_pspeu_09259AD8(s32 velocityX) {
    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_STAND);
    MarSetAnimation(D_pspeu_0927A5E0);
}

// local copy of func_us_801B9D74 (see boss/bo6/us_39144.c)
void func_pspeu_09259B30(void) {
    g_Maria.unk44 = 0;
    MarSetStep(0x1A);
    MarSetAnimation(D_pspeu_0927A8A8);
    MarSetSpeedX(FIX(2.25));
    g_Maria.timers[11] = 0x28;
    MARIA.velocityY = 0;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x50001, 0);
}

// local copy of RicSetWalk (see ric/pl_setstep.c) minus the unk7A checks
void func_pspeu_09259BB0(s32 arg0) {
    if (g_Maria.timers[PL_T_8]) {
        func_pspeu_09259B30();
        return;
    }
    g_Maria.timers[PL_T_CURSE] = 8;
    if (g_Maria.timers[PL_T_CURSE]) {
        g_Maria.timers[PL_T_8] = 12;
    }
    g_Maria.timers[PL_T_CURSE] = 12;
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_WALK);
    MarSetAnimation(D_pspeu_0927A6B0);
    MarSetSpeedX(FIX(1.25));
    MARIA.velocityY = 0;
}

// local copy of func_us_801B9E70 (see boss/bo6/us_39144.c)
void func_pspeu_09259C58(void) {
    if ((MarCheckFacing() != 0) || (MARIA.step == 0x18)) {
        MarSetAnimation(D_pspeu_0927A738);
        if (MARIA.step == 0x1A) {
            MarSetSpeedX(FIX(2.25));
            g_Maria.unk44 = 0x10;
        } else {
            MarSetSpeedX(0x14000);
            g_Maria.unk44 = 0;
        }
    } else {
        MarSetAnimation(D_pspeu_0927A718);
        MARIA.velocityX = 0;
        g_Maria.unk44 = 4;
    }
    MarSetStep(5);
    MARIA.velocityY = FIX(-4.6875);
}

// local copy of RicSetFall (see boss/bo6/us_39144.c)
void func_pspeu_09259D48(void) {
    if (g_Maria.prev_step != PL_S_RUN && g_Maria.prev_step != PL_S_SLIDE) {
        MARIA.velocityX = 0;
    }
    if (g_Maria.prev_step != PL_S_WALK && g_Maria.prev_step != PL_S_RUN) {
        MarSetAnimation(D_pspeu_0927A768);
    }
    if (g_Maria.prev_step == PL_S_RUN) {
        g_Maria.unk44 = 0x10;
    }
    MarSetStep(PL_S_FALL);
    MARIA.velocityY = FIX(2);
    g_Maria.timers[PL_T_5] = 8;
    g_Maria.timers[PL_T_6] = 8;
    g_Maria.timers[PL_T_CURSE] = 0;
    g_Maria.timers[PL_T_8] = 0;
    if (g_Maria.prev_step == PL_S_SLIDE) {
        g_Maria.timers[PL_T_5] = g_Maria.timers[PL_T_6] = 0;
        MARIA.pose = 2;
        MARIA.poseTimer = 0x10;
        MARIA.velocityX /= 2;
    }
}

// local copy of func_us_801BA050 (see boss/bo6/us_39144.c)
void func_pspeu_09259EA8(void) {
    MarSetStep(9);
    MARIA.velocityX = 0;
    MarSetSpeedX(FIX(1.25));
    MARIA.velocityY = FIX(-7.5);
    g_Maria.high_jump_timer = 0;
    MarSetAnimation(D_pspeu_0927A9E0);
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x2D, 0), 0);
    g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
    g_Maria.timers[12] = 4;
}

s32 MarCheckSubwpnChainLimit(s16 subwpnId, s16 limit) {
    Entity* entity;
    s32 i;
    s32 nFound;
    s32 nEmpty;

    // Iterate through entities 96-128 (which hold subweapons)
    // Any that match the proposed ID increments the count.
    // If at any point the count reaches the limit, return -1.
    for (entity = &g_Entities[96], i = 0, nFound = 0, nEmpty = 0; i < 32; i++,
        entity++) {
        if (!entity->entityId) {
            nEmpty++;
        }

        if (entity->ext.subweapon.subweaponId &&
            entity->ext.subweapon.subweaponId == subwpnId) {
            nFound++;
        }

        if (nFound >= limit) {
            return -1;
        }
    }

    // This will indicate that there is an available entity slot
    // to hold the subweapon we're trying to spawn.
    // At the end, if this is zero, there are none available so return
    // -1 to indicate there is no room for the proposed subweapon.
    if (nEmpty) {
        return 0;
    }

    return -1;
}

// local copy of RicDoSubweapon (see boss/bo6/us_39144.c)
s32 func_pspeu_0925A028(void) {
    SubweaponDef subweapon;
    s16 subweaponId;
    s16 chainLimit;
    s16 unused;

    unused = 0;
    if (!(g_Maria.padPressed & PAD_UP)) {
        return 1;
    }

    subweaponId = func_pspeu_09248D20(&subweapon, 0, 0);
    chainLimit = subweapon.chainLimit;
    if (MarCheckSubwpnChainLimit(subweaponId, chainLimit) < 0) {
        return 2;
    }

    MarCreateEntFactoryFromEntity(g_CurrentEntity, subweapon.blueprintNum, 0);
    g_Maria.timers[PL_T_10] = 4;
    switch (MARIA.step) {
    case PL_S_RUN:
        MARIA.step = PL_S_STAND;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
        MarSetAnimation(D_pspeu_0927A820);
        break;
    case PL_S_STAND:
    case PL_S_WALK:
    case PL_S_CROUCH:
        MARIA.step = PL_S_STAND;
        MarSetAnimation(D_pspeu_0927A820);
        break;
    case PL_S_FALL:
    case PL_S_JUMP:
        MARIA.step = PL_S_JUMP;
        MarSetAnimation(D_pspeu_0927A870);
        break;
    }
    g_Maria.unk46 = 3;
    MARIA.step_s = 0x42;
    // n.b.! this was just set before the switch
    g_Maria.timers[PL_T_10] = 4;
    return 0;
}

extern AnimationFrame D_pspeu_0927A7C0[];
extern AnimationFrame D_pspeu_0927A7E0[];
extern AnimationFrame D_pspeu_0927A800[];
bool func_pspeu_0925A1F0(void) {
    s32 i;
    s16 poisoned;
    s16 sfxGrunt;

    sfxGrunt = rand() % 6;
    if (func_pspeu_0925A028() == 0) {
        if (sfxGrunt == 0) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
        }
        if (sfxGrunt == 1) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
        }
        if (sfxGrunt == 2) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_C);
        }
        if (sfxGrunt == 3) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_D);
        }
        return true;
    }
    if (g_Maria.timers[PL_T_POISON]) {
        poisoned = true;
    } else {
        poisoned = false;
    }
    for (i = 80; i < 95; i++) {
        DestroyEntity(&g_Entities[i]);
    }
    if (MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_WHIP, poisoned), 0)) {
        if (poisoned) {
            g_api.PlaySfx(SFX_RIC_FLAME_WHIP);
        } else {
            g_api.PlaySfx(SFX_BOSS_RIC_WHIP_ATTACK);
        }
        if (sfxGrunt == 0) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
        }
        if (sfxGrunt == 1) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
        }
        if (sfxGrunt == 2) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_C);
        }
        if (sfxGrunt == 3) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_D);
        }
        switch (MARIA.step) {
        case PL_S_STAND:
        case PL_S_WALK:
            MARIA.step = PL_S_STAND;
            MarSetAnimation(D_pspeu_0927A7C0);
            g_CurrentEntity->velocityX = 0;
            break;
        case PL_S_RUN:
            MARIA.step = PL_S_STAND;
            MarSetAnimation(D_pspeu_0927A7C0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
            break;
        case PL_S_CROUCH:
            MarSetAnimation(D_pspeu_0927A7E0);
            g_CurrentEntity->velocityX = 0;
            break;
        case PL_S_FALL:
        case PL_S_JUMP:
            MARIA.step = PL_S_JUMP;
            MarSetAnimation(D_pspeu_0927A800);
            break;
        default:
            return false;
        }
        g_Maria.unk46 = 1;
        MARIA.step_s = 0x40;
        g_Maria.timers[PL_T_ATTACK] = 4;
        return true;
    }
    return false;
}

extern AnimationFrame D_pspeu_0927AA88[];
extern AnimationFrame D_pspeu_0927A840[];
extern AnimationFrame D_pspeu_0927A890[];
extern AnimationFrame D_pspeu_0927A9F0[];
extern AnimationFrame D_pspeu_0927AB00[];
bool func_pspeu_0925A588(void) {
    SubweaponDef subWpn;
    Entity* subWpnEnt;
    s16 subWpnID;

    subWpnID = func_pspeu_09248D20(&subWpn, true, false);
    if (subWpnID == PL_W_HOLYWATER && g_Maria.timers[PL_T_3]) {
        return 0;
    }
    if (subWpn.blueprintNum) {
        if (subWpnID == 1) {
            subWpnEnt = MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 1), 0);
        } else {
            subWpnEnt = MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 0), 0);
        }
    }
    if (subWpnEnt == NULL) {
        return 0;
    }
    g_Maria.unk46 = 4;
    g_Maria.unk4E = 0;
    MARIA.velocityX = MARIA.velocityY = 0;
    switch (subWpnID) {
    case SUBWPN_NONE:
        MarSetStep(PL_S_FLAME_WHIP);
        MarSetAnimation(D_pspeu_0927AA88);
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_36, 1), 0);
        break;
    case SUBWPN_DAGGER:
        MarSetStep(PL_S_THROW_DAGGERS);
        MarSetAnimation(D_pspeu_0927A840);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case SUBWPN_AXE:
        MarSetStep(PL_S_STAND_IN_AIR);
        MarSetAnimation(D_pspeu_0927A890);
        MARIA.velocityY = FIX(-4.6875);
        func_maria_8015CC28();
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case SUBWPN_HOLYWATER:
        MarSetStep(PL_S_HYDROSTORM);
        MarSetAnimation(D_pspeu_0927A9F0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_HYDRO_STORM);
        break;
    case SUBWPN_REBNDSTONE:
    case SUBWPN_VIBHUTI:
    case SUBWPN_AGUNEA:
        MarSetStep(PL_S_SUBWPN_CRASH);
        MarSetAnimation(D_pspeu_0927A9F0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        break;
    case SUBWPN_BIBLE:
    case SUBWPN_STOPWATCH:
        MarSetStep(PL_S_SUBWPN_CRASH);
        MarSetAnimation(D_pspeu_0927AB00);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        break;
    case SUBWPN_CROSS:
        MarSetStep(PL_S_STAND_IN_AIR);
        MarSetAnimation(D_pspeu_0927A890);
        MARIA.velocityY = FIX(-4.6875);
        func_maria_8015CC28();
        g_api.PlaySfx(SFX_BOSS_RIC_HOLY_CROSS);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    }
    g_Maria.timers[PL_T_12] = 4;
    return 1;
}

void RicSetDeadPrologue() { MarSetStep(PL_S_DEAD_PROLOGUE); }

// local copy of RicSetSlide (see boss/bo6/us_39144.c)
void MarSetSlide(void) {
    MarCheckFacing();
    MarSetStep(PL_S_SLIDE);
    MarSetAnimation(D_pspeu_0927A988);
    g_CurrentEntity->velocityY = 0;
    MarSetSpeedX(FIX(5.5));
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(SFX_BOSS_RIC_SLIDE_SKID);
    g_Maria.timers[PL_T_12] = 4;
}

// local copy of RicSetSlideKick (see boss/bo6/us_39144.c)
void func_pspeu_0925AA50(void) {
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_SLIDE_KICK);
    MarSetAnimation(D_pspeu_0927A9B8);
    g_CurrentEntity->velocityY = FIX(-2);
    MarSetSpeedX(FIX(5.5));
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
    g_Maria.timers[PL_T_12] = 4;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_31, 0);
}

// local copy of RicSetBladeDash (see ric/pl_setstep.c)
void MarSetBladeDash(void) {
    MarSetStep(PL_S_BLADEDASH);
    MarSetAnimation(D_pspeu_0927AA20);
    g_CurrentEntity->velocityY = 0;
    MarSetSpeedX(FIX(5.5));
    g_Maria.unk46 = 5;
    g_Maria.timers[PL_T_12] = 4;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_BLADE_DASH, 0);
    func_maria_8015CC28();
    g_api.PlaySfx(SFX_BOSS_RIC_DASH_ATTACK);
    g_api.PlaySfx(SFX_RIC_SLIDE_SKID);
}

#include "../../mar_init_after_image.h"

#include "../../mar_draw_after_image.h"

#include "../../mar_set_step.h"

#include "../../mar_set_animation.h"

#include "../../mar_decelerate_x.h"

#include "../../mar_check_facing.h"

#include "../../mar_set_speed_x.h"

void func_pspeu_0924EA50_from_rbo5(s32 velocityX) {
    if (MARIA.entityRoomIndex == 1) {
        velocityX = -velocityX;
    }
    MARIA.velocityX = velocityX;
}

// local copy of RicSetInvincibilityFrames (see boss/bo6/us_39144.c)
void func_pspeu_0925B4E0(s32 kind, s16 invincibilityFrames) {
    if (!kind) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_CRASH_DAGGER, 0x15), 0);
        if (g_Maria.timers[PL_T_INVINCIBLE_SCENE] <= invincibilityFrames) {
            g_Maria.timers[PL_T_INVINCIBLE_SCENE] = invincibilityFrames;
        }
    } else if (g_Maria.timers[PL_T_INVINCIBLE] <= invincibilityFrames) {
        g_Maria.timers[PL_T_INVINCIBLE] = invincibilityFrames;
    }
}

#include "../../mar_disable_after_image.h"

#include "../../mar_reset_after_image.h"

extern AnimationFrame D_pspeu_0927A978[];
extern AnimationFrame D_pspeu_0927A970[];
bool func_pspeu_0925B680(s32 checks) {
    s32 velYChange;
    u32 effects;

    if (checks & CHECK_FACING) {
        if (g_Maria.unk46 == 0) {
            MarCheckFacing();
        }
    }
    velYChange = 0;
    if (checks & CHECK_GRAVITY_FALL) {
        velYChange = FIX(28.0 / 128);
    }
    if (checks & CHECK_GRAVITY_JUMP) {
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8) && !(g_Maria.unk44 & 0x20) &&
            (g_Maria.padPressed & PAD_CROSS)) {
            // Note that 5.6 is precisely 1/5 of 28.
            velYChange = FIX(5.6 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    if (checks & CHECK_GRAVITY_HIT) {
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8)) {
            velYChange = FIX(14.0 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    MARIA.velocityY += velYChange;
    if (MARIA.velocityY > FIX(7)) {
        MARIA.velocityY = FIX(7);
    }
    if ((checks & CHECK_80) && (g_Maria.vram_flag & TOUCHING_CEILING) &&
        (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-1);
    }
    if (MARIA.velocityY >= 0) {
        if ((checks & CHECK_GROUND) && (g_Maria.vram_flag & TOUCHING_GROUND)) {
            switch (g_Maria.unk46) {
            case 0:
            default:
                if (g_Maria.unk44 & 8) {
                    MarSetCrouch(3, MARIA.velocityX);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0U, 0);
                    return true;
                }
                if (MARIA.velocityY > FIX(6.875)) {
                    MarSetCrouch(1, 0);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0U, 0);
                } else if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
                    if (g_Maria.unk44 & 0x10
                    ) {
                        func_pspeu_09259B30();
                    } else {
                        func_pspeu_09259BB0(0);
                    }
                } else {
                    func_pspeu_09259AD8(0);
                }
                return true;
            case 1:
                if (MARIA.velocityY > FIX(6.875)) {
                    MARIA.step = PL_S_CROUCH;
                    MARIA.anim = D_pspeu_0927A7E0;
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0U, 0);
                } else {
                    MARIA.step = PL_S_STAND;
                    MARIA.anim = D_pspeu_0927A7C0;
                    if (!(g_Maria.unk44 & 8)) {
                        MARIA.velocityX = 0;
                    } else {
                        MarCreateEntFactoryFromEntity(
                            g_CurrentEntity, 0U, 0);
                        g_api.PlaySfx(SFX_STOMP_HARD_B);
                    }
                }
                MARIA.velocityY = 0;
                g_Maria.unk44 = 0;
                return true;
            case 2:
                if (MARIA.velocityY > FIX(6.875)) {
                    MARIA.step = PL_S_CROUCH;
                    MARIA.anim = D_pspeu_0927A978;
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0U, 0);
                } else {
                    MARIA.step = PL_S_STAND;
                    MARIA.anim = D_pspeu_0927A970;
                    if (!(g_Maria.unk44 & 8)) {
                        MARIA.velocityX = 0;
                    } else {
                        MarCreateEntFactoryFromEntity(
                            g_CurrentEntity, 0U, 0);
                        g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    }
                }
                MARIA.velocityY = 0;
                g_Maria.unk44 = 0;
                return true;
            case 3:
                if ((MARIA.velocityY > FIX(6.875)) || (g_Maria.unk44 & 8)) {
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0U, 0);
                }
                MARIA.velocityY = 0;
                if (!(g_Maria.unk44 & 8)) {
                    MARIA.velocityX = 0;
                }
                MARIA.step = PL_S_STAND;
                MARIA.anim = D_pspeu_0927A820;
                g_Maria.unk44 = 0;
                return true;
            }
        } else if (checks & CHECK_GROUND_AFTER_HIT &&
                   (g_Maria.vram_flag & TOUCHING_GROUND)) {
            MarSetCrouch(1, MARIA.velocityX);
            g_api.PlaySfx(SFX_STOMP_SOFT_A);
            if (g_Maria.unk5C && (g_Status.hp < 2)) {
                RicSetDeadPrologue();
                return true;
            } else {
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, 0U, 0);
            }
            return true;
        }
    }
    if (checks & CHECK_FALL && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        if (g_Maria.unk46) {
            if (g_Maria.unk46 == 1) {
                MARIA.step_s = 0x40;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A800;
                return true;
            }
            if (g_Maria.unk46 == 2) {
                MARIA.step_s = 0x41;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A800;
                return true;
            }
            if (g_Maria.unk46 == 3) {
                MARIA.step_s = 0x42;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A870;
                return true;
            }
        } else {
            func_pspeu_09259D48();
            return true;
        }
    }
    if (g_Maria.unk46) {
        return false;
    }
    if (checks & CHECK_CRASH && (g_Maria.padTapped & PAD_TRIANGLE) &&
        func_pspeu_0925A588()) {
        return true;
    }
    if (checks & CHECK_SLIDE &&
        !(MARIA.facingLeft == 0 &&
          g_Player.colFloor[2].effects & EFFECT_UNK_8000) &&
        !(MARIA.facingLeft &&
          g_Player.colFloor[3].effects & EFFECT_UNK_8000) &&
        (MARIA.posX.i.hi < (u8)-4 || MARIA.facingLeft != 0) &&
        (MARIA.posX.i.hi >= 5 || !MARIA.facingLeft) &&
        g_Maria.padPressed & PAD_DOWN && g_Maria.padTapped & PAD_CROSS) {
        MarSetSlide();
        return true;
    }
    if (checks & CHECK_JUMP && (g_Maria.padTapped & PAD_CROSS)) {
        func_pspeu_09259C58();
        return true;
    }
    if (checks & CHECK_ATTACK && (g_Maria.padTapped & PAD_SQUARE) &&
        func_pspeu_0925A1F0()) {
        return true;
    }
    if (checks & CHECK_CROUCH && (g_Maria.padPressed & PAD_DOWN)) {
        MarSetCrouch(2, 0);
        return true;
    }
    return false;
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

void func_pspeu_092509D0_from_rbo5(Entity* self) {
    s16 playerX;
    s16 playerY;
    Collider collider;
    s32 speed;
    s32 currX;
    s32 currY;
    s32 collX;
    s32 collY;
    s32 deltaX;
    s32 deltaY;
    s32 i;
    s32 colliderFlags;
    PrimLineG2* prim;

    speed = 0x400;
    self->ext.reboundStone.unk82 = 0;
    playerX = self->posX.i.hi;
    playerY = self->posY.i.hi;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->posY.i.hi -= 0x10;
        playerY = self->posY.i.hi;
        for (prim = (PrimLineG2*)&g_PrimBuf[self->primIndex], i = 0;
             prim != NULL; i++, prim = prim->next) {
            PGREY(prim, 0) = PGREY(prim, 1) = 0xFF;
            prim->priority = MARIA.zPriority + 2;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            if (i != 0) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim->x0 = prim->x1 = playerX;
            prim->y0 = prim->y1 = playerY;
            prim->timer = 20;
        }
        self->flags =
            FLAG_POS_CAMERA_LOCKED | FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->zPriority = MARIA.zPriority + 2;

        if (MARIA.facingLeft) {
            self->ext.reboundStone.stoneAngle = 0x980;
        } else {
            self->ext.reboundStone.stoneAngle = 0xE80;
        }
        self->ext.reboundStone.stoneAngle += (rand() & 0x7F) - 0x40;

        self->ext.reboundStone.lifeTimer = 0x40;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            self->ext.reboundStone.unk84 = 4;
        }
        self->step++;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        break;
    case 1:
        deltaX = rcos(self->ext.reboundStone.stoneAngle) * 0x10;
        deltaY = -rsin(self->ext.reboundStone.stoneAngle) * 0x10;
        currX = self->posX.val;
        currY = self->posY.val;
        if (!self->ext.reboundStone.unk84) {
            for (i = 0; i < 6; i++) {
                collX = FIX_TO_I(currX);
                collY = FIX_TO_I(currY + deltaY);
                g_api.CheckCollision(collX, collY, &collider, 0);
                colliderFlags =
                    collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
                if (colliderFlags & EFFECT_SOLID) {
                    colliderFlags &= 0xFF00;
                    if (deltaY > 0) {
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_0800)) {
                            func_pspeu_092508C0_from_rbo5(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            func_pspeu_09250948_from_rbo5(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xA0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x760);
                        }
                    }
                    if (deltaY < 0) {
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_8000)) {
                            func_pspeu_092508C0_from_rbo5(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            func_pspeu_09250948_from_rbo5(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xF60);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x8A0);
                        }
                    }
                }
                collY = FIX_TO_I(currY);
                collX = FIX_TO_I(currX + deltaX);
                g_api.CheckCollision(collX, collY, &collider, 0);
                colliderFlags =
                    collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
                if (colliderFlags & EFFECT_SOLID) {
                    colliderFlags &= 0xFF00;
                    // Cases when traveling right
                    if (deltaX > 0) {
                        if ((colliderFlags == 0) ||
                            TEST_BITS(colliderFlags, 0x4800) ||
                            TEST_BITS(colliderFlags, 0xC000)) {
                            func_pspeu_092508C0_from_rbo5(0x400);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            func_pspeu_09250948_from_rbo5(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xF60);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            func_pspeu_09250948_from_rbo5(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xA0);
                        }
                    }
                    // Cases when traveling left
                    if (deltaX < 0) {
                        if ((colliderFlags == 0) ||
                            ((colliderFlags & 0x4800) == 0x800) ||
                            ((colliderFlags & 0xC000) == 0x8000)) {
                            func_pspeu_092508C0_from_rbo5(0x400);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x8A0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x760);
                        }
                    }
                }
                if (self->ext.reboundStone.unk82) {
                    goto block_93;
                }
                currX += deltaX;
                currY += deltaY;
            }
        } else {
            self->ext.reboundStone.unk84--;
        }
    block_93:
        if (self->ext.reboundStone.unk82) {
            // @bug: should call RicCreateEntFactoryFromEntity instead in case
            // E_FACTORY goes out of order between RIC and DRA
            g_api.CreateEntFactoryFromEntity(self, FACTORY(42, 2), 0);
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
        }
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x40 || self->posY.i.hi > 0x140 ||
            self->ext.reboundStone.unk80 == 15) {
            self->step = 2;
        } else {
            deltaX =
                ((rcos(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posX.val += deltaX;
            deltaY =
                -((rsin(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posY.val += deltaY;
        }
        break;
    case 2:
        if (--self->ext.reboundStone.lifeTimer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.reboundStone.lifeTimer == 0x20) {
            self->hitboxState = 0;
        }
        prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->timer = 0;
            prim = prim->next;
        }
        break;
    }

    prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
    i = 0;
    if (self->step == 2) {
        colliderFlags = 4;
    } else {
        colliderFlags = 2;
    }
    // cleaner to use previous 3 lines than to put them in the for's initializer
    for (; prim != NULL; i++, prim = prim->next) {
        if (self->ext.reboundStone.unk82) {
            if (i == self->ext.reboundStone.unk80) {
                prim->x0 = playerX;
                prim->y0 = playerY;
                prim->drawMode &= ~DRAW_HIDE;
            }
        }
        if (i == self->ext.reboundStone.unk80) {
            prim->x1 = self->posX.i.hi;
            prim->y1 = self->posY.i.hi;
        }
        if (!(prim->drawMode & DRAW_HIDE)) {
            if (prim->timer) {
                prim->timer--;
            } else {
                // again not colliderFlags, seems to control stone fading
                if (colliderFlags < prim->b1) {
                    prim->b1 -= colliderFlags;
                }
                PGREY(prim, 0) = PGREY(prim, 1);
            }
        }
    }
}

void func_pspeu_0925CC00(Entity* self) {
    Collider col;
    FakePrim* prim;
    s16 randomAngle;
    s16 x;
    s16 y;
    s16 temp; // used for multiple unrelated things
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 13);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->hitboxWidth = self->hitboxHeight = 4;
        self->ext.subweapon.timer = 0x80;
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        self->posY.i.hi -= 15;
        if (MARIA.facingLeft) {
            self->posX.i.hi -= 13;
        } else {
            self->posX.i.hi += 13;
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        i = 0;
        while (true) {
            prim->drawMode = DRAW_UNK02;
            prim->priority = MARIA.zPriority - 1;
            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                break;
            }
            prim->posX.i.hi = x;
            prim->posY.i.hi = y;
            prim->posX.i.lo = prim->posY.i.lo = 0;
            randomAngle = (rand() & 0xFF) + 0x100;
            temp = (rand() & 0xFF) + 0x80;
            prim->velocityX.val = ((rcos(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val += FIX(0.5);
            prim->velocityY.val = -((rsin(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val = (prim->velocityX.val * 3) >> 1;
            if (self->facingLeft) {
                prim->velocityX.val = -prim->velocityX.val;
            }
            prim->posY.i.hi -= 4;
            prim->delay = 1;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim->r0 = 0xFF;
            prim->g0 = 0xFF;
            prim->b0 = 0xFF;
            prim->w = 2;
            prim->h = 2;
            i++;
            prim = prim->next;
        }
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        temp = 2;
        if (self->facingLeft) {
            temp = -temp;
        }
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (true) {
            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                break;
            }
            prim->posX.i.hi = prim->x0;
            prim->posY.i.hi = prim->y0;
            if (prim->delay) {
                if (prim->velocityX.val != 0) {
                    prim->posX.val += prim->velocityX.val;
                    g_api.CheckCollision(
                        prim->posX.i.hi + temp, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SIDE) {
                        prim->velocityX.val = 0;
                    }
                }
                prim->posY.val += prim->velocityY.val;
                prim->velocityY.val += FIX(12.0 / 128);
                if (prim->velocityY.val > FIX(4)) {
                    prim->velocityY.val = FIX(4);
                }
                if (prim->velocityY.val > 0) {
                    g_api.CheckCollision(
                        prim->posX.i.hi, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SOLID) {
                        prim->delay = 0;
                        prim->posY.i.hi += col.unk18;
                        prim->posY.i.hi -= (i % 3 + 1);
                        prim->w = prim->h = 3;
                    }
                }
            }
            if ((self->ext.subweapon.timer & 7) == i) {
                self->posX.i.hi = prim->posX.i.hi;
                self->posY.i.hi = prim->posY.i.hi;
                if (prim->drawMode & DRAW_HIDE) {
                    self->hitboxWidth = self->hitboxHeight = 0;
                } else {
                    self->hitboxWidth = self->hitboxHeight = 4;
                }
                if (prim->delay) {
                    self->hitboxOffY = 0;
                } else {
                    self->hitboxOffY = -6;
                }
            }
            if (self->hitFlags && ((self->ext.subweapon.timer + 1) & 7) == i) {
                prim->drawMode = DRAW_HIDE;
            }
            if (self->ext.subweapon.timer - 1 == i) {
                prim->drawMode = DRAW_HIDE;
            }
            i++;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim = prim->next;
        }
        self->hitFlags = 0;
        break;
    }
}
