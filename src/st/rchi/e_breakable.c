// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rchi.h"

/*
 * RCHI's breakable is the inverted-castle sibling of CHI's
 * e_breakable_with_lighting.h implementation: the light aura and debris
 * position offsets are applied in the opposite direction, the light is not
 * destroyed before being recreated, and the debris pops up with less force.
 */

#ifdef VERSION_PSP
extern s32 E_ID(BACKGROUND_BLOCK);
extern s32 E_ID(BREAKABLE_DEBRIS);
#endif

extern EInit g_EInitBreakable;

void CreateEntityFromCurrentEntity(u16 entityId, Entity* entity);
void ReplaceBreakableWithItemDrop(Entity*);

#define CANDLE_HIT_SFX SFX_FIRE_SHOT
#define BREAKABLE_ZPRIORITY 112

#define LIGHT_AURA 1
#define LIGHT_AURA_Y_OFFSET 16
#define LIGHT_AURA_Y_OFFSET_TALL 32
#define BASE_DEBRIS_COUNT 4
#define TALL_EXTRA_DEBRIS 3
#define DEBRIS_Y_OFFSET_TALL 20

static AnimateEntityFrame anim_brazier_short[] = {
    {3, 14}, {4, 15}, {4, 16}, {4, 17},
    {4, 18}, {4, 19}, {3, 20}, POSE_LOOP(0)};
static AnimateEntityFrame anim_brazier_tall[] = {
    {3, 21}, {4, 22}, {4, 23}, {4, 24},
    {4, 25}, {4, 26}, {3, 27}, POSE_LOOP(0)};
static AnimateEntityFrame* animations[] = {
    anim_brazier_short, anim_brazier_tall};
static u8 hitbox_heights[] = {12, 20};
static u8 explosion_types[] = {EXPLOSION_BIG, EXPLOSION_BIG};
static u16 anim_sets[] = {ANIMSET_OVL(2), ANIMSET_OVL(2)};
static u8 blend_modes[] = {BLEND_TRANSP | BLEND_ADD, BLEND_TRANSP | BLEND_ADD};
static s16 debris_offsets_y[] = {
    -4, -4, 3, -6, 2, 9, -4, 12, 0, 2, 0, 15, 0, 31};

// params: (>> 12) selects breakableType (0 or 1 in this stage);
//         a non-zero type is a "tall" breakable that breaks into more debris
void EntityBreakable(Entity* self) {
    Entity* entity;
    s16* debrisOffsetsY;
    u16 breakableType;
    s32 debrisCount;

    breakableType = self->params >> 12;
    if (!self->step) {
        InitializeEntity(g_EInitBreakable);
        self->zPriority = BREAKABLE_ZPRIORITY;
        self->blendMode = blend_modes[breakableType];
        self->hitboxHeight = hitbox_heights[breakableType];
        self->animSet = anim_sets[breakableType];

        entity = self + 1;
        CreateEntityFromEntity(E_ID(BACKGROUND_BLOCK), self, entity);
        // The inverted castle shifts the light aura downwards
        if (breakableType) {
            entity->posY.i.hi += LIGHT_AURA_Y_OFFSET_TALL;
        } else {
            entity->posY.i.hi += LIGHT_AURA_Y_OFFSET;
        }
        entity->params = LIGHT_AURA;
    }

    AnimateEntity(animations[breakableType], self);

    if (self->hitParams) {
        g_api.PlaySfx(CANDLE_HIT_SFX);
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_EXPLOSION, entity);
            entity->params = explosion_types[breakableType];
            entity->params |= 16;
        }
        // The inverted castle scatters the debris in the opposite direction
        debrisOffsetsY = debris_offsets_y;
        for (debrisCount = 0; debrisCount < BASE_DEBRIS_COUNT; debrisCount++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(BREAKABLE_DEBRIS), self, entity);
                entity->posX.i.hi -= *debrisOffsetsY++;
                entity->posY.i.hi -= *debrisOffsetsY++;
                if (breakableType) {
                    entity->posY.i.hi += DEBRIS_Y_OFFSET_TALL;
                }
                entity->params = debrisCount;
            }
        }

        if (breakableType) {
            for (debrisCount = 0; debrisCount < TALL_EXTRA_DEBRIS;
                 debrisCount++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(
                        E_ID(BREAKABLE_DEBRIS), self, entity);
                    entity->posX.i.hi -= *debrisOffsetsY++;
                    entity->posY.i.hi -= *debrisOffsetsY++;
                    entity->params = debrisCount + BASE_DEBRIS_COUNT;
                }
            }
        }

        entity = self + 1;
        DestroyEntity(entity);
        ReplaceBreakableWithItemDrop(self);
    }
}

enum BreakableDebrisSteps {
    INIT,
    UPDATE,
};

// params: Index of breakable debris to use
//         (>= 4) Considered a "tall" breakable debris
//         (== 6) The "last standing" debris, that doesn't move
void EntityBreakableDebris(Entity* self) {
    Entity* entity;
    Collider collider;
    Primitive* prim;
    s32 primIndex;
    s32 facingLeft;

    switch (self->step) {
    case INIT:
        InitializeEntity(g_EInitBreakable);
        self->animSet = ANIMSET_OVL(2);
        self->hitboxState = 0;
        self->zPriority = BREAKABLE_ZPRIORITY;
        self->drawFlags = ENTITY_ROTATE;
        self->animCurFrame = self->params + 28;
        facingLeft = GetSideToPlayer() & 1;

        self->ext.breakableDebris.angle = (Random() & 30) + 8;
        if (self->facingLeft) {
            self->ext.breakableDebris.angle = -self->ext.breakableDebris.angle;
        }
        if (self->params > BASE_DEBRIS_COUNT - 1) {
            self->ext.breakableDebris.angle = -self->ext.breakableDebris.angle;
        }

        if (facingLeft) {
            self->velocityX = FIX(1);
        } else {
            self->velocityX = FIX(-1);
        }
        self->velocityX += FIX(0.5) - (Random() << 8);
        self->velocityY = FIX(-2.25);
        self->velocityY += (self->params >> 1) * FIX(0.375);

        // "Last standing" debris just sits there (tall breakables only)
        if (self->params == 6) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->step = 2;
        }

        self->primIndex = 0;
        if (!self->params) {
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];

            UnkPolyFunc2(prim);

            prim->tpage = 26;
            prim->clut = 0x159;
            prim->u0 = prim->u2 = 64;
            prim->u1 = prim->u3 = 96;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 32;

            prim->next->x1 = self->posX.i.hi - 4;
            prim->next->y0 = self->posY.i.hi + 8;
            LOH(prim->next->r2) = 32;
            LOH(prim->next->b2) = 32;
            prim->next->b3 = 16;

            prim->priority = BREAKABLE_ZPRIORITY + 2;
            prim->drawMode = DRAW_TRANSP | DRAW_UNK02 | DRAW_COLORS |
                             DRAW_TPAGE | DRAW_TPAGE2;
        }
        break;

    case UPDATE:
        MoveEntity();
        self->rotate += self->ext.breakableDebris.angle;
        self->velocityY += FIX(0.25);
        g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi + 6, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            self->posY.i.hi += collider.unk18;
            self->velocityY = -self->velocityY / 2;
            self->velocityX -= self->velocityX / 3;
            if (self->velocityY > FIX(-0.625)) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, entity);
                    entity->params = 16;
                }
                DestroyEntity(self);
                break;
            }
        }

        if (self->primIndex != 0) {
            prim = &g_PrimBuf[self->primIndex];
            UnkPrimHelper(prim);
            LOH(prim->next->r2) = LOH(prim->next->b2) += 4;
            if (LOH(prim->next->r2) > 64) {
                prim->next->b3 -= 4;
                if (!prim->next->b3) {
                    g_api.FreePrimitives(self->primIndex);
                    self->flags &= ~FLAG_HAS_PRIMS;
                    self->primIndex = 0;
                }
            }
        }
        break;
    }
}
