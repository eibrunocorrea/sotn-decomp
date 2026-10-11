// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"
#include <cutscene.h>

extern Dialogue g_Dialogue;

#include "../../st/set_cutscene_events.h"

#define CUTSCENE_TILEMAP_SCROLL
#include "../../st/cutscene_events.h"

#include "../../st/cutscene_skip.h"

extern s32 D_pspeu_0927BB40;
extern u8 cutsceneUnk3Unknown[0x380];
extern u8* D_pspeu_0927BAD0;
extern u8* D_pspeu_0927BAC8;
extern u8* D_pspeu_0927BAC0;
extern u8* D_pspeu_0927BAB8;
extern u8* D_pspeu_0927BAB0;
extern u8* D_pspeu_0927BAA8;
extern u8* D_pspeu_0927BAA0;
extern u8* D_pspeu_0927BA98;
extern u8* D_pspeu_0927BA90;
extern u8* D_pspeu_0927BD50;
extern u8 D_pspeu_09279658[];
extern u8 D_pspeu_0927BB48[];
extern u16 D_pspeu_09279660[];
extern u16 D_pspeu_09279668[];
extern u16 D_pspeu_09279670[];
extern s16 D_pspeu_09279678[72];
extern u8 D_pspeu_09279708[];
extern u8 D_pspeu_09279710[];
extern u16 D_pspeu_0927BB50[];
extern u8 D_pspeu_0927C0D8[32];
extern u8 D_pspeu_0927C0F8;
extern u32 D_pspeu_09272048;
extern u32 D_893DD0C;
extern u32 D_893EA8C;
extern u32 D_893F80C;
extern u32 D_894058C;
extern u32 D_894130C;
extern u32 D_894208C;
extern u32 D_pspeu_09272DC8;
extern u32 D_8943B8C;
extern u32 D_894490C;
extern u32 D_pspeu_09273B48;

u8 SetCutsceneScript(u8* script);
void CutsceneUnk1(void);
void CutsceneUnk4(void);
void DrawCutsceneActorName(u16 actorIndex, Entity* self, u16* actorNames,
                           u8* arg3, u8* arg4, s32 nActors);

enum DialogueSteps {
    DIALOGUE_INIT,
    DIALOGUE_RUN,
    DIALOGUE_LOAD_PORTRAIT,
    DIALOGUE_START_TEXT,
    DIALOGUE_UNLOAD_PORTRAIT,
    DIALOGUE_OPEN_DIALOG_BOX,
    DIALOGUE_CLOSE_DIALOG_BOX,
    DIALOGUE_END,
};

enum DialogueSubsteps {
    DIALOG_BOX_INIT,
    DIALOG_BOX_DRAW_RED,
    DIALOG_BOX_DRAW_BLUE,
};

void EntityCutsceneDialogue(Entity* self) {
    u16 nextChar;
    s32 primIndex;
    bool endLoop;
    s16 uCoord, vCoord;
    Primitive* prim;
    s32 i, j;
    s32 charWidth;
    u8* charBuffer1;
    u8* charBuffer2;
    u32 tempChar;
    RECT rect;
    u_long ptr;

    DisableAutoPowerOff();

    if (self->step && D_pspeu_0927BB40 && !g_SkipCutscene) {
        if (g_GameClearFlag) {
            CutsceneSkip(self);
        } else {
            switch (self->params) {
            case 0:
                if (g_Settings.D_8003CB04 & 0x8000) {
                    CutsceneSkip(self);
                }
                break;
            case 2:
                if (g_Settings.D_8003CB04 & 0x10000) {
                    CutsceneSkip(self);
                }
                break;
            case 3:
                if (g_Settings.D_8003CB04 & 0x20000) {
                    CutsceneSkip(self);
                }
                break;
            }
        }
    }
    if (self->step && g_Dialogue.hasEvents) {
        RunCutsceneEvents();
    }

    switch (self->step) {
    case DIALOGUE_INIT:
        switch (self->params) {
        case 0:
            if (g_DemoMode || g_CastleFlags[SHAFT_ORB_DEFEATED] ||
                g_PlayableCharacter) {
                DestroyEntity(self);
                return;
            }
            i = SetCutsceneScript(D_pspeu_0927BAD0);
            D_pspeu_0927BD50 = D_pspeu_0927BAC8;
            *D_pspeu_0927BAA0 = 0;
            if (g_CastleFlags[MET_MARIA_IN_CEN]) {
                *D_pspeu_0927BAA0 = 1;
            }
            *D_pspeu_0927BA90 = *D_pspeu_0927BA98 = 0;
            if (g_api.CheckEquipmentItemCount(ITEM_HOLY_GLASSES, EQUIP_HEAD)) {
                *D_pspeu_0927BA90 = *D_pspeu_0927BA98 = 1;
            }
            break;
        case 1:
            i = SetCutsceneScript(D_pspeu_0927BAC0);
            D_pspeu_0927BD50 = D_pspeu_0927BAB8;
            break;
        case 2:
        case 3:
            if (g_DemoMode || g_CastleFlags[SHAFT_ORB_DEFEATED]) {
                DestroyEntity(self);
                return;
            }
            i = SetCutsceneScript(D_pspeu_0927BAB0);
            D_pspeu_0927BD50 = D_pspeu_0927BAA8;
            *D_pspeu_0927BA90 = *D_pspeu_0927BA98 = self->params - 2;
            break;
        }
        if (i) {
            self->flags |= FLAG_HAS_PRIMS | FLAG_UNK_2000;
            self->primIndex = g_Dialogue.primIndex[2];
            g_CutsceneHasControl = true;
            g_CutsceneFlags = 0;
            D_pspeu_0927BB40 = false;
            g_SkipCutscene = false;
            self->step++;
            self->flags |= FLAG_UNK_10000;
        }

    case DIALOGUE_RUN:
        nextChar = 0;

        while (true) {
            nextChar = *g_Dialogue.scriptCur++;
            endLoop = false;
            if (PadReadPSP() & PAD_START) {
                g_Dialogue.nextCharTimer = 0;
            }
            if ((g_Dialogue.nextCharTimer) && !g_SkipCutscene) {
                g_Dialogue.nextCharTimer--;
                g_Dialogue.scriptCur--;
                return;
            }
            if (!(nextChar & 0x80)) {
                switch (nextChar) {
                case CSOP_END_CUTSCENE:
                    self->step = DIALOGUE_END;
                    return;
                case CSOP_LINE_BREAK:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    g_Dialogue.nextCharX = g_Dialogue.nextLineX;
                    if (!(g_Dialogue.unk12 & 1)) {
                        g_Dialogue.nextLineY += CS_LINE_SPACING;
                    }

                    g_Dialogue.nextCharY++;
                    g_Dialogue.nextCharY &= CS_LINE_MAX;
                    CutsceneUnk4();
                    if (!(g_Dialogue.unk12 & 1)) {
                        if (g_Dialogue.nextCharY > CS_LINE_MAX - 1) {
                            g_Dialogue.unk12 |= 1;
                            g_Dialogue.portraitAnimTimer = 0;
                            self->step_s = DIALOG_BOX_INIT;
                            self->step++;
                        }
                    } else {
                        g_Dialogue.portraitAnimTimer = 0;
                        self->step_s = DIALOG_BOX_INIT;
                        self->step++;
                    }
                    return;
                case CSOP_SET_SPEED:
                    g_Dialogue.scriptCur++;
                    continue;
                case CSOP_SET_WAIT:
                    g_Dialogue.nextCharTimer = *g_Dialogue.scriptCur++;
                    if (g_SkipCutscene) {
                        continue;
                    }
                    return;
                case CSOP_HIDE_DIALOG:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    prim = g_Dialogue.prim[0];
                    for (i = 0; i < LEN(g_Dialogue.prim) - 1; i++) {
                        prim->drawMode = DRAW_HIDE;
                        prim = prim->next;
                    }
                    return;
                case CSOP_SET_PORTRAIT:
                    if (g_SkipCutscene) {
                        g_Dialogue.scriptCur += 2;
                        continue;
                    }
                    i = *g_Dialogue.scriptCur++;
                    prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                    j = *g_Dialogue.scriptCur++;
                    charWidth = j & 1;
                    uCoord = D_pspeu_09279658[charWidth];
                    vCoord = D_pspeu_0927BB48[charWidth];
                    prim->clut = D_pspeu_09279660[i];
                    prim->tpage = 0x94;
                    prim->u0 = prim->u2 = uCoord;
                    prim->u1 = prim->u3 = uCoord + 47;
                    prim->v0 = prim->v1 = vCoord;
                    prim->v2 = prim->v3 = vCoord + 72;
                    prim->x0 = prim->x1 = prim->x2 = prim->x3 =
                        g_Dialogue.startX - 30;
                    prim->y0 = prim->y1 = prim->y2 = prim->y3 =
                        g_Dialogue.startY + 36;
                    g_Dialogue.clutIndex = D_pspeu_09279670[i];
                    CutsceneUnk1();
                    CutsceneUnk4();
                    prim->priority = 510;
                    prim->drawMode = DRAW_DEFAULT;
                    DrawCutsceneActorName(
                        i, self, D_pspeu_0927BB50, D_pspeu_09279708,
                        D_pspeu_09279710, 4);
                    g_Dialogue.portraitAnimTimer = 6;
                    self->step = DIALOGUE_START_TEXT;
                    return;
                case CSOP_NEXT_DIALOG:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    prim = g_Dialogue.prim[0];
                    for (i = 0; i < LEN(g_Dialogue.prim) - 1; i++) {
                        prim->drawMode = DRAW_HIDE;
                        prim = prim->next;
                    }
                    g_api.FreePrimitives(g_Dialogue.primIndex[1]);
                    g_Dialogue.primIndex[1] = -1;
                    g_Dialogue.portraitAnimTimer = 6;
                    self->step = DIALOGUE_UNLOAD_PORTRAIT;
                    return;
                case CSOP_SET_POS:
                    if (g_SkipCutscene) {
                        g_Dialogue.scriptCur += 2;
                        continue;
                    }
                    g_Dialogue.startX = *g_Dialogue.scriptCur++;
                    g_Dialogue.startY = *g_Dialogue.scriptCur++;
                    prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                    prim = prim->next;
                    prim->y0 = prim->y1 = g_Dialogue.startY;
                    prim->y2 = prim->y3 = g_Dialogue.startY + 72;
                    prim = prim->next;
                    prim->y0 = g_Dialogue.startY - 1;
                    prim->u0 = 246;
                    g_Dialogue.portraitAnimTimer = 24;
                    self->step = DIALOGUE_OPEN_DIALOG_BOX;
                    self->step_s = DIALOG_BOX_INIT;
                    return;
                case CSOP_CLOSE_DIALOG:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    g_Dialogue.portraitAnimTimer = 24;
                    self->step = DIALOGUE_CLOSE_DIALOG_BOX;
                    return;
                case CSOP_PLAY_SOUND:
                    if (g_SkipCutscene) {

                        g_Dialogue.scriptCur += 2;
                        continue;
                    }
                    nextChar = *g_Dialogue.scriptCur++;
                    nextChar <<= 4;
                    nextChar |= *g_Dialogue.scriptCur++;
                    g_api.PlaySfx(nextChar);
                    continue;
                case CSOP_WAIT_FOR_SOUND:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    if (g_api.func_80131F68()) {
                        continue;
                    }
                    g_Dialogue.scriptCur--;
                    return;
                case CSOP_SCRIPT_UNKNOWN_11:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    if (g_api.func_80131F68() != 1) {
                        continue;
                    }
                    g_Dialogue.scriptCur--;
                    return;
                case CSOP_SET_EVENTS:
                    ptr = *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr += (u32)D_pspeu_0927BD50;
                    SetCutsceneEvents((u8*)ptr);
                    continue;
                case CSOP_SCRIPT_UNKNOWN_13:
                    continue;
                case CSOP_SCRIPT_SWITCH:
                    ptr = *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr += (u32)D_pspeu_0927BD50;
                    g_Dialogue.scriptCur += *CS_NEXT(ptr) * 4;

                    ptr = *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur;
                    ptr += (u32)D_pspeu_0927BD50;
                    g_Dialogue.scriptCur = CS_PTR(ptr);
                    continue;
                case CSOP_SCRIPT_UNKNOWN_15:
                    ptr = *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur++;
                    ptr <<= 4;
                    ptr |= *g_Dialogue.scriptCur;
                    ptr += (u32)D_pspeu_0927BD50;
                    g_Dialogue.scriptCur = CS_PTR(ptr);
                    continue;
                case CSOP_WAIT_FOR_FLAG:
                    if (!((g_CutsceneFlags >> *g_Dialogue.scriptCur) & 1)) {
                        g_Dialogue.scriptCur--;
                        return;
                    }
                    g_CutsceneFlags &= ~(1 << *g_Dialogue.scriptCur);
                    g_Dialogue.scriptCur++;
                    continue;
                case CSOP_SET_FLAG:
                    g_CutsceneFlags |= 1 << *g_Dialogue.scriptCur++;
                    continue;
                case CSOP_STOP_EVENTS:
                    g_Dialogue.hasEvents = 0;
                    continue;
                case CSOP_LOAD_PORTRAIT:
                    if (g_SkipCutscene) {
                        g_Dialogue.scriptCur += 5;
                    } else {
                        ptr = *g_Dialogue.scriptCur++;
                        ptr <<= 4;
                        ptr |= *g_Dialogue.scriptCur++;
                        ptr <<= 4;
                        ptr |= *g_Dialogue.scriptCur++;
                        ptr <<= 4;
                        ptr |= *g_Dialogue.scriptCur++;
                        switch (ptr) {
                        case 0:
                            ptr = (u32)&D_pspeu_09272048;
                            break;
                        case 1:
                            ptr = (u32)&D_893DD0C;
                            break;
                        case 2:
                            ptr = (u32)&D_893EA8C;
                            break;
                        case 3:
                            ptr = (u32)&D_893F80C;
                            break;
                        case 4:
                            ptr = (u32)&D_894058C;
                            break;
                        case 5:
                            ptr = (u32)&D_894130C;
                            break;
                        case 6:
                            ptr = (u32)&D_894208C;
                            break;
                        case 7:
                            ptr = (u32)&D_pspeu_09272DC8;
                            break;
                        case 8:
                            ptr = (u32)&D_8943B8C;
                            break;
                        case 9:
                            ptr = (u32)&D_894490C;
                            break;
                        case 10:
                            ptr = (u32)&D_pspeu_09273B48;
                            break;
                        }
                        j = *g_Dialogue.scriptCur++;
                        LoadTPage((u_long*)ptr, 1, 0, D_pspeu_09279668[j], 256,
                                  48, 72);
                    }
                    continue;
                case CSOP_SCRIPT_UNKNOWN_20:
                    nextChar = *g_Dialogue.scriptCur++;
                    nextChar <<= 4;
                    nextChar |= *g_Dialogue.scriptCur++;
                    g_api.PlaySfx(nextChar);
                    continue;
                case CSOP_SCRIPT_UNKNOWN_21:
                    g_CutsceneFlags = CUTSCENE_FLAG_NONE;
                    g_SkipCutscene = false;
                    D_pspeu_0927BB40 = false;
                    continue;
                case CSOP_SCRIPT_UNKNOWN_22:
                    g_CutsceneFlags &= ~(1 << *g_Dialogue.scriptCur++);
                    continue;
                case CSOP_SCRIPT_UNKNOWN_23:
                    return;
                case CSOP_WAIT_FOR_FLAG_RESET:
                    if (!((g_CutsceneFlags >> *g_Dialogue.scriptCur) & 1)) {
                        g_Dialogue.scriptCur--;
                        return;
                    }
                    g_Dialogue.scriptCur++;
                    continue;
                default:
                    if (g_SkipCutscene) {
                        continue;
                    }
                    g_Dialogue.nextCharTimer = g_Dialogue.unk17;
                    endLoop = true;
                }

                if (endLoop) {
                    break;
                }
                continue;
            } else {
                if (g_SkipCutscene) {
                    g_Dialogue.scriptCur++;
                    continue;
                }
                if (167 <= nextChar && nextChar < 173) {
                    nextChar = nextChar + 39;
                } else if (nextChar == 166) {
                    nextChar = 204;
                } else if (nextChar == 221) {
                    nextChar = 205;
                } else {
                    nextChar = nextChar - 17;
                }
                g_Dialogue.nextCharTimer = g_Dialogue.unk17;
            }
            break;
        }

        charBuffer1 = D_pspeu_0927C0D8;
        rect.x = ((nextChar & 0xF) * 2) + 896;
        rect.y = ((nextChar & 0xF0) >> 1) + 240;
        rect.w = 2;
        rect.h = 8;
        vCoord = (g_Dialogue.nextCharY * 12) + 384;
        StoreImage(&rect, (u_long*)charBuffer1);
        for (i = 0; i < 32; i++) {

            charBuffer1[i] =
                ((charBuffer1[i] & 0x0F) << 4) | ((charBuffer1[i] & 0xF0) >> 4);
        }
        while (true) {
            if (nextChar == 32) {
                break;
            }
            for (i = 0; i < 8; i++) {
                if (charBuffer1[(i * 8) / 2] & 0xF0) {
                    break;
                }
            }
            if (i != 8) {
                break;
            }
            for (i = 0; i < 8; i++) {

                charBuffer2 = &charBuffer1[(i * 8) / 2];
                tempChar = (charBuffer2[0] << 24);
                tempChar |= (charBuffer2[1] << 16);
                tempChar |= (charBuffer2[2] << 8);
                tempChar |= (charBuffer2[3]);
                tempChar <<= 4;
                charBuffer2[0] = (tempChar >> 24) & 0xFF;
                charBuffer2[1] = (tempChar >> 16) & 0xFF;
                charBuffer2[2] = (tempChar >> 8) & 0xFF;
                charBuffer2[3] = (tempChar) & 0xFF;
            }
        }
        for (i = 0, charWidth = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if ((charBuffer1[j / 2 + (i * 8) / 2]) &
                    (0xF0 >> ((j & 1) * 4))) {
                    if (j > charWidth) {
                        charWidth = j;
                    }
                };
            }
        }
        charWidth += 2;
        for (i = 0; i < 32; i++) {

            charBuffer1[i] =
                ((charBuffer1[i] & 0x0F) << 4) | ((charBuffer1[i] & 0xF0) >> 4);
        }
        rect.w = charWidth;
        charBuffer1 = D_pspeu_0927C0D8;
        for (i = 0; i < 8; i++) {

            charBuffer2 =
                &cutsceneUnk3Unknown
                    [((g_Dialogue.nextCharX - g_Dialogue.nextLineX) / 2) +
                     (i * 112)];
            if ((g_Dialogue.nextCharX - g_Dialogue.nextLineX) & 1) {
                charBuffer2[0] =
                    (charBuffer2[0] & 0xF0) | ((charBuffer1[0] & 0x0F) << 4);
                charBuffer2[1] = ((charBuffer1[0] & 0xF0) >> 4) |
                                 ((charBuffer1[1] & 0x0F) << 4);
                charBuffer2[2] = ((charBuffer1[1] & 0xF0) >> 4) |
                                 ((charBuffer1[2] & 0x0F) << 4);
                charBuffer2[3] = ((charBuffer1[2] & 0xF0) >> 4) |
                                 ((charBuffer1[3] & 0x0F) << 4);
                charBuffer2[4] = ((charBuffer1[3] & 0xF0) >> 4);
                charBuffer1 += 4;
            } else {
                charBuffer2[0] = charBuffer1[0];
                charBuffer2[1] = charBuffer1[1];
                charBuffer2[2] = charBuffer1[2];
                charBuffer2[3] = charBuffer1[3];
                charBuffer1 += 4;
            }
        }
        LoadTPage((u_long*)cutsceneUnk3Unknown, 0, 0, g_Dialogue.nextLineX,
                  vCoord, 224, 8);
        if (nextChar != 32) {
            g_Dialogue.nextCharX += rect.w;
        } else {
            g_Dialogue.nextCharX += 4;
        }

        if (((g_Dialogue.nextCharX - g_Dialogue.nextLineX) / 2) < 112) {
        };
        D_pspeu_0927C0F8 = nextChar;
        break;
    case DIALOGUE_LOAD_PORTRAIT:
        ptr = (g_Dialogue.nextCharY + 1) & CS_LINE_MAX;
        if (!self->step_s) {
            prim = g_Dialogue.prim[ptr];
            prim->v0 += 2;
            prim->v1 -= 2;
            if (!prim->v1) {
                self->step_s++;
                prim->drawMode = DRAW_HIDE;
            }
        }
        for (i = 0; i < LEN(g_Dialogue.prim) - 1; i++) {
            if (i != ptr) {
                prim = g_Dialogue.prim[i];
                prim->y0 -= 2;
            }
        }
        g_Dialogue.portraitAnimTimer++;
        if (g_Dialogue.portraitAnimTimer >= 8) {
            self->step--;
        }
        break;
    case DIALOGUE_START_TEXT:
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
        prim->x0 = prim->x2 -= 4;
        prim->x1 = prim->x3 += 4;
        prim->y0 = prim->y1 -= 6;
        prim->y2 = prim->y3 += 6;
        g_Dialogue.portraitAnimTimer--;
        if (!g_Dialogue.portraitAnimTimer) {
            self->step = DIALOGUE_RUN;
            for (prim = &g_PrimBuf[g_Dialogue.primIndex[1]]; prim;
                 prim = prim->next) {
                prim->drawMode = DRAW_DEFAULT;
            }
        }
        break;
    case DIALOGUE_UNLOAD_PORTRAIT:
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
        prim->x0 = prim->x2 += 4;
        prim->x1 = prim->x3 -= 4;
        prim->y0 = prim->y1 += 6;
        prim->y2 = prim->y3 -= 6;
        if (prim->x1 >= (g_Dialogue.startX - 2)) {
            prim->x1 = prim->x3 = g_Dialogue.startX - 3;
        }
        g_Dialogue.portraitAnimTimer--;
        if (!g_Dialogue.portraitAnimTimer) {
            self->step = DIALOGUE_RUN;
        }
        break;
    case DIALOGUE_OPEN_DIALOG_BOX:
        switch (self->step_s) {
        case DIALOG_BOX_INIT:

            D_pspeu_0927BB40 = true;
            primIndex =
                g_api.AllocPrimitives(PRIM_LINE_G2, LEN(D_pspeu_09279678));
            if (primIndex == -1) {
                DestroyEntity(self);
                break;
            }
            g_Dialogue.primIndex[0] = primIndex;
            for (prim = &g_PrimBuf[primIndex], uCoord = 0; prim;
                 prim = prim->next) {
                prim->r0 = prim->r1 = 127;
                prim->b0 = prim->b1 = prim->g0 = prim->g1 = 0;

                prim->x0 = prim->x1 = 247;
                prim->y0 = prim->y1 = g_Dialogue.startY + uCoord;
                prim->priority = 510;
                prim->drawMode = DRAW_DEFAULT;
                prim->x2 = D_pspeu_09279678[uCoord];

                prim->x3 = 3952;
                uCoord++;
            }
            self->step_s++;
            break;
        case DIALOG_BOX_DRAW_RED:

            uCoord = false;
            for (prim = &g_PrimBuf[g_Dialogue.primIndex[0]]; prim;
                 prim = prim->next) {
                prim->x3 -= prim->x2;
                prim->x2 += 2;
                prim->x0 = prim->x3 / 16;
                if (prim->x0 < 5) {
                    prim->x0 = 4;
                } else {
                    uCoord = true;
                }
            }
            if (!uCoord) {
                g_api.FreePrimitives(g_Dialogue.primIndex[0]);
                g_Dialogue.primIndex[0] = -1;
                prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
                prim = prim->next;
                prim->drawMode = DRAW_TPAGE | DRAW_TRANSP;
                prim = prim->next;
                prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_TRANSP;
                self->step_s++;
            }
            break;
        case DIALOG_BOX_DRAW_BLUE:
            uCoord = false;
            prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
            prim = prim->next;
            if (prim->r0 < 16) {
                PRED(prim) = 0;
            } else {
                PRED(prim) -= 16;
                uCoord = true;
            }
            if (prim->b0 >= 127) {
                prim->b0 = prim->b1 = 127;
            } else {
                prim->b0 = prim->b1 += 8;
                uCoord = true;
            }

            prim = prim->next;
            if (prim->r0 < 16) {
                PGREY(prim, 0) = 16;
            } else {
                PGREY(prim, 0) -= 15;
                uCoord = true;
            }
            if (!uCoord) {
                self->step = DIALOGUE_RUN;
            }
        }
        break;
    case DIALOGUE_CLOSE_DIALOG_BOX:
        prim = g_Dialogue.prim[LEN(g_Dialogue.prim) - 1];
        prim = prim->next;
        g_Dialogue.portraitAnimTimer--;
        if (g_Dialogue.portraitAnimTimer >= 12) {
            prim = prim->next;
            prim->u0 -= 20;
            if (g_Dialogue.portraitAnimTimer & 1) {
                prim->u0--;
            }
        } else {
            if (!g_Dialogue.portraitAnimTimer) {
                self->step = DIALOGUE_RUN;
                prim->drawMode = DRAW_HIDE;
            } else {
                prim->y2 = (prim->y3 -= 6);
            }
            prim = prim->next;
            prim->drawMode = DRAW_HIDE;
        }
        break;
    case DIALOGUE_END:
        DestroyEntity(self);
        switch (self->params) {
        case 0:
            g_Settings.D_8003CB04 |= 0x8000;
            break;
        case 2:
            g_Settings.D_8003CB04 |= 0x10000;
            break;
        case 3:
            g_Settings.D_8003CB04 |= 0x20000;
            g_CastleFlags[SHAFT_ORB_DEFEATED] = 1;
            break;
        }
        g_CutsceneHasControl = false;
        break;
    }
}
