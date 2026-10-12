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
    const SaturnSpriteResource* bank;

    if (self->step == 0) {
        TekiInit(self, 5);
        self->step++;
        bank = &g_RStage16SpriteBank16;
        sprite = CreateSpriteObject(
            bank->allocationIndex, bank->flags, bank->images, 1);
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
            sprite->scaleX = self->scaleX >> 2;
        }
        if (self->drawFlags & 2) {
            sprite->flags |= 0x40;
            sprite->scaleY = self->scaleY >> 2;
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

    distance = (player->posX.i.hi - entity->posX.i.hi);
    distance = ABS(distance);
    if (distance > 22) {
        return 0;
    }

    distance = (player->posY.i.hi - entity->posY.i.hi);
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
            sprite->scaleX = self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = self->scaleY >> 2;
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
            sprite->scaleX = self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = self->scaleY >> 2;
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
            sprite->scaleX = self->scaleX >> 2;
        }
        if (self->scaleY != 0) {
            sprite->flags |= 0x40;
            sprite->scaleY = self->scaleY >> 2;
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
                    *clut, resource->flags, resource->images, 1);
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
u16 func_0607B0D0(Entity* self, Entity* target);
u16 func_0607B134(u16 step, u16 angle, u16 target);
void func_0607B04C(Entity* self, u16 angle, s16 speed);
extern u8 DAT_060E9F0C[];
extern s16 DAT_060E9F1A[];
extern s16 DAT_060E9F4A[];

u16 LocalLookupTblNoToVram(u16 tableNo);

static u16 LookupTblNoToVram(u16 clut) {
    if ((clut >> 14) & 1) {
        return LocalLookupTblNoToVram(clut & 0xFFF);
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
        self->flags |= FLAG_HAS_PRIMS;
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
void func_060E1860(Entity*, s32);
void func_060E1C64(s32, s32, s32, s32);
void func_060E1814(void);
void func_060E1834(Entity*);
void func_060E22C4(Primitive*, s32, s32);
void func_060E16B8(void*);
extern s32 DAT_060E9F6C;
static inline SetGeomScreen(u32 h) { DAT_06061DE0.x = DAT_06061DE0.y = h; }
static inline void SetGeomOffset(u16 ofx, u16 ofy) {
    DAT_06061DE8.x = ofx;
    DAT_06061DE8.y = ofy;
}

void func_060E09EC(Entity* self) {
    s32 orig;
    s32 iVar6;
    s32 uVar13;
    Entity* player;
    s32 primIndex;

    player = &PLAYER;
    orig = DAT_060EFCC0[1];
    switch (self->step) {
    case 0:
        TekiInit(self, 5);
        self->step++;
        primIndex = AllocPrimitives(2, 0x3B);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->primIndex = primIndex;
        self->flags |= FLAG_HAS_PRIMS;
        DAT_060E9F6C = 0;
        func_060E1860(self, 0);
        self->ext.save.unk0 = 0;
        self->ext.save.unk38 = 0;
        self->ext.save.unk4 = 0;
        if (DAT_0605DD60 != 0) {
            DAT_0605C668 = 0;
            self->ext.save.unk4 = 0x28;
            self->ext.save.unk24 = 0x100;
            self->ext.save.unk0 = 7;
            DAT_0605DD60 = 0;
        }
        break;
    case 1:
        break;
    default:
        return;
    }

    SetGeomScreen(0x10000);
    SetGeomOffset(0xA0, self->ext.save.unk30 + 0x80);

    switch (self->ext.save.unk0) {
    case 0:
        DAT_060EFCC0[1] += 6;
        uVar13 = g_GameTimer & 0x7F;
        if (uVar13 == 0) {
            PlaySfx(SFX_SAVE_HEARTBEAT);
        }
        if (uVar13 & 0x40) {
            uVar13 = 0x7F - uVar13;
        }
        if (uVar13 >= 8) {
            uVar13 = 0xC0;
        } else {
            uVar13 = uVar13 * 4 + 0xA0;
        }
        func_060E1C64(self->ext.save.unk8, 0, uVar13, 0);
        self->ext.save.unk24 = uVar13;
        if (func_060E17D0(0x78, 0xC8) != 0) {
            self->ext.save.unk28 = DAT_060485C0.unk4;
            self->ext.save.unk2C = DAT_060485C0.unk5;
            DAT_0605C668 = 0;
            if (DAT_060E9F6C == 0) {
                DAT_0605C658 = 0;
            }
            D_8013B5E8 = 0x10;
            g_Status.hp = g_Status.hpMax;
            g_Status.mp = g_Status.mpMax;
            self->ext.save.unk0++;
        }
        break;

    case 1:
        DAT_060EFCC0[1] += 7;
        if (self->ext.save.unk24 < 0xC0) {
            self->ext.save.unk24 += 4;
        }
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if ((self->ext.save.unk30 < (g_CurrentRoom.stageID >> 1 & 0x10) + 8) &&
            (g_GameTimer != 0)) {
            self->ext.save.unk30++;
        }
        if (player->posX.i.hi < 0x9E) {
            g_Player.padSim = PAD_RIGHT;
            g_Player.demo_timer = 1;
        } else if (player->posX.i.hi > 0xA0) {
            g_Player.padSim = PAD_LEFT;
            g_Player.demo_timer = 1;
        } else {
            if (self->ext.save.unk30 ==
                (g_CurrentRoom.stageID >> 1 & 0x10) + 8) {
                func_060E1814();
                self->ext.save.unk34 = 8;
                if ((DAT_060485C0.unk4 >= 0) && (DAT_060485C0.unk5 != 0)) {
                    func_060E1834(self);
                    self->ext.save.unk0 = 0x80;
                    if (DAT_060485C0.unk5 < 0) {
                        self->ext.save.unk2C = 0;
                        self->ext.save.unk38 = 1;
                        self->ext.save.unk0 = 2;
                        self->ext.save.unk4 = 10;
                    }
                } else {
                    DAT_060485C0.unk4 = 0;
                    func_060E1834(self);
                    self->ext.save.unk0 = 0x81;
                }
            } else {
                func_060E1814();
            }
        }
        break;

    case 0x80:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (func_06066B30(self, 3) != 0) {
            if (self->ext.save.unk1C == 0) {
                self->ext.save.unk38 = 2;
                self->ext.save.unk0 = 2;
                self->ext.save.unk4 = 0x1E;
            } else {
                func_060E1834(self);
                self->ext.save.unk0++;
            }
        }
        break;

    case 0x81:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (func_06066B30(self, 4) != 0) {
            if (self->ext.save.unk1C == 0) {
                DAT_060485C0.unk4 = 0;
                func_060E1834(self);
                self->ext.save.unk0 = 0x100;
            } else {
                DAT_060485C0.unk4 = self->ext.save.unk28;
                DAT_060485C0.unk5 = self->ext.save.unk2C;
                self->ext.save.unk0 = 0x101;
            }
        }
        break;

    case 0x100:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (DAT_0605DD94 != 0) {
            iVar6 = func_06066B30(self, 1);
            if (iVar6 == 0) {
                break;
            }
            if (iVar6 == 2) {
                self->ext.save.unk0++;
                break;
            }
            self->ext.save.unk38 = 1;
            self->ext.save.unk0 = 2;
            self->ext.save.unk4 = 10;
        } else {
            self->ext.save.unk38 = 1;
            self->ext.save.unk0 = 2;
            self->ext.save.unk4 = 10;
        }
        break;

    case 0x101:
        DAT_060EFCC0[1] += self->ext.save.unk34;
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (g_pads[0].pressed & 0x700) {
            DAT_060485C0.unk4 = self->ext.save.unk28;
            DAT_060485C0.unk5 = self->ext.save.unk2C;
            g_Player.padSim = 0;
            g_Player.demo_timer = 1;
        } else {
            DAT_0605C668 = 1;
            DAT_0605C658 = 1;
            self->ext.save.unk0 = 0;
        }
        break;

    case 2:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk24 += 4;
        if (self->ext.save.unk24 > 0x100) {
            self->ext.save.unk24 = 0x100;
        }
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (self->ext.save.unk24 == 0x100) {
            if ((self->ext.save.unk4 > 0x1D) || (DAT_060E9F6C != 0)) {
                self->ext.save.unk0++;
            } else {
                self->ext.save.unk34++;
                if (self->ext.save.unk34 > 0x80) {
                    self->ext.save.unk34 = 0x80;
                }
            }
        }
        break;

    case 3:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk34++;
        if (self->ext.save.unk34 > 0x80) {
            self->ext.save.unk34 = 0x80;
        }
        func_060E1C64(self->ext.save.unk8, 0, self->ext.save.unk24, 0);
        if (self->ext.save.unk34 == 0x80) {
            self->ext.save.unk0++;
            self->ext.save.unk20 = 0;
        }
        break;

    case 4:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk20 += 2;
        if (self->ext.save.unk20 > 0x7F) {
            self->ext.save.unk20 = 0x7F;
        }
        func_060E1C64(
            self->ext.save.unk8, 0, self->ext.save.unk24, self->ext.save.unk20);
        if (self->ext.save.unk20 >= 0x10) {
            uVar13 = (self->ext.save.unk20 - 0x10) * 0x10;
            if (uVar13 > 0x100) {
                uVar13 = 0x100;
            }
            func_060E22C4(self->ext.save.unkC, uVar13, 0);
        }
        if (self->ext.save.unk20 != 0x7F) {
            break;
        }
        if (self->ext.save.unk4 > 0x27 || DAT_060E9F6C != 0) {
            if ((DAT_060EFCC0[1] & 0xFFF) >= 0x100 &&
                (DAT_060EFCC0[1] & 0xFFF) < 0x180) {
                self->ext.save.unk0++;
            }
        }
        break;

    case 5:
        func_060E1814();
        if (self->ext.save.unk34 > 0x40) {
            self->ext.save.unk34--;
        } else {
            if ((DAT_060EFCC0[1] & 0x7FF) >= 0x400) {
                self->ext.save.unk34 = (0x800 - (DAT_060EFCC0[1] & 0x7FF)) / 16;
            }
        }
        if (self->ext.save.unk34 < 0x10) {
            self->ext.save.unk34 = 0x10;
        }
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk20 -= 2;
        if (self->ext.save.unk20 < 0) {
            self->ext.save.unk20 = 0;
        }
        func_060E1C64(
            self->ext.save.unk8, 1, self->ext.save.unk24, self->ext.save.unk20);
        if (self->ext.save.unk20 != 0) {
            func_060E22C4(self->ext.save.unkC, self->ext.save.unk24, 0);
        } else {
            func_060E22C4(self->ext.save.unkC, 0, 0);
        }

        if (self->ext.save.unk34 == 0x10 &&
            (DAT_060EFCC0[1] & 0x7FF) >= 0x7F0) {
            DAT_060EFCC0[1] = 0;
            func_060E1C64(self->ext.save.unk8, 1, self->ext.save.unk24, 0);
            func_060E1834(self);
            self->ext.save.unk0++;
        }
        break;

    case 6:
        func_060E1814();
        if (self->ext.save.unk4 > 0x27 && func_06066B30(self, 0)) {
            if (self->ext.save.unk4 == 0x2B) {
                func_060E1834(self);
                self->ext.save.unk0 = 0x200;
            } else if (self->ext.save.unk4 != 0x28) {
                self->ext.save.unk0 = 0x20;
                self->ext.save.unk20 = 0x60;
            } else {
                self->ext.save.unk0++;
            }
        }
        break;

    case 0x200:
        func_060E1814();
        if (func_06066B30(self, 2) != 0) {
            if (self->ext.save.unk1C == 0) {
                DAT_060485C0.unk5 = 0;
                PlaySfx(SFX_UI_CONFIRM);
                self->ext.save.unk0++;
            } else {
                self->ext.save.unk0 = 0x20;
                self->ext.save.unk20 = 0x60;
            }
        }
        break;

    case 0x201:
        func_060E1814();
        if (BUP_Format(DAT_060485C0.unk4) == 0) {
            self->ext.save.unk0 = 0x30;
            self->ext.save.unk20 = 0x60;
        } else {
            self->ext.save.unk4 = 0x2A;
            func_060E1834(self);
            self->ext.save.unk0++;
        }
        break;

    case 0x202:
        func_060E1814();
        if (func_06066B30(self, 0) != 0) {
            self->ext.save.unk0 = 0x20;
            self->ext.save.unk20 = 0x60;
        }
        break;

    case 7:
        func_060E1814();
        func_060E1C64(self->ext.save.unk8, 1, self->ext.save.unk24, 0);
        self->ext.save.unk0++;
        break;

    case 8:
        func_060E1814();
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk34 += 2;
        if (self->ext.save.unk34 > 0x40) {
            self->ext.save.unk34 = 0x40;
        }
        func_060E1C64(self->ext.save.unk8, 1, self->ext.save.unk24, 0);
        if (self->ext.save.unk34 == 0x40) {
            self->ext.save.unk0++;
            self->ext.save.unk20 = 0;
        }
        break;

    case 9:
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk20++;
        if (self->ext.save.unk20 > 0x7F) {
            self->ext.save.unk20 = 0x7F;
        }
        func_060E1C64(
            self->ext.save.unk8, 2, self->ext.save.unk24, self->ext.save.unk20);
        if (self->ext.save.unk20 >= 0x40) {
            func_060E22C4(self->ext.save.unkC, self->ext.save.unk24, 0);
            DAT_0605C668 = 1;
            DAT_0605C658 = 1;
        } else {
            func_060E22C4(self->ext.save.unkC, self->ext.save.unk24,
                          self->ext.save.unk20 * 2 + 0x80);
            func_060E1814();
        }
        if (self->ext.save.unk20 == 0x7F) {
            self->ext.save.unk0++;
            self->ext.save.unk20 = 0;
        }
        break;

    case 10:
        DAT_060EFCC0[1] += self->ext.save.unk34;
        self->ext.save.unk20++;
        if (self->ext.save.unk20 > 0x22) {
            self->ext.save.unk20 = 0x22;
        }
        func_060E22C4(
            self->ext.save.unkC, self->ext.save.unk24, self->ext.save.unk20);
        if (self->ext.save.unk34 == 0) {
            self->ext.save.unk0++;
        }
        break;

    case 11:
        break;

    case 12:
        DAT_060EFCC0[1] = 0;
        self->ext.save.unk24 = 0x180;
        func_060E1C64(self->ext.save.unk8, 1, self->ext.save.unk24, 0);
        self->ext.save.unk20 = 0;
        self->ext.save.unk0++;
        break;

    case 13:
        DAT_060EFCC0[1] += 9;
        func_060E1C64(
            self->ext.save.unk8, 3, self->ext.save.unk24, self->ext.save.unk20);
        self->ext.save.unk20++;
        if (self->ext.save.unk20 == 0x60) {
            self->ext.save.unk0++;
        }
        break;

    case 14:
        DAT_060EFCC0[1] += 9;
        func_060E1C64(
            self->ext.save.unk8, 3, self->ext.save.unk24, self->ext.save.unk20);
        self->ext.save.unk20--;
        if (self->ext.save.unk20 == 0) {
            self->ext.save.unk0--;
        }
        break;

    case 0x20:
    case 0x30:
        if (self->ext.save.unk0 == 0x30) {
            func_060E1814();
        }
        if (self->ext.save.unk20 > 0x30) {
            func_060E1814();
        }
        DAT_060EFCC0[1] += 6;
        if (self->ext.save.unk24 > 0xC0) {
            self->ext.save.unk24 -= 2;
        }
        func_060E1C64(
            self->ext.save.unk8, 3, self->ext.save.unk24, self->ext.save.unk20);
        self->ext.save.unk20 -= 2;
        if ((g_Player.demo_timer == 0) && (g_pads[0].pressed & 0x700)) {
            g_Player.padSim = 0;
            g_Player.demo_timer = 1;
            if (self->ext.save.unk20 < 1) {
                self->ext.save.unk20 = 0;
            }
        } else if (self->ext.save.unk20 < 1) {
            if (self->ext.save.unk0 == 0x30) {
                self->ext.save.unk38 = 0;
                self->ext.save.unk4 = 0;
                self->ext.save.unk0 = 1;
            } else {
                DAT_060485C0.unk4 = self->ext.save.unk28;
                DAT_060485C0.unk5 = self->ext.save.unk2C;
                self->ext.save.unk38 = 0;
                self->ext.save.unk4 = 0;
                self->ext.save.unk0 = 0;
                DAT_0605C668 = 1;
                DAT_0605C658 = 1;
            }
        }
        break;

    default:
        func_060E1C64(self->ext.save.unk8, 1, self->ext.save.unk24, 0);
        break;
    }

    if (((orig + 0x400) ^ (DAT_060EFCC0[1] + 0x400)) & 0x800) {
        if (self->ext.save.unk0 >= 3 && self->ext.save.unk0 < 10) {
            PlaySfx(SFX_SAVE_COFFIN_SWISH);
        }
    }
    func_060E16B8(self);
}

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

    workspace = DAT_060485E0;
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

    TransformAndProjectPoints(base, ((char*)base + 0x408), 14);
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

    workspace = DAT_060485E0;
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
                    value = (arg2 & 0x7F) >> 3;
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
            work = prim->next;
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
                work = prim->next;
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
extern s16 DAT_060ED938[], DAT_060ED958[], DAT_060ED95E[], DAT_060ED9C2[];
extern u8 DAT_060ED968[], DAT_060ED980[], DAT_060ED994[], DAT_060ED9A0[],
    DAT_060ED9B8[];
void PlaySfxPositional(s32 sfxId);
s32 func_06079C04(Entity* self, s16* sensors);
u8 func_0607A118(Entity* self, s16* sensors, s32 count);
s32 GetSideToPlayer(Entity* self);
extern Entity DAT_060A4FF8;

void func_060E4EDC(Entity* self) {
    Entity* entity;
    Entity* sibling;
    s32 i;
    u16* bank;
    SaturnSpriteImage* images = DAT_060EDE4C;
    void (*sync)(Entity*, s16*);
    if (!self->ext.et_060E4EDC.unk7D && (self->hitFlags & 3) &&
        self->step != 3) {
        self->hitboxState = 0;
        self->hitPoints = g_EnemyDefs[15].hitPoints;
        SetStep(3);
        PlaySfxPositional(0x784);
    }
    if (self->flags & FLAG_DEAD) {
        if (!self->ext.et_060E4EDC.unk7D) {
            self->hitboxState = 0;
            self->flags &= ~FLAG_DEAD;
            self->hitPoints = g_EnemyDefs[15].hitPoints;
            SetStep(3);
            PlaySfxPositional(0x784);
        } else if (self->step != 7) {
            self->hitboxState = 0;
            SetStep(7);
        }
    }
    switch (self->step) {
    case 0:
        bank = &g_RStage16SpriteBankWight.flags;
        self->unk0 = CreateSpriteObject(
            g_RStage16SpriteBankWight.allocationIndex, *bank, images, 5);
        sync = SyncSpriteObjectPosUnchecked;
        sync(self, DAT_060ED9FA);
        self->step++;
        TekiInit(self, 15);
        self->hitboxOffX = 1;
        self->hitboxOffY = 10;
        CreateEntityFromEntity(29, self, self + 1);
        self->animCurFrame = 1;
        func_06079BB4(self);
        func_0600B004(self->unk0, DAT_060EDDE4[self->animCurFrame]);
        /* fallthrough */
    case 1:
        func_06079BB4(self);
        if (func_06079C04(self, DAT_060ED938) & 1) {
            func_06079BB4(self);
            func_0600B004(self->unk0, DAT_060EDDE4[self->animCurFrame]);
            self->step_s = 0;
            self->step++;
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            self->facingLeft = (u8)((GetSideToPlayer(self) & 1) ^ 1);
            if (self->facingLeft)
                self->unk0->flags |= 0x10;
            else
                self->unk0->flags &= ~0x10;
            if (self->facingLeft)
                self->velocityX = 0x2800;
            else
                self->velocityX = -0x2800;
            self->step_s++;
            break;
        case 1:
            AnimateEntityWithSpriteData(self, DAT_060ED968, DAT_060EDDE4);
            func_06079DEC(self, DAT_060ED95E);
            if (func_0607A118(self, DAT_060ED958, (u8)self->facingLeft))
                self->velocityX = 0;
            if (self->ext.et_060E4EDC.unk78++ > 128) {
                self->ext.et_060E4EDC.unk78 = 0;
                self->step_s--;
            }
            if (self->animCurFrame == 7) {
                if (self->facingLeft)
                    self->velocityX -= 0x280;
                else
                    self->velocityX += 0x280;
            }
            if (self->animCurFrame == 8) {
                if (self->facingLeft)
                    self->velocityX += 0x280;
                else
                    self->velocityX -= 0x280;
            }
            break;
        }
        break;
    case 3:
        if (!self->step_s) {
            PlaySfxPositional(0x736);
            self->ext.et_060E4EDC.unk7D = 1;
            for (i = 0; i < 5; i++) {
                entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
                if (entity) {
                    CreateEntityFromEntity(31, self, entity);
                    entity->params = 0;
                    if (self->facingLeft)
                        entity->posX.i.hi -= 11;
                    if (!(Random() & 1))
                        entity->posX.i.hi += 5;
                    else
                        entity->posX.i.hi -= 5;
                    if (!(Random() & 1))
                        entity->posY.i.hi += 5;
                    else
                        entity->posY.i.hi -= 5;
                }
            }
            entity = self + 1;
            entity->step = 2;
            entity->hitboxState = 0;
            self->step_s++;
        }
        if (!AnimateEntityWithSpriteData(self, DAT_060ED9A0, DAT_060EDDE4)) {
            entity = self + 1;
            entity->step = 3;
            entity->step_s = 0;
            entity->hitboxOffX = 0;
            entity->hitboxOffY = 0;
            entity->facingLeft = self->facingLeft;
            entity->posY.i.hi = self->posY.i.hi - 26;
            entity->flags &= ~FLAG_DEAD;
            self->hitPoints = g_EnemyDefs[14].hitPoints;
            self->animCurFrame = 18;
            func_06079BB4(self);
            func_0600B004(self->unk0, DAT_060EDDE4[self->animCurFrame]);
            self->hitboxState = 3;
            SetStep(5);
        }
        break;
    case 4:
        switch (self->step_s) {
        case 0:
            sibling = self + 1;
            sibling->step = 3;
            sibling->animCurFrame = 16;
            sibling->pose = 0;
            sibling->poseTimer = 0;
            sibling->facingLeft = self->facingLeft;
            sibling->zPriority -= 8;
            sibling->unk0->zPriority += 8;
            sibling->posY.i.hi = self->posY.i.hi - 10;
            func_06079BB4(sibling);
            func_0600B004(
                (self + 1)->unk0, DAT_060EDDE4[(self + 1)->animCurFrame]);
            self->ext.et_060E4EDC.unk7D = 1;
            self->animCurFrame = 18;
            self->ext.et_060E4EDC.unk78 = 8;
            self->ext.et_060E4EDC.unk7A = 0;
            self->step_s++;
            func_06079BB4(self);
            func_0600B004(self->unk0, DAT_060EDDE4[self->animCurFrame]);
            break;
        case 1:
            if (!--self->ext.et_060E4EDC.unk78) {
                self->ext.et_060E4EDC.unk7A = 0;
                self->ext.et_060E4EDC.unk78 = 2;
                self->step_s++;
            }
            break;
        case 2:
            if (!--self->ext.et_060E4EDC.unk78) {
                entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
                if (entity) {
                    CreateEntityFromEntity(2, self, entity);
                    entity->params = 1;
                    entity->posY.i.hi -= self->ext.et_060E4EDC.unk7A * 8;
                }
                self->ext.et_060E4EDC.unk78 = 6;
                if (++self->ext.et_060E4EDC.unk7A > 1) {
                    PlaySfxPositional(0x694);
                    DestroyEntity(self + 1);
                    SetStep(5);
                }
            }
            break;
        }
        break;
    case 5:
        if (!AnimateEntityWithSpriteData(self, DAT_060ED9B8, DAT_060EDDE4)) {
            self->pose = 0;
            self->poseTimer = 0;
            self->step++;
        }
        break;
    case 6:
        switch (self->step_s) {
        case 0:
            self->ext.et_060E4EDC.unk78 = DAT_060ED9C2[Random() & 3];
            if (self->facingLeft)
                self->velocityX = 0x1400;
            else
                self->velocityX = -0x1400;
            self->step_s++;
            break;
        case 1:
            AnimateEntityWithSpriteData(self, DAT_060ED980, DAT_060EDDE4);
            func_06079DEC(self, DAT_060ED95E);
            if (func_0607A118(self, DAT_060ED958, (u8)self->facingLeft))
                self->velocityX = 0;
            if (self->animCurFrame == 1) {
                if (self->facingLeft)
                    self->velocityX -= 200;
                else
                    self->velocityX += 200;
            }
            if (self->animCurFrame == 2) {
                if (self->facingLeft)
                    self->velocityX += 200;
                else
                    self->velocityX -= 200;
            }
            if (!--self->ext.et_060E4EDC.unk78) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s++;
            }
            break;
        case 2:
            if (!AnimateEntityWithSpriteData(
                    self, DAT_060ED994, DAT_060EDDE4)) {
                if ((self->facingLeft = Random() & 1))
                    self->unk0->flags |= 0x10;
                else
                    self->unk0->flags &= ~0x10;
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 0;
            }
            break;
        }
        break;
    case 7:
        entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
        if (entity) {
            CreateEntityFromEntity(2, self, entity);
            entity->params = 2;
        }
        PlaySfxPositional(0x694);
        DestroyEntity(self);
        break;
    }
}

extern s16 DAT_060EDA14[];
extern u8 DAT_060ED9B0[], DAT_060ED974[];
extern s16 DAT_060ED948[], DAT_060ED9CA[];
u8 func_0607AF94(Entity* self, Entity* other);
void func_0607AF3C(Entity* self, u8 direction, s16 limit);

void func_060E56DC(Entity* self) {
    Entity* entity;
    s16 angle;
    u8 direction;
    u32 randOffset;
    s16* x;
    u16* bank;
    SaturnSpriteImage* images = DAT_060EDE4C;
    void (*sync)(Entity*, s16*);
    s32 dx;
    s32 dy;
    if ((self->flags & 0x100) && self->step > 5) {
        self->step = 5;
        self->step_s = 0;
        self->hitboxState = 0;
        self->ext.et_060E4EDC.unk7A = 0;
        self->ext.et_060E4EDC.unk7C = 0;
        PlaySfxPositional(0x785);
        PlaySfxPositional(0x691);
    }
    switch (self->step) {
    case 0:
        bank = &g_RStage16SpriteBankWight.flags;
        self->unk0 = CreateSpriteObject(bank[-1], bank[0], images, 5);
        sync = SyncSpriteObjectPosUnchecked;
        sync(self, DAT_060EDA14);
        self->step = 1;
        TekiInit(self, 14);
        self->hitboxOffX = -4;
        self->hitboxOffY = -14;
        self->ext.et_060E4EDC.unk7C = 0;
        break;
    case 1:
        self->facingLeft = (self - 1)->facingLeft;
        if ((self->hitFlags & 3) || (self->flags & 0x100)) {
            self->hitboxState = 0;
            self->flags &= ~0x100;
            self->hitPoints = g_EnemyDefs[14].hitPoints;
            (self - 1)->step = 4;
            (self - 1)->step_s = 0;
            (self - 1)->pose = 0;
            (self - 1)->poseTimer = 0;
            self->step++;
        }
        break;
    case 3:
        self->hitboxState = 3;
        if (self->step_s == 0) {
            self->velocityY = -0xD000;
            self->step_s++;
        }
        if (self->facingLeft)
            self->unk0->flags |= 0x10;
        else
            self->unk0->flags &= ~0x10;
        MoveEntity(self);
        self->velocityY += 0x800;
        if (AnimateEntityWithSpriteData(self, DAT_060ED9B0, DAT_060EDDE4) ==
            0) {
            self->step_s = 0;
            self->step = 6;
        }
        break;
    case 5:
        switch (self->step_s) {
        case 0:
            entity = &g_Entities[0];
            dx = self->posX.val - entity->posX.val;
            dy = entity->posY.val - self->posY.val;
            angle = ratan2(dx, dy);
            self->rotate = angle;
            self->velocityX = rsin(angle) * 40;
            self->velocityY = rcos(angle) * -40;
            self->ext.et_060E4EDC.unk78 = 2;
            self->step_s++;
            break;
        case 1:
            if (func_06079C04(self, DAT_060ED948) & 1) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity) {
                    CreateEntityFromEntity(2, self, entity);
                    entity->params = 3;
                }
                DestroyEntity(self);
                return;
            }
            if (--self->ext.et_060E4EDC.unk78 == 0) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity) {
                    CreateEntityFromEntity(2, self, entity);
                    entity->params = 1;
                }
                self->ext.et_060E4EDC.unk78 = 6;
            }
            break;
        }
        break;
    case 6:
        MoveEntity(self);
        self->velocityY += 0x800;
        if (self->velocityY > 0x8000) {
            self->velocityY = 0;
            self->step = 7;
        }
        break;
    case 7:
        if (!(Random() & 0x7F))
            self->ext.et_060E4EDC.unk7C = 1;
        if (!(Random() & 0x1F)) {
            entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
            if (entity) {
                CreateEntityFromEntity(30, self, entity);
                randOffset = Random();
                x = &entity->posX.i.hi;
                randOffset = (randOffset & 8) + 0xFFFC;
                *x += randOffset;
                entity->posY.i.hi += 13;
            }
        }
        if (self->step_s == 0) {
            self->facingLeft = (u8)((GetSideToPlayer(self) & 1) ^ 1);
            if (self->facingLeft)
                self->unk0->flags |= 0x10;
            else
                self->unk0->flags &= ~0x10;
            if (self->facingLeft)
                self->velocityX = 0xA000;
            else
                self->velocityX = -0xA000;
            self->ext.et_060E4EDC.unk78 = 96;
            self->step_s++;
        } else {
            MoveEntity(self);
            AnimateEntityWithSpriteData(self, DAT_060ED974, DAT_060EDDE4);
            if (--self->ext.et_060E4EDC.unk78 == 0) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 0;
                self->step++;
            }
        }
        break;
    case 8:
        entity = &g_Entities[0];
        if (self->step_s == 0) {
            self->ext.et_060E4EDC.unk78 = 64;
            angle = func_0607AF94(self, entity);
            func_0607AF3C(self, angle, 6);
            self->step_s++;
        } else {
            if (!(Random() & 0x7F))
                self->ext.et_060E4EDC.unk7C = 1;
            if (!(Random() & 0x1F)) {
                entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
                if (entity) {
                    CreateEntityFromEntity(30, self, entity);
                    randOffset = Random();
                    x = &entity->posX.i.hi;
                    randOffset = (randOffset & 8) + 0xFFFC;
                    *x += randOffset;
                    entity->posY.i.hi += 13;
                }
            }
            MoveEntity(self);
            AnimateEntityWithSpriteData(self, DAT_060ED974, DAT_060EDDE4);
            if (--self->ext.et_060E4EDC.unk78 == 0) {
                self->velocityY = 0;
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 0;
                self->step--;
            }
        }
        break;
    }
    if (self->step <= 1) {
        self->posX.i.hi = (self - 1)->posX.i.hi;
        self->posY.i.hi = (self - 1)->posY.i.hi;
    }
    if (self->ext.et_060E4EDC.unk7C && g_Timer % 3 == 0) {
        self->posY.i.hi += DAT_060ED9CA[self->ext.et_060E4EDC.unk7A];
        if (++self->ext.et_060E4EDC.unk7A > 23) {
            self->ext.et_060E4EDC.unk7A = 0;
            self->ext.et_060E4EDC.unk7C = 0;
        }
    }
    func_06079BB4(self);
}

extern s16 DAT_060EDA28[];
extern s16 DAT_060EDA2C[];

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
        self->ext.et_060E4EDC.prim = prim;
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
        prim = self->ext.et_060E4EDC.prim;
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

extern s16 DAT_060EDE8C[];
extern s32 DAT_060EDEA4, DAT_060EDEA8;
extern u8 DAT_060EDED0[], DAT_060EDEF6[], DAT_060EDF02[], DAT_060EDF18[];
s32 GetDistanceToPlayerX(Entity* self);
s32 GetDistanceToPlayerY(Entity* self);
void PlaySfxPositional(s32 sfxId);
u16 func_0607B134(u16 step, u16 angle, u16 target);

void func_060E5F48(Entity* self) {
    Entity* entity;
    s32 result;
    s32 angle;
    s32 i;

    if ((self->flags & 0x100) && self->step != 5) {
        SetStep(5);
    }
    switch (self->step) {
    case 0:
        self->unk0 = CreateSpriteObject(
            g_RStage16SpriteBankGargoyle[0].allocationIndex,
            g_RStage16SpriteBankGargoyle[0].flags,
            (SaturnSpriteImage*)DAT_060EF0C0, 11);
        SyncSpriteObjectPosUnchecked(self, DAT_060EDE8C);
        if (self->params == 0) {
            TekiInit(self, 0x190);
        }
        if (self->params == 1) {
            TekiInit(self, 0x19F);
        }
        if (self->params == 2) {
            TekiInit(self, 0x1A0);
        }
        self->animCurFrame = 2;
        self->ext.gargoyle.variant = self->params;
        self->ext.gargoyle.clutBase = self->unk0->clutBase;
        switch (self->ext.gargoyle.variant) {
        case 1:
            self->unk0->clutBase = self->ext.gargoyle.clutBase;
            break;
        case 2:
            self->unk0->clutBase = self->ext.gargoyle.clutBase + 9;
            break;
        case 0:
            self->unk0->clutBase = self->ext.gargoyle.clutBase + 0x12;
            break;
        }
        entity = self + 1;
        CreateEntityFromCurrentEntity(34, entity);
        entity->ext.gargoyle.variant = self->ext.gargoyle.variant;
        self->posY.i.hi -= 27;
        func_06079BB4(self);
        func_0600B004(self->unk0, DAT_060EEFD0[self->animCurFrame]);
        self->step = 1;
    case 1:
        if ((s16)GetDistanceToPlayerX(self) > 223) {
            break;
        }
        if (AnimateEntityWithSpriteData(self, DAT_060EDED0, DAT_060EEFD0)) {
            break;
        }
        SetStep(2);
        self->posY.i.hi -= 12;
        self->ext.gargoyle.unk80 = g_Tilemap.scrollX.i.hi + self->posX.i.hi;
        self->facingLeft = (GetSideToPlayer(self) & 1) ^ 1;
        self->ext.gargoyle.unk7C = 0;
        self->ext.gargoyle.unk8A = 16;
        break;
    case 2:
        if (g_Timer % 24 == 0) {
            PlaySfxPositional(0x68C);
        }
        AnimateEntityWithSpriteData(self, DAT_060EDEF6, DAT_060EEFD0);
        MoveEntity(self);
        switch (self->step_s) {
        case 0:
            entity = &g_Entities[0];
            angle = ratan2(entity->posY.i.hi - self->posY.i.hi,
                           entity->posX.i.hi - self->posX.i.hi);
            if (angle > 0x800) {
                angle -= 0x1000;
            }
            switch (self->ext.gargoyle.variant) {
            case 0:
                self->ext.gargoyle.unk7E =
                    func_0607B134(16, self->ext.gargoyle.unk7E, angle);
                self->velocityX =
                    (rcos(self->ext.gargoyle.unk7E) * 0x20000) >> 12;
                self->velocityY =
                    (rsin(self->ext.gargoyle.unk7E) * 0x20000) >> 12;
                break;
            case 1:
                self->ext.gargoyle.unk7E =
                    func_0607B134(40, self->ext.gargoyle.unk7E, angle);
                self->velocityX =
                    (rcos(self->ext.gargoyle.unk7E) * 0x10000) >> 12;
                self->velocityY =
                    (rsin(self->ext.gargoyle.unk7E) * 0x10000) >> 12;
                break;
            case 2:
                self->ext.gargoyle.unk7E =
                    func_0607B134(32, self->ext.gargoyle.unk7E, angle);
                self->velocityX =
                    (rcos(self->ext.gargoyle.unk7E) * 0x30000) >> 12;
                self->velocityY =
                    (rsin(self->ext.gargoyle.unk7E) * 0x30000) >> 12;
                break;
            }
            if (self->velocityX > 0) {
                self->facingLeft = 1;
            } else {
                self->facingLeft = 0;
            }
            result = (GetSideToPlayer(self) & 1) ^ 1;
            if (self->facingLeft == result &&
                (s16)GetDistanceToPlayerX(self) < 32) {
                self->step_s = 1;
            }
            break;
        case 1:
            if ((s16)GetDistanceToPlayerY(self) > 64 ||
                (s16)GetDistanceToPlayerX(self) > 64) {
                self->step_s = 0;
            }
            break;
        }
        if (--self->ext.gargoyle.unk8A == 0) {
            self->ext.gargoyle.unk8A = 2;
            if ((s16)GetDistanceToPlayerY(self) < 16 &&
                (s16)GetDistanceToPlayerX(self) < 32) {
                SetStep(3);
                self->velocityX = 0;
                self->velocityY = 0;
                self->ext.gargoyle.unk88 = 0;
                self->facingLeft = (u8)((GetSideToPlayer(self) & 1) ^ 1);
                if (self->facingLeft) {
                    self->unk0->flags |= 0x10;
                } else {
                    self->unk0->flags &= ~0x10;
                }
            }
        }
        break;
    case 3:
        if (LOW(self->pose) == 0x30000) {
            PlaySfx(0x8FE);
        }
        result = AnimateEntityWithSpriteData(self, DAT_060EDF02, DAT_060EEFD0);
        if (result == 0) {
            if (self->ext.gargoyle.unk88 == 1) {
                SetStep(4);
                self->velocityX = result;
                self->velocityY = result;
                self->ext.gargoyle.unk88 = 0;
            } else {
                SetStep(2);
                self->ext.gargoyle.unk8A = 16;
                self->velocityX = result;
                self->velocityY = result;
            }
        }
        if (g_Player.status & 0x10000) {
            self->ext.gargoyle.unk88 = 1;
        }
        if ((u16)(self->animCurFrame - 24) <= 2) {
            self->hitboxWidth = 22;
            self->hitboxHeight = 14;
            self->hitboxOffX = -16;
        } else {
            self->hitboxWidth = 12;
            self->hitboxHeight = 14;
            self->hitboxOffX = -6;
        }
        self->hitboxOffY = 0;
        break;
    case 4:
        if (AnimateEntityWithSpriteData(self, DAT_060EDF18, DAT_060EEFD0) ==
            0) {
            self->ext.gargoyle.unk88++;
        }
        if (self->ext.gargoyle.unk88 > 1) {
            SetStep(2);
            self->velocityX = 0;
            self->velocityY = 0;
            self->ext.gargoyle.unk88 = 0;
            self->ext.gargoyle.unk8A = 16;
        }
        break;
    case 5:
        switch (self->step_s) {
        case 0:
        case 1:
        case 2:
            self->step_s++;
            break;
        case 3:
            angle = !(GetSideToPlayer(self) & 1) ? 0xB00 : 0xD00;
            for (i = 0; i < 16; i++) {
                entity = AllocEntity(&g_Entities[0xE0], &g_Entities[0x100]);
                if (entity) {
                    CreateEntityFromEntity(33, self, entity);
                    entity->params = i + 41;
                    entity->ext.gargoyle.variant = self->ext.gargoyle.variant;
                    DAT_060EDEA4 = (((Random() & 15) << 8) >> 2) << 8;
                    entity->velocityX = (DAT_060EDEA4 * rcos(angle)) >> 12;
                    entity->velocityY = (DAT_060EDEA8 * rsin(angle)) >> 12;
                }
                if (!(GetSideToPlayer(self) & 1)) {
                    angle -= 45;
                } else {
                    angle += 45;
                }
            }
            DestroyEntity(self);
            return;
        }
        break;
    }
    self->unk0->flags |= 0x40;
    self->unk0->rotate = self->rotate;
    if (self->facingLeft == 0) {
        self->unk0->flags &= ~0x10;
    } else {
        self->unk0->flags |= 0x10;
    }
}

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
        source = projected;
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
extern SaturnSpriteImage DAT_060EFC20[];
extern SaturnSpriteFrameHeader* DAT_060EFB84[];
extern u8 DAT_060EF5D4[], DAT_060EF5E4[], DAT_060EF5F4[], DAT_060EF600[],
    DAT_060EF644[], DAT_060EF6EC[], DAT_060EF684[];
extern s16 DAT_060EF5BC[], DAT_060EF5CC[], DAT_060EF6CC[], DAT_060EF6DC[];
extern s32 DAT_060EF68C[], DAT_060EF6AC[];
extern u16 DAT_0605AF54[][2], DAT_0605AF40[][2];
void func_060E7400(Entity* self);
u16 SPR_2LookupTblNoToVram(u16 tableNo);
u16 LocalLookupTblNoToVram(u16 tableNo);

/* Native resident prototypes verified in game_2b.c. Keep existing global
 * generated sensor declarations unchanged; best0 casts only its scoped sensor
 * call pointer. */
void PlaySfxPositional(s32);
s32 GetDistanceToPlayerX(Entity*);
s32 func_06079C04(Entity*, s16*);

void func_060E7698(Entity* self) {
    Entity* entity;
    Primitive* prim;
    s32 primIndex;
    u16 clut;
    u16 clut2;
    s32 i;
    s32 j;
    s32 index;
    u16* facing;
    s32 side;
    u16* texture;
    u16* texture2;

    if (self->flags & 0x100) {
        SetStep(8);
    }
    switch (self->step) {
    case 0:
        self->unk0 =
            CreateSpriteObject(g_RStage16SpriteBank23.allocationIndex,
                               g_RStage16SpriteBank23.flags, DAT_060EFC20, 5);
        TekiInit(self, 0x7E);
        self->unk0->flags |= 0x40;
        self->ext.et_060E7698.unk7D = 80;
        primIndex = AllocPrimitives(2, 2);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= 0x800000;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_060E7698.unk78 = prim;
        texture = DAT_0605AF54[g_RStage16SpriteBank23.allocationIndex];
        prim->unk8 = texture[0];
        prim->unkA = texture[1];
        clut = g_RStage16SpriteBank23.flags + 3;
        prim->unk6 = LookupTblNoToVram(clut);
        prim->priority = 0xB0;
        prim->drawMode = 8;
        prim = prim->next;
        texture2 = DAT_0605AF40[g_RStage16SpriteBank23.allocationIndex];
        prim->unk8 = texture2[0];
        prim->unkA = texture2[1];
        clut2 = g_RStage16SpriteBank23.flags + 2;
        prim->unk6 = LookupTblNoToVram(clut2);
        prim->priority = 0xA8;
        prim->drawMode = 8;
        prim->unk4 |= 0x100;
        func_06079BB4(self);
        self->animCurFrame = 0;
        func_0600B004(self->unk0, DAT_060EFB84[0]);
        self->step++;
        break;
    case 1:
        if ((u16)func_06079C04(self, DAT_060EF5BC)) {
            SetStep(2);
        }
        break;
    case 2:
        self->facingLeft = (GetSideToPlayer(self) & 1) ^ 1;
        AnimateEntityWithSpriteData(self, DAT_060EF5F4, DAT_060EFB84);
        if ((s16)GetDistanceToPlayerX(self) <= 111) {
            SetStep(4);
        }
        break;
    case 3:
        if (AnimateEntityWithSpriteData(self, DAT_060EF5D4, DAT_060EFB84) ==
            0) {
            self->facingLeft = (GetSideToPlayer(self) & 1) ^ 1;
        }
        self->ext.et_060E7698.unk7C = self->facingLeft;
        if (self->ext.et_060E7698.unk7C) {
            self->velocityX = 0xA000;
        } else {
            self->velocityX = -0xA000;
        }
        if ((s16)GetDistanceToPlayerX(self) <= 94) {
            self->step = 4;
        }
        {
            s32 (*sensorFunc)(Entity*, s16*) =
                (s32(*)(Entity*, s16*))func_06079DEC;
            sensorFunc(self, DAT_060EF5CC);
        }
        if (self->ext.et_060E7698.unk7D == 0) {
            if ((s16)GetDistanceToPlayerX(self) <= 159) {
                facing = &self->facingLeft;
                side = GetSideToPlayer(self) & 1;
                if (*facing != side) {
                    SetStep(6);
                }
            }
        } else {
            self->ext.et_060E7698.unk7D--;
        }
        break;
    case 4:
        if (AnimateEntityWithSpriteData(self, DAT_060EF5E4, DAT_060EFB84) ==
            0) {
            self->facingLeft = (GetSideToPlayer(self) & 1) ^ 1;
        }
        self->ext.et_060E7698.unk7C = self->facingLeft ^ 1;
        if (self->ext.et_060E7698.unk7C) {
            self->velocityX = 0xA000;
        } else {
            self->velocityX = -0xA000;
        }
        if ((s16)GetDistanceToPlayerX(self) > 115) {
            self->step = 3;
        }
        {
            s32 (*sensorFunc)(Entity*, s16*) =
                (s32(*)(Entity*, s16*))func_06079DEC;
            sensorFunc(self, DAT_060EF5CC);
        }
        if (self->ext.et_060E7698.unk7D == 0) {
            if ((s16)GetDistanceToPlayerX(self) <= 159) {
                facing = &self->facingLeft;
                side = GetSideToPlayer(self) & 1;
                if (*facing != side) {
                    SetStep(6);
                }
            }
        } else {
            self->ext.et_060E7698.unk7D--;
        }
        break;
    case 6:
        if (AnimateEntityWithSpriteData(self, DAT_060EF600, DAT_060EFB84) ==
            0) {
            self->ext.et_060E7698.unk84 = 0;
            SetStep(7);
        }
        if (*(u32*)&self->pose == 0x20000) {
            PlaySfxPositional(0x614);
        }
        break;
    case 7:
        switch (self->step_s) {
        case 0:
            entity = self + 1;
            CreateEntityFromEntity(0x27, self, entity);
            if (self->facingLeft) {
                entity->posX.i.hi += 10;
            } else {
                entity->posX.i.hi -= 10;
            }
            entity->posY.i.hi -= 4;
            entity->facingLeft = self->facingLeft;
            self->step_s++;
            break;
        case 1:
            prim = self->ext.et_060E7698.unk78;
            break;
        }
        func_060E7400(self);
        if (AnimateEntityWithSpriteData(self, DAT_060EF644, DAT_060EFB84) ==
            0) {
            self->ext.et_060E7698.unk78->drawMode |= 8;
            index = ++self->ext.et_060E7698.unk7E & 7;
            self->ext.et_060E7698.unk7D = DAT_060EF6EC[index];
            SetStep(4);
        }
        break;
    case 8:
        for (i = 0, j = 0; i < 6; i++, j++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity == NULL) {
                break;
            }
            CreateEntityFromCurrentEntity(0x29, entity);
            entity->facingLeft = self->facingLeft;
            entity->params = i;
            entity->ext.et_060E7C60.timer = DAT_060EF684[i];
            if (self->facingLeft) {
                entity->posX.i.hi -= DAT_060EF6CC[j];
            } else {
                entity->posX.i.hi += DAT_060EF6CC[j];
            }
            entity->posY.i.hi += DAT_060EF6DC[j];
            entity->velocityX = DAT_060EF68C[i];
            entity->velocityY = DAT_060EF6AC[i];
        }
        PlaySfxPositional(0x62A);
        DestroyEntity(self);
        return;
    }
    if (self->facingLeft == 0) {
        self->unk0->flags &= ~0x10;
    } else {
        self->unk0->flags |= 0x10;
    }
}

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
extern SaturnSpriteImage DAT_060EFC20[];
extern Entity DAT_060A4FF8;
void PlaySfxPositional(s32 sfxId);
void func_060E7DB8(Entity* self) {
    SpriteObject* sprite;
    Primitive* prim;
    Entity* entity;
    s32 primIndex;
    s32 i;
    s32 frame;
    u16 palette;
    u16* texture;
    s16 x;
    s16 y;
    s32 edge;

    switch (self->step) {
    case 0:
        self->step++;
        sprite =
            CreateSpriteObject(g_RStage16SpriteBank23.allocationIndex,
                               g_RStage16SpriteBank23.flags, DAT_060EFC20, 2);
        self->unk0 = sprite;
        sprite->flags |= 0x40;
        TekiInit(self, 0x7F);
        primIndex = AllocPrimitives(2, 3);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_060E7698.unk78 = prim;
        i = 0;
        do {
            u16 allocation = g_RStage16SpriteBank23.allocationIndex;
            s32 delta = i - 0x24;
            texture = DAT_0605aec0[allocation - delta];
            prim->unk8 = texture[0];
            prim->unkA = texture[1];
            palette = g_RStage16SpriteBank23.flags + 3;
            prim->unk6 = LookupTblNoToVram(palette);
            prim->priority = 0xB0;
            prim->drawMode = 2;
            prim = prim->next;
            i++;
        } while (i < 3);
        self->ext.et_060E7698.unk82 = 0x60;
        self->ext.et_060E7698.unk8C = 0;
        break;
    case 1:
        self->ext.et_060E7698.unk8E = 0x10;
        if (self->ext.et_060E7698.unk8C <= 0x9F) {
            self->ext.et_060E7698.unk8C += 0x10;
        } else {
            self->ext.et_060E7698.unk8C = 0xA0;
            self->hitboxState = 1;
            self->step++;
        }
        /* fallthrough */
    case 2:
        if (!(self->ext.et_060E7698.unk82 & 3)) {
            entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
            if (entity != NULL) {
                CreateEntityFromEntity(0x28, self, entity);
                entity->ext.et_060E7698.unk8C = self->ext.et_060E7698.unk8C;
                entity->facingLeft = self->facingLeft;
            }
        }
        if (!(self->ext.et_060E7698.unk82 & 0xF)) {
            PlaySfxPositional(0x61E);
        }
        if (self->ext.et_060E7698.unk82 <= 0xF) {
            PlaySfxPositional(0x621);
            self->step++;
        }
        /* fallthrough */
    case 3:
        if (Random() & 1) {
            if (self->ext.et_060E7698.unk8C <= 0xA7) {
                self->ext.et_060E7698.unk8C++;
            }
        } else if (self->ext.et_060E7698.unk8C > 0xB8) {
            self->ext.et_060E7698.unk8C--;
        }
        self->hitboxWidth = self->ext.et_060E7698.unk8C / 2 + 0x10;
        self->hitboxOffX = -self->ext.et_060E7698.unk8C / 2 - 0x10;
        self->hitboxHeight = 8;
        if (self[-1].entityId != 0x26) {
            self->ext.et_060E7698.unk82 = 1;
        }
        if (--self->ext.et_060E7698.unk82 == 0) {
            self->hitboxState = 0;
            self->step++;
        }
        break;
    case 4:
        if (--self->ext.et_060E7698.unk8E == 0) {
            entity = AllocEntity(&g_Entities[224], &DAT_060A4FF8);
            if (entity != NULL) {
                CreateEntityFromCurrentEntity(0x2A, entity);
                entity->facingLeft = self->facingLeft;
            }
            DestroyEntity(self);
            return;
        }
        break;
    }
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim = self->ext.et_060E7698.unk78;
    i = 0;
    do {
        prim->y0 = prim->y1 = y - self->ext.et_060E7698.unk8E;
        prim->y3 = prim->y2 = self->ext.et_060E7698.unk8E + y;
        if (g_Timer & 1) {
            prim->drawMode = 2;
        } else {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = prim->next;
        i++;
    } while (i < 3);
    prim = self->ext.et_060E7698.unk78;
    if (self->facingLeft) {
        edge = x - 0x10;
    } else {
        edge = x + 0x10;
    }
    prim->x1 = prim->x2 = edge;
    if (self->facingLeft) {
        edge += 0x20;
    } else {
        edge -= 0x20;
    }
    prim->x0 = prim->x3 = edge;
    prim = prim->next;
    prim->x1 = prim->x2 = edge;
    if (self->facingLeft) {
        edge += self->ext.et_060E7698.unk8C;
    } else {
        edge -= self->ext.et_060E7698.unk8C;
    }
    prim->x0 = prim->x3 = edge;
    prim = prim->next;
    prim->x1 = prim->x2 = edge;
    if (self->facingLeft) {
        edge += 0x20;
    } else {
        edge -= 0x20;
    }
    prim->x0 = prim->x3 = edge;
}

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

void func_060E8368(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    u16 timer;

    switch (self->step) {
    case 0:
        primIndex = AllocPrimitives(5, 24);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= 0xC0202000;
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_060E8368.unk78 = prim;
        while (prim != NULL) {
            prim->unk6 = 0xFA90;
            prim->drawMode = 0;
            prim->priority = 0x90;
            prim->y0 = prim->y1 = self->posY.i.hi + (Random() & 7) - 4;
            if (self->facingLeft) {
                prim->x0 = self->posX.i.hi + ((u8)Random() % 160);
            } else {
                prim->x0 = self->posX.i.hi - ((u8)Random() % 160);
            }
            prim->x1 = prim->x0;
            LOW(prim->x2) = ((Random() & 7) + 1) << 15;
            if (self->facingLeft) {
                LOW(prim->x2) = -LOW(prim->x2);
            }
            LOW(prim->x3) = prim->x0 << 16;
            prim->unk8 = Random() & 15;
            prim = prim->next;
        }
        self->step++;
        break;
    case 1:
        prim = self->ext.et_060E8368.unk78;
        while (prim != NULL) {
            timer = ++prim->unk8;
            if (timer % (timer >> 2)) {
                prim->drawMode |= DRAW_HIDE;
            } else {
                prim->drawMode = 2;
            }
            LOW(prim->x3) +=
                self->ext.et_060E8368.unkA0 - g_Tilemap.scrollX.val;
            LOW(prim->x3) -= LOW(prim->x2);
            prim->x0 = LOW(prim->x3) >> 16;
            if (Random() & 7) {
                prim->x1 = prim->x0;
            } else {
                prim->x1 = prim->x0 + 1;
            }
            if ((u16)prim->unk8 > 150) {
                DestroyEntity(self);
                return;
            }
            prim = prim->next;
        }
        break;
    }
    self->ext.et_060E8368.unkA0 = g_Tilemap.scrollX.val;
}

const u32 DAT_060E859C = 0x08E8DE06;
const u16 DAT_060E85A0[] = {
    0x0009, 0x8000, 0x81D8, 0x8154, 0x80D0, 0x804C, 0x8008, 0x8006, 0x8004,
    0x82DA, 0x8256, 0x81D4, 0x810E, 0x8088, 0x8046, 0x8004, 0x8001, 0x8000,
    0xA98D, 0xA14B, 0x9909, 0x90C7, 0x8885, 0x8443, 0x8000, 0x9EC6, 0x9604,
    0x8942, 0x80A0, 0xCA95, 0xC253, 0xBA11, 0xB1CF, 0x8000, 0xA193, 0x9951,
    0x910F, 0x88CD, 0x848B, 0x8049, 0x8026, 0x96EB, 0x8E4A, 0x8588, 0x80C6,
    0xBE9B, 0xBA59, 0xB217, 0xA9D5, 0x80A0, 0xEF5A, 0xE718, 0xDED6, 0xD694,
    0xCE52, 0xC610, 0xBDCE, 0xB58C, 0xAD4A, 0xA508, 0x9CC6, 0x98A5, 0x9484,
    0x9063, 0x8842, 0x8880, 0x8882, 0xE318, 0xDAD6, 0xD6B5, 0xCE73, 0xCA52,
    0xC210, 0xBDEF, 0xB5AD, 0xAD6B, 0xA94A, 0xA108, 0x9CE7, 0x94A5, 0x9084,
    0x80A0, 0x890D, 0x99B3, 0xA215, 0xAA99, 0x8154, 0x8216, 0x86B9, 0x8421,
    0x8089, 0xFBBD, 0xCA95, 0xBD0C, 0xA4E4, 0xCD89, 0xF24F, 0x8000, 0xC294,
    0xBA31, 0xB1CE, 0xA96B, 0xA129, 0x98E7, 0x90A5, 0x8000, 0x8863, 0x8002,
    0x8024, 0x8046, 0x8088, 0x80CA, 0xDB18, 0xA870, 0x8000, 0x8865, 0x990C,
    0xADB2, 0xBE57, 0xCABA, 0xA54F, 0xA54B, 0x8444, 0x94C8, 0x8C87, 0x8886,
    0x8884, 0x8906, 0xDF7D, 0x80A0, 0xA5D3, 0x9D90, 0x952D, 0x90CA, 0x8888,
    0x90E9, 0x8024, 0x80E9, 0x80E9, 0x8084, 0x8024, 0x8006, 0x800E, 0x8017,
    0x801F,
};
const u16 DAT_060E86C2[] = {
    0x4001, 0x8000, 0xC294, 0xBA31, 0xB1CE, 0xA96B, 0xA129, 0x98E7, 0x90A5,
    0x8000, 0x8863, 0x8002, 0x8024, 0x8046, 0x8088, 0x80CA, 0xDB18,
};
const SaturnSpriteImage DAT_060E86E4[] = {
    {8, 8, 0x0000},   {16, 26, 0x0010}, {12, 8, 0x0078},  {16, 10, 0x0090},
    {8, 8, 0x00B8},   {8, 7, 0x00C8},   {12, 18, 0x00D8}, {16, 16, 0x0110},
    {16, 16, 0x0150}, {20, 10, 0x0190}, {12, 16, 0x01C4}, {20, 14, 0x01F4},
    {16, 13, 0x023C}, {16, 12, 0x0270}, {48, 16, 0x02A0}, {32, 20, 0x0360},
    {48, 16, 0x0400}, {44, 17, 0x04C0}, {28, 31, 0x057C}, {20, 31, 0x0658},
    {0, 0, 0x06F4},
};
const SaturnSpriteFrameHeader DAT_060E8738 = {0x0000, 0x0000};
const SaturnSpriteFrame2 DAT_060E873C = {
    {0x0200, 0x0000},
    {
        {48, 0, 40, 0},
        {304, 0, 25, 1},
    },
};
const SaturnSpriteFrame3 DAT_060E8750 = {
    {0x0300, 0x0000},
    {
        {48, -3, 40, 2},
        {304, 0, 25, 1},
        {560, -1, 38, 3},
    },
};
const SaturnSpriteFrame2 DAT_060E876C = {
    {0x0200, 0x0000},
    {
        {48, 0, 40, 4},
        {304, 0, 25, 1},
    },
};
const SaturnSpriteFrame3 DAT_060E8780 = {
    {0x0300, 0x0000},
    {
        {48, -1, 39, 5},
        {304, 0, 25, 1},
        {560, -1, 38, 3},
    },
};
const SaturnSpriteFrame1 DAT_060E879C = {
    {0x0100, 0x0000},
    {
        {304, -1, 17, 6},
    },
};
const SaturnSpriteFrame2 DAT_060E87A8 = {
    {0x0200, 0x0000},
    {
        {816, -2, 0, 7},
        {48, -3, 24, 2},
    },
};
const SaturnSpriteFrame3 DAT_060E87BC = {
    {0x0300, 0x0000},
    {
        {816, -2, 0, 7},
        {48, 0, 24, 0},
        {1072, -2, 0, 8},
    },
};
const SaturnSpriteFrame2 DAT_060E87D8 = {
    {0x0200, 0x0000},
    {
        {816, -2, 0, 7},
        {48, 0, 24, 4},
    },
};
const SaturnSpriteFrame3 DAT_060E87EC = {
    {0x0300, 0x0000},
    {
        {816, -2, 0, 7},
        {48, -1, 23, 5},
        {1072, -2, 0, 8},
    },
};
const SaturnSpriteFrame1 DAT_060E8808 = {
    {0x0100, 0x0000},
    {
        {816, -2, 0, 7},
    },
};
const SaturnSpriteFrame1 DAT_060E8814 = {
    {0x0100, 0x0000},
    {
        {1328, -2, 8, 9},
    },
};
const SaturnSpriteFrame1 DAT_060E8820 = {
    {0x0100, 0x0000},
    {
        {560, -3, 16, 10},
    },
};
const SaturnSpriteFrame1 DAT_060E882C = {
    {0x0100, 0x0000},
    {
        {1584, -3, 20, 11},
    },
};
const SaturnSpriteFrame1 DAT_060E8838 = {
    {0x0100, 0x0000},
    {
        {1584, 0, 20, 12},
    },
};
const SaturnSpriteFrame1 DAT_060E8844 = {
    {0x0100, 0x0000},
    {
        {1584, -1, 20, 13},
    },
};
const SaturnSpriteFrame1 DAT_060E8850 = {
    {0x0100, 0x0000},
    {
        {1840, 28, 16, 14},
    },
};
const SaturnSpriteFrame1 DAT_060E885C = {
    {0x0100, 0x0000},
    {
        {1840, -18, 20, 15},
    },
};
const SaturnSpriteFrame1 DAT_060E8868 = {
    {0x0100, 0x0000},
    {
        {1840, -32, 16, 16},
    },
};
const SaturnSpriteFrame1 DAT_060E8874 = {
    {0x0100, 0x0000},
    {
        {1840, -29, 17, 17},
    },
};
const SaturnSpriteFrame1 DAT_060E8880 = {
    {0x0100, 0x0000},
    {
        {2096, -5, -31, 18},
    },
};
const SaturnSpriteFrame1 DAT_060E888C = {
    {0x0100, 0x0000},
    {
        {2096, -13, -31, 19},
    },
};
const SaturnSpriteFrameHeader* const DAT_060E8898[] = {
    &DAT_060E8738,        &DAT_060E873C.header, &DAT_060E8750.header,
    &DAT_060E876C.header, &DAT_060E8780.header, &DAT_060E879C.header,
    &DAT_060E87A8.header, &DAT_060E87BC.header, &DAT_060E87D8.header,
    &DAT_060E87EC.header, &DAT_060E8808.header, &DAT_060E8814.header,
    &DAT_060E8820.header, &DAT_060E882C.header, &DAT_060E8838.header,
    &DAT_060E8844.header, &DAT_060E8850.header, &DAT_060E885C.header,
    &DAT_060E8868.header, &DAT_060E8874.header, &DAT_060E8880.header,
    &DAT_060E888C.header,
};
const SaturnSpriteResource g_RStage16SpriteBank16 = {
    DAT_060E86E4, DAT_060E85A0, 0xFFFF, 0xFFFF};
const SaturnSpriteResource g_RStage16SpriteBank17[] = {
    {NULL, DAT_060E86C2, 0xFFFF, 0xFFFF}};
