// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

#ifndef VERSION_PC
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

extern u8 D_us_80181084[2];
extern s16 D_us_80181280[50]; // unknown size?

void func_us_801C5364(Entity* self) {
    Primitive* prim;
    u32 subTileX;
    int bgColorValue;
    int tileY1Wrap;
    s32 tileY1;
    s32 tileX0;
    s32 tileX1;
    u8* tilePos;
    s32 tileX1Wrap;
    s32 primIndex;
    s32 x0;
    long y0;
    s32 tileY0;
    s32 x1;
    s32 y1;
    s32 i;
    s16* tileDimensions; // It holds some dimensions of something, I'm guessing
                         // some tile or entity sprite

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        self->ext.et_801C5364.unk80 = 4;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x10);
        if (primIndex == (-1)) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C5364.prim7C = prim;
        while (prim) {
            prim->tpage = 0xF;
            prim->clut = 0x85;
            prim->priority = 0x18;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }
    if (g_Timer & 0x80) {
        if (g_Timer & 0x40) {
            bgColorValue = 0x3f - (g_Timer & 0x3f);
        } else {
            bgColorValue = g_Timer & 0x3f;
        }
    } else {
        bgColorValue = 0;
    }
    bgColorValue = bgColorValue + 0x80;
    bgColorValue = 0x80;

    g_GpuBuffers[0].draw.r0 = ((bgColorValue * 24) / 0x80) & 0xf8;
    g_GpuBuffers[0].draw.g0 = 0;
    g_GpuBuffers[0].draw.b0 = ((bgColorValue * 8) / 128) & 0xf8;
    g_GpuBuffers[1].draw.r0 = ((bgColorValue * 24) / 128) & 0xf8;
    g_GpuBuffers[1].draw.g0 = 0;
    g_GpuBuffers[1].draw.b0 = ((bgColorValue * 8) / 0x80) & 0xf8;

    prim = self->ext.et_801C5364.prim7C;

    i = (self->params >> 8) & 0xff;
    tileDimensions = D_us_80181280 + ((self->params & 0xff) * 4);
    x0 = g_Tilemap.scrollX.i.hi - 0x10;
    y0 = g_Tilemap.scrollY.i.hi;
    x1 = x0 + 0x120;
    y1 = y0 + 0xe0;
    tilePos = D_us_80181084;
    tileY1Wrap = tilePos[1];
    while (i > 0) {
        tileX0 = *(tileDimensions++);
        tileX1 = *(tileDimensions++);
        if ((x0 >= tileX0) || (x1 < tileX1)) {
            tileDimensions = tileDimensions + 2;
        } else {
            tileY0 = *(tileDimensions++);
            tileY1 = *(tileDimensions++);
            if ((y0 < tileY0) && (!(y1 < tileY1))) {
                {
                    if (x0 > tileX1) {
                        tileX1 = x0;
                    }
                }
                if (tileX0 > x1) {
                    tileX0 = x1;
                }
                tileX0 -= tileX1;
                tileX1 -= (x0 + 0x10);
                subTileX = ((x0 / 4) + tileX1) % 126;
                if (tileY1 < y0) {
                    tileY1 = y0;
                }
                if (y1 < tileY0) {
                    tileY0 = y1;
                }
                tileY1Wrap = 0x63 - ((y0 - (g_Tilemap.height - 0x100)) / 4);
                y1 = y1 + 0x46;
                tileY1 = tileY1 - y0;
                tileY0 -= y0;
                if ((tileY1Wrap < tileY0) && (y1 >= tileY1)) {
                    if (tileY1 < tileY1Wrap) {
                        tileY1 = tileY1Wrap;
                    }
                    if (y1 < tileY0) {
                        tileY0 = y1;
                    }
                    tileY0 -= tileY1;
                    tileY1Wrap = ((y0 - (g_Tilemap.height - 0x100)) / 4) +
                                 (tileY1 - 0x63);
                    subTileX += tilePos[0];
                    do {
                        prim->u0 = prim->u2 = subTileX;
                        tileX1Wrap = 0x7e - (subTileX - tilePos[0]);
                        if (tileX0 < tileX1Wrap) {
                            tileX1Wrap = tileX0;
                        }
                        prim->u1 = prim->u3 = subTileX + tileX1Wrap;
                        prim->x0 = prim->x2 = tileX1;
                        tileX1 = tileX1 + tileX1Wrap;

                        prim->x1 = (prim->x3 = tileX1);
                        tileX0 = tileX0 - tileX1Wrap;
                        subTileX = tilePos[0];

                        // There's this empty statement after all,
                        // I missed it when I thought I'd found the solution
                        // I had permuter running it for ~20 hours and couldn't
                        // find anything else
                        tilePos = tilePos;

                        if (tileY1Wrap > 0x46) {
                            tileX1Wrap = 0;
                        } else {
                            tileX1Wrap = 0x46 - tileY1Wrap;
                        }
                        prim->v0 = prim->v1 = tilePos[1] + tileX1Wrap;
                        prim->v2 = prim->v3 = tilePos[1];
                        prim->y0 = prim->y1 = tileY1;
                        prim->y2 = prim->y3 = tileY1 + tileX1Wrap;
                        prim->drawMode = DRAW_COLORS;
                        prim->r0 = prim->r1 = prim->r2 = prim->r3 = prim->b0 =
                            prim->b1 = prim->b2 = prim->b3 = bgColorValue;
                        prim->g0 = prim->g1 = prim->g2 = prim->g3 = 0x80;
                        prim = prim->next;
                    } while (tileX0);
                }
            }
        }
        i -= 1;
    }

    while (prim) {
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", EntityBgColumnsParallax_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C1EE4_from_no4);

extern Tilemap* D_pspeu_0929B8B8; // This variable is assigned to a different
                                  // var that is never used

void func_us_801C5C78(Entity* self) {
    Primitive* prim;
    Tilemap** tilemap_pp_0;
    u32 primIndex;
    u32 xOffset;
    u16 params;
    u32 clut;
    s32 scrollY;
    s32 posY;
    u32 scrollYModulo;

    params = self->params;
    if (!self->step) {
        InitializeEntity(g_EInitParticle);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (primIndex == (-1)) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA |
                       FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA | FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801C5C78.prim = prim;
        while (prim) {
            if (params) {
                prim->tpage = 0xF;
                prim->u0 = (prim->u2 = 0x82);
                prim->u1 = (prim->u3 = 0x9d);
            } else {
                prim->tpage = 0xE;
                prim->u0 = (prim->u2 = 0xe9);
                prim->u1 = (prim->u3 = 0xf7);
            }
            prim->priority = 0x62;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }

    prim = self->ext.et_801C5C78.prim;
    tilemap_pp_0 = &D_pspeu_0929B8B8;
    self->ext.et_801C5C78.unk84 += 1;

    if (self->ext.et_801C5C78.unk84 >= 0xE) {
        self->ext.et_801C5C78.unk84 = 0;
    }

    posY = self->posY.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;

    if (posY >= 0) {
        scrollY %= 32;
        if (params) {
            clut = 0x90;
        } else {
            clut = 0xB0;
        }
        clut = 0xB0;
        clut += self->ext.et_801C5C78.unk84;

        if (((scrollY + posY) - 0x50) > 0x60) {
            scrollYModulo = 0x60 - scrollY;
        }
        scrollYModulo = 0x60;

        prim->clut = clut;

        if (params) {
            xOffset = self->posX.i.hi - 0xD;
            prim->v2 = (prim->v3 = 3);
            prim->x1 = (prim->x3 = xOffset + 0x1B);
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
        } else {
            xOffset = self->posX.i.hi - 7;
            prim->v2 = (prim->v3 = 0x83);
            prim->x1 = (prim->x3 = xOffset + 0xe);
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
        }

        prim->v0 = (prim->v1 = prim->v2 + 0x60);
        prim->x0 = (prim->x2 = xOffset);
        prim->y0 = (prim->y1 = 0x4C);
        prim->y2 = (prim->y3 = 0xAC);
        prim = prim->next;
        prim->clut = clut;

        if (params) {
            prim->v0 = (prim->v1 = 0x63);
            prim->v2 = (prim->v3 = 0x2f);
            prim->x0 = (prim->x2 = xOffset);
            prim->x1 = (prim->x3 = xOffset + 0x1B);
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
        } else {
            prim->v0 = (prim->v1 = 0xe3);
            prim->v2 = (prim->v3 = 0xaf);
            prim->x0 = (prim->x2 = xOffset);
            prim->x1 = (prim->x3 = xOffset + 0xe);
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
        }

        prim->y0 = (prim->y1 = 0xAC);
        prim->y2 = (prim->y3 = 0xE0);
        prim = prim->next;
    }
}

void func_us_801C5EE4(Entity* self) {
    Primitive* prim;
    Entity* player;
    s32 scrollX;
    s32 clut;
    s32 upperBound;
    int playerPosRelTop;
    int playerPos;
    s32 scrollY;
    s32 scrollYWrap;
    s32 primIndex;

    player = g_Entities;
    scrollX = g_Tilemap.scrollX.i.hi;

    scrollY = g_Tilemap.scrollY.i.hi;
    scrollYWrap = ((0x500 - scrollY) / 24) + 0x4D;
    if (scrollYWrap > 0x7F) {
        scrollYWrap = 0x7F;
    }
    playerPos = (scrollX + player->posX.i.hi) - 0x128;
    if (playerPos > 0) {
        if (playerPos < 32) {
            playerPos = 0;
        } else {
            { playerPos = (playerPos - 32) / (-16); }
        }
        if (playerPos < (-8)) {
            playerPos = -8;
        }
    } else {
        if (playerPos > (-32)) {
            playerPos = 0;
        } else {
            playerPos = (playerPos + 32) / (-24);
        }
        if (playerPos > 8) {
            playerPos = 8;
        }
    }
    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 4);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.et_801C5EE4.unk84 = 0;
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = (&g_PrimBuf[primIndex]);
        self->ext.et_801C5EE4.prim7C = prim;

        while (prim) {
            prim->priority = 0x9C;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
    }
    g_api.SetVolumeCommand22_23(scrollYWrap, playerPos);
    self->ext.et_801C5EE4.unk84 += 1;
    if (self->ext.et_801C5EE4.unk84 >= 0xE) {
        self->ext.et_801C5EE4.unk84 = 0;
    }
    clut = self->ext.et_801C5EE4.unk84 + 0xa0;
    prim = self->ext.et_801C5EE4.prim7C;
    playerPos = 0x50;
    upperBound = 0x550;
    upperBound = upperBound - scrollY;
    playerPos = playerPos - scrollY;
    if (playerPos < 0) {
        playerPos = 0;
    }
    if (upperBound > 0xF0) {
        upperBound = 0xF0;
    }
    playerPosRelTop = upperBound - playerPos;
    while (prim) {
        upperBound = playerPos + scrollY;
        {
            if (upperBound >= 0x452) {
                prim->tpage = 0xE;
                prim->u0 = (prim->u2 = 0x87);
                prim->u1 = (prim->u3 = 0xe7);
                prim->v2 = (prim->v3 = 0x550 - upperBound);
                prim->v0 = (prim->v1 = 0);
                scrollYWrap = 0x550 - upperBound;
                prim->x0 = (prim->x2 = 0x140 - scrollX);
                prim->x1 = (prim->x3 = 0xE0 - scrollX);
            } else {
                prim->tpage = 0xF;

                scrollYWrap = (0x452 - (playerPos + scrollY)) % 126;
                if (!scrollYWrap) {
                    scrollYWrap = 0x7E;
                }
                prim->u0 = (prim->u2 = 0xCC);
                prim->u1 = (prim->u3 = 0xFD);
                prim->v2 = (prim->v3 = scrollYWrap);
                if ((playerPos + 0x7E) < 0xF0) {
                    prim->v0 = (prim->v1 = 0);
                } else {
                    prim->v0 = (prim->v1 = playerPos - 0x72);
                };
                scrollYWrap = prim->v2 - prim->v0;
                if ((upperBound + scrollYWrap) > 0x452) {
                    scrollYWrap = upperBound - 0x452;
                    prim->v0 = (prim->v1 = prim->v2 - scrollYWrap);
                };
                prim->x0 = (prim->x2 = 0x140 - scrollX);
                prim->x1 = (prim->x3 = 0x10F - scrollX);
            }

            prim->y2 = (prim->y3 = playerPos);
            playerPos += scrollYWrap;
            playerPosRelTop -= scrollYWrap;
            prim->y0 = prim->y1 = playerPos;
#if defined(VERSION_PSP)
            prim->y2 -= 1;
            prim->y3 -= 1;
#endif
            prim->clut = clut;
            prim->drawMode = DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
        }
        prim = prim->next;
        if (playerPosRelTop <= 0) {
            break;
        }
    }

    while (prim) {
        prim->drawMode = DRAW_HIDE;
        prim = prim->next;
    }
}

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

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C3CC4_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C3FB0_from_no4);

INCLUDE_ASM("st/rno4/nonmatchings/unk_44B0C", func_us_801C4228_from_no4);

void EntityWaterBox(Entity* self) {
    Entity* player;
    u16 collision;
    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = ANIMSET_OVL(1);
        self->animCurFrame = 6;
        if (g_CastleFlags[NO4_WATER_BLOCKED]) {
            self->posX.i.hi = 0x720 - g_Tilemap.scrollX.i.hi;
        } else {
            self->posX.i.hi = 0x760 - g_Tilemap.scrollX.i.hi;
        }
    }

    player = &PLAYER;
    collision = GetPlayerCollisionWith(self, 0x10, 0x11, 5);
    if ((collision & 1) && (g_Player.vram_flag & TOUCHING_GROUND)) {
        if (self->posX.i.hi > player->posX.i.hi) {
            if ((g_pads[0].pressed & PAD_RIGHT) && (PLAYER.step == 1)) {
                if (self->ext.timer.t) {
                    self->ext.timer.t--;
                    return;
                }
                if (self->posX.i.hi + g_Tilemap.scrollX.i.hi < 0x7A0) {
                    self->posX.i.hi++;
                    player->posX.i.hi++;
                }
                self->ext.timer.t = 3;
            }
        } else {
            if ((g_pads[0].pressed & PAD_LEFT) && (PLAYER.step == 1)) {
                if (self->ext.timer.t) {
                    self->ext.timer.t--;
                    return;
                }
                if (self->posX.i.hi + g_Tilemap.scrollX.i.hi > 0x720) {
                    self->posX.i.hi--;
                    player->posX.i.hi--;
                    if (self->posX.i.hi + g_Tilemap.scrollX.i.hi == 0x720) {
                        g_CastleFlags[NO4_WATER_BLOCKED] = 1;
                    }
                }
                self->ext.timer.t = 3;
            }
        }
    }
}

#ifdef VERSION_PSP
extern s32 E_ID(UNK_26);
extern s32 E_ID(UNK_27);
#endif
extern s16 D_us_801814D4;

void func_us_801C81C8(Entity* self) {
    Entity* child;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = -0x7FFE;
        self->palette = 0x44;
        self->drawFlags = ENTITY_MASK_R;
        self->posX.i.hi = (0x1EF - g_Tilemap.scrollX.i.hi);
        child = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (child) {
            CreateEntityFromCurrentEntity(E_ID(UNK_27), child);
            child->params = 1;
        }
        self->ext.et_801C81C8.unk80 = child;
        child = AllocEntity(child, &g_Entities[256]);
        if (child) {
            CreateEntityFromCurrentEntity(E_ID(UNK_26), child);
            child->params = 1;
        }
        self->ext.et_801C81C8.unk84 = child;
        self->ext.et_801C81C8.unk7C = 0;
    }
    AnimateEntity(&D_us_801814D4, self);
}

void EntityFloatingIcePlatform(Entity* self) {
    extern u16 g_FloatingIcePlatformHitbox[];
    u16* hitboxPtr;
    u16 collision;
    Entity* player;
    s16 prevPosY;
    s16 dx, dy;
    u16 hitboxIndex;

    player = &PLAYER;
    hitboxIndex = self->params;

    if (!self->step) {
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(1);
        self->animCurFrame = hitboxIndex + 25;
        self->drawFlags = ENTITY_ROTATE;
        self->ext.floatingIcePlatform.baseY =
            self->posY.i.hi + g_Tilemap.scrollY.i.hi;
    }

    hitboxPtr = &g_FloatingIcePlatformHitbox[hitboxIndex * 2];

    prevPosY = self->posY.i.hi;
    self->posY.i.hi =
        self->ext.floatingIcePlatform.baseY - g_Tilemap.scrollY.i.hi +
        self->ext.floatingIcePlatform.bobOffset;
#ifdef VERSION_PSP
    collision = GetPlayerCollisionWith(self, hitboxPtr[0], hitboxPtr[1], 4);
#else
    collision = GetPlayerCollisionWith(self, *hitboxPtr++, *hitboxPtr, 4);
#endif
    self->posY.i.hi = prevPosY;
    self->ext.floatingIcePlatform.previousBobOffset =
        self->ext.floatingIcePlatform.bobOffset;

    dx = self->posX.i.hi - player->posX.i.hi;

    if (collision) {
        if (self->ext.floatingIcePlatform.bobOffset < 4) {
            self->ext.floatingIcePlatform.bobOffset++;
        }
    } else {
        if (self->ext.floatingIcePlatform.bobOffset) {
            self->ext.floatingIcePlatform.bobOffset--;
        }
    }

    dy = self->ext.floatingIcePlatform.bobOffset;
    if (dx < 0) {
        prevPosY = (dx * dy * -0x100) / 56;
    } else {
        prevPosY = (dx * dy * 0x100) / 56;
    }

    self->posY.i.hi = self->ext.floatingIcePlatform.baseY -
                      g_Tilemap.scrollY.i.hi + (dy - prevPosY / 256);

    if (collision) {
        dy = dy - self->ext.floatingIcePlatform.previousBobOffset;
        player->posY.i.hi += dy;
        g_unkGraphicsStruct.shoveX.i.hi += dy;
    }

    prevPosY = -prevPosY;
    if (collision || dy) {
        if (dx < 0) {
            self->rotate = ratan2(prevPosY, -0x3800);
            self->rotate = (self->rotate - 0x800) & 0xFFF;
            return;
        }
        self->rotate = ratan2(prevPosY, 0x3800);
    } else {
        self->rotate = 0;
    }
}

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

void func_us_801C8668(Entity* self) {
    s32 i;
    u16* tile;
    Tilemap* tilemap;

    tilemap = &g_Tilemap;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        tile = tilemap->fg + 0x1052;

        for (i = 0; i < 5; ++i) {
            *tile = 0xac7;
            tile += 1;
        }
        *tile = 0x59D;
        tile = tilemap->fg + 0x1062;

        for (i = 0; i < 0xA; ++i) {
            *tile = 0xAC7;
            tile += 1;
        }
    }
}

void RNO4_Unused801C8704(void) {}

void func_us_801C870C(Entity* self) {
    s16 i;
    u16* tilePtr;

    if (self->params == 0) {
        tilePtr = g_Tilemap.fg + 0x143;
    } else {
        tilePtr = g_Tilemap.fg + 0x53;
    }

    for (i = 0; i < 0xA; ++i) {
        tilePtr[0] = 0;
        ++tilePtr;
    }
}

void RNO4_Unused801C8768(void) {}

void RNO4_Unused801C8770(void) {}

// Donor: src/st/no4/e_ferryman.c (matched). All 4 data tables already have
// their own glabels in rno4's asm (confirmed in
// asm/us/st/rno4/nonmatchings/unk_44B0C/EntityBoatElevatorChains.s, matched
// to donor tables by order of first use) -- declared extern rather than
// reproduced as local statics, per the data-glabel rule.
// papéis provados pelo reloc original (48854 lhu de 1508 = índices;
// 48868 lbu de 1528 = bytes) — o donor tem a ordem inversa
extern u16 D_us_80181508[];    // índices (donor: arr_indexes)
extern u8 D_us_80181528[2][4]; // dados (donor: D_us_801816D4)
extern s16 D_us_80181530[];    // donor: D_us_801816DC
extern s16 D_us_80181548[];    // donor: D_us_801816F4

// Controls drawing of the boat elevator chains and rotating gear mechanism
void EntityBoatElevatorChains(Entity* self) {
    u32 primIndex;
    u32 scrollX;

    u32 scrollY;
    s16 cos;
    u8* ptr;
    s32 i;
    s16* ptrTwo;
    s16 sin;
    s16 xOffset;
    s16 yOffset;
    Primitive* prim;

    scrollX = g_Tilemap.scrollX.i.hi;
    scrollY = g_Tilemap.scrollY.i.hi;
    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 13);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];

            i = 0;
            while (prim != NULL) {
                prim->tpage = 0xF;
                prim->clut = 0x5F;
                ptr = *D_us_80181528;
                ptr += D_us_80181508[i] * 4;
                prim->u0 = prim->u2 = *ptr++;
                prim->u1 = prim->u3 = *ptr++;
                prim->v0 = prim->v1 = *ptr++;
                prim->v2 = prim->v3 = *ptr;
                prim->priority = 0x80;
                prim->drawMode = DRAW_HIDE;
                prim = prim->next;
                i++;
            }
            self->rotate = 0x200;
        } else {
            self->step = 0;
            return;
        }
    }

    if (self->ext.boatElevator_child.unk7C) {
        if (self->ext.boatElevator_child.unk7C < 0) {
            self->ext.boatElevator_child.unk7E++;
            self->rotate += 0x10;
        } else {
            self->ext.boatElevator_child.unk7E--;
            self->rotate -= 0x10;
        }
    }
    self->ext.boatElevator_child.unk7E &= 0xF;
    prim = &g_PrimBuf[self->primIndex];
    i = 0;
    while (prim != NULL) {
        if (i < 3) {
            ptrTwo = &D_us_80181530[(self->params * 6) + (i * 2)];
            xOffset = *ptrTwo++ - scrollX;
            yOffset = *ptrTwo - scrollY;
            if (self->params) {
                sin = (rsin(-self->rotate) * 0x1A) >> 0xC;
                cos = (rcos(-self->rotate) * 0x1A) >> 0xC;
            } else {
                sin = (rsin(self->rotate) * 0x1A) >> 0xC;
                cos = (rcos(self->rotate) * 0x1A) >> 0xC;
            }

            prim->x0 = xOffset - cos;
            prim->x1 = xOffset + sin;
            prim->x2 = xOffset - sin;
            prim->x3 = xOffset + cos;
            prim->y0 = yOffset - sin;
            prim->y1 = yOffset - cos;
            prim->y2 = yOffset + cos;
            prim->y3 = yOffset + sin;
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        } else {
            ptrTwo = &D_us_80181548[(self->params * 3) * 10 + ((i - 3) * 3)];
            sin = *ptrTwo++;
            xOffset = *ptrTwo++ - scrollX;
            yOffset = *ptrTwo - scrollY;
            switch (sin) {
            case 0:
                prim->x0 = prim->x2 = xOffset - 4;
                prim->x1 = prim->x3 = xOffset + 4;
                yOffset += self->ext.boatElevator_child.unk7E;
                prim->y0 = prim->y1 = yOffset;
                prim->y2 = prim->y3 = yOffset + 0x60;
                break;
            case 1:
                prim->x0 = prim->x2 = xOffset - 4;
                prim->x1 = prim->x3 = xOffset + 4;
                yOffset -= self->ext.boatElevator_child.unk7E;
                prim->y0 = prim->y1 = yOffset;
                prim->y2 = prim->y3 = yOffset + 0x60;
                break;
            case 2:
                xOffset -= self->ext.boatElevator_child.unk7E;
                prim->x0 = prim->x1 = xOffset;
                prim->x2 = prim->x3 = xOffset + 0x60;
                prim->y1 = prim->y3 = yOffset - 4;
                prim->y0 = prim->y2 = yOffset + 4;
                break;
            case 3:
                xOffset += self->ext.boatElevator_child.unk7E;
                prim->x0 = prim->x1 = xOffset;
                prim->x2 = prim->x3 = xOffset + 0x60;
                prim->y1 = prim->y3 = yOffset - 4;
                prim->y0 = prim->y2 = yOffset + 4;
                break;
            }
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        }
        i++;
    }
}

void RNO4_Unused801C8BD4(void) {}

void RNO4_Unused801C8BDC(void) {}

// Donor: src/st/no4/e_ferryman.c (matched). Table already has its own
// glabel in rno4's asm (D_us_801815C0, confirmed in
// asm/us/st/rno4/nonmatchings/unk_44B0C/LoadFerrymanGateTiles.s).
extern u16 D_us_801815C0[7][2];

void LoadFerrymanGateTiles(void) {
    u16* tileLayoutPtr;
    Tilemap* tileMap;
    s32 i;
    s16 offset;

    tileMap = &g_Tilemap;
    // rno4 fix: donor (no4) start offset is 0x595; rno4's mirrored tilemap
    // position is 0xF89 (confirmed via strict-fn-diff).
    offset = 0xF89;
    tileLayoutPtr = *D_us_801815C0;

    for (i = 0; i < LEN(D_us_801815C0); i++) {
        tileMap->fg[offset] = *tileLayoutPtr++;
        offset++;
        tileMap->fg[offset] = *tileLayoutPtr++;
        offset += 0xCF;
    }
}

void func_us_801C8C54(Entity* self) {
    LoadFerrymanGateTiles();
    DestroyEntity(self);
}

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

extern u16 D_us_80181610[];

static void func_us_801C9048(s32 arg0) {
    s16 tilePos;
    s32 i;
    u16* tileLayoutPtr;

    Tilemap* tilemap;
    tilemap = &g_Tilemap;

    tilePos = 0x1E0;
    tileLayoutPtr = &(D_us_80181610[arg0 * 4]);

    for (i = 0; i < 4; ++i) {
        tilemap->fg[tilePos] = *(tileLayoutPtr++);
        tilePos += 0x50;
    }
}

extern s16 D_us_801815DC;

void func_us_801C909C(Entity* self) {
    s16 screen_pos;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(1);
        self->animCurFrame = 0x25;
        if (D_us_801815DC) {
            func_us_801C9048(1);
            self->step = 0x4;
            break;
        }
        func_us_801C9048(0);
        self->posY.i.hi -= 0x40;
        break;

    case 1:
        if (D_us_801815DC) {
            self->step += 1;
            self->velocityY = FIX(4.0);
        }

        break;

    case 2:
        self->posX.i.hi -= 8;
        GetPlayerCollisionWith(self, 0x10, 0x20, 9);
        self->posX.i.hi += 8;
        MoveEntity();
        screen_pos = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
        if (screen_pos >= 0x80) {
            g_api.PlaySfxVolPan(SFX_EXPLODE_B, 0x7F, -6);
            func_us_801C9048(1);
            self->velocityY = FIX(-1.0);
            self->step += 1;
        }
        break;

    case 3:
        self->velocityY += FIX(0.125);
        MoveEntity();
        screen_pos = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
        if (screen_pos >= 0x80) {
            self->posY.i.hi = 0x80 - g_Tilemap.scrollY.i.hi;
            self->step = 6;
        }
        break;

    case 4:
        if (!D_us_801815DC) {
            self->step += 1;
            self->velocityY = FIX(-1.0);
        }
        break;

    case 5:
        self->posX.i.hi -= 8;
        GetPlayerCollisionWith(self, 0x10, 0x20, 0xB);
        self->posX.i.hi += 8;
        MoveEntity();
        screen_pos = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
        if (screen_pos < 0x41) {
            func_us_801C9048(0);
            self->step += 1;
        }
        break;

    case 6:
        break;
    }
}

extern s32 g_ExplosionVariantVelocity[];
extern u8 g_ExplosionVariantFrames[];
extern u16 g_ExplosionVariantDurations[];

void EntityExplosionVariants(Entity* self) {
    if (!self->step) {
        self->velocityY =
            g_ExplosionVariantVelocity[self->ext.destructAnim.index];
        self->flags =
            FLAG_UNK_2000 | FLAG_KEEP_ALIVE_OFFCAMERA | FLAG_POS_CAMERA_LOCKED;
        self->palette = PAL_FLAG(PAL_UNK_195);
        self->animSet = ANIMSET_DRA(2);
        self->animCurFrame = g_ExplosionVariantFrames[self->params];
        self->blendMode = BLEND_TRANSP;
        self->step++;
        return;
    }

    self->posY.val -= self->velocityY;
    ++self->poseTimer;
    if ((self->poseTimer % 2) == 0) {
        self->animCurFrame++;
    }
    if (self->poseTimer > g_ExplosionVariantDurations[self->params]) {
        DestroyEntity(self);
    }
}

void EntityGreyPuff(Entity* self) {
    extern s16 g_GreyPuffScale[];
    extern s32 g_GreyPuffVelocity[];

    if (!self->step) {
        self->flags =
            FLAG_UNK_2000 | FLAG_KEEP_ALIVE_OFFCAMERA | FLAG_POS_CAMERA_LOCKED;
        self->palette = PAL_FLAG(PAL_UNK_195);
        self->animSet = ANIMSET_DRA(5);
        self->animCurFrame = 1;
        self->blendMode = BLEND_TRANSP;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = g_GreyPuffScale[self->params];
        self->scaleY = self->scaleX;
        self->velocityY = g_GreyPuffVelocity[self->params];
        self->step++;
    } else {
        self->posY.val -= self->velocityY;
        self->poseTimer++;
        if ((self->poseTimer % 2) == 0) {
            self->animCurFrame++;
        }
        if (self->poseTimer > 36) {
            DestroyEntity(self);
        }
    }
}

void EntityIntenseExplosion(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitParticle);
        self->palette = PAL_FLAG(PAL_UNK_170);
        self->animSet = 5;
        self->animCurFrame = 1;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(PAL_UNK_195);
            self->blendMode = BLEND_TRANSP;
        }
        if (self->params & 0xFF00) {
            self->zPriority = (u16)((self->params & 0xFF00) >> 8);
        }
        self->zPriority += 8;
        return;
    }

    self->poseTimer++;
    self->posY.val -= FIX(0.25);

    if (!((u16)self->poseTimer & 1)) {
        self->animCurFrame++;
    }

    if (self->poseTimer >= 0x25) {
        DestroyEntity(self);
    }
}
#endif

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

