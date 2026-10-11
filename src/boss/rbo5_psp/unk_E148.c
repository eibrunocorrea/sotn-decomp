// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"
extern s32 D_pspeu_0926CCD8;
extern s32 D_pspeu_0926CCF8;
extern s32 D_pspeu_0926CD00;
extern u8 D_pspeu_09263FF8[][8];
extern RECT D_pspeu_09263FF0;
extern s32 D_pspeu_0926BBB0;
void func_pspeu_09249838(void);
extern s16 D_pspeu_09262FD8[7];
extern s16 D_pspeu_09263B40[24][5];
extern u8** D_pspeu_09266768;
extern s16* D_pspeu_092641C8[];

// The Doppleganger boss carries its own PSP variant of the player-anim block;
// splat's pre-split names (Mar*/func_pspeu_*) are kept until a rename pass.
// rbo5.h only pulls doppleganger.h outside PSP, so DOPPLEGANGER is local.
#define DOPPLEGANGER g_Entities[STAGE_ENTITY_START]
#define MARIA g_Entities[STAGE_ENTITY_START]
#define g_Dop g_Maria
#define SetDopplegangerStep MarSetStep
#define InitPlayerAfterImage InitMariaAfterImage
#define DrawPlayerAfterImage DrawMariaAfterImage
#define SetDopplegangerAnim func_pspeu_0924DCB8
#define UpdateUnarmedAnim func_pspeu_0924DE50
#define PlayAnimation func_pspeu_0924DFB0
#define UpdateAnim func_pspeu_0924E260

extern PlayerState g_Maria;

// collision sensor tables (.rodata block shared with bo4, different VMAs)
extern Point16 g_MarSensorsCeiling[];
extern Point16 g_MarSensorsFloor[];
extern Point16 g_MarSensorsWall[];

#define BP_BLINK_WHITE 44
#define BP_4 4
#define BP_REBOUND_STONE 24
// entity id and steps from the doppleganger.h enum, not pulled in on PSP
#define E_FACTORY 1
#define Dop_Stand 1
#define Dop_Walk 2
#define Dop_Crouch 3
#define Dop_Fall 4
#define Dop_Jump 5
#define Dop_MorphBat 6
#define Dop_UnmorphBat 10
#define Dop_Hit 11
#define Dop_StatusStone 12
#define Dop_Kill 17
#define Dop_UnmorphMist 15
#define E_MIST 0x22
#define B_ORIGIN_DEFAULT 0
#define B_ORIGIN_1 1
#define B_ORIGIN_2 2
#define B_ORIGIN_3 3
#define B_ORIGIN_4 4
#define B_ORIGIN_5 5
#define B_ORIGIN_6 6
#define B_ORIGIN_7 7

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
void MarDecelerateX_0924E7C8(s32 speed);
void MarDecelerateX_0924E868(s32 speed);
void MarDisableAfterImage(s32 arg0, s32 arg1);
void func_pspeu_0924CD20(void);
void func_pspeu_09248828(void);
void func_80159C04(void);
s32 func_pspeu_092497C0(void);
void func_pspeu_0924EA50(s32 arg0);
void func_pspeu_0924E788(s16 a0, s16 minTime);
// crouch pose/anim pairs and stand anim table (.rodata shared with bo4)
extern u8 D_pspeu_09263028[];
extern u8 D_pspeu_09263030[];

void SetDopplegangerStep(s16 step);
void SetDopplegangerAnim();
s32 func_pspeu_0924EA98(s32 arg0);
s32 func_pspeu_0924CF50(void);
void func_pspeu_0924C9C8(s32 arg0, s32 arg1);
void func_pspeu_0924CA58(s32 arg0);
void func_pspeu_0924CBF0(s32 arg0);
void func_pspeu_0924CDE0(void);
void func_pspeu_0924CE30(void);
s32 func_pspeu_0924D108(void);
void func_pspeu_0924D4E8(void);
void func_pspeu_0924D528(void);

extern s32 D_pspeu_0926BC38[32];
extern s32 D_pspeu_0926BBB8[32];
// local copy of func_us_801C096C (see rbo5/doppleganger.c)
void func_pspeu_092457C8(void) {
    Entity* entity;
    Primitive* prim;
    s16 angle;
    s16 scale;
    s32 i;
    s32 colliderSize;
    s32* colliders;

    g_CurrentEntity = &DOPPLEGANGER;
    DOPPLEGANGER.animSet = ANIMSET_OVL(1);
    g_CurrentEntity->unk5A = 8;
    g_PlayerDraw[8].enableColorBlend = 0;
    DOPPLEGANGER.zPriority = g_unkGraphicsStruct.g_zEntityCenter + 8;
    if (DOPPLEGANGER.posX.i.hi < PLAYER.posX.i.hi) {
        DOPPLEGANGER.facingLeft = false;
    } else {
        DOPPLEGANGER.facingLeft = true;
    }

    DOPPLEGANGER.blendMode = BLEND_NO;
    DOPPLEGANGER.palette = PAL_FLAG(0x200);
    DOPPLEGANGER.scaleY = DOPPLEGANGER.scaleX = 0x100;
    DOPPLEGANGER.flags =
        FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_SUPPRESS_STUN;

    // BUG: this loop seems to be clearing parts of the floor
    //      ceiling colliders, but not all of them
    colliderSize = (sizeof(Collider) * 6) + 0x1C;
    colliders = (s32*)g_Dop.colFloor;
    for (i = 0; i < colliderSize; i++) {
        *colliders++ = 0;
    }

    g_Dop.vram_flag = g_Dop.unk04 = TOUCHING_GROUND;
    func_pspeu_0924CA58(0);
    entity = &g_Entities[65];

    for (i = 0; i < 3; i++, entity++) {
        DestroyEntity(entity);
        entity->animSet = ANIMSET_OVL(1);
        entity->unk5A = i + 9;
        entity->palette = PAL_FLAG(0x200);
        entity->flags = FLAG_POS_CAMERA_LOCKED;
    }
    g_Entities[65].primIndex = g_api.AllocPrimitives(PRIM_TILE, 6);
    g_Entities[65].flags |= FLAG_HAS_PRIMS;
    prim = &g_PrimBuf[g_Entities[65].primIndex];

    i = 0;
    while (prim != NULL) {
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;
        i++;
        prim = prim->next;
    }

    for (i = 0; i < 32; i++) {
        angle = (rand() & 0x3FF) + 0x100;
        scale = (rand() & 0xFF) + 0x100;
        D_pspeu_0926BC38[i] = ((rcos(angle) << 4) * scale) >> 8;
        D_pspeu_0926BBB8[i] = -(((rsin(angle) << 4) * scale) >> 7);
    }

    g_api.TimeAttackController(
        TIMEATTACK_EVENT_DOPPLEGANGER_40_DEFEAT, TIMEATTACK_SET_VISITED);
}

extern s16 D_pspeu_09263078[];
extern s16 D_pspeu_09263080[];
extern s16 D_pspeu_09263088[];
extern s16 D_pspeu_09263098[];
extern s16 D_pspeu_092630A0[];
extern s16 D_pspeu_092630A8[];
void MarCheckFloor(void);
void MarCheckCeiling(void);
void func_pspeu_09250260(void);
void MarCheckWallLeft(void);
void func_pspeu_09245AD8(s32 checkSensors) {
    s16 argX;
    s16 argY;
    s32 xVel;
    s32 i;
    s32* vram_ptr;
    s32* unk04_ptr;
    s32 status;
    s32 unk04;

    vram_ptr = &g_Maria.vram_flag;
    unk04_ptr = &g_Maria.unk04;
    *unk04_ptr = *vram_ptr;
    *vram_ptr = 0;
    status = g_Maria.status;

    if (checkSensors) {
        for (i = 0; i < NUM_HORIZONTAL_SENSORS; i++) {
            if (status & Dop_Crouch) {
                g_MarSensorsFloor[i].y = D_pspeu_09263080[i];
                g_MarSensorsCeiling[i].y = D_pspeu_09263078[i];
            } else {
                g_MarSensorsFloor[i].y = D_pspeu_092630A0[i];
                g_MarSensorsCeiling[i].y = D_pspeu_09263098[i];
            }
        }
        for (i = 0; i < NUM_VERTICAL_SENSORS; i++) {
            if (status & Dop_Crouch) {
                g_MarSensorsWall[i].y = D_pspeu_09263088[i];
                g_MarSensorsWall[i + NUM_VERTICAL_SENSORS].y =
                    D_pspeu_09263088[i];
            } else {
                g_MarSensorsWall[i].y = D_pspeu_092630A8[i];
                g_MarSensorsWall[i + NUM_VERTICAL_SENSORS].y =
                    D_pspeu_092630A8[i];
            }
        }
    }
    xVel = MARIA.velocityX;
    unk04 = (*unk04_ptr & 0xF000);
    if (MARIA.velocityX < 0 && !(*unk04_ptr & 8)) {
        if (unk04 == 0xC000) {
            xVel = xVel * 10 / 16;
        }
        if (unk04 == 0xD000) {
            xVel = xVel * 13 / 16;
        }
        MARIA.posX.val += xVel;
    }
    if (MARIA.velocityX > 0 && !(*unk04_ptr & 4)) {
        if (unk04 == 0x8000) {
            xVel = xVel * 10 / 16;
        }
        if (unk04 == 0x9000) {
            xVel = xVel * 13 / 16;
        }
        MARIA.posX.val += xVel;
    }
    if ((MARIA.velocityY < 0) && !(*unk04_ptr & 2)) {
        MARIA.posY.val += MARIA.velocityY;
    }
    if ((MARIA.velocityY > 0) && !(*unk04_ptr & 1)) {
        MARIA.posY.val += MARIA.velocityY;
    }
    for (i = 0; i < NUM_HORIZONTAL_SENSORS; i++) {
        argX = MARIA.posX.i.hi + g_MarSensorsFloor[i].x;
        argY = MARIA.posY.i.hi + g_MarSensorsFloor[i].y;
        g_api.CheckCollision(argX, argY, &g_Maria.colFloor[i], 0);
    }
    MarCheckFloor();
    for (i = 0; i < NUM_HORIZONTAL_SENSORS; i++) {
        argX = MARIA.posX.i.hi + g_MarSensorsCeiling[i].x;
        argY = MARIA.posY.i.hi + g_MarSensorsCeiling[i].y;
        g_api.CheckCollision(argX, argY, &g_Maria.colCeiling[i], 0);
    }
    MarCheckCeiling();
    if ((*vram_ptr & 1) && (MARIA.velocityY >= 0)) {
        MARIA.posY.i.lo = 0;
    }
    if ((*vram_ptr & 2) && (MARIA.velocityY <= 0)) {
        MARIA.posY.i.lo = 0;
    }
    for (i = 0; i < NUM_VERTICAL_SENSORS * 2; i++) {
        argX = MARIA.posX.i.hi + g_MarSensorsWall[i].x;
        argY = MARIA.posY.i.hi + g_MarSensorsWall[i].y;
        g_api.CheckCollision(argX, argY, &g_Maria.colWall[i], 0);
    }
    func_pspeu_09250260();
    MarCheckWallLeft();
    if ((*vram_ptr & 4) && (MARIA.velocityX > 0)) {
        MARIA.posX.i.lo = 0;
    }
    if ((*vram_ptr & 8) && (MARIA.velocityX < 0)) {
        MARIA.posX.i.lo = 0;
    }
    if (*vram_ptr & 0x8000) {
        *vram_ptr |= 0x20;
    }
    if (!(g_Maria.colFloor[1].effects & EFFECT_SOLID) ||
        !(g_Maria.colFloor[2].effects & EFFECT_SOLID) ||
        !(g_Maria.colFloor[3].effects & EFFECT_SOLID)) {
        *vram_ptr |= 0x20;
    }
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09246210);

char D_pspeu_09262FC0[16] = "noroi:%02x\n";
s16 D_pspeu_09262FD0[] = {1, 31, 0, 27};
s16 D_pspeu_09262FD8[7] = {2013, 2012, 2011, 2010, 2009, 2008, 2007};

extern s32 D_pspeu_0926BBA0;
extern s32 D_pspeu_0926BBA8;
// local copy of func_us_801B5A14 (see bo6/richter.c): set the boss
// think-step and reset its timer
void func_pspeu_09247190(s32 step) {
    D_pspeu_0926BBA8 = step;
    D_pspeu_0926BBA0 = 0;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092471B8);

extern EInit g_EInitDoppleganger40;
void func_pspeu_09247190(s32 step);
void func_pspeu_092471B8(void);
void func_pspeu_09246210(void);
void func_pspeu_09252B48(void);
// local copy of EntityUnkId16 (see rbo5/doppleganger.c)
void func_pspeu_09248698(Entity* self) {
    Entity* entity;
    s32 i;

    g_Dop.unk6A = DOPPLEGANGER.hitPoints;
    if (self->step == 0) {
        InitializeEntity(g_EInitDoppleganger40);
        if (PLAYER.posX.i.hi > 0x80) {
            DOPPLEGANGER.posX.i.hi = 0x10;
        } else {
            DOPPLEGANGER.posX.i.hi = 0xF0;
        }
        func_pspeu_092457C8();
        entity = &g_Entities[68];
        self = &DOPPLEGANGER;
        for (i = 68; i < 144; i++, entity++) {
            DestroyEntity(entity);
        }
        g_Dop.unk6E = g_Dop.unk6A = g_Dop.unk6C = DOPPLEGANGER.hitPoints;
        g_Dop.unk70 = DOPPLEGANGER.hitboxState;
        if (PLAYER.posX.i.hi > 0x80) {
            func_pspeu_09247190(0x12);
        } else {
            func_pspeu_09247190(0x14);
        }
        MarDisableAfterImage(1, 0x30);
    } else {
        func_pspeu_092471B8();
        func_pspeu_09246210();
        func_pspeu_09252B48();
    }
    g_Dop.unk6C = g_Dop.unk6A;
}

void func_pspeu_09248828(void) {
    if (g_Maria.timers[1]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x17), 0);
    }
    if (g_Maria.timers[0]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x16), 0);
    }
}

// local copy of DopplegangerStepStand (see bo4/unk_45354.c); note the PSP
// build allocates var_s0 before anim (s0/s1)
void func_pspeu_09248898(void) {
    u16 var_s0;
    s32 anim;

    var_s0 = 3;
    anim = 0;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        anim = 1;
    }

    if (func_pspeu_0924EA98(0x4301C) == 0) {
        MarDecelerateX_0924E7C8(FIX(0.125));
        switch (MARIA.step_s) {
        case 0:
        case 2:
            break;
        case 1:
            var_s0 = 1;
            if (!(g_Maria.padPressed & PAD_UP)) {
                var_s0 = 5;
            }
            break;
        case 3:
            var_s0 = 0;
            if (MARIA.pose > 3) {
                var_s0 = 1;
            }
            if (MARIA.pose > 6 || MARIA.poseTimer < 0) {
                var_s0 = 7;
            }
            break;
        case 0x40:
        case 0x41:
        case 0x42:
            MarDisableAfterImage(1, 1);
            if (MARIA.pose < g_Maria.unk54) {
                var_s0 = 0;
            } else {
                g_Maria.unk46 &= 0x7FFF;
                var_s0 = 0x1B;
                if (MARIA.poseTimer < 0) {
                    var_s0 = 0xF;
                }
            }
            break;
        case 0x51:
            MarDisableAfterImage(1, 1);
            var_s0 = 0;
            if (MARIA.poseTimer < 0) {
                var_s0 = 0xF;
            }
            break;
        }

        if (var_s0 & 4) {
            func_pspeu_0924CA58(0);
            var_s0 |= 0x8000;
        }
        if (var_s0 & 2 && g_Maria.padPressed & PAD_UP && !g_Maria.unk48) {
            SetDopplegangerAnim(anim);
            MARIA.step_s = 1;
            var_s0 |= 0x8000;
        }

        if (var_s0 & 1 && MarCheckFacing() != 0) {
            func_pspeu_0924CBF0(0);
            var_s0 |= 0x8000;
        }
        if (var_s0 & 0x8000 && var_s0 & 8) {
            func_pspeu_0924D4E8();
        }
    }
}

void func_pspeu_09248B88(void) {
    if (func_pspeu_0924EA98(0x4301C) == 0) {
        MarSetSpeedX(FIX(1.5));
        if (MarCheckFacing() == 0) {
            func_pspeu_0924CA58(0);
        }
    }
}

extern s16 D_pspeu_09262FD0[];
// local copy of DopplegangerStepJump (see bo4/unk_45354.c)
void func_pspeu_09248BE8(void) {
    s32 moveDirection;
    s16 index;

    MarDecelerateX_0924E7C8(FIX(1.0 / 16.0));
    if (MARIA.velocityY < FIX(-1)) {
        if (!(g_Maria.unk44 & 0x40) && !(g_Maria.padPressed & PAD_CROSS)) {
            MARIA.velocityY = FIX(-1);
        }
        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            MARIA.velocityY = FIX(-0.25);
            g_Maria.unk44 |= 0x20;
        }
    }

    if (func_pspeu_0924EA98(0x11029)) {
        return;
    }

    switch (MARIA.step_s) {
    case 0:
        moveDirection = MarCheckFacing();
        if (moveDirection) {
            if (MARIA.ext.player.anim == 22 || MARIA.ext.player.anim == 25) {
                SetDopplegangerAnim(24);
            }
            MarSetSpeedX(FIX(1.5));
        } else if (MARIA.ext.player.anim == 26 || MARIA.ext.player.anim == 24) {
            SetDopplegangerAnim(25);
        }
        if (moveDirection <= 0) {
            g_Maria.unk44 &= 0xFFEF;
        }
        if (MARIA.velocityY > 0) {
            if (MARIA.ext.player.anim != 27) {
                SetDopplegangerAnim(27);
            }
            MARIA.step_s = 1;
        }
        break;
    case 1:
        moveDirection = MarCheckFacing();
        if (moveDirection != 0) {
            MarSetSpeedX(FIX(1.5));
        }
        if (moveDirection <= 0) {
            g_Maria.unk44 &= 0xFFEF;
        }
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x51:
        MarDisableAfterImage(1, 1);
        if (g_Maria.padPressed & PAD_LEFT) {
            MARIA.velocityX = FIX(-1.5);
        }
        if (g_Maria.padPressed & PAD_RIGHT) {
            MARIA.velocityX = FIX(1.5);
        }
        if (MARIA.poseTimer < 0) {
            if (MARIA.velocityY > FIX(1)) {
                index = 0;
            } else {
                index = 2;
            }
            MARIA.step_s = D_pspeu_09262FD0[index];
            SetDopplegangerAnim((u8)D_pspeu_09262FD0[index + 1]);
            func_pspeu_0924D4E8();
        }
        break;
    }
}

// local copy of DopplegangerStepFall (see bo4/unk_45354.c)
void func_pspeu_09248F50(void) {
    if (func_pspeu_0924EA98(0x9029) == 0) {
        MarDecelerateX_0924E7C8(FIX(1.0 / 16.0));
        if (MarCheckFacing() != 0) {
            MarSetSpeedX(FIX(3.0 / 4.0));
        }
    }
}

// local copy of DopplegangerStepCrouch (see bo4/unk_45354.c); same decl
// swap as StepStand, and the shared case labels flip to 4,3
void func_pspeu_09248FA8(void) {
    s16 var_s0;
    s32 anim;

    var_s0 = 0;
    anim = 0;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        anim = 1;
    }

    if (func_pspeu_0924EA98(0x100C) == 0) {
        MarDecelerateX_0924E7C8(FIX(0.125));
        switch (MARIA.step_s) {
        case 0:
            var_s0 = 6;
            break;
        case 1:
            if (!(g_Maria.padPressed & PAD_DOWN)) {
                var_s0 = 1;
                SetDopplegangerAnim(0x13);
                MARIA.step_s = 2;
                MARIA.pose = 1;
                return;
            }
            if (MARIA.ext.player.anim == 0x65) {
                MARIA.step_s = 0;
            } else if (MARIA.poseTimer < 0) {
                var_s0 = 0x20;
            }
            break;
        case 4:
        case 3:
            if (MARIA.poseTimer < 0) {
                var_s0 = 0x20;
            }
            break;
        case 2:
            var_s0 = 1;
            if (MARIA.poseTimer < 0) {
                func_pspeu_0924CA58(0);
            }
            break;
        case 0x40:
        case 0x41:
        case 0x42:
            MarDisableAfterImage(1, 1);
            if (MARIA.pose < g_Maria.unk54) {
                var_s0 = 0;
            } else {
                g_Maria.unk46 &= 0x7FFF;
                var_s0 = 0xE;
                if (MARIA.poseTimer < 0) {
                    var_s0 = 0x2E;
                }
            }
            break;
        case 0x51:
            MarDisableAfterImage(1, 1);
            if (MARIA.poseTimer < 0) {
                var_s0 = 0x2E;
            }
            break;
        }

        if (var_s0 & 0x20) {
            func_pspeu_0924C9C8(0, 0);
            var_s0 |= 0x8000;
        }

        if (var_s0 & 2 && g_Maria.unk4C) {
            SetDopplegangerAnim(0x14);
            MARIA.step_s = 0;
            var_s0 |= 0x8000;
        }

        if (var_s0 & 4 && !(g_Maria.padPressed & PAD_DOWN)) {
            SetDopplegangerAnim(0x13);
            MARIA.step_s = 2;
            var_s0 |= 0x8000;
        }

        if (var_s0 & 1 && MarCheckFacing()) {
            func_pspeu_0924CBF0(0);
            var_s0 |= 0x8000;
        }

        if (var_s0 & 0x8000 && var_s0 & 8) {
            func_pspeu_0924D4E8();
        }
    }
}

// cen's func_80158B04 with the Doppleganger's taller ember origin (22 vs 16)
void func_80158B04(u16 arg0) {
    s16 xMod = 3;
    if (MARIA.facingLeft) {
        xMod = -xMod;
    }

    MARIA.posY.i.hi -= 22;
    MARIA.posX.i.hi += xMod;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_4, 1), 0);
    MARIA.posY.i.hi += 22;
    MARIA.posX.i.hi -= xMod;

    if (arg0 & 1) {
        g_api.ShakeCamera(3);
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
    }

    if (arg0 & 2) {
        MARIA.velocityX = 0;
        MARIA.velocityY = 0;
    }
}

// local copy of DopplegangerStepHighJump (see bo4/unk_46E7C.c); the PSP
// build reorders the rotation setup and splits the facingLeft flip
void func_pspeu_09249460(void) {
    s16 var_s1;

    var_s1 = 0;
    g_Maria.gravBootTimer++;
    if (func_pspeu_0924EA98(2) != 0) {
        return;
    }

    switch (MARIA.step_s) {
    case 0:
        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            func_80158B04(3);
            if (g_Maria.gravBootTimer > 4) {
                MARIA.step_s = 2;
                MARIA.drawFlags |= ENTITY_ROTATE;
                MARIA.rotate = 0x800;
                MARIA.rotPivotX = 0;
                MARIA.rotPivotY = 2;
                MARIA.facingLeft++;
                MARIA.facingLeft &= 1;
                SetDopplegangerAnim(0x2B);
            } else {
                MARIA.step_s = 3;
            }
        } else if (g_Maria.gravBootTimer > 28) {
            MARIA.step_s = 1;
            MARIA.velocityY = -0x60000;
            SetDopplegangerAnim(0x1B);
        }
        break;
    case 1:
        if (g_Maria.vram_flag & TOUCHING_CEILING) {
            MARIA.step_s = 2;
            func_80158B04(3);
        } else {
            MARIA.velocityY = MARIA.velocityY + 0x6000;
            if (MARIA.velocityY > 0x8000) {
                var_s1 = 1;
            }
        }
        break;
    case 2:
        MARIA.drawFlags |= ENTITY_ROTATE;
        MARIA.rotPivotX = 0;
        MARIA.rotPivotY = 2;
        if (g_Maria.gravBootTimer > 56) {
            SetDopplegangerAnim(0x2D);
            MARIA.drawFlags &=
                ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G | ENTITY_MASK_R |
                ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
            MARIA.rotate = 0;
            MARIA.facingLeft++;
            MARIA.facingLeft &= 1;
            MARIA.step_s = 4;
        }
        break;
    case 3:
        if (g_Maria.gravBootTimer > 20) {
            var_s1 = 1;
        }
        break;
    case 4:
        MARIA.velocityY += FIX(1.0 / 16.0);
        if (MARIA.poseTimer < 0) {
            var_s1 = 2;
        }
        break;
    }

    if (var_s1) {
        if (--var_s1) {
            SetDopplegangerAnim(0x1C);
        }
        MARIA.palette = PAL_FLAG(0x200);
        MARIA.step_s = 1;
        MARIA.step = Dop_Jump;
    }
}

s32 func_pspeu_092497C0(void) {
    s16 rnd = rand() & PSP_RANDMASK;
    MARIA.ext.player.anim = 0x2E + (rnd % 3);
    return rnd % 16;
}

// local copy of func_us_801C72BC (see bo4/unk_46E7C.c): reset the boss
// visual state; the PSP build reorders the stores and chains pose/poseTimer
void func_pspeu_09249838(void) {
    MARIA.pose = MARIA.poseTimer = 0;
    MARIA.animSet = ANIMSET_OVL(1);
    MARIA.blendMode = BLEND_NO;
    g_Maria.unk44 = 0;
    g_Maria.unk46 = 0;
    MARIA.drawFlags &= ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G |
                       ENTITY_MASK_R | ENTITY_SCALEY | ENTITY_SCALEX;
    MARIA.rotate = 0;
    if (g_Entities[80].entityId == E_MIST) {
        func_pspeu_0924D4E8();
    }
}

void func_80159C04(void) {
    if (MARIA.posX.i.hi <= PLAYER.posX.i.hi) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

void func_pspeu_09249910(DamageParam* damage, s16 step, s16 step_s) {
    s16 anim;
    s16 sfxIndex;

    switch (MARIA.step_s) {
    case 0:
        func_pspeu_09249838();
        func_80159C04();
        anim = 0;
        sfxIndex = 0;
        switch (damage->damageKind) {
        case 3:
            sfxIndex = (rand() & 1) + 3;
            MARIA.velocityY = FIX(-4);
            func_pspeu_0924EA50(FIX(-5.0 / 6));
            MARIA.step_s = 1;
            if (func_pspeu_092497C0() == 0) {
                MARIA.ext.player.anim = 0x40;
            }
            break;
        case 2:
            sfxIndex = (rand() & 1) + 5;
            switch (step) {
            case 1:
            case 2:
                MARIA.velocityY = 0;
                func_pspeu_0924EA50(FIX(-5.0 / 3));
                MARIA.step_s = 6;

                MARIA.ext.player.anim = 0x31;
                if (MARIA.entityRoomIndex != MARIA.facingLeft) {
                    MARIA.ext.player.anim = 0x33;
                }

                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(0, 6), 0);
                break;
            case 3:
                MARIA.velocityY = 0;
                func_pspeu_0924EA50(FIX(-1.25));
                MARIA.step_s = 7;
                MARIA.ext.player.anim = 0x23;
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(0, 0), 0);
                break;
            default:
            case 5:
            case 4:
                MARIA.velocityY = FIX(-2);
                func_pspeu_0924EA50(FIX(-1.25));
                MARIA.step_s = 1;
                func_pspeu_092497C0();
                break;
            }
            break;
        default:
            FntPrint("dam_kind:%04x\n", damage->damageKind);
            break;
        }

        g_Maria.damagePalette = PAL_FLAG(PAL_CC_RED_EFFECT_A);
        g_Maria.timers[2] = 6;
        g_api.PlaySfx(D_pspeu_09262FD8[sfxIndex]);

        if (damage->effects & EFFECT_UNK_8000) {
            g_api.PlaySfx(SFX_FM_EXPLODE_SWISHES);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 0x45), 0);
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_FIRE_EFFECT);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(17, 1), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(18, 0), 0);
            g_Maria.timers[2] = 0x10;
        } else if (damage->effects & EFFECT_UNK_0100) {
            g_Maria.timers[1] = 0x400;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_CURSE_EFFECT);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 23), 0);
        } else if (damage->effects & EFFECT_SOLID_FROM_BELOW) {
            g_Maria.timers[0] = 0x400;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_DARK_EFFECT);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 22), 0);
        } else if (damage->effects & EFFECT_UNK_4000) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x2D, 0), 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 0x46), 0);
            g_Maria.timers[2] = 0x18;
            g_Maria.damagePalette = PAL_FLAG(0x202);
        } else if (damage->effects & EFFECT_UNK_2000) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x2E, 0), 0);
            g_Maria.timers[2] = 0xC;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_BLUE_EFFECT_A);
            MARIA.ext.player.anim = 0x2E;
        } else if (damage->effects & EFFECT_UNK_1000) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x72, 0), 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 0x63), 0);
            g_Maria.timers[2] = 8;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_DARK_EFFECT);
        } else if (damage->effects & EFFECT_UNK_0800) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(0x71, 0), 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(0x2C, 0x62), 0);
            g_Maria.timers[2] = 16;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_DARK_EFFECT);
        } else {
            damage->effects & EFFECT_UNK_0400;
            if (!(damage->effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                   EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_UNK_0200 |
                   EFFECT_SOLID_FROM_BELOW | EFFECT_SOLID_FROM_ABOVE))) {
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(0x2C, 0x58), 0);
            }
        }
        break;
    case 1:
        if ((func_pspeu_0924EA98(0x20280) == 0) && (MARIA.poseTimer < 0)) {
            func_pspeu_0924DCB8(0x1C);
            MARIA.facingLeft = (MARIA.facingLeft + 1) & 1;
            return;
        }
        break;
    case 8:
        MARIA.palette = PAL_FLAG(0x200);
        // fallthrough
    case 6:
    case 7:
        MarDecelerateX_0924E7C8(FIX(1.0 / 8));
        if (!(g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_0924CE30();
        }
        if (MARIA.poseTimer < 0) {
            if (MARIA.step_s == 6) {
                func_pspeu_0924CA58(0);
                return;
            }
            func_pspeu_0924C9C8(0, MARIA.velocityX);
        }
        break;
    }
}

extern s32 D_pspeu_0926CCD0;
extern s32 D_pspeu_0926C0B8;
extern s32 D_pspeu_0926C0B0;
extern s32 D_pspeu_0926C0A8;
extern s32 D_pspeu_0926C0A0;
extern u_long D_pspeu_0926C110[];
extern RECT D_pspeu_09263FE8;
void func_pspeu_09252480(s32 arg0);
void func_pspeu_09249FE8(DamageParam* damage, s16 dopStep, s16 arg2) {
    s32 i;
    s32 j;
    Entity* ent;
    u8* s2;
    u8* data;
    PlayerDraw* plDraw;

    DOPPLEGANGER.drawFlags = DRAW_COLORS;
    plDraw = &g_PlayerDraw[8];

    switch (DOPPLEGANGER.step_s) {
    case 0:
        DOPPLEGANGER.velocityY = 0;
        DOPPLEGANGER.velocityX = 0;
        if (dopStep == Dop_StatusStone) {
            ent = &DOPPLEGANGER + 16;
            for (j = 16; j < 64; j++, ent++) {
                // Entity 32 appears to be EntityDopplegangerDissolves
                if (ent->entityId == 32) {
                    g_api.PlaySfx(SFX_VO_DOP_DEATH);
                    DOPPLEGANGER.step_s = 16;
                    return;
                }
            }
        }
        g_api.PlaySfx(SFX_VO_DOP_DEATH);
        func_pspeu_09249838();
        func_80159C04();
        DOPPLEGANGER.velocityY = FIX(-3.25);
        func_pspeu_0924EA50(FIX(-1.25));
        DOPPLEGANGER.ext.player.anim = 0xC0;
        DOPPLEGANGER.rotate = 0;
        DOPPLEGANGER.rotPivotY = 0;
        DOPPLEGANGER.rotPivotX = 0;
        if (damage->effects & ELEMENT_FIRE) {
            func_pspeu_09252480(3);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x4F), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(51, 2), 0);
            D_pspeu_0926C0A8 = 1;
        } else if (damage->effects & ELEMENT_THUNDER) {
            func_pspeu_09252480(9);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x59), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(45, 1), 0);
            D_pspeu_0926C0A8 = 2;
        } else if (damage->effects & ELEMENT_ICE) {
            func_pspeu_09252480(10);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x5A), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 46, 0);
            D_pspeu_0926C0A8 = 3;
            DOPPLEGANGER.blendMode = BLEND_TRANSP | BLEND_ADD;
        } else {
            func_pspeu_09252480(1);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x53), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(49, 5), 0);
            D_pspeu_0926C0A8 = 0;
        }
        plDraw->r0 = plDraw->g0 = plDraw->b0 = plDraw->r1 = plDraw->g1 =
            plDraw->b1 = plDraw->r2 = plDraw->g2 = plDraw->b2 = plDraw->r3 =
                plDraw->g3 = plDraw->b3 = 128;
        plDraw->enableColorBlend = 1;
        DOPPLEGANGER.step_s++;
        break;
    case 1:
        if (!D_pspeu_0926C0A8) {
            if (plDraw->r0 < 248) {
                plDraw->r0++;
            }
            if (plDraw->b0 > 8) {
                plDraw->b0--;
            }

            plDraw->r3 = plDraw->r2 = plDraw->r1 = plDraw->r0;
            plDraw->g0 = plDraw->g1 = plDraw->b1 = plDraw->g2 = plDraw->b2 =
                plDraw->g3 = plDraw->b3 = plDraw->b0;
        }
        if (D_pspeu_0926C0A8 == 1 || D_pspeu_0926C0A8 == 2) {
            if (plDraw->b0 > 8) {
                plDraw->b0--;
            }
            plDraw->r3 = plDraw->r2 = plDraw->r1 = plDraw->r0 = plDraw->g0 =
                plDraw->g1 = plDraw->b1 = plDraw->g2 = plDraw->b2 = plDraw->g3 =
                    plDraw->b3 = plDraw->b0;
        }
        if (D_pspeu_0926C0A8 == 3) {
            if (plDraw->r0 < 248) {
                plDraw->r0--;
            }
            plDraw->r3 = plDraw->r2 = plDraw->r1 = plDraw->g3 = plDraw->g2 =
                plDraw->g1 = plDraw->g0 = plDraw->r0;
            if (plDraw->b0 < 248) {
                plDraw->b0++;
            }
            plDraw->b3 = plDraw->b2 = plDraw->b1 = plDraw->b0;
        }
        DOPPLEGANGER.velocityY += FIX(11.0 / 128);
        if (DOPPLEGANGER.velocityY > FIX(1.0 / 4)) {
            DOPPLEGANGER.velocityY = FIX(1.0 / 16);
        }
        if (DOPPLEGANGER.poseTimer < 0) {
            StoreImage(&D_pspeu_09263FE8, (u_long*)&D_pspeu_0926C110);
            D_pspeu_0926C0B8 = 0;
            D_pspeu_0926C0B0 = 0x40;
            g_CurrentEntity->step_s++;
        }
        break;
    case 2:
        for (i = 0; i < 4; i++) {
            s2 = data = (u8*)&D_pspeu_0926C110[0];
            s2 += ((D_pspeu_0926C0B8 >> 1) & 7);
            s2 += ((D_pspeu_0926C0B8 & 0xFF) >> 4) << 5;
            for (j = 0; j < 16; j++) {
                if (D_pspeu_0926C0B8 & 1) {
                    *(s2 + ((j & 3) * 8) + ((j >> 2) * 0x200)) &= 0xF0;
                } else {
                    *(s2 + ((j & 3) * 8) + ((j >> 2) * 0x200)) &= 0x0F;
                }
            }
            D_pspeu_0926C0B8 += 0x23;
            D_pspeu_0926C0B8 &= 0xFF;
        }
        LoadImage(&D_pspeu_09263FE8, (u_long*)data);
        if (--D_pspeu_0926C0B0 == 0) {
            DOPPLEGANGER.velocityY = 0;
            plDraw->enableColorBlend = 0;
            g_CurrentEntity->step_s = 0x80;
        }
        break;
    case 16:
        D_pspeu_0926C0A0 = 0x50;
        DOPPLEGANGER.step_s++;
        break;
    case 17:
        g_Dop.unk5E = 5;
        if (D_pspeu_0926C0A0 % 16 == 7) {
            g_Dop.padTapped = PAD_UP;
            g_api.PlaySfx(SFX_STONE_MOVE_B);
        }
        if (--D_pspeu_0926C0A0 == 0) {
            func_pspeu_0924DCB8(0x3E);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(16, 3), 0);
            DOPPLEGANGER.step_s++;
        }
        break;
    case 18:
        if (DOPPLEGANGER.poseTimer < 0) {
            plDraw->enableColorBlend = 0;
            g_CurrentEntity->step_s = 0x80;
        }
        break;
    case 0x80:
        D_pspeu_0926CCD0 |= 4;
        break;
    }
    MarDecelerateX_0924E7C8(FIX(1.0 / 64));
    if (DOPPLEGANGER.pose >= 15) {
        if ((DOPPLEGANGER.pose == 22) && (DOPPLEGANGER.poseTimer == 1)) {
            DOPPLEGANGER.rotate -= 0x100;
        }
        DOPPLEGANGER.rotate -= 6;
        if (DOPPLEGANGER.rotate < -0x280) {
            DOPPLEGANGER.rotate = -0x280;
        }
    }
}

extern u16 D_pspeu_092629BA;
// local copy of BatFormFinished (see bo4/unk_46E7C.c); the u16 is the
// bat morph animation's [0].pose field
s32 func_pspeu_0924A948(void) {
    if (MARIA.step_s == 0) {
        return 0;
    }
    if (g_Maria.padTapped & PAD_R1) {
        MarSetStep(Dop_UnmorphBat);
        SetDopplegangerAnim(202);
        D_pspeu_092629BA = 6;
        MARIA.palette = PAL_FLAG(0x20D);
        g_Maria.unk66 = 0;
        g_Maria.unk68 = 0;
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x21), 0);
        MARIA.velocityY >>= 1;
        return 1;
    }
    return 0;
}

void func_pspeu_0924AA18(s16 arg0) {
    if (MARIA.rotate < arg0) {
        MARIA.rotate += 16;
        if (MARIA.rotate > arg0) {
            MARIA.rotate = arg0;
        }
    }
    if (MARIA.rotate > arg0) {
        MARIA.rotate -= 16;
        if (MARIA.rotate < arg0) {
            MARIA.rotate = arg0;
        }
    }
}

// unlike bo4, the rbo5 wall checks padPressed (see the US bo4 note
// about Dop40)
s32 func_pspeu_0924AAF0(void) {
    if (g_Maria.padPressed & PAD_SQUARE) {
        return true;
    }
    return false;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924AB20);

void func_pspeu_0924CD20(void);
// local copy of DopplegangerStepUnmorphBat (see bo4/unk_46E7C.c)
void func_pspeu_0924B918(void) {
    s32 i;
    s32 count;

    MARIA.drawFlags = ENTITY_ROTATE;
    MarDecelerateX_0924E7C8(FIX(1.0 / 8.0));
    if (g_Maria.vram_flag & (TOUCHING_CEILING | TOUCHING_GROUND)) {
        MARIA.velocityY = 0;
    }
    MarDecelerateX_0924E868(FIX(1.0 / 8.0));
    func_pspeu_0924AA18(0);
    count = 0;

    switch (MARIA.step_s) {
    case 0:
        for (i = 0; i < 4; i++) {
            if (g_MarSensorsFloor[i].y < D_pspeu_092630A0[i]) {
                g_MarSensorsFloor[i].y++;
            } else {
                count++;
            }
            if (g_MarSensorsCeiling[i].y > D_pspeu_09263098[i]) {
                g_MarSensorsCeiling[i].y--;
            } else {
                count++;
            }
            if (i == 0 && (g_Maria.vram_flag & TOUCHING_ANY_SLOPE)) {
                MARIA.posY.i.hi--;
            }
        }
        if (count == 8) {
            MARIA.animSet = ANIMSET_OVL(1);
            MARIA.drawFlags = ENTITY_DEFAULT;
            MARIA.rotate = 0;
            g_Maria.unk66 = 1;
            MARIA.step_s = 1;
            D_pspeu_092629BA = 0x5F;
        }
        break;
    case 1:
        if (g_Maria.unk66 == 3) {
            func_pspeu_0924CD20();
            if (!(g_Maria.vram_flag & TOUCHING_ANY_SLOPE)) {
                MARIA.velocityY = FIX(-1);
            }
            MARIA.palette = PAL_FLAG(0x200);
            func_pspeu_09248828();
        }
        break;
    }
}

// local copy of func_us_801C8EE4 (see bo4/unk_46E7C.c): leave mist form
s32 func_pspeu_0924BB78(void) {
    if (MARIA.step_s == 0) {
        return 0;
    }
    if (g_Maria.padTapped & PAD_R2) {
        MarCheckFacing();
        MarSetStep(Dop_UnmorphMist);
        return 1;
    }
    return 0;
}

void func_pspeu_0924BBE8(void) {
    u32 padDirection;

    if (func_pspeu_0924BB78() == 0) {
        padDirection = g_Maria.padPressed & PAD_DIRECTION_MASK;
        switch (MARIA.step_s) {
        case 0:
            MarCheckFacing();
            g_Maria.unk44 = g_Maria.unk46 = g_Maria.unk48 = 0;
            g_api.func_800EA5E4(0x8003U);
            func_pspeu_0924D4E8();
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x49U, 0);
            if (MARIA.velocityX > 0) {
                MARIA.velocityX = FIX(1);
            }
            if (MARIA.velocityX < 0) {
                MARIA.velocityX = FIX(-1);
            }
            if (MARIA.velocityY > 0) {
                MARIA.velocityY = FIX(1);
            }
            if (MARIA.velocityY < 0) {
                MARIA.velocityY = FIX(-1);
            }
            func_pspeu_0924DCB8(0xCA);
            D_pspeu_092629BA = MARIA.animCurFrame;
            g_api.func_800EA538(5);
            g_api.func_800EA5E4(0x8801U);
            MARIA.step_s++;
            break;

        case 1:
            switch (padDirection) {
            case PAD_NONE:
            default:
                MarDecelerateX_0924E7C8(FIX(3.0 / 256.0));
                MarDecelerateX_0924E868(FIX(3.0 / 256.0));
                break;
            case PAD_UP:
                if (MARIA.velocityY < FIX(-1.0)) {
                    MarDecelerateX_0924E868(FIX(3.0 / 256.0));
                } else {
                    MARIA.velocityY = FIX(-1.0);
                }
                MarDecelerateX_0924E7C8(FIX(3.0 / 256.0));
                break;
            case PAD_DOWN:
                if (MARIA.velocityY > FIX(1.0)) {
                    MarDecelerateX_0924E868(FIX(3.0 / 256.0));
                } else {
                    MARIA.velocityY = FIX(1.0);
                }
                MarDecelerateX_0924E7C8(FIX(3.0 / 256.0));
                break;
            case PAD_RIGHT:
                MARIA.facingLeft = false;
                if (MARIA.velocityX > FIX(1.0)) {
                    MarDecelerateX_0924E7C8(FIX(3.0 / 256.0));
                } else {
                    MARIA.velocityX = FIX(1.0);
                }
                MarDecelerateX_0924E868(FIX(3.0 / 256.0));
                break;
            case PAD_LEFT:
                MARIA.facingLeft = true;
                if (MARIA.velocityX < FIX(-1.0)) {
                    MarDecelerateX_0924E7C8(FIX(3.0 / 256.0));
                } else {
                    MARIA.velocityX = FIX(-1.0);
                }
                MarDecelerateX_0924E868(FIX(3.0 / 256.0));
                break;

            case (PAD_UP | PAD_RIGHT):
                MARIA.facingLeft = false;
                if (MARIA.velocityX > FIX(0.625)) {
                    MarDecelerateX_0924E7C8(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityX = FIX(0.625);
                }

                if (MARIA.velocityY < FIX(-0.625)) {
                    MarDecelerateX_0924E868(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityY = FIX(-0.625);
                }
                break;
            case (PAD_UP | PAD_LEFT):
                MARIA.facingLeft = true;
                if (MARIA.velocityX < FIX(-0.625)) {
                    MarDecelerateX_0924E7C8(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityX = FIX(-0.625);
                }

                if (MARIA.velocityY < FIX(-0.625)) {
                    MarDecelerateX_0924E868(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityY = FIX(-0.625);
                }
                break;

            case (PAD_DOWN | PAD_RIGHT):
                MARIA.facingLeft = false;
                if (MARIA.velocityX > FIX(0.625)) {
                    MarDecelerateX_0924E7C8(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityX = FIX(0.625);
                }
                if (MARIA.velocityY > FIX(0.625)) {
                    MarDecelerateX_0924E868(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityY = FIX(0.625);
                }
                break;
            case (PAD_DOWN | PAD_LEFT):
                MARIA.facingLeft = 1;
                if (MARIA.velocityX < FIX(-0.625)) {
                    MarDecelerateX_0924E7C8(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityX = FIX(-0.625);
                }

                if (MARIA.velocityY > FIX(0.625)) {
                    MarDecelerateX_0924E868(FIX(1.0 / 128.0));
                } else {
                    MARIA.velocityY = FIX(0.625);
                }
                break;
            }
            break;

        default:
            FntPrint("error step\n");
            break;
        }

        if (D_pspeu_0926BBB0 != 0) {
            if (D_pspeu_0926BBB0 > 0) {
                D_pspeu_0926BBB0--;
                MARIA.posY.i.hi++;
            } else {
                D_pspeu_0926BBB0++;
                MARIA.posY.i.hi--;
            }
        }
    }
}

s16 D_pspeu_09263008[] = {
    0, 0, 256, 0, -256, 0, 256, 0, -256, -512, -256, 0, 256, 512, 256, 0};
u8 D_pspeu_09263028[] = {0, 17, 4, 21, 1, 16, 3, 35};
u8 D_pspeu_09263030[] = {4, 5, 10, 11, 14, 15, 29, 30, 4, 3, 0, 0, 0, 0, 0, 0};

// local copy of DopplegangerStepUnmorphMist (see bo4/unk_46E7C.c)
void func_pspeu_0924C200(void) {
    s32 i;
    s32 count;

    if ((g_Maria.vram_flag & TOUCHING_GROUND) && MARIA.velocityY > 0) {
        MARIA.velocityY = 0;
    }
    if ((g_Maria.vram_flag & TOUCHING_CEILING) && MARIA.velocityY < 0) {
        MARIA.velocityY = 0;
    }

    MarDecelerateX_0924E7C8(FIX(1.0 / 128.0));
    MarDecelerateX_0924E868(FIX(1.0 / 128.0));
    count = 0;

    for (i = 0; i < 4; i++) {
        if (g_MarSensorsFloor[i].y < D_pspeu_092630A0[i]) {
            g_MarSensorsFloor[i].y++;
        } else {
            count++;
        }
        if (g_MarSensorsCeiling[i].y > D_pspeu_09263098[i]) {
            g_MarSensorsCeiling[i].y--;
        } else {
            count++;
        }
        if (i == 0 && (g_Maria.vram_flag & TOUCHING_ANY_SLOPE)) {
            MARIA.posY.i.hi--;
        }
    }

    if (count == 8) {
        MARIA.animSet = ANIMSET_OVL(1);
        SetDopplegangerAnim(0xCB);
        if (MARIA.step_s) {
            SetDopplegangerAnim(0xCC);
        }

        if (g_Entities[E_BOSS_WEAPON].step < 3) {
            g_Entities[E_BOSS_WEAPON].step = 3;
            return;
        }
        if (g_Entities[E_BOSS_WEAPON].step == 5) {
            MARIA.palette = PAL_FLAG(0x200);
            func_pspeu_0924D4E8();
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x5B), 0);
            func_pspeu_0924CD20();
            if (!(g_Maria.vram_flag & TOUCHING_ANY_SLOPE)) {
                MARIA.velocityY = FIX(-1);
            }
            func_pspeu_09248828();
        }
    }
}

extern s32 D_pspeu_0926C090;
// local copy of DopplegangerStepSwordWarp (see bo4/unk_46E7C.c)
void func_pspeu_0924C498(void) {
    if (MARIA.step_s == 0) {
        if (g_Entities[E_BOSS_WEAPON].entityId == E_NONE) {
            D_pspeu_0926C090 = 0x10;
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(61, 0x15), 0);
            MARIA.step_s++;
        }
    } else if (--D_pspeu_0926C090 == 0) {
        MARIA.palette = PAL_FLAG(0x200);
        func_pspeu_0924CA58(0);
    }
}

// rotation angle table for the stone wobble (US D_us_8018134C)
extern s16 D_pspeu_09263008[];
extern s32 D_pspeu_0926C088;
// local copy of DopplegangerStepStone (see bo4/unk_46E7C.c)
void func_pspeu_0924C550(s32 arg0) {
    switch (MARIA.step_s) {
    case 0:
        func_pspeu_09249838();
        func_80159C04();
        MARIA.velocityY = FIX(-4);
        func_pspeu_0924EA50(FIX(-0.625));
        func_pspeu_092497C0();
        MARIA.palette = PAL_FLAG(PAL_CC_STONE_EFFECT);
        g_api.PlaySfx(SFX_VO_DOP_PAIN_F);
        g_Maria.timers[ALU_T_HITEFFECT] = 0;
        g_Maria.unk5E = 8;
        MARIA.step_s = 1;
        break;

    case 1:
        func_pspeu_0924E788(1, 4);
        MARIA.palette = PAL_FLAG(PAL_CC_STONE_EFFECT);
        if (func_pspeu_0924EA98(0x20280) != 0) {
            MARIA.step = Dop_StatusStone;
            MARIA.velocityX = MARIA.velocityY = 0;
            g_api.ShakeCamera(1);
            g_api.PlaySfx(SFX_WALL_DEBRIS_B);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(39, 0), 0);
            if (g_Maria.unk6A <= 0) {
                D_pspeu_0926C088 = 0x20;
            }
            MARIA.palette = PAL_FLAG(PAL_UNK_19E);
            SetDopplegangerAnim(0x38);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(16, 3), 0);
            MARIA.step_s = 2;
        }
        break;

    case 2:
        if (g_Maria.unk6A <= 0) {
            if (--D_pspeu_0926C088 == 0) {
                MARIA.step = Dop_Kill;
                g_api.PlaySfx(SFX_VO_DOP_DEATH);
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(16, 3), 0);
                MARIA.step_s = 16;
            }
            func_pspeu_0924E788(1, 4);
            break;
        }

        if ((g_Maria.padTapped & PAD_DIRECTION_MASK) || arg0 != 0) {
            g_Maria.padTapped |= PAD_DIRECTION_MASK;
            g_Maria.unk5E--;
            MARIA.poseTimer = 16;
            g_api.PlaySfx(SFX_STONE_MOVE_B);

            if (g_Maria.unk5E == 0) {
                SetDopplegangerAnim(0x3B);
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(16, 3), 0);
                g_api.PlaySfx(SFX_VO_DOP_YELL);
                MARIA.step = Dop_Hit;
                MARIA.step_s = 8;
                MARIA.palette = PAL_FLAG(0x200);
                break;
            }
            func_pspeu_0924E788(1, 4);
            MARIA.step_s = 3;
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(13, 3), 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(31, 3), 0);
        }
        MARIA.palette = PAL_FLAG(PAL_UNK_19E);
        break;

    case 3:
        if (MARIA.poseTimer < 0) {
            MARIA.step_s = 2;
            MARIA.drawFlags &=
                ENTITY_BLINK | ENTITY_MASK_B | ENTITY_MASK_G | ENTITY_MASK_R |
                ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
        } else {
            MARIA.rotPivotX = 0;
            MARIA.drawFlags |= ENTITY_ROTATE;
            MARIA.rotate = D_pspeu_09263008[MARIA.poseTimer] >> 0x4;
            if (MARIA.rotate) {
                MARIA.rotPivotY = 20;
            } else {
                MARIA.rotPivotY = 24;
            }
        }
        MARIA.palette = PAL_FLAG(PAL_UNK_19E);
        break;
    }
}

// local variant of the crouch setter (see bo4/unk_45354.c func_8010E470)
void func_pspeu_0924C9C8(s32 arg0, s32 velocityX) {
    s32 unused_stack[2];

    MARIA.velocityX = velocityX;
    MARIA.velocityY = 0;
    MARIA.step = Dop_Crouch;
    MARIA.step_s = (s32)D_pspeu_09263028[arg0 * 2];
    SetDopplegangerAnim(D_pspeu_09263028[arg0 * 2 + 1]);
}

// local variant of the stand setter (see bo4/unk_45354.c func_8010E570)
void func_pspeu_0924CA58(s32 arg0) {
    s32 anim = 0;
    bool atLedge = false;

    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        atLedge = true;
    }
    MARIA.velocityX = arg0;
    MARIA.velocityY = 0;
    SetDopplegangerStep(Dop_Stand);
    if (g_Maria.unk48) {
        MARIA.step_s = 2;
        atLedge = false;
    }

    switch (g_Maria.prev_step) {
    case Dop_UnmorphBat:
        anim = 4;
        break;
    case Dop_Walk:
        anim = 4;
        if (MARIA.ext.player.anim == 9) {
            MARIA.ext.player.anim = D_pspeu_09263030[2 + atLedge];
            return;
        }
        if (MARIA.ext.player.anim == 7) {
            anim = 0;
        }
        break;
    case Dop_Jump:
    case Dop_Fall:
        anim = 6;
        if (abs(MARIA.velocityX) > FIX(2.5)) {
            anim = 4;
        }
        break;
    default:
        anim = 8;
        break;
    }
    anim += atLedge;
    SetDopplegangerAnim(D_pspeu_09263030[anim]);
}

// local variant of the walk setter (see bo4/unk_45354.c func_8010E6AC)
void func_pspeu_0924CBF0(s32 forceAnim13) {
    bool atLedge;

    atLedge = false;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        atLedge = true;
    }

    MarSetSpeedX(FIX(1.5));
    MARIA.velocityY = 0;
    SetDopplegangerStep(Dop_Walk);

    if (forceAnim13) {
        if (MARIA.ext.player.anim != 13) {
            SetDopplegangerAnim(13);
        }
    } else {
        SetDopplegangerAnim(7);
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(1, 5), 0);
    }

    if (g_Maria.unk4C) {
        MARIA.ext.player.anim = 9;
    }

    if (MARIA.ext.player.anim == 7 && atLedge) {
        MARIA.pose = 1;
    }

    if (g_Maria.prev_step == Dop_Crouch) {
        MARIA.pose = 4;
    }
}

// local variant of the jump setter (see bo4/unk_45354.c func_us_801C58E4)
void func_pspeu_0924CD20(void) {
    if (MarCheckFacing() != 0) {
        SetDopplegangerAnim(0x1A);
        MarSetSpeedX(FIX(3.0 / 2.0));
        g_Maria.unk44 = 0;
    } else {
        SetDopplegangerAnim(0x16);
        MARIA.velocityX = 0;
        g_Maria.unk44 = 4;
    }
    MARIA.velocityY = FIX(-4.875);
    SetDopplegangerStep(Dop_Jump);
    if (g_Maria.prev_step == Dop_Walk) {
        g_Maria.unk44 |= 0x10;
    }
}

// local variant of the double jump setter (see bo4/unk_45354.c
// func_us_801C5990)
void func_pspeu_0924CDE0(void) {
    g_Maria.unk44 |= 0x21;
    MARIA.velocityY = FIX(-4.25);
    SetDopplegangerAnim(0x20);
    MARIA.step_s = 0;
}

// local variant of the fall setter (see bo4/unk_45354.c func_us_801C59DC)
void func_pspeu_0924CE30(void) {
    SetDopplegangerStep(Dop_Fall);
    if (g_Maria.prev_step != Dop_Walk) {
        SetDopplegangerAnim(0x1C);
    }
    MARIA.velocityX = 0;
    MARIA.velocityY = FIX(2.0);
    g_Maria.timers[ALU_T_5] = 8;
    g_Maria.timers[ALU_T_6] = 8;
    g_Maria.unk44 = 0x10;
}

// local copy of func_us_801C504C (see rbo5/unk_44954.c)
void func_pspeu_0924CEB0(void) {
    if (MarCheckFacing() != 0) {
        MarSetSpeedX(FIX(3));
    } else {
        DOPPLEGANGER.velocityX = 0;
    }
    SetDopplegangerStep(9);
    DOPPLEGANGER.velocityY = FIX(-12);
    SetDopplegangerAnim(0x21);
    g_Dop.gravBootTimer = 0;
    g_Dop.unk44 &= 0xFFFE;
    MarCreateEntFactoryFromEntity(g_CurrentEntity, 2, 0);
}

// local variant of the up-throw subweapon check (see rbo5/unk_44954.c
// func_us_801C5168): counts live rebound stones before spawning another
s32 func_pspeu_0924CF50(void) {
    Entity* entity;
    s32 i;
    s32 entityCount;
    s32 targetCount;
    s32 animBase;
    s32 playerAnimOffset;
    s32 animOffset;

    playerAnimOffset = 0;
    if (!(g_Maria.padPressed & PAD_UP)) {
        return 1;
    }
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        playerAnimOffset = 1;
    }

    targetCount = 3;
    for (entity = &g_Entities[96], i = 0, entityCount = 0; i < 16; i++,
        entity++) {
        if (entity->entityId == E_EXPLOSION_VARIANTS) {
            entityCount++;
        }
        if (entityCount >= targetCount) {
            return -1;
        }
    }

    MarCreateEntFactoryFromEntity(g_CurrentEntity, BP_REBOUND_STONE, 0);

    g_Maria.timers[ALU_T_USE_SUBWPN] = 4;
    if (MARIA.step_s >= 0x40) {
        return 0;
    }

    animBase = 0x5D;
    switch (MARIA.step) {
    case Dop_Stand:
        animOffset = playerAnimOffset;
        SetDopplegangerAnim(animBase + animOffset);
        break;
    case Dop_Crouch:
        animOffset = 2;
        if (MARIA.step_s == 2) {
            animOffset = playerAnimOffset;
            SetDopplegangerStep(1);
        }
        SetDopplegangerAnim(animBase + animOffset);
        break;
    }
    return 0;
}

// local variant of the subweapon handler (see bo4/unk_45354.c
// func_us_801C5B68); unlike bo4, the throw needs distance to the real
// player instead of a specific button
s32 func_pspeu_0924D108(void) {
    s16 var_s4;
    s16 animBase;
    s32 playerAnimOffset;
    s32 tapped;

    playerAnimOffset = 0;
    if (g_Maria.vram_flag & IN_AIR_OR_EDGE) {
        playerAnimOffset = 1;
    }
    tapped = g_Maria.padTapped & (PAD_SQUARE | PAD_CIRCLE);
    animBase = (s16)func_pspeu_0924CF50();
    if (!animBase) {
        return 1;
    }
    if (animBase < 0) {
        return 0;
    }
    if (g_Maria.unk46 & 0x8000) {
        return 0;
    }
    if (g_Maria.timers[ALU_T_CURSE]) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(57, 1), 0);
        switch (MARIA.step) {
        case Dop_Stand:
        case Dop_Walk:
            SetDopplegangerAnim(0xB5);
            MARIA.step = Dop_Stand;
            break;
        case Dop_Crouch:
            SetDopplegangerAnim(0xB6);
            MARIA.step = Dop_Crouch;
            break;
        case Dop_Fall:
        case Dop_Jump:
            SetDopplegangerAnim(0xB7);
            MARIA.step = Dop_Jump;
            break;
        }
        g_Maria.unk46 = 0x8012;
        g_Maria.unk54 = 0xFF;
        MARIA.step_s = 0x51;
        g_api.PlaySfx(SFX_VO_DOP_PAIN_A);
        return 1;
    }
    if (abs(MARIA.posX.i.hi - PLAYER.posX.i.hi) > 0x24) {
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(48, 0), 0);
        MARIA.step_s = 0x41;
        g_Maria.unk46 = 0x8002;
        g_Maria.unk54 = 0xD;
        animBase = 0x41;
    } else {
        g_Maria.unk46 = 0x8003;
        MARIA.step_s = 0x42;
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(48, 1), 0);
        g_Maria.unk54 = 8;
        animBase = 0xA7;
    }
    switch (MARIA.step) {
    case Dop_Stand:
    case Dop_Walk:
        g_CurrentEntity->velocityX = g_CurrentEntity->velocityX >> 1;
        MARIA.step = Dop_Stand;
        var_s4 = playerAnimOffset;
        break;
    case Dop_Crouch:
        var_s4 = 2;
        if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
            var_s4++;
        }
        if (MARIA.step_s == 2) {
            var_s4 = playerAnimOffset;
            MARIA.step = Dop_Stand;
        }
        break;
    case Dop_Fall:
    case Dop_Jump:
        var_s4 = 4;
        if (MARIA.velocityY > 0) {
            var_s4++;
            if (g_Maria.padPressed & PAD_DOWN) {
                var_s4++;
            }
        }
        break;
    }
    SetDopplegangerAnim(animBase + var_s4);
    g_Maria.timers[ALU_T_9] = 4;
    return 1;
}

// destroys the subweapon crash entity and clears the attack state
void func_pspeu_0924D4E8(void) {
    Entity* entity = &g_Entities[80];

    DestroyEntity(entity);
    g_Maria.unk46 = 0;
}

// backdash; anim 0xDB is the gate the input router checks
void func_pspeu_0924D528(void) {
    MARIA.step = Dop_Stand;
    MARIA.step_s = 3;
    MarSetSpeedX(FIX(-3.5));
    g_CurrentEntity->velocityY = 0;
    SetDopplegangerAnim(0xDB);
    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
}

void SetDopplegangerStep(s16 step) {
    DOPPLEGANGER.step = step;
    DOPPLEGANGER.step_s = 0;
}

static u8 g_afterImageTimerTable[] = {
    10, 8, 8, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 255, 255, 0, 0};

extern PlayerState g_Dop;

// Same function in RIC is func_8015C4AC (InitRicAfterImage)
void InitPlayerAfterImage(void) {
    byte stackpad[40];
    Primitive* prim;
    s32 i;

    if (g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
            .ext.afterImage.disableFlag) {
        return;
    }

    switch (DOPPLEGANGER.ext.player.anim) {
    case 0x5E:
    case 0x5D:
    case 0x60:
    case 0x61:
    case 0x62:
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.index =
            10;
        return;
    }

    if ((g_Dop.padTapped & GAMEBUTTONS) ||
        ((g_Dop.padHeld ^ g_Dop.padPressed) & g_Dop.padHeld & GAMEBUTTONS) ||
        (DOPPLEGANGER.velocityY > FIX(0.5))) {
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.index =
            0;
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.timer =
            0;
    } else {
        if (g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                .ext.afterImage.index >= MaxAfterImageIndex) {
            return;
        }
        if (g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                .ext.afterImage.timer == 0) {
            g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                .ext.afterImage.timer = g_afterImageTimerTable
                [g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                     .ext.afterImage.index];
        }
        if (--g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                  .ext.afterImage.timer == 0) {
            g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                .ext.afterImage.index++;
            g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                .ext.afterImage.timer = g_afterImageTimerTable
                [g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                     .ext.afterImage.index];
        }
    }
    if (g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].pose) {
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].pose--;
        return;
    }
    prim =
        &g_PrimBuf[g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].primIndex];
    i = 0;
    while (prim) {
        if (i == g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].entityId) {
            prim->r0 = prim->g0 = prim->b0 = 0x80;
            prim->x0 = DOPPLEGANGER.posX.i.hi;
            prim->y0 = DOPPLEGANGER.posY.i.hi;
            prim->x1 = DOPPLEGANGER.animCurFrame;
            prim->y1 = 0;
            prim->x2 = DOPPLEGANGER.facingLeft;
            prim->y2 = DOPPLEGANGER.palette;
        }
        i++;
        prim = prim->next;
    }

    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].pose = 2;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].entityId++;
    if (g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].entityId >=
        MaxAfterImages) {
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].entityId = 0;
    }
}

static u8 g_shadowOpacityReductionTable[] = {
    4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 16, 16, 16, 16, 16, 16};
static u8 g_D_800ACF3C[] = {
    8, 12, 16, 20, 24, 28, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};

// Equivalent in RIC is func_8015C6D4 (DrawRicAfterImage)
void DrawPlayerAfterImage(void) {
    PlayerDraw pad;
    PlayerDraw* plDraw;
    Primitive* prim;
    s32 i;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2;

    // PSP zeroes the index right after fetching prim
    temp_t2 = g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                  .ext.afterImage.resetFlag;
    prim =
        &g_PrimBuf[g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].primIndex];
    i = 0;
    plDraw = &g_PlayerDraw[9];
    temp_t1 = g_shadowOpacityReductionTable
        [g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.index];
    temp_t0 = g_D_800ACF3C[g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
                               .ext.afterImage.index];

    for (; prim != NULL; i++, prim = prim->next) {
        if (prim->b0 > temp_t0) {
            prim->b0 -= temp_t1;
        }
        if (prim->b0 < 112 && prim->r0 < 240) {
            prim->r0 += 6;
        }
        if (prim->b0 < 88) {
            prim->y1 = 16;
        } else {
            prim->y1 = 0;
        }
        if (prim->b0 <= temp_t0) {
            prim->x1 = 0;
        }
        if (!((i ^ g_Timer) & 1)) {
            continue;
        }

        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].posX.i.hi =
            prim->x0;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].posY.i.hi =
            prim->y0;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].animCurFrame =
            prim->x1;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].blendMode =
            prim->y1;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].facingLeft =
            prim->x2;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].palette =
            prim->y2;
        g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1].zPriority =
            DOPPLEGANGER.zPriority - 2;
        if (temp_t2) {
            g_Entities[STAGE_ENTITY_START + (i / 2) + E_AFTERIMAGE_1]
                .animCurFrame = 0;
            prim->x1 = 0;
        }

        PBLU(plDraw) = PGRN(plDraw) = prim->b0;
        PRED(plDraw) = prim->r0;
        plDraw->enableColorBlend = true;
        plDraw++;
    }
}

// not present in the PSP builds of this block

// K&R definition: the psp callers predate the prototype and pass
// promoted ints
void SetDopplegangerAnim(anim) u8 anim;
{
    g_CurrentEntity->ext.player.anim = anim;
    g_CurrentEntity->poseTimer = 0;
    g_CurrentEntity->pose = 0;
}

extern AnimationFrame* g_DopAnimationFrames[];

static AnimationFrame* func_8010DA70(AnimationFrame** frames) {
    u16* anim;
    s32 idx;
    u16* subanim;
    s32 var_s1;
    s32 idxSub;

    anim = (u16*)frames[g_CurrentEntity->ext.player.anim];
    idx = 0;
    var_s1 = 0;
    while (true) {
        if ((&anim[idx * 2])[0] == 0xFFFD) {
            for (idxSub = 0; true; idxSub++, var_s1++) {
                subanim =
                    (u16*)g_DopAnimationFrames[(&anim[idx * 2])[1] & 0xFF];
                if ((&subanim[idxSub * 2])[0] == 0xFFFF) {
                    idx++;
                    // Probably fake. Makes PSP registers match.
                    idxSub = idxSub;
                    break;
                }
                if (var_s1 == g_CurrentEntity->pose) {
                    return (AnimationFrame*)&subanim[idxSub * 2];
                }
            }
        } else if (var_s1 == g_CurrentEntity->pose) {
            return (AnimationFrame*)(anim + idx * 2);
        } else {
            var_s1++;
            idx++;
        }
    }
}

s32 UpdateUnarmedAnim(s8* frameProps, u16** frames) {
    u16* frameIndex;

    frameIndex = frames[g_CurrentEntity->ext.player.anim] + DOPPLEGANGER.pose;
    if (*frameIndex == 0xFFFF) {
        return -1;
    }
    if (frameProps != NULL) {
        frameProps = &frameProps[((*frameIndex >> 9) & 0x7F) << 2];
        g_CurrentEntity->hitboxOffX = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxOffY = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxWidth = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxHeight = *frameProps;
    }
    g_CurrentEntity->animCurFrame = *frameIndex & 0x1FF;
    if (DOPPLEGANGER.poseTimer < 0) {
        return -1;
    } else {
        return 0;
    }
}

void PlayAnimation(s8* frameProps, AnimationFrame** frames) {
    AnimationFrame* animFrame;

    animFrame = func_8010DA70(frames);
    if (g_CurrentEntity->poseTimer != -1) {
        if (g_CurrentEntity->poseTimer == 0) {
            g_CurrentEntity->poseTimer = animFrame->duration;
        } else if (--g_CurrentEntity->poseTimer == 0) {
            g_CurrentEntity->pose++;
            animFrame = func_8010DA70(frames);
            // Using a switch doesn't work
            if (animFrame->duration == 0x0) {
                g_CurrentEntity->pose = animFrame->pose;
                animFrame = func_8010DA70(frames);
                g_CurrentEntity->poseTimer = animFrame->duration;
            } else if (animFrame->duration == 0xFFFF) {
                g_CurrentEntity->pose--;
                g_CurrentEntity->poseTimer = -1;
                animFrame = func_8010DA70(frames);
            } else if (animFrame->duration == 0xFFFE) {
                g_CurrentEntity->ext.player.anim = animFrame->pose & 0xFF;
                g_CurrentEntity->pose = animFrame->pose >> 8;
                animFrame = func_8010DA70(frames);
                g_CurrentEntity->poseTimer = animFrame->duration;
            } else {
                g_CurrentEntity->poseTimer = animFrame->duration;
            }
        }
    }

    if (frameProps != NULL) {
        // This is ugly - theoretically the type for frameProps should be
        // FrameProperty* but anything besides this where we assign this big
        // expression fails.

        // Please check function UpdateAnim down below
        frameProps = &frameProps[((animFrame->pose >> 9) & 0x7F) * 4];
        g_CurrentEntity->hitboxOffX = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxOffY = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxWidth = *frameProps;
        frameProps++;
        g_CurrentEntity->hitboxHeight = *frameProps;
    }
    g_CurrentEntity->animCurFrame = animFrame->pose & 0x1FF;
}

// Nasty casting. This is just
// g_CurrentEntity->anim[g_CurrentEntity->pose] But PSP is weird and
// does odd stuff with the struct indexing. So we cast the pointer to u16, index
// off of pose*2, and cast back to AnimationFrame.
#define CURRANIM                                                               \
    (*((AnimationFrame*)(&(                                                    \
        ((u16*)g_CurrentEntity->anim)[g_CurrentEntity->pose * 2]))))

u32 UpdateAnim(s8* hitboxes, AnimationFrame** anims) {
#if defined(FIX_UB)
    s32 ret = 0;
#else
    s32 ret;
#endif

    if (g_CurrentEntity->poseTimer == -1) {
        ret = -1;
    } else if (g_CurrentEntity->poseTimer == 0) {
        g_CurrentEntity->poseTimer = CURRANIM.duration;
        ret = 0;
    } else if ((--g_CurrentEntity->poseTimer) == 0) {
        g_CurrentEntity->pose++;
        // Effectively a switch statement, but breaks if I actually use one.
        if (CURRANIM.duration == 0) {
            g_CurrentEntity->pose = CURRANIM.pose;
            g_CurrentEntity->poseTimer = CURRANIM.duration;
            ret = 0;
        } else if (CURRANIM.duration == 0xFFFF) {
            g_CurrentEntity->pose--;
            g_CurrentEntity->poseTimer = -1;
            ret = -1;
        } else if (CURRANIM.duration == 0xFFFE) {
            g_CurrentEntity->anim = anims[CURRANIM.pose];
            g_CurrentEntity->pose = 0;
            g_CurrentEntity->poseTimer = CURRANIM.duration;
            ret = -2;
        } else {
            g_CurrentEntity->poseTimer = CURRANIM.duration;
        }
    }
    if (hitboxes) {
        // This is ugly - theoretically the type for frameProps should be
        // FrameProperty* but anything besides this where we assign this big
        // expression fails.
        hitboxes = &hitboxes[((CURRANIM.pose >> 9) & 0x7F) << 2];
        g_CurrentEntity->hitboxOffX = *hitboxes;
        hitboxes++;
        g_CurrentEntity->hitboxOffY = *hitboxes;
        hitboxes++;
        g_CurrentEntity->hitboxWidth = *hitboxes;
        hitboxes++;
        g_CurrentEntity->hitboxHeight = *hitboxes;
    }
    g_CurrentEntity->animCurFrame = CURRANIM.pose & 0x1FF;
    return ret;
}

typedef struct {
    AnimationFrame* frames;
    s8* frameProps;
    u16 soundId;
    u8 frameStart;
    u8 soundFrame;
} DopWeaponAnimation;

s16 D_pspeu_09263078[] = {-8, -8, -8, -8};

s16 D_pspeu_09263080[] = {8, 8, 8, 8};

s16 D_pspeu_09263088[] = {7, 0, 0, 0, 0, 0, -7, 0};

s16 D_pspeu_09263098[] = {-22, -22, -22, -22};

s16 D_pspeu_092630A0[] = {29, 25, 25, 25};

s16 D_pspeu_092630A8[] = {24, 17, 9, 1, -7, -14, -21, 0};

Point16 g_MarSensorsCeiling[] = {{0, -22}, {0, -22}, {4, -22}, {-4, -22}};

Point16 g_MarSensorsFloor[] = {{0, 29}, {0, 25}, {4, 25}, {-4, 25}};

Point16 g_MarSensorsWall[] = {
    {5, 24},  {5, 17},  {5, 9},  {5, 1},  {5, -7},  {5, -14},  {5, -21},
    {-5, 24}, {-5, 17}, {-5, 9}, {-5, 1}, {-5, -7}, {-5, -14}, {-5, -21}};

u8 D_pspeu_09263110[] = {
    1, 2, 2,  4, 3,  4, 4,  4, 5,  4, 6,   6,   7, 6, 8, 6,
    9, 6, 10, 8, 11, 8, 12, 8, 13, 8, 255, 255, 0, 0, 0, 0};

u8 D_pspeu_09263130[] = {
    14, 10, 15, 12, 16, 12, 17, 12, 18, 12, 19,  14,  20, 14, 21, 14,
    22, 14, 31, 8,  32, 8,  33, 8,  34, 8,  255, 255, 0,  0,  0,  0};

u8 D_pspeu_09263150[] = {
    14, 10, 23, 16, 24, 18, 25, 16, 26, 18, 27,  20,  28, 20, 29, 20,
    30, 20, 31, 8,  32, 8,  33, 8,  34, 8,  255, 255, 0,  0,  0,  0};

u8 D_pspeu_09263170[] = {35, 22, 36, 24, 37, 24, 38, 24, 39,  24,  40, 26,
                         41, 26, 42, 26, 43, 26, 44, 8,  255, 255, 0,  0};

u8 D_pspeu_09263188[] = {45, 28, 46, 30, 47, 30, 48, 30, 49,  30,  50, 32,
                         51, 32, 52, 32, 53, 32, 62, 8,  255, 255, 0,  0};

u8 D_pspeu_092631A0[] = {45, 28, 54, 34, 55, 34, 56, 34, 57,  34,  58, 36,
                         59, 36, 60, 36, 61, 36, 62, 8,  255, 255, 0,  0};

s8 D_pspeu_092631B8[] = {
    0,   0,   0,   0,   4,   -23, 8,  5,  30,  -17, 35, 8,  30,  -14, 18,
    4,   -32, -31, 0,   0,   -4,  -6, 8,  5,   22,  0,  35, 8,   22,  3,
    18,  4,   29,  22,  13,  11,  12, 7,  19,  12,  14, 14, 18,  4,   1,
    -25, 8,   5,   27,  -19, 35,  8,  27, -16, 18,  4,  -8, -26, 8,   5,
    24,  -17, 35,  8,   24,  -14, 18, 4,  23,  0,   27, 14, 18,  2,   21,
    11,  29,  -14, 34,  4,   21,  3,  34, 4,   28,  21, 12, 10,  26,  -16,
    34,  4,   23,  -14, 34,  4,   22, 3,  26,  10,  0,  0,  0,   0};

u8 D_pspeu_09263220[] = {
    1, 2, 2, 4, 3, 4, 4, 6, 5, 6, 6, 6, 7, 6, 8, 6, 9, 6, 255, 255, 0, 0, 0, 0};

u8 D_pspeu_09263238[] = {10, 8, 11, 10, 12, 10, 13,  6,   14, 6, 19, 6,
                         20, 6, 21, 6,  22, 6,  255, 255, 0,  0, 0,  0};

u8 D_pspeu_09263250[] = {10, 8, 15, 12, 16, 14, 17,  16,  18, 6, 19, 6,
                         20, 6, 21, 6,  22, 6,  255, 255, 0,  0, 0,  0};

u8 D_pspeu_09263268[] = {
    23, 18, 24, 20, 25, 20, 26, 6, 27, 6, 28, 6, 255, 255, 0, 0};

u8 D_pspeu_09263278[] = {
    29, 22, 30, 24, 31, 24, 32, 6, 33, 6, 38, 6, 255, 255, 0, 0};

u8 D_pspeu_09263288[] = {
    29, 22, 34, 26, 35, 28, 36, 6, 37, 6, 38, 6, 255, 255, 0, 0};

s8 D_pspeu_09263298[] = {
    0,  0,   0,  0, 3,  -24, 5,  4,  25, -13, 24, 3, -32, -31, 0,  0,
    -5, -5,  5,  4, 17, 4,   24, 3,  7,  15,  14, 8, 19,  21,  15, 7,
    13, 18,  17, 5, -1, -25, 5,  4,  23, -15, 24, 7, -9,  -27, 5,  4,
    19, -14, 24, 7, 15, 2,   19, 10, 29, 8,   11, 8, 0,   0,   0,  0};

AnimationFrame* D_pspeu_092632D8[] = {
    (AnimationFrame*)D_pspeu_09263110, (AnimationFrame*)D_pspeu_09263110,
    (AnimationFrame*)D_pspeu_09263130, (AnimationFrame*)D_pspeu_09263150,
    (AnimationFrame*)D_pspeu_09263170, (AnimationFrame*)D_pspeu_09263188,
    (AnimationFrame*)D_pspeu_092631A0, NULL};

AnimationFrame* D_pspeu_092632F8[] = {
    (AnimationFrame*)D_pspeu_09263220, (AnimationFrame*)D_pspeu_09263220,
    (AnimationFrame*)D_pspeu_09263238, (AnimationFrame*)D_pspeu_09263250,
    (AnimationFrame*)D_pspeu_09263268, (AnimationFrame*)D_pspeu_09263278,
    (AnimationFrame*)D_pspeu_09263288, NULL};

DopWeaponAnimation D_pspeu_09263318[] = {
    {(AnimationFrame*)D_pspeu_092632D8, D_pspeu_092631B8, 0x60B, 0x41, 4},
    {(AnimationFrame*)D_pspeu_092632F8, D_pspeu_09263298, 0x60B, 0xA7, 4},
    {(AnimationFrame*)D_pspeu_092632D8, D_pspeu_092631B8, 0x60B, 0x41, 4},
};

u8 D_pspeu_09263340[][4] = {
    {0x10, 0x7F, 0x3F, 0x00}, {0x10, 0x7F, 0x00, 0x00},
    {0x10, 0x3F, 0x3F, 0x3F}, {0x10, 0x5F, 0x2F, 0x00},
    {0x0C, 0x7F, 0x3F, 0x00}, {0x0C, 0x7F, 0x3F, 0x7F},
    {0x02, 0x7F, 0x3F, 0x7F}, {0x06, 0x00, 0x3F, 0x7F},
    {0x06, 0x3F, 0x3F, 0x3F}, {0x10, 0x3F, 0x3F, 0x7F},
    {0x10, 0xFF, 0xFF, 0xFF}, {0x12, 0x7F, 0x00, 0x00},
    {0x12, 0x7F, 0x7F, 0x7F}, {0x00, 0x00, 0x00, 0x00}};

u8 D_pspeu_09263378[][8] = {
    {0x00, 0x50, 0x10, 0x50, 0x00, 0x60, 0x10, 0x60},
    {0x10, 0x50, 0x20, 0x50, 0x10, 0x60, 0x20, 0x60},
    {0x70, 0x40, 0x80, 0x40, 0x70, 0x50, 0x80, 0x50},
    {0x70, 0x30, 0x78, 0x30, 0x70, 0x38, 0x78, 0x38},
    {0x78, 0x30, 0x80, 0x30, 0x78, 0x38, 0x80, 0x38},
    {0x70, 0x38, 0x78, 0x38, 0x77, 0x40, 0x78, 0x40}};

// local copy of func_us_801C4954 (see rbo5/unk_44954.c); the
// primitive index still comes from the player afterimage slot.
void MarDisableAfterImage(s32 resetAnims, s32 arg1) {
    Primitive* prim;

    if (resetAnims) {
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
            .ext.disableAfterImage.resetFlag = 1;
        g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].animCurFrame =
            g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_2].animCurFrame =
                g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_3].animCurFrame =
                    0;
        prim = &g_PrimBuf[g_Entities[E_AFTERIMAGE_1].primIndex];
        while (prim != NULL) {
            prim->x1 = 0;
            prim = prim->next;
        }
    }
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
        .ext.disableAfterImage.disableFlag = 1;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
        .ext.disableAfterImage.index = MaxAfterImageIndex;
    if (arg1) {
        if (arg1 < 4) {
            g_Dop.timers[15] = 4;
        } else {
            g_Dop.timers[15] = arg1;
        }
    }
}

void func_pspeu_0924E770(void) {
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.resetFlag =
        0;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.disableFlag =
        0;
}

// reduced variant of func_us_801C5430 (see bo4/unk_45354.c): a0 is ignored
void func_pspeu_0924E788(s16 a0, s16 minTime) {
    if (g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] <= minTime) {
        g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] = minTime;
    }
}

// local copy of DecelerateX (see decelerate.h)
void MarDecelerateX_0924E7C8(s32 speed) {
    if (g_CurrentEntity->velocityX < 0) {
        g_CurrentEntity->velocityX += speed;
        if (g_CurrentEntity->velocityX > 0) {
            g_CurrentEntity->velocityX = 0;
        }
    } else {
        g_CurrentEntity->velocityX -= speed;
        if (g_CurrentEntity->velocityX < 0) {
            g_CurrentEntity->velocityX = 0;
        }
    }
}

// local copy of DecelerateY (see decelerate.h)
void MarDecelerateX_0924E868(s32 speed) {
    if (g_CurrentEntity->velocityY < 0) {
        g_CurrentEntity->velocityY += speed;
        if (g_CurrentEntity->velocityY > 0) {
            g_CurrentEntity->velocityY = 0;
        }
    } else {
        g_CurrentEntity->velocityY -= speed;
        if (g_CurrentEntity->velocityY < 0) {
            g_CurrentEntity->velocityY = 0;
        }
    }
}

#include "../../mar_check_facing.h"

#include "../../mar_set_speed_x.h"

void func_pspeu_0924EA50(s32 velocityX) {
    if (MARIA.entityRoomIndex == 1) {
        velocityX = -velocityX;
    }
    MARIA.velocityX = velocityX;
}

// local variant of MarCheckInput (see maria/pl_collision.c): same CHECK_*
// bit layout, but each gravity block writes velocityY directly (no
// accumulator) and the boss falls at 22/128 per frame instead of 28/128
s32 func_pspeu_0924EA98(s32 checks) {
    if ((checks & 8) && g_Maria.unk46 == 0) {
        MarCheckFacing();
    }
    if (checks & 0x8000) {
        MARIA.velocityY += FIX(22.0 / 128);
        if (MARIA.velocityY > FIX(7)) {
            MARIA.velocityY = FIX(7);
        }
    }
    if (checks & 0x10000) {
        if (MARIA.velocityY < FIX(3.0 / 8) && MARIA.velocityY > FIX(-1.0 / 8) &&
            !(g_Maria.unk44 & 0x20) && (g_Maria.padPressed & PAD_CROSS)) {
            // 4.4 is precisely 1/5 of 22, like maria's 5.6 is of 28
            MARIA.velocityY += FIX(4.4 / 128);
        } else {
            MARIA.velocityY += FIX(22.0 / 128);
            if (MARIA.velocityY > FIX(7)) {
                MARIA.velocityY = FIX(7);
            }
        }
    }
    if ((checks & 0x80) && (g_Maria.vram_flag & TOUCHING_CEILING) &&
        (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-1);
    }
    if (checks & 0x200) {
        if (MARIA.velocityY < FIX(3.0 / 8) && MARIA.velocityY > FIX(-1.0 / 8)) {
            MARIA.velocityY += FIX(11.0 / 128);
        } else {
            MARIA.velocityY += FIX(22.0 / 128);
            if (MARIA.velocityY > FIX(7)) {
                MARIA.velocityY = FIX(7);
            }
        }
    }
    if (MARIA.velocityY >= 0) {
        if ((checks & 1) && (g_Maria.vram_flag & TOUCHING_GROUND)) {
            if (g_Maria.unk46) {
                if ((g_Maria.unk46 & 0x7FFF) == 0xFF) {
                    func_pspeu_0924CA58(0);
                    func_pspeu_0924D4E8();
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                    return 1;
                }
                if (MARIA.velocityY > FIX(6.875)) {
                    func_pspeu_0924C9C8(1, 0);
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                } else {
                    if (g_Maria.unk44 & 0x10) {
                        func_pspeu_0924CBF0(1);
                    } else {
                        func_pspeu_0924CA58(0);
                    }
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                }
                func_pspeu_0924D4E8();
                return 1;
            } else {
                if (MARIA.velocityY > FIX(6.875)) {
                    if (MARIA.step_s == 0x70 || MARIA.step == 5) {
                        func_pspeu_0924C9C8(3, MARIA.velocityX / 2);
                    } else {
                        func_pspeu_0924C9C8(1, 0);
                    }
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                } else if (g_Maria.unk44 & 0x10) {
                    func_pspeu_0924CBF0(1);
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                } else if (abs(MARIA.velocityX) > FIX(2)) {
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                    func_pspeu_0924CA58(MARIA.velocityX);
                } else {
                    g_api.PlaySfx(SFX_STOMP_SOFT_B);
                    func_pspeu_0924CA58(0);
                }
                return 1;
            }
        } else if (
            (checks & 0x20000) && (g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_0924C9C8(3, MARIA.velocityX);
            g_api.PlaySfx(SFX_STOMP_HARD_B);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
            return 1;
        }
    }
    if ((checks & 4) && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        func_pspeu_0924CE30();
        return 1;
    }
    if ((checks & 0x1000) && (g_Maria.padTapped & (PAD_SQUARE | PAD_CIRCLE))) {
        if (func_pspeu_0924D108()) {
            return 1;
        }
    }
    if (!(g_Maria.unk46 & 0x8000)) {
        if ((checks & 0x10) && (g_Maria.padTapped & PAD_CROSS)) {
            func_pspeu_0924CD20();
            return 1;
        }
        if ((checks & 0x20) && (g_Maria.padTapped & PAD_CROSS) &&
            !(g_Maria.unk44 & 1)) {
            func_pspeu_0924CDE0();
            return 1;
        }
        if ((checks & 0x2000) && (g_Maria.padPressed & PAD_DOWN)) {
            func_pspeu_0924C9C8(2, 0);
            return 1;
        }
        if ((checks & 0x40000) && (g_Maria.padTapped & PAD_TRIANGLE) &&
            (MARIA.ext.player.anim != 0xDB)) {
            func_pspeu_0924D528();
            return 1;
        }
    }
    return 0;
}

// local variant of MarCheckFloor (see maria/pl_collision.c); the boss
// walls drop the g_unkGraphicsStruct gate and the quicksand/water tail
// blocks and add g_Maria.status guards around the sensor probes
void MarCheckFloor(void) {
    Collider col;
    s32 i;
    s32 effects2;
    s32 effects;
    s16 yCheck;
    s16 xCheck;
    s16* x;
    s16* y;
    s32* vramFlag;
    s32 vramApply;
    s16 xMod;
    s32 xMax;
    s16 yMod;

    yMod = 0;
    i = 0;
    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    if (MARIA.velocityY == 0 && g_Maria.unk04 & 1 &&
        !(g_Maria.status &
          (PLAYER_STATUS_DEAD | PLAYER_STATUS_STONE | PLAYER_STATUS_MIST_FORM |
           PLAYER_STATUS_BAT_FORM))) {
        yMod = 4;
    } else {
        i = 1;
    }
    for (; i < NUM_HORIZONTAL_SENSORS; i++, yMod = 0) {
        if (g_Maria.colFloor[i].effects & EFFECT_SOLID_FROM_BELOW) {
            continue;
        }
        if (!(g_Maria.colFloor[i].effects & EFFECT_SIDE ||
              MARIA.velocityY >= 0 ||
              g_Maria.status &
                  (PLAYER_STATUS_MIST_FORM | PLAYER_STATUS_BAT_FORM) ||
              abs(MARIA.velocityX) >= FIX(2))) {
            continue;
        }
        effects2 = g_Maria.colFloor[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_0800)) {
            if (g_Maria.status &
                    (PLAYER_STATUS_MIST_FORM | PLAYER_STATUS_BAT_FORM) &&
                !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsFloor[i].x;
            yCheck = *y + g_MarSensorsFloor[i].y;
            yCheck += g_Maria.colFloor[i].unk18 - 1;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (g_Maria.status & PLAYER_STATUS_MIST_FORM &&
                col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_SIDE | EFFECT_SOLID);
            }
            effects = col.effects;
            if (!(effects & EFFECT_SOLID)) {
                if (g_Maria.colFloor[i].effects != EFFECT_SOLID ||
                    MARIA.velocityY >= 0) {
                    if (effects2 & EFFECT_UNK_0800) {
                        *y += yMod + g_Maria.colFloor[i].unk8;
                    } else {
                        *y += yMod + g_Maria.colFloor[i].unk18;
                    }
                    *vramFlag |= 1;
                    return;
                }
                continue;
            }
            if ((effects & (EFFECT_UNK_8000 | EFFECT_SIDE | EFFECT_SOLID)) ==
                (EFFECT_UNK_8000 | EFFECT_SOLID)) {
                if (i < 2) {
                    *vramFlag |= (effects & (EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                                             EFFECT_UNK_1000)) +
                                 (EFFECT_UNK_8000 | EFFECT_SOLID);
                    *y += g_Maria.colFloor[i].unk8 + col.unk18 - 1 + yMod;
                    return;
                }
                if (i == 2 && (effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 |
                                          EFFECT_SOLID)) ==
                                  (EFFECT_UNK_8000 | EFFECT_SOLID)) {
                    g_Maria.colFloor[2].effects = effects;
                    g_Maria.colFloor[2].unk10 = g_Maria.colFloor[2].unk8;
                }
                if (i == 3 &&
                    (effects &
                     (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID)) ==
                        (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID)) {
                    g_Maria.colFloor[3].effects = effects;
                    g_Maria.colFloor[3].unk10 = g_Maria.colFloor[3].unk8;
                }
            }
        }
        if (effects2 == (EFFECT_UNK_8000 | EFFECT_SOLID) && i < 2) {
            *vramFlag |= g_Maria.colFloor[i].effects &
                         (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                          EFFECT_UNK_1000 | EFFECT_SOLID);
            *y += yMod + g_Maria.colFloor[i].unk18;
            return;
        }
    }
    if (MARIA.velocityY < 0) {
        return;
    }
    xCheck = *x + g_MarSensorsFloor[0].x;
    yCheck = *y + g_MarSensorsFloor[0].y + 10;
    g_api.CheckCollision(xCheck, yCheck, &col, 0);
    if ((col.effects & (EFFECT_UNK_8000 | EFFECT_SOLID)) != EFFECT_NONE) {
        return;
    }

    for (i = 2; i < NUM_HORIZONTAL_SENSORS; i++) {
        if ((g_Maria.colFloor[3].effects & EFFECT_UNK_8000) &&
            (g_Maria.colFloor[2].effects & EFFECT_UNK_8000)) {
            return;
        }
        effects2 = g_Maria.colFloor[i].effects;
        vramApply = ((effects2 &
                      (EFFECT_UNK_4000 | EFFECT_UNK_2000 | EFFECT_UNK_1000)) +
                     (EFFECT_UNK_8000 | EFFECT_SOLID));
        if (!(effects2 & EFFECT_UNK_8000)) {
            continue;
        }
        if (i == 2) {
            effects = EFFECT_UNK_4000;
            xMod = g_Maria.colFloor[2].unk4;
            xMax = xMod + 8;
        } else {
            effects = 0;
            xMod = g_Maria.colFloor[3].unkC;
            xMax = 8 - xMod;
        }
        if ((effects2 & EFFECT_UNK_4000) == effects) {
            xCheck = xMod + (*x + g_MarSensorsFloor[i].x);
            yCheck = *y + g_MarSensorsFloor[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (col.effects & EFFECT_SOLID) {
                *y += col.unk18;
                *vramFlag |= vramApply;
                return;
            }
            continue;
        }
        if (xMax <= 0) {
            continue;
        }
        if (!(effects2 & 1)) {
            continue;
        }
        xCheck = *x + g_MarSensorsFloor[i].x + xMod;
        yCheck = *y + g_MarSensorsFloor[i].y + g_Maria.colFloor[i].unk10;
        g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
        if (col.effects & EFFECT_SOLID) {
            *y += (col.unk18 + g_Maria.colFloor[i].unk10);
            *vramFlag |= vramApply;
            return;
        }
    }
}

// local variant of MarCheckCeiling (see maria/pl_collision.c); same
// boss-wall edits as MarCheckFloor, including a status adjust on the
// stale collider at the top of the loop
void MarCheckCeiling(void) {
    Collider col;
    s32 i;
    u32 effects;
    u32 effects2;
    s16 xCheck;
    s16 yCheck;
    s16* x;
    s16* y;
    s32* vramFlag;
    s32 vramApply;
    s16 xMod;
    s32 xMax;

    i = 1;
    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    for (; i < NUM_HORIZONTAL_SENSORS; i++) {
        if (g_Maria.colCeiling[i].effects & EFFECT_SOLID_FROM_ABOVE) {
            continue;
        }
        if (g_Maria.status & PLAYER_STATUS_MIST_FORM &&
            col.effects & EFFECT_MIST_ONLY) {
            col.effects &= ~(EFFECT_SIDE | EFFECT_SOLID);
        }
        effects2 = g_Maria.colCeiling[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_8000)) {
            if (g_Maria.status &
                    (PLAYER_STATUS_MIST_FORM | PLAYER_STATUS_BAT_FORM) &&
                !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsCeiling[i].x;
            yCheck =
                *y + g_MarSensorsCeiling[i].y + g_Maria.colCeiling[i].unk10 + 1;
            g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
            if (g_Maria.status & PLAYER_STATUS_MIST_FORM &&
                col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_SIDE | EFFECT_SOLID);
            }
            effects = col.effects;
            if (!(effects & EFFECT_SOLID)) {
                if (g_Maria.colCeiling[i].effects != EFFECT_SOLID ||
                    MARIA.velocityY <= 0) {
                    *vramFlag |= 2;
                    if (!(*vramFlag & 1)) {
                        if (g_Maria.colCeiling[i].effects & EFFECT_UNK_8000) {
                            *y += g_Maria.colCeiling[i].unk10;
                        } else {
                            *y += g_Maria.colCeiling[i].unk20;
                        }
                    }
                    return;
                }
                continue;
            }
            if ((effects & (EFFECT_UNK_0800 | EFFECT_SIDE | EFFECT_SOLID)) ==
                (EFFECT_UNK_0800 | EFFECT_SOLID)) {
                if (i < 2) {
                    *vramFlag |=
                        (EFFECT_UNK_0800 | EFFECT_SIDE |
                         ((effects >> 4) & (EFFECT_UNK_0400 | EFFECT_UNK_0200 |
                                            EFFECT_UNK_0100)));
                    if (!(*vramFlag & 1)) {
                        *y += 1 + (g_Maria.colCeiling[i].unk10 + col.unk20);
                    }
                    return;
                }
                if (i == 2 && (effects & (EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                                          EFFECT_SOLID)) ==
                                  (EFFECT_UNK_0800 | EFFECT_SOLID)) {
                    g_Maria.colFloor[2].effects = effects;
                    g_Maria.colFloor[2].unk8 = g_Maria.colFloor[2].unk10;
                }
                if (i == 3 &&
                    (effects &
                     (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID)) ==
                        (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID)) {
                    g_Maria.colFloor[3].effects = effects;
                    g_Maria.colFloor[3].unk8 = g_Maria.colFloor[3].unk10;
                }
            }
        }
        if ((effects2 == (EFFECT_UNK_0800 | EFFECT_SOLID)) && i < 2) {
            *vramFlag |=
                (EFFECT_UNK_0800 | EFFECT_SIDE |
                 ((g_Maria.colCeiling[i].effects >> 4) &
                  (EFFECT_UNK_0400 | EFFECT_UNK_0200 | EFFECT_UNK_0100)));
            if (!(*vramFlag & 1)) {
                *y += g_Maria.colCeiling[i].unk20;
            }
            return;
        }
    }
    if (MARIA.velocityY > 0) {
        return;
    }
    xCheck = *x + g_MarSensorsCeiling[0].x;
    yCheck = (*y + g_MarSensorsCeiling[0].y) - 10;
    g_api.CheckCollision(xCheck, yCheck, &col, 0);
    if ((col.effects & EFFECT_SOLID) != EFFECT_NONE) {
        return;
    }
    for (i = 2; i < NUM_HORIZONTAL_SENSORS; i++) {
        if ((g_Maria.colCeiling[3].effects & EFFECT_UNK_0800) &&
            (g_Maria.colCeiling[2].effects & EFFECT_UNK_0800)) {
            return;
        }
        effects2 = g_Maria.colCeiling[i].effects;
        vramApply = ((effects2 >> 4) &
                     (EFFECT_UNK_0400 | EFFECT_UNK_0200 | EFFECT_UNK_0100)) +
                    (EFFECT_UNK_0800 | EFFECT_SIDE);
        if (!(effects2 & EFFECT_UNK_0800)) {
            continue;
        }
        if (i == 2) {
            effects = EFFECT_UNK_4000;
            xMod = g_Maria.colCeiling[2].unk4;
            xMax = xMod + 8;
        } else {
            effects = 0;
            xMod = g_Maria.colCeiling[3].unkC;
            xMax = 8 - xMod;
        }
        if ((effects2 & EFFECT_UNK_4000) == effects) {
            xCheck = xMod + (*x + g_MarSensorsCeiling[i].x);
            yCheck = *y + g_MarSensorsCeiling[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (col.effects & EFFECT_SOLID) {
                *vramFlag |= vramApply;
                if (!(*vramFlag & 1)) {
                    *y += col.unk20;
                }
                return;
            }
            continue;
        }
        if (xMax <= 0) {
            continue;
        }
        if (!(effects2 & 1)) {
            continue;
        }
        xCheck = *x + g_MarSensorsCeiling[i].x + xMod;
        yCheck = *y + g_MarSensorsCeiling[i].y + g_Maria.colCeiling[i].unk8;
        g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
        if (col.effects & EFFECT_SOLID) {
            if (!(*vramFlag & 1)) {
                *y += col.unk20 + g_Maria.colCeiling[i].unk8;
            }
            *vramFlag |= vramApply;
            return;
        }
    }
}

// local copy of MarCheckWallRight (see maria/pl_collision.c) minus the
// g_unkGraphicsStruct gate
void func_pspeu_09250260(void) {
    Collider col;
    s32 i;
    s32 effects;
    s16 xCheck;
    s16 yCheck;
    s16* y;
    s16* x;
    s32* vramFlag;

    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    effects =
        g_Maria.unk04 & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                         EFFECT_UNK_0400 | EFFECT_SIDE | EFFECT_SOLID);
    if ((effects == (EFFECT_UNK_8000 | EFFECT_SIDE | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_0800 | EFFECT_SIDE | EFFECT_SOLID)) ||
        (effects ==
         (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SIDE | EFFECT_SOLID))) {
        *vramFlag |= 4;
        return;
    }

    for (i = 0; i < NUM_VERTICAL_SENSORS; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_SIDE | EFFECT_SOLID);
        if (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SIDE |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SIDE |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_SIDE | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_0800 | EFFECT_SIDE | EFFECT_SOLID) ||
            effects == (EFFECT_SIDE | EFFECT_SOLID)) {
            xCheck = *x + g_MarSensorsWall[i].x + g_Maria.colWall[i].unk4 - 1;
            yCheck = *y + g_MarSensorsWall[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if ((col.effects & EFFECT_SOLID) == EFFECT_NONE) {
                *vramFlag |= 4;
                *x += g_Maria.colWall[i].unk4;
                return;
            }
        }
        if ((effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
                EFFECT_UNK_8000 &&
            (i != 0) &&
            ((g_Maria.colWall[0].effects & EFFECT_UNK_0800) ||
             !(g_Maria.colWall[0].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SIDE)))) {
            *vramFlag |= 4;
            *x += g_Maria.colWall[i].unk4;
            return;
        }
        if ((effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
                EFFECT_UNK_0800 &&
            (i != 6) &&
            ((g_Maria.colWall[6].effects & EFFECT_UNK_8000) ||
             !(g_Maria.colWall[6].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SIDE)))) {
            *vramFlag |= 4;
            *x += g_Maria.colWall[i].unk4;
            return;
        }
    }
}

// local copy of MarCheckWallLeft (see maria/pl_collision.c) minus the
// g_unkGraphicsStruct gate
void MarCheckWallLeft(void) {
    Collider col;
    s32 i;
    s32 effects;
    s16 xCheck;
    s16 yCheck;
    s16* y;
    s16* x;
    s32* vramFlag;

    y = &MARIA.posY.i.hi;
    x = &MARIA.posX.i.hi;
    vramFlag = &g_Maria.vram_flag;

    effects =
        g_Maria.unk04 & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                         EFFECT_UNK_0400 | EFFECT_SIDE | EFFECT_SOLID);
    if ((effects ==
         (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SIDE | EFFECT_SOLID)) ||
        (effects ==
         (EFFECT_UNK_0800 | EFFECT_UNK_0400 | EFFECT_SIDE | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                     EFFECT_UNK_0400 | EFFECT_SIDE | EFFECT_SOLID))) {
        *vramFlag |= 8;
        return;
    }
    for (i = NUM_VERTICAL_SENSORS; i < NUM_VERTICAL_SENSORS * 2; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_SIDE | EFFECT_SOLID);
        if ((effects == (EFFECT_UNK_8000 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_SIDE | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_SIDE | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SIDE |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SIDE |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_SIDE | EFFECT_SOLID))) {
            xCheck = *x + g_MarSensorsWall[i].x + g_Maria.colWall[i].unkC + 1;
            yCheck = *y + g_MarSensorsWall[i].y;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if ((col.effects & EFFECT_SOLID) == EFFECT_NONE) {
                *vramFlag |= 8;
                *x += g_Maria.colWall[i].unkC;
                return;
            }
        }
        if (((effects &
              (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
             (EFFECT_UNK_8000 | EFFECT_UNK_4000)) &&
            (i != 7) &&
            ((g_Maria.colWall[7].effects & EFFECT_UNK_0800) ||
             !(g_Maria.colWall[7].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SIDE)))) {
            *vramFlag |= 8;
            *x += g_Maria.colWall[i].unkC;
            return;
        }
        if (((effects &
              (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
             (EFFECT_UNK_4000 | EFFECT_UNK_0800)) &&
            (i != 13) &&
            ((g_Maria.colWall[13].effects & EFFECT_UNK_8000) ||
             !(g_Maria.colWall[13].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SIDE)))) {
            *vramFlag |= 8;
            *x += g_Maria.colWall[i].unkC;
            return;
        }
    }
}

// local copy of ReboundStoneBounce1 (see rebound_stone.h)
void func_pspeu_092508C0(s16 bounceAngle) {
    g_CurrentEntity->ext.reboundStone.stoneAngle =
        (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

// local copy of ReboundStoneBounce2 (see rebound_stone.h)
void func_pspeu_09250948(s16 bounceAngle) {
    if (g_CurrentEntity->ext.reboundStone.unk82 == 0) {
        g_CurrentEntity->ext.reboundStone.stoneAngle =
            (bounceAngle * 2) - g_CurrentEntity->ext.reboundStone.stoneAngle;
        g_CurrentEntity->ext.reboundStone.unk80++;
        g_CurrentEntity->ext.reboundStone.unk82++;
    }
}

extern EInit D_pspeu_092666E8;
void func_pspeu_092509D0(Entity* self) {
    s16 playerX;
    s16 playerY;
    Collider collider;
    s32 speed;
    s32 currX;
    s32 currY;
    s32 collX;
    s32 collY;
    s32 deltaX;
    s32 deltaY;
    s32 i;
    s32 colliderFlags;
    PrimLineG2* prim;

    speed = 0x400;
    self->ext.reboundStone.unk82 = 0;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_092666E8);
        if (g_Maria.status & PLAYER_STATUS_POISON) {
            self->attack = self->attack / 2;
        }
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 8);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->posY.i.hi -= 12;
        playerX = self->posX.i.hi;
        playerY = self->posY.i.hi;
        for (prim = (PrimLineG2*)&g_PrimBuf[self->primIndex], i = 0;
             prim != NULL; i++, prim = prim->next) {
            prim->r0 = prim->r1 = 0xFF;
            prim->g0 = prim->g1 = 0x7F;
            prim->priority = MARIA.zPriority + 2;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            if (i) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim->x0 = prim->x1 = playerX;
            prim->y0 = prim->y1 = playerY;
            prim->timer = 20;
        }
        self->flags |=
            FLAG_POS_CAMERA_LOCKED | FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->zPriority = MARIA.zPriority + 2;

        if (MARIA.facingLeft) {
            self->ext.reboundStone.stoneAngle = 0x980;
        } else {
            self->ext.reboundStone.stoneAngle = 0xE80;
        }
        self->ext.reboundStone.stoneAngle += (rand() & 0x7F) - 0x40;

        self->ext.reboundStone.lifeTimer = 0x40;
        self->step = 1;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        break;
    case 1:
        if (self->flags & FLAG_DEAD) {
            MarCreateEntFactoryFromEntity(self, FACTORY(10, 0), 0);
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
            self->step = 2;
            break;
        }
        playerX = self->posX.i.hi;
        playerY = self->posY.i.hi;
        deltaX = rcos(self->ext.reboundStone.stoneAngle) * 0x10;
        deltaY = -rsin(self->ext.reboundStone.stoneAngle) * 0x10;
        currX = self->posX.val;
        currY = self->posY.val;
        for (i = 0; i < 6; i++) {
            collX = FIX_TO_I(currX);
            collY = FIX_TO_I(currY + deltaY);
            g_api.CheckCollision(collX, collY, &collider, 0);
            colliderFlags =
                collider.effects &
                (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                 EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
            if (colliderFlags & EFFECT_SOLID) {
                colliderFlags &= 0xFF00;
                if (deltaY > 0) {
                    if (!colliderFlags || (colliderFlags & EFFECT_UNK_0800)) {
                        func_pspeu_092508C0(0x800);
                    }
                    if (colliderFlags == EFFECT_UNK_8000) {
                        func_pspeu_09250948(0x200);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x12E);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0xA0);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                        func_pspeu_09250948(0x600);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_8000 + EFFECT_UNK_4000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x6D2);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_8000 + EFFECT_UNK_4000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0x760);
                    }
                }
                if (deltaY < 0) {
                    if (!colliderFlags || (colliderFlags & EFFECT_UNK_8000)) {
                        func_pspeu_092508C0(0x800);
                    }
                    if (colliderFlags == EFFECT_UNK_0800) {
                        func_pspeu_09250948(0xE00);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0xED2);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0xF60);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                        func_pspeu_09250948(0xA00);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_0800 + EFFECT_UNK_4000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x92E);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_0800 + EFFECT_UNK_4000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0x8A0);
                    }
                }
            }
            collY = FIX_TO_I(currY);
            collX = FIX_TO_I(currX + deltaX);
            g_api.CheckCollision(collX, collY, &collider, 0);
            colliderFlags =
                collider.effects &
                (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_2000 |
                 EFFECT_UNK_1000 | EFFECT_UNK_0800 | EFFECT_BLOCK);
            if (colliderFlags & EFFECT_SOLID) {
                colliderFlags &= 0xFF00;
                // Cases when traveling right
                if (deltaX > 0) {
                    if (!colliderFlags || TEST_BITS(colliderFlags, 0x4800) ||
                        TEST_BITS(colliderFlags, 0xC000)) {
                        func_pspeu_092508C0(0x400);
                    }
                    if (colliderFlags == EFFECT_UNK_0800) {
                        func_pspeu_09250948(0xE00);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0xED2);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0xF60);
                    }
                    if (colliderFlags == EFFECT_UNK_8000) {
                        func_pspeu_09250948(0x200);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x12E);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0xA0);
                    }
                }
                // Cases when traveling left
                if (deltaX < 0) {
                    if (!colliderFlags || ((colliderFlags & 0x4800) == 0x800) ||
                        ((colliderFlags & 0xC000) == 0x8000)) {
                        func_pspeu_092508C0(0x400);
                    }
                    if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                        func_pspeu_09250948(0xA00);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_0800 + EFFECT_UNK_4000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x92E);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_0800 + EFFECT_UNK_4000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0x8A0);
                    }
                    if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                        func_pspeu_09250948(0x600);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_8000 + EFFECT_UNK_4000 + EFFECT_UNK_1000) {
                        func_pspeu_09250948(0x6D2);
                    }
                    if (colliderFlags ==
                        EFFECT_UNK_8000 + EFFECT_UNK_4000 + EFFECT_UNK_2000) {
                        func_pspeu_09250948(0x760);
                    }
                }
            }
            if (self->ext.reboundStone.unk82) {
                goto block_93;
            }
            currX += deltaX;
            currY += deltaY;
        }
    block_93:
        if (self->ext.reboundStone.unk82) {
            MarCreateEntFactoryFromEntity(self, FACTORY(10, 0), 0);
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
        }
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x40 || self->posY.i.hi > 0x140 ||
            self->ext.reboundStone.unk80 == 5) {
            self->step = 2;
        } else {
            deltaX =
                ((rcos(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posX.val += deltaX;
            deltaY =
                -((rsin(self->ext.reboundStone.stoneAngle) << 4) * speed) >> 8;
            self->posY.val += deltaY;
        }
        break;
    case 2:
        playerX = self->posX.i.hi;
        playerY = self->posY.i.hi;
        if (--self->ext.reboundStone.lifeTimer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.reboundStone.lifeTimer == 0x20) {
            self->hitboxState = 0;
        }
        prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->timer = 0;
            prim = prim->next;
        }
        break;
    }

    prim = (PrimLineG2*)&g_PrimBuf[self->primIndex];
    i = 0;
    if (self->step == 2) {
        colliderFlags = 4;
    } else {
        colliderFlags = 2;
    }
    // cleaner to use previous 3 lines than to put them in the for's initializer
    for (; prim != NULL; i++, prim = prim->next) {
        if (self->ext.reboundStone.unk82) {
            if (i == self->ext.reboundStone.unk80) {
                prim->x0 = playerX;
                prim->y0 = playerY;
                prim->drawMode &= ~DRAW_HIDE;
            }
        }
        if (i == self->ext.reboundStone.unk80) {
            prim->x1 = self->posX.i.hi;
            prim->y1 = self->posY.i.hi;
        }
        if (!(prim->drawMode & DRAW_HIDE)) {
            if (prim->timer) {
                prim->timer--;
            } else {
                // again not colliderFlags, seems to control stone fading
                if (colliderFlags < prim->r1) {
                    prim->r1 -= colliderFlags;
                }
                prim->r0 = prim->r1;
                if (prim->g1 > (colliderFlags / 2)) {
                    prim->g1 -= colliderFlags / 2;
                }
                prim->g0 = prim->g1;
            }
        }
    }
}

extern EInit D_pspeu_09266718;
extern EInit D_pspeu_09266728;
extern DopWeaponAnimation D_pspeu_09263318[];

void func_pspeu_092514F0(Entity* self) {
    s16 animIndex;
    DopWeaponAnimation* anim;

    animIndex = (self->params & 0x7FFF) >> 8;
    self->posX.val = DOPPLEGANGER.posX.val;
    self->posY.val = DOPPLEGANGER.posY.val;
    self->facingLeft = DOPPLEGANGER.facingLeft;
    anim = &D_pspeu_09263318[animIndex];

    if (DOPPLEGANGER.ext.player.anim < anim->frameStart ||
        DOPPLEGANGER.ext.player.anim >= (anim->frameStart + 7) ||
        !g_Dop.unk46) {
        DestroyEntity(self);
        return;
    }

    if (self->step == 0) {
        if (animIndex) {
            InitializeEntity(D_pspeu_09266728);
        } else {
            InitializeEntity(D_pspeu_09266718);
        }
        if (g_Dop.status & PLAYER_STATUS_POISON) {
            self->attack = self->attack / 2;
        }
        self->zPriority = DOPPLEGANGER.zPriority - 2;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->step = Dop_Stand;
    }
    self->ext.weapon.anim = DOPPLEGANGER.ext.player.anim - anim->frameStart;
    if (DOPPLEGANGER.poseTimer == 1 && DOPPLEGANGER.pose == anim->soundFrame) {
        g_api.PlaySfx(anim->soundId);
    }
    if (UpdateUnarmedAnim(anim->frameProps, (u16**)anim->frames) < 0) {
        DestroyEntity(self);
    }
}

extern EInit D_pspeu_092666F8;
void func_pspeu_09251748(Entity* self) {
    Collider collider;
    Primitive* prim;
    s16 offsetX;
    s16 offsetY;
    s16 angle1;
    s16 angle2;
    s16 angle3;
    s16 angle4;
    s16 x;
    s16 y;
    s16 xCol;
    s32 modX;
    s32 modY;
    s32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_092666F8);
        if (g_Maria.status & PLAYER_STATUS_POISON) {
            self->attack = self->attack / 2;
        }
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->facingLeft = MARIA.facingLeft;
        self->hitboxWidth = 4;
        self->hitboxHeight = 2;
        self->hitboxOffX = 4;
        self->hitboxOffY = 0;
        if (!(g_Maria.status & PLAYER_STATUS_CROUCH)) {
            self->posY.i.hi -= 9;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->tpage = 0x1C;
        prim->clut = PAL_UNK_1AB;
        prim->u0 = prim->u1 = 0x18;
        prim->v0 = prim->v2 = 0x18;
        prim->u2 = prim->u3 = 0x20;
        prim->v1 = prim->v3 = 0;
        prim->priority = MARIA.zPriority + 2;
        prim->drawMode = DRAW_HIDE | DRAW_UNK02;

        prim = prim->next;
        prim->type = PRIM_LINE_G2;
        prim->priority = MARIA.zPriority + 2;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
        prim->r0 = 0x7F;
        prim->g0 = 0x3F;
        prim->b0 = 0;
        MarSetSpeedX(FIX(8));
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        g_Maria.timers[ALU_T_USE_SUBWPN] = 4;
        break;
    case 1:
        self->ext.subweapon.timer++;
        if (self->velocityX > 0) {
            xCol = 8;
        }
        if (self->velocityX < 0) {
            xCol = -8;
        }
        for (i = 0; i < 8; i++) {
            if (self->velocityX > 0) {
                self->posX.i.hi++;
            }
            if (self->velocityX < 0) {
                self->posX.i.hi--;
            }
            g_api.CheckCollision(
                self->posX.i.hi + xCol, self->posY.i.hi, &collider, 0);
            if (collider.effects & EFFECT_BLOCK || (self->flags & FLAG_DEAD)) {
                self->ext.subweapon.timer = 64;
                self->velocityX = -(self->velocityX >> 3);
                self->velocityY = FIX(-2.5);
                self->hitboxState = 0;
                self->posX.i.hi += xCol;
                MarCreateEntFactoryFromEntity(self, FACTORY(10, 0), 0);
                self->posX.i.hi -= xCol;
                if (collider.effects & EFFECT_BLOCK) {
                    g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
                } else {
                    g_api.PlaySfx(SFX_DOP_SUBWEAPON_TINK);
                }
                self->step++;
                return;
            }
        }
        if (self->hitFlags & 0x80) {
            self->ext.subweapon.timer = 4;
            self->step = 3;
            self->hitboxState = 0;
            return;
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        offsetX = 12;
        offsetY = 8;
        if (self->facingLeft) {
            offsetX = -offsetX;
            offsetY = -offsetY;
        }
        prim = &g_PrimBuf[self->primIndex];
        prim->x0 = x - offsetX;
        prim->y0 = y - 4;
        prim->x1 = x + offsetX;
        prim->y1 = y - 4;
        prim->x2 = x - offsetX;
        prim->y2 = y + 4;
        prim->x3 = x + offsetX;
        prim->y3 = y + 4;
        prim->clut = PAL_UNK_1AB;
        (g_GameTimer >> 1) & 1;
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->x0 = x - offsetY;
        prim->y0 = y - 1;
        prim->x1 = x - (offsetX * (self->ext.subweapon.timer / 2));
        prim->y1 = y - 1;
        prim->drawMode &= ~DRAW_HIDE;
        if (self->step != 1) {
            prim->drawMode |= DRAW_HIDE;
            return;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.subweapon.timer == 0x20) {
            prim->drawMode |=
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) =
                0x60;
        }
        self->posX.val += self->velocityX;
        self->posY.val += self->velocityY;
        self->velocityY += FIX(0.125);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        offsetX = 12;
        if (self->facingLeft == 0) {
            angle1 = 0x800 - 0xD2;
            angle2 = 0xD2;
            angle3 = 0x800 + 0xD2;
            angle4 = -0xD2;
            self->rotate -= ROT(11.25);
        } else {
            angle2 = 0x800 - 0xD2;
            angle1 = 0xD2;
            angle4 = 0x800 + 0xD2;
            angle3 = -0xD2;
            self->rotate += ROT(11.25);
        }
        angle1 += self->rotate;
        angle2 += self->rotate;
        angle3 += self->rotate;
        angle4 += self->rotate;
        if (self->facingLeft) {
            offsetX = -offsetX;
        }
        prim = &g_PrimBuf[self->primIndex];
        modX = (rcos(angle1) * 0xCA0) >> 0x14;
        modY = -(rsin(angle1) * 0xCA0) >> 0x14;
        prim->x0 = x + (s16)modX;
        prim->y0 = y - (s16)modY;
        modX = (rcos(angle2) * 0xCA0) >> 0x14;
        modY = -(rsin(angle2) * 0xCA0) >> 0x14;
        prim->x1 = x + (s16)modX;
        prim->y1 = y - (s16)modY;
        modX = (rcos(angle3) * 0xCA0) >> 0x14;
        modY = -(rsin(angle3) * 0xCA0) >> 0x14;
        prim->x2 = x + (s16)modX;
        prim->y2 = y - (s16)modY;
        modX = (rcos(angle4) * 0xCA0) >> 0x14;
        modY = -(rsin(angle4) * 0xCA0) >> 0x14;
        prim->x3 = x + (s16)modX;
        prim->y3 = y - (s16)modY;
        prim->clut = PAL_UNK_1AB;
        (g_GameTimer >> 1) & 1;
        if (self->ext.subweapon.timer < 0x21) {
            prim->r0 -= 2;
            prim->g0 = prim->b0 = PGREY(prim, 1) = PGREY(prim, 2) =
                PGREY(prim, 3) = prim->r0;
        }
        prim->drawMode &= ~DRAW_HIDE;
        prim = prim->next;
        prim->drawMode |= DRAW_HIDE;
        break;
    case 3:
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
        }
        break;
    }
}

// local copy of RicGetFreeEntity (see bo6/us_39144.c); byte-identical
// across the three boss walls
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

// local copy of RicGetFreeEntityReverse (see bo6/us_39144.c)
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

extern u8 D_pspeu_09263340[][4];
extern u8 D_pspeu_0926CCC0;
extern u8 D_pspeu_0926CCB8;
extern u8 D_pspeu_0926CCB0;
extern u8 D_pspeu_0926CCA8;
// local copy of func_us_801BB314 (see bo6/us_39144.c)
void func_pspeu_09252480(s32 arg0) {
    D_pspeu_0926CCC0 = D_pspeu_09263340[arg0][0];
    D_pspeu_0926CCB8 = D_pspeu_09263340[arg0][1];
    D_pspeu_0926CCB0 = D_pspeu_09263340[arg0][2];
    D_pspeu_0926CCA8 = D_pspeu_09263340[arg0][3];
}

extern u8 D_pspeu_09263378[][8];
// local copy of Maria func_8015FDB0 (see maria/pl_blueprints.c);
// this boss copy stops at frame six instead of restarting the animation.
s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY) {
    s16 offset;
    u8* uvAnim;

    uvAnim = D_pspeu_09263378[0];
    uvAnim = &uvAnim[prim->b0 * 8];

    if (prim->b0 >= 3) {
        offset = 4;
    } else {
        offset = 6;
    }

    if (prim->b0 == 6) {
        return -1;
    }

    prim->x0 = posX - offset;
    prim->y0 = posY - offset;
    prim->x1 = posX + offset;
    prim->y1 = posY - offset;
    prim->x2 = posX - offset;
    prim->y2 = posY + offset;
    prim->x3 = posX + offset;
    prim->y3 = posY + offset;
    prim->u0 = *uvAnim++;
    prim->v0 = *uvAnim++;
    prim->u1 = *uvAnim++;
    prim->v1 = *uvAnim++;
    prim->u2 = *uvAnim++;
    prim->v2 = *uvAnim++;
    prim->u3 = *uvAnim++;
    prim->v3 = *uvAnim;

    prim->b1++;
    if (!(prim->b1 & 1)) {
        prim->b0++;
    }
    return 0;
}

s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY);
extern Point16 D_pspeu_0926CC68[16];
// local copy of RicEntityHitByHoly (see ric/pl_blueprints.c); the boss
// walls track the doppleganger in slot 64 and camera-lock the effect
void func_pspeu_09252768(Entity* entity) {
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
            (s16)g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_0926CC68));
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        hitboxX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        hitboxY = MARIA.posY.i.hi + MARIA.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < LEN(D_pspeu_0926CC68); i++) {
            temp_xRand = hitboxX + rand() % 24 - 12;
            temp_yRand = hitboxY + rand() % 48 - 24;
            D_pspeu_0926CC68[i].x = temp_xRand;
            D_pspeu_0926CC68[i].y = temp_yRand;
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
            hitboxX = D_pspeu_0926CC68[i].x;
            hitboxY = D_pspeu_0926CC68[i].y;
            temp = func_8015FDB0(prim, hitboxX, hitboxY);
            D_pspeu_0926CC68[i].y--;
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

void func_pspeu_09252B20(Entity* self) { FntPrint("dummy set\n"); }

void func_pspeu_09252E78(Entity* self);
void func_pspeu_09253620(Entity* self);
void func_pspeu_09256D30(Entity* self);
void func_pspeu_09257298(Entity* self);
void func_pspeu_09254180(Entity* self);
void func_maria_80161C2C(Entity* self);
void MarEntityHitByLightning(Entity* self);
void func_pspeu_09256258(Entity* self);
void func_pspeu_0925BA80(Entity* self);
void func_pspeu_09258A50(Entity* self);
void func_pspeu_09259A20(Entity* self);
void func_pspeu_092570B8(Entity* self);
void func_pspeu_09253500(Entity* self);
void func_pspeu_09253EA0(Entity* self);
PfnEntityUpdate D_pspeu_092633B8[] = {
    func_pspeu_09252B20,     func_pspeu_09252E78, func_pspeu_09253620,
    func_pspeu_09256D30,     func_pspeu_09251748, func_pspeu_09257298,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09254180,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_maria_80161C2C, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_092509D0,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    MarEntityHitByLightning, func_pspeu_09256258, func_pspeu_0925BA80,
    func_pspeu_09258A50,     func_pspeu_09259A20, func_pspeu_092570B8,
    func_pspeu_09253500,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252768,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09252B20,     func_pspeu_09252B20, func_pspeu_09252B20,
    func_pspeu_09253EA0,     func_pspeu_092514F0,
};
extern AnimationFrame* D_pspeu_0926C0C0[];
void func_pspeu_09252B48(void) {
    Entity* entity;
    PfnEntityUpdate entityUpdate;
    s32 i;

    entity = g_CurrentEntity = &g_Entities[68];

    for (i = 68; i < 144; i++, g_CurrentEntity++, entity++) {
        if (i == 16 && entity->entityId == 0) {
            g_Dop.unk48 = 0;
        }

        if (entity->entityId == 0) {
            continue;
        }

        entityUpdate = D_pspeu_092633B8[entity->entityId];
        entityUpdate(entity);

        entity = g_CurrentEntity;
        if (entity->entityId != 0) {
            if (!(entity->flags & FLAG_UNK_10000000) &&
                (entity->posX.i.hi > 288 || entity->posX.i.hi < -32 ||
                 entity->posY.i.hi > 256 || entity->posY.i.hi < -16)) {
                DestroyEntity(g_CurrentEntity);
            } else {
                if (entity->flags & FLAG_UNK_20000000) {
                    UpdateAnim(0, D_pspeu_0926C0C0);
                }
                entity->flags |= FLAG_NOT_AN_ENEMY;
            }
        }
    }

    if (D_pspeu_0926CCC0) {
        D_pspeu_0926CCC0--;
        if (D_pspeu_0926CCC0 & 1) {
            g_api.func_800EA5AC(
                1, D_pspeu_0926CCB8, D_pspeu_0926CCB0, D_pspeu_0926CCA8);
        }
    }

    if (g_Dop.status & (PLAYER_STATUS_DEAD | PLAYER_STATUS_UNK80000)) {
        FntPrint("dead boss\n");
        entity = &g_Entities[68];
        for (i = 68; i < 144; i++, entity++) {
            entity->hitboxState = 0;
        }
    }
}

// local copy of RicCreateEntFactoryFromEntity (see bo6/us_39144.c);
// same (68, 80) slot range as bo6
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

extern FactoryBlueprint D_pspeu_092634A8[];
extern u8 D_pspeu_09263760[][2];
// local copy of the doppleganger entity factory (see func_us_801CA2AC in
// bo4/unk_46E7C.c); aka MarEntityFactory in the bo6 wall
void func_pspeu_09252E78(Entity* self) {
    Entity* newEntity;
    s16 i;
    s16 n;
    s16 endIndex;
    s16 startIndex;
    u8* data;

    if (self->step == 0) {
        data = (u8*)&D_pspeu_092634A8[self->params];
        self->ext.factory.newEntityId = *data++;
        self->ext.factory.amount = *data++;
        self->ext.factory.nPerCycle = *data & 0x3F;
        self->ext.factory.isNonCritical = (s16)(*data >> 7) & 1;
        self->ext.factory.incParamsKind = (s16)(*data++ >> 6) & 1;
        self->ext.factory.tCycle = *data++;
        self->ext.factory.kind = *data & 0xF;
        self->ext.factory.origin = (s16)(*data++ >> 4) & 0xF;
        self->ext.factory.delay = *data;
        self->flags |= FLAG_UNK_10000000;

        self->step++;

        switch (self->ext.factory.origin) {
        case B_ORIGIN_DEFAULT:
        case B_ORIGIN_6:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        case B_ORIGIN_2:
        case B_ORIGIN_4:
        case B_ORIGIN_5:
        case B_ORIGIN_7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        }
    } else {
        switch (self->ext.factory.origin) {
        case B_ORIGIN_DEFAULT:
        case B_ORIGIN_1:
        case B_ORIGIN_3:
        case B_ORIGIN_5:
        case B_ORIGIN_6:
            break;
        case B_ORIGIN_2:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case B_ORIGIN_4:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != Dop_Walk) {
                self->entityId = 0;
                return;
            }
            break;
        case B_ORIGIN_7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != Dop_Hit) {
                self->entityId = 0;
                return;
            }
            break;
        }
    }

    if (self->ext.factory.delay) {
        if (--self->ext.factory.delay) {
            return;
        }
        self->ext.factory.delay = self->ext.factory.tCycle;
    }

    // Save this value so we don't have to re-fetch on every for-loop cycle
    n = self->ext.factory.nPerCycle;
    for (i = 0; i < n; i++) {
        // !FAKE, this should probably be &entity_ranges[unk9C] or similar,
        // instead of doing &entity_ranges followed by +=
        data = (u8*)&D_pspeu_09263760[0];
        data += self->ext.factory.kind * 2;

        startIndex = *data++;
        endIndex = *data;

        if (self->ext.factory.kind == 3) {
            DestroyEntity(&g_Entities[startIndex]);
            newEntity = &g_Entities[startIndex];
            g_Maria.unk48 = 0;
        } else if (self->ext.factory.kind == 0) {
            newEntity = MarGetFreeEntityReverse(startIndex, endIndex + 1);
        } else if (self->ext.factory.kind == 8) {
            if ((self->ext.factory.spawnIndex % 3) == 0) {
                newEntity = MarGetFreeEntity(81, 96);
            }
            if ((self->ext.factory.spawnIndex % 3) == 1) {
                newEntity = MarGetFreeEntity(96, 112);
            }
            if ((self->ext.factory.spawnIndex % 3) == 2) {
                newEntity = MarGetFreeEntity(112, 128);
            }
        } else {
            newEntity = MarGetFreeEntity(startIndex, endIndex + 1);
        }

        if (newEntity == NULL) {
            if (self->ext.factory.isNonCritical == 1) {
                self->entityId = 0;
            } else {
                self->ext.factory.delay = self->ext.factory.tCycle;
            }
            return;
        }

        DestroyEntity(newEntity);
        newEntity->entityId = self->ext.factory.newEntityId;
        newEntity->params = self->ext.factory.paramsBase;
        // The child (newEntity) is not an ent factory, but because the
        // factory creates many entities, we can't pick a particular
        // extension. But we're not allowed to use generic, so i'll just
        // reuse entFactory.
        newEntity->ext.factory.parent = self->ext.factory.parent;
        newEntity->posX.val = self->posX.val;
        newEntity->posY.val = self->posY.val;
        newEntity->facingLeft = self->facingLeft;
        newEntity->zPriority = self->zPriority;

        if (self->ext.factory.incParamsKind) {
            newEntity->params += self->ext.factory.spawnIndex;
        } else {
            newEntity->params += i;
        }

        self->ext.factory.spawnIndex++;

        if (self->ext.factory.spawnIndex == self->ext.factory.amount) {
            self->entityId = 0;
            return;
        }
    }
    self->ext.factory.delay = self->ext.factory.tCycle;
}

extern u16 D_pspeu_09266708[];
// local copy of func_us_801CA748 (see bo4/unk_46E7C.c); contact damage
// entity that tracks the doppleganger during the bat-form slide
void func_pspeu_09253500(Entity* self) {
    if (MARIA.step_s != Dop_Crouch || MARIA.step != Dop_MorphBat) {
        DestroyEntity(self);
        return;
    }

    if (self->step == 0) {
        InitializeEntity(D_pspeu_09266708);
        if (g_Maria.status & PLAYER_STATUS_POISON) {
            self->attack = self->attack / 2;
        }
        self->hitboxOffX = 4;
        self->step++;
    }

    self->flags =
        FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_NOT_AN_ENEMY;
    self->facingLeft = MARIA.facingLeft;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->posX.i.hi = MARIA.posX.i.hi;
}

extern s16 D_pspeu_09263778[6];
extern s32 D_pspeu_09263788[6];
extern s16 D_pspeu_092637A0[6];
extern AnimationFrame D_pspeu_092637B0[];
extern u8 D_pspeu_09263818[14];
extern u8 D_pspeu_09263828[10];
void func_pspeu_09253620(Entity* self) {
    s16 posX;
    s32 i;
    s16 paramsLo;
    s16 paramsHi;

    switch (self->step) {
    case 0:
        paramsHi = self->params >> 8;
        paramsLo = self->params & 0xFF;
        self->animSet = ANIMSET_DRA(5);
        self->anim = D_pspeu_092637B0;
        self->zPriority = MARIA.zPriority + 2;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->palette = PAL_FLAG(PAL_UNK_195);
        self->blendMode = BLEND_TRANSP;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;

        posX = D_pspeu_09263778[paramsLo];
        if (paramsHi == 0) {
            posX += 6;
        }
        if (paramsHi == 1) {
            posX -= 8;
        }
        if ((paramsHi == 2) || (paramsHi == 0xA)) {
            posX -= 2;
        }
        if (paramsHi == 5) {
            posX = -6;
        }
        if (paramsHi == 6) {
            posX = -24;
        }
        if (paramsHi == 3) {
            posX = (rand() % 30) - 0xE;
            paramsLo = (rand() & 3) + 2;
            self->posY.i.hi -= rand() % 4;
        }
        if (paramsHi == 7) {
            posX = (rand() % 60) - 0x1E;
            paramsLo = (rand() & 3) + 2;
            self->posY.i.hi -= rand() % 4;
        }
        if (paramsHi == 9) {
            posX = rand() % 16 - 8;
            paramsLo = (rand() & 3) + 2;
            self->posY.i.hi -= (20 + (rand() % 4));
        }
        if (paramsHi == 4) {
            for (i = paramsLo * 2; i < LEN(D_pspeu_09263818); i++) {
                if (g_Maria.colWall[D_pspeu_09263818[i]].effects &
                    EFFECT_BLOCK) {
                    break;
                }
            }
            if (i == LEN(D_pspeu_09263818)) {
                DestroyEntity(self);
                return;
            }
            self->posX.i.hi =
                MARIA.posX.i.hi + g_MarSensorsWall[D_pspeu_09263818[i]].x;
            self->posY.i.hi =
                MARIA.posY.i.hi + g_MarSensorsWall[D_pspeu_09263818[i]].y;
            self->velocityY = FIX(-0.25);
            self->scaleX = D_pspeu_092637A0[1] + 0x40;
            self->scaleY = self->scaleX;
            self->step++;
            return;
        }
        if (paramsHi == 8) {
            for (i = paramsLo * 2; i < LEN(D_pspeu_09263828); i++) {
                if (g_Maria.colWall[D_pspeu_09263828[i]].effects &
                    EFFECT_BLOCK) {
                    break;
                }
            }
            if (i == LEN(D_pspeu_09263828)) {
                DestroyEntity(self);
                return;
            }
            self->posX.i.hi =
                MARIA.posX.i.hi + g_MarSensorsWall[D_pspeu_09263828[i]].x;
            self->posY.i.hi =
                MARIA.posY.i.hi + g_MarSensorsWall[D_pspeu_09263828[i]].y;
            self->velocityY = D_pspeu_09263788[paramsLo];
            self->scaleX = D_pspeu_092637A0[paramsLo] + 0x20;
            self->scaleY = self->scaleX;
            self->step++;
            return;
        }
        if (paramsHi == 1) {
            if (g_Maria.vram_flag & TOUCHING_ANY_SLOPE) {
                posX /= 2;
            }
        }
        if (paramsHi == 6) {
            if (MARIA.velocityX > 0) {
                posX = -posX;
            }
        } else {
            if (self->facingLeft) {
                posX = -posX;
            }
        }
        self->posX.i.hi += posX;
        self->posY.i.hi += 0x18;
        self->scaleX = D_pspeu_092637A0[paramsLo] + 0x40;
        self->velocityY = D_pspeu_09263788[paramsLo];
        if (paramsHi == 1) {
            self->velocityY = FIX(-0.25);
            MarSetSpeedX(-0x3000);
            self->scaleX = D_pspeu_092637A0[1] + 0x40;
        }
        if (paramsHi == 5) {
            self->velocityY = D_pspeu_09263788[4 - paramsLo * 2];
        }
        self->scaleY = self->scaleX;
        if (paramsHi == 10) {
            self->posY.i.hi -= 6;
        }
        self->step++;
        break;
    case 1:
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern AnimationFrame D_pspeu_09263838[];
extern s32 D_pspeu_0926CC60;
// local copy of RicEntityHitByDark (see ric/pl_blueprints.c); the boss
// walls use different flags and track the doppleganger's z-priority
void func_pspeu_09253EA0(Entity* entity) {
    s16 x, y;

    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        entity->unk5A = 0x79;
        entity->animSet = ANIMSET_DRA(14);
        entity->zPriority = MARIA.zPriority + 2;
        entity->palette = PAL_FLAG(PAL_UNK_19F);
        if (D_pspeu_0926CC60 & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        D_pspeu_0926CC60++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_pspeu_09263838;
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

// local copy of func_us_801BD47C (see bo6/us_39144.c); true when
// another live entity shares this id and params
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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09254180);

void func_pspeu_09256258(Entity* self) {
    s16* animFramePtr;
    u8* spritesheetPtr;
    s16 xOffset;
    s16 yOffset;
    s16 width;
    Primitive* prim;
    s16 spriteIdx;
    s32 i;
    s16 upperparams;
    u8 spriteX;
    s16* primData;
    s16 xVar;
    s16 yVar;
    u8 four;
    u8 one;
    s16 height;
    u8 spriteY;
    s16 selfX;
    s16 selfY;

    if ((g_Dop.status & (PLAYER_STATUS_AXEARMOR | PLAYER_STATUS_TRANSFORM)) ||
        !DOPPLEGANGER.animSet || !(DOPPLEGANGER.animCurFrame & INT16_MAX)) {
        DestroyEntity(self);
        return;
    }

    upperparams = (self->params & 0x7F00) >> 8;
    self->posX.i.hi = DOPPLEGANGER.posX.i.hi;
    self->posY.i.hi = DOPPLEGANGER.posY.i.hi;
    self->facingLeft = DOPPLEGANGER.facingLeft;
    animFramePtr = D_pspeu_092641C8[DOPPLEGANGER.animCurFrame & INT16_MAX];
    spriteIdx = *animFramePtr++;
    spriteIdx &= 0x7FFF;
    selfX = self->posX.i.hi;
    selfY = self->posY.i.hi;
    spritesheetPtr = ((u8**)D_pspeu_09266768)[spriteIdx];
    four = 4;
    one = 1;
    spriteX = four + spritesheetPtr[0];
    spriteY = one + spritesheetPtr[1];
    width = spriteX - four;
    height = spriteY - one;
    xOffset = animFramePtr[0] + spritesheetPtr[2];
    yOffset = animFramePtr[1] + spritesheetPtr[3];
    self->rotate = DOPPLEGANGER.rotate;
    self->drawFlags = DOPPLEGANGER.drawFlags | (ENTITY_SCALEX | ENTITY_SCALEY);
    primData = D_pspeu_09263B40[upperparams];
    switch (self->step) {
    case 0: // Initialization
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        // This is just not a for-loop, that's weird
        for (i = 0; i < 1; i++) {
            prim->tpage = 0x10;
            prim->clut = primData[3]; // Always 259
            prim->priority = DOPPLEGANGER.zPriority + 2;
            // primData[4] is always 49; DRAW_UNK_40 | DRAW_HIDE | DRAW_TRANSP
            prim->drawMode = primData[4] + (DRAW_COLORS | DRAW_UNK02);
            prim = prim->next;
        }
        switch (upperparams) {
        case 0: // MP refill
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 17: // Dark Metamorphosis
        case 18:
        case 20: // Sword Warp Spell (#1)
        case 22: // Soul Steal
            self->ext.playerOutline.brightness = 0x80;
            self->scaleX =
                DOPPLEGANGER.scaleX; // Player scaleX is (always?) 0x100
            self->scaleY =
                DOPPLEGANGER.scaleY; // Player scaleY is (always?) 0x100
            self->rotPivotY = DOPPLEGANGER.rotPivotY;
            self->rotPivotX = DOPPLEGANGER.rotPivotX;
            self->ext.playerOutline.timer = 8;
            break;
        case 2: // Agunea
            self->ext.playerOutline.brightness = 0xC0;
            self->scaleX =
                DOPPLEGANGER.scaleX; // Player scaleX is (always?) 0x100
            self->scaleY =
                DOPPLEGANGER.scaleY; // Player scaleY is (always?) 0x100
            self->rotPivotY = DOPPLEGANGER.rotPivotY;
            self->rotPivotX = DOPPLEGANGER.rotPivotX;
            self->ext.playerOutline.timer = 8;
            break;
        case 1: // Curse attack
            self->ext.playerOutline.brightness = 0x100;
            self->scaleX =
                DOPPLEGANGER.scaleX; // Player scaleX is (always?) 0x100
            self->scaleY =
                DOPPLEGANGER.scaleY; // Player scaleY is (always?) 0x100
            self->rotPivotY = DOPPLEGANGER.rotPivotY;
            self->rotPivotX = DOPPLEGANGER.rotPivotX;
            self->ext.playerOutline.timer = 8;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
        case 19:
        case 21: // Sword Warp Spell (#2)
            self->ext.playerOutline.brightness = 0x80;
            self->scaleX = DOPPLEGANGER.scaleX + 0x60;
            self->scaleY = DOPPLEGANGER.scaleY + 0x60;
            self->rotPivotY = DOPPLEGANGER.rotPivotY;
            self->rotPivotX = DOPPLEGANGER.rotPivotX;
            self->ext.playerOutline.timer = 8;
            break;
        }
        self->step++;
        break;

    case 1: // 8 frames at constant size
        switch (upperparams) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
        case 18:
        case 21: // Sword Warp Spell (#2)
            self->ext.playerOutline.brightness += 16;
        case 0: // MP refill
        case 1: // Curse attack
        case 2: // Agunea
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 17: // Dark Metamorphosis
        case 19:
        case 20: // Sword Warp Spell (#1)
        case 22: // Soul Steal
            if (--self->ext.playerOutline.timer == 0) {
                self->step++;
            }
        }
        break;
    case 2: // Outline grows/shrinks, and dims
        switch (upperparams) {
        case 0: // MP refill
        case 2: // Agunea
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 15:
        case 16:
        case 17: // Dark Metamorphosis
        case 18:
        case 20: // Sword Warp Spell (#1)
        case 22: // Soul Steal
            self->scaleX += 8;
            self->scaleY += 8;
            self->ext.playerOutline.brightness -= 5;
            if (self->ext.playerOutline.brightness < 0) {
                DestroyEntity(self);
                return;
            }
            break;
        case 1: // Curse attack, grows slower and dims faster
            self->scaleX += 2;
            self->scaleY += 2;
            self->ext.playerOutline.brightness -= 16;
            if (self->ext.playerOutline.brightness < 0) {
                DestroyEntity(self);
                return;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 14:
        case 19:
        case 21: // Sword Warp Spell (#2)
            // Shrinks inward, and when at size 0x100, holds there for 8 frames
            // in step 3
            self->scaleX -= 8;
            self->scaleY -= 8;
            if (self->scaleX <= 0x100) {
                self->scaleY = self->scaleX = 0x100;
                self->ext.playerOutline.timer = 8;
                self->step++;
            }
        }
        break;
    case 3: // Outline continues static until done
        if (--self->ext.playerOutline.timer == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    if (self->facingLeft) {
        selfX = selfX - xOffset;
    } else {
        selfX = selfX + xOffset;
    }
    selfY = selfY + yOffset;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i <= 0; i++) {
        if (self->facingLeft) {
            prim->x0 = prim->x2 = (selfX - width) + 1;
            prim->x1 = prim->x3 = selfX + 1;
        } else {
            prim->x0 = prim->x2 = selfX;
            prim->x1 = prim->x3 = selfX + width;
        }

        prim->y0 = prim->y1 = selfY;
        prim->y2 = prim->y3 = selfY + height;
        if (self->facingLeft) {
            prim->u0 = prim->u2 = spriteX - 1;
            prim->u1 = prim->u3 = four - 1;
        } else {
            prim->u0 = prim->u2 = four;
            prim->u1 = prim->u3 = spriteX;
        }
        prim->v0 = prim->v1 = one;
        prim->v2 = prim->v3 = one + height;
        ((void (*)(s32, s32, Entity*, s32, POLY_GT4*, s32))g_api.func_800EB758)(
            self->posX.i.hi, self->posY.i.hi, self, self->drawFlags,
            (POLY_GT4*)prim, (u16)self->facingLeft);
        prim->r0 = prim->r1 = prim->r2 = prim->r3 =
            primData[0] * self->ext.playerOutline.brightness / 256;
        prim->g0 = prim->g1 = prim->g2 = prim->g3 =
            primData[1] * self->ext.playerOutline.brightness / 256;
        prim->b0 = prim->b1 = prim->b2 = prim->b3 =
            primData[2] * self->ext.playerOutline.brightness / 256;
        prim->priority = DOPPLEGANGER.zPriority + 2;
        prim = prim->next;
    }
    MarDisableAfterImage(1, 1);
}

void func_pspeu_09256D30(Entity* self) {
    Primitive* prim;
    s16 halfWidth;
    s32 i;
    s16 yOffset = -12;

    switch (self->step) {
    case 0:
        self->posY.i.hi = MARIA.posY.i.hi + 37;
        self->ext.subweapon.timer = 1536;
        self->primIndex = g_api.AllocPrimitives(PRIM_G4, 4);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;

        for (prim = &g_PrimBuf[self->primIndex], i = 0; prim != NULL; i++,
            prim = prim->next) {
            halfWidth =
                (self->ext.subweapon.timer >> 8) - i; // unused in this loop
            prim->r0 = 0;
            prim->g0 = 0;
            prim->b0 = 0xC0;
            prim->r1 = 0;
            prim->g1 = 0;
            prim->b1 = 0xC0;
            prim->r2 = 0x40;
            prim->g2 = 0x40;
            prim->b2 = 0x40;
            prim->r3 = 0x40;
            prim->g3 = 0x40;
            prim->b3 = 0x40;

            prim->priority = MARIA.zPriority - 2;
            prim->drawMode = DRAW_DITHERING | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        }
        self->step++;
        break;

    case 1:
        if (MARIA.velocityY > FIX(-1.5)) {
            self->step = 2;
        }
        // If transformed, timer drains faster
        if (g_Maria.status &
            (PLAYER_STATUS_BAT_FORM | PLAYER_STATUS_MIST_FORM)) {
            self->step = 3;
        }
        break;

    case 3:
        // note that with the fallthrough these decrements stack
        self->ext.subweapon.timer -= 160;
    case 2:
        self->ext.subweapon.timer -= 96;
        if (self->ext.subweapon.timer < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    for (prim = &g_PrimBuf[self->primIndex], i = 0; prim != NULL; i++,
        prim = prim->next) {
        // As timer counts down, beam gets narrower.
        halfWidth = (self->ext.subweapon.timer >> 8) - i;
        if (halfWidth < 0) {
            halfWidth = 0;
        }
        prim->x0 = self->posX.i.hi - halfWidth;
        prim->x1 = self->posX.i.hi + halfWidth;
        prim->x2 = MARIA.posX.i.hi - halfWidth;
        prim->x3 = MARIA.posX.i.hi + halfWidth;
        prim->y2 = prim->y3 = MARIA.posY.i.hi - yOffset;
        prim->y0 = prim->y1 = self->posY.i.hi;
    }
}

// local copy of EntityWingSmashTrail (see bo4/unk_46E7C.c)
void func_pspeu_092570B8(Entity* self) {
    if (!(MARIA.step_s == 3 && MARIA.step == Dop_MorphBat)) {
        DestroyEntity(self);
        return;
    }

    if (self->step == 0) {
        self->flags = FLAG_POS_CAMERA_LOCKED;
        self->animSet = MARIA.animSet;
        self->animCurFrame = MARIA.animCurFrame | ANIM_FRAME_LOAD;
        self->unk5A = 8;
        self->zPriority = MARIA.zPriority - 2;
        self->drawFlags =
            MARIA.drawFlags | (ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX);
        self->opacity = 0x80;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->rotate = MARIA.rotate;
        self->facingLeft = MARIA.facingLeft;
        self->palette = PAL_FLAG(0x202);
        self->scaleX = self->scaleY = 0x100;
        self->step++;
        return;
    }
    // This actually makes the wing smashes shrink over time, not rotate.
    self->scaleX -= 8;
    self->scaleY -= 8;
    self->animCurFrame = MARIA.animCurFrame | ANIM_FRAME_LOAD;
    if (self->opacity >= 5) {
        self->opacity -= 5;
    } else {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09257298);

extern AnimationFrame D_pspeu_09263D38[];
extern AnimationFrame D_pspeu_09263CF8[];
extern AnimationFrame D_pspeu_092637B0[];
// local copy of func_maria_80161C2C (see maria/pl_blueprints.c)
void func_maria_80161C2C(Entity* self) {
    s16 paramsHi;
    s16 paramsLo;

    paramsLo = self->params & 0xFF;
    paramsHi = (self->params >> 8) & 0xFF;
    switch (self->step) {
    case 0:
        if (paramsHi == 1) {
            self->scaleX = 0xC0;
            self->scaleY = 0xC0;
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->animSet = ANIMSET_DRA(2);
            self->anim = D_pspeu_09263D38;
        }
        if (paramsHi == 0 || paramsHi == 2) {
            if (paramsLo & 3) {
                self->anim = D_pspeu_09263CF8;
                self->scaleX = 0x120;
                self->scaleY = 0x120;
                self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
                self->animSet = ANIMSET_DRA(2);
            } else {
                self->animSet = ANIMSET_DRA(5);
                self->anim = D_pspeu_092637B0;
                self->palette = PAL_FLAG(PAL_UNK_170);
            }
        }
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        if (rand() & 3) {
            self->zPriority = MARIA.zPriority + 2;
        } else {
            self->zPriority = MARIA.zPriority - 2;
        }
        if (paramsHi == 2) {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() % 44) - 22;
        } else {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() & 15) - 8;
        }
        self->posY.i.hi =
            MARIA.posY.i.hi + MARIA.hitboxOffY + (rand() & 31) - 16;
        self->velocityY = FIX(-0.5);
        self->velocityX = MARIA.velocityX >> 2;
        self->step++;
        break;
    case 1:
        self->scaleX -= 4;
        self->scaleY -= 4;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if ((self->pose == 8) && (self->anim != D_pspeu_092637B0)) {
            self->blendMode = BLEND_TRANSP;
            if (!(paramsLo & 1) && self->poseTimer == 1) {
                MarCreateEntFactoryFromEntity(self, FACTORY(BP_4, 4), 0);
            }
        }
        if (self->pose == 16 && self->anim == D_pspeu_092637B0) {
            self->blendMode = BLEND_TRANSP;
        }
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern u16 D_pspeu_09263D70[];
// local copy of MarEntityHitByLightning (see maria/pl_blueprints.c),
// using the boss lifetime, flags and sound effects from rbo5/unk_4648C.c.
void MarEntityHitByLightning(Entity* self) {
    s16 x;
    s16 y;
    s16 mul2;
    s32 i;
    s32 xMod;
    s32 yMod;
    bool terminate;
    s32 mul;
    s16 angle;
    Primitive* primPrev;
    Primitive* prim;

    terminate = false;
    if ((self->params & 0xFF00) != 0) {
        if (++self->ext.hitbylightning.unk9C > 0xA8) {
            terminate = true;
        }
    } else if (MARIA.step != Dop_Hit) {
        terminate = true;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }

        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x3F);
        self->ext.hitbylightning.unk80 = (rand() & PSP_RANDMASK);
        self->ext.hitbylightning.unk82 = (rand() & 0x1FF) + 0x100;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi;
            prim->tpage = 0x1A;
            prim->clut = D_pspeu_09263D70[rand() & 1];
            prim->priority = MARIA.zPriority - 2;
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            prim = prim->next;
        }
        if ((MARIA.velocityY != 0) && (MARIA.step != Dop_Kill)) {
            self->ext.hitbylightning.unk92 = 1;
        }
        self->ext.hitbylightning.unk94 = 0x10;
        g_api.PlaySfx(SFX_MAGIC_SWITCH);
        g_api.PlaySfx(SFX_THUNDER_B);
        self->step++;
        break;
    case 1:
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x1FF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) * 12;
        yMod = (-((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) * 7) << 1;
        self->posX.val = xMod + MARIA.posX.val;
        self->posY.val = yMod + MARIA.posY.val;
        if (self->ext.hitbylightning.unk92 &&
            g_Maria.vram_flag &
                (TOUCHING_L_WALL | TOUCHING_R_WALL | TOUCHING_CEILING)) {
            terminate = true;
        }
        if (terminate) {
            self->ext.hitbylightning.unk90 = (rand() & 0xF) + 0x10;
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbylightning.unk90 == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.hitbylightning.unk94 > 0) {
            self->ext.hitbylightning.unk94--;
        }
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0xFF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) *
               ((rand() % 8) + 8);
        yMod = -((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) *
                   ((rand() % 8) + 10) +
               self->ext.hitbylightning.unk98;
        self->posX.val = MARIA.posX.val + xMod;
        self->posY.val = MARIA.posY.val + yMod;
        self->ext.hitbylightning.unk98 -= 0x8000;
        prim = &g_PrimBuf[self->primIndex];
        break;
    }

    x = (self->posX.i.hi + (rand() & 7)) - 4;
    y = (self->posY.i.hi + (rand() & 0x1F)) - 0x18;
    mul2 = self->ext.hitbylightning.unk94;
    mul2 = (mul2 * rsin(self->ext.hitbylightning.unk80)) >> 0xC;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 5; i++) {
        primPrev = prim;
        prim = prim->next;
        *primPrev = *prim;
        primPrev->next = prim;
        primPrev->u0 = primPrev->u2 = (i * 0x10) + 0x90;
        primPrev->u1 = primPrev->u3 = ((i + 1) * 0x10) + 0x90;
        primPrev->v0 = primPrev->v1 = 0xC0;
        primPrev->v2 = primPrev->v3 = 0xCF;
    }
    prim->x0 = prim->x1;
    prim->y0 = prim->y1;
    prim->x2 = prim->x3;
    prim->y2 = prim->y3;
    angle = self->ext.hitbylightning.unk7C + 0x400;
    prim->x1 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y1 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk7C - 0x400;
    prim->x3 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y3 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk80 & 0xFFF;
    if (angle < 0x400) {
        prim->priority = MARIA.zPriority + 2;
    } else if (angle < 0xC00) {
        prim->priority = MARIA.zPriority - 2;
    } else {
        prim->priority = MARIA.zPriority + 2;
    }
    prim->u0 = prim->u2 = (i << 4) + 0x90;
    prim->u1 = prim->u3 = ((i + 1) << 4) + 0x90;
    prim->v0 = prim->v1 = 0xC0;
    prim->v2 = prim->v3 = 0xCF;
}

extern Point16* D_pspeu_09263E08[72];
void func_pspeu_09258A50(Entity* self) {
    const int PrimCount = LEN(D_pspeu_09263E08) / 3;
    s16 x;
    s16 y;
    s32 i;
    bool terminateFlag;
    s16 deltaX;
    s16 deltaY;
    s16 yMod;
    s32 distance;
    s16 angle;
    Point16* pos;
    Primitive* prim;

    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    terminateFlag = 0;
    if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
        terminateFlag = 1;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT3, PrimCount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = (rand() & 0x0F) + 0x30;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = (rand() & 0x7F) + 0x80;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = (rand() & 0x1F) + 0x30;
            if (rand() & 1) {
                prim->drawMode =
                    DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            } else {
                prim->drawMode = DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            }
            prim->type = PRIM_G4;
            prim->priority = MARIA.zPriority + 2;
            prim = prim->next;
        }
        if (MARIA.velocityY != 0) {
            self->ext.hitbyice.unk7E = 1;
        }
        if (MARIA.step == Dop_Kill) {
            self->ext.hitbyice.unk80 = 1;
            self->ext.hitbyice.unk82 = 0x14;
            self->ext.hitbyice.unk7E = 0;
        }
        if (MARIA.velocityY != 0) {
            if (MARIA.facingLeft) {
                self->rotate = ROT(22.5);
            } else {
                self->rotate = ROT(-22.5);
            }
        } else {
            if (MARIA.velocityX > 0) {
                self->rotate = ROT(11.25);
            } else {
                self->rotate = ROT(348.75);
            }
        }
        self->step++;
        break;
    case 1:
        if (self->ext.hitbyice.unk80 && --self->ext.hitbyice.unk82 == 0) {
            terminateFlag = true;
        }
        // Could rewrite as a series of && and || but that would probably reduce
        // readability
        if (self->ext.hitbyice.unk7E) {
            if (g_Dop.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) {
                terminateFlag = true;
            }
            if (MARIA.step == Dop_Hit && MARIA.step_s == 5) {
                terminateFlag = true;
            }
        }
        if (terminateFlag) {
            self->ext.hitbyice.unk7C = 0x40;
            g_api.PlaySfx(SFX_MAGIC_SWITCH);
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbyice.unk7C == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < PrimCount; i++) {
        pos = D_pspeu_09263E08[i * 3];
        if (prim->u0 < 2) {
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x0 = x + deltaX;
            prim->y0 = y + deltaY;

            pos = D_pspeu_09263E08[i * 3 + 1];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x1 = x + deltaX;
            prim->y1 = y + deltaY;

            pos = D_pspeu_09263E08[i * 3 + 2];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x2 = prim->x3 = x + deltaX;
            prim->y2 = prim->y3 = y + deltaY;
        }
        if (prim->u0 == 0 && terminateFlag) {
            prim->u0++;
            prim->v0 = (rand() & 15) + 1;
        }
        if (prim->u0 == 1) {
            if (--prim->v0 == 0) {
                prim->u0++;
                prim->v0 = 0x20;
                prim->u2 = 0xF0;
            }
        }
        if (prim->u0 == 2) {
            if (prim->u2 < 0x70 || prim->u2 > 0xD0) {
                prim->u2 += 4;
            }
            yMod = (s8)prim->u2 >> 4;
            prim->y0 += yMod;
            prim->y1 += yMod;
            prim->y2 += yMod;
            prim->y3 += yMod;
            if (prim->r3 < 4) {
                prim->r3 -= 4;
            }
            if (prim->g3 < 4) {
                prim->g3 -= 4;
            }
            if (prim->b3 < 4) {
                prim->b3 -= 4;
            }
            prim->r0 = prim->r1 = prim->r2 = prim->r3;
            prim->b0 = prim->b1 = prim->b2 = prim->b3;
            prim->g0 = prim->g1 = prim->g2 = prim->g3;
            prim->drawMode |= DRAW_UNK02;
            prim->drawMode &= ~DRAW_UNK02;
            if (--prim->v0 == 0) {
                prim->drawMode |= DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}

typedef struct {
    f32 posX;
    f32 posY;
    s16 angle1;
    s16 angle2;
    s16 size;
    s16 xOffset;
    s16 yOffset;
    s16 pad;
} mistStruct;

extern Primitive D_pspeu_0926C920[8];
extern u32 D_pspeu_09263FA8[8];
extern u32 D_pspeu_09263FC8[8];
extern mistStruct D_pspeu_0926CB20[16];
Primitive* func_pspeu_09259480(
    bool arg0, s32 arg1, Primitive* arg2, s16 facingLeft) {
    s32 primU0;
    s32 tempU;
    s32 tempV;
    s32 primU1;
    s32 primV0;
    s32 primV1;
    s16 angle1;
    s16 angle2;
    Primitive* prim;
    s32 i;

    if (!arg0) {
        if (facingLeft) {
            prim = &D_pspeu_0926C920[D_pspeu_09263FA8[arg1 >> 1]];
            primU0 = prim->u0;
            tempU = prim->u1;
            primV0 = prim->v0;
            tempV = prim->v1;
        } else {
            prim = &D_pspeu_0926C920[D_pspeu_09263FC8[arg1 >> 1]];
            primU0 = prim->u1;
            tempU = prim->u0;
            primV0 = prim->v1;
            tempV = prim->v0;
        }

        primU1 = (prim->u0 + prim->u1) / 2;
        primV1 = (prim->v0 + prim->v1) / 2;

        if (arg1 & 1) {
            primU0 = primU1;
            primU1 = tempU;
            primV0 = primV1;
            primV1 = tempV;
        }
        arg2->u0 = primU0;
        arg2->v0 = primV0;
        arg2->u1 = primU1;
        arg2->v1 = primV1;
        arg2->u3 = arg2->u2 = prim->u2;
        arg2->v3 = arg2->v2 = prim->v2;
        if (DOPPLEGANGER.animSet == 0xF) {
            arg2->tpage = 0x118;
        } else {
            arg2->tpage = 0x18;
        }
        arg2->clut = 0x20F;
        arg2->priority = DOPPLEGANGER.zPriority + 2;
        arg2->drawMode = DRAW_DITHERING | DRAW_UNK_100 | DRAW_TPAGE2 |
                         DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        arg2 = arg2->next;
    } else {
        angle1 = D_pspeu_0926CB20[arg1].angle1;
        angle2 = D_pspeu_0926CB20[(arg1 + 1) % 16].angle1;

        for (i = 0; i < 4; i++) {
            // nb: the cos/sin arguments seem to be invariant, could've been
            // extracted outside the loop
            arg2->u0 = 0x60 + ((((rcos(angle1) >> 4) * 8) * (i + 1)) >> 8);
            arg2->v0 = 0xA0 - ((((rsin(angle1) >> 4) * 8) * (i + 1)) >> 8);
            arg2->u1 = 0x60 + ((((rcos(angle2) >> 4) * 8) * (i + 1)) >> 8);
            arg2->v1 = 0xA0 - ((((rsin(angle2) >> 4) * 8) * (i + 1)) >> 8);

            if (i == 3) {
                if (arg2->u0 < 4) {
                    arg2->u0 = -1;
                }
                if (arg2->u1 < 4) {
                    arg2->u1 = -1;
                }
                if (arg2->v0 < 4) {
                    arg2->v0 = -1;
                }
                if (arg2->v1 < 4) {
                    arg2->v1 = -1;
                }
            }

            arg2->u2 = 0x60 + ((((rcos(angle1) >> 4) * 8) * i) >> 8);
            arg2->v2 = 0xA0 - ((((rsin(angle1) >> 4) * 8) * i) >> 8);
            arg2->u3 = 0x60 + ((((rcos(angle2) >> 4) * 8) * i) >> 8);
            arg2->v3 = 0xA0 - ((((rsin(angle2) >> 4) * 8) * i) >> 8);

            arg2->tpage = 0x18;
            arg2->clut = 0x20F;
            arg2->priority = DOPPLEGANGER.zPriority + 4;
            arg2->drawMode =
                DRAW_DITHERING | DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            arg2 = arg2->next;
        }
    }
    return arg2;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09259A20);

void func_pspeu_0925BA80(Entity* self) {
    const int PrimCount = 36;
    const int Iterations = 16;

    u8 xMargin;
    u8 yMargin;
    u8 wSprite;
    u8 hSprite;
    s16 blueOffset;
    s16 xPivot;
    s16 yPivot;
    s16 width;
    s16 height;
    s16 s5;
    s16 s7;
    s16 sp42;
    u_long* data;
    s32 sp3C;
    s16* sp38;
    Primitive* prim;
    s32 i;
    u8* s2;
    s16 s3;
    s16 s4;
    s32 s6;
    u8* plSprite;

    s32 xBase, yBase, angle, tangent;

    if (DOPPLEGANGER.ext.player.anim != 0x38 &&
        (self->step == 2 || self->step == 3)) {
        self->step = 4;
        LoadImage(&D_pspeu_09263FF0, (u_long*)D_pspeu_0926C110);
        self->ext.dissolve.unk7E = rand() & 0xFF;
        self->ext.dissolve.unk7C = 0x30;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < PrimCount; i++) {
            prim->drawMode &= ~DRAW_HIDE;
            prim = prim->next;
        }
    }

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, PrimCount);
        if (self->primIndex == -1) {
            return;
        }
        self->flags = FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        self->ext.dissolve.unk7C = 0;
        self->ext.dissolve.unk80 = rand() & 7;
        self->ext.dissolve.unk7E = D_pspeu_09263FF8[self->ext.dissolve.unk80]
                                                   [self->ext.dissolve.unk7C];

        s5 = self->posX.i.hi = DOPPLEGANGER.posX.i.hi;
        s7 = self->posY.i.hi = DOPPLEGANGER.posY.i.hi;
        self->facingLeft = DOPPLEGANGER.facingLeft;

        sp38 = (s16*)D_pspeu_092641C8[DOPPLEGANGER.animCurFrame & 0x7FFF];
        sp42 = *sp38++;
        sp42 &= 0x7FFF;
        plSprite = D_pspeu_09266768[sp42];
        xMargin = 4;
        yMargin = 1;
        wSprite = xMargin + plSprite[0];
        hSprite = yMargin + plSprite[1];
        width = wSprite - xMargin;
        height = hSprite - yMargin;
        s3 = width / 6;
        s4 = height / 6;
        xPivot = sp38[0] + plSprite[2];
        yPivot = sp38[1] + plSprite[3];
        if (self->facingLeft) {
            s5 = s5 - xPivot;
        } else {
            s5 = s5 + xPivot;
        }
        s7 = s7 + yPivot;

        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < PrimCount; i++) {

            if (self->facingLeft) {
                prim->x0 = prim->x2 = (s5 - (s3 * (i % 6))) + 1;
                prim->x1 = prim->x3 = prim->x0 - s3;
            } else {
                prim->x0 = prim->x2 = s5 + (s3 * (i % 6));
                prim->x1 = prim->x3 = prim->x0 + s3;
            }
            prim->y0 = prim->y1 = s7 + (s4 * (i / 6));
            prim->y2 = prim->y3 = prim->y0 + s4;
            prim->u0 = prim->u2 = (xMargin) + (s3 * (i % 6)) + 0x80;
            prim->u1 = prim->u3 = prim->u0 + s3;
            prim->v0 = prim->v1 = (yMargin + (s4 * (i / 6)));
            prim->v2 = prim->v3 = prim->v0 + s4;
            prim->g0 = (((i / 6) * 2) + (rand() & 3));
            prim->tpage = 0x10;
            prim->clut = DOPPLEGANGER.palette & 0x7FFF;
            prim->priority = DOPPLEGANGER.zPriority + 2;
            prim->drawMode = DRAW_HIDE | DRAW_UNK02;
            prim->r3 = i * 7;
            prim = prim->next;
        }
        self->step++;
        break;
    case 1:
        StoreImage(&D_pspeu_09263FE8, (u_long*)D_pspeu_0926C110);

        self->step++;
        // fallthrough
    case 2:
        DOPPLEGANGER.animCurFrame |= 0x8000;
        if (g_Dop.padTapped & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) {
            self->step++;
        }
        break;
    case 3:
        DOPPLEGANGER.animCurFrame |= 0x8000;
        self->ext.dissolve.unk7E = D_pspeu_09263FF8[self->ext.dissolve.unk80]
                                                   [self->ext.dissolve.unk7C];
        data = (u_long*)D_pspeu_0926C110;
        s2 = (u8*)data;
        s2 = s2 + ((self->ext.dissolve.unk7E >> 1) & 7);
        s2 = s2 + (((self->ext.dissolve.unk7E & 0xFF) >> 4) << 5);
        for (i = 0; i < Iterations; i++) {
            if (rand() & 3) {
                if (self->ext.dissolve.unk7E & 1) {
                    if (*(s2 + (i & 3) * 8 + (i >> 2) * 0x200)) {
                        *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) &= 0xF0;
                        *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) |= 1;
                    }
                } else if (*(s2 + (i & 3) * 8 + (i >> 2) * 0x200)) {
                    *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) &= 0x0F;
                    *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) |= 0x10;
                }
            }
        }
        LoadImage(&D_pspeu_09263FE8, data);
        if (++self->ext.dissolve.unk7C == 8) {
            self->ext.dissolve.unk7C = 0;
            self->ext.dissolve.unk80 += 1;
            self->ext.dissolve.unk80 &= 7;
            self->step = 2;
        }
        break;
    case 4:
        if (DOPPLEGANGER.step == Dop_Kill || g_Dop.unk6A <= 0) {
            D_pspeu_0926CCD8 = 1;
            if (g_Timer % 2 == 0) {
                goto after_loop;
            }
        } else if (g_Timer % 3 == 0) {
            break;
        }

        for (sp3C = 0; sp3C < 6; sp3C++) {
            data = (u_long*)D_pspeu_0926C110;
            s2 = (u8*)data;
            s2 = s2 + ((self->ext.dissolve.unk7E >> 1) & 7);
            s2 = s2 + (((self->ext.dissolve.unk7E & 0xFF) >> 4) << 5);
            for (i = 0; i < Iterations; i++) {
                if (self->ext.dissolve.unk7E & 1) {
                    *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) &= 0xF0;
                } else {
                    *(s2 + (i & 3) * 8 + (i >> 2) * 0x200) &= 0x0F;
                }
            }
            // some sort of prng state
            self->ext.dissolve.unk7E += 0x23;
            self->ext.dissolve.unk7E &= 0xFF;
        }

        LoadImage(&D_pspeu_09263FF0, data);
        if (--self->ext.dissolve.unk7C <= 0) {
            if ((DOPPLEGANGER.step == Dop_Kill) || g_Dop.unk6A <= 0) {
                if (self->ext.dissolve.unk7C < -0x1F) {
                    D_pspeu_0926CCD8 = 2;
                    DestroyEntity(self);
                    return;
                }
            } else {
                DestroyEntity(self);
                return;
            }
        }

    after_loop:
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < PrimCount; i++) {
            // prim->drawMode &= DRAW_UNK_200;
            // prim->drawMode |= DRAW_UNK02;
            if (prim->r0 == 0) {
                if (--prim->g0 == 0) {
                    prim->r0++;
                    prim->b0 = ((rand() & 3) + 0xF8);
                    prim->r1 = 0x20 - ((i / 6) * 2);
                }
            } else if (DOPPLEGANGER.step == Dop_Kill || g_Dop.unk6A <= 0) {
                xBase = D_pspeu_0926CD00 - (prim->x0 + prim->x1) / 2;
                yBase = D_pspeu_0926CCF8 - ((prim->y0 + prim->y2) / 2);
                tangent = ratan2(-yBase, xBase);
                angle = prim->r3;
                angle *= 0x10;
                if ((angle - tangent) < 0) {
                    angle += 0x80;
                }

                if ((angle - tangent) > 0) {
                    angle -= 0x80;
                }
                angle &= 0xFFF;
                xBase = rcos(angle) >> 0xB;
                yBase = rsin(angle) >> 0xB;
                angle >>= 4;
                prim->r3 = angle;
                prim->x0 += xBase;
                prim->x1 += xBase;
                prim->x2 += xBase;
                prim->x3 += xBase;
                prim->y0 -= yBase;
                prim->y1 -= yBase;
                prim->y2 -= yBase;
                prim->y3 -= yBase;
            } else {
                if (prim->r1) {
                    prim->r1--;
                } else {
                    prim->drawMode = DRAW_HIDE;
                }
                if (prim->b0 < 0x30 || prim->b0 > 0xD0) {
                    prim->b0 += 2;
                }
                blueOffset = (s16)((s8)prim->b0 >> 4);
                prim->y0 += blueOffset;
                prim->y1 += blueOffset;
                prim->y2 += blueOffset;
                prim->y3 += blueOffset;
            }
            prim = prim->next;
        }
        break;
    }

    if (DOPPLEGANGER.step == Dop_Kill || g_Dop.unk6A <= 0) {
        D_pspeu_0926CCD8 = 1;
    }
    MarDisableAfterImage(1, 1);
}
