// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

// Maria fights alongside the player in this overlay; her state mirrors cen.h
#define MARIA g_Entities[STAGE_ENTITY_START]
#define MARIA_AFTERIMAGE_1 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
#define MARIA_AFTERIMAGE_2 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_2]
#define MARIA_AFTERIMAGE_3 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_3]
#define MAR_DRAW_AFTER_IMAGE_SLOT 9

extern PlayerState g_Maria;

extern EInit D_pspeu_0926EB98;            // slide kick hitbox entity init
extern AnimationFrame D_pspeu_0927A418[]; // rising sparkle anim
extern AnimationFrame D_pspeu_0927A5C0[]; // Maria's stop-run anim

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetBladeDash(void);
void MarSetAnimation(AnimationFrame* anim);
void MarDecelerateX(s32 speed);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
bool func_pspeu_0925B680(s32 checks);
void func_pspeu_09259AD8(s32 velocityX);
void func_pspeu_09259D48(void);
void func_pspeu_09259C58(void);
void func_pspeu_0925AA50(void);
void MarSetStep(s16 step);
void MarSetCrouch(s32 kind, s32 velocityX);
void func_maria_8015CC28(void);
void func_pspeu_0924EA50_from_rbo5(s32 velocityX);
s32 func_pspeu_09248D20(SubweaponDef* subweapon, s32 isItemCrash, s32 useHearts);
s32 MarCheckSubwpnChainLimit(s16 subwpnId, s16 limit);

// Maria's anim tables live in the raw data blobs of the overlay
extern AnimationFrame D_pspeu_0927A5E0[]; // stand
extern AnimationFrame D_pspeu_0927A6B0[]; // walk
extern AnimationFrame D_pspeu_0927A6D8[]; // crouch from stand 2
extern AnimationFrame D_pspeu_0927A6E8[]; // crouch
extern AnimationFrame D_pspeu_0927A6F0[]; // land from air run
extern AnimationFrame D_pspeu_0927A6F8[]; // crouch from stand
extern AnimationFrame D_pspeu_0927A718[]; // jump neutral
extern AnimationFrame D_pspeu_0927A738[]; // jump moving
extern AnimationFrame D_pspeu_0927A768[]; // fall
extern AnimationFrame D_pspeu_0927A790[]; // slide kick escape jump
extern AnimationFrame D_pspeu_0927A820[]; // subweapon stand
extern AnimationFrame D_pspeu_0927A870[]; // subweapon jump
extern AnimationFrame D_pspeu_0927A8A8[]; // run
extern AnimationFrame D_pspeu_0927A8D0[]; // death
extern AnimationFrame D_pspeu_0927A988[]; // slide
extern AnimationFrame D_pspeu_0927A9B8[]; // slide kick
extern AnimationFrame D_pspeu_0927A9C8[]; // slide kick bounce
extern AnimationFrame D_pspeu_0927A9E0[]; // high jump
extern AnimationFrame D_pspeu_0927AA20[]; // blade dash
extern s32 D_pspeu_0927C760; // throw dagger timer (raw bss)

// local copy of RicGetFreeEntity (see us_39144.c)
Entity* MarGetFreeEntity(s16 start, s16 end) {
    Entity* entity = &g_Entities[start];
    s16 i;

    for (i = start; i < end; i++, entity++) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

// local copy of RicGetFreeEntityReverse (see us_39144.c)
Entity* MarGetFreeEntityReverse(s16 start, s16 end) {
    Entity* entity = &g_Entities[end - 1];
    s16 i;
    for (i = end - 1; i >= start; i--, entity--) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

extern u8 D_pspeu_092797D0[][4];
extern u8 D_pspeu_0927C230;
extern u8 D_pspeu_0927C228;
extern u8 D_pspeu_0927C220;
extern u8 D_pspeu_0927C218;
// local copy of func_us_801BB314 (see bo6/us_39144.c)
void func_pspeu_09252480_from_rbo5(s32 arg0) {
    D_pspeu_0927C230 = D_pspeu_092797D0[arg0][0];
    D_pspeu_0927C228 = D_pspeu_092797D0[arg0][1];
    D_pspeu_0927C220 = D_pspeu_092797D0[arg0][2];
    D_pspeu_0927C218 = D_pspeu_092797D0[arg0][3];
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09248D20);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_8015FDB0);

s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY);
extern Point16 D_pspeu_0927C1D8[16];
// local copy of RicEntityHitByHoly (see ric/pl_blueprints.c); the boss
// walls track the doppleganger in slot 64 and camera-lock the effect
void func_pspeu_09252768_from_rbo5(Entity* entity) {
    Primitive* prim;
    s32 i;
    s32 temp;
    s16 hitboxX;
    s16 hitboxY;
    s16 temp_xRand;
    s16 temp_yRand;

    switch (entity->step) {
    case 0:
        entity->primIndex =
            (s16)g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_0927C1D8));
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        hitboxX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        hitboxY = MARIA.posY.i.hi + MARIA.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < LEN(D_pspeu_0927C1D8); i++) {
            temp_xRand = hitboxX + rand() % 24 - 12;
            temp_yRand = hitboxY + rand() % 48 - 24;
            D_pspeu_0927C1D8[i].x = temp_xRand;
            D_pspeu_0927C1D8[i].y = temp_yRand;
            prim->clut = PAL_UNK_1B2;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->b1 = 0;
            prim->g0 = 0;
            prim->g1 = (rand() & 7) + 1;
            prim->g2 = 0;
            prim->priority = MARIA.zPriority + 4;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE | DRAW_HIDE |
                             DRAW_UNK02 | DRAW_TRANSP;
            if (rand() & 1) {
                prim->drawMode =
                    DRAW_UNK_100 | DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                    DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }
        entity->step++;
        break;
    case 1:
        if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
            DestroyEntity(entity);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[entity->primIndex];
    for (i = 0; i < 16; i++) {
        switch (prim->g0) {
        case 0:
            if (--prim->g1 == 0) {
                prim->g0++;
            }
            break;
        case 1:
            hitboxX = D_pspeu_0927C1D8[i].x;
            hitboxY = D_pspeu_0927C1D8[i].y;
            temp = func_8015FDB0(prim, hitboxX, hitboxY);
            D_pspeu_0927C1D8[i].y--;
            if (temp < 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->g0++;
            } else {
                prim->drawMode &= ~DRAW_HIDE;
            }
            break;
        }
        prim = prim->next;
    }
}

extern AnimationFrame D_pspeu_09279A70[];
extern s32 D_pspeu_0927C1D0;
// local copy of RicEntityHitByDark (see ric/pl_blueprints.c); the boss
// walls use different flags and track the doppleganger's z-priority
void func_pspeu_09253EA0_from_rbo5(Entity* entity) {
    s16 x, y;

    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        entity->unk5A = 0x79;
        entity->animSet = ANIMSET_DRA(14);
        entity->zPriority = MARIA.zPriority + 2;
        entity->palette = PAL_FLAG(PAL_UNK_19F);
        if (D_pspeu_0927C1D0 & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        D_pspeu_0927C1D0++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_pspeu_09279A70;
        entity->posY.i.hi += (rand() % 35) - 15;
        entity->posX.i.hi += (rand() % 20) - 10;
        entity->velocityY = -0x6000 - (rand() & 0x3FFF);
        entity->step++;
        break;
    case 1:
        if (entity->opacity > 16) {
            entity->opacity -= 8;
        }
        entity->posY.val += entity->velocityY;
        entity->scaleX += 8;
        entity->scaleY += 8;
        if (entity->poseTimer < 0) {
            DestroyEntity(entity);
        }
        break;
    }
}

// declaring the ignored arg reproduces mwcc's 0x10 stack frame
void Unused09249778(s32 arg0) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09252B48_from_rbo5);

// local copy of RicCreateEntFactoryFromEntity (see us_39144.c); note the
// (68, 80) slot range - cen's Maria uses (72, 80)
Entity* MarCreateEntFactoryFromEntity(
    Entity* source, u32 factoryParams, s32 arg2) {
    Entity* entity = MarGetFreeEntity(68, 80);
    if (!entity) {
        return NULL;
    }
    DestroyEntity(entity);
    entity->entityId = E_FACTORY;
    // the parent pointer must align for anything the factory creates
    entity->ext.factory.parent = source;
    entity->posX.val = source->posX.val;
    entity->posY.val = source->posY.val;
    entity->facingLeft = source->facingLeft;
    entity->zPriority = source->zPriority;
    entity->params = factoryParams & 0xFFF;
    entity->ext.factory.paramsBase = (factoryParams & 0xFF0000) >> 8;
    return entity;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityFactory);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntitySlideKick);

// local copy of func_us_801BC3E0 (see us_39144.c) - slide kick hitbox
void func_pspeu_0924A2E0(Entity* self) {
    if (MARIA.step != PL_S_SLIDE_KICK) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->facingLeft = MARIA.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_pspeu_0926EB98);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0x14;
        self->hitboxWidth = self->hitboxHeight = 9;
        self->step = 1;
    }

    if (MARIA.animCurFrame == 140) {
        self->hitboxOffY = 0;
    }

    if (MARIA.animCurFrame == 141) {
        self->hitboxOffY = 12;
    }

    if (self->hitFlags) {
        g_Maria.unk44 |= 0x80;
    } else {
        g_Maria.unk44 &= ~0x80;
    }
    self->hitFlags = 0;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A448);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A578);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A660);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByCutBlood);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_maria_80161C2C);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924B8D0);

// local copy of func_us_801BD47C (see us_39144.c) - true when another
// live entity shares this id and params
bool func_maria_80162E9C(Entity* entity) {
    Entity* e;
    s32 i;
    s16 objId;
    s16 params;

    objId = entity->entityId;
    params = entity->params;
    for (e = &g_Entities[0x50], i = 0x50; i < 0x90; e++, i++) {
        if (objId == e->entityId && params == e->params && e != entity) {
            return true;
        }
    }
    return false;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityPlayerBlinkWhite);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924D7F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityShrinkingPowerUpRing);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByIce);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByLightning);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924FFB8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09250DA8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251100);

// local copy of func_us_801C13A8 (see us_3E79C.c) - rising sparkle
void func_pspeu_092516D0(Entity* self) {
    s16 params = self->params & 0x7F00;
    switch (self->step) {
    case 0:
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->unk5A = 0x79;
        self->animSet = ANIMSET_DRA(14);
        self->zPriority = MARIA.zPriority + 6;
        self->palette = PAL_FLAG(0x25E);
        self->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0xC0;
        self->anim = D_pspeu_0927A418;
        if (params) {
            self->scaleX = self->scaleY = 0x80;
            self->anim = D_pspeu_0927A418;
        }
        self->velocityY = -FIX(0.25);
        self->step++;
        break;

    case 1:
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251850);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092522A8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251748_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253AA8);

// local copy of func_us_801B4EAC (see richter.c) - integrate velocity
// and clamp to the arena walls
void func_pspeu_09253E08(void) {
    g_Maria.unk04 = g_Maria.vram_flag;
    g_Maria.vram_flag = 0;
    MARIA.posY.val += MARIA.velocityY;
    MARIA.posX.val += MARIA.velocityX;

    if (MARIA.posY.val >= 0xB30000) {
        MARIA.posY.val = 0xB30000;
        g_Maria.vram_flag |= TOUCHING_GROUND;
    }
    if (MARIA.posY.val <= 0x280000) {
        MARIA.posY.val = 0x280000;
        g_Maria.vram_flag |= TOUCHING_CEILING;
    }
    if (MARIA.posX.val >= 0xF80000) {
        MARIA.posX.val = 0xF80000;
        g_Maria.vram_flag |= TOUCHING_R_WALL;
    }
    if (MARIA.posX.val <= 0x80000) {
        MARIA.posX.val = 0x80000;
        g_Maria.vram_flag |= TOUCHING_L_WALL;
    }
}

// local copy of static CheckBladeDashInput (see richter.c); non-static
// so the remaining INCLUDE_ASM stubs can reference the symbol
void func_pspeu_09253F48(void) {
    if ((MARIA.step == 1 || MARIA.step == 2 || MARIA.step == 3 ||
         MARIA.step == 5 || MARIA.step == 4) &&
        !g_Maria.unk46 && (g_Maria.padTapped & PAD_R1)) {
        MarSetBladeDash();
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09254008);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092540C8);

extern s32 D_pspeu_0927C278;
extern s32 D_pspeu_0927C280;
// local copy of func_us_801B5A14 (see bo6/richter.c): set the boss
// think-step and reset its timer
void func_pspeu_09247190_from_rbo5(s32 step) {
    D_pspeu_0927C280 = step;
    D_pspeu_0927C278 = 0;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09254F38);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09256610);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09256778);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092568D0);

// local copy of RicStepWalk (see ric/pl_steps.c) driving Maria
void func_pspeu_09256BA0(void) {
    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP |
                             CHECK_CRASH | CHECK_ATTACK | CHECK_CROUCH)) {
        MarDecelerateX(FIX(0.125));
        if (MarCheckFacing() == 0) {
            func_pspeu_09259AD8(0);
            return;
        }
        if (MARIA.step_s != 0) {
            if (MARIA.step_s) {
            }
        } else {
            MarSetSpeedX(FIX(1.25));
        }
    }
}

// local copy of RicStepRun (see ric/pl_steps.c) driving Maria
void func_pspeu_09256C28(void) {
    g_Maria.timers[PL_T_8] = 8;
    g_Maria.timers[PL_T_CURSE] = 8;
    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP |
                             CHECK_CRASH | CHECK_ATTACK | CHECK_CROUCH)) {
        MarDecelerateX(FIX(0.125));
        if (MarCheckFacing() == 0) {
            func_pspeu_09259AD8(0);
            if (g_Maria.timers[PL_T_RUN] == 0) {
                if (!(g_Maria.vram_flag &
                      (TOUCHING_L_WALL | TOUCHING_R_WALL))) {
                    MarSetAnimation(D_pspeu_0927A5C0);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, BP_SKID_SMOKE, 0);
                }
            } else {
                MARIA.velocityX = 0;
            }
            return;
        }
        if (MARIA.step_s != 0) {
            if (MARIA.step_s) {
            }
        } else {
            MarSetSpeedX(FIX(2.25));
        }
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09256D28);

// local copy of RicStepFall (see boss/bo6/richter.c)
void MarStepFall(void) {
    if (func_pspeu_0925B680(
            CHECK_GROUND | CHECK_FACING | CHECK_ATTACK | CHECK_GRAVITY_FALL)) {
        return;
    }
    MarDecelerateX(FIX(1. / 16));
    switch (MARIA.step_s) {
    case 0:
        if (g_Maria.timers[PL_T_5] && g_Maria.padTapped & PAD_CROSS) {
            func_pspeu_09259C58();
        } else if (MarCheckFacing()) {
            MarSetSpeedX(FIX(0.75));
        }
        break;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257418);

void MarResetPose(void) {
    MARIA.pose = MARIA.poseTimer = 0;
    g_Maria.unk44 = 0;
    g_Maria.unk46 = 0;
    MARIA.drawFlags &= ~ENTITY_ROTATE;
}

// local copy of func_us_801B77D8 (see boss/bo6/richter.c)
void func_pspeu_092577D8(void) {
    if ((MARIA.posX.i.hi - PLAYER.posX.i.hi) <= 0) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257820);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258010);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258830);

// local copy of RicStepEnableFlameWhip (see ric/pl_steps.c); the bo6
// variant drops the trailing fall check
void func_pspeu_092588E8(void) {
    if (MARIA.animCurFrame == 181 && MARIA.poseTimer == 1) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_35, 0);
        g_api.PlaySfx(SFX_WEAPON_APPEAR);
    }

    if (MARIA.poseTimer < 0) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x45), 0);
        g_Maria.timers[PL_T_POISON] = 0x800;
    }
}

// local copy of the bo6 RicStepHydrostorm (see boss/bo6/us_39144.c)
void func_pspeu_092589B0(void) {
    if (MARIA.poseTimer < 0) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
    }
}

// local copy of RicStepGenericSubwpnCrash minus the fall check (bo6 variant)
void func_pspeu_092589F0(void) {
    if (g_Maria.unk4E) {
        func_pspeu_09259AD8(0);
        g_Maria.unk46 = 0;
    }
}

// local copy of RicStepThrowDaggers (see ric/pl_steps.c) minus the fall
// check; the timer lives in the wall's raw bss
void func_pspeu_09258A30(void) {
    if (MARIA.step_s == 0) {
        D_pspeu_0927C760 = 0x200;
        MARIA.step_s++;
    } else {
        MarCheckFacing();
        if (!--D_pspeu_0927C760) {
            g_Maria.unk46 = 0;
            func_pspeu_09259AD8(0);
            g_Maria.unk4E = 1;
        }
    }
    if (g_Maria.padTapped & PAD_CROSS) {
        func_pspeu_09259C58();
        g_Maria.unk46 = 0;
        g_Maria.unk4E = 1;
        D_pspeu_0927C760 = 0;
    }
}

// local copy of RicStepSlide (see boss/bo6/richter.c); reads the real
// player's floor sensors via g_Player
void MarStepSlide(void) {
    Collider collider;
    s32 isTouchingGround = 0;

    if (MARIA.facingLeft == 0 && g_Maria.vram_flag & TOUCHING_R_WALL) {
        isTouchingGround = 1;
    }
    if (MARIA.facingLeft && g_Maria.vram_flag & TOUCHING_L_WALL) {
        isTouchingGround = 1;
    }
    if (MARIA.posX.i.hi >= STAGE_WIDTH - 4 && MARIA.facingLeft == 0) {
        isTouchingGround = 1;
    }
    if (MARIA.posX.i.hi <= 4 && MARIA.facingLeft) {
        isTouchingGround = 1;
    }
    if ((MARIA.facingLeft == 0 &&
         g_Player.colFloor[2].effects & EFFECT_UNK_8000) ||
        (MARIA.facingLeft && g_Player.colFloor[3].effects & EFFECT_UNK_8000)) {
        isTouchingGround = 1;
    }
    if (isTouchingGround && MARIA.pose < 6) {
        MARIA.pose = 6;
        if (MARIA.velocityX > FIX(1)) {
            MARIA.velocityX = FIX(2);
        }
        if (MARIA.velocityX < FIX(-1)) {
            MARIA.velocityX = FIX(-2);
        }
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
    }
    if (MARIA.pose < 5) {
        if (func_pspeu_0925B680(CHECK_FALL | CHECK_CRASH)) {
            return;
        }
        if (g_Maria.padTapped & PAD_CROSS) {
            MARIA.posY.i.hi -= 4;
            func_pspeu_0925AA50();
            return;
        }
    } else if (MARIA.pose < 7) {
        if (func_pspeu_0925B680(CHECK_FALL | CHECK_CRASH | CHECK_SLIDE)) {
            return;
        }
    } else if (func_pspeu_0925B680(
                   CHECK_FALL | CHECK_FACING | CHECK_CRASH | CHECK_SLIDE)) {
        return;
    }

    MarDecelerateX(FIX(0.125));
    switch (MARIA.step_s) {
    case 0:
        if (!(g_GameTimer & 3) && MARIA.pose < 6 && MARIA.pose > 2) {
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_SLIDE, 2), 0);
        }
        if (MARIA.pose == 6 && MARIA.poseTimer == 1) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
        }
        if (MARIA.poseTimer < 0) {
            MarSetCrouch(0, MARIA.velocityX);
        }
        break;
    }
}

// local copy of RicStepSlideKick (see boss/bo6/richter.c)
void func_pspeu_09258E60(void) {
    if (g_Maria.padPressed & PAD_SQUARE && g_Maria.unk44 & 0x80) {
        MARIA.step = PL_S_JUMP;
        MarSetAnimation(D_pspeu_0927A790);
        MarSetSpeedX(FIX(-1.5));
        MARIA.velocityY = FIX(-3.5);
        g_Maria.unk44 |= (8 + 2);
        g_Maria.unk44 &= ~4;
        MARIA.step_s = 2;
        return;
    }
    MarDecelerateX(FIX(0.0625));
    MARIA.velocityY += 0x1000;

    if (g_Maria.vram_flag & TOUCHING_GROUND) {
        g_CurrentEntity->velocityX /= 2;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
        MARIA.facingLeft++;
        MARIA.facingLeft &= 1;
        MarSetCrouch(3, MARIA.velocityX);
        g_api.PlaySfx(SFX_STOMP_SOFT_A);
        return;
    }
    if (MARIA.velocityX < 0) {
        if (g_Maria.padPressed & PAD_RIGHT) {
            MarDecelerateX(FIX(0.125));
        }
        if (MARIA.velocityX > FIX(-3) ||
            (g_Maria.vram_flag & TOUCHING_L_WALL)) {
            MARIA.facingLeft++;
            MARIA.facingLeft &= 1;
            MARIA.velocityX /= 2;
            MarSetAnimation(D_pspeu_0927A9C8);
            g_Maria.unk44 = 0xA;
            MARIA.step_s = 2;
            MARIA.step = PL_S_JUMP;
        }
    }
    if (MARIA.velocityX > 0) {
        if (g_Maria.padPressed & PAD_LEFT) {
            MarDecelerateX(FIX(0.125));
        }
        if (MARIA.velocityX < FIX(3) ||
            (g_Maria.vram_flag & TOUCHING_R_WALL)) {
            MARIA.facingLeft++;
            MARIA.facingLeft &= 1;
            MARIA.velocityX /= 2;
            MarSetAnimation(D_pspeu_0927A9C8);
            g_Maria.unk44 = 0xA;
            MARIA.step_s = 2;
            MARIA.step = PL_S_JUMP;
        }
    }
}

void MarStepBladeDash(void) {
    MarDecelerateX(FIX(7.0 / 64));
    if (MARIA.poseTimer < 0) {
        g_Maria.unk46 = 0;
        func_pspeu_09259AD8(0);
        return;
    }

    if (MARIA.pose >= 0x12 && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        g_Maria.unk46 = 0;
        func_pspeu_09259D48();
        return;
    }

    if (!(g_GameTimer & 3) && MARIA.pose < 0x12 &&
        (g_Maria.vram_flag & TOUCHING_GROUND)) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_SLIDE, 2), 0);
    }

    if (MARIA.pose == 18 && MARIA.poseTimer == 1 &&
        (g_Maria.vram_flag & TOUCHING_GROUND)) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_SKID_SMOKE, 0);
    }
}

// matches cen's func_80158B04 (see st/cen_psp/e_maria_step.c)
void func_80158B04(u16 arg0) {
    s16 xMod = 3;
    if (MARIA.facingLeft) {
        xMod = -xMod;
    }

    MARIA.posY.i.hi -= 16;
    MARIA.posX.i.hi += xMod;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_EMBERS, 1), 0);
    MARIA.posY.i.hi += 16;
    MARIA.posX.i.hi -= xMod;

    if (arg0 & 1) {
        g_api.func_80102CD8(3);
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
    }

    if (arg0 & 2) {
        MARIA.velocityX = 0;
        MARIA.velocityY = 0;
    }
}

// local copy of RicStepHighJump (see ric/pl_steps.c) minus the US debug
// prints
void func_pspeu_09259418(void) {
    bool loadAnim;

    loadAnim = false;
    g_Maria.high_jump_timer++;
    switch (MARIA.step_s) {
    case 0:
        if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
            if (MARIA.facingLeft) {
                if (!(g_Maria.padPressed & PAD_LEFT)) {
                    MarDecelerateX(FIX(0.0625));
                }
            } else {
                if (!(g_Maria.padPressed & PAD_RIGHT)) {
                    MarDecelerateX(FIX(0.0625));
                }
            }
        } else {
            MarDecelerateX(FIX(0.0625));
        }

        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            func_80158B04(3);
            g_Maria.high_jump_timer = 0;
            MARIA.step_s = 2;
        } else if (g_Maria.high_jump_timer > 0x1C) {
            MARIA.step_s = 1;
            MARIA.velocityY = -0x60000;
        }
        break;
    case 1:
        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            MARIA.step_s = 2;
            func_80158B04(3);
            g_Maria.high_jump_timer = 0;
        } else {
            MARIA.velocityY += 0x6000;
            if (MARIA.velocityY > 0x8000) {
                loadAnim = true;
            }
        }
        break;
    case 2:
        if (g_Maria.high_jump_timer > 4) {
            loadAnim = true;
        }
        break;
    }

    if (loadAnim) {
        MarSetAnimation(D_pspeu_0927A768);
        MarSetStep(PL_S_JUMP);
    }
}

void MarLoadGraphics_09259630(void) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259638);

void MarLoadGraphics_09259858(void) {}

// local copy of func_us_801B9340 (see boss/bo6/us_39144.c)
void func_pspeu_09259860(void) {
    switch (MARIA.step_s) {
    case 0:
        MarResetPose();
        MARIA.velocityY = FIX(-5);
        func_pspeu_0924EA50_from_rbo5(0xFFFF1000);
        MARIA.anim = D_pspeu_0927A8D0;
        g_api.PlaySfx(SFX_BOSS_RIC_DEATH);
        g_Maria.damagePalette = 0x8166;
        g_Maria.timers[2] = 8;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(E_ID_21, 0x58), 0);
        MARIA.step_s += 1;
        return;
    case 1:
        if ((g_Maria.vram_flag & TOUCHING_CEILING) &&
            (FIX(-1) > MARIA.velocityY)) {
            MARIA.velocityY = FIX(-1);
        }
        if (func_pspeu_0925B680(0x20280) != 0) {
            MARIA.step = 0x70;
            MARIA.step_s = 2;
        }
        break;
    case 2:
        MarDecelerateX(FIX(0.125));
        if ((PLAYER.posX.i.hi - MARIA.posX.i.hi) > 0) {
            MARIA.facingLeft = 0;
            return;
        }
        MARIA.facingLeft = 1;
        break;
    }
}

// local copy of RicSetCrouch (see ric/pl_setstep.c)
void MarSetCrouch(s32 kind, s32 velocityX) {
    MarSetStep(PL_S_CROUCH);
    MarSetAnimation(D_pspeu_0927A6E8);
    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    if (kind == 1) {
        MARIA.anim = D_pspeu_0927A6D8;
        MARIA.step_s = 4;
    }
    if (kind == 2) {
        MARIA.anim = D_pspeu_0927A6F8;
        MARIA.step_s = 1;
    }
    if (kind == 3) {
        MARIA.anim = D_pspeu_0927A6F0;
        MARIA.step_s = 4;
    }
}

// local copy of RicSetStand (see boss/bo6/us_39144.c)
void func_pspeu_09259AD8(s32 velocityX) {
    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_STAND);
    MarSetAnimation(D_pspeu_0927A5E0);
}

// local copy of func_us_801B9D74 (see boss/bo6/us_39144.c)
void func_pspeu_09259B30(void) {
    g_Maria.unk44 = 0;
    MarSetStep(0x1A);
    MarSetAnimation(D_pspeu_0927A8A8);
    MarSetSpeedX(FIX(2.25));
    g_Maria.timers[11] = 0x28;
    MARIA.velocityY = 0;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x50001, 0);
}

// local copy of RicSetWalk (see ric/pl_setstep.c) minus the unk7A checks
void func_pspeu_09259BB0(s32 arg0) {
    if (g_Maria.timers[PL_T_8]) {
        func_pspeu_09259B30();
        return;
    }
    g_Maria.timers[PL_T_CURSE] = 8;
    if (g_Maria.timers[PL_T_CURSE]) {
        g_Maria.timers[PL_T_8] = 12;
    }
    g_Maria.timers[PL_T_CURSE] = 12;
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_WALK);
    MarSetAnimation(D_pspeu_0927A6B0);
    MarSetSpeedX(FIX(1.25));
    MARIA.velocityY = 0;
}

// local copy of func_us_801B9E70 (see boss/bo6/us_39144.c)
void func_pspeu_09259C58(void) {
    if ((MarCheckFacing() != 0) || (MARIA.step == 0x18)) {
        MarSetAnimation(D_pspeu_0927A738);
        if (MARIA.step == 0x1A) {
            MarSetSpeedX(FIX(2.25));
            g_Maria.unk44 = 0x10;
        } else {
            MarSetSpeedX(0x14000);
            g_Maria.unk44 = 0;
        }
    } else {
        MarSetAnimation(D_pspeu_0927A718);
        MARIA.velocityX = 0;
        g_Maria.unk44 = 4;
    }
    MarSetStep(5);
    MARIA.velocityY = FIX(-4.6875);
}

// local copy of RicSetFall (see boss/bo6/us_39144.c)
void func_pspeu_09259D48(void) {
    if (g_Maria.prev_step != PL_S_RUN && g_Maria.prev_step != PL_S_SLIDE) {
        MARIA.velocityX = 0;
    }
    if (g_Maria.prev_step != PL_S_WALK && g_Maria.prev_step != PL_S_RUN) {
        MarSetAnimation(D_pspeu_0927A768);
    }
    if (g_Maria.prev_step == PL_S_RUN) {
        g_Maria.unk44 = 0x10;
    }
    MarSetStep(PL_S_FALL);
    MARIA.velocityY = FIX(2);
    g_Maria.timers[PL_T_5] = 8;
    g_Maria.timers[PL_T_6] = 8;
    g_Maria.timers[PL_T_CURSE] = 0;
    g_Maria.timers[PL_T_8] = 0;
    if (g_Maria.prev_step == PL_S_SLIDE) {
        g_Maria.timers[PL_T_5] = g_Maria.timers[PL_T_6] = 0;
        MARIA.pose = 2;
        MARIA.poseTimer = 0x10;
        MARIA.velocityX /= 2;
    }
}

// local copy of func_us_801BA050 (see boss/bo6/us_39144.c)
void func_pspeu_09259EA8(void) {
    MarSetStep(9);
    MARIA.velocityX = 0;
    MarSetSpeedX(FIX(1.25));
    MARIA.velocityY = FIX(-7.5);
    g_Maria.high_jump_timer = 0;
    MarSetAnimation(D_pspeu_0927A9E0);
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x2D, 0), 0);
    g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
    g_Maria.timers[12] = 4;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarCheckSubwpnChainLimit);

// local copy of RicDoSubweapon (see boss/bo6/us_39144.c)
s32 func_pspeu_0925A028(void) {
    SubweaponDef subweapon;
    s16 subweaponId;
    s16 chainLimit;
    s16 unused;

    unused = 0;
    if (!(g_Maria.padPressed & PAD_UP)) {
        return 1;
    }

    subweaponId = func_pspeu_09248D20(&subweapon, 0, 0);
    chainLimit = subweapon.chainLimit;
    if (MarCheckSubwpnChainLimit(subweaponId, chainLimit) < 0) {
        return 2;
    }

    MarCreateEntFactoryFromEntity(g_CurrentEntity, subweapon.blueprintNum, 0);
    g_Maria.timers[PL_T_10] = 4;
    switch (MARIA.step) {
    case PL_S_RUN:
        MARIA.step = PL_S_STAND;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
        MarSetAnimation(D_pspeu_0927A820);
        break;
    case PL_S_STAND:
    case PL_S_WALK:
    case PL_S_CROUCH:
        MARIA.step = PL_S_STAND;
        MarSetAnimation(D_pspeu_0927A820);
        break;
    case PL_S_FALL:
    case PL_S_JUMP:
        MARIA.step = PL_S_JUMP;
        MarSetAnimation(D_pspeu_0927A870);
        break;
    }
    g_Maria.unk46 = 3;
    MARIA.step_s = 0x42;
    // n.b.! this was just set before the switch
    g_Maria.timers[PL_T_10] = 4;
    return 0;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925A1F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925A588);

void RicSetDeadPrologue() { MarSetStep(PL_S_DEAD_PROLOGUE); }

// local copy of RicSetSlide (see boss/bo6/us_39144.c)
void MarSetSlide(void) {
    MarCheckFacing();
    MarSetStep(PL_S_SLIDE);
    MarSetAnimation(D_pspeu_0927A988);
    g_CurrentEntity->velocityY = 0;
    MarSetSpeedX(FIX(5.5));
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(SFX_BOSS_RIC_SLIDE_SKID);
    g_Maria.timers[PL_T_12] = 4;
}

// local copy of RicSetSlideKick (see boss/bo6/us_39144.c)
void func_pspeu_0925AA50(void) {
    g_Maria.unk44 = 0;
    MarSetStep(PL_S_SLIDE_KICK);
    MarSetAnimation(D_pspeu_0927A9B8);
    g_CurrentEntity->velocityY = FIX(-2);
    MarSetSpeedX(FIX(5.5));
    func_maria_8015CC28();
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_25, 0);
    g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
    g_Maria.timers[PL_T_12] = 4;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_31, 0);
}

// local copy of RicSetBladeDash (see ric/pl_setstep.c)
void MarSetBladeDash(void) {
    MarSetStep(PL_S_BLADEDASH);
    MarSetAnimation(D_pspeu_0927AA20);
    g_CurrentEntity->velocityY = 0;
    MarSetSpeedX(FIX(5.5));
    g_Maria.unk46 = 5;
    g_Maria.timers[PL_T_12] = 4;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_BLADE_DASH, 0);
    func_maria_8015CC28();
    g_api.PlaySfx(SFX_BOSS_RIC_DASH_ATTACK);
    g_api.PlaySfx(SFX_RIC_SLIDE_SKID);
}

#include "../../mar_init_after_image.h"

#include "../../mar_draw_after_image.h"

#include "../../mar_set_step.h"

#include "../../mar_set_animation.h"

#include "../../mar_decelerate_x.h"

#include "../../mar_check_facing.h"

#include "../../mar_set_speed_x.h"

void func_pspeu_0924EA50_from_rbo5(s32 velocityX) {
    if (MARIA.entityRoomIndex == 1) {
        velocityX = -velocityX;
    }
    MARIA.velocityX = velocityX;
}

// local copy of RicSetInvincibilityFrames (see boss/bo6/us_39144.c)
void func_pspeu_0925B4E0(s32 kind, s16 invincibilityFrames) {
    if (!kind) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_CRASH_DAGGER, 0x15), 0);
        if (g_Maria.timers[PL_T_INVINCIBLE_SCENE] <= invincibilityFrames) {
            g_Maria.timers[PL_T_INVINCIBLE_SCENE] = invincibilityFrames;
        }
    } else if (g_Maria.timers[PL_T_INVINCIBLE] <= invincibilityFrames) {
        g_Maria.timers[PL_T_INVINCIBLE] = invincibilityFrames;
    }
}

#include "../../mar_disable_after_image.h"

#include "../../mar_reset_after_image.h"

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925B680);

// local copy of ReboundStoneBounce1 (see rebound_stone.h)
void func_pspeu_092508C0_from_rbo5(s16 bounceAngle) {
    g_CurrentEntity->ext.reboundStone.stoneAngle =
        (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

// local copy of ReboundStoneBounce2 (see rebound_stone.h)
void func_pspeu_09250948_from_rbo5(s16 bounceAngle) {
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.stoneAngle =
            (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092509D0_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925CC00);
