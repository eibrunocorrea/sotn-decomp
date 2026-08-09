// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo4/bo4.h"

// The Doppleganger boss reuses the shared player-anim block (dop_anim.h);
// splat's pre-split names (Mar*/func_pspeu_*) are kept until a rename pass
#define MARIA g_Entities[STAGE_ENTITY_START]
#define g_Dop g_Maria
#define SetDopplegangerStep MarSetStep
#define InitPlayerAfterImage InitMariaAfterImage
#define DrawPlayerAfterImage DrawMariaAfterImage
#define SetDopplegangerAnim func_pspeu_0924DCB8_from_rbo5
#define UpdateUnarmedAnim func_pspeu_0924DE50_from_rbo5
#define PlayAnimation func_pspeu_0924DFB0_from_rbo5
#define UpdateAnim func_pspeu_0924E260_from_rbo5
#define DOP_ANIM_NO_NULL_CHECK

extern PlayerState g_Maria;

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
s32 func_pspeu_0924EA98_from_rbo5(s32 arg0);
void func_pspeu_0924CA58_from_rbo5(s32 arg0);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_us_801C0B9C_from_no1);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarGetFreeEntity);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarGetFreeEntityReverse);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252480_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_8015FDB0);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252768_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252B20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252B48_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarCreateEntFactoryFromEntity);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09252E78_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09253500_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09253620_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09253EA0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_maria_80162E9C);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09254180_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09256258_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09256D30_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092570B8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09257298_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_maria_80161C2C);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarEntityHitByLightning);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09258A50_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09259480_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09259A20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0925BA80_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092457C8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09245AD8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09246210_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09254448);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09255070);

void func_pspeu_09248828_from_rbo5(void) {
    if (g_Maria.timers[1]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x17), 0);
    }
    if (g_Maria.timers[0]) {
        MarCreateEntFactoryFromEntity(
            g_CurrentEntity, FACTORY(BP_BLINK_WHITE, 0x16), 0);
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248898_from_rbo5);

void func_pspeu_09248B88_from_rbo5(void) {
    if (func_pspeu_0924EA98_from_rbo5(0x4301C) == 0) {
        MarSetSpeedX(FIX(1.5));
        if (MarCheckFacing() == 0) {
            func_pspeu_0924CA58_from_rbo5(0);
        }
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248BE8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248F50_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09248FA8_from_rbo5);

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
        g_api.func_80102CD8(3);
        g_api.PlaySfx(SFX_WALL_DEBRIS_B);
    }

    if (arg0 & 2) {
        MARIA.velocityX = 0;
        MARIA.velocityY = 0;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249460_from_rbo5);

s32 func_pspeu_092497C0_from_rbo5(void) {
    s16 rnd = rand() & PSP_RANDMASK;
    MARIA.ext.player.anim = 0x2E + (rnd % 3);
    return rnd % 16;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249838_from_rbo5);

void func_80159C04(void) {
    if (MARIA.posX.i.hi <= PLAYER.posX.i.hi) {
        MARIA.entityRoomIndex = 0;
    } else {
        MARIA.entityRoomIndex = 1;
    }
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249910_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09249FE8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924A948_from_rbo5);

void func_pspeu_0924AA18_from_rbo5(s16 arg0) {
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

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924AAF0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924AB20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924B918_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924BB78_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924BBE8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C200_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C498_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C550_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924C9C8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CA58_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CBF0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CD20_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CDE0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CE30_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924CF50_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924D108_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924D4E8_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924D528_from_rbo5);

#include "../dop_anim.h"

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarDisableAfterImage);

void func_pspeu_0924E770_from_rbo5(void) {
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.resetFlag =
        0;
    g_Entities[STAGE_ENTITY_START + E_AFTERIMAGE_1].ext.afterImage.disableFlag =
        0;
}

// reduced variant of func_us_801C5430 (see bo4/unk_45354.c): a0 is ignored
void func_pspeu_0924E788_from_rbo5(s16 a0, s16 minTime) {
    if (g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] <= minTime) {
        g_Dop.timers[ALU_T_INVINCIBLE_CONSUMABLES] = minTime;
    }
}

// local copy of DecelerateX (see decelerate.h)
void MarDecelerateX_0925B090(s32 speed) {
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
void MarDecelerateX_0925B130(s32 speed) {
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

void func_pspeu_0924EA50_from_rbo5(s32 velocityX) {
    if (MARIA.entityRoomIndex == 1) {
        velocityX = -velocityX;
    }
    MARIA.velocityX = velocityX;
}

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_0924EA98_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarCheckFloor);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarCheckCeiling);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09250260_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", MarCheckWallLeft);

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

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092509D0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_092514F0_from_rbo5);

INCLUDE_ASM("boss/bo4_psp/nonmatchings/bo4_psp/unk_107C8", func_pspeu_09251748_from_rbo5);
