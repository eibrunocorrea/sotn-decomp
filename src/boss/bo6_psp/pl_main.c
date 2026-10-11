// SPDX-License-Identifier: AGPL-3.0-or-later
#include "player_copy.h"

extern u32 D_pspeu_0927C388;
extern s32 D_pspeu_0927A488[];
extern u16 D_pspeu_0927A4C8[];
extern u16 D_pspeu_0927A4E8[];
extern AnimationFrame* D_pspeu_0927A5B0[];
extern FrameProperty D_pspeu_0927AB38[];

void func_pspeu_092540C8(void) {
    s16 angle;
    s32 i;
    s32 status;
    s16 step;
    s16 step_s;
    PlayerDraw* playerDraw;
    DamageParam damage;
    s32 posX, posY;
    status = 0;
    g_CurrentEntity = &MARIA;
    playerDraw = &g_PlayerDraw[8];
    g_Maria.unk4C = 0;
    for (i = 0; i < LEN(g_Maria.timers); i++) {
        if (g_Maria.timers[i]) {
            switch (i) {
            case PL_T_POISON:
                g_Maria.timers[PL_T_POISON] = 0x800;
                break;
            case PL_T_CURSE:
                break;
            case PL_T_2:
                MARIA.palette = g_Maria.damagePalette;
                break;
            case PL_T_3:
                break;
            case PL_T_4:
                angle = ((g_GameTimer & 0xF) << 8);
                playerDraw->r0 = playerDraw->g0 = playerDraw->b0 =
                    (rsin(angle) + FLT(1)) / 64 + 0x60;

                angle += FLT(1.0 / 8.0);
                playerDraw->r1 = playerDraw->g1 = playerDraw->b1 =
                    (rsin(angle) + FLT(1)) / 64 + 0x60;

                angle += FLT(1.0 / 8.0);
                playerDraw->r3 = playerDraw->g3 = playerDraw->b3 =
                    (rsin(angle) + FLT(1)) / 64 + 0x60;

                angle += FLT(1.0 / 8.0);
                playerDraw->r2 = playerDraw->g2 = playerDraw->b2 =
                    (rsin(angle) + FLT(1)) / 64 + 0x60;

                playerDraw->enableColorBlend = true;
                break;
            case PL_T_5:
            case PL_T_6:
            case PL_T_7:
            case PL_T_8:
            case PL_T_ATTACK:
            case PL_T_10:
            case PL_T_RUN:
            case PL_T_12:
                break;
            case PL_T_INVINCIBLE_SCENE:
                g_Maria.timers[PL_T_INVINCIBLE_SCENE] = 4;
                break;
            case PL_T_INVINCIBLE:
                break;
            case PL_T_AFTERIMAGE_DISABLE:
                MarDisableAfterImage(0, 0);
                break;
            }

            if (--g_Maria.timers[i] == 0) {
                switch (i) {
                case PL_T_POISON:
                case PL_T_CURSE:
                    break;
                case PL_T_2:
                    MARIA.palette = PAL_FLAG(0x220);
                    break;
                case PL_T_3:
                    break;
                case PL_T_4:
                    playerDraw->enableColorBlend = false;
                    break;
                case PL_T_INVINCIBLE_SCENE:
                    func_pspeu_0925B4E0(1, 0x10);
                    break;
                case PL_T_6:
                    if ((MARIA.step == 4) && (MARIA.anim != D_pspeu_0927A768)) {
                        MarSetAnimation(D_pspeu_0927A768);
                        g_Maria.unk44 &= ~0x10;
                    }
                    break;
                case PL_T_INVINCIBLE:
                    break;
                case PL_T_AFTERIMAGE_DISABLE:
                    func_maria_8015CC28();
                    break;
                case PL_T_5:
                case PL_T_7:
                case PL_T_8:
                case PL_T_ATTACK:
                case PL_T_10:
                case PL_T_RUN:
                case PL_T_12:
                default:
                    break;
                }
            }
        }
    }

    g_Maria.padHeld = g_Maria.padPressed;
    if (g_Maria.demo_timer) {
        g_Maria.demo_timer--;
        g_Maria.padPressed = g_Maria.padSim;
    } else {
        g_Maria.padPressed = g_pads[1].pressed;
    }
    g_Maria.padTapped =
        (g_Maria.padHeld ^ g_Maria.padPressed) & g_Maria.padPressed;

    if ((MARIA.step != PL_S_ENDING_4) && (MARIA.step != PL_S_DEAD)) {
        if (MARIA.step != PL_S_DEAD && MARIA.flags & FLAG_DEAD) {
            D_pspeu_0927C388 |= 2;
            step = MARIA.step;
            step_s = MARIA.step_s;
            damage.effects =
                D_pspeu_0927A488[g_CurrentEntity->nFramesInvincibility];
            MarSetStep(0x11);
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_SAVE_RICHTER, TIMEATTACK_SET_RECORD);
        } else if (D_pspeu_0927C1C8 != 0) {
            MarSetStep(0x70);
        } else {
            if ((g_Maria.timers[PL_T_INVINCIBLE_SCENE] |
                 g_Maria.timers[PL_T_INVINCIBLE]) == 0) {
                if ((g_CurrentEntity->hitFlags > 0) &&
                    (g_CurrentEntity->hitFlags < 4)) {
                    step = MARIA.step;
                    step_s = MARIA.step_s;
                    damage.effects =
                        D_pspeu_0927A488[g_CurrentEntity->nFramesInvincibility];
                    if ((g_Maria.unk6C - g_Maria.unk6A) >= 10) {
                        damage.damageKind = 3;
                    } else {
                        damage.damageKind = 2;
                    }
                    MarSetStep(0xB);
                }
            }
            goto check_input_combo;
        }
    } else {
    check_input_combo:
        func_pspeu_09253F48();
        func_pspeu_09254008();
    }
    g_Maria.prev_step = MARIA.step;
    g_Maria.prev_step_s = MARIA.step_s;

    switch (MARIA.step) {
    case PL_S_STAND:
        func_pspeu_092568D0();
        break;
    case PL_S_WALK:
        func_pspeu_09256BA0();
        break;
    case PL_S_CROUCH:
        func_pspeu_09257418();
        break;
    case PL_S_FALL:
        MarStepFall();
        break;
    case PL_S_JUMP:
        func_pspeu_09256D28();
        break;
    case PL_S_HIGHJUMP:
        func_pspeu_09259418();
        break;
    case PL_S_HIT:
        func_pspeu_09257820(damage.effects, damage.damageKind, step, step_s);
        break;
    case PL_S_DEAD:
        func_pspeu_09258010(damage.effects, damage.damageKind, step, step_s);
        break;
    case PL_S_STAND_IN_AIR:
        func_pspeu_09258830();
        break;
    case PL_S_FLAME_WHIP:
        func_pspeu_092588E8();
        break;
    case PL_S_HYDROSTORM:
        func_pspeu_092589B0();
        break;
    case PL_S_THROW_DAGGERS:
        func_pspeu_09258A30();
        break;
    case PL_S_SUBWPN_CRASH:
        func_pspeu_092589F0();
        break;
    case PL_S_SLIDE:
        MarStepSlide();
        break;
    case PL_S_RUN:
        func_pspeu_09256C28();
        break;
    case PL_S_SLIDE_KICK:
        func_pspeu_09258E60();
        break;
    case PL_S_BLADEDASH:
        MarStepBladeDash();
        break;
    case PL_S_ENDING_1:
        MarLoadGraphics_09259630();
        break;
    case PL_S_ENDING2:
        func_pspeu_09259638();
        break;
    case PL_S_ENDING_3:
        MarLoadGraphics_09259858();
        break;
    case PL_S_ENDING_4:
        func_pspeu_09259860();
        break;
    }

    g_Maria.unk08 = g_Maria.status;

    switch (MARIA.step) {
    case PL_S_STAND:
        status = 0x08000000;
        break;
    case PL_S_WALK:
        status = 0x08000000;
        break;
    case PL_S_CROUCH:
        status = 0x08000000;
        if (MARIA.step_s != 2) {
            status = 0x08000020;
        }
        break;
    case PL_S_FALL:
        status = 0x08002000;
        break;
    case PL_S_JUMP:
        status = 0x08002000;
        break;
    case PL_S_HIGHJUMP:
        func_pspeu_0925B4E0(1, 4);
        break;
    case PL_S_HIT:
        status = 0x08010000;
        func_pspeu_0925B4E0(1, 8);
        break;
    case PL_S_DEAD:
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_SAVE_RICHTER, TIMEATTACK_SET_RECORD);
        status = 0x08050000;
        if (MARIA.step_s == 0x80) {
            status |= 0x80000;
        }
        func_pspeu_0925B4E0(1, 8);
        break;

    case PL_S_FLAME_WHIP:
    case PL_S_HYDROSTORM:
    case PL_S_THROW_DAGGERS:
    case PL_S_SUBWPN_CRASH:
        status = 0x08000000;
        // fallthrough
    case PL_S_STAND_IN_AIR:
        func_pspeu_0925B4E0(1, 8);
        break;
    case PL_S_SLIDE:
    case PL_S_SLIDE_KICK:
        status = 0x20;
        break;
    case PL_S_RUN:
    case PL_S_BLADEDASH:
        break;
    case PL_S_ENDING_1:
    case PL_S_ENDING2:
        func_pspeu_0925B4E0(1, 8);
        status |= 0x08000000;
        break;
    case PL_S_ENDING_3:
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_SAVE_RICHTER, TIMEATTACK_SET_RECORD);
        func_pspeu_0925B4E0(1, 8);
        status |= 0x08000000;
        break;
    case PL_S_ENDING_4:
        g_api.TimeAttackController(
            TIMEATTACK_EVENT_SAVE_RICHTER, TIMEATTACK_SET_RECORD);
        func_pspeu_0925B4E0(1, 8);
        status |= 0x08000000;
        break;
    }

    if (g_Maria.timers[PL_T_ATTACK]) {
        status |= 0x400;
    }
    if (g_Maria.timers[PL_T_10]) {
        status |= 0x800;
    }
    if (g_Maria.timers[PL_T_12]) {
        status |= 0x1000;
    }
    status |= 0x10000000;
    g_Maria.status = status;
    if (g_Maria.unk08 & 0x10000) {
        if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
            if (g_Maria.unk5C) {
                if (g_Status.hp < 2) {
                    RicSetDeadPrologue();
                    func_pspeu_0925B4E0(1, 8);
                }
            } else {
                func_pspeu_0925B4E0(1, 8);
                g_Maria.timers[PL_T_4] = 8;
                MARIA.palette = PAL_FLAG(0x220);
            }
        }
    }
    if (status & 0x08000000) {
        MarDisableAfterImage(1, 4);
    }
    g_api.UpdateAnim(D_pspeu_0927AB38, D_pspeu_0927A5B0);
    if (MARIA.anim == &D_pspeu_0927A900[0]) {
        MARIA.palette = D_pspeu_0927A4C8[MARIA.pose];
    }
    if ((MARIA.anim == &D_pspeu_0927A890[0]) && (MARIA.pose == 4)) {
        MARIA.palette = D_pspeu_0927A4E8[MARIA.poseTimer & 3];
    }
    if (g_Maria.status & PLAYER_STATUS_DEAD) {
        if (MARIA.poseTimer < 0) {
            MARIA.animCurFrame |= 0x8000;
        }
        MARIA.hitboxState = 0;
    } else {
        MARIA.hitboxState = g_Maria.unk70;
        if ((g_Maria.timers[PL_T_INVINCIBLE_SCENE] |
             g_Maria.timers[PL_T_INVINCIBLE]) != 0) {
            MARIA.hitboxState = 0;
        }
    }
    InitMariaAfterImage();
    posX = MARIA.posX.val;
    posY = MARIA.posY.val;
    func_pspeu_09253E08();
    DrawMariaAfterImage();
    FntPrint("pl_color:%04x\n", MARIA.palette);
}
