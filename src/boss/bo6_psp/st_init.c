// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

#include "../../st/get_lang_at.h"

extern s32 E_ID(BACKGROUND_BLOCK);
extern s32 E_ID(LOCK_CAMERA);
extern s32 E_ID(ID_13);
extern s32 E_ID(EXPLOSION_VARIANTS);
extern s32 E_ID(GREY_PUFF);
extern s32 E_ID(ID_16);
extern s32 E_ID(ID_17);
extern s32 E_ID(ID_18);
extern s32 E_ID(ID_19);
extern s32 E_ID(ID_1A);
extern s32 E_ID(ID_1B);

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

INCLUDE_ASM("boss/bo6_psp/nonmatchings/bo6_psp/st_init", Load);
