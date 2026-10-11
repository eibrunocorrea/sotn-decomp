// SPDX-License-Identifier: AGPL-3.0-or-later
#include "player_copy.h"

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_1CA48", func_pspeu_092540C8);

extern s32 D_pspeu_0927C278;
extern s32 D_pspeu_0927C280;
// local copy of func_us_801B5A14 (see bo6/richter.c): set the boss
// think-step and reset its timer
void func_pspeu_09247190_from_rbo5(s32 step) {
    D_pspeu_0927C280 = step;
    D_pspeu_0927C278 = 0;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_1CA48", func_pspeu_09254F38);

extern s32 D_pspeu_0927C248;
extern s32 D_pspeu_0927C148;
extern s32 D_pspeu_0927C250;
void func_pspeu_09256610(void) {
    switch (D_pspeu_0927C248) {
    case 0:
        break;
    case 10:
        if (!g_CastleFlags[SHAFT_ORB_DEFEATED] &&
            (g_DemoMode == Demo_None)) {
            g_unkGraphicsStruct.pauseEnemies = true;
            D_pspeu_0927C250 = 0;
        }
        D_pspeu_0927C248++;
        break;
    case 11:
        if (++D_pspeu_0927C250 > 1) {
            if (!g_CastleFlags[SHAFT_ORB_DEFEATED] &&
                (g_DemoMode == Demo_None)) {
                g_unkGraphicsStruct.unk28 = 0xFF;
            }
            D_pspeu_0927C248++;
        }
        break;
    case 12:
        break;
    case 20:
        if (D_pspeu_0927C148 & 4) {
            D_pspeu_0927C248 = 0x1E;
        }
        break;
    case 30:
        break;
    case 40:
        D_pspeu_0927C148 |= 8;
        break;
    case 50:
    default:
        break;
    }
}


extern EInit g_EInitRichterBelmont;
extern s32 D_pspeu_0927C258;
extern s32 D_pspeu_0927C260;
void MarDisableAfterImage(s32 resetAnims, s32 arg1);
void func_pspeu_09253AA8(void);
void func_pspeu_09254F38(void);
void func_pspeu_092540C8(void);
void func_pspeu_09252B48_from_rbo5(void);
void func_pspeu_09256610(void);
// Local copy of EntityRichter (see boss/bo6/richter.c).
void func_pspeu_09256778(Entity* self) {
    Entity* entity;
    s32 i;

    g_Maria.unk6A = MARIA.hitPoints;
    if (self->step == 0) {
        InitializeEntity(g_EInitRichterBelmont);
        func_pspeu_09253AA8();
        entity = &g_Entities[STAGE_ENTITY_START + 4];
        for (i = STAGE_ENTITY_START + 4; i < 144; i++, entity++) {
            DestroyEntity(entity);
        }
        g_Maria.unk6E = g_Maria.unk6A = g_Maria.unk6C = MARIA.hitPoints;
        D_pspeu_0927C258 = g_Maria.unk6E / 2;
        D_pspeu_0927C260 = 0;
        g_Maria.unk70 = MARIA.hitboxState;
        func_pspeu_09247190_from_rbo5(18);
        MarDisableAfterImage(1, 48);
    } else {
        func_pspeu_09254F38();
        func_pspeu_092540C8();
        func_pspeu_09252B48_from_rbo5();
        func_pspeu_09256610();
    }
    g_Maria.unk6C = g_Maria.unk6A;
}

extern AnimationFrame D_pspeu_0927A5B8[];
extern AnimationFrame D_pspeu_0927A7E0[];
extern AnimationFrame D_pspeu_0927A970[];
void func_pspeu_092568D0(void) {
    s16 xMod;
    s32 facing;

    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP | CHECK_CRASH |
                       CHECK_ATTACK | CHECK_CROUCH | CHECK_SLIDE)) {
        MarDecelerateX(FIX(0.125));
        switch (MARIA.step_s) {
        case 0:
            if (MarCheckFacing()) {
                func_pspeu_09259BB0(0);
            } else if (g_Maria.padPressed & PAD_UP) {
                MarSetAnimation(D_pspeu_0927A5B8);
                MARIA.step_s = 1;
                break;
            }
            break;
        case 1:
            if (MarCheckFacing()) {
                func_pspeu_09259BB0(0);
            } else if (g_Maria.padPressed & PAD_UP) {
            } else {
                func_pspeu_09259AD8(0);
            }
            break;
        case 64:
            MarDisableAfterImage(1, 1);
            if (MARIA.pose < 3) {
                facing = MarCheckFacing();
                if (g_Maria.padPressed & PAD_DOWN) {
                    MARIA.step = PL_S_CROUCH;
                    MARIA.anim = D_pspeu_0927A7E0;
                    break;
                }
            }

            if (MARIA.poseTimer < 0) {
                if (g_Maria.padPressed & PAD_SQUARE) {
                    MARIA.step_s++;
                    g_Maria.unk46 = 2;
                    MarSetAnimation(D_pspeu_0927A970);
                    MarCreateEntFactoryFromEntity(
                        g_CurrentEntity, 0x11, 0);
                    break;
                }
                g_Maria.unk46 = 0;
                func_pspeu_09259AD8(0);
            }
            break;
        case 65:
            MarDisableAfterImage(1, 1);
            if (g_Maria.padPressed & PAD_SQUARE) {
                break;
            }
            g_Maria.unk46 = 0;
            func_pspeu_09259AD8(0);
            break;
        case 66:
            MarDisableAfterImage(1, 1);
            if (MARIA.pose < 3) {
                MarCheckFacing();
            }
            if (MARIA.poseTimer < 0) {
                g_Maria.unk46 = 0;
                func_pspeu_09259AD8(0);
            }
            break;
        }
    }
}


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

extern AnimationFrame D_pspeu_0927A980[];
extern AnimationFrame D_pspeu_0927A758[];
void MarDisableAfterImage(s32 resetAnims, s32 arg1);
void func_pspeu_09256D28(void) {
    s32 facing;

    if ((g_Maria.vram_flag & TOUCHING_CEILING) && (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-0.25);
        g_Maria.unk44 |= 0x20;
    }
    if (func_pspeu_0925B680(
            CHECK_GROUND | CHECK_FACING | CHECK_ATTACK | CHECK_GRAVITY_JUMP)) {
        return;
    }
    switch (MARIA.step_s) {
    case 0:
        MarDecelerateX(FIX(0.0625));
        facing = MarCheckFacing();
        if (facing) {
            if (g_Maria.unk44 & 0x10) {
                MarSetSpeedX(FIX(2.25));
            } else {
                MarSetSpeedX(FIX(1.25));
            }
            g_Maria.unk44 &= ~4;
        } else {
            g_Maria.unk44 &= ~0x10;
            if ((MARIA.pose < 2) && !(g_Maria.unk44 & 8) &&
                (g_Maria.unk44 & 4) && (g_Maria.padTapped & PAD_CROSS)) {
                MarSetAnimation(D_pspeu_0927A790);
                MarSetSpeedX(FIX(-1.5));
                MARIA.velocityY = FIX(-2.625);
                g_Maria.unk44 |= 0xA;
                g_Maria.unk44 &= ~4;
                MARIA.step_s = 2;
            }
        }
        break;
    case 2:
        break;
    case 0x40:
        MarDisableAfterImage(1, 1);
        if (MARIA.pose < 3) {
            facing = MarCheckFacing();
            if (facing) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        } else {
            if (((g_Maria.padPressed & PAD_RIGHT) && MARIA.facingLeft == 0) ||
                ((g_Maria.padPressed & PAD_LEFT) && MARIA.facingLeft)) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                MarSetSpeedX(FIX(1.25));
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        }
        if (MARIA.poseTimer < 0) {
            if (g_Maria.padPressed & PAD_SQUARE) {
                MARIA.step_s += 1;
                g_Maria.unk46 = 2;
                MarSetAnimation(D_pspeu_0927A980);
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, BP_ARM_BRANDISH_WHIP, 0);
            } else {
                g_Maria.unk46 = 0;
                MARIA.step_s = 0;
                MarSetAnimation(D_pspeu_0927A758);
            }
        }
        break;
    case 0x41:
        MarDisableAfterImage(1, 1);
        if (!(g_Maria.padPressed & PAD_SQUARE)) {
            g_Maria.unk46 = 0;
            MARIA.step_s = 0;
            MarSetAnimation(D_pspeu_0927A758);
        }
        break;
    case 0x42:
        MarDisableAfterImage(1, 1);
        if (MARIA.pose < 3) {
            facing = MarCheckFacing();
            if (facing) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        } else {
            if (((g_Maria.padPressed & PAD_RIGHT) && MARIA.facingLeft == 0) ||
                ((g_Maria.padPressed & PAD_LEFT) && MARIA.facingLeft)) {
                if (g_Maria.unk44 & 0x10) {
                    MarSetSpeedX(FIX(2.25));
                } else {
                    MarSetSpeedX(FIX(1.25));
                }
                MarSetSpeedX(FIX(1.25));
                g_Maria.unk44 &= ~4;
            } else {
                g_Maria.unk44 &= ~0x10;
            }
        }
        if (MARIA.poseTimer < 0) {
            g_Maria.unk46 = 0;
            MARIA.step_s = 0;
            MarSetAnimation(D_pspeu_0927A758);
        }
        break;
    }
}

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

extern AnimationFrame D_pspeu_0927A708[];
extern AnimationFrame D_pspeu_0927A7C0[];
extern AnimationFrame D_pspeu_0927A978[];
void func_pspeu_09257418(void) {
    s32 facing;
    if (!func_pspeu_0925B680(CHECK_FALL | CHECK_FACING | CHECK_JUMP |
                           CHECK_CRASH | CHECK_ATTACK | CHECK_SLIDE)) {
        MarDecelerateX(FIX(0.125));
        switch (MARIA.step_s) {
        case 0:
            if (!(g_Maria.padPressed & PAD_DOWN)) {
                MarSetAnimation(D_pspeu_0927A708);
                MARIA.step_s = 2;
                return;
            }
            break;
        case 1:
            if (!(g_Maria.padPressed & PAD_DOWN)) {
                if (MarCheckFacing()) {
                    func_pspeu_09259BB0(0);
                    return;
                }
                MARIA.anim = D_pspeu_0927A708;
                MARIA.step_s = 2;
                MARIA.pose = 2 - MARIA.pose;
                MARIA.poseTimer = 1;
                break;
            }
        case 4:
            if (MARIA.poseTimer != -1) {
                return;
            }
            MarSetAnimation(D_pspeu_0927A6E8);
            MARIA.step_s = 0;
            break;
        case 2:
            if (MarCheckFacing()) {
                func_pspeu_09259BB0(0);
                return;
            }
            if (MARIA.poseTimer == -1) {
                func_pspeu_09259AD8(0);
                return;
            }
            break;
        case 3:
            if (MARIA.poseTimer < 0) {
                MarSetAnimation(D_pspeu_0927A6E8);
                MARIA.step_s = 0;
                return;
            }
            break;
        case 0x40:
            MarDisableAfterImage(1, 1);
            if (MARIA.pose < 3) {
                facing = MarCheckFacing();
                if (!(g_Maria.padPressed & PAD_DOWN)) {
                    MARIA.step = PL_S_STAND;
                    MARIA.anim = D_pspeu_0927A7C0;
                    return;
                }
            }
            if (MARIA.poseTimer < 0) {
                if (g_Maria.padPressed & PAD_SQUARE) {
                    MARIA.step_s++;
                    g_Maria.unk46 = 2;
                    MarSetAnimation(D_pspeu_0927A978);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x11, 0);
                    return;
                }
                g_Maria.unk46 = 0;
                MARIA.step_s = 0;
                MarSetAnimation(D_pspeu_0927A6E8);
            }
            break;
        case 0x41:
            MarDisableAfterImage(1, 1);
            if (!(g_Maria.padPressed & PAD_SQUARE)) {
                g_Maria.unk46 = 0;
                MARIA.step_s = 0;
                MarSetAnimation(D_pspeu_0927A6E8);
            }
            break;
        }
    }
}


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

// SPDX-License-Identifier: AGPL-3.0-or-later
// Unverified PSP seed adapted from ric/pl_steps.c::RicStepHit and target asm.
// Unused fourth argument is preserved for the target frame layout.

extern PlayerState g_Maria;
extern AnimationFrame D_pspeu_0927A8D0[];
extern AnimationFrame D_pspeu_0927A8D8[];
extern AnimationFrame D_pspeu_0927A900[];
extern AnimationFrame D_pspeu_0927A940[];

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 blueprint, s32 arg2);
void MarResetPose(void);
void MarDecelerateX(s32 speed);
void MarSetCrouch(s32 kind, s32 velocityX);
void RicSetDeadPrologue(void);
void func_pspeu_0924EA50_from_rbo5(s32 velocityX);
void func_pspeu_092577D8(void);
void func_pspeu_09259AD8(s32 velocityX);
void func_pspeu_09259D48(void);
bool func_pspeu_0925B680(s32 checks);

void func_pspeu_09257820(
    s32 damageEffect, u32 damageKind, s16 prevStep, s32 prevStepS) {
    switch (MARIA.step_s) {
    case 0:
        MarResetPose();
        if (damageKind < 0x10U) {
            func_pspeu_092577D8();
        } else {
            MARIA.entityRoomIndex = MARIA.facingLeft;
        }
        if (damageEffect & ELEMENT_THUNDER) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x30, 0);
            MARIA.velocityY = FIX(-4);
            func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
            MARIA.step_s = 1U;
            MARIA.anim = D_pspeu_0927A900;
            g_Maria.damagePalette = PAL_FLAG(0x220);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x30, 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x460021, 0);
            g_Maria.timers[PL_T_2] = 6;
            g_api.PlaySfx(SFX_BOSS_RIC_PAIN_F);
            return;
        }
        if (damageEffect & ELEMENT_ICE) {
            damageKind = 3;
        }
        switch (damageKind) {
        case 2:
            switch (prevStep) {
            case 1:
            case 2:
                MARIA.velocityY = 0;
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 6U;
                MARIA.anim = D_pspeu_0927A8D8;
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_B);
                MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                break;
            case 3:
                MARIA.velocityY = 0;
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 7U;
                MARIA.anim = D_pspeu_0927A940;
                MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_A);
                break;
            case 5:
            case 4:
                MARIA.velocityY = FIX(-3);
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 1U;
                MARIA.anim = D_pspeu_0927A8D0;
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_C);
                break;
            }
            break;
        default:
        case 4:
        case 16:
        case 3:
            switch (prevStep) {
            default:
            case 1:
            case 2:
                MARIA.velocityY = FIX(-4);
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 1U;
                MARIA.anim = D_pspeu_0927A8D0;
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_D);
                break;
            case 3:
                MARIA.velocityY = 0;
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 7U;
                MARIA.anim = D_pspeu_0927A940;
                MarCreateEntFactoryFromEntity(g_CurrentEntity, 0, 0);
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_E);
                break;
            case 5:
            case 4:
                MARIA.velocityY = FIX(-3);
                func_pspeu_0924EA50_from_rbo5(FIX(-1.25));
                MARIA.step_s = 1U;
                MARIA.anim = D_pspeu_0927A8D0;
                g_api.PlaySfx(SFX_VO_RIC_PAIN_B);
                g_api.PlaySfx(SFX_BOSS_RIC_PAIN_E);
                break;
            }
            break;
        }
        g_Maria.damagePalette = PAL_FLAG(PAL_CC_RED_EFFECT_A);
        g_Maria.timers[PL_T_2] = 6;
        if (damageEffect & ELEMENT_FIRE) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x10008, 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 9, 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x430021, 0);
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_FIRE_EFFECT);
            g_Maria.timers[PL_T_2] = 0x10;
            return;
        }
        if (damageEffect & ELEMENT_ICE) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x2F, 0);
            g_Maria.timers[PL_T_2] = 0xC;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_BLUE_EFFECT_A);
            return;
        }
        if (damageEffect & ELEMENT_DARK) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x46, 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x560021, 0);
            g_Maria.timers[PL_T_2] = 0x10;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_DARK_EFFECT);
            return;
        }
        if (damageEffect & ELEMENT_HOLY) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x47, 0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x570021, 0);
            g_Maria.timers[PL_T_2] = 8;
            g_Maria.damagePalette = PAL_FLAG(PAL_CC_GREEN_EFFECT_A);
            return;
        }
        if (!(damageEffect & 0xF840)) {
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0x530021, 0);
            return;
        }
        break;
    case 1:
        if ((g_Maria.vram_flag & TOUCHING_CEILING) && (MARIA.velocityY < FIX(-1))) {
            MARIA.velocityY = FIX(-1);
        }
        if (func_pspeu_0925B680(
                CHECK_80 | CHECK_GRAVITY_HIT | CHECK_GROUND_AFTER_HIT)) {
            return;
        }
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        MarDecelerateX(FIX(0.125));
        if (!(g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_09259D48();
        }
        if (MARIA.poseTimer < 0) {
            if (g_Maria.unk5C) {
                if (g_Status.hp < 0x14) {
                    RicSetDeadPrologue();
                    return;
                }
                func_pspeu_09259AD8(MARIA.velocityX);
                return;
            }
            func_pspeu_09259AD8(MARIA.velocityX);
            return;
        }
        break;
    case 7:
        MarDecelerateX(FIX(0.125));
        if (!(g_Maria.vram_flag & TOUCHING_GROUND)) {
            func_pspeu_09259D48();
        }
        if (MARIA.poseTimer < 0) {
            if (g_Maria.unk5C) {
                if (g_Status.hp < 0x14) {
                    RicSetDeadPrologue();
                    return;
                }
                MarSetCrouch(0, MARIA.velocityX);
                return;
            }
            MarSetCrouch(0, MARIA.velocityX);
        }
        break;
    }
}


extern AnimationFrame D_pspeu_0927A778[];
extern RECT D_pspeu_0927A518;
extern s32 D_pspeu_0927C768;
extern s32 D_pspeu_0927C770;
extern s32 D_pspeu_0927C248;
extern s16 D_pspeu_0927C778;
extern s16 D_pspeu_0927C780;
extern u8 D_pspeu_0927C788[];
void func_pspeu_09258010(
    s32 damageEffects, s32 damageKind, s32 prevStep, s32 prevStepS) {
    s32 j;
    s32 i;
    u8* s2;
    u8* imgPtr;
    s32 disableColorChange = 0;
    PlayerDraw* playerDraw = &g_PlayerDraw[8];
    Entity* entity;

    switch (MARIA.step_s) {
    case 0:
        MarResetPose();
        func_pspeu_092577D8();
        func_pspeu_0924EA50_from_rbo5(FIX(-0.75));
        MarSetAnimation(D_pspeu_0927A778);
        MARIA.velocityY = FIX(-1.625);
        g_api.PlaySfx(SFX_BOSS_RIC_DEATH);
        D_pspeu_0927C768 = damageEffects;
        MARIA.step_s++;
        if (!g_CastleFlags[SHAFT_ORB_DEFEATED] && !g_DemoMode) {
            entity = &g_Entities[200];
            CreateEntityFromCurrentEntity(E_ID(ID_17), entity);
            entity->params = 2;
            entity->flags = FLAG_UNK_10000;
        }
        D_pspeu_0927C248 = 10;
        MARIA.palette = PAL_FLAG(0x220);
        return;
    case 1:
        if (D_pspeu_0927C768 & ELEMENT_FIRE) {
            func_pspeu_09252480_from_rbo5(0);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x48), 0);
            // RIC blueprint 53 has child 9, func_80161C2C
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_DEATH_BY_FIRE, 2), 0);
            D_pspeu_0927C770 = 1;
        } else if (D_pspeu_0927C768 & ELEMENT_THUNDER) {
            func_pspeu_09252480_from_rbo5(2);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4C), 0);
            // RIC blueprint 48 has child 41, EntityHitByLightning
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_THUNDER, 1), 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_THUNDER, 2), 0);
            D_pspeu_0927C770 = 2;
        } else if (D_pspeu_0927C768 & ELEMENT_ICE) {
            func_pspeu_09252480_from_rbo5(3);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4D), 0);
            // RIC blueprint 47 has child 40, EntityHitByIce
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 47, 0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_HIT_BY_ICE, 1), 0);
            D_pspeu_0927C770 = 3;
            MARIA.blendMode = BLEND_TRANSP | BLEND_ADD;
        } else {
            func_pspeu_09252480_from_rbo5(1);
            // RIC blueprint 33 has child 31, EntityPlayerBlinkWhite
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x4A), 0);
            // RIC blueprint 11 has child 5, RicEntityHitByCutBlood
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(BP_MULTIPLE_EMBERS, 5), 0);
            D_pspeu_0927C770 = 0;
        }
        playerDraw->r0 = playerDraw->g0 = playerDraw->b0 = playerDraw->r1 =
            playerDraw->g1 = playerDraw->b1 = playerDraw->r2 = playerDraw->g2 =
                playerDraw->b2 = playerDraw->r3 = playerDraw->g3 =
                    playerDraw->b3 = 0x80;
        playerDraw->enableColorBlend = 1;
        MARIA.step_s++;
        break;
    case 2:
        if (MARIA.pose != 4) {
            break;
        }
        MARIA.step_s++;
        break;
    case 3:
        // Unusual, FIX usually has denominator of 128
        MARIA.velocityY += FIX(11.0 / 256);
        if (MARIA.velocityY > FIX(1.0 / 16)) {
            MARIA.velocityY >>= 2;
            MARIA.velocityX >>= 3;
            StoreImage(&D_pspeu_0927A518, (u_long*)D_pspeu_0927C788);
            D_pspeu_0927C778 = 0;
            D_pspeu_0927C780 = 0x40;
            MARIA.step_s++;
        }
        break;
    case 4:
        if (g_Timer & 1) {
            break;
        }
        if (D_pspeu_0927C780 > 16) {
            // Another really weird velocityY value.
            // It's 0x233.
            MARIA.velocityY += FIX(0.0086);
        } else {
            MARIA.velocityX = 0;
            MARIA.velocityY = 0;
        }
        for (i = 0; i < 4; i++) {
            s2 = imgPtr = D_pspeu_0927C788;
            s2 += ((D_pspeu_0927C778 >> 1) & 7);
            s2 += ((D_pspeu_0927C778 & 0xFF) >> 4) << 6;
            for (j = 0; j < 0x28; j++) {
                if (D_pspeu_0927C778 & 1) {
                    *(s2 + ((j & 7) * 8) + ((j >> 3) * 0x400)) &= 0xF0;
                } else {
                    *(s2 + ((j & 7) * 8) + ((j >> 3) * 0x400)) &= 0x0F;
                }
            }
            D_pspeu_0927C778 += 0x23;
            D_pspeu_0927C778 &= 0xFF;
        }
        LoadImage(&D_pspeu_0927A518, (u_long*)imgPtr);
        if (--D_pspeu_0927C780 == 0) {
            MARIA.velocityY = 0;
            playerDraw->enableColorBlend = 0;
            MARIA.step_s = 0x80;
        }
        break;
    case 0x80:
        break;
    }
    // Always happens, kind of weird
    if (!disableColorChange) {
        if (D_pspeu_0927C770 == 0) {
            if (playerDraw->r0 < 0xF8) {
                playerDraw->r0 += 2;
            }
            if (playerDraw->b0 > 8) {
                playerDraw->b0 -= 2;
            }

            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->r0;
            playerDraw->g0 = playerDraw->g1 = playerDraw->b1 = playerDraw->g2 =
                playerDraw->b2 = playerDraw->g3 = playerDraw->b3 =
                    playerDraw->b0;
        }
        if (D_pspeu_0927C770 == 1 || D_pspeu_0927C770 == 2) {
            if (playerDraw->b0 > 8) {
                playerDraw->b0 -= 2;
            }
            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->r0 =
                playerDraw->g0 = playerDraw->g1 = playerDraw->b1 =
                    playerDraw->g2 = playerDraw->b2 = playerDraw->g3 =
                        playerDraw->b3 = playerDraw->b0;
        }
        if (D_pspeu_0927C770 == 3) {
            if ((playerDraw->r0 > 8) && (g_Timer & 1)) {
                playerDraw->r0 -= 1;
            }
            playerDraw->r3 = playerDraw->r2 = playerDraw->r1 = playerDraw->g3 =
                playerDraw->g2 = playerDraw->g1 = playerDraw->g0 =
                    playerDraw->r0;
        }
    }
}

// Local copy of RicStepStandInAir; the boss copy restores Maria's palette.
void func_pspeu_09258830(void) {
    if (MARIA.step_s == 0) {
        MARIA.velocityY += 0x3800;
        if (MARIA.velocityY > 0) {
            MARIA.velocityY = 0;
            MARIA.step_s = 1;
        }
    } else if (g_Maria.unk4E) {
        g_Maria.unk46 = 0;
        MarSetStep(PL_S_JUMP);
        MarSetAnimation(D_pspeu_0927A758);
        g_Maria.unk44 = 0;
        MARIA.palette = 0x8220;
    }
}

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
        if (MARIA.velocityX < FIX(3) || (g_Maria.vram_flag & TOUCHING_R_WALL)) {
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
        g_api.ShakeCamera(3);
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

extern AnimationFrame D_pspeu_0927A5B8[];
extern AnimationFrame D_pspeu_0927AA88[];
void func_pspeu_09259638(void) {
    Entity* entity;
    switch (MARIA.step_s) {
    case 0:
        MarSetAnimation(D_pspeu_0927A5B8);
        g_api.PlaySfx(SFX_BOSS_RIC_LAUGH);
        if (MARIA.posX.i.hi < 0x80) {
            MARIA.facingLeft = 0;
        } else {
            MARIA.facingLeft = 1;
        }
        MARIA.step_s++;
        // fallthrough

    case 1:
        D_pspeu_0927C248 = 0x14;
        entity = &g_Entities[200];
        CreateEntityFromCurrentEntity(E_ID(ID_17), entity);
        entity->params = 1;
        MARIA.step_s++;
        // fallthrough

    case 2:
        if (D_pspeu_0927C248 == 0x1E) {
            MarSetAnimation(D_pspeu_0927AA88);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(E_ID_24, 0x1), 0);
            MARIA.step_s++;
        }
        break;
    case 3:
        if (MARIA.animCurFrame == 0xB5) {
            if (MARIA.poseTimer == 1) {
                MarCreateEntFactoryFromEntity(
                    g_CurrentEntity, FACTORY(E_ID_23, 0), 0);
                g_api.PlaySfx(SFX_WEAPON_APPEAR);
            }
        }
        if (MARIA.poseTimer < 0) {
            D_pspeu_0927C248 = 0x28;
            func_pspeu_09259AD8(0);
            MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(E_ID_21, 0x45), 0);
            g_Maria.timers[ALU_T_POISON] = 0x800;
        }
        break;
    }
}


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
        g_Maria.damagePalette = PAL_FLAG(PAL_CC_RED_EFFECT_A);
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

s32 MarCheckSubwpnChainLimit(s16 subwpnId, s16 limit) {
    Entity* entity;
    s32 i;
    s32 nFound;
    s32 nEmpty;

    // Iterate through entities 96-128 (which hold subweapons)
    // Any that match the proposed ID increments the count.
    // If at any point the count reaches the limit, return -1.
    for (entity = &g_Entities[96], i = 0, nFound = 0, nEmpty = 0; i < 32; i++,
        entity++) {
        if (!entity->entityId) {
            nEmpty++;
        }

        if (entity->ext.subweapon.subweaponId &&
            entity->ext.subweapon.subweaponId == subwpnId) {
            nFound++;
        }

        if (nFound >= limit) {
            return -1;
        }
    }

    // This will indicate that there is an available entity slot
    // to hold the subweapon we're trying to spawn.
    // At the end, if this is zero, there are none available so return
    // -1 to indicate there is no room for the proposed subweapon.
    if (nEmpty) {
        return 0;
    }

    return -1;
}

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

extern AnimationFrame D_pspeu_0927A7C0[];
extern AnimationFrame D_pspeu_0927A7E0[];
extern AnimationFrame D_pspeu_0927A800[];
bool func_pspeu_0925A1F0(void) {
    s32 i;
    s16 poisoned;
    s16 sfxGrunt;

    sfxGrunt = rand() % 6;
    if (func_pspeu_0925A028() == 0) {
        if (sfxGrunt == 0) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
        }
        if (sfxGrunt == 1) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
        }
        if (sfxGrunt == 2) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_C);
        }
        if (sfxGrunt == 3) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_D);
        }
        return true;
    }
    if (g_Maria.timers[PL_T_POISON]) {
        poisoned = true;
    } else {
        poisoned = false;
    }
    for (i = 80; i < 95; i++) {
        DestroyEntity(&g_Entities[i]);
    }
    if (MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_WHIP, poisoned), 0)) {
        if (poisoned) {
            g_api.PlaySfx(SFX_RIC_FLAME_WHIP);
        } else {
            g_api.PlaySfx(SFX_BOSS_RIC_WHIP_ATTACK);
        }
        if (sfxGrunt == 0) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_A);
        }
        if (sfxGrunt == 1) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_B);
        }
        if (sfxGrunt == 2) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_C);
        }
        if (sfxGrunt == 3) {
            g_api.PlaySfx(SFX_BOSS_RIC_ATTACK_D);
        }
        switch (MARIA.step) {
        case PL_S_STAND:
        case PL_S_WALK:
            MARIA.step = PL_S_STAND;
            MarSetAnimation(D_pspeu_0927A7C0);
            g_CurrentEntity->velocityX = 0;
            break;
        case PL_S_RUN:
            MARIA.step = PL_S_STAND;
            MarSetAnimation(D_pspeu_0927A7C0);
            MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
            break;
        case PL_S_CROUCH:
            MarSetAnimation(D_pspeu_0927A7E0);
            g_CurrentEntity->velocityX = 0;
            break;
        case PL_S_FALL:
        case PL_S_JUMP:
            MARIA.step = PL_S_JUMP;
            MarSetAnimation(D_pspeu_0927A800);
            break;
        default:
            return false;
        }
        g_Maria.unk46 = 1;
        MARIA.step_s = 0x40;
        g_Maria.timers[PL_T_ATTACK] = 4;
        return true;
    }
    return false;
}

extern AnimationFrame D_pspeu_0927AA88[];
extern AnimationFrame D_pspeu_0927A840[];
extern AnimationFrame D_pspeu_0927A890[];
extern AnimationFrame D_pspeu_0927A9F0[];
extern AnimationFrame D_pspeu_0927AB00[];
bool func_pspeu_0925A588(void) {
    SubweaponDef subWpn;
    Entity* subWpnEnt;
    s16 subWpnID;

    subWpnID = func_pspeu_09248D20(&subWpn, true, false);
    if (subWpnID == PL_W_HOLYWATER && g_Maria.timers[PL_T_3]) {
        return 0;
    }
    if (subWpn.blueprintNum) {
        if (subWpnID == 1) {
            subWpnEnt = MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 1), 0);
        } else {
            subWpnEnt = MarCreateEntFactoryFromEntity(
                g_CurrentEntity, FACTORY(subWpn.blueprintNum, 0), 0);
        }
    }
    if (subWpnEnt == NULL) {
        return 0;
    }
    g_Maria.unk46 = 4;
    g_Maria.unk4E = 0;
    MARIA.velocityX = MARIA.velocityY = 0;
    switch (subWpnID) {
    case SUBWPN_NONE:
        MarSetStep(PL_S_FLAME_WHIP);
        MarSetAnimation(D_pspeu_0927AA88);
        MarCreateEntFactoryFromEntity(g_CurrentEntity, FACTORY(BP_36, 1), 0);
        break;
    case SUBWPN_DAGGER:
        MarSetStep(PL_S_THROW_DAGGERS);
        MarSetAnimation(D_pspeu_0927A840);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case SUBWPN_AXE:
        MarSetStep(PL_S_STAND_IN_AIR);
        MarSetAnimation(D_pspeu_0927A890);
        MARIA.velocityY = FIX(-4.6875);
        func_maria_8015CC28();
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    case SUBWPN_HOLYWATER:
        MarSetStep(PL_S_HYDROSTORM);
        MarSetAnimation(D_pspeu_0927A9F0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_HYDRO_STORM);
        break;
    case SUBWPN_REBNDSTONE:
    case SUBWPN_VIBHUTI:
    case SUBWPN_AGUNEA:
        MarSetStep(PL_S_SUBWPN_CRASH);
        MarSetAnimation(D_pspeu_0927A9F0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        break;
    case SUBWPN_BIBLE:
    case SUBWPN_STOPWATCH:
        MarSetStep(PL_S_SUBWPN_CRASH);
        MarSetAnimation(D_pspeu_0927AB00);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x40), 0);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 0x47), 0);
        g_api.PlaySfx(SFX_BOSS_RIC_ITEM_CRASH_ATTACK);
        break;
    case SUBWPN_CROSS:
        MarSetStep(PL_S_STAND_IN_AIR);
        MarSetAnimation(D_pspeu_0927A890);
        MARIA.velocityY = FIX(-4.6875);
        func_maria_8015CC28();
        g_api.PlaySfx(SFX_BOSS_RIC_HOLY_CROSS);
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_RIC_BLINK, 2), 0);
        break;
    }
    g_Maria.timers[PL_T_12] = 4;
    return 1;
}

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

extern AnimationFrame D_pspeu_0927A978[];
extern AnimationFrame D_pspeu_0927A970[];
bool func_pspeu_0925B680(s32 checks) {
    s32 velYChange;
    u32 effects;

    if (checks & CHECK_FACING) {
        if (g_Maria.unk46 == 0) {
            MarCheckFacing();
        }
    }
    velYChange = 0;
    if (checks & CHECK_GRAVITY_FALL) {
        velYChange = FIX(28.0 / 128);
    }
    if (checks & CHECK_GRAVITY_JUMP) {
        if (MARIA.velocityY < FIX(3.0 / 8) && MARIA.velocityY > FIX(-1.0 / 8) &&
            !(g_Maria.unk44 & 0x20) && (g_Maria.padPressed & PAD_CROSS)) {
            // Note that 5.6 is precisely 1/5 of 28.
            velYChange = FIX(5.6 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    if (checks & CHECK_GRAVITY_HIT) {
        if (MARIA.velocityY < FIX(3.0 / 8) && MARIA.velocityY > FIX(-1.0 / 8)) {
            velYChange = FIX(14.0 / 128);
        } else {
            velYChange = FIX(28.0 / 128);
        }
    }
    MARIA.velocityY += velYChange;
    if (MARIA.velocityY > FIX(7)) {
        MARIA.velocityY = FIX(7);
    }
    if ((checks & CHECK_80) && (g_Maria.vram_flag & TOUCHING_CEILING) &&
        (MARIA.velocityY < FIX(-1))) {
        MARIA.velocityY = FIX(-1);
    }
    if (MARIA.velocityY >= 0) {
        if ((checks & CHECK_GROUND) && (g_Maria.vram_flag & TOUCHING_GROUND)) {
            switch (g_Maria.unk46) {
            case 0:
            default:
                if (g_Maria.unk44 & 8) {
                    MarSetCrouch(3, MARIA.velocityX);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                    return true;
                }
                if (MARIA.velocityY > FIX(6.875)) {
                    MarSetCrouch(1, 0);
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                } else if (g_Maria.padPressed & (PAD_LEFT | PAD_RIGHT)) {
                    if (g_Maria.unk44 & 0x10) {
                        func_pspeu_09259B30();
                    } else {
                        func_pspeu_09259BB0(0);
                    }
                } else {
                    func_pspeu_09259AD8(0);
                }
                return true;
            case 1:
                if (MARIA.velocityY > FIX(6.875)) {
                    MARIA.step = PL_S_CROUCH;
                    MARIA.anim = D_pspeu_0927A7E0;
                    g_api.PlaySfx(SFX_STOMP_HARD_B);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                } else {
                    MARIA.step = PL_S_STAND;
                    MARIA.anim = D_pspeu_0927A7C0;
                    if (!(g_Maria.unk44 & 8)) {
                        MARIA.velocityX = 0;
                    } else {
                        MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                        g_api.PlaySfx(SFX_STOMP_HARD_B);
                    }
                }
                MARIA.velocityY = 0;
                g_Maria.unk44 = 0;
                return true;
            case 2:
                if (MARIA.velocityY > FIX(6.875)) {
                    MARIA.step = PL_S_CROUCH;
                    MARIA.anim = D_pspeu_0927A978;
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                } else {
                    MARIA.step = PL_S_STAND;
                    MARIA.anim = D_pspeu_0927A970;
                    if (!(g_Maria.unk44 & 8)) {
                        MARIA.velocityX = 0;
                    } else {
                        MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                        g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    }
                }
                MARIA.velocityY = 0;
                g_Maria.unk44 = 0;
                return true;
            case 3:
                if ((MARIA.velocityY > FIX(6.875)) || (g_Maria.unk44 & 8)) {
                    g_api.PlaySfx(SFX_STOMP_SOFT_A);
                    MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
                }
                MARIA.velocityY = 0;
                if (!(g_Maria.unk44 & 8)) {
                    MARIA.velocityX = 0;
                }
                MARIA.step = PL_S_STAND;
                MARIA.anim = D_pspeu_0927A820;
                g_Maria.unk44 = 0;
                return true;
            }
        } else if (checks & CHECK_GROUND_AFTER_HIT &&
                   (g_Maria.vram_flag & TOUCHING_GROUND)) {
            MarSetCrouch(1, MARIA.velocityX);
            g_api.PlaySfx(SFX_STOMP_SOFT_A);
            if (g_Maria.unk5C && (g_Status.hp < 2)) {
                RicSetDeadPrologue();
                return true;
            } else {
                MarCreateEntFactoryFromEntity(g_CurrentEntity, 0U, 0);
            }
            return true;
        }
    }
    if (checks & CHECK_FALL && !(g_Maria.vram_flag & TOUCHING_GROUND)) {
        if (g_Maria.unk46) {
            if (g_Maria.unk46 == 1) {
                MARIA.step_s = 0x40;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A800;
                return true;
            }
            if (g_Maria.unk46 == 2) {
                MARIA.step_s = 0x41;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A800;
                return true;
            }
            if (g_Maria.unk46 == 3) {
                MARIA.step_s = 0x42;
                MARIA.step = PL_S_JUMP;
                MARIA.anim = D_pspeu_0927A870;
                return true;
            }
        } else {
            func_pspeu_09259D48();
            return true;
        }
    }
    if (g_Maria.unk46) {
        return false;
    }
    if (checks & CHECK_CRASH && (g_Maria.padTapped & PAD_TRIANGLE) &&
        func_pspeu_0925A588()) {
        return true;
    }
    if (checks & CHECK_SLIDE &&
        !(MARIA.facingLeft == 0 &&
          g_Player.colFloor[2].effects & EFFECT_UNK_8000) &&
        !(MARIA.facingLeft && g_Player.colFloor[3].effects & EFFECT_UNK_8000) &&
        (MARIA.posX.i.hi < (u8)-4 || MARIA.facingLeft != 0) &&
        (MARIA.posX.i.hi >= 5 || !MARIA.facingLeft) &&
        g_Maria.padPressed & PAD_DOWN && g_Maria.padTapped & PAD_CROSS) {
        MarSetSlide();
        return true;
    }
    if (checks & CHECK_JUMP && (g_Maria.padTapped & PAD_CROSS)) {
        func_pspeu_09259C58();
        return true;
    }
    if (checks & CHECK_ATTACK && (g_Maria.padTapped & PAD_SQUARE) &&
        func_pspeu_0925A1F0()) {
        return true;
    }
    if (checks & CHECK_CROUCH && (g_Maria.padPressed & PAD_DOWN)) {
        MarSetCrouch(2, 0);
        return true;
    }
    return false;
}

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

void func_pspeu_092509D0_from_rbo5(Entity* self) {
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
    playerX = self->posX.i.hi;
    playerY = self->posY.i.hi;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 16);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->posY.i.hi -= 0x10;
        playerY = self->posY.i.hi;
        for (prim = (PrimLineG2*)&g_PrimBuf[self->primIndex], i = 0;
             prim != NULL; i++, prim = prim->next) {
            PGREY(prim, 0) = PGREY(prim, 1) = 0xFF;
            prim->priority = MARIA.zPriority + 2;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
            if (i != 0) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim->x0 = prim->x1 = playerX;
            prim->y0 = prim->y1 = playerY;
            prim->timer = 20;
        }
        self->flags =
            FLAG_POS_CAMERA_LOCKED | FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->zPriority = MARIA.zPriority + 2;

        if (MARIA.facingLeft) {
            self->ext.reboundStone.stoneAngle = 0x980;
        } else {
            self->ext.reboundStone.stoneAngle = 0xE80;
        }
        self->ext.reboundStone.stoneAngle += (rand() & 0x7F) - 0x40;

        self->ext.reboundStone.lifeTimer = 0x40;
        self->hitboxWidth = 4;
        self->hitboxHeight = 4;
        g_api.CheckCollision(self->posX.i.hi, self->posY.i.hi, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            self->ext.reboundStone.unk84 = 4;
        }
        self->step++;
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        break;
    case 1:
        deltaX = rcos(self->ext.reboundStone.stoneAngle) * 0x10;
        deltaY = -rsin(self->ext.reboundStone.stoneAngle) * 0x10;
        currX = self->posX.val;
        currY = self->posY.val;
        if (!self->ext.reboundStone.unk84) {
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
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_0800)) {
                            func_pspeu_092508C0_from_rbo5(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            func_pspeu_09250948_from_rbo5(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xA0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x760);
                        }
                    }
                    if (deltaY < 0) {
                        if ((colliderFlags == 0) ||
                            (colliderFlags & EFFECT_UNK_8000)) {
                            func_pspeu_092508C0_from_rbo5(0x800);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            func_pspeu_09250948_from_rbo5(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xF60);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x8A0);
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
                        if ((colliderFlags == 0) ||
                            TEST_BITS(colliderFlags, 0x4800) ||
                            TEST_BITS(colliderFlags, 0xC000)) {
                            func_pspeu_092508C0_from_rbo5(0x400);
                        }
                        if (colliderFlags == EFFECT_UNK_0800) {
                            func_pspeu_09250948_from_rbo5(0xE00);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0xED2);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xF60);
                        }
                        if (colliderFlags == EFFECT_UNK_8000) {
                            func_pspeu_09250948_from_rbo5(0x200);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x12E);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0xA0);
                        }
                    }
                    // Cases when traveling left
                    if (deltaX < 0) {
                        if ((colliderFlags == 0) ||
                            ((colliderFlags & 0x4800) == 0x800) ||
                            ((colliderFlags & 0xC000) == 0x8000)) {
                            func_pspeu_092508C0_from_rbo5(0x400);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_0800 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0xA00);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x92E);
                        }
                        if (colliderFlags == EFFECT_UNK_0800 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x8A0);
                        }
                        if (colliderFlags ==
                            EFFECT_UNK_8000 + EFFECT_UNK_4000) {
                            func_pspeu_09250948_from_rbo5(0x600);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_1000) {
                            func_pspeu_09250948_from_rbo5(0x6D2);
                        }
                        if (colliderFlags == EFFECT_UNK_8000 + EFFECT_UNK_4000 +
                                                 EFFECT_UNK_2000) {
                            func_pspeu_09250948_from_rbo5(0x760);
                        }
                    }
                }
                if (self->ext.reboundStone.unk82) {
                    goto block_93;
                }
                currX += deltaX;
                currY += deltaY;
            }
        } else {
            self->ext.reboundStone.unk84--;
        }
    block_93:
        if (self->ext.reboundStone.unk82) {
            // @bug: should call RicCreateEntFactoryFromEntity instead in case
            // E_FACTORY goes out of order between RIC and DRA
            g_api.CreateEntFactoryFromEntity(self, FACTORY(42, 2), 0);
            g_api.PlaySfx(SFX_UI_SUBWEAPON_TINK);
        }
        if (self->posX.i.hi < -0x40 || self->posX.i.hi > 0x140 ||
            self->posY.i.hi < -0x40 || self->posY.i.hi > 0x140 ||
            self->ext.reboundStone.unk80 == 15) {
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
                if (colliderFlags < prim->b1) {
                    prim->b1 -= colliderFlags;
                }
                PGREY(prim, 0) = PGREY(prim, 1);
            }
        }
    }
}

void func_pspeu_0925CC00(Entity* self) {
    Collider col;
    FakePrim* prim;
    s16 randomAngle;
    s16 x;
    s16 y;
    s16 temp; // used for multiple unrelated things
    s32 i;

    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, 13);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->hitboxWidth = self->hitboxHeight = 4;
        self->ext.subweapon.timer = 0x80;
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        self->posY.i.hi -= 15;
        if (MARIA.facingLeft) {
            self->posX.i.hi -= 13;
        } else {
            self->posX.i.hi += 13;
        }
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        i = 0;
        while (true) {
            prim->drawMode = DRAW_UNK02;
            prim->priority = MARIA.zPriority - 1;
            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                break;
            }
            prim->posX.i.hi = x;
            prim->posY.i.hi = y;
            prim->posX.i.lo = prim->posY.i.lo = 0;
            randomAngle = (rand() & 0xFF) + 0x100;
            temp = (rand() & 0xFF) + 0x80;
            prim->velocityX.val = ((rcos(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val += FIX(0.5);
            prim->velocityY.val = -((rsin(randomAngle) << 4) * temp >> 9);
            prim->velocityX.val = (prim->velocityX.val * 3) >> 1;
            if (self->facingLeft) {
                prim->velocityX.val = -prim->velocityX.val;
            }
            prim->posY.i.hi -= 4;
            prim->delay = 1;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim->r0 = 0xFF;
            prim->g0 = 0xFF;
            prim->b0 = 0xFF;
            prim->w = 2;
            prim->h = 2;
            i++;
            prim = prim->next;
        }
        g_api.PlaySfx(SFX_WEAPON_SWISH_C);
        self->step++;
        break;
    case 1:
        temp = 2;
        if (self->facingLeft) {
            temp = -temp;
        }
        if (--self->ext.subweapon.timer == 0) {
            DestroyEntity(self);
            return;
        }
        prim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (true) {
            if (prim->next == NULL) {
                prim->drawMode &= ~DRAW_HIDE;
                prim->y0 = prim->x0 = prim->w = 0;
                break;
            }
            prim->posX.i.hi = prim->x0;
            prim->posY.i.hi = prim->y0;
            if (prim->delay) {
                if (prim->velocityX.val != 0) {
                    prim->posX.val += prim->velocityX.val;
                    g_api.CheckCollision(
                        prim->posX.i.hi + temp, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SIDE) {
                        prim->velocityX.val = 0;
                    }
                }
                prim->posY.val += prim->velocityY.val;
                prim->velocityY.val += FIX(12.0 / 128);
                if (prim->velocityY.val > FIX(4)) {
                    prim->velocityY.val = FIX(4);
                }
                if (prim->velocityY.val > 0) {
                    g_api.CheckCollision(
                        prim->posX.i.hi, prim->posY.i.hi, &col, 0);
                    if (col.effects & EFFECT_SOLID) {
                        prim->delay = 0;
                        prim->posY.i.hi += col.unk18;
                        prim->posY.i.hi -= (i % 3 + 1);
                        prim->w = prim->h = 3;
                    }
                }
            }
            if ((self->ext.subweapon.timer & 7) == i) {
                self->posX.i.hi = prim->posX.i.hi;
                self->posY.i.hi = prim->posY.i.hi;
                if (prim->drawMode & DRAW_HIDE) {
                    self->hitboxWidth = self->hitboxHeight = 0;
                } else {
                    self->hitboxWidth = self->hitboxHeight = 4;
                }
                if (prim->delay) {
                    self->hitboxOffY = 0;
                } else {
                    self->hitboxOffY = -6;
                }
            }
            if (self->hitFlags && ((self->ext.subweapon.timer + 1) & 7) == i) {
                prim->drawMode = DRAW_HIDE;
            }
            if (self->ext.subweapon.timer - 1 == i) {
                prim->drawMode = DRAW_HIDE;
            }
            i++;
            prim->x0 = prim->posX.i.hi;
            prim->y0 = prim->posY.i.hi;
            prim = prim->next;
        }
        self->hitFlags = 0;
        break;
    }
}
