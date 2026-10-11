// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern EInit g_EInitOlroxAfterImage;
extern u8 D_pspeu_0928FAC0[];
extern s32 E_ID(UNK_2F);
extern EInit D_pspeu_09275F60;
extern u8 D_pspeu_0928FAA8[];
extern u8 D_pspeu_0928FAB8[];
extern s32 D_pspeu_09290798;

s16 func_pspeu_09258DE0(s16 delta, s16 current, s16 target) {
    s16 diff;

    current &= 0xFFF;
    target &= 0xFFF;
    diff = target - current;
    if (diff > ROT(180)) {
        diff = diff - ROT(360);
    }
    if (diff < ROT(-180)) {
        diff = diff + ROT(360);
    }
    if (diff < -delta) {
        diff = -delta;
    }
    if (diff > delta) {
        diff = delta;
    }
    target = current + diff;
    target &= 0xFFF;
    return target;
}

s32 func_pspeu_09258EF0(s16 x, s16 y) {
    Collider collider;

    g_api.CheckCollision(x, y, &collider, 0);
    if (collider.effects & EFFECT_SOLID) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_us_801AD338);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925B1B0);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925C178);

void func_pspeu_0925C580(Entity* self) {
    Entity* entity;
    s16 angle;
    Entity* player;
    s16 x;
    s16 y;

    if (D_pspeu_09290798) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        PlaySfxPositional(SFX_EXPLODE_B);
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity) {
            CreateEntityFromEntity(E_ID(LASER_EXPLOSION), self, entity);
            entity->params = 2;
        }
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F60);
        self->hitboxState = 0;
        self->drawFlags |= ENTITY_OPACITY;
        self->hitboxOffY = -1;
        self->ext.et_801AF8C0.unkAA = ((Random() & 0x3F) * 0x10) + ROT(45);
        if (self->params) {
            self->step = 2;
            self->zPriority--;
            self->hitboxState = 0;
            self->blendMode |= BLEND_TRANSP | BLEND_ADD;
            break;
        }
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity) {
            CreateEntityFromEntity(E_ID(UNK_2F), self, entity);
            entity->params = 1;
            entity->ext.et_801AF8C0.parent = self;
        }
        break;
    case 1:
        if (self->opacity < 0x80) {
            self->opacity += 4;
            self->hitboxState = 0;
        } else {
            self->hitboxState = 3;
        }
        AnimateEntity(D_pspeu_0928FAB8, self);
        MoveEntity();
        if (self->velocityX > 0) {
            self->facingLeft = 1;
        } else {
            self->facingLeft = 0;
        }
        player = &PLAYER;
        x = player->posX.i.hi - self->posX.i.hi;
        y = player->posY.i.hi - self->posY.i.hi;
        y -= 0x20;
        angle = ratan2(-y, x);
        angle = func_pspeu_09258DE0(8, self->ext.et_801AF8C0.unkAA, angle);
        self->velocityX = rcos(angle) * 16;
        self->velocityY = -rsin(angle) * 16;
        self->ext.et_801AF8C0.unkAA = angle;
        break;
    case 2:
        AnimateEntity(D_pspeu_0928FAA8, self);
        entity = self->ext.et_801AF8C0.parent;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        self->opacity = entity->opacity;
        if (!entity->entityId) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_pspeu_0925C938(Entity* self) {
    Entity* entity;

    entity = self->ext.et_801AF8C0.parent;
    if (entity->flags & FLAG_DEAD) {
        self->flags |= FLAG_DEAD;
        self->step = 3;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitOlroxAfterImage);
        self->animCurFrame = 0x54;
        self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0;
        self->zPriority--;
        self->hitboxState = 0;
        break;
    case 1:
        AnimateEntity(D_pspeu_0928FAC0, self);
        if (self->scaleX < 0x100) {
            self->scaleX = self->scaleY += 8;
        } else {
            self->scaleX = self->scaleY = 0x100;
            self->step++;
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_0928FAC0, self);
        if (!(g_Timer % 12)) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[168]);
            if (entity) {
                CreateEntityFromEntity(E_ID(UNK_2F), self, entity);
                entity->ext.et_801AF8C0.parent = self;
                PlaySfxPositional(SFX_SMALL_FLAME_IGNITE);
            }
        }
        if (g_Timer % 5) {
            self->palette = PAL_FLAG(0x213);
        } else {
            self->palette = PAL_FLAG(0x215);
        }
        entity = self->ext.et_801AF8C0.parent;
        if (entity->ext.et_801AD338.unk85) {
            self->step++;
        }
        break;
    case 3:
        AnimateEntity(D_pspeu_0928FAC0, self);
        self->scaleX = self->scaleY -= 8;
        if (self->scaleX < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925CBE8);

void EntityOlroxAfterImage(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitOlroxAfterImage);
        self->hitboxState = 0;
        self->palette = 0x217;
        self->animCurFrame = self->params;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0x80;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->zPriority -= 2;
    }
    self->opacity -= 2;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_us_801B001C);
