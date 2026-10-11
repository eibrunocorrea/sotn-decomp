// SPDX-License-Identifier: AGPL-3.0-or-later
#include "inc_asm.h"
#include "sattypes.h"
#include "rstage16.h"
#define _SPR2_
#include "lib/spr/spr.h"

extern SprGourTbl* SpGourTbl;

extern RStage16BreakableData DAT_060E99C8[];

void func_060DC040(Entity* self) {
    RStage16BreakableData* data;
    SaturnSpriteResource* resource;
    SpriteObject* sprite;
    Primitive* prim;
    s32 primIndex;

    data = &DAT_060E99C8[self->params >> 12];
    if (self->step == 0) {
        resource = data->resource;
        TekiInit(self, 0);
        self->step++;
        sprite = CreateSpriteObject(resource->allocationIndex, resource->flags,
                                    resource->images, data->maxParts);
        self->unk0 = sprite;
        sprite->zPriority = 0x7C;
        sprite->posX = self->posX.val;
        sprite->posY = self->posY.val;
        self->unk1C = data->unk1C;
        self->hitboxHeight = data->hitboxHeight;
        if ((u16)data->paletteOffset != 0) {
            sprite->clutBase = (resource->flags + data->paletteOffset) | 0x8000;
            self->palette = data->paletteOffset;
        }
        self->hitboxOffY = -data->offsetY;
        if (data->primitiveFrames != NULL) {
            primIndex = AllocPrimitives(0, 1);
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            self->primIndex = primIndex;
            self->flags |= FLAG_HAS_PRIMS;
            prim = &g_PrimBuf[primIndex];
            prim->unk4 &= ~0x38;
            prim->priority = 0x20;
            prim->drawMode = DRAW_HIDE;
        }
    }
    AnimateEntityWithSpriteData(self, data->animation, data->spriteFrames);
    if (data->primitiveFrames != NULL) {
        func_060DC318(self, data);
    }
    if (self->hitParams != 0) {
        func_060DC1A8(self);
    }
}

void func_060DC1A8(Entity* self) {
    Entity* entity;
    RStage16BreakableData* data;
    u16 variant;

    variant = self->params >> 12;
    data = &DAT_060E99C8[variant];
    if (data->primitiveFrames != NULL) {
        FreePrimitives(self->primIndex);
        self->flags &= ~FLAG_HAS_PRIMS;
    }
    entity = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
    if (entity != NULL) {
        CreateEntityFromCurrentEntity(2, entity);
        entity->posY.i.hi -= data->offsetY;
        entity->params = data->explosionParams;
    }
    switch (variant) {
    case 0:
    case 1:
        entity = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(0x12, entity);
            entity->params = variant;
        }
        self->posY.i.hi -= data->offsetY;
        PlaySfx(0x634);
        func_0607B4B8(self);
        break;
    case 2:
        PlaySfx(0x61D);
        entity = AllocEntity(&g_Entities[0xA0], &g_Entities[0xC0]);
        if (entity != NULL) {
            CreateEntityFromEntity(0xC, self, entity);
            entity->posY.i.hi -= data->offsetY;
            entity->params = self->params & 0x1FF;
        }
        PreventEntityFromRespawning(self);
        DestroyEntity(self);
        break;
    default:
        PlaySfx(0x634);
        func_0607B4B8(self);
        break;
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DC318, func_060DC318);
void func_060DC418(Entity* self) {
    SpriteObject* sprite;

    if (self->step == 0) {
        TekiInit(self, 5);
        self->step++;
        sprite = CreateSpriteObject(
            g_RStage16SpriteBank16.allocationIndex,
            g_RStage16SpriteBank16.flags, g_RStage16SpriteBank16.images, 1);
        self->unk0 = sprite;
        if (self->params == 0) {
            self->animCurFrame = 10;
        } else {
            self->animCurFrame = 5;
        }
        func_0600AFA8(sprite, DAT_060E8898[self->animCurFrame]);
        sprite->zPriority = 0x6A;
        self->step = 0x100;
        func_06079BB4(self);
    }
}
typedef struct {
    SaturnSpriteResource* resource;
    const u32* frames;
    s16 clutOffset;
    u16 priority;
    u8 spriteCount;
    u8 commandFlags;
    u8 flipFlags;
    u8 unk56;
    u8 padding10;
    u8 drawFlags;
    u8 padding12;
    u8 unk1C;
    u32 flags;
    const u8* animation;
} RStage16AnimatedSpriteData;

extern RStage16AnimatedSpriteData DAT_060E9A30[];

void func_060DC4C0(Entity* self) {
    RStage16AnimatedSpriteData* data;
    SpriteObject* sprite;
    u16 spriteFlags;

    data = DAT_060E9A30 + self->params;
    if (self->step == 0) {
        TekiInit(self, 3);
        sprite = CreateSpriteObject(
            data->resource->allocationIndex, data->resource->flags,
            data->resource->images, data->spriteCount);
        self->unk0 = sprite;
        sprite->posX = self->posX.val;
        sprite->posY = self->posY.val;
        self->step++;
        sprite->zPriority = data->priority;
        if ((u16)data->clutOffset != 0) {
            sprite->clutBase =
                (g_RStage16SpriteBank16.flags + data->clutOffset) | 0x8000;
        }
        spriteFlags =
            (u16)((sprite->flags & 0xFFF8) | (data->commandFlags & 7)) & 0xFFCF;
        sprite->flags = spriteFlags;
        sprite->flags = data->flipFlags ^ spriteFlags;
        self->unk56 = data->unk56;
        self->drawFlags = data->drawFlags;
        self->unk1C = data->unk1C;
        if (self->drawFlags & 1) {
            sprite->flags |= 0x40;
            sprite->scaleX = (u32)self->scaleX >> 2;
        }
        if (self->drawFlags & 2) {
            sprite->flags |= 0x40;
            sprite->scaleY = (u32)self->scaleY >> 2;
        }
        if (data->flags != 0) {
            self->flags = data->flags;
        }
    }
    AnimateEntityWithSpriteData(self, data->animation, data->frames);
}

const u16 DAT_060DC60C[] = {
    0x0415, 0x1010, 0x10FF, 0x0000, 0x0412, 0x1010, 0x10FF, 0x0000,
    0x0411, 0x1010, 0x10FF, 0x0000, 0x0417, 0x1010, 0xFF00, 0x0009,
    0x0414, 0x1010, 0xFF00, 0x0009, 0x0412, 0x1510, 0xFF00, 0x0009,
    0x0411, 0x1010, 0xFF00, 0x0009, 0x0415, 0x10FF, 0x0000, 0x0009,
    0x0412, 0x15FF, 0x0000, 0x0009, 0x0411, 0xFF00};

void func_060DC658(u16 arg0) {
    Entity* entity;
    Entity* current;
    Collider collider;

    entity = g_CurrentEntity;
    if (entity->velocityY < 0) {
        CheckCollision(
            entity->posX.val, entity->posY.val - FIX(7), &collider, 0);
        if (collider.effects & 5) {
            entity->velocityY = 0;
        }
    }

    CheckCollision(entity->posX.val, entity->posY.val + FIX(8), &collider, 0);
    if (arg0) {
        if (!(collider.effects & 5)) {
            MoveEntity(entity);
            func_06079BE4(entity);
            return;
        }

        entity->velocityX = 0;
        entity->velocityY = 0;
        if (collider.effects & 4) {
            entity->posY.val += FIX(0.125);
        } else {
            entity->posY.val += collider.unk18;
        }
        entity->posY.i.hi++;
        entity->posY.i.lo = 0;
    } else if (!(collider.effects & 5)) {
        MoveEntity(entity);
        current = g_CurrentEntity;

        if (current->velocityY >= 0) {
            current->ext.equipItemDrop.fallSpeed +=
                current->ext.equipItemDrop.gravity;
            current->velocityX = current->ext.equipItemDrop.fallSpeed;
            if (current->velocityX == FIX(1) || current->velocityX == FIX(-1)) {
                current->ext.equipItemDrop.gravity =
                    -current->ext.equipItemDrop.gravity;
            }
        }

        if (current->velocityY < FIX(0.25)) {
            current->velocityY += FIX(0.125);
        }
    }
}
void func_060DC764(u16 cardIndex) {
    Entity* self;
    Entity* player;
    PlayerStatus* status;
    s16 subWeaponId;
    u16 params;

    self = g_CurrentEntity;
    PlaySfx(0x67C);

    status = &g_Status;
    params = status->subWeapon;
    player = g_Entities;
    subWeaponId = DAT_060E9CFC[cardIndex];
    status->subWeapon = subWeaponId;

    if (params == subWeaponId) {
        params = 1;
        self->unk6D[0] = 0x10;
    } else {
        params = DAT_060E9AE0[params];
        self->unk6D[0] = 0x60;
    }

    if (params != 0) {
        self->params = params;
        self->posY.i.hi = player->posY.i.hi + 0xC;
        SetStep(7);
        self->unk0->flags |= 8;
        self->ext.subweaponCard.unk86 = 5;
        self->velocityY = -FIX(2.5);

        if (player->facingLeft != 1) {
            self->velocityX = -FIX(2.5);
            return;
        }
        self->velocityX = FIX(2.5);
    } else {
        DestroyEntity(self);
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DC840, func_060DC840);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DD09C, func_060DD09C);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DDD7C, func_060DDD7C);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DE0BC, func_060DE0BC);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DE6D0, func_060DE6D0);
s32 func_060DED40(Entity* entity) {
    Entity* player = &g_Entities[0];
    s16 distance;

    distance = (s16)(player->posX.i.hi - entity->posX.i.hi);
    distance = ABS(distance);
    if (distance > 22) {
        return 0;
    }

    distance = (s16)(player->posY.i.hi - entity->posY.i.hi);
    distance = ABS(distance);
    if (distance > 32) {
        return 0;
    }

    return 1;
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DED98, func_060DED98);
#include "set_entity_collision_tiles.h"

const u32 DAT_060DF1D4 = 0xCCCCCCCD;
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60DF1D8, func_060DF1D8);
extern s32 DAT_060E9DB0[];
extern const u8* DAT_060E9E5C[];

void func_060DF8AC(Entity* self) {
    SpriteObject* sprite;
    s32 paramsSnapshot;
    s32 variant;
    s32 priority;
    SpriteObject* current;

    paramsSnapshot = self->params;
    variant = 0xF & paramsSnapshot;
    if (self->step == 0) {
        TekiInit(self, 2);
        sprite = CreateSpriteObject(
            g_EntitySpriteBank01.allocationIndex, g_EntitySpriteBank01.flags,
            g_EntitySpriteBank01.images, DAT_060E9E70[variant]);
        self->unk0 = sprite;
        self->step++;
        self->pose = 0;
        self->poseTimer = 0;
        self->animSet = 2;
        self->unk1C = 0x30;
        if (paramsSnapshot & 0xF0) {
            sprite->clutBase = g_EntitySpriteBank05.flags | 0x8000;
            self->unk1C = 0x10;
        }
        priority = paramsSnapshot & 0xFF00;
        if (priority != 0) {
            sprite->zPriority = priority >> 8;
        }
        if (self->scaleX != 0) {
            sprite->flags |= 0x40;
            sprite->scaleX = (u32)self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = (u32)self->scaleY >> 2;
        }
        self->params &= 0xF;
        self->velocityY = DAT_060E9DB0[variant];
    } else {
        self->posY.val += self->velocityY;
        if (AnimateEntityWithSpriteData(
                self, DAT_060E9E5C[variant], DAT_06045E14) == 0) {
            DestroyEntity(self);
        }
    }
    current = self->unk0;
    if (current != NULL) {
        current->posX = self->posX.val;
        current->posY = self->posY.val;
    }
}

void func_060DFA14(Entity* self) {
    SpriteObject* sprite;
    SaturnSpriteResource* resource;
    u32 priority;

    if (self->step == 0) {
        TekiInit(self, 2);
        resource = &g_EntitySpriteBank05;
        self->step++;
        sprite = CreateSpriteObject(
            resource->allocationIndex, g_EntitySpriteBank01.flags,
            resource->images, 1);
        self->unk0 = sprite;
        func_0600B004(sprite, DAT_060465F4[1]);
        self->animCurFrame = 1;
        self->unk1C = 0x30;
        if (self->scaleX != 0) {
            sprite->flags |= 0x40;
            sprite->scaleX = (u32)self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = (u32)self->scaleY >> 2;
        }
        if (self->drawFlags & 4) {
            sprite->flags |= 0x40;
            sprite->rotate = self->rotate;
        }
        if (self->params & 0xF0) {
            sprite->clutBase = resource->flags | 0x8000;
            self->unk1C = 0x10;
        }
        priority = self->params & 0xFF00;
        if (priority != 0) {
            sprite->zPriority = priority >> 8;
        }
        sprite->zPriority += 8;
    } else {
        self->poseTimer++;
        self->posY.val -= FIX(0.25);
        if (!(self->poseTimer & 1)) {
            self->animCurFrame++;
            func_0600B004(self->unk0, DAT_060465F4[self->animCurFrame]);
        }
        if (self->poseTimer > 0x24) {
            DestroyEntity(self);
        }
    }
    func_06079BB4(self);
}
void func_060DFB7C(Entity* self) {
    Entity* particle;

    switch (self->step) {
    case 0:
        TekiInit(self, 2);
        self->step++;
        self->ext.explosionEmitter.parentId =
            self->ext.explosionEmitter.parent->entityId;
        /* fall through */
    case 1:
        if (self->ext.explosionEmitter.timer++ > 4U) {
            particle = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
            if (particle != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, particle);
                particle->entityId = E_EXPLOSION;
                particle->pfnUpdate = func_060DF8AC;
                particle->params = self->params;
            }
            self->ext.explosionEmitter.timer = 0;
        }
        self->posX.i.hi = self->ext.explosionEmitter.parent->posX.i.hi;
        self->posY.i.hi = self->ext.explosionEmitter.parent->posY.i.hi;
        if (self->ext.explosionEmitter.parent->entityId !=
            self->ext.explosionEmitter.parentId) {
            DestroyEntity(self);
        }
        break;
    }
}
extern u8 DAT_060E9EBC[];
extern s32 DAT_060E9EA4[];
extern u16 DAT_060E9EC0[];

void EntityDestructAnim(Entity* self) {
    SpriteObject* sprite;
    s32 variant;

    variant = self->params & 0xF;
    if (self->step == 0) {
        if (self->unk0 != NULL) {
            sprite = self->unk0;
        } else {
            sprite = CreateSpriteObject(
                g_EntitySpriteBank01.allocationIndex,
                g_EntitySpriteBank01.flags, g_EntitySpriteBank01.images,
                DAT_060E9E70[variant]);
        }
        self->unk0 = sprite;
        sprite->clutBase = g_EntitySpriteBank05.flags | 0x8000;
        self->animCurFrame = DAT_060E9EBC[variant];
        func_0600AFA8(sprite, DAT_06045E14[self->animCurFrame]);
        if (self->scaleX != 0) {
            sprite->flags |= 0x40;
            sprite->scaleX = (u32)self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = (u32)self->scaleY >> 2;
        }
        self->velocityY = DAT_060E9EA4[self->ext.destructAnim.index];
        self->flags = 0x0C002000;
        self->unk1C = 0x10;
        self->step++;
    } else {
        self->posY.val -= self->velocityY;
        self->poseTimer++;
        if (!(self->poseTimer & 1)) {
            self->animCurFrame++;
            func_0600B004(self->unk0, DAT_06045E14[self->animCurFrame]);
        }
        if (self->poseTimer > DAT_060E9EC0[variant]) {
            DestroyEntity(self);
        }
    }
    func_06079BB4(self);
}
extern u8 DAT_060E9EE1[];
extern s32 DAT_060E9EC8[];

void func_060DFDD4(Entity* self) {
    SpriteObject* sprite;
    s32 variant;

    variant = self->params & 0xF;
    if (self->step == 0) {
        if (self->unk0 != NULL) {
            sprite = self->unk0;
        } else {
            sprite = CreateSpriteObject(
                g_EntitySpriteBank05.allocationIndex,
                g_EntitySpriteBank05.flags, g_EntitySpriteBank05.images, 1);
        }
        self->unk0 = sprite;
        sprite->clutBase = g_EntitySpriteBank05.flags | 0x8000;
        self->animCurFrame = 1;
        func_0600AFA8(sprite, DAT_060465F4[1]);
        sprite->flags |= 0x40;
        sprite->scaleX = sprite->scaleY = DAT_060E9EE1[variant * 2];
        self->flags = 0x0C002000;
        self->unk1C = 0x10;
        self->drawFlags = 3;
        self->velocityY = DAT_060E9EC8[variant];
        self->step++;
    } else {
        self->posY.val -= self->velocityY;
        self->poseTimer++;
        if (!(self->poseTimer & 1)) {
            self->animCurFrame++;
            func_0600B004(self->unk0, DAT_060465F4[self->animCurFrame]);
        }
        if (self->poseTimer > 36) {
            DestroyEntity(self);
        }
    }
    func_06079BB4(self);
}
#define SPAWN_DESTRUCT_ANIM_ID 19
#include "spawn_destruct_anim.h"
extern u16 DAT_060466E8;

void func_060E0024(
    Entity* self, u8 count, u8 params, s16 x, s16 y, u8 variant, s16 xStep) {
    Entity* entity;
    SpriteObject* sprite;
    SaturnSpriteResource* resource;
    u16* clut;
    s32 i;
    s16 initialX;
    s16 currentY;

    i = 0;
    initialX = x + self->posX.i.hi;
    currentY = y + self->posY.i.hi;
    if (i < count) {
        resource = &g_EntitySpriteBank05;
        do {
            entity = AllocEntity(&g_Entities[0xA0], &g_Entities[0xC0]);
            if (entity != NULL) {
                entity->entityId = 20;
                entity->pfnUpdate = func_060DFDD4;
                entity->posX.i.hi = initialX + i * xStep;
                entity->posY.i.hi = currentY;
                entity->params = i;
                clut = &DAT_060466E8;
                sprite = CreateSpriteObject(
                    (u16)*clut, (u16)(s16)resource->flags, resource->images, 1);
                entity->unk0 = sprite;
                if (self->unk0 != NULL) {
                    sprite->zPriority = self->unk0->zPriority + 1;
                }
            }
            i++;
        } while (i < count);
    }
}
extern u16 DAT_06046734;
extern u16 DAT_06046736;
u16 func_06007CE0(u16 index);
u16 func_0607B0D0(Entity* self, Entity* target);
u16 func_0607B134(u16 step, u16 angle, u16 target);
void func_0607B04C(Entity* self, u16 angle, s16 speed);
extern u8 DAT_060E9F0C[];
extern s16 DAT_060E9F1A[];
extern s16 DAT_060E9F4A[];

/* LookupTblNoToVram counterpart: src/saturn/game_1.c. */
static u16 LookupTblNoToVram(u16 clut) {
    if ((clut >> 14) & 1) {
        return func_06007CE0(clut & 0xFFF);
    } else {
        return SPR_2LookupTblNoToVram(clut & 0xFFF);
    }
}

void func_060E0104(Entity* self) {
    u16 angle;
    Primitive* prim;
    s32 primIndex;
    s16* spr;
    u16* texture;
    s16* destinationTpage;
    s16* frame;
    s32 direction;
    Entity* player;

    player = &g_Entities[0];
    switch (self->step) {
    case 0:
        primIndex = AllocPrimitives(1, 1);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        TekiInit(self, 1);
        self->step++;
        prim = &g_PrimBuf[primIndex];
        prim->drawMode = 8;
        prim->unk6 = LookupTblNoToVram(DAT_06046736);
        prim->priority = 0x80;
        self->flags |= 0x800000;
        self->primIndex = primIndex;
        self->animSet = 0;
        angle = func_0607B0D0(self, player);
        direction = 0;
        if (self->posY.i.hi > 120) {
            direction = 1;
        }
        if (self->posX.i.hi < player->posX.i.hi) {
            direction ^= 1;
        }
        if (direction) {
            angle -= DAT_060E9F4A[Random() & 7];
        } else {
            angle += DAT_060E9F4A[Random() & 7];
        }
        self->ext.et_060E0104.unk78 = angle;
        self->ext.et_060E0104.unk7C = 0x400;
        self->ext.et_060E0104.unk7A = 0;
        self->hitboxState = 0;
        break;
    case 1:
        if (++self->ext.et_060E0104.unk7E == 16) {
            self->hitboxState = 1;
        }
        if (self->hitFlags) {
            if (!g_Player.healKind) {
                g_Player.healKind = 1;
                g_Player.healAmount = 8;
            }
            DestroyEntity(self);
            return;
        }
        if (self->scaleX < 0x100) {
            self->scaleX = self->scaleY += 0x10;
        }
        if (self->ext.et_060E0104.unk7A < 0x200) {
            self->ext.et_060E0104.unk7A += 2;
        }
        if ((u16)self->ext.et_060E0104.unk7C < 0x800) {
            self->ext.et_060E0104.unk7C += 4;
        }
        angle = func_0607B0D0(self, player);
        self->ext.et_060E0104.unk78 = angle = func_0607B134(
            self->ext.et_060E0104.unk7A, self->ext.et_060E0104.unk78, angle);
        func_0607B04C(self, angle, self->ext.et_060E0104.unk7C);
        MoveEntity(self);
        prim = &g_PrimBuf[self->primIndex];
        AnimateEntity(DAT_060E9F0C, self);
        frame = &self->animCurFrame;
        destinationTpage = &prim->unk8;
        angle = *frame - 1;
        texture = DAT_0605aec0[DAT_06046734 + angle];
        *destinationTpage = texture[0];
        prim->unkA = texture[1];
        angle = angle << 2;
        spr = &DAT_060E9F1A[angle];
        prim->x0 = self->posX.i.hi + spr[0];
        prim->y0 = self->posY.i.hi + spr[1];
        prim->x2 = prim->x0 + spr[2];
        prim->y2 = prim->y0 + spr[3];
        prim->drawMode = 0;
        break;
    }
}

INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E03F0, func_060E03F0);
#include "init_solid_prop.h"
void func_060E09D4(void) { DestroyEntity(); }
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E09EC, func_060E09EC);

void func_060E16B8(void* arg0) {
    Entity* self;
    s32* state_ptr;
    BupStat sttb;
    s32 state;
    s32 status;
    s32 count;
    s8 item;

    self = arg0;
    state_ptr = &self->ext.save.unk4;
    state = *state_ptr;

    if (state == 10)
        goto L_case10;
    if (state > 10) {
        if (state == 30)
            goto L_case30;
    }
    return;

L_case10_success:
    DAT_060485C0.unk5 = count;
    DAT_060485C0.unk4 = item;
    self->ext.save.unk4 = 30;
    goto L_case10_after;

L_case10:
    item = DAT_060485C0.unk4;
    count = 1;

    for (;;) {
        status = func_0600D028(item, count);
        if (status == 0 || status == BUP_BROKEN) {
            count += 1;
            if (count > 5)
                break;
        } else {
            goto L_case10_success;
        }
    }

L_case10_after:
    if (count == 6) {
        self->step = 1;
        self->ext.save.unk4 = 41;
    }
    return;

L_case30:
    item = DAT_060485C0.unk4;
    count = DAT_060485C0.unk5;

    status = BUP_Stat(item, 70, &sttb);
    if (status == BUP_UNFORMAT) {
        self->ext.save.unk4 = 43;
        return;
    }

    status = func_0600D028(item, count);
    if (status == BUP_NOT_FOUND && sttb.freeblock <= 0x4D) {
        self->ext.save.unk4 = 44;
        return;
    }

    if (func_0600D264(item, count) == 0 && func_0600D264(item, count) == 0 &&
        func_0600D47C(item, count) == 0) {
        self->ext.save.unk4 = 40;
    } else {
        self->ext.save.unk4 = 45;
    }
}

u16 func_060E17D0(s32 minX, s32 maxX) {
    u16 result;

    g_Player.unk7A = 1;
    if (PLAYER.step != 0 || (result = PLAYER.step_s, result != 1) ||
        PLAYER.posX.i.hi < minX || PLAYER.posX.i.hi > maxX) {
        result = 0;
    }
    return result;
}
void func_060E1814(void) {
    g_Player.padSim = PAD_UP;
    g_Player.demo_timer = 1;
}
void func_060E1834(Entity* self) {
    Primitive* prim = self->ext.save.unk10;
    s32 i;

    for (i = 0; i < 9; i++) {
        prim->x3 = 0;
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E1860, func_060E1860);
void func_060E1C64(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_060E1CB4(arg1, arg3);
    func_060E1E74(arg1, arg2, arg3);
    func_060E1FC0(arg0, arg1, arg3);
}

/* TU-local view of the model scratch buffer; keep DAT_060485E0 as s32[]. */
typedef struct {
    MthXyz vertices[14];
    MthXyz faceVertices[24][3];
    XyInt projected[72];
    MthXyz* translations;
    u8 (*faces)[4];
    s32 faceCount;
} RStage16ModelWorkspace;

extern MthXyz DAT_06092FF8[];
extern s32 DAT_060E9F70[];
extern u8 DAT_060EA0A8[][4];
extern u8 DAT_060EA108[][4];
extern u8 DAT_060EA168[][4];
extern u8 DAT_060EA210[][2];

void func_060E1CB4(s32 arg0, s32 arg1) {
    RStage16ModelWorkspace* workspace;
    s32* destinationX;
    s32* zeroDestinationY;
    s32* oneDestinationY;
    s32* threeDestinationY;
    s32* zeroSource;
    s32* zeroSourceY;
    s32* oneSourceY;
    s32* threeSource;
    s32 zeroLast;
    s32 oneLast;
    s32* base;
    s32* baseOrigin;
    s32 i;
    s32 threeLast;
    MthXyz* translations;
    u8* map0;
    u8* map1;

    workspace = (RStage16ModelWorkspace*)DAT_060485E0;
    translations = DAT_06092FF8;
    destinationX = &workspace->vertices[0].x;
    switch (arg0) {
    case 0:
        workspace->translations = translations;
        workspace->faceCount = 20;
        workspace->faces = DAT_060EA0A8;
        zeroSource = DAT_060E9F70;
        i = 0;
        zeroLast = 11;
        zeroSourceY = zeroSource + 1;
        zeroDestinationY = destinationX + 1;
        do {
            *destinationX = *zeroSource << 16;
            destinationX += 3;
            *zeroDestinationY = zeroSourceY[0] << 16;
            zeroDestinationY[1] = zeroSourceY[1] << 16;
            zeroSource += 3;
            zeroDestinationY += 3;
            zeroSourceY += 3;
            i++;
        } while (i <= zeroLast);
        destinationX[0] = destinationX[1] = destinationX[2] = 0;
        destinationX += 3;
        destinationX[0] = destinationX[1] = destinationX[2] = 0;
        break;
    case 1:
    case 2:
        workspace->translations = translations + 20;
        workspace->faceCount = 24;
        workspace->faces = DAT_060EA108;
        threeSource = DAT_060EA000;
        i = 0;
        oneLast = 13;
        oneSourceY = threeSource + 1;
        oneDestinationY = destinationX + 1;
        do {
            *destinationX = *threeSource << 16;
            destinationX += 3;
            *oneDestinationY = oneSourceY[0] << 16;
            oneDestinationY[1] = oneSourceY[1] << 16;
            threeSource += 3;
            oneDestinationY += 3;
            oneSourceY += 3;
            i++;
        } while (i <= oneLast);
        break;
    case 3:
        workspace->translations = translations + 20;
        workspace->faceCount = 24;
        workspace->faces = DAT_060EA168;
        map0 = &DAT_060EA210[0][0];
        arg1 <<= 12;
        i = 0;
        baseOrigin = DAT_060EA000;
        do {
            map1 = map0 + 1;
            threeDestinationY = destinationX + 1;
            threeSource = baseOrigin + *map0 * 3;
            base = DAT_060E9F70 + *map1 * 3;
            *destinationX =
                (base[0] << 16) + ((threeSource[0] - base[0]) * arg1) / 6;
            *threeDestinationY =
                (base[1] << 16) + ((threeSource[1] - base[1]) * arg1) / 6;
            threeLast = 13;
            threeDestinationY[1] =
                (base[2] << 16) + ((threeSource[2] - base[2]) * arg1) / 6;
            destinationX += 3;
            map0 += 2;
            i++;
        } while (i <= threeLast);
        break;
    }
}

INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E1E74, func_060E1E74);

INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E1FC0, func_060E1FC0);
void func_060E22C4(Primitive* arg0, s32 arg1, s32 arg2) {
    func_060E2300(arg1, arg2);
    func_060E23D0(arg0, arg1, arg2);
}
void func_060E2300(s32 scale, s32 unused) {
    s32* base;
    s32* srcX;
    s32* dstX;
    s32* srcY;
    s32* dstY;
    s32 factor;
    s32 scaledZ;
    s32 i;
    s32 last;

    base = DAT_060485E0;
    srcX = DAT_060EA000;
    dstX = base;
    factor = scale << 8;
    i = 0;
    last = 13;
    srcY = srcX + 1;
    dstY = dstX + 1;
    do {
        *dstX = *srcX * factor;
        i++;
        dstY[0] = srcY[0] * factor;
        srcX += 3;
        scaledZ = srcY[1] * factor;
        dstX += 3;
        srcY += 3;
        dstY[1] = scaledZ;
        dstY += 3;
    } while (i <= last);

    SetCurrentMatrixBinAngle(DAT_060EFCC0, DAT_060EFCD0);

    DAT_06061DF0.current->val[0][0] = DAT_06061DF0.current->val[0][0] * 5 / 4;
    DAT_06061DF0.current->val[0][1] = DAT_06061DF0.current->val[0][1] * 5 / 4;
    DAT_06061DF0.current->val[0][2] = DAT_06061DF0.current->val[0][2] * 5 / 4;

    TransformAndProjectPoints((MthXyz*)base, (XyInt*)((char*)base + 0x408), 14);
}

extern u8 DAT_060EA1C8[][4];

void func_060E23D0(Primitive* prim, s32 arg1, s32 arg2) {
    RStage16ModelWorkspace* workspace;
    SprGourTbl* gouraud;
    SprGourTbl* second;
    u16 gouraudIndex;
    XyInt* p0;
    XyInt* p1;
    XyInt* p2;
    u8* face;
    u16 value;
    s32 visible;
    s32 i;

    workspace = (RStage16ModelWorkspace*)DAT_060485E0;
    i = 0;
    do {
        if (arg1 == 0) {
            prim->drawMode = 8;
        } else {
            face = DAT_060EA1C8[i];
            p0 = &workspace->projected[face[0]];
            p1 = &workspace->projected[face[1]];
            p2 = &workspace->projected[face[2]];
            visible = func_0600E61C(p0, p1, p2);
            LOW(prim->x0) = LOW(p0->x);
            LOW(prim->x1) = LOW(p1->x);
            if (visible != 0) {
                prim->priority = 0x93;
            } else {
                prim->priority = 0x8D;
            }
            if (arg2 != 0) {
                gouraudIndex = prim->unk1C;
                gouraud = &SpGourTbl[gouraudIndex];
                second = gouraud;
                if (arg2 & 0x80) {
                    value = (u32)(arg2 & 0x7F) >> 3;
                    SpGourTbl[gouraudIndex].entry[0] = gouraud->entry[1] =
                        RGB16_COLOR(value, value, value);
                } else if (arg2 <= 0x10) {
                    value = 0x10 - arg2;
                    SpGourTbl[gouraudIndex].entry[0] =
                        RGB16_COLOR(value, value, value);
                } else if (arg2 > 0x20) {
                    prim->drawMode = 8;
                    goto next;
                } else {
                    value = 0x20 - arg2;
                    second->entry[1] = RGB16_COLOR(value, value, value);
                }
            }
            prim->drawMode = 0x35;
        }
    next:
        i++;
        prim = prim->next;
    } while (i <= 0x11);
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E253C, func_060E253C);
extern SaturnSpriteImage DAT_060ECC10[];
extern u8 DAT_060EA316[];
extern u32 DAT_060ECA7C[];
extern s8 DAT_060EA3E4[][2];
extern u8 DAT_060EA414[][2];

void func_060E3244(Entity* self) {
    Entity* parent;
    Entity* entity;
    s32 frame;
    parent = self - 1;
    if (parent->entityId != 0x15 || parent->step == 0x12) {
        DestroyEntity(self);
        return;
    }
    if (self->step == 0) {
        self->unk0 =
            CreateSpriteObject(g_RStage16SpriteBank18.allocationIndex,
                               g_RStage16SpriteBank18.flags, DAT_060ECC10, 4);
        TekiInit(self, 0x18D);
        self->attack /= 3;
        self->unk0->zPriority = parent->unk0->zPriority + 1;
        self->step++;
    }
    self->posX.i.hi = parent->posX.i.hi;
    self->posY.i.hi = parent->posY.i.hi;
    self->facingLeft = parent->facingLeft;
    if (!self->facingLeft) {
        self->unk0->flags &= ~0x10;
    } else {
        self->unk0->flags |= 0x10;
    }
    if (self->params) {
        AnimateEntityWithSpriteData(self, DAT_060EA316, DAT_060ECA7C);
        if (!(g_Timer & 1)) {
            entity = AllocEntity(&g_Entities[0xA0], &g_Entities[0xC0]);
            if (entity != NULL) {
                CreateEntityFromEntity(0x17, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->animCurFrame = self->animCurFrame;
                entity->zPriority = self->unk0->zPriority;
            }
        }
    }
    frame = (u8)self->animCurFrame - 0x4D;
    if ((u8)frame <= 0x17) {
        self->hitboxOffX = DAT_060EA3E4[frame][0];
        self->hitboxOffY = -DAT_060EA3E4[frame][1];
        self->hitboxWidth = DAT_060EA414[frame][0];
        self->hitboxHeight = DAT_060EA414[frame][1];
        self->hitboxState = parent->hitboxState;
    } else {
        self->hitboxState = 0;
    }
    self->unk0->posX = self->posX.val;
    self->unk0->posY = self->posY.val + FIX(32);
}
void func_060E3438(Entity* self) {
    switch (self->step) {
    case 0:
        self->unk0 =
            CreateSpriteObject(g_RStage16SpriteBank18.allocationIndex,
                               g_RStage16SpriteBank18.flags, DAT_060ECC10, 4);
        func_06079BB4(self);
        self->unk0->flags |= 0x40;
        TekiInit(self, 5);
        self->step++;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0x60;
        self->ext.et_060E4664.unk9C = 0;
        self->unk0->zPriority = self->zPriority - 2;
        func_0600B004(self->unk0, DAT_060ECA7C[self->animCurFrame]);
        if (self->facingLeft == 0) {
            self->unk0->flags &= ~0x10;
        } else {
            self->unk0->flags |= 0x10;
        }
        /* fallthrough */
    case 1:
        if (!(self->ext.et_060E4664.unk9C & 3)) {
            self->unk0->zPriority--;
        }
        if (self->facingLeft != 0) {
            self->posX.val -= FIX(0.25);
        } else {
            self->posX.val += FIX(0.25);
        }
        self->posY.val -= FIX(0.75);
        if (self->ext.et_060E4664.unk9C <= 11) {
            self->opacity = 0x60 - self->ext.et_060E4664.unk9C * 6;
        } else {
            self->step++;
        }
        switch (self->ext.et_060E4664.unk9C) {
        case 4:
            self->unk0->flags |= 0x80;
            break;
        case 8:
            self->drawFlags |= ENTITY_BLINK;
            break;
        }
        self->ext.et_060E4664.unk9C++;
        break;
    case 2:
        DestroyEntity(self);
        return;
    }
    self->unk0->posX = self->posX.val;
    self->unk0->posY = self->posY.val + FIX(32);
}
void func_060E35E0(Entity* entity, s16 step) {
    entity->pose = 0;
    entity->poseTimer = 0;
    entity->step = step;
    entity->step_s = 0;
}

const u16 DAT_060E3600[] = {0x8888, 0x8889, 0xAAAA, 0xAAAB};
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E3608, func_060E3608);
extern SaturnSpriteImage DAT_060ED840[];
extern u32 DAT_060ED788[];

void func_060E4664(Entity* self) {
    Entity* entity;

    entity = self->ext.et_060E4664.unkA0;
    switch (self->step) {
    case 0:
        self->unk0 = CreateSpriteObject(
            g_RStage16SpriteBankWraith.allocationIndex,
            g_RStage16SpriteBankWraith.flags, DAT_060ED840, 7);
        func_06079BB4(self);
        self->unk0->flags |= 0x40;
        TekiInit(self, 5);
        self->step++;
        self->drawFlags = ENTITY_OPACITY | ENTITY_MASK_R;
        self->opacity = 0x80;
        self->ext.et_060E4664.unk9C = 0;
        self->unk0->zPriority = entity->unk0->zPriority - 2;
        func_0600B004(self->unk0, DAT_060ED788[self->animCurFrame]);
        if (self->facingLeft == 0) {
            self->unk0->flags &= ~0x10;
        } else {
            self->unk0->flags |= 0x10;
        }
        break;
    case 1:
        self->ext.et_060E4664.unk9C++;
        if (!(self->ext.et_060E4664.unk9C & 3)) {
            self->unk0->zPriority--;
        }
        if (self->facingLeft != 0) {
            self->posX.val -= FIX(0.125);
        } else {
            self->posX.val += FIX(0.125);
        }
        self->posY.val += FIX(0.5);
        if (self->ext.et_060E4664.unk9C <= 24) {
            self->opacity = 0x80 - self->ext.et_060E4664.unk9C * 4;
        } else {
            self->step++;
        }
        switch (self->ext.et_060E4664.unk9C) {
        case 8:
            self->unk0->flags |= 0x80;
            break;
        case 16:
            self->drawFlags |= ENTITY_BLINK;
            break;
        }
        break;
    case 2:
        DestroyEntity(self);
        return;
    }
    func_06079BB4(self);
}
/* Function/global declarations and TU-local primitive scratch view. */
typedef struct {
    s16 unk0;
    s16 unk2;
    SotnFixed32 posX;
    SotnFixed32 posY;
    s32 velocityX;
    s32 velocityY;
    s32 : 32;
    s32 : 32;
    s16 : 16;
    u16 drawMode;
    Primitive* next;
} RStage16ParticleWork;
extern Primitive g_PrimBuf[];

void func_060E4804(Entity* self) {
    Primitive* prim;
    Primitive** primPtr;
    RStage16ParticleWork* work;
    s32 primIndex;
    s32 magnitude;
    s16 angle;
    s32 initialAngle;
    s16 speed;
    s32 deltaX;
    s32 deltaY;
    u16 blue;
    u16 green;
    u16 red;
    u16 color;

    primPtr = &self->ext.et_060E3608.unk7C;
    switch (self->step) {
    case 0:
        TekiInit(self, 5);
        primIndex = AllocPrimitives(3, 64);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        *primPtr = prim;
        while (prim != NULL) {
            work = (RStage16ParticleWork*)prim->next;
            work->unk0 = 0x100F;
            work->drawMode = 8;
            prim->priority = 0x80;
            prim->drawMode = 0;
            Random();
            prim->unk6 = 0x8008;
            magnitude = (MTH_GetRand() & 0x7FFF) + 0x8000;
            initialAngle = MTH_GetRand() % 0xFFF;
            work->velocityX = (magnitude * rcos(initialAngle)) >> 13;
            work->velocityY =
                ((magnitude * rsin(initialAngle)) >> 12) - 0x12000;
            if (self->facingLeft == 0) {
                work->posX.val = self->posX.val - FIX(64);
            } else {
                work->posX.val = self->posX.val + FIX(61);
            }
            work->posY.val = self->posY.val;
            prim = prim->next;
            prim = prim->next;
        }
        self->ext.et_060E3608.unk90 = 0;
        self->step++;
        /* fallthrough */
    case 1:
        self->ext.et_060E3608.unk90++;
        blue = self->ext.et_060E3608.unk90 / 2 + 16;
        green = self->ext.et_060E3608.unk90 / 3 + 16;
        if (green > 31) {
            green = 31;
        }
        if (blue > 31) {
            blue = 31;
        }
        red = 0;
        if (blue == 31) {
            red = self->ext.et_060E3608.unk90 / 2;
        }
        if (red > 31) {
            red = 31;
        }
        color = RGB16_COLOR(red, green, blue);
        prim = self->ext.et_060E3608.unk7C;
        while (prim != NULL) {
            if (!(prim->drawMode & 8)) {
                work = (RStage16ParticleWork*)prim->next;
                deltaX = self->posX.val - work->posX.val;
                deltaY = self->posY.val - work->posY.val;
                if ((u32)(deltaX + 15) <= 30 && (u32)(deltaY + 15) <= 30) {
                    prim->drawMode |= 8;
                }
                angle = ratan2(deltaY, deltaX);
                speed = self->ext.et_060E3608.unk90 / 5;
                work->velocityX += speed * rcos(angle) / 8;
                work->velocityY += speed * rsin(angle) / 4;
                work->posX.val += work->velocityX;
                work->posY.val += work->velocityY;
                prim->x0 = prim->x3 = work->posX.i.hi;
                prim->x1 = prim->x2 = prim->x0 + 2;
                prim->y0 = prim->y1 = work->posY.i.hi;
                prim->y3 = prim->y2 = prim->y0 + 2;
                prim->unk6 = color;
            }
            prim = prim->next;
            prim = prim->next;
        }
        if (self->ext.et_060E3608.unk90 > 58) {
            DestroyEntity(self);
        }
        break;
    }
}

INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E4AFC, func_060E4AFC);
void func_060E4E50(Entity* entity) {
    entity->unk0 =
        CreateSpriteObject(g_RStage16SpriteBankWight.allocationIndex,
                           g_RStage16SpriteBankWight.flags, DAT_060EDE4C, 5);
    SyncSpriteObjectPosUnchecked(entity, DAT_060ED9FA);
    entity->step++;
}
void func_060E4EA4(Entity* self) {
    func_06079BB4(self);
    func_0600B004(self->unk0, DAT_060EDDE4[self->animCurFrame]);
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E4EDC, func_060E4EDC);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E56DC, func_060E56DC);
extern s16 DAT_060EDA28[];
extern s16 DAT_060EDA2C[];
u8 func_0607A118(Entity* self, s16* sensors, s32 count);

void func_060E5CB0(Entity* self) {
    s32 primIndex;
    s32 idx;
    SprGourTbl* temp;
    Primitive* prim;
    u16* flags;
    void (*syncSpriteObjectPos)(Entity*, s16*);
    SaturnSpriteImage* images = DAT_060EDE4C;
    switch (self->step) {
    case 0:
        flags = &g_RStage16SpriteBankWight.flags;
        self->unk0 = CreateSpriteObject(
            g_RStage16SpriteBankWight.allocationIndex, *flags, images, 1);
        syncSpriteObjectPos = SyncSpriteObjectPosUnchecked;
        syncSpriteObjectPos(self, DAT_060EDA28);
        TekiInit(self, 2);
        self->animCurFrame = 0;
        primIndex = AllocPrimitives(0x8005, 1);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        self->hitboxState = 0;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_060E5CB0.prim = prim;
        prim->x0 = prim->x1 = self->posX.i.hi;
        prim->y0 = prim->y1 = self->posY.i.hi;
        idx = prim->unk1C;
        temp = &SpGourTbl[idx];
        temp->entry[0] = RGB16_COLOR(31, 0, 6);
        temp->entry[1] = RGB16_COLOR(4, 0, 2);
        prim->unk6 = 0xC210;
        prim->priority = 0x80;
        prim->drawMode = 0x37;
        self->step = 1;
        break;
    case 1:
        prim = self->ext.et_060E5CB0.prim;
        if (func_0607A118(self, DAT_060EDA2C, 0)) {
            prim->y1 += 2;
            if (self->step_s == 0) {
                self->step_s = 1;
            }
        } else {
            self->velocityY += 0x1800;
            self->posY.val += self->velocityY;
            if (prim->y0 - prim->y1 > 8) {
                prim->y1 = prim->y0 - 8;
            }
        }
        prim->x0 = self->posX.i.hi;
        prim->x1 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        if (prim->y0 < prim->y1) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}
void func_060E5E70(Entity* entity) {
    SpriteObject* (*createSpriteObject)(u16, u16, SaturnSpriteImage*, s32);
    void (*syncSpriteObjectPos)(Entity*, s16*);
    u16* bank;
    s32 maxParts;

    switch (entity->step) {
    case 0:
        bank = &g_RStage16SpriteBankWight.flags;
        createSpriteObject = CreateSpriteObject;
        maxParts = 5;
        entity->unk0 =
            createSpriteObject(bank[-1], bank[0], DAT_060EDE4C, maxParts);
        syncSpriteObjectPos = SyncSpriteObjectPosUnchecked;
        syncSpriteObjectPos(entity, DAT_060EDA60);
        TekiInit(entity, 2);
        entity->step = 1;
        entity->hitboxState = 0;
    case 1:
        if (AnimateEntityWithSpriteData(entity, DAT_060EDA34, DAT_060EDDE4) ==
            0) {
            DestroyEntity(entity);
        }
        break;
    }
}
void func_060E5F0C(Entity* self) {
    func_06079BB4(self);
    func_0600B004(self->unk0, DAT_060EEFD0[self->animCurFrame]);
}

const u32 DAT_060E5F44 = 0xAAAAAAAB;
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E5F48, func_060E5F48);
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E660C, func_060E660C);
void func_060E6828(Entity* self) {
    if (self->step == 0) {
        self->unk0 = CreateSpriteObject(
            g_RStage16SpriteBankGargoyle[0].allocationIndex,
            g_RStage16SpriteBankGargoyle[0].flags, DAT_060EF0C0, 1);
        SyncSpriteObjectPosUnchecked(self, &DAT_060EDEBC);
        self->step = 1;
        TekiInit(self, 5);
        self->ext.gargoyle.clutBase = self->unk0->clutBase;

        switch (self->ext.gargoyle.variant) {
        case 1:
            self->animCurFrame = 1;
            self->unk0->clutBase = self->ext.gargoyle.clutBase;
            break;
        case 2:
            self->animCurFrame = 0x3A;
            self->unk0->clutBase = self->ext.gargoyle.clutBase + 9;
            break;
        case 0:
            self->animCurFrame = 0x3B;
            self->unk0->clutBase = self->ext.gargoyle.clutBase + 0x12;
            break;
        }

        self->unk0->zPriority -= 2;
        func_06079BB4(self);
        func_0600B004(self->unk0, DAT_060EEFD0[self->animCurFrame]);
    }
}
void func_060E6930(Entity* self) {
    u8 sensorHit;
    u16 collision;

    sensorHit = func_06079DEC(self, &DAT_060EF2B4);
    collision = func_06079F60(self, &DAT_060EF2BC, 3);
    if (sensorHit == 0x80 || (collision & 2)) {
        SetStep(5);
        return;
    }
    if (self->ext.afterImage.disableFlag == 0) {
        SetStep(4);
        return;
    }
    self->ext.afterImage.disableFlag--;
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E69A4, func_060E69A4);
extern s16 DAT_060EF230[];
extern SaturnSpriteImage DAT_060EF568[];
extern SaturnSpriteFrameHeader* DAT_060EF510[];

void func_060E6F38(Entity* self) {
    Entity* entity;
    SaturnSpriteResource* bank;
    SaturnSpriteImage* images;

    if (self->step != 0) {
        if (--self->ext.et_060E6F38.timer) {
            self->rotate += DAT_060EF230[self->params];
            func_06079BE4(self);
            MoveEntity(self);
        } else {
            entity = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
            if (entity != NULL) {
                CreateEntityFromEntity(2, self, entity);
                entity->params = 0;
            }
            DestroyEntity(self);
            return;
        }
        self->unk0->flags |= 0x40;
        self->unk0->rotate = self->rotate;
    } else {
        self->step++;
        images = DAT_060EF568;
        bank = &g_RStage16SpriteBank22;
        self->unk0 =
            CreateSpriteObject(bank->allocationIndex, bank->flags, images, 1);
        self->unk0->flags |= 0x40;
        TekiInit(self, 2);
        self->animCurFrame = (self->params & 0xFF) + 0xF;
        if (self->params & 0x100) {
            self->palette++;
            self->unk0->clutBase++;
        }
        func_06079BB4(self);
        func_0600AFA8(self->unk0, DAT_060EF510[self->animCurFrame]);
        self->drawFlags = 4;
        if (self->facingLeft) {
            self->velocityX = -self->velocityX;
        }
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E70A0, func_060E70A0);
void func_060E7338(MthXyz* rotation, MthXyz* position) {
    MthMatrix* current;
    s32 angle;

    MTH_ClearMatrix(&DAT_06061DF0);
    current = DAT_06061DF0.current;
    current->val[0][3] = position->x;
    current->val[1][3] = position->y;
    current->val[2][3] = position->z;

    angle = rotation->y & 0x0FFF;
    if (angle != 0) {
        angle = (angle << 12) + (angle << 10) + (angle << 9) + (angle << 7);
        if (angle > 0xB3FFFF) {
            angle += 0xFE980000;
        }
        MTH_RotateMatrixY(&DAT_06061DF0, angle);
    }

    angle = rotation->z & 0x0FFF;
    if (angle != 0) {
        angle = (angle << 12) + (angle << 10) + (angle << 9) + (angle << 7);
        if (angle > 0xB3FFFF) {
            angle += 0xFE980000;
        }
        MTH_RotateMatrixZ(&DAT_06061DF0, angle);
    }
}
extern MthXy DAT_06061DE0;
extern XyInt DAT_06061DE8;
extern MthXyz DAT_060EF6F4[];
extern s32 DAT_060EFCE0;

void func_060E7400(Entity* self) {
    MthXyz position;
    MthXyz rotation;
    XyInt projected[4];
    s32 count;
    s16* source;
    s16* dest;
    s16* destNext;
    s16 i;
    s32 x;
    s32 y;
    Primitive* prim;
    Primitive* next;
    s32 angle;
    s32 remainder;
    MthXy* projection;

    prim = self->ext.et_060E7698.unk78;
    next = prim->next;
    switch (self->ext.et_060E7698.unk84) {
    case 0:
        self->ext.et_060E7698.unk88 = 0;
        prim->drawMode = 2;
        next->drawMode = 2;
        self->ext.et_060E7698.unk98.val = 0;
        self->ext.et_060E7698.unk9C = 0;
        self->ext.et_060E7698.unk84 = 1;
        goto draw;
    case 1:
        self->ext.et_060E7698.unk8A -= 0x55;
        self->ext.et_060E7698.unk88 += 0x200;
        self->ext.et_060E7698.unk98.val += 0x20;
        if (++self->ext.et_060E7698.unk9C > 40) {
            prim->drawMode |= 8;
            next->drawMode |= 8;
            self->ext.et_060E7698.unk84 = 16;
            return;
        }
    draw:
        angle = (s16)(self->ext.et_060E7698.unk98.i.lo + 0x200);
        projection = &DAT_06061DE0;
        projection->x = projection->y = angle << 8;
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        if (self->facingLeft) {
            x += 10;
            y -= 2;
        } else {
            x -= 10;
            y -= 2;
        }
        DAT_06061DE8.x = x;
        DAT_06061DE8.y = y;
        rotation.x = 0;
        if (self->facingLeft) {
            rotation.y = -0x2E0;
        } else {
            rotation.y = 0x2E0;
        }
        rotation.z = self->ext.et_060E7698.unk8A;
        position.x = 0;
        position.y = 0;
        position.z = 0x2000000;
        func_060E7338(&rotation, &position);
        prim = self->ext.et_060E7698.unk78;
        next = prim->next;
        source = (s16*)projected;
        TransformAndProjectPoints(DAT_060EF6F4, projected, 4);
        dest = &prim->x0;
        destNext = &next->x0;
        i = 0;
        do {
            *dest = *destNext = *source++;
            i++;
            dest++;
            destNext++;
        } while (i <= 7);
        count = self->ext.et_060E7698.unk9C;
        remainder = count % (count >> 3);
        if (remainder == 0) {
            prim->drawMode = 2;
            DAT_060EFCE0 = remainder;
        } else {
            prim->drawMode |= 8;
            DAT_060EFCE0++;
        }
        if (DAT_060EFCE0 == 1 || DAT_060EFCE0 == 3) {
            next->drawMode = 2;
        } else {
            next->drawMode |= 8;
        }
        break;
    case 16:
        break;
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E7698, func_060E7698);
extern s16 DAT_060EF674[];
extern SaturnSpriteImage DAT_060EFC20[];
extern SaturnSpriteFrameHeader* DAT_060EFB84[];

void func_060E7C60(Entity* self) {
    Entity* entity;
    SaturnSpriteResource* bank;
    SaturnSpriteImage* images;

    if (self->step != 0) {
        if (--self->ext.et_060E7C60.timer) {
            self->rotate += DAT_060EF674[self->params];
            func_06079BE4(self);
            MoveEntity(self);
        } else {
            entity = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
            if (entity != NULL) {
                CreateEntityFromEntity(2, self, entity);
                entity->params = 0;
            }
            DestroyEntity(self);
            return;
        }
        self->unk0->flags |= 0x40;
        self->unk0->rotate = self->rotate;
    } else {
        self->step++;
        images = DAT_060EFC20;
        bank = &g_RStage16SpriteBank23;
        self->unk0 =
            CreateSpriteObject(bank->allocationIndex, bank->flags, images, 1);
        self->unk0->flags |= 0x40;
        TekiInit(self, 0x7E);
        self->hitboxState = 0;
        self->flags |= 0xC0202000;
        self->animCurFrame = self->params + 0x1D;
        self->drawFlags = 4;
        if (self->facingLeft) {
            self->velocityX = -self->velocityX;
        }
        func_06079BB4(self);
        func_0600AFA8(self->unk0, DAT_060EFB84[self->animCurFrame]);
    }
}
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E7DB8, func_060E7DB8);
void func_060E81F4(Entity* self) {
    SaturnSpriteResource* bank;
    SaturnSpriteImage* images;
    s32 remaining;
    s32* distance;

    switch (self->step) {
    case 0:
        self->step++;
        images = DAT_060EFC20;
        bank = &g_RStage16SpriteBank23;
        self->unk0 =
            CreateSpriteObject(bank->allocationIndex, bank->flags, images, 1);
        self->unk0->flags |= 0x40;
        TekiInit(self, 0x7F);
        self->hitboxState = 0;
        self->animCurFrame = 0x24;
        self->drawFlags |= 3;
        self->scaleX = self->scaleY = 0x10;
        if (self->facingLeft) {
            self->velocityX = FIX(10);
        } else {
            self->velocityX = FIX(-10);
        }
        self->unk0->zPriority = 0xA8;
        func_06079BB4(self);
        func_0600B004(self->unk0, DAT_060EFB84[self->animCurFrame]);

        /* fallthrough */
    case 1:
        MoveEntity(self);
        distance = &self->ext.et_060E81F4.unk90;
        remaining = ABS(self->velocityX);
        *distance += remaining;
        self->scaleX = self->scaleY += 0x40;
        if (self->scaleX < 0x100) {
            return;
        }
        self->step++;
        return;
    case 2:
        MoveEntity(self);
        distance = &self->ext.et_060E81F4.unk90;
        remaining = ABS(self->velocityX);
        *distance += remaining;
        remaining = ((self->ext.et_060E81F4.unk8C + 0x20) << 16) - *distance;
        if (remaining < 0) {
            DestroyEntity(self);
            return;
        }
        remaining >>= 16;
        remaining <<= 3;
        if (remaining > 0x100) {
            remaining = 0x100;
        }
        break;
    }
}

const u32 DAT_060E8364 = 0xCCCCCCCD;
INCLUDE_ASM("asm/saturn/rstage16/f_nonmat", f60E8368, func_060E8368);
