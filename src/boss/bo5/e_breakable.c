// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"
#include <sfx.h>

// BO5's breakable adjusts the hitbox offsets after the common setup (with a
// tighter hitbox for breakableType 0), so it cannot reuse st/e_breakable.h.
static u8 anim_1[] = {3, 3, 3, 4, 3, 5, 3, 6, 0, 0};
static u8 anim_2[] = {3, 7, 3, 8, 3, 9, 3, 10, 3, 11, 0, 0};
static u8* g_eBreakableAnimations[8] = {anim_1, anim_2};
static u8 g_eBreakableHitboxes[8] = {8, 8};
static u8 g_eBreakableExplosionTypes[8] = {0, 0};
static u16 g_eBreakableanimSets[8] = {ANIMSET_OVL(1), ANIMSET_OVL(1)};

static u8 blend_modes[8] = {
    BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,
};

static u8 unused[] = {0, 0, 0, 0, 0, 0, 0, 0};

extern EInit g_EInitBreakable;

void CreateEntityFromCurrentEntity(u16 entityId, Entity* entity);
void ReplaceBreakableWithItemDrop(Entity*);
Entity* AllocEntity(Entity* start, Entity* end);

void EntityBreakable(Entity* entity) {
    u16 breakableType = entity->params >> 12;
    if (entity->step) {
        AnimateEntity(g_eBreakableAnimations[breakableType], entity);
        if (entity->hitParams) { // If the candle is destroyed
            Entity* entityDropItem;
            g_api.PlaySfx(SFX_CANDLE_HIT);
            entityDropItem = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entityDropItem != NULL) {
                CreateEntityFromCurrentEntity(E_EXPLOSION, entityDropItem);
                entityDropItem->params =
                    g_eBreakableExplosionTypes[breakableType];
            }
            ReplaceBreakableWithItemDrop(entity);
        }
    } else {
        InitializeEntity(g_EInitBreakable);
        entity->zPriority = g_unkGraphicsStruct.g_zEntityCenter - 20;
        entity->blendMode = blend_modes[breakableType];
        entity->hitboxHeight = g_eBreakableHitboxes[breakableType];
        entity->animSet = g_eBreakableanimSets[breakableType];
        entity->hitboxOffX = 8;
        entity->hitboxOffY = 8;
        if (breakableType == 0) {
            entity->hitboxWidth = 4;
            entity->hitboxOffX = 2;
            entity->hitboxOffY = 2;
        }
    }
}
