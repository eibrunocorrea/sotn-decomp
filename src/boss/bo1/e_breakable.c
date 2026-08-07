// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo1.h"
#include <sfx.h>

// BO1's breakable overrides zPriority with a per-type table right after the
// common g_zEntityCenter computation, so it cannot reuse st/e_breakable.h.
static u8 anim_1[] = {6, 4, 6, 5, 6, 6, 6, 7, 6, 8, 0, 0};
static u8 anim_2[] = {6, 9, 6, 10, 6, 11, 6, 12, 6, 13, 0, 0};
static u8 anim_3[] = {4, 14, 4, 15, 4, 14, 4, 15, 6, 16, 0, 0};
static u8* g_eBreakableAnimations[4] = {anim_1, anim_2, anim_3, NULL};
static u8 g_eBreakableHitboxes[4] = {8, 8, 8, 8};
static u8 g_eBreakableExplosionTypes[4] = {0, 0, 0, 0};
static u16 g_eBreakableanimSets[4] = {
    ANIMSET_OVL(2), ANIMSET_OVL(2), ANIMSET_OVL(2), ANIMSET_OVL(2)};
static u8 blend_modes[4] = {
    BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,
};
static u16 g_eBreakableZPriorities[8] = {0xA8, 0xA8, 0x60, 0x60};

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
        entity->zPriority = g_eBreakableZPriorities[breakableType];
        entity->blendMode = blend_modes[breakableType];
        entity->hitboxHeight = g_eBreakableHitboxes[breakableType];
        entity->animSet = g_eBreakableanimSets[breakableType];
    }
}
