// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

extern s32 D_pspeu_09265CC8;
extern EInit D_us_801804DC;

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12AE8", func_pspeu_0924A168);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12AE8", func_us_801A17C8);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12AE8", func_us_8019DA04);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12AE8", func_us_8019E558);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12AE8", func_us_8019E920);

void func_us_8019ECCC(Entity* self) {
    if (D_pspeu_09265CC8 & 0x10) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804DC);
        self->animCurFrame = self->params;
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->opacity = 0x60;
        if (self->rotate) {
            self->drawFlags |= ENTITY_ROTATE;
        }
    }
    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}
