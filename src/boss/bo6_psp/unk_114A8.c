// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

// Maria fights alongside the player in this overlay; her state mirrors cen.h
#define MARIA g_Entities[STAGE_ENTITY_START]
#define MARIA_AFTERIMAGE_1 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1]
#define MARIA_AFTERIMAGE_2 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_2]
#define MARIA_AFTERIMAGE_3 g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_3]
#define MAR_DRAW_AFTER_IMAGE_SLOT 9

extern PlayerState g_Maria;

extern AnimationFrame D_pspeu_0927A5C0[]; // Maria's stop-run anim

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetAnimation(AnimationFrame* anim);
void MarDecelerateX(s32 speed);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
bool func_pspeu_0925B680(s32 checks);
void func_pspeu_09259AD8(s32 velocityX);
void func_pspeu_09259D48(void);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarGetFreeEntity);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarGetFreeEntityReverse);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09252480_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09248D20);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_8015FDB0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09252768_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253EA0_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", Unused09249778);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09252B48_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarCreateEntFactoryFromEntity);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityFactory);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntitySlideKick);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A2E0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A448);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A578);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924A660);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByCutBlood);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_maria_80161C2C);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924B8D0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_maria_80162E9C);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityPlayerBlinkWhite);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924D7F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityShrinkingPowerUpRing);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByIce);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarEntityHitByLightning);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0924FFB8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09250DA8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251100);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092516D0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251850);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092522A8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09251748_from_rbo5);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253AA8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253E08);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09253F48);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09254008);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092540C8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09247190_from_rbo5);

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

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarStepFall);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257418);

void MarResetPose(void) {
    MARIA.pose = MARIA.poseTimer = 0;
    g_Maria.unk44 = 0;
    g_Maria.unk46 = 0;
    MARIA.drawFlags &= ~ENTITY_ROTATE;
}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092577D8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09257820);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258010);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258830);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092588E8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092589B0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_092589F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258A30);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarStepSlide);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09258E60);

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

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259418);

void MarLoadGraphics_09259630(void) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259638);

void MarLoadGraphics_09259858(void) {}

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259860);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarSetCrouch);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259AD8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259B30);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259BB0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259C58);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259D48);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_09259EA8);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarCheckSubwpnChainLimit);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925A028);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925A1F0);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925A588);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", RicSetDeadPrologue);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarSetSlide);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925AA50);

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", MarSetBladeDash);

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

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/unk_114A8", func_pspeu_0925B4E0);

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
