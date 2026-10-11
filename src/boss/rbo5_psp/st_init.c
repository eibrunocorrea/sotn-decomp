// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo5/rbo5.h"

#include "../../st/pfn_entity_update.h"

extern s32 E_ID(BACKGROUND_BLOCK);
extern s32 E_ID(LOCK_CAMERA);
extern s32 E_ID(UNK_ID13);
extern s32 E_ID(EXPLOSION_VARIANTS);
extern s32 E_ID(GREY_PUFF);
extern s32 E_ID(UNK_16);
extern s32 E_ID(UNK_17);
extern s32 E_ID(UNK_18);
extern s32 E_ID(UNK_19);
extern s32 E_ID(UNK_1A);
extern s32 E_ID(UNK_1B);
extern s32 E_ID(UNK_1C);
extern s32 E_ID(UNK_1D);
extern s32 E_ID(UNK_1E);

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
}

extern LayoutEntity* D_8D2DDD8[];
extern LayoutEntity* D_8D2DEAC[];
extern AbbreviatedOverlay2 g_BossOverlay;
extern void* D_pspeu_09266768;
extern void* D_pspeu_09266770;

void func_892A018(void);
void func_8929FA8(void* arg0, s32 arg1);

void RBO5_Load(void) {
    InitEntityIds();
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = D_8D2DDD8;
    g_pStObjLayoutVertical = D_8D2DEAC;
    func_892A018();
    func_8929FA8(D_pspeu_09266768, 0xD3);
    func_8929FA8(D_pspeu_09266770, 0x6);
    memcpy(&g_api.o, &g_BossOverlay, sizeof(Overlay));
}
