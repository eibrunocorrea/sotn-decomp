// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092457C8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09245AD8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09246210);

extern s32 D_pspeu_0926BBA0;
extern s32 D_pspeu_0926BBA8;
// local copy of func_us_801B5A14 (see bo6/richter.c): set the boss
// think-step and reset its timer
void func_pspeu_09247190(s32 step) {
    D_pspeu_0926BBA8 = step;
    D_pspeu_0926BBA0 = 0;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092471B8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09248698);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249910);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249FE8);

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

extern s16 D_pspeu_09263098[];
extern s16 D_pspeu_092630A0[];
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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924BBE8);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarDisableAfterImage);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092509D0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092514F0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09251748);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_8015FDB0);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252B20);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252B48);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253620);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09256258);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09256D30);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_maria_80161C2C);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarEntityHitByLightning);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09258A50);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09259480);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09259A20);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0925BA80);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0925CA08);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_us_801BDA0C);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0925D968);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0925DB68);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_us_801BE2C8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092481F0_from_bo3);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0925EF30);
