// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

extern EInit g_EInitParticle;
extern EInit g_EInitInteractable;
extern EInit g_EInitCommon;

void func_us_801C123C_from_no4(Entity* self) {
    u32 pad[10];
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->animSet = ANIMSET_OVL(9);
        self->unk5A = 0x5B;
        self->palette = 0x25D;
        self->animCurFrame = 0x15;
        self->zPriority = 0x6A;
        self->step = 0x100;
        break;
    }
}

extern u8 D_us_80181070[];

extern u8 D_us_80181088[];

extern s16 D_us_80181094[];

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
        InitializeEntity(g_EInitInteractable);
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
            prim->priority = 0x9E;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        break;
    }

    prim = self->ext.et_801C12B0.prim;
    AnimateEntity(D_us_80181088, self);
    i = (self->params >> 8) & 0xFF;
    ptr = &D_us_80181094[(self->params & 0xFF) * 5];
    scrollX = g_Tilemap.scrollX.i.hi - 0x10;
    scrollY = g_Tilemap.scrollY.i.hi;
    xOffset = scrollX + 0x120;
    var_s5 = &D_us_80181070[self->animCurFrame * 4];
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
            // This fixes a bunch of registers on PSP
            // It should mean the else below is not necessary but
            // removing it reintroduces the issue.
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


INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C15F8_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C5364);


INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C1C94_from_no4);


INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C1EE4_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C5C78);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C5EE4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C2850_from_no4);

void func_us_801C2B78_from_no4(Entity* self) {
    Primitive* prim;
    s32 i;
    s32 var_s2;
    s32 var_s3;
    s32 scrollX;
    s32 primIndex;
    s32 scrollY;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_TILE, 128);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C12B0.prim = prim;
        while (prim != NULL) {
            prim->priority = 0x6C;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }

    prim = self->ext.et_801C12B0.prim;
    scrollX = g_Tilemap.scrollX.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;
    self->ext.et_801C12B0.unk80++;
    if (scrollY < 0x130) {
        var_s3 = 0x50;
        var_s3 -= scrollY;
        for (i = 0; i < 4; i++) {
            while (prim != NULL) {
                if (prim->drawMode == DRAW_HIDE) {
                    prim->r0 = prim->b0 = prim->g0 = 0x80;
                    prim->x0 = ((((rand() % 11) * 4) + 0x115) - scrollX) + i;
                    prim->y0 = var_s3 - 4 + (rand() & 7);
                    prim->x1 = 0;
                    LOH(prim->r1) = 0;
                    prim->u0 = prim->v0 = 2;
                    LOW(prim->r2) = FIX(4);
                    LOW(prim->u1) = 0;
                    LOW(prim->x3) = (0x4000 - (rand() & 0xFF0));
                    LOW(prim->r3) = ((rand() & 0x1F00) - 0xF80);
                    prim->clut = 0x20;
                    prim->drawMode = DRAW_UNK02;
                    break;
                }

                prim = prim->next;
            }
        }
    }

    // This is probably an alternate Primitive
    prim = self->ext.et_801C12B0.prim;
    while (prim != NULL) {
        if (prim->drawMode != DRAW_HIDE) {
            LOH(prim->b1) = prim->x0;
            prim->y1 = prim->y0;
            LOW(prim->x1) -= LOW(prim->r2);
            LOW(prim->r1) += LOW(prim->u1);
            prim->x0 = LOH(prim->b1);
            prim->y0 = prim->y1;

            var_s2 = 0;
            if (LOW(prim->r2) < 0) {

            } else {
                var_s2 = 1;
            }
            LOW(prim->r2) -= LOW(prim->x3);
            LOW(prim->u1) -= LOW(prim->r3);

            if (!--prim->clut) {
                prim->drawMode = DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}

extern u16 D_us_80181478[];

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C2E60_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C3160_from_no4);

extern s16 D_us_80181494[][5];

void func_us_801C34EC_from_no4(Entity* self) {
    s32 scrollX;
    s16 xOffset;
    s16 randX;
    s16 randY;
    s16 tpage;
    s32 yOffset;
    s32 scrollY;
    s32 primIndex;
    s16* ptr;
    Primitive* prim;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_TILE, 0x40);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C12B0.prim = prim;
        while (prim != NULL) {
            prim->priority = 0x9B;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }

    ptr = D_us_80181494[self->params];
    xOffset = *ptr++;
    randX = *ptr++;
    yOffset = *ptr++;
    randY = *ptr++;
    tpage = *ptr;
    prim = self->ext.et_801C12B0.prim;
    scrollX = g_Tilemap.scrollX.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;
    self->ext.et_801C12B0.unk80++;
    if (scrollY < 0xF0) {
        yOffset -= scrollY;

        while (prim != NULL) {
            if (prim->drawMode == DRAW_HIDE) {
                prim->r0 = prim->b0 = prim->g0 = 0x80;
                prim->y0 = yOffset + (rand() % randY);
                prim->x0 = (xOffset - scrollX) + (rand() % randX);
                prim->x1 = 0;
                LOH(prim->r1) = 0;
                prim->u0 = prim->v0 = 2;
                LOW(prim->u1) = 0;
                prim->tpage = tpage;
                prim->drawMode = DRAW_UNK02;
                break;
            }

            prim = prim->next;
        }
    }

    // This is probably an alternate Primitive
    prim = self->ext.et_801C12B0.prim;
    while (prim != NULL) {
        if (prim->drawMode != DRAW_HIDE) {
            LOH(prim->b1) = prim->x0;
            LOW(prim->r1) += LOW(prim->u1);
            prim->x0 = LOH(prim->b1);
            prim->r0 -= 8;
            prim->b0 = prim->g0 = prim->r0;
            if (prim->r0 < 8) {
                prim->drawMode = DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C37C8_from_no4);

void func_us_801C3A04_from_no4(Entity* self) {
    Primitive* prim;
    s32 i;
    s32 var_s2;
    s32 yOffset;
    s32 posX;
    s32 primIndex;
    s32 scrollY;

    if (!self->step) {
        InitializeEntity(g_EInitCommon);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_TILE, 0x40);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C12B0.prim = prim;

        while (prim != NULL) {
            prim->priority = 0x6C;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }

    prim = self->ext.et_801C12B0.prim;
    posX = self->posX.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;
    self->ext.et_801C12B0.unk80++;
    yOffset = 0xE0;
    yOffset -= scrollY;

    // This is probably an alternate Primitive
    for (i = 0; i < 2; i++) {
        while (prim != NULL) {
            if (prim->drawMode == DRAW_HIDE) {
                prim->r0 = prim->b0 = prim->g0 = 0x80;
                prim->x0 = ((rand() % 0x10) * 2) + i + posX;
                prim->y0 = yOffset - 4 + (rand() & 7);
                prim->x1 = 0;
                LOH(prim->r1) = 0;
                prim->u0 = prim->v0 = 2;
                LOW(prim->r2) = 0x40000;
                LOW(prim->u1) = 0;
                LOW(prim->x3) = 0x4000 - (rand() & 0xFF0);
                LOW(prim->r3) = (rand() & 0x1F00) - 0xF80;
                prim->clut = 0x20;
                prim->drawMode = DRAW_UNK02;
                break;
            }

            prim = prim->next;
        }
    }

    // This is probably an alternate Primitive
    prim = self->ext.et_801C12B0.prim;
    while (prim != NULL) {
        if (prim->drawMode != DRAW_HIDE) {
            LOH(prim->b1) = prim->x0;
            prim->y1 = prim->y0;
            LOW(prim->x1) -= LOW(prim->r2);
            LOW(prim->r1) += LOW(prim->u1);
            prim->x0 = LOH(prim->b1);
            prim->y0 = prim->y1;

            var_s2 = 0;
            if (LOW(prim->r2) >= 0) {
                var_s2 = 1;
            }
            LOW(prim->r2) -= LOW(prim->x3);
            LOW(prim->u1) -= LOW(prim->r3);
            if (!--prim->clut) {
                prim->drawMode = DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}

extern u16 D_us_801814A8[];

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C3CC4_from_no4);


INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C3FB0_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C4228_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityWaterBox);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C81C8);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityFloatingIcePlatform);

extern s16 D_us_801814E8[];
extern bool D_us_8018104C;

void func_us_801C4BD8_from_no4(Entity* self) {
    Entity* player;
    s16* dataPtr;
    s32 volume;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
    }

    player = &PLAYER;
    dataPtr = &D_us_801814E8[self->params * 4];

    volume = player->posX.i.hi + g_Tilemap.scrollX.i.hi - *dataPtr++;
    volume = (volume * *dataPtr++) / 0x1000;
    volume += *dataPtr++;

    if (volume < 0) {
        volume = 0;
    } else if (volume > 0x7F) {
        volume = 0x7F;
    }

    if (!volume) {
        if (D_us_8018104C) {
            D_us_8018104C = false;
            g_api.PlaySfx(SET_UNK_A6);
            return;
        }
    }
    if (D_us_8018104C) {
        g_api.SetVolumeCommand22_23(volume, *dataPtr++);
        return;
    }

    g_api.PlaySfxVolPan(SFX_WATERFALL_LOOP, volume, *dataPtr++);
    D_us_8018104C = true;
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C8668);

void RNO4_Unused801C8704(void) {}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C870C);

void RNO4_Unused801C8768(void) {}

void RNO4_Unused801C8770(void) {}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityBoatElevatorChains);

void RNO4_Unused801C8BD4(void) {}

void RNO4_Unused801C8BDC(void) {}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", LoadFerrymanGateTiles);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C8C54);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801A071C_from_bo3);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801A07CC_from_bo3);

extern s16 D_us_801815F8[];

void func_us_801C5518_from_no4(Entity* self) {
    Entity* player;
    u16 diff;
    s16* dataPtr;

    player = &PLAYER;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
    }

    dataPtr = &D_us_801815F8[self->params * 4];

    diff = player->posX.i.hi + g_Tilemap.scrollX.i.hi - *dataPtr++;
    if (diff > *dataPtr++) {
        return;
    }
    diff = player->posY.i.hi + g_Tilemap.scrollY.i.hi - *dataPtr++;
    if (diff > *dataPtr++) {
        return;
    }
    if (player->velocityY < 0) {
        player->velocityY *= 7;
        player->velocityY /= 8;
    } else if (player->velocityY > 0) {
        player->nFramesInvincibility = 1;
    }
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C9048);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C909C);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityExplosionVariants);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityGreyPuff);

// params: (& 0xF0) Use an alternate set of hardcoded palette and drawMode
//         (& 0xFF00) if non-zero, uses ((& 0xFF00) >> 8) as the zPriority
void EntityIntenseExplosion(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitParticle);
        self->palette = PAL_FLAG(PAL_UNK_170);
        self->animSet = ANIMSET_DRA(5);
        self->animCurFrame = 1;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(PAL_UNK_195);
            self->blendMode = BLEND_TRANSP;
        }

        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        self->zPriority += 8;
    } else {
        self->poseTimer++;
        self->posY.val -= FIX(0.25);
        if ((self->poseTimer % 2) == 0) {
            self->animCurFrame++;
        }

        if (self->poseTimer > 36) {
            DestroyEntity(self);
        }
    }
}

void PlaySfxPositional(s16 sfxId) {
    s32 posX, posY;
    s16 sfxPan;
    s16 sfxVol;

    posX = g_CurrentEntity->posX.i.hi - 128;
    sfxPan = (abs(posX) - 32) >> 5;
    if (sfxPan > 8) {
        sfxPan = 8;
    } else if (sfxPan < 0) {
        sfxPan = 0;
    }
    if (posX < 0) {
        sfxPan = -sfxPan;
    }
    sfxVol = abs(posX) - 96;
    posY = abs(g_CurrentEntity->posY.i.hi - 128) - 112;
    if (posY > 0) {
        sfxVol += posY;
    }
    if (sfxVol < 0) {
        sfxVol = 0;
    }
    sfxVol = 127 - (sfxVol >> 1);
    if (sfxVol > 0) {
        g_api.PlaySfxVolPan(sfxId, sfxVol, sfxPan);
    }
}
