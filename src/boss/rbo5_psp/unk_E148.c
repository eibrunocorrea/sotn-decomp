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

// collision sensor tables (.rodata block shared with rbo5, different VMAs)
extern Point16 D_pspeu_092630B8[];
extern Point16 D_pspeu_092630C8[];
extern Point16 D_pspeu_092630D8[];
#define g_MarSensorsCeiling D_pspeu_092630B8
#define g_MarSensorsFloor D_pspeu_092630C8
#define g_MarSensorsWall D_pspeu_092630D8

#define BP_BLINK_WHITE 44
#define BP_4 4
// entity id from the doppleganger.h enum, not pulled in on PSP
#define E_FACTORY 1

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
s32 func_pspeu_0924EA98(s32 arg0);
void func_pspeu_0924C9C8(s32 arg0, s32 arg1);
void func_pspeu_0924CA58(s32 arg0);
void func_pspeu_0924CBF0(s32 arg0);
void func_pspeu_0924CD20(void);
void func_pspeu_0924CDE0(void);
void func_pspeu_0924CE30(void);
s32 func_pspeu_0924D108(void);
void func_pspeu_0924D4E8(void);
void func_pspeu_0924D528(void);

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
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8) && !(g_Maria.unk44 & 0x20) &&
            (g_Maria.padPressed & PAD_CROSS)) {
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
        if (MARIA.velocityY < FIX(3.0 / 8) &&
            MARIA.velocityY > FIX(-1.0 / 8)) {
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
        } else if ((checks & 0x20000) &&
                   (g_Maria.vram_flag & TOUCHING_GROUND)) {
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
    if ((checks & 0x1000) &&
        (g_Maria.padTapped & (PAD_SQUARE | PAD_CIRCLE))) {
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
        !(g_Maria.status & 0x40083)) {
        yMod = 4;
    } else {
        i = 1;
    }
    for (; i < NUM_HORIZONTAL_SENSORS; i++, yMod = 0) {
        if (g_Maria.colFloor[i].effects & EFFECT_SOLID_FROM_BELOW) {
            continue;
        }
        if (!(g_Maria.colFloor[i].effects & EFFECT_UNK_0002 ||
              MARIA.velocityY >= 0 || g_Maria.status & 3 ||
              abs(MARIA.velocityX) >= FIX(2))) {
            continue;
        }
        effects2 = g_Maria.colFloor[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_0800)) {
            if (g_Maria.status & 3 && !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsFloor[i].x;
            yCheck = *y + g_MarSensorsFloor[i].y;
            yCheck += g_Maria.colFloor[i].unk18 - 1;
            g_api.CheckCollision(xCheck, yCheck, &col, 0);
            if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
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
            if ((effects &
                 (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ==
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
        if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
            col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
        }
        effects2 = g_Maria.colCeiling[i].effects &
                   (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_SOLID);
        if ((effects2 == EFFECT_SOLID) || (effects2 & EFFECT_UNK_8000)) {
            if (g_Maria.status & 3 && !(effects2 & EFFECT_SOLID)) {
                continue;
            }
            xCheck = *x + g_MarSensorsCeiling[i].x;
            yCheck = *y + g_MarSensorsCeiling[i].y +
                     g_Maria.colCeiling[i].unk10 + 1;
            g_api.CheckCollision((s16)xCheck, (s16)yCheck, &col, 0);
            if (g_Maria.status & 2 && col.effects & EFFECT_MIST_ONLY) {
                col.effects &= ~(EFFECT_UNK_0002 | EFFECT_SOLID);
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
            if ((effects &
                 (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ==
                (EFFECT_UNK_0800 | EFFECT_SOLID)) {
                if (i < 2) {
                    *vramFlag |=
                        (EFFECT_UNK_0800 | EFFECT_UNK_0002 |
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
                (EFFECT_UNK_0800 | EFFECT_UNK_0002 |
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
                    (EFFECT_UNK_0800 | EFFECT_UNK_0002);
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
                         EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID);
    if ((effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                     EFFECT_SOLID))) {
        *vramFlag |= 4;
        return;
    }

    for (i = 0; i < NUM_VERTICAL_SENSORS; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_UNK_0002 | EFFECT_SOLID);
        if (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                        EFFECT_SOLID) ||
            effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID) ||
            effects == (EFFECT_UNK_0002 | EFFECT_SOLID)) {
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
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
            *vramFlag |= 4;
            *x += g_Maria.colWall[i].unk4;
            return;
        }
        if ((effects & (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800)) ==
                EFFECT_UNK_0800 &&
            (i != 6) &&
            ((g_Maria.colWall[6].effects & EFFECT_UNK_8000) ||
             !(g_Maria.colWall[6].effects &
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
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
                         EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID);
    if ((effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                     EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0400 | EFFECT_UNK_0002 |
                     EFFECT_SOLID)) ||
        (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                     EFFECT_UNK_0400 | EFFECT_UNK_0002 | EFFECT_SOLID))) {
        *vramFlag |= 8;
        return;
    }
    for (i = NUM_VERTICAL_SENSORS; i < NUM_VERTICAL_SENSORS * 2; i++) {
        effects = g_Maria.colWall[i].effects &
                  (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0800 |
                   EFFECT_UNK_0002 | EFFECT_SOLID);
        if ((effects == (EFFECT_UNK_8000 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0800 | EFFECT_UNK_0002 | EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_8000 | EFFECT_UNK_4000 | EFFECT_UNK_0002 |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_4000 | EFFECT_UNK_0800 | EFFECT_UNK_0002 |
                         EFFECT_SOLID)) ||
            (effects == (EFFECT_UNK_0002 | EFFECT_SOLID))) {
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
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
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
               (EFFECT_UNK_8000 | EFFECT_UNK_0800 | EFFECT_UNK_0002)))) {
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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252480);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_8015FDB0);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252768);

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

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09252E78);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253500);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253620);

INCLUDE_ASM("boss/rbo5_psp/nonmatchings/rbo5_psp/unk_E148", func_pspeu_09253EA0);

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
