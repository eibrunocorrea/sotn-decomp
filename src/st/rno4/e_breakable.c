// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

extern EInit g_EInitBreakable;

static u8 D_us_80180D50[] = {0x04, 0x01, 0x04, 0x02, 0x00};
static u8 D_us_80180D58[] = {0x04, 0x00, 0x04, 0x00, 0x00};
static u8 D_us_80180D60[] = {
    0x05, 0x01, 0x05, 0x02, 0x05, 0x03, 0x05, 0x04, 0x00};
static u8 D_us_80180D6C[] = {
    0x05, 0x05, 0x05, 0x06, 0x05, 0x07, 0x05, 0x08, 0x00};
static u8 D_us_80180D78[] = {0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180D7C[] = {
    0x05, 0x0D, 0x05, 0x0E, 0x05, 0x0F, 0x05, 0x10, 0x00};
static u8 D_us_80180D88[] = {0x05, 0x11, 0x05, 0x12, 0x05, 0x13, 0x00};
static u8 D_us_80180D90[] = {0x05, 0x17, 0x00, 0x00};
static u8 D_us_80180D94[] = {0x05, 0x16, 0x00, 0x00};
static u8 D_us_80180D98[] = {
    0x05, 0x14, 0xFF, 0xFF, 0x05, 0x15, 0x05, 0x15, 0xFF, 0x00, 0x00, 0x00};
static u8* anims[] = {
    D_us_80180D50, D_us_80180D58, D_us_80180D60, D_us_80180D6C, D_us_80180D78,
    D_us_80180D7C, D_us_80180D88, D_us_80180D90, D_us_80180D94, D_us_80180D98};

static u8 hitbox_heights[] = {
    0x08, 0x08, 0x28, 0x18, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00};
static u8 params_arr[] = {
    0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00};
static u16 palettes[] = {
    PAL_NULL, PAL_NULL, 0x25D, 0x25D, 0x25D, 0x25D, 0x25D, 0x25D, 0x25D, 0x25D};
static u16 anim_sets[] = {
    ANIMSET_DRA(0x3), ANIMSET_DRA(0x3), ANIMSET_OVL(0x9), ANIMSET_OVL(0x9),
    ANIMSET_OVL(0x9), ANIMSET_OVL(0x9), ANIMSET_OVL(0x9), ANIMSET_OVL(0x9),
    ANIMSET_OVL(0x9), ANIMSET_OVL(0x9)};
static u16 unk5a_arr[] = {0x0000, 0x007C, 0x005B, 0x005B, 0x005B,
                          0x005B, 0x005B, 0x005B, 0x005B, 0x005B};
static u8 blend_modes[] = {
    DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE,
    DRAW_TPAGE2 | DRAW_TPAGE};
static u16 hitbox_y_offsets[] = {0x0000, 0x0000, 0xFFE8, 0xFFF0, 0x0000,
                                 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

void EntityBreakable(Entity* self) {
    Entity* tempEntity;
    u16 params;

    params = self->params >> 0xC;
    if (!self->step) {
        InitializeEntity(g_EInitBreakable);
        self->zPriority = g_unkGraphicsStruct.g_zEntityCenter - 0x14;
        self->blendMode = blend_modes[params];
        self->hitboxHeight = hitbox_heights[params];
        self->animSet = anim_sets[params];
        self->unk5A = unk5a_arr[params];
        self->palette = palettes[params];
        self->hitboxOffY = hitbox_y_offsets[params];
        if (!params) {
            self->drawFlags = ENTITY_ROTATE;
            self->rotate = ROT(180);
        }
    }
    AnimateEntity(anims[params], self);
    if (self->hitParams) {
        tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (tempEntity != NULL) {
            CreateEntityFromCurrentEntity(E_EXPLOSION, tempEntity);
            tempEntity->params = params_arr[params];
        }
        switch (params) {
        case 0:
            self->drawFlags = ENTITY_DEFAULT;
            self->rotate = ROT(0);
            g_api.PlaySfx(SFX_CANDLE_HIT);
            break;

        case 9:
            tempEntity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (tempEntity != NULL) {
                CreateEntityFromCurrentEntity(E_UNK_3C, tempEntity);
                tempEntity->params = 0x100;
            }
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            break;

        case 7:
        case 8:
            g_api.PlaySfx(SFX_GLASS_BREAK_E);
            tempEntity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(E_HEART_DROP, self, tempEntity);
                tempEntity->params = self->params & 0x1FF;
            }
            PreventEntityFromRespawning(self);
            DestroyEntity(self);
            return;

        case 2:
        case 3:
            break;

        default:
            g_api.PlaySfx(SFX_CANDLE_HIT);
            break;
        }
        ReplaceBreakableWithItemDrop(self);
    }
}
