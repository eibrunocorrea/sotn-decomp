// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

static bool SealedDoorIsNearPlayer(Entity* self) {
    s16 distanceX;
    s16 diffX;
    s16 distanceY;
    s16 diffY;

    diffX = PLAYER.posX.i.hi - self->posX.i.hi;
    distanceX = abs(diffX);
    if (distanceX > 24) {
        return false;
    }

    diffY = PLAYER.posY.i.hi - self->posY.i.hi;
    distanceY = abs(diffY);
    if (distanceY > 32) {
        return false;
    }

    return true;
}

// Donor: src/st/no2/e_sealed_door.c EntitySealedDoor (matched). bo0's own
// asm confirms same overall structure, with concrete differences: no
// PlaySfxVolPan calls anywhere (bo0 never plays door open/close SFX via
// that API), case0's near-player branch skips g_Player.padSim/demo_timer
// but adds PLAYER.step = 0, case1's trigger condition drops the
// PLAYER.step==25-while-running alternative, and case4's demo_timer reset
// is unconditional (not gated by VERSION_PSP as in the donor).
extern u8 D_us_801813D4[][8];
extern s16 D_us_80181404[][8];

void func_us_801BB08C(Entity* self) {
    Primitive* prim;
    s32 i;
    s16 angle;
    u8* uv;
    s16 x, y;
    u8 params;
    s16 endX;
    s16 scrollX, scrollY;
    s32 tileIdx;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animSet = 7;
        self->animCurFrame = 1;
        if (self->params & 0x1000) {
            self->animSet = 0;
        }
        self->zPriority = PLAYER.zPriority - 0x20;
        self->facingLeft = 0;
        self->posY.i.hi += 0x1F;

        if (self->params & 0x100) {
            self->ext.sealedDoor.unk86 = -4;
        } else {
            self->ext.sealedDoor.unk86 = 4;
        }
        self->posX.i.hi += self->ext.sealedDoor.unk86;
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 3);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        uv = D_us_801813D4[0];
        if (self->params & 0x1000) {
            uv += 0x18;
        }
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0, y = self->posY.i.hi - 0x1F; prim != NULL; i++, uv += 8,
            prim = prim->next) {
            prim->u0 = uv[0];
            prim->u1 = uv[1];
            prim->u2 = uv[2];
            prim->u3 = uv[3];
            prim->v0 = uv[4];
            prim->v1 = uv[5];
            prim->v2 = uv[6];
            prim->v3 = uv[7];
            prim->tpage = 0x1F;
            prim->clut = 0x198;
            if (self->params & 0x1000) {
                prim->tpage = 0x12;
                prim->clut = 0x21B;
            }
            prim->priority = PLAYER.zPriority - 0x20;
            prim->y0 = prim->y1 = y;
            prim->y2 = prim->y3 = y + 0x3E;
            if (i == 0) {
                prim->y0 = prim->y1 = y;
                prim->y2 = prim->y3 = y + 0x3E;
            }
            prim->drawMode = DRAW_COLORS | DRAW_UNK02;
            prim->r0 = prim->b0 = prim->g0 = 0x7F;
            prim->r1 = prim->b1 = prim->g1 = 0x7F;
            prim->r2 = prim->b2 = prim->g2 = 0x7F;
            prim->r3 = prim->b3 = prim->g3 = 0x7F;
            if (i == 2 && (self->params & 0x100) == 0) {
                prim->drawMode |= DRAW_HIDE;
            }
            if (i == 1 && (self->params & 0x100)) {
                prim->drawMode |= DRAW_HIDE;
            }
        }
        if (SealedDoorIsNearPlayer(self)) {
            if ((self->params & 0x100) == 0) {
                self->ext.sealedDoor.angle = 0x1000;
            }
            if ((self->params & 0x100) != 0) {
                self->ext.sealedDoor.angle = 0x800;
            }
            self->animCurFrame = 0;
            self->step = 4;
            PLAYER.step = 0;
            PLAYER.velocityY = 0;
        } else {
            self->ext.sealedDoor.angle = 0xC00;
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; prim != NULL; prim = prim->next) {
                if ((self->params & 0x1000) == 0 || i != 0) {
                    prim->drawMode |= DRAW_HIDE;
                }
                i++;
                if (i == 3) {
                    break;
                }
            }
        }
        break;

    case 1:
        if (PLAYER.step == 1 && PLAYER.facingLeft != GetSideToPlayer() &&
            SealedDoorIsNearPlayer(self)) {
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; prim != NULL; i++, prim = prim->next) {
                if (i == 1 && (self->params & 0x100) == 0) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
                if (i == 2 && (self->params & 0x100) != 0) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
                if (i == 0) {
                    prim->drawMode &= ~DRAW_HIDE;
                }
            }
            self->animCurFrame = 0;
            g_Player.padSim = 0;
            g_Player.demo_timer = 2;
            g_api.PlaySfx(SFX_DOOR_OPEN);
            self->step++;
        }
        break;

    case 2:
        g_Player.padSim = 0;
        g_Player.demo_timer = 24;
        if ((self->params & 0x100) == 0) {
            self->ext.sealedDoor.angle += 0x20;
            if (self->ext.sealedDoor.angle >= 0x1000) {
                self->ext.sealedDoor.angle = 0x1000;
            }
            if (self->ext.sealedDoor.angle == 0x1000) {
                self->step++;
            }
        } else {
            self->ext.sealedDoor.angle -= 0x20;
            if (self->ext.sealedDoor.angle <= 0x800) {
                self->ext.sealedDoor.angle = 0x800;
            }
            if (self->ext.sealedDoor.angle == 0x800) {
                self->step++;
            }
        }
        break;

    case 3:
        if (g_Player.demo_timer >= 4) {
            return;
        }
        if ((self->params & 0x100) == 0) {
            g_Player.padSim = PAD_LEFT;
        } else {
            g_Player.padSim = PAD_RIGHT;
        }
        g_Player.demo_timer = 3;
        break;

    case 4:
        if ((self->params & 0x100) == 0) {
            g_Player.padSim = PAD_RIGHT;
        } else {
            g_Player.padSim = PAD_LEFT;
        }
        g_Player.demo_timer = 4;
        if (!SealedDoorIsNearPlayer(self)) {
            g_api.PlaySfx(SFX_DOOR_OPEN);
            self->step++;
            g_Player.demo_timer = 0;
        }
        break;

    case 5:
        g_Player.padSim = 0;
        g_Player.demo_timer = 4;
        if ((self->params & 0x100) == 0) {
            self->ext.sealedDoor.angle -= 0x20;
            if (self->ext.sealedDoor.angle <= 0xC00) {
                self->ext.sealedDoor.angle = 0xC00;
            }
        } else {
            self->ext.sealedDoor.angle += 0x20;
            if (self->ext.sealedDoor.angle >= 0xC00) {
                self->ext.sealedDoor.angle = 0xC00;
            }
        }
        if (self->ext.sealedDoor.angle == 0xC00) {
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; prim != NULL; i++, prim = prim->next) {
                if ((self->params & 0x1000) == 0 || i != 0) {
                    prim->drawMode |= DRAW_HIDE;
                }
            }
            self->animCurFrame = 1;
            self->step = 1;
        }
        break;
    }

    if (self->step != 1) {
        g_api.func_8010E168(1, 0x20);
        g_api.func_8010DFF0(1, 1);
    }

    x = self->posX.i.hi - self->ext.sealedDoor.unk86;
    if (self->params & 0x100) {
        x--;
    } else {
        x++;
    }

    i = 0;
    angle = self->ext.sealedDoor.angle;
    prim = &g_PrimBuf[self->primIndex];
    for (; prim != NULL; i++, prim = prim->next) {
        if (prim->drawMode & DRAW_HIDE) {
            continue;
        }
        if ((self->params & 0x100) == 0) {
            if (i == 0) {
                endX = prim->x0 = prim->x2 = x + ((rcos(angle) >> 8) * 32 >> 4);
                prim->x1 = prim->x3 = prim->x0 - ((rsin(angle) >> 4) * 6 >> 8);
                if (angle > 0xF80) {
                    prim->x1 = prim->x3 = prim->x0 + 1;
                }
                if (angle > 0xE00) {
                    prim->u0 = prim->u2 = 0xB2;
                    prim->u1 = prim->u3 = 0xB6;
                    if (self->params & 0x1000) {
                        prim->u0 = prim->u2 = 4;
                        prim->u1 = prim->u3 = 12;
                    }
                }
                if (angle <= 0xE00) {
                    prim->u0 = prim->u2 = 0xB1;
                    prim->u1 = prim->u3 = 0xB7;
                    if (self->params & 0x1000) {
                        prim->u0 = prim->u2 = 3;
                        prim->u1 = prim->u3 = 13;
                    }
                }
                if (angle == 0x1000) {
                    prim->r1 = prim->b1 = prim->g1 = 0x3F;
                    prim->r3 = prim->b3 = prim->g3 = 0x3F;
                } else {
                    prim->r1 = prim->b1 = prim->g1 =
                        0x7F - ((angle & 0x3FF) >> 4);
                    prim->r3 = prim->b3 = prim->g3 =
                        0x7F - ((angle & 0x3FF) >> 4);
                }
            } else {
                prim->x0 = prim->x2 = x;
                prim->x1 = prim->x3 = endX;
                if (angle == 0x1000) {
                    prim->r0 = prim->b0 = prim->g0 = 0x3F;
                    prim->r2 = prim->b2 = prim->g2 = 0x3F;
                } else {
                    prim->r0 = prim->b0 = prim->g0 = (angle & 0x3FF) >> 4;
                    prim->r2 = prim->b2 = prim->g2 = (angle & 0x3FF) >> 4;
                }
            }
        } else {
            if (i == 0) {
                endX = prim->x1 = prim->x3 = x + ((rcos(angle) >> 8) * 32 >> 4);
                prim->x0 = prim->x2 =
                    prim->x1 + (((rsin(angle) >> 4) * 6) >> 8);
                if (angle < 0x880) {
                    prim->x0 = prim->x2 = prim->x1 - 1;
                }
                if (angle < 0xA00) {
                    prim->u0 = prim->u2 = 0xB2;
                    prim->u1 = prim->u3 = 0xB6;
                    if (self->params & 0x1000) {
                        prim->u0 = prim->u2 = 4;
                        prim->u1 = prim->u3 = 12;
                    }
                }
                if (angle > 0xA00) {
                    prim->u0 = prim->u2 = 0xB1;
                    prim->u1 = prim->u3 = 0xB7;
                    if (self->params & 0x1000) {
                        prim->u0 = prim->u2 = 3;
                        prim->u1 = prim->u3 = 13;
                    }
                }
                if (angle == 0x800) {
                    prim->r0 = prim->b0 = prim->g0 = 0x7F;
                    prim->r2 = prim->b2 = prim->g2 = 0x7F;
                } else {
                    prim->r0 = prim->b0 = prim->g0 =
                        0x3F + ((angle & 0x3FF) >> 4);
                    prim->r2 = prim->b2 = prim->g2 =
                        0x3F + ((angle & 0x3FF) >> 4);
                }
            } else {
                prim->x0 = prim->x2 = endX - 1;
                prim->x1 = prim->x3 = x;
                if (angle == 0x800) {
                    prim->r1 = prim->b1 = prim->g1 = 0x3F;
                    prim->r3 = prim->b3 = prim->g3 = 0x3F;
                } else {
                    prim->r1 = prim->b1 = prim->g1 =
                        0x3F - ((angle & 0x3FF) >> 4);
                    prim->r3 = prim->b3 = prim->g3 =
                        0x3F - ((angle & 0x3FF) >> 4);
                }
            }
        }
    }
    params = self->params & 0xFF;
    if (self->animCurFrame) {
        for (i = 0; i < 4; i++) {
            x = self->posX.i.hi;
            y = self->posY.i.hi - 24 + i * 0x10;
            scrollX = x + g_Tilemap.scrollX.i.hi;
            scrollY = y + g_Tilemap.scrollY.i.hi;
            tileIdx = (scrollX >> 4) + (scrollY >> 4) * g_Tilemap.hSize * 0x10;
            g_Tilemap.fg[tileIdx] = D_us_80181404[params][i];
        }
    } else {
        for (i = 0; i < 4; i++) {
            x = self->posX.i.hi;
            y = self->posY.i.hi - 24 + i * 0x10;
            scrollX = x + g_Tilemap.scrollX.i.hi;
            scrollY = y + g_Tilemap.scrollY.i.hi;
            tileIdx = (scrollX >> 4) + (scrollY >> 4) * g_Tilemap.hSize * 0x10;
            g_Tilemap.fg[tileIdx] = D_us_80181404[params][i + 4];
        }
    }
}
