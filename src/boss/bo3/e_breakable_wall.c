// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo3.h"

extern EInit g_EInitParticle;

void EntityBreakableWallDebris(Entity* self) {
    Collider collider;
    Entity* newEntity;
    s32 posX;
    s32 posY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->drawFlags = ENTITY_ROTATE;
        self->animSet = ANIMSET_OVL(1);
        self->palette = 0x800B;
        if (Random() & 1) {
            self->animCurFrame = 0x26;
        } else {
            self->animCurFrame = 0x27;
        }
        if (self->velocityX < 0) {
            self->facingLeft = 1;
        }
        // fallthrough
    case 1:
        MoveEntity();
        self->rotate += 0x20;
        if (self->params) {
            self->rotate += 0x20;
        }
        self->velocityY += FIX(0.125);
        posX = self->posX.i.hi;
        posY = self->posY.i.hi + 6;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            self->posY.i.hi += collider.unk18;
            if (self->velocityY < FIX(1.0)) {
                newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (newEntity != NULL) {
                    CreateEntityFromEntity(
                        E_INTENSE_EXPLOSION, self, newEntity);
                    newEntity->params = 0x10;
                }
                DestroyEntity(self);
                break;
            }

            self->velocityY = -self->velocityY / 2;
        }
        break;
    }
}
