// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern EInit g_EInitOlroxAfterImage;
extern u8 D_pspeu_0928FAC0[];
extern s32 E_ID(UNK_2F);

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

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925C580);

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
            self->palette = 0x8213;
        } else {
            self->palette = 0x8215;
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
