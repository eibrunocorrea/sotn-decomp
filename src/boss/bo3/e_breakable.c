// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo3.h"
#include <sfx.h>

/*
 * BO3's breakable is a cousin of NZ1's stage-specific implementation: same
 * candelabra/urn/jug/bust cast, but without NZ1's wall sconce flame primitive
 * and debris shower, and the urn takes its heart drop from the placement
 * params instead of a hardcoded index.
 */

void CreateEntityFromCurrentEntity(u16 entityId, Entity* entity);
void ReplaceBreakableWithItemDrop(Entity* entity);
void PreventEntityFromRespawning(Entity* entity);

extern EInit g_EInitBreakable;

enum BreakableTypes {
    CANDELABRA_WALL_DOUBLE,
    BREAKABLE_TYPE_1,
    CANDELABRA_TALL,
    CANDELABRA_SHORT,
    BREAKABLE_TYPE_4,
    BRAZIER,
    CANDELABRA_WALL_TRIPLE,
    URN,
    JUG,
    BUST,
};

static AnimateEntityFrame anim_type0[] = {{4, 1}, {4, 2}, POSE_LOOP(0)};
static AnimateEntityFrame anim_type1[] = {{4, 0}, {4, 0}, POSE_LOOP(0)};
static AnimateEntityFrame anim_candelabra_tall[] = {
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, POSE_LOOP(0)};
static AnimateEntityFrame anim_candelabra_short[] = {
    {5, 5}, {5, 6}, {5, 7}, {5, 8}, POSE_LOOP(0)};
static AnimateEntityFrame anim_type4[] = {POSE_LOOP(0)};
static AnimateEntityFrame anim_brazier[] = {
    {5, 13}, {5, 14}, {5, 15}, {5, 16}, POSE_LOOP(0)};
static AnimateEntityFrame anim_candelabra_wall_triple[] = {
    {5, 17}, {5, 18}, {5, 19}, POSE_LOOP(0)};
static AnimateEntityFrame anim_urn[] = {{5, 23}, POSE_LOOP(0)};
static AnimateEntityFrame anim_jug[] = {{5, 22}, POSE_LOOP(0)};
static AnimateEntityFrame anim_bust[] = {
    {5, 20}, {-1, -1}, {5, 21}, {5, 21}, POSE_END};

static AnimateEntityFrame* animations[] = {
    anim_type0,
    anim_type1,
    anim_candelabra_tall,
    anim_candelabra_short,
    anim_type4,
    anim_brazier,
    anim_candelabra_wall_triple,
    anim_urn,
    anim_jug,
    anim_bust,
};
static u8 hitbox_heights[] = {8, 8, 40, 24, 16, 16, 8, 8, 8, 8, 8};
static u8 explosion_types[] = {
    EXPLOSION_SMALL,          EXPLOSION_SMALL,
    EXPLOSION_SMALL_MULTIPLE, EXPLOSION_SMALL_MULTIPLE,
    EXPLOSION_SMALL_MULTIPLE, EXPLOSION_SMALL_MULTIPLE,
    EXPLOSION_SMALL_MULTIPLE, EXPLOSION_SMALL_MULTIPLE,
    EXPLOSION_SMALL_MULTIPLE, EXPLOSION_SMALL_MULTIPLE,
};
static u16 palettes[] = {
    PAL_NONE, PAL_NONE, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228,
};
static u16 anim_sets[] = {
    ANIMSET_DRA(3),  ANIMSET_DRA(3),  ANIMSET_OVL(11), ANIMSET_OVL(11),
    ANIMSET_OVL(11), ANIMSET_OVL(11), ANIMSET_OVL(11), ANIMSET_OVL(11),
    ANIMSET_OVL(11), ANIMSET_OVL(11),
};
static u16 unk_5A[] = {0, 0x7C, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5B};
static u8 blend_modes[] = {
    BLEND_TRANSP | BLEND_QUARTER, BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,     BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,     BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,     BLEND_TRANSP | BLEND_ADD,
    BLEND_TRANSP | BLEND_ADD,     BLEND_TRANSP | BLEND_ADD,
};
static s16 hitbox_offsets_y[] = {0, 0, -24, -16, 0, 0, 0, 0, 0, 0};

void EntityBreakable(Entity* entity) {
    Entity* child;
    u16 breakableType = entity->params >> 12;

    if (!entity->step) {
        InitializeEntity(g_EInitBreakable);
        entity->zPriority = g_unkGraphicsStruct.g_zEntityCenter - 20;
        entity->blendMode = blend_modes[breakableType];
        entity->hitboxHeight = hitbox_heights[breakableType];
        entity->animSet = anim_sets[breakableType];
        entity->unk5A = unk_5A[breakableType];
        entity->palette = palettes[breakableType];
        entity->hitboxOffY = hitbox_offsets_y[breakableType];
    }

    AnimateEntity(animations[breakableType], entity);

    if (entity->hitParams) {
        child = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (child != NULL) {
            CreateEntityFromCurrentEntity(E_EXPLOSION, child);
            child->params = explosion_types[breakableType];
        }

        switch (breakableType) {
        case BUST:
            child = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (child != NULL) {
                CreateEntityFromCurrentEntity(E_UNK_43, child);
                child->params = 0x100;
            }
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            break;
        case URN:
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            child = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (child != NULL) {
                CreateEntityFromEntity(E_HEART_DROP, entity, child);
                child->params = entity->params & 0x1FF;
            }
            PreventEntityFromRespawning(entity);
            DestroyEntity(entity);
            return;
        case JUG:
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            child = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (child != NULL) {
                CreateEntityFromEntity(E_HEART_DROP, entity, child);
                child->params = 0x29;
            }
            PreventEntityFromRespawning(entity);
            DestroyEntity(entity);
            return;
        case CANDELABRA_TALL:
        case CANDELABRA_SHORT:
            break;
        default:
            g_api.PlaySfx(SFX_CANDLE_HIT);
            break;
        }
        ReplaceBreakableWithItemDrop(entity);
    }
}
