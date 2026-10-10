// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo3.h"

static u8 D_us_801806E4[] = {0x04, 0x01, 0x04, 0x02, 0x00};
static u8 D_us_801806EC[] = {0x04, 0x00, 0x04, 0x00, 0x00};
static u8 D_us_801806F4[] = {
    0x05, 0x01, 0x05, 0x02, 0x05, 0x03, 0x05, 0x04, 0x00};
static u8 D_us_80180700[] = {
    0x05, 0x05, 0x05, 0x06, 0x05, 0x07, 0x05, 0x08, 0x00};
static u8 D_us_8018070C[] = {0x00, 0x00, 0x00, 0x00};
static u8 D_us_80180710[] = {
    0x05, 0x0D, 0x05, 0x0E, 0x05, 0x0F, 0x05, 0x10, 0x00};
static u8 D_us_8018071C[] = {0x05, 0x11, 0x05, 0x12, 0x05, 0x13, 0x00};
static u8 D_us_80180724[] = {0x05, 0x17, 0x00, 0x00};
static u8 D_us_80180728[] = {0x05, 0x16, 0x00, 0x00};
static u8 D_us_8018072C[] = {
    0x05, 0x14, 0xFF, 0xFF, 0x05, 0x15, 0x05, 0x15, 0xFF, 0x00, 0x00, 0x00};
static u8* anims[] = {
    D_us_801806E4, D_us_801806EC, D_us_801806F4, D_us_80180700, D_us_8018070C,
    D_us_80180710, D_us_8018071C, D_us_80180724, D_us_80180728, D_us_8018072C};

static u8 hitbox_heights[] = {
    0x08, 0x08, 0x28, 0x18, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00};
static u8 params_arr[] = {
    0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00};
static u16 palettes[] = {
    PAL_NULL, PAL_NULL, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228, 0x228};
static u16 anim_sets[] = {
    ANIMSET_DRA(0x3), ANIMSET_DRA(0x3), ANIMSET_OVL(0xB), ANIMSET_OVL(0xB),
    ANIMSET_OVL(0xB), ANIMSET_OVL(0xB), ANIMSET_OVL(0xB), ANIMSET_OVL(0xB),
    ANIMSET_OVL(0xB), ANIMSET_OVL(0xB)};
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

#define E_BREAKABLE_RELIC E_ID(UNK_43)

#include "../../st/e_breakable_no4.h"

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C123C_from_no4);

extern u16 g_EInitInteractable;
extern u8 D_us_8018080C[];
extern u8 D_us_80180824[];
extern s16 D_us_80180830[];

void func_us_801C12B0_from_no4(Entity* self) {
    u32 var_s8;
    s16* ptr;
    s32 var_s6;
    u8* var_s5;
    s32 var_s4;
    s32 var_s3;
    s32 var_s2;
    s32 var_s1;

    Primitive* prim;
    s32 primIndex;
    s32 scrollX;
    s32 scrollY;
    s32 i;
    s32 xOffset;
    s32 sp38;

    switch (self->step) {
    case 0:
        InitializeEntity(&g_EInitInteractable);
        self->animSet = 0;
        self->ext.et_801C12B0.unk80 = 4;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 16);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C12B0.prim = prim;
        self->ext.et_801C12B0.unk82 = 0;
        while (prim != NULL) {
            prim->tpage = 0xF;
            prim->clut = 0x5E;
            prim->priority = 0x9C;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        break;
    }

    prim = self->ext.et_801C12B0.prim;
    AnimateEntity(D_us_80180824, self);
    i = (self->params >> 8) & 0xFF;
    ptr = &D_us_80180830[(self->params & 0xFF) * 5];
    scrollX = g_Tilemap.scrollX.i.hi - 0x10;
    scrollY = g_Tilemap.scrollY.i.hi;
    xOffset = scrollX + 0x120;
    var_s5 = &D_us_8018080C[self->animCurFrame * 4];
    var_s8 = var_s5[1];

    for (; i > 0; i--) {
        var_s3 = *ptr++;
        var_s2 = var_s3 + *ptr++;
        if (scrollX >= var_s2 || xOffset < var_s3) {
            ptr += 3;
            continue;
        }

        var_s6 = *ptr++;
        if (scrollY - 4 > var_s6 || var_s6 > scrollY + 0xE0) {
            ptr += 2;
            continue;
        } else {
            ptr++;
            sp38 = *ptr++;
            var_s6 -= scrollY;
            if (var_s3 < scrollX) {
                var_s3 = scrollX;
            }
            if (xOffset < var_s2) {
                var_s2 = xOffset;
            }
            if (sp38 != 0) {
                self->ext.et_801C12B0.unk82 += sp38;
                var_s4 = (u32)(var_s3 - self->ext.et_801C12B0.unk82) % 126;
            } else {
                var_s4 = (u32)var_s3 % 126;
            }
            var_s2 -= var_s3;
            var_s3 -= scrollX + 0x10;
            var_s4 += var_s5[0];

            do {
                prim->u0 = prim->u2 = var_s4;
                var_s1 = 0x7E - (var_s4 - var_s5[0]);
                if (var_s2 < var_s1) {
                    var_s1 = var_s2;
                }
                prim->u1 = prim->u3 = var_s4 + var_s1;
                prim->v0 = prim->v1 = var_s8;
                prim->v2 = prim->v3 = var_s8 + 1;
                prim->x0 = prim->x2 = var_s3;
                var_s3 += var_s1;
                prim->x1 = prim->x3 = var_s3;
                var_s2 -= var_s1;
                var_s4 = var_s5[0];
                prim->y0 = prim->y1 = var_s6;
                prim->y2 = prim->y3 = var_s6 + 1;
                prim->drawMode = DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                prim = prim->next;
            } while (var_s2 != 0);
        }
    }

    while (prim != NULL) {
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C15F8_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C1844_from_no4);

extern s16 D_us_80180BC4[];

void EntityBgColumnsParallax_from_no4(Entity* self) {
    s32 primIndex;
    s32 scrollY;
    s32 scrollX;
    s32 var_s5;
    s16* ptr;
    s32 var_s3;
    s32 var_s2;
    s32 var_s1;
    Primitive* prim;

    if (!self->step) {
        InitializeEntity(&g_EInitInteractable);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 10);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C12B0.prim = prim;
        while (prim != NULL) {
            prim->tpage = 0xE;
            prim->clut = 0x84;
            prim->u0 = prim->u2 = 0xE9;
            prim->u1 = prim->u3 = 0xFD;
            prim->v0 = prim->v1 = 1;
            prim->v2 = prim->v3 = 0x7F;
            prim->priority = 0x1C;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }

    prim = self->ext.et_801C12B0.prim;
    ptr = D_us_80180BC4;
    scrollX = g_Tilemap.scrollX.i.hi;
    scrollX /= 2;
    scrollX &= 0xFF;
    scrollY = g_Tilemap.scrollY.i.hi;

    if (scrollY < 0xB0) {
        scrollY /= 2;
        while (1) {
            var_s1 = *ptr++;
            var_s2 = *ptr++;
            var_s5 = *ptr++;
            var_s1 -= scrollX;
            var_s2 -= scrollY;
            var_s3 = var_s2 + 0x7E;
            if (var_s1 < -0x14) {
                continue;
            }

            if (var_s1 >= 0x100) {
                break;
            }

            if (var_s3 >= 0 && var_s2 < 0xE0) {
                if (var_s5 & 1) {
                    prim->x0 = prim->x2 = var_s1 + 0x14;
                    prim->x1 = prim->x3 = var_s1;
                } else {
                    prim->x0 = prim->x2 = var_s1;
                    prim->x1 = prim->x3 = var_s1 + 0x14;
                }

                if (var_s5 & 2) {
                    prim->y0 = prim->y1 = var_s2;
                    prim->y2 = prim->y3 = var_s3;
                } else {
                    prim->y0 = prim->y1 = var_s3;
                    prim->y2 = prim->y3 = var_s2;
                }

                prim->drawMode = DRAW_DEFAULT;
                prim = prim->next;
            }
        }
    }

    while (prim != NULL) {
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C1EE4_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C21AC_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_8019E398);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C2850_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C2B78_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C2E60_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_8019F03C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C34EC_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C37C8_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C3A04_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C3CC4_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C3FB0_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C4228_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", EntityWaterBox);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A051C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A069C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A071C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A07CC);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A09DC);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A0A80);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A1120);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A16E4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C5518_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801B5004_from_rbo5);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A1BE8);

extern s32 g_SplashYMovement[]; // velocityY/accelY pairs
extern s16 g_splashAspects[];
extern EInit g_EInitCommon;

// twin of EntitySplashWater (src/st/water_effects.h); this overlay plays a
// fixed splash SFX instead of indexing the (per-stage) g_WaterSounds table

void EntitySplashWater(Entity* self) {
    s32 primIndex;
    Tilemap* tilemap = &g_Tilemap;
    u16 params;
    u16 index1;
    u16 width;
    s16 aspect; // sets y size based on x size
    u16 index2;

    Primitive *prim, *prim2;
    Entity* newEntity;
    s16 selfY;
    s16 selfX;
    s32 i;

    params = self->params;
    width = params / 0x800;
    index1 = (params >> 8) & 7;
    index2 = (params >> 5) & 7;
    params &= 0xF;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        if (width && index2 != 7) {
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        } else {
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        }
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        i = 0;
        selfX = self->posX.i.hi;
        selfY = self->posY.i.hi;
        self->ext.waterEffects.unk82 = selfY + tilemap->scrollY.i.hi;
        for (; prim != NULL; i++, prim = prim->next) {
            if (i & 1) {
                prim->u0 = prim->u2 = prim2->u0;
                prim->u1 = prim->u3 = prim2->u1;
                prim->v0 = prim->v1 = prim2->v0;
                prim->v2 = prim->v3 = prim2->v2;
                prim->y2 = prim2->y2;
                prim->y3 = prim2->y3;
                prim->x2 = prim->x0 = prim2->x0;
                prim->x3 = prim->x1 = prim2->x1;
            } else {
                prim->u0 = prim->u2 = 0;
                prim->u1 = prim->u3 = 0x20;
                prim->v0 = prim->v1 = 96;
                prim->v2 = prim->v3 = 0x7C;
                prim->y2 = prim->y3 = selfY;
                prim->x2 = prim->x0 = selfX - 0xE;
                prim->x3 = prim->x1 = selfX + 0xE;
                if (i > 1) {
                    aspect = g_splashAspects[index2];
                    if (width > 14) {
                        prim->u0 = prim->u2 = prim2->u1;
                        prim->x0 = prim->x2 = prim2->x1;
                        prim->y2 = prim2->y3;
                        if (aspect) {
                            prim->y3 =
                                prim->y2 - (prim->x1 - prim->x0) / aspect;
                        } else {
                            prim->y3 = prim->y2;
                        }
                    } else {
                        prim->u1 = prim->u3 = prim2->u0;
                        prim->x1 = prim->x3 = prim2->x0;
                        prim->y3 = prim2->y2;
                        if (aspect) {
                            prim->y2 =
                                prim->y3 + (prim->x1 - prim->x0) / aspect;
                        } else {
                            prim->y2 = prim->y3;
                        }
                    }
                } else {
                    if (width) {
                        if (width > 14) {
                            prim->u1 = prim->u3 = prim->u0 + (width << 5) / 28;
                            prim->x1 = prim->x3 = prim->x0 + width;
                        } else {
                            prim->u0 = prim->u2 += (width << 5) / 28;
                            prim->x0 = prim->x2 += width;
                        }
                    }
                    if (index1) {
                        aspect = g_splashAspects[index1];
                        if (aspect < 0) {
                            if (selfX >= prim->x1) {
                                prim->y2 += (prim->x1 - prim->x0) / aspect;
                            } else {
                                prim->y2 += (selfX - prim->x0) / aspect;
                                prim->y3 -= (prim->x1 - selfX) / aspect;
                            }
                        } else if (prim->x0 >= selfX) {
                            prim->y3 -= (prim->x1 - prim->x0) / aspect;
                        } else {
                            prim->y2 += (selfX - prim->x0) / aspect;
                            prim->y3 -= (prim->x1 - selfX) / aspect;
                        }
                    }
                }
            }
            PGREY(prim, 0) = PGREY(prim, 1) = 0xFF;
            PGREY(prim, 2) = PGREY(prim, 3) = 0x80;
            prim->clut = PAL_CC_MAGIC_HUD_EFFECT;
            prim->tpage = 0x1A;
            prim->priority = self->zPriority + 2;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            if (i & 1) {
                PGREY(prim, 0) = PGREY(prim, 1) = 0x00;
                PGREY(prim, 2) = PGREY(prim, 3) = 0x60;
                prim->clut = PAL_FILL_WHITE;
                prim->priority += 2;
                prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
            }
            prim2 = prim;
        }

        aspect = (self->posX.i.hi - 120) >> 4;
        if (aspect < -8) {
            aspect = -8;
        }
        if (aspect > 8) {
            aspect = 8;
        }

        g_api.PlaySfxVolPan(SFX_WATER_SPLASH_JUMP, 0x7F, aspect);

        self->velocityY = g_SplashYMovement[params * 2];
        self->ext.waterEffects.accelY = g_SplashYMovement[params * 2 + 1];

        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromCurrentEntity(E_WATER_DROP, newEntity);
            newEntity->velocityY = self->velocityY;
        }
        break;

    case 1:
        MoveEntity(self);
        self->velocityY += self->ext.waterEffects.accelY;
        if (self->velocityY > FIX(2.5)) {
            self->step++;
        }
        break;

    case 2:
        MoveEntity(self);
        prim = &g_PrimBuf[self->primIndex];
        if (prim->r0 < 9) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    selfY =
        self->ext.waterEffects.unk82 - self->posY.i.hi - tilemap->scrollY.i.hi;
    for (i = 0, prim = &g_PrimBuf[self->primIndex]; prim != NULL; i++,
        prim = prim->next) {
        prim->y0 = prim->y2 - selfY;
        prim->y1 = prim->y3 - selfY;
        if (i & 1) {
            if (prim->b3 >= 4) {
                prim->b3 -= 4;
            }
            PGREY(prim, 2) = PGREY(prim, 3);
        } else {
            if (prim->b3 >= 8) {
                prim->b3 -= 4;
            }
            PGREY(prim, 2) = PGREY(prim, 3);
            if (prim->b3 <= 8) {
                if (prim->b1 >= 8) {
                    prim->b1 -= 8;
                }
                PGREY(prim, 0) = PGREY(prim, 1);
            }
        }
    }
}

// twin of EntityWaterDrop (src/st/water_effects.h)
void EntityWaterDrop(Entity* self) {
    s16 x = self->posX.i.hi;
    s16 y = self->posY.i.hi;
    FakePrim* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 0x21);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        self->ext.timer.t = 0x2F;
        prim = (FakePrim*)&g_PrimBuf[primIndex];

        while (1) {
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->priority = self->zPriority + 2;

            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                break;
            }

            prim->posX.i.lo = prim->posY.i.lo = 0;
            prim->velocityY.val = (rand() & PSP_RANDMASK) * 8 + self->velocityY;
            prim->posY.i.hi = y + (rand() & 15);
            prim->posX.i.hi = x + (rand() & 31) - 16;
            prim->delay = (rand() & 15) + 32;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim->r0 = 255;
            prim->g0 = 255;
            prim->b0 = 255;
            prim->w = 2;
            prim->h = 2;
            prim = prim->next;
        }
        break;

    case 1:
        if (--self->ext.timer.t == 0) {
            DestroyEntity(self);
            return;
        }

        prim = (FakePrim*)&g_PrimBuf[self->primIndex];

        while (1) {
            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                return;
            }
            prim->posX.i.hi = prim->x0;
            prim->posY.i.hi = prim->y0;
            if (!--prim->delay) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim->posY.val += prim->velocityY.val;
            if (prim->velocityY.val > FIX(0.5)) {
                prim->r0 -= 4;
                prim->g0 -= 4;
                prim->b0 -= 4;
            } else {
                prim->velocityY.val += FIX(28.0 / 128);
            }
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim = prim->next;
        }
        break;
    }
}

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A2A90);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A2AEC);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A365C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A3CD8);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A3EE0);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A42A8);

INCLUDE_RODATA("boss/bo3/nonmatchings/unk_1CEEC", D_us_8019CA94);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A4680);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A4988);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A4C0C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A4E24);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A4FB8);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A516C);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A51E4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A5338);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A57A4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801A5948);
