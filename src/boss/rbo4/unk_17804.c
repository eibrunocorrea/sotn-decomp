// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo4.h"


INCLUDE_ASM("boss/rbo4/nonmatchings/unk_17804", func_us_80197938);

INCLUDE_ASM("boss/rbo4/nonmatchings/unk_17804", func_us_8019818C);

INCLUDE_ASM("boss/rbo4/nonmatchings/unk_17804", func_us_8019846C);


INCLUDE_ASM("boss/rbo4/nonmatchings/unk_17804", func_us_80192B38_from_rbo3);

extern EInit D_us_801804E4;

// grupo de familia interna (classe C): mesmo corpo serve como gemeo para
// st/rno1/unk_29930.c func_us_80198A18_from_rbo4 (fora do territorio desta onda)
void func_us_80198A18(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804E4);
        self->drawFlags |= 4;
        if (self->params & 1) {
            self->rotate = -0x400;
        } else {
            self->rotate = 0;
        }
        self->zPriority = (0x40 - self->params) & 0xF;
        if (self->params & 0x100) {
            self->animCurFrame = 0x64;
        } else {
            self->animCurFrame = 0x62;
        }
        self->ext.et_801BDA0C.unk84 = 0;
        self->ext.et_801BDA0C.unk80 = 0;
        break;
    case 1:
        self->palette = 0x804F;
        break;
    }
}

INCLUDE_ASM("boss/rbo4/nonmatchings/unk_17804", func_us_801C0B9C_from_no1);
