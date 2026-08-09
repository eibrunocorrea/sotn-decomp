// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

// The Doppleganger boss reuses the shared player-anim block (dop_anim.h);
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
#define DOP_ANIM_NO_NULL_CHECK

extern PlayerState g_Maria;

#define BP_BLINK_WHITE 44

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
s32 func_pspeu_0924EA98(s32 arg0);
void func_pspeu_0924CA58(s32 arg0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092457C8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09245AD8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09246210);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09247190);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09248898);

void func_pspeu_09248B88(void) {
    if (func_pspeu_0924EA98(0x4301C) == 0) {
        MarSetSpeedX(FIX(1.5));
        if (MarCheckFacing() == 0) {
            func_pspeu_0924CA58(0);
        }
    }
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09248BE8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09248F50);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09248FA8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_80158B04);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249460);

s32 func_pspeu_092497C0(void) {
    s16 rnd = rand() & PSP_RANDMASK;
    MARIA.ext.player.anim = 0x2E + (rnd % 3);
    return rnd % 16;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249838);

void func_80159C04(void) {
    if (MARIA.posX.i.hi <= PLAYER.posX.i.hi) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249910);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09249FE8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924A948);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924AAF0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924AB20);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924B918);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924BB78);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924BBE8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924C200);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924C498);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924C550);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924C9C8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CA58);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CBF0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CD20);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CDE0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CE30);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CEB0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924CF50);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924D108);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924D4E8);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924D528);

#include "../dop_anim.h"

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarDisableAfterImage);

void func_pspeu_0924E770(void) {
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.resetFlag =
        0;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.disableFlag =
        0;
}

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924E788);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_0924EA98);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarCheckFloor);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarCheckCeiling);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09250260);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarCheckWallLeft);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarGetFreeEntity);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarGetFreeEntityReverse);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252480);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_8015FDB0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252768);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252B20);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252B48);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", MarCreateEntFactoryFromEntity);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252E78);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253500);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253620);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253EA0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_maria_80162E9C);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09254180);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09256258);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09256D30);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_092570B8);

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
