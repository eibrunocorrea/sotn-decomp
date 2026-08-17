// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo3.h"

extern s32 D_us_80180C80[]; // g_SplashYMovement (velocityY/accelY pairs)
extern s16 D_us_80180D38[]; // g_splashAspects
#define g_SplashYMovement D_us_80180C80
#define g_splashAspects D_us_80180D38


extern EInit g_EInitCommon;

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C123C_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C12B0_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C15F8_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", func_us_801C1844_from_no4);

INCLUDE_ASM("boss/bo3/nonmatchings/unk_1CEEC", EntityBgColumnsParallax_from_no4);

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
