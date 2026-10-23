// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

#include "../../st/get_lang_at.h"

s32 E_ID(BACKGROUND_BLOCK);
s32 E_ID(LOCK_CAMERA);
s32 E_ID(UNK_ID13);
s32 E_ID(EXPLOSION_VARIANTS);
s32 E_ID(GREY_PUFF);
s32 E_ID(UNK_16);
s32 E_ID(UNK_17);
s32 E_ID(UNK_18);
s32 E_ID(UNK_19);
s32 E_ID(UNK_1A);
s32 E_ID(UNK_1B);
s32 E_ID(UNK_1C);
s32 E_ID(UNK_1D);
s32 E_ID(UNK_1E);
s32 E_ID(UNK_1F);
s32 E_ID(BOSS_FIGHT_MANAGER);
s32 E_ID(CERBERUS_GATE_DEBUG);
s32 E_ID(LIFE_UP_SPAWN);
s32 E_ID(CUTSCENE_DIALOGUE);
s32 E_ID(UNK_24);

void InitEntityIds(void) {
    SET_E_ID(BACKGROUND_BLOCK);
    SET_E_ID(LOCK_CAMERA);
    SET_E_ID(UNK_ID13);
    SET_E_ID(EXPLOSION_VARIANTS);
    SET_E_ID(GREY_PUFF);
    SET_E_ID(UNK_16);
    SET_E_ID(UNK_17);
    SET_E_ID(UNK_18);
    SET_E_ID(UNK_19);
    SET_E_ID(UNK_1A);
    SET_E_ID(UNK_1B);
    SET_E_ID(UNK_1C);
    SET_E_ID(UNK_1D);
    SET_E_ID(UNK_1E);
    SET_E_ID(UNK_1F);
    SET_E_ID(BOSS_FIGHT_MANAGER);
    SET_E_ID(CERBERUS_GATE_DEBUG);
    SET_E_ID(LIFE_UP_SPAWN);
    SET_E_ID(CUTSCENE_DIALOGUE);
    SET_E_ID(UNK_24);
}

#include "../../st/pfn_entity_update.h"

extern u8 D_pspeu_09253648[];
extern u8 D_pspeu_09253418[];
extern u8 D_pspeu_09253310[];
extern u8 D_pspeu_09253520[];
extern u8 D_pspeu_09253208[];
extern u8* D_pspeu_09265C20;
extern u8* D_pspeu_09265C18;
extern LayoutEntity* D_8D2DE30;
extern LayoutEntity* D_8D2DF04;
extern AbbreviatedOverlay2 g_BossOverlay;

void RBO2_Load(void) {
    D_pspeu_09265C20 =
        GetLangAt(0, D_pspeu_09253648, D_pspeu_09253418, D_pspeu_09253310,
                  D_pspeu_09253520, D_pspeu_09253208);
    D_pspeu_09265C18 =
        GetLangAt(0, D_pspeu_09253648, D_pspeu_09253418, D_pspeu_09253310,
                  D_pspeu_09253520, D_pspeu_09253208);
    InitEntityIds();
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = &D_8D2DE30;
    g_pStObjLayoutVertical = &D_8D2DF04;
    func_892A018();
    memcpy(&g_api.o, &g_BossOverlay, sizeof(Overlay));
}
