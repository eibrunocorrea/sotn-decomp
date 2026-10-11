// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

#include "../../st/get_lang_at.h"

void InitEntityIds(void) {
    SET_E_ID(BACKGROUND_BLOCK);
    SET_E_ID(LOCK_CAMERA);
    SET_E_ID(ID_13);
    SET_E_ID(EXPLOSION_VARIANTS);
    SET_E_ID(GREY_PUFF);
    SET_E_ID(ID_16);
    SET_E_ID(ID_17);
    SET_E_ID(ID_18);
    SET_E_ID(ID_19);
    SET_E_ID(ID_1A);
    SET_E_ID(ID_1B);
}

#include "../../st/pfn_entity_update.h"

extern u8 D_pspeu_0926FF78[];
extern u8 D_pspeu_0926F130[];
extern u8 D_pspeu_0926F5F8[];
extern u8 D_pspeu_0926FAB8[];
extern u8 D_pspeu_0926EC68[];
extern u8 D_pspeu_092701C0[];
extern u8 D_pspeu_0926F3C0[];
extern u8 D_pspeu_0926F878[];
extern u8 D_pspeu_0926FD48[];
extern u8 D_pspeu_0926EEE8[];
extern u8 D_pspeu_09270208[];
extern u8 D_pspeu_0926F408[];
extern u8 D_pspeu_0926F8C0[];
extern u8 D_pspeu_0926FD90[];
extern u8 D_pspeu_0926EF30[];
extern u8* D_pspeu_0927BAD0;
extern u8* D_pspeu_0927BAC8;
extern u8* D_pspeu_0927BAC0;
extern u8* D_pspeu_0927BAB8;
extern u8* D_pspeu_0927BAB0;
extern u8* D_pspeu_0927BAA8;
extern u8* D_pspeu_0927BAA0;
extern u8* D_pspeu_0927BA98;
extern u8* D_pspeu_0927BA90;
extern LayoutEntity* D_8D2DDC0;
extern LayoutEntity* D_8D2DE94;
extern AbbreviatedOverlay2 g_BossOverlay;
extern u8** D_pspeu_092703C0;

void func_892A018(void);
void func_8929FA8(void* arg0, s32 arg1);

void BO6_Load(void) {
    D_pspeu_0927BAD0 =
        GetLangAt(8, D_pspeu_0926FF78, D_pspeu_0926F130, D_pspeu_0926F5F8,
                  D_pspeu_0926FAB8, D_pspeu_0926EC68);
    D_pspeu_0927BAC8 =
        GetLangAt(0, D_pspeu_0926FF78, D_pspeu_0926F130, D_pspeu_0926F5F8,
                  D_pspeu_0926FAB8, D_pspeu_0926EC68);
    D_pspeu_0927BAC0 =
        GetLangAt(0, D_pspeu_092701C0, D_pspeu_0926F3C0, D_pspeu_0926F878,
                  D_pspeu_0926FD48, D_pspeu_0926EEE8);
    D_pspeu_0927BAB8 =
        GetLangAt(0, D_pspeu_092701C0, D_pspeu_0926F3C0, D_pspeu_0926F878,
                  D_pspeu_0926FD48, D_pspeu_0926EEE8);
    D_pspeu_0927BAB0 =
        GetLangAt(4, D_pspeu_09270208, D_pspeu_0926F408, D_pspeu_0926F8C0,
                  D_pspeu_0926FD90, D_pspeu_0926EF30);
    D_pspeu_0927BAA8 =
        GetLangAt(0, D_pspeu_09270208, D_pspeu_0926F408, D_pspeu_0926F8C0,
                  D_pspeu_0926FD90, D_pspeu_0926EF30);
    D_pspeu_0927BAA0 =
        GetLangAt(0, D_pspeu_0926FF78, D_pspeu_0926F130, D_pspeu_0926F5F8,
                  D_pspeu_0926FAB8, D_pspeu_0926EC68);
    D_pspeu_0927BA98 =
        GetLangAt(4, D_pspeu_0926FF78, D_pspeu_0926F130, D_pspeu_0926F5F8,
                  D_pspeu_0926FAB8, D_pspeu_0926EC68);
    D_pspeu_0927BA90 =
        GetLangAt(0, D_pspeu_09270208, D_pspeu_0926F408, D_pspeu_0926F8C0,
                  D_pspeu_0926FD90, D_pspeu_0926EF30);
    InitEntityIds();
    PfnEntityUpdates = EntityUpdates;
    g_pStObjLayoutHorizontal = &D_8D2DDC0;
    g_pStObjLayoutVertical = &D_8D2DE94;
    func_892A018();
    func_8929FA8(D_pspeu_092703C0, 0xC5);
    memcpy(&g_api.o, &g_BossOverlay, sizeof(Overlay));
}
