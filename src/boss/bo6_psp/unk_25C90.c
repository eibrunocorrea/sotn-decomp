// SPDX-License-Identifier: AGPL-3.0-or-later
#include "player_copy.h"

extern EInit D_pspeu_0926EBB8;

extern EInit D_pspeu_0926EC08;
extern RECT D_pspeu_0927B018;
extern u16 D_pspeu_0927AFB8[];

extern EInit D_pspeu_0926EBE8;

extern AnimationFrame D_pspeu_0927A558[];

extern u16 D_pspeu_0927AEA0[];
extern u16 D_pspeu_0927AE88[];
extern u16 D_pspeu_0927AED0[];
extern u16 D_pspeu_0927AEB8[];
extern u16 D_pspeu_0927AF00[];
extern u16 D_pspeu_0927AEE8[];
extern u32 D_pspeu_0927DC00;

// Boss Richter reuses his subweapon-crash entities; splat's pre-split
// func_pspeu_* names are kept until a rename pass
extern s16 D_pspeu_0927AF38[4][6]; // glass shatter offsets (stage data blob)
extern s16 D_pspeu_0927B048[];     // cross trail anim frames (D_us_80182994)

extern s16 D_pspeu_0927AF18[];

void func_pspeu_0925D310(Entity* self) {
    Primitive* prim;
    s32 heartCost;
    s16 tempX;
    s16 tempY;
    u32 heartBroachesWorn;
    if (g_Maria.status & (PLAYER_STATUS_TRANSFORM | PLAYER_STATUS_UNK10000)) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->facingLeft = MARIA.facingLeft;
        self->hitboxWidth = self->hitboxHeight = 4;
        self->hitboxOffX = 4;
        self->hitboxOffY = 0;
        self->posY.i.hi = self->ext.agunea.unk82 =
            MARIA.posY.i.hi + MARIA.hitboxOffY - 8;
        self->posX.i.hi = self->ext.agunea.unk80 = MARIA.posX.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        prim->type = PRIM_LINE_G2;
        prim->priority = MARIA.zPriority + 2;
        prim->drawMode = DRAW_UNK_200 | DRAW_UNK_100 | DRAW_TPAGE2 |
                         DRAW_TPAGE | DRAW_TRANSP;
        prim->r1 = 0x60;
        prim->g1 = 0;
        prim->b1 = 0x80;
        MarSetSpeedX(FIX(6));
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        self->posX.val += self->velocityX;
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x20 || self->posY.i.hi > 0x120) {
            self->step = 2;
        }
        if (self->hitFlags) {
            self->ext.agunea.parent = self->unkB8;
            self->step = 3;
        }
        break;
    case 4:
        self->posX.i.hi = self->ext.agunea.parent->posX.i.hi;
        self->posY.i.hi = self->ext.agunea.parent->posY.i.hi;
        if (++self->ext.agunea.unk7C < 16) {
            break;
        }
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (prim->r1 < 5) {
            DestroyEntity(self);
            return;
        }
        break;
    case 3:
        if ((g_Maria.padPressed & (PAD_UP + PAD_SQUARE)) !=
            (PAD_UP + PAD_SQUARE)) {
            self->step = 4;
        }
        if (!self->ext.agunea.parent->entityId) {
            self->step = 2;
            return;
        }
        if (self->ext.agunea.unk7C &&
            (self->ext.agunea.parent->hitPoints > 0x7000 ||
             !self->ext.agunea.parent->hitPoints ||
             !self->ext.agunea.parent->hitboxState)) {
            self->step = 2;
            return;
        }
        tempX = self->posX.i.hi = self->ext.agunea.parent->posX.i.hi;
        tempY = self->posY.i.hi = self->ext.agunea.parent->posY.i.hi;
        if ((self->ext.agunea.unk7C % 12) == 0) {
            self->posX.i.hi += (rand() & 0xF) - 8;
            self->posY.i.hi += (rand() & 0xF) - 8;
            if (g_Status.hearts >= 5) {
                g_Status.hearts -= 5;
                MarCreateEntFactoryFromEntity(self, BP_AGUNEA_HIT_ENEMY, 0);
                g_api.PlaySfx(SFX_THUNDER_B);
            } else if (self->ext.agunea.unk84 == 0) {
                MarCreateEntFactoryFromEntity(self, BP_AGUNEA_HIT_ENEMY, 0);
                g_api.PlaySfx(SFX_THUNDER_B);
                self->ext.agunea.unk84++;
            } else {
                self->step = 4;
            }
        }
        self->posX.i.hi = tempX;
        self->posY.i.hi = tempY;
        self->ext.agunea.unk7C++;
        break;
    }
    prim = &g_PrimBuf[self->primIndex];
    if (prim->r1 >= 4) {
        prim->r1 -= 4;
    }
    if (prim->g1 >= 4) {
        prim->g1 -= 4;
    }
    if (prim->b1 >= 4) {
        prim->b1 -= 4;
    }
    if (prim->b1 < 5) {
        prim->drawMode |= DRAW_HIDE;
    }
    prim->x0 = self->ext.agunea.unk80;
    prim->y0 = self->ext.agunea.unk82;
    prim->x1 = self->posX.i.hi;
    prim->y1 = self->posY.i.hi;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925D980);

void func_pspeu_0925E330(Entity* entity) {
    s32 angle;
    switch (entity->step) {
    case 0:
        entity->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        entity->posX.val = entity->ext.vibCrashCloud.parent->ext.vibhutiCrash.x;
        entity->posY.val = entity->ext.vibCrashCloud.parent->ext.vibhutiCrash.y;
        entity->facingLeft =
            entity->ext.vibCrashCloud.parent->ext.vibhutiCrash.facing;
        entity->flags |= FLAG_UNK_20000000;
        entity->unk5A = 0x64;
        entity->animSet = ANIMSET_DRA(14);
        entity->palette = PAL_FLAG(PAL_UNK_19E);
        entity->anim = D_pspeu_0927A558;
        entity->blendMode = BLEND_TRANSP | BLEND_ADD;
        entity->drawFlags = ENTITY_OPACITY;
        entity->opacity = 0x60;
        entity->hitboxWidth = 8;
        entity->hitboxHeight = 8;
        angle = (rand() % 512) + 0x300;
        entity->velocityX = rcos(angle) * 32;
        entity->velocityY = -(rsin(angle) * 32);
        entity->step++;
        break;
    case 1:
        entity->ext.vibCrashCloud.unk7C++;
        if (entity->ext.vibCrashCloud.unk7C >= 39) {
            DestroyEntity(entity);
        } else {
            entity->posX.val += entity->velocityX;
            entity->posY.val += entity->velocityY;
        }
        break;
    }
}

void func_pspeu_0925E578(Entity* self) {
    FakePrim* prim;
    s32 angle;
    s32 magnitude;
    s32 i;
    s32 facing;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_TILE, 9);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            g_Maria.unk4E = 1;
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        for (i = 0; i < 9; i++) {
            prim->r0 = prim->g0 = prim->b0 = 0xFF;
            prim->w = prim->h = 1;
            prim->priority = MARIA.zPriority + 8;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
            prim = prim->next;
        }
        self->step++;
        break;
    case 1:
        self->ext.vibhutiCrash.unk7E++;
        if (!(self->ext.vibhutiCrash.unk7E % 2) &&
            self->ext.vibhutiCrash.timer < 8) {
            self->ext.vibhutiCrash.timer++;
            self->ext.vibhutiCrash.unk80++;
            if (self->ext.vibhutiCrash.unk80 >= 0x30) {
                self->step++;
            }
            prim = (FakePrim*)&g_PrimBuf[self->primIndex];
            for (i = 0; i < 9; i++) {
                if (prim->drawMode & DRAW_HIDE) {
                    break;
                }
                prim = prim->next;
            }
            prim->posX.val = MARIA.posX.val;
            prim->posY.val = MARIA.posY.val - FIX(24);
            angle = rand() % 0x200 + 0x300;
            magnitude = (rand() % 24) + 0x20;
            prim->velocityX.val = (rcos(angle) * magnitude);
            prim->velocityY.val = -(rsin(angle) * magnitude);
            prim->drawMode &= ~DRAW_HIDE;
            prim->delay = 0x10;
        }
    case 2:
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        for (i = 0; i < 9; i++) {
            if (!(prim->drawMode & DRAW_HIDE)) {
                if (!--prim->delay) {
                    prim->drawMode |= DRAW_HIDE;
                    self->ext.vibhutiCrash.timer--;
                    self->ext.vibhutiCrash.x = prim->posX.val;
                    self->ext.vibhutiCrash.y = prim->posY.val;
                    if (prim->velocityX.val > 0) {
                        facing = false;
                    } else {
                        facing = true;
                    }
                    self->ext.vibhutiCrash.facing = facing;
                    MarCreateEntFactoryFromEntity(
                        self, BP_VITHUBI_CRASH_CLOUD, 0);
                } else {
                    prim->posX.val += prim->velocityX.val;
                    prim->posY.val += prim->velocityY.val;
                    prim->velocityY.val += FIX(0.25);
                    prim->x0 = prim->posX.i.hi;
                    prim->y0 = prim->posY.i.hi;
                }
            }
            prim = prim->next;
        }
        if (self->step == 2 && !self->ext.vibhutiCrash.timer) {
            self->step++;
        }
        break;
    case 3:
        g_Maria.unk4E = 1;
        g_Maria.unk4E = 1;
        DestroyEntity(self);
        break;
    }
}

// local copy of func_us_801C8590 (see us_3E79C.c); the similarly-placed
// ric function has a different body
void func_pspeu_0925EA20(Entity* self) {
    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_10000000;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        self->step++;
        break;
    case 1:
        self->ext.timer.t++;
        if (self->ext.timer.t > 3) {
            DestroyEntity(self);
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925EAD8);

// local copy of RicEntityCrashReboundStoneExplosion (see us_3E79C.c)
void func_pspeu_0925F210(Entity* self) {
    Primitive* prim;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_G4, 0x10);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 0x10; i++) {
            prim->priority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            prim->x0 = prim->x1 = 0x80;
            prim->y0 = prim->y1 = 0;
            prim = prim->next;
        }
        self->ext.reboundStoneCrashExplosion.unk7C = 0x40;
        self->ext.reboundStoneCrashExplosion.unk7E = 0;
        self->ext.reboundStoneCrashExplosion.unk84 = 0;
        self->ext.reboundStoneCrashExplosion.unk80 = 0x10;
        self->ext.reboundStoneCrashExplosion.unk82 = 8;
        self->step++;
        break;
    case 1:
        self->ext.reboundStoneCrashExplosion.unk84 += 0x20;
        if (self->ext.reboundStoneCrashExplosion.unk84 > 0x120) {
            self->posX.val = FIX(128.0);
            self->posY.val = FIX(128.0);
            self->hitboxWidth = 0x80;
            self->hitboxHeight = 0x80;
            self->step++;
        }
        break;
    case 2:
        self->ext.reboundStoneCrashExplosion.unk86++;
        if (self->ext.reboundStoneCrashExplosion.unk86 == 5) {
            self->ext.reboundStoneCrashExplosion.unk80 = -0x18;
        } else if (self->ext.reboundStoneCrashExplosion.unk86 >= 0xF) {
            self->ext.reboundStoneCrashExplosion.unk82 = -0x18;
            self->step++;
        }
        break;
    case 3:
        break;
    case 4:
        DestroyEntity(self);
        return;
    }
    self->ext.reboundStoneCrashExplosion.unk7C +=
        self->ext.reboundStoneCrashExplosion.unk80;
    if (self->ext.reboundStoneCrashExplosion.unk7C > 0xFF) {
        self->ext.reboundStoneCrashExplosion.unk7C = 0xFF;
        self->ext.reboundStoneCrashExplosion.unk80 = 0;
    } else if (self->ext.reboundStoneCrashExplosion.unk7C < 0) {
        self->ext.reboundStoneCrashExplosion.unk7C =
            self->ext.reboundStoneCrashExplosion.unk80 = 0;
    }
    self->ext.reboundStoneCrashExplosion.unk7E +=
        self->ext.reboundStoneCrashExplosion.unk82;
    if (self->ext.reboundStoneCrashExplosion.unk7E > 0xFF) {
        self->ext.reboundStoneCrashExplosion.unk7E = 0xFF;
        self->ext.reboundStoneCrashExplosion.unk82 = 0;
    } else if (self->ext.reboundStoneCrashExplosion.unk7E < 0) {
        self->ext.reboundStoneCrashExplosion.unk7E =
            self->ext.reboundStoneCrashExplosion.unk82 = 0;
        self->step += 1;
    }
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        prim->b0 = prim->b1 = self->ext.reboundStoneCrashExplosion.unk7C;
        prim->b2 = prim->b3 = self->ext.reboundStoneCrashExplosion.unk7E;
        prim->r0 = prim->r1 = prim->g0 = prim->g1 =
            self->ext.reboundStoneCrashExplosion.unk7C;
        prim->r2 = prim->r3 = prim->g2 = prim->g3 =
            self->ext.reboundStoneCrashExplosion.unk7E;
        if (self->step < 2) {
            prim->x2 =
                ((rcos(i << 7) * self->ext.reboundStoneCrashExplosion.unk84) >>
                 0xC) +
                0x80;
            prim->x3 = ((rcos((i + 1) << 7) *
                         self->ext.reboundStoneCrashExplosion.unk84) >>
                        0xC) +
                       0x80;
            prim->y2 =
                ((rsin(i << 7) * self->ext.reboundStoneCrashExplosion.unk84) >>
                 0xC);
            prim->y3 = ((rsin((i + 1) << 7) *
                         self->ext.reboundStoneCrashExplosion.unk84) >>
                        0xC);
        }
        prim = prim->next;
    }
}

// local copy of RicEntityCrashReboundStone (see us_3E79C.c)
void func_pspeu_0925F728(Entity* entity) {
    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_10000000;
        entity->step++;
        entity->ext.timer.t = 0x14;
        // fallthrough
    case 1:
        if (--entity->ext.timer.t) {
            break;
        }
    case 3:
    case 5:
        MarCreateEntFactoryFromEntity(entity, BP_57, 0);
        entity->step++;
    case 2:
    case 4:
    case 6:
        entity->ext.timer.t++;
        if (entity->ext.timer.t > 10) {
            entity->ext.timer.t = 0;
            entity->posX.val = FIX(128.0);
            entity->posY.val = 0;
            MarCreateEntFactoryFromEntity(entity, FACTORY(BP_EMBERS, 1), 0);
            entity->step++;
        }
        break;
    case 7:
        entity->ext.timer.t++;
        if (entity->ext.timer.t > 15) {
            DestroyEntity(entity);
            g_Maria.unk4E = 1;
            MarCreateEntFactoryFromEntity(
                entity, BP_CRASH_REBOUND_STONE_EXPLOSION, 0);
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0925F8A8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09260020);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09260AE8);

// local copy of func_us_801CA340 (see us_3E79C.c); the ric function at
// this position (RicEntityStopwatchCrashLightning) has a different body
void func_pspeu_092612E8(Entity* self) {
    MarCreateEntFactoryFromEntity(self, FACTORY(0x3F, 1), 0);
    DestroyEntity(self);
}

// local copy of GetAguneaLightningAngle (see ric/319C4.c)
s16 func_pspeu_09261328(s16* arg0, s16 arg1, s16 arg2, s16* arg3) {
    arg1 += rand() % 256 - 0x80;
    *arg3 = (rand() % 48) + 0x10;
    arg0[0] = arg0[1];
    arg0[2] = arg0[3];
    if (arg2) {
        arg0[1] += (rcos(arg1) * *arg3) >> 0xC;
        arg0[3] += (rsin(arg1) * *arg3) >> 0xC;
        if (arg2 % 2) {
            return func_pspeu_09261328(arg0, arg1 - 0x140, arg2 / 2, arg3);
        } else {
            rand();
            rand();
            return func_pspeu_09261328(
                arg0, arg1 + 0x140, (arg2 - 1) / 2, arg3);
        }
    }
    return arg1;
}

// local copy of AguneaShuffleParams (see ric/319C4.c)
void func_pspeu_09261530(s32 bufSize, s32* buf) {
    s32 i, idx, swapTemp;

    for (i = bufSize - 1; i > 0; i--) {
        // Pick random index
        idx = rand() % bufSize;

        // Hold swap value temporarily
        swapTemp = buf[i];
        buf[i] = buf[idx];
        buf[idx] = swapTemp;
    }
}

// local copy of RicEntityAguneaLightning (see us_3E79C.c)
void func_pspeu_092615E8(Entity* self) {
    s16 sp20;
    s16 angle;
    s16 sp18;
    s32 randomSeed;
    s16 sp10[4];
    s16 xCoord;
    s16 yCoord;
    s16 psp_s6;
    s16 psp_s5;
    s32 psp_s4;
    s32 psp_s3;
    s32 i;
    Primitive* prim;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0xF);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_UNK_194;
            xCoord = (rand() % 5) * 0x10;
            prim->u0 = prim->u2 = xCoord + 0x90;
            prim->u1 = prim->u3 = xCoord + 0xB0;
            if (rand() % 2) {
                prim->v0 = prim->v1 = 0xD0;
                prim->v2 = prim->v3 = 0xE0;
            } else {
                prim->v0 = prim->v1 = 0xE0;
                prim->v2 = prim->v3 = 0xD0;
            }
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim->priority = 0xC1;
            prim->drawMode = DRAW_HIDE | DRAW_COLORS | DRAW_UNK02;
            prim = prim->next;
        }
        prim = &g_PrimBuf[self->primIndex];
        sp20 = ((self->params & 0xFF00) >> 8) * 0x200;
        sp20 += rand() % 0x200 - 0x100;
        randomSeed = rand() & PSP_RANDMASK;
        for (i = 0; i < 15; i++) {
            srand(randomSeed);
            sp10[1] = self->posX.i.hi;
            sp10[3] = self->posY.i.hi;
            angle = func_pspeu_09261328(sp10, sp20, i, &sp18);
            xCoord = sp10[0];
            yCoord = sp10[2];
            psp_s6 = !i ? 2 : 8;
            psp_s5 = (i < 7) ? 8 : 2;

            psp_s4 = rcos(angle);
            psp_s3 = rsin(angle);
            prim->x0 = xCoord + (-(psp_s3 * -psp_s6) >> 0xC);
            prim->y0 = yCoord + ((psp_s4 * -psp_s6) >> 0xC);
            prim->x1 = xCoord + ((psp_s4 * sp18 - (psp_s3 * -psp_s5)) >> 0xC);
            prim->y1 = yCoord + ((psp_s3 * sp18 + (psp_s4 * -psp_s5)) >> 0xC);
            prim->x2 = xCoord + (-(psp_s3 * psp_s6) >> 0xC);
            prim->y2 = yCoord + ((psp_s4 * psp_s6) >> 0xC);
            prim->x3 = xCoord + ((psp_s4 * sp18 - (psp_s3 * psp_s5)) >> 0xC);
            prim->y3 = yCoord + ((psp_s3 * sp18 + (psp_s4 * psp_s5)) >> 0xC);
            prim = prim->next;
        }
        self->ext.et_8017091C.unk7E = 1;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->drawMode &= ~DRAW_HIDE;
            prim = prim->next;
        }
        self->step++;
    case 2:
        self->ext.et_8017091C.unk7C++;
        if (self->ext.et_8017091C.unk7C > 4) {
            prim = &g_PrimBuf[self->primIndex];
            for (i = 0; i < 15; i++) {
                prim->v0 = prim->v1 = prim->v0 - 0x10;
                prim->v2 = prim->v3 = prim->v2 - 0x10;
                prim->clut = PAL_FILL_WHITE;
                prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 =
                    prim->b1 = prim->r2 = prim->g2 = prim->b2 = prim->r3 =
                        prim->g3 = prim->b3 = 0xFF;
                prim = prim->next;
            }
            self->ext.et_8017091C.unk7C = 0;
            self->step++;
        }
        break;
    case 3:
    case 5:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->clut = PAL_UNK_194;
            prim = prim->next;
        }
        self->step++;
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->clut = PAL_FILL_WHITE;
            prim = prim->next;
        }
        self->step++;
        break;
    case 6:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 15; i++) {
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x60 - (self->ext.et_8017091C.unk7C * 4);
            prim = prim->next;
        }
        self->ext.et_8017091C.unk7C++;
        if (self->ext.et_8017091C.unk7C > 15) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09261ED8);

// local copy of RicEntitySubwpnStopwatchCircle (see ric/319C4.c)
void func_pspeu_09262838(Entity* self) {
    s16 temp_s0_4;
    s16 psp_s4;
    s32 sine;
    s32 cosine;
    s16 xCoord;
    s16 yCoord;
    Primitive* prim;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x10);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim->tpage = 0x1A;
            prim->clut = PAL_FILL_WHITE;
            prim->priority = self->zPriority = 0xC2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_TRANSP;
            psp_s4 = i * 0x100;
            prim->u0 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v0 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            psp_s4 = (i + 1) * 0x100;
            prim->u1 = ((rsin(psp_s4) << 5) >> 0xC) + 0x20;
            prim->v1 = -((rcos(psp_s4) << 5) >> 0xC) + 0xDF;
            prim->u2 = prim->u3 = 0x20;
            prim->v2 = prim->v3 = 0xE0;
            prim->r0 = prim->r1 = prim->g0 = prim->g1 = prim->b0 = prim->b1 =
                0x40;
            prim->r2 = prim->r3 = prim->g2 = prim->g3 = 0;
            prim->b2 = prim->b3 = 0x20;
            prim = prim->next;
        }
        self->ext.et_stopwatchCircle.size = 0x20;
        self->step++;
        break;
    case 1:
        self->ext.et_stopwatchCircle.size += 0x18;
        self->ext.et_stopwatchCircle.timer++;
        if (self->ext.et_stopwatchCircle.timer > 0x1E) {
            DestroyEntity(self);
            return;
        }
    }
    xCoord = self->posX.i.hi;
    yCoord = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        sine = rsin(i * 0x100);
        cosine = rcos(i * 0x100);
        temp_s0_4 = self->ext.et_stopwatchCircle.size - 0x20;
        prim->x0 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y0 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x2 = xCoord + ((sine * (temp_s0_4)) >> 0xC);
        prim->y2 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        sine = rsin((i + 1) * 0x100);
        cosine = rcos((i + 1) * 0x100);
        prim->x1 = xCoord + ((sine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->y1 =
            yCoord - ((cosine * self->ext.et_stopwatchCircle.size) >> 0xC);
        prim->x3 = xCoord + ((sine * temp_s0_4) >> 0xC);
        prim->y3 = yCoord - ((cosine * temp_s0_4) >> 0xC);
        prim = prim->next;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09262CE8);

// local copy of RicEntitySubwpnBibleTrail (see us_3E79C.c)
void func_pspeu_09264730(Entity* entity) {
    Primitive* prim;

    switch (entity->step) {
    case 0:
        entity->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[entity->primIndex];
        prim->tpage = 0x1C;
        prim->clut = 0x19D;
        prim->u0 = prim->u2 = 0x20;
        prim->v0 = prim->v1 = 0;
        prim->u1 = prim->u3 = 0x30;
        prim->v2 = prim->v3 = 0x10;
        prim->x0 = prim->x2 = entity->posX.i.hi - 8;
        prim->x1 = prim->x3 = entity->posX.i.hi + 8;
        prim->y0 = prim->y1 = entity->posY.i.hi - 8;
        prim->y2 = prim->y3 = entity->posY.i.hi + 8;
        prim->priority = entity->zPriority;
        prim->drawMode = DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        entity->ext.et_BibleSubwpn.unk7E = 0x60;
        entity->step++;
        break;
    case 1:
        entity->ext.et_BibleSubwpn.unk7C++;
        if (entity->ext.et_BibleSubwpn.unk7C > 5) {
            entity->step++;
        }
        entity->ext.et_BibleSubwpn.unk7E -= 8;
        break;
    case 2:
        DestroyEntity(entity);
        return;
    }
    prim = &g_PrimBuf[entity->primIndex];
    PCOL(prim) = entity->ext.et_BibleSubwpn.unk7E;
}

void func_pspeu_092649F0(Entity* self) {
    Primitive* prim;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s16 selfX;
    s16 selfY;
    s32 sp3C;
    s32 psp_s8;
    s32 psp_s7;
    s32 psp_s4;
    s32 psp_s5;
    s32 psp_s6;
    s32 psp_s3;
    s32 psp_s2;
    s32 psp_s1;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1E;
        prim->clut = 0x17F;
        prim->u0 = prim->u2 = 0x98;
        prim->v0 = prim->v1 = 0xD8;
        prim->u1 = prim->u3 = 0xA8;
        prim->v2 = prim->v3 = 0xF0;
        prim->priority = MARIA.zPriority + 1;
        prim->drawMode = DRAW_HIDE;
        if (self->facingLeft) {
            sp44 = 0x20;
        } else {
            sp44 = -0x20;
        }
        self->ext.et_BibleSubwpn.unk84 = sp44;
        self->hitboxWidth = 6;
        self->hitboxHeight = 6;
        self->step++;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        prim->drawMode &= ~DRAW_HIDE;
        self->ext.et_BibleSubwpn.unk86++;
        self->step++;
    case 2:
        self->ext.et_BibleSubwpn.unk7C++;
        self->ext.et_BibleSubwpn.unk7E++;
        if (self->ext.et_BibleSubwpn.unk7E >= 0x30) {
            self->step++;
        }
        break;
    case 3:
        self->ext.et_BibleSubwpn.unk7C++;
        if (self->ext.et_BibleSubwpn.unk7C >= 0x12C) {
            self->flags &= ~FLAG_UNK_10000000;
            if (self->facingLeft) {
                sp40 = FIX(-12);
            } else {
                sp40 = FIX(12);
            }
            self->velocityX = sp40;
            self->velocityY = FIX(-12);
            self->ext.et_BibleSubwpn.unk86++;
            self->step++;
        }
        break;
    }
    switch (self->ext.et_BibleSubwpn.unk86) {
    case 0:
        break;
    case 1:
        psp_s2 = rsin(self->ext.et_BibleSubwpn.unk80);
        psp_s1 = rcos(self->ext.et_BibleSubwpn.unk80);
        psp_s5 = (psp_s2 * self->ext.et_BibleSubwpn.unk7E) >> 0xC;
        psp_s3 = (psp_s1 * self->ext.et_BibleSubwpn.unk7E) >> 0xC;
        psp_s7 = (psp_s1 * psp_s5 + psp_s2 * psp_s3);
        sp48 = (psp_s1 * psp_s3 - psp_s2 * psp_s5);
        psp_s5 = psp_s7 >> 0xC;
        psp_s3 = sp48 >> 0xC;
        psp_s2 = rsin(self->ext.et_BibleSubwpn.unk82);
        psp_s1 = rcos(self->ext.et_BibleSubwpn.unk82);
        psp_s7 = ((psp_s1 * psp_s5) + (psp_s2 * psp_s4)) >> 0xC;
        psp_s6 = ((psp_s1 * psp_s4) - (psp_s2 * psp_s5)) >> 0xC;
        psp_s4 = psp_s6;
        if (self->facingLeft) {
            psp_s6 = ((psp_s1 * psp_s4) + (psp_s2 * psp_s3)) >> 0xC;
        } else {
            psp_s6 = ((psp_s1 * psp_s4) - (psp_s2 * psp_s3)) >> 0xC;
        }
        self->ext.et_BibleSubwpn.unk80 += self->facingLeft ? 0x80 : -0x80;
        self->ext.et_BibleSubwpn.unk80 &= 0xFFF;
        self->ext.et_BibleSubwpn.unk82 += self->ext.et_BibleSubwpn.unk84;
        if (abs(self->ext.et_BibleSubwpn.unk82) >= 0x200) {
            self->ext.et_BibleSubwpn.unk84 *= -1;
        }
        self->posX.i.hi = MARIA.posX.i.hi + psp_s7;
        self->posY.i.hi = MARIA.posY.i.hi + psp_s6;
        self->zPriority = MARIA.zPriority + (psp_s3 < 0 ? 2 : -2);
        break;
    case 2:
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY -= FIX(2);
        break;
    }
    if (self->ext.et_BibleSubwpn.unk86) {
        selfX = self->posX.i.hi;
        selfY = self->posY.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = prim->x2 = selfX - 8;
        prim->x1 = prim->x3 = selfX + 8;
        prim->y0 = prim->y1 = selfY - 12;
        prim->y2 = prim->y3 = selfY + 12;
        prim->priority = self->zPriority;
        MarCreateEntFactoryFromEntity(self, BP_BIBLE_TRAIL, 0);
        if (g_GameTimer % 10 == 0) {
            g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        }
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09265100);

void func_pspeu_09266AB0(Entity* entity) {
    if (g_Maria.unk46 == 0) {
        DestroyEntity(entity);
        return;
    }
    entity->facingLeft = MARIA.facingLeft;
    if (entity->step == 0) {
        entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        entity->animSet = ANIMSET_OVL(3);
        entity->unk5A = 0x24;
        entity->palette = PAL_FLAG(0x220);
        entity->zPriority = MARIA.zPriority + 2;
    }
    if (MARIA.step == PL_S_CROUCH) {
        if (MARIA.facingLeft) {
            entity->animCurFrame = D_pspeu_0927AEA0[D_pspeu_0927DC00];
        } else {
            entity->animCurFrame = D_pspeu_0927AE88[D_pspeu_0927DC00];
        }
    } else if (MARIA.step == PL_S_STAND) {
        if (MARIA.facingLeft) {
            entity->animCurFrame = D_pspeu_0927AED0[D_pspeu_0927DC00];
        } else {
            entity->animCurFrame = D_pspeu_0927AEB8[D_pspeu_0927DC00];
        }
    } else if (MARIA.facingLeft) {
        entity->animCurFrame = D_pspeu_0927AF00[D_pspeu_0927DC00];
    } else {
        entity->animCurFrame = D_pspeu_0927AEE8[D_pspeu_0927DC00];
    }
    entity->posX.val = MARIA.posX.val;
    entity->posY.val = MARIA.posY.val;
}

void func_pspeu_09266CE8(Entity* entity) {
    if (g_Maria.unk46 == 0) {
        DestroyEntity(entity);
        return;
    }
    if (entity->step == 0) {
        entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
    }
    if (!(entity->params & 0xFF00)) {
        g_Entities[D_pspeu_0927AF18[entity->poseTimer]].palette =
            PAL_FLAG(0x40);
    }
    g_Entities[D_pspeu_0927AF18[entity->poseTimer]].ext.player.unkA4 = 4;
    entity->poseTimer++;
    if (entity->poseTimer == 15) {
        DestroyEntity(entity);
    }
}

void Unused09266E30(Entity* self) {}

// local copy of RicEntitySubwpnHolyWaterBreakGlass (see
// ric/pl_subweapon_holywater.c); prim priority tracks the boss entity
#define FAKEPRIM ((FakePrim*)prim)
void func_pspeu_09266E40(Entity* self) {
    Point16 sp10[8];
    Primitive* prim;
    s16 posX;
    s16 posY;
    s16 arrIndex;
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        posX = self->posX.i.hi;
        posY = self->posY.i.hi;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                sp10[i].x = FAKEPRIM->posX.i.hi = FAKEPRIM->x0 = posX;
                sp10[i].y = FAKEPRIM->posY.i.hi = FAKEPRIM->y0 = posY;
                // Random velocity from 0.25 to 0.5
                FAKEPRIM->velocityX.val = (rand() & 0x3FFF) + FIX(0.25);
                if (i & 1) {
                    FAKEPRIM->velocityX.val = -FAKEPRIM->velocityX.val;
                }
                FAKEPRIM->velocityY.val =
                    -(((rand() & PSP_RANDMASK) * 2) + FIX(2.5));
                FAKEPRIM->drawMode = DRAW_HIDE | DRAW_UNK02;
                FAKEPRIM->type = PRIM_TILE;
            } else {
                prim->r0 = prim->r1 = prim->r2 = prim->r3 =
                    (rand() & 0xF) + 0x30;
                prim->b0 = prim->b1 = prim->b2 = prim->b3 =
                    (rand() & 0x7F) + 0x80;
                prim->g0 = prim->g1 = prim->g2 = prim->g3 =
                    (rand() & 0x1F) + 0x30;
                if (rand() & 1) {
                    prim->drawMode = (DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                      DRAW_UNK02 | DRAW_TRANSP);
                } else {
                    prim->drawMode = (DRAW_COLORS | DRAW_UNK02);
                }
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                prim->u0 = arrIndex = i & 3;
                prim->x0 = posX + D_pspeu_0927AF38[arrIndex][0];
                prim->y0 = posY + D_pspeu_0927AF38[arrIndex][1];
                prim->x1 = posX + D_pspeu_0927AF38[arrIndex][2];
                prim->y1 = posY + D_pspeu_0927AF38[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_pspeu_0927AF38[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_pspeu_0927AF38[arrIndex][5];
                prim->type = PRIM_G4;
                prim->priority = RIC.zPriority + 2;
            }
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.timer.t = 20;
        self->step++;
        break;
    case 1:
        if (--self->ext.timer.t == 0) {
            DestroyEntity(self);
            return;
        }
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; prim != NULL; i++, prim = prim->next) {
            if (i < 8) {
                FAKEPRIM->posX.i.hi = FAKEPRIM->x0;
                FAKEPRIM->posY.i.hi = FAKEPRIM->y0;
                FAKEPRIM->posX.val += FAKEPRIM->velocityX.val;
                FAKEPRIM->posY.val += FAKEPRIM->velocityY.val;
                FAKEPRIM->velocityY.val += FIX(36.0 / 128);
                sp10[i].x = FAKEPRIM->posX.i.hi;
                sp10[i].y = FAKEPRIM->posY.i.hi;
                FAKEPRIM->x0 = FAKEPRIM->posX.i.hi;
                FAKEPRIM->y0 = FAKEPRIM->posY.i.hi;
            } else {
                posX = sp10[i - 8].x;
                posY = sp10[i - 8].y;
                arrIndex = prim->u0;
                prim->x0 = posX + D_pspeu_0927AF38[arrIndex][0];
                prim->y0 = posY + D_pspeu_0927AF38[arrIndex][1];
                prim->x1 = posX + D_pspeu_0927AF38[arrIndex][2];
                prim->y1 = posY + D_pspeu_0927AF38[arrIndex][3];
                prim->x3 = prim->x2 = posX + D_pspeu_0927AF38[arrIndex][4];
                prim->y3 = prim->y2 = posY + D_pspeu_0927AF38[arrIndex][5];
            }
        }
        break;
    }
}

void func_pspeu_09267520(Entity* self) {
    PrimLineG2* line;
    s16 primcount;
    s32 i;
    if (self->params < 24) {
        primcount = 32;
    } else {
        primcount = 33 - (self->params - 32) * 2;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, primcount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.subweapon.subweaponId = PL_W_HYDROSTORM;
        InitializeEntity(D_pspeu_0926EBE8);
        self->posX.i.hi = 0x80;
        self->posY.i.hi = 0x70;
        self->hitboxWidth = 0x78;
        self->hitboxHeight = 0x78;
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->facingLeft = 0;
        line = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (line != NULL) {
            line->r0 = 0x1F;
            line->g0 = 0x1F;
            line->b0 = 0x30;
            line->r1 = 0x3F;
            line->g1 = 0x50;
            line->b1 = 0x7F;
            line->x0 = rand() & 0x1FF;
            line->x1 = line->x0;
            line->y0 = -(rand() & 0xF);
            line->y1 = line->y0;
            line->preciseX.i.hi = line->x1;
            line->preciseY.i.hi = line->y1;
            line->velocityX.val = (rcos(ROT(258.75)) * 16) * 12;
            line->velocityY.val = -(rsin(ROT(258.75)) * 16) * 12;
            line->timer = 0;
            line->delay = (rand() & 0xF) + 12;
            if (rand() & 1) {
                line->priority = MARIA.zPriority + 2;
            } else {
                line->priority = MARIA.zPriority - 2;
            }
            line->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
            line = line->next;
        }
        if (self->params == 1) {
            g_api.SetFadeMode(FADE_BLUE_TINT);
        }
        self->ext.subweapon.timer = 0x160;
        if ((self->params < 16) && !(self->params & 3)) {
            g_api.PlaySfx(SFX_BOSS_RIC_HYDRO_STORM_RAIN);
        }
        self->step = 1;
        break;
    case 1:
        line = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        for (i = 0; line != NULL; i++, line = line->next) {
            if (line->timer == 0) {
                line->preciseX.i.hi = line->x1;
                line->preciseY.i.hi = line->y1;
                line->preciseX.val += line->velocityX.val;
                line->preciseY.val += line->velocityY.val;
                line->x1 = line->preciseX.i.hi;
                line->y1 = line->preciseY.i.hi;
                if (line->y1 > line->delay) {
                    line->timer++;
                    line->xLength = line->x0 - line->x1;
                    line->yLength = line->y0 - line->y1;
                }
            } else {
                line->preciseX.i.hi = line->x1;
                line->preciseY.i.hi = line->y1;
                line->preciseX.val += line->velocityX.val;
                line->preciseY.val += line->velocityY.val;
                line->x1 = line->preciseX.i.hi;
                line->y1 = line->preciseY.i.hi;
                line->y0 = line->y1 + line->yLength;
                line->x0 = line->x1 + line->xLength;
                if (line->y0 >= 0xD8) {
                    self->step = 2;
                }
            }
        }
        self->ext.subweapon.timer++;
        break;
    case 2:
        if (self->params == 0x18) {
            g_Maria.unk4E = 1;
        }
        DestroyEntity(self);
        break;
    }
    g_Maria.timers[PL_T_3] = 16;
}

// local copy of RicCheckHolyWaterCollision (see us_3E79C.c)
s32 func_pspeu_09267A60(s16 height, s16 width) {
    Collider collider;
    Collider collider2;
    s16 maskedEffects;
    s16 maskedEffects2;
    s16 posX;
    s16 posY;
    s16 newPosY;

    if ((g_CurrentEntity->posX.val + width) < 0 ||
        (g_CurrentEntity->posX.i.hi + width) > 256) {
        if ((g_CurrentEntity->posY.i.hi + height) >= 212) {
            g_CurrentEntity->posY.i.hi = 212 - height;
            return EFFECT_SOLID;
        }
        return EFFECT_NONE;
    }
    posX = g_CurrentEntity->posX.i.hi + width;
    posY = g_CurrentEntity->posY.i.hi + height;

    g_api.CheckCollision(posX, posY, &collider, 0);
    maskedEffects = collider.effects &
                    (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                     EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_SOLID);
    posY = posY - 1 + collider.unk18;
    g_api.CheckCollision(posX, posY, &collider2, 0);

    newPosY = height + (g_CurrentEntity->posY.i.hi + collider.unk18);
    if ((maskedEffects & (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
            EFFECT_SOLID ||
        (maskedEffects & (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
            (EFFECT_UNK_0800 | EFFECT_SOLID)) {
        maskedEffects = collider2.effects &
                        (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                         EFFECT_UNK_1000 | EFFECT_SOLID);
        if (!(maskedEffects & EFFECT_SOLID)) {
            g_CurrentEntity->posY.i.hi = newPosY;
            return EFFECT_SOLID;
        }
        if (((s32)collider2.effects & (EFFECT_UNK_8000 | EFFECT_SOLID)) ==
            (EFFECT_UNK_8000 | EFFECT_SOLID)) {
            g_CurrentEntity->posY.i.hi = newPosY - 1 + collider2.unk18;
            return maskedEffects;
        }
        return EFFECT_NONE;
    }
    if ((maskedEffects & (EFFECT_UNK_8000 | EFFECT_SOLID)) ==
        (EFFECT_UNK_8000 | EFFECT_SOLID)) {
        g_CurrentEntity->posY.i.hi = newPosY;
        return maskedEffects &
               (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                EFFECT_UNK_1000 | EFFECT_SOLID);
    }
    return EFFECT_NONE;
}

// local copy of static func_8016840C (see us_3E79C.c); non-static so the
// remaining INCLUDE_ASM stubs can reference the symbol. The US declares it
// unprototyped but calls it as a collision stub with (height, width);
// declaring both args reproduces mwcc's 0x20 stack frame
s32 func_801B2E5C(s16 height, s16 width) { return EFFECT_NONE; }

void func_pspeu_09267D20(Entity* self) {
    s16 xMod;
    s32 colRes;

    colRes = 0;
    if (self->step > 2) {
        self->posY.i.hi += 5;
    }
    switch (self->step) {
    case 0:
        self->ext.holywater.subweaponId = PL_W_HOLYWATER;
        InitializeEntity(D_pspeu_0926EBB8);
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->animSet = ANIMSET_OVL(3);
        self->animCurFrame = 0x23;
        self->zPriority = MARIA.zPriority + 2;
        self->unk5A = 0x24;
        self->palette = PAL_FLAG(0x22F);
        xMod = 0;
        if (self->facingLeft) {
            xMod = -xMod;
        }
        self->posX.i.hi += xMod;
        self->posY.i.hi += -16;
        self->ext.holywater.angle = (rand() & 0x7F) + ROT(309.375);
        if (MARIA.facingLeft == true) {
            self->ext.holywater.angle = (rand() & 0x7F) + ROT(219.375);
        }
        self->velocityX =
            ((rcos(self->ext.holywater.angle) << 4) * FIX(3.0 / 128.0)) >>
            8;
        self->velocityY =
            -((rsin(self->ext.holywater.angle) << 4) * FIX(3.0 / 128.0)) >>
            8;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        self->ext.holywater.unk80 = 0x200;
        self->step = 1;
        break;
    case 1:
        self->posY.val += self->velocityY;
        colRes = func_pspeu_09267A60(0, 0);
        self->posX.val += self->velocityX;
        if ((colRes & EFFECT_SOLID) || self->hitFlags) {
            MarCreateEntFactoryFromEntity(self, BP_HOLYWATER_GLASS, 0);
            g_api.PlaySfx(SFX_RIC_HOLY_WATER_ATTACK);
            self->animSet = 0;
            self->velocityX = self->velocityX >> 2;
            self->ext.holywater.timer = 80;
            self->step = 3;
        } else if (self->flags & FLAG_DEAD) {
            MarCreateEntFactoryFromEntity(self, BP_HOLYWATER_GLASS, 0);
            g_api.PlaySfx(SFX_RIC_HOLY_WATER_ATTACK);
            self->animSet = 0;
            self->velocityX = self->velocityX >> 2;
            self->velocityX = -self->velocityX;
            self->ext.holywater.timer = 80;
            self->step = 3;
        }
        break;
    case 2:
        if (self->flags & FLAG_DEAD) {
            DestroyEntity(self);
            return;
        }
        if (--self->ext.holywater.timer == 0) {
            self->velocityX = self->velocityX >> 2;
            self->ext.holywater.timer = 80;
            self->step++;
        }
        break;
    case 3:
        if (self->flags & FLAG_DEAD) {
            self->velocityX = 0;
        }
        if (!(self->ext.holywater.timer & 3)) {
            MarCreateEntFactoryFromEntity(
                self, FACTORY(BP_HOLYWATER_FIRE, self->ext.holywater.unk82), 0);
            self->ext.holywater.unk82 += 1;
            self->velocityX -= (self->velocityX / 32);
        }
        self->posX.val += self->velocityX;
        colRes = func_pspeu_09267A60(6, 0);
        if (!(colRes & EFFECT_SOLID)) {
            self->velocityX = self->velocityX >> 1;
            self->step++;
        }
        break;
    case 4:
        if (self->flags & FLAG_DEAD) {
            self->velocityX = 0;
        }
        if (!(self->ext.holywater.timer & 3)) {
            MarCreateEntFactoryFromEntity(
                self, FACTORY(BP_HOLYWATER_FIRE, self->ext.holywater.unk82), 0);
            self->ext.holywater.unk82 += 1;
        }
        self->velocityY += FIX(12.0 / 128);
        if (self->velocityY > FIX(4)) {
            self->velocityY = FIX(4);
        }
        self->posY.val += self->velocityY;
        colRes = func_pspeu_09267A60(0, 0);
        self->posX.val += self->velocityX;
        xMod = 4;
        if (self->velocityX < 0) {
            xMod = -xMod;
        }
        colRes |= func_801B2E5C(-7, xMod);
        if (colRes & EFFECT_SOLID) {
            self->velocityX = self->velocityX << 1;
            self->step--;
        }
        break;
    case 5:
        break;
    }
    if (self->step > 2) {
        if (--self->ext.holywater.timer < 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.holywater.timer == 2) {
            self->step = 5;
        }
        self->posY.i.hi -= 5;
        self->animCurFrame = 0;
    }
    g_Maria.timers[PL_T_3] = 2;
    self->hitFlags = 0;
    self->flags &= ~FLAG_DEAD;
    FntPrint("judge:%02x\n", self->hitboxState);
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09268448);

void func_pspeu_09268D98(Entity* self) {
    s16 psp_s4;
    s16 psp_s3;
    s16 right;
    s16 left;
    Primitive* prim;
    psp_s4 = 3;
    psp_s3 = 1;
    self->posY.i.hi = 0x78;
    self->posX.i.hi = MARIA.posX.i.hi;
    switch (self->step) {
    case 0:
        self->ext.crashcross.subweaponId = PL_W_CRASH_CROSS;
        InitializeEntity(D_pspeu_0926EC08);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->ext.crashcross.unk80 = 1;
        self->zPriority = 0xC2;
        LoadImage(&D_pspeu_0927B018, (u_long*)D_pspeu_0927AFB8);
        g_api.PlaySfx(SFX_CRASH_CROSS);
        g_api.PlaySfx(SFX_TELEPORT_BANG_B);
        self->step = 1;
        break;
    case 1:
        self->ext.crashcross.unk7E.val += psp_s4;
        self->ext.crashcross.unk82 += psp_s4 * 2;
        if (self->ext.crashcross.unk7E.i.lo >= 0x70) {
            MarCreateEntFactoryFromEntity(self, BP_CRASH_CROSSES_ONLY, 0);
            MarCreateEntFactoryFromEntity(self, BP_CRASH_CROSS_PARTICLES, 0);
            self->step++;
        }
        break;
    case 2:
        if (g_Timer & 1) {
            self->ext.crashcross.unk7C += psp_s3;
            self->ext.crashcross.unk80 += psp_s3 * 2;
            if (self->ext.crashcross.unk80 >= 0x2C) {
                self->step++;
                self->ext.crashcross.unk84 = 0x80;
            }
        }
        break;
    case 3:
        if (--self->ext.crashcross.unk84 == 0) {
            g_api.SetFadeMode(FADE_NONE);
            left = self->posX.i.hi - self->ext.crashcross.unk7C;
            if (left < 0) {
                left = 0;
            }
            right = self->posX.i.hi + self->ext.crashcross.unk7C;
            if (right > 0xFF) {
                right = 0xFF;
            }
            g_api.PlaySfx(SFX_WEAPON_APPEAR);
            self->step++;
        }
        break;
    case 4:
        psp_s3 *= 3;
        left = abs(self->posX.i.hi - 0x80);
        psp_s3 = psp_s3 * (left + 0x80) / 112;
        self->ext.crashcross.unk7C += psp_s3;
        left = self->posX.i.hi - self->ext.crashcross.unk7C;
        if (left < 0) {
            left = 0;
        }
        right = self->posX.i.hi + self->ext.crashcross.unk7C;
        if (right > 0xFF) {
            right = 0xFF;
        }
        if (right - left > 0xF8) {
            g_Maria.unk4E = 1;
            DestroyEntity(self);
            return;
        }
        break;
    }
    self->hitboxOffY = 0;
    self->hitboxHeight = self->ext.crashcross.unk7E.val;
    if (self->step == 4) {
        self->hitboxWidth = (right - left) >> 1;
        self->hitboxOffX = ((left + right) >> 1) - self->posX.i.hi;
    } else {
        self->hitboxWidth = self->ext.crashcross.unk7C;
        self->hitboxOffX = 0;
    }
    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = prim->x2 = self->posX.i.hi - self->ext.crashcross.unk7C;
    prim->y1 = prim->y0 = self->posY.i.hi - self->ext.crashcross.unk7E.val;
    prim->x1 = prim->x3 = prim->x0 + self->ext.crashcross.unk80;
    prim->y2 = prim->y3 = prim->y0 + self->ext.crashcross.unk82;
    prim->u0 = prim->u2 = 1;
    prim->u1 = prim->u3 = 0x30;
    prim->v0 = prim->v1 = prim->v2 = prim->v3 = 0xF8;
    prim->tpage = 0x11C;
    if (self->step == 4) {
        prim->x0 = prim->x2 = left;
        prim->x1 = prim->x3 = right;
    }
    prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
    prim->priority = self->zPriority;
    g_Maria.timers[PL_T_3] = 2;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_09269428);

void func_pspeu_09269E60(Entity* self) {
    Primitive* prim;

    if (self->step == 0) {
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->velocityY = FIX(0.5);
        self->posX.i.hi += (rand() & 0xF) - 8;
        self->posY.i.hi += (rand() & 0xF) - 4;
        prim = &g_PrimBuf[self->primIndex];
        prim->clut = PAL_UNK_1B0;
        prim->tpage = 0x1A;
        prim->b0 = 0;
        prim->b1 = 0;
        prim->priority = self->zPriority + 4;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
        func_8015FDB0(prim, self->posX.i.hi, self->posY.i.hi);
        self->step++;
    } else {
        prim = &g_PrimBuf[self->primIndex];
        self->posY.val += self->velocityY;
        if (func_8015FDB0(prim, self->posX.i.hi, self->posY.i.hi)) {
            DestroyEntity(self);
        }
    }
}

// local copy of RicEntitySubwpnCrossTrail (see us_3E79C.c)
void func_pspeu_0926A048(Entity* self) {
    s16* temp;

    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        // the parent pointer is set by the entity factory.
        // the value of unk84 is set by the cross subweapon entity
        self->ext.crossBoomerang.unk84 =
            self->ext.crossBoomerang.parent->ext.crossBoomerang.unk84;
        self->animSet = ANIMSET_OVL(4);
        self->animCurFrame = D_pspeu_0927B048[self->params];
        self->unk5A = 0x44;
        self->palette = PAL_FLAG(PAL_UNK_1B0);
        self->blendMode = BLEND_TRANSP;
        self->facingLeft = RIC.facingLeft;
        self->zPriority = RIC.zPriority;
        self->drawFlags = ENTITY_ROTATE;
        self->rotate = ROT(270);
        self->step++;
        break;
    case 1:
        self->rotate -= ROT(11.25);
        if (self->ext.crossBoomerang.parent->step == 7 ||
            self->ext.crossBoomerang.parent->step == 6) {
            self->step++;
            self->ext.crossBoomerang.timer = (self->params + 1) * 4;
        }
        break;
    case 2:
        self->rotate -= ROT(11.25);
        if (--self->ext.crossBoomerang.timer == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    // get the x and y position from the parent (must align)
    temp = (s16*)&self->ext.crossBoomerang.unk84[0];
    temp += self->ext.crossBoomerang.unk80 * 2;
    self->posX.i.hi = *temp - g_Tilemap.scrollX.i.hi;
    temp++;
    self->posY.i.hi = *temp - g_Tilemap.scrollY.i.hi;
    self->ext.crossBoomerang.unk80++;
    self->ext.crossBoomerang.unk80 &= 0x3F;
}

void func_pspeu_0926A2B8(Entity* self) {
    Primitive* prim;
    s16 psp_s4;
    s16 psp_s3;
    s16 psp_s2;
    u8* psp_s1;
    s16 temp_s0;
    if (self->step == 0) {
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->ext.timer.t = 0xC0;
        self->step++;
        return;
    }
    if (--self->ext.timer.t == 0) {
        DestroyEntity(self);
        return;
    }
    if (self->ext.timer.t > 8 && !(self->ext.timer.t & 3)) {
        for (prim = &g_PrimBuf[self->primIndex]; prim; prim = prim->next) {
            if (prim->r0 == 0) {
                prim->r0 = 1;
                prim->r1 = 0;
                break;
            }
        }
    }
    for (prim = &g_PrimBuf[self->primIndex]; prim; prim = prim->next) {
        if (!prim->r0) {
            continue;
        }
        if (prim->r1 == 0) {
            temp_s0 = rand() & 0x3F;
            prim->g0 = (rand() % 237) + 9;
            prim->g1 = 0xF0 - (rand() & 0x20);
            prim->clut = PAL_UNK_1B0;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->priority = MARIA.zPriority + temp_s0 - 0x20;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_TRANSP;
            prim->g3 = ((u8)temp_s0 >> 2) + 4;
            prim->r1++;
        } else {
            prim->g1 -= prim->g3;
            if ((u8)prim->b0 >= 6 || (u8)prim->g1 < 0x18) {
                prim->drawMode = DRAW_HIDE;
                prim->r0 = 0;
            }
        }
        if (!prim->r0) {
            continue;
        }
        psp_s4 = 0;
        psp_s3 = 0;
        psp_s4 |= prim->g0;
        psp_s3 |= prim->g1;
        psp_s1 = D_pspeu_09279A40[0];
        psp_s1 += prim->b0 * 8;
        if (prim->b0 >= 3) {
            psp_s2 = 4;
        } else {
            psp_s2 = 8;
        }
        prim->x0 = psp_s4 - psp_s2;
        prim->y0 = psp_s3 - psp_s2;
        prim->x1 = psp_s4 + psp_s2;
        prim->y1 = psp_s3 - psp_s2;
        prim->x2 = psp_s4 - psp_s2;
        prim->y2 = psp_s3 + psp_s2;
        prim->x3 = psp_s4 + psp_s2;
        prim->y3 = psp_s3 + psp_s2;
        prim->u0 = *psp_s1;
        psp_s1++;
        prim->v0 = *psp_s1;
        psp_s1++;
        prim->u1 = *psp_s1;
        psp_s1++;
        prim->v1 = *psp_s1;
        psp_s1++;
        prim->u2 = *psp_s1;
        psp_s1++;
        prim->v2 = *psp_s1;
        psp_s1++;
        prim->u3 = *psp_s1;
        psp_s1++;
        prim->v3 = *psp_s1;
        psp_s1++;
        if (!(g_GameTimer & 1)) {
            prim->b0++;
        }
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_25C90", func_pspeu_0926A738);
