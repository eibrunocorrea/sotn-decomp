// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"

#include "../../st/get_lang_at.h"

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/st_init", InitEntityIds);

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
