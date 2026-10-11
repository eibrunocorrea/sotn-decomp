// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"

#include "../../st/get_lang_at.h"

extern s32 D_pspeu_0926C038;
extern s32 D_pspeu_0926C030;
extern s32 D_pspeu_0926C028;
extern s32 D_pspeu_0926C020;
extern s32 D_pspeu_0926C018;
extern s32 D_pspeu_0926C010;
extern s32 D_pspeu_0926C008;
extern s32 D_pspeu_0926C000;
extern s32 D_pspeu_0926BFF8;
extern s32 D_pspeu_0926BFF0;
extern s32 D_pspeu_0926BFE8;
extern s32 D_pspeu_0926BFE0;
extern s32 D_pspeu_0926BFD8;
extern s32 D_pspeu_0926BFD0;
extern s32 D_pspeu_0926BFC8;
extern s32 D_pspeu_0926BFC0;
extern s32 D_pspeu_0926BFB8;
extern s32 D_pspeu_0926BFB0;
extern s32 D_pspeu_0926BFA8;
extern s32 D_pspeu_0926BFA0;
extern s32 D_pspeu_0926BF98;
extern s32 D_pspeu_0926BF90;
extern s32 D_pspeu_0926BF88;
extern s32 D_pspeu_0926BF80;
extern s32 D_pspeu_0926BF78;
extern s32 D_pspeu_0926BF70;
extern s32 D_pspeu_0926BF68;
extern s32 D_pspeu_0926BF60;
extern s32 D_pspeu_0926BF58;
extern s32 D_pspeu_0926BF50;
extern s32 D_pspeu_0926BF48;
extern s32 D_pspeu_0926BF40;
extern s32 D_pspeu_0926BF38;
extern s32 D_pspeu_0926BF30;

void InitEntityIds(void) {
    D_pspeu_0926C038 = E_BACKGROUND_BLOCK;
    D_pspeu_0926C030 = E_LOCK_CAMERA;
    D_pspeu_0926C028 = E_UNK_ID13;
    D_pspeu_0926C020 = E_EXPLOSION_VARIANTS;
    D_pspeu_0926C018 = E_GREY_PUFF;
    D_pspeu_0926C010 = E_UNK_16;
    D_pspeu_0926C008 = E_UNK_17;
    D_pspeu_0926C000 = E_UNK_18;
    D_pspeu_0926BFF8 = E_UNK_19;
    D_pspeu_0926BFF0 = E_UNK_1A;
    D_pspeu_0926BFE8 = E_UNK_1B;
    D_pspeu_0926BFE0 = E_UNK_1C;
    D_pspeu_0926BFD8 = E_UNK_1D;
    D_pspeu_0926BFD0 = E_UNK_1E;
    D_pspeu_0926BFC8 = E_UNK_1F;
    D_pspeu_0926BFC0 = E_UNK_20;
    D_pspeu_0926BFB8 = E_UNK_21;
    D_pspeu_0926BFB0 = E_UNK_22;
    D_pspeu_0926BFA8 = E_UNK_23;
    D_pspeu_0926BFA0 = E_UNK_24;
    D_pspeu_0926BF98 = E_UNK_25;
    D_pspeu_0926BF90 = E_UNK_26;
    D_pspeu_0926BF88 = E_UNK_27;
    D_pspeu_0926BF80 = E_UNK_28;
    D_pspeu_0926BF78 = E_UNK_29;
    D_pspeu_0926BF70 = E_UNK_2A;
    D_pspeu_0926BF68 = E_UNK_2B;
    D_pspeu_0926BF60 = E_UNK_2C;
    D_pspeu_0926BF58 = E_UNK_2D;
    D_pspeu_0926BF50 = E_BACKGROUND_VORTEX;
    D_pspeu_0926BF48 = E_UNK_2F;
    D_pspeu_0926BF40 = E_UNK_30;
    D_pspeu_0926BF38 = E_UNK_31;
    D_pspeu_0926BF30 = E_UNK_32;
}

#include "../../st/pfn_entity_update.h"

extern u8 D_pspeu_09255AE0[];
extern u8 D_pspeu_09255578[];
extern u8 D_pspeu_09255318[];
extern u8 D_pspeu_09255818[];
extern u8 D_pspeu_09255048[];
extern u8 D_pspeu_09256968[];
extern u8 D_pspeu_09256308[];
extern u8 D_pspeu_09256028[];
extern u8 D_pspeu_09256638[];
extern u8 D_pspeu_09255D28[];
extern u8* D_pspeu_0926BF28;
extern u8* D_pspeu_0926BF20;
extern u8* D_pspeu_0926BF18;
extern u8* D_pspeu_0926BF10;
extern LayoutEntity* D_8D2DE98;
extern LayoutEntity* D_8D2DF6C;
extern AbbreviatedOverlay2 g_BossOverlay;

void InitEntityIds(void);

void RBO6_Load(void) {
    D_pspeu_0926BF28 = GetLangAt(0, D_pspeu_09255AE0, D_pspeu_09255578,
                               D_pspeu_09255318, D_pspeu_09255818,
                               D_pspeu_09255048);
    D_pspeu_0926BF20 = GetLangAt(0, D_pspeu_09255AE0, D_pspeu_09255578,
                               D_pspeu_09255318, D_pspeu_09255818,
                               D_pspeu_09255048);
    D_pspeu_0926BF18 = GetLangAt(0, D_pspeu_09256968, D_pspeu_09256308,
                               D_pspeu_09256028, D_pspeu_09256638,
                               D_pspeu_09255D28);
    D_pspeu_0926BF10 = GetLangAt(0, D_pspeu_09256968, D_pspeu_09256308,
                               D_pspeu_09256028, D_pspeu_09256638,
                               D_pspeu_09255D28);
    InitEntityIds();
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = &D_8D2DE98;
    g_pStObjLayoutVertical = &D_8D2DF6C;
    func_892A018();
    memcpy(&g_api.o, &g_BossOverlay, sizeof(Overlay));
}
