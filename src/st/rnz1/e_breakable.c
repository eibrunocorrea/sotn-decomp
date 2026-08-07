// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rnz1.h"

/*
 * RNZ1's breakable mirrors NZ1's stage-specific implementation for the
 * inverted castle: the wall sconce flame primitive is flipped on both axes,
 * the candelabra debris shower falls upward, and the urn and jug share a
 * single heart drop index instead of NZ1's distinct ones.
 */

enum BreakableTypes {
    CANDELABRA_WALL_DOUBLE,
    WALL_SCONCE_FLAME,
    CANDELABRA_TALL,
    CANDELABRA_SHORT,
    CANDELABRA_TABLE,
    BRAZIER,
    CANDELABRA_WALL_TRIPLE,
    URN,
    JUG,
    BUST,
};

enum BreakableDebrisSteps {
    INIT,
    UPDATE,
    DEBRIS_NOP = 256,
};

#ifdef VERSION_PSP
extern s32 E_ID(BREAKABLE_DEBRIS);
#endif

extern EInit g_EInitBreakable;
extern EInit g_EInitInteractable;
extern EInit g_EInitParticle;
extern AnimateEntityFrame* animations[];
extern u8 hitbox_heights[];
extern u8 explosion_types[];
extern u16 palettes[];
extern u16 anim_sets[];
extern u16 unk_5A[];
extern u8 blend_modes[];
extern u16 hitbox_offsets_y[];
extern s16 candelabra_debris_offsets_y[];

void EntityBreakable(Entity* self) {
    Entity* entity;
    Primitive* prim;
    s16* debrisOffsetsY;
    s16 primIndex;
    u16 breakableType;
    s32 debrisIndex;
    s32 debrisCount;
    s16 posX, posY;

    breakableType = self->params >> 12;
    if (!self->step) {
        InitializeEntity(g_EInitBreakable);
        self->zPriority = g_unkGraphicsStruct.g_zEntityCenter - 20;
        self->blendMode = blend_modes[breakableType];
        self->hitboxHeight = hitbox_heights[breakableType];
        self->animSet = anim_sets[breakableType];
        self->unk5A = unk_5A[breakableType];
        self->palette = palettes[breakableType];
        self->hitboxOffY = hitbox_offsets_y[breakableType];

        if (breakableType == WALL_SCONCE_FLAME) {
            self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
            if (self->primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            self->flags |= FLAG_HAS_PRIMS;
            prim = &g_PrimBuf[self->primIndex];
            prim->tpage = 21;
            prim->u0 = prim->u2 = 200;
            prim->u1 = prim->u3 = 248;
            prim->v0 = prim->v1 = 128;
            prim->v2 = prim->v3 = 160;
            // These are equivalent to the -25 and -9, but pspeu doesn't like
            // subtraction here
            posX = self->posX.i.hi;
            prim->x0 = prim->x2 = posX + 23;
            prim->x1 = prim->x3 = posX + 0xFFE7;
            primIndex = self->posY.i.hi;
            prim->y0 = prim->y1 = primIndex + 23;
            prim->y2 = prim->y3 = primIndex + 0xFFF7;
            prim->priority = self->zPriority;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
        }
    }
    AnimateEntity(animations[breakableType], self);
    if (breakableType == WALL_SCONCE_FLAME) {
        prim = &g_PrimBuf[self->primIndex];
        if (g_Timer & 2) {
            prim->clut = PAL_WALL_CANDELABRA_A;
        } else {
            prim->clut = PAL_WALL_CANDELABRA_B;
        }
    }
    if (self->hitParams) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_EXPLOSION, entity);
            entity->params = explosion_types[breakableType];
        }
        switch (breakableType) {
        case WALL_SCONCE_FLAME:
            g_api.FreePrimitives(self->primIndex);
            self->flags &= ~FLAG_HAS_PRIMS;
            break;
        case CANDELABRA_TALL:
        case CANDELABRA_SHORT:
            self->facingLeft = GetSideToPlayer() & 1;
            posY = self->posY.i.hi + 40;
            if (breakableType == CANDELABRA_TALL) {
                debrisCount = 4;
            } else {
                debrisCount = 3;
            }
            debrisOffsetsY = candelabra_debris_offsets_y;
            if (breakableType == CANDELABRA_SHORT) {
                debrisOffsetsY += 5;
            }
            for (debrisIndex = 0; debrisIndex < debrisCount; debrisIndex++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_ID(BREAKABLE_DEBRIS), self, entity);
                    entity->posY.i.hi = posY;
                    entity->params = debrisOffsetsY[debrisIndex];
                    entity->facingLeft = self->facingLeft;
                }
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, entity);
                    entity->posY.i.hi = posY;
                    entity->params = EXPLOSION_SMALL;
                }
                posY -= 16;
            }
            g_api.PlaySfx(SFX_CANDLE_HIT);
            break;
        case BUST:
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromCurrentEntity(E_ID(BREAKABLE_DEBRIS), entity);
                entity->params = 256;
            }
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            break;
        case URN:
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_HEART_DROP, self, entity);
                entity->params = 0;
            }
            PreventEntityFromRespawning(self);
            DestroyEntity(self);
            return;
        case JUG:
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_HEART_DROP, self, entity);
                entity->params = 0;
            }
            PreventEntityFromRespawning(self);
            DestroyEntity(self);
            return;
        default:
            g_api.PlaySfx(SFX_CANDLE_HIT);
            break;
        }
        ReplaceBreakableWithItemDrop(self);
    }
}

void EntityBreakableDebris(Entity* self) {
    Collider collider;
    Entity* explosion;
    Primitive* prim;
    s32 primIndex;
    s16 posX, posY;

    switch (self->step) {
    case INIT:
        // Always applies to the bust
        // Applies to the urn and jug if they have params & 0x1FF
        // Doesn't apply to any others
        if (self->params & 256) {
            InitializeEntity(g_EInitInteractable);
            self->animSet = ANIMSET_OVL(10);
            self->unk5A = 91;
            self->palette = PAL_BREAKABLE;
            self->animCurFrame = 21;
            self->zPriority = 106;
            self->step = DEBRIS_NOP; // No case defined, resulting in nop
            return;
        } else {
            InitializeEntity(g_EInitParticle);
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.breakableDebris.prim = prim;
            UnkPolyFunc2(prim);
            prim->tpage = 22;
            prim->clut = PAL_BREAKABLE_DEBRIS;
            prim->u0 = prim->u2 = 152;
            prim->u1 = prim->u3 = 167;
            posY = 132;
            posY += self->params * 16;
            prim->v0 = prim->v1 = posY + 15;
            prim->v2 = prim->v3 = posY;
            prim->next->x1 = self->posX.i.hi;
            prim->next->y0 = self->posY.i.hi;
            LOH(prim->next->r2) = 16;
            LOH(prim->next->b2) = 16;
            prim->next->b3 = 128;
            prim->priority = self->zPriority;
            prim->drawMode = DRAW_UNK02;
            self->velocityX = ((Random() & 7) << 12) + FIX(0.5);
            if (!self->facingLeft) {
                self->velocityX = -self->velocityX;
            }
            self->velocityY = ((Random() & 7) << 12) - FIX(0.5);
        }
    case UPDATE:
        MoveEntity();
        self->velocityY += FIX(0.125);
        prim = self->ext.breakableDebris.prim;
        prim->next->x1 = self->posX.i.hi;
        prim->next->y0 = self->posY.i.hi;
        if (self->facingLeft) {
            LOH(prim->next->tpage) += 16;
        } else {
            LOH(prim->next->tpage) -= 16;
        }
        UnkPrimHelper(prim);
        posX = self->posX.i.hi;
        posY = self->posY.i.hi + 8;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            g_api.PlaySfx(SFX_QUICK_STUTTER_EXPLODE_B);
            explosion = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (explosion != NULL) {
                CreateEntityFromCurrentEntity(E_EXPLOSION, explosion);
                explosion->params = EXPLOSION_SMALL;
            }
            DestroyEntity(self);
        }
        break;
    }
}
