// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

extern EInit g_EInitCommon;

// background by sword bros, shared with no2/bo0 via no2_bg.h
extern u16 D_us_80180B74[];
extern u32 D_us_80180B84[];
extern u8 D_us_80180BA0[];
extern u8 D_us_80180BAC[];

INCLUDE_ASM("st/rno2/nonmatchings/unk_322E4", func_us_801B3D8C_from_bo0);

void func_us_801B3F30_from_bo0(Entity* self) {
    u8 colorLo;
    u16 color;
    s16 deltaPosXHi;
    s16 absDeltaPosXHi;
    u32 curPal;
    s32 i;
    s32 j;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(2);
        self->animCurFrame = 3;
        self->ext.et_801B3F30.unk7C = 2;
        self->ext.et_801B3F30.unk80 = 0x10;
        self->zPriority = 0x80;
        self->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        break;
    case 1:
        if (g_Tilemap.scrollY.i.hi >= 0x304) {
            deltaPosXHi = self->posX.i.hi - PLAYER.posX.i.hi;
            absDeltaPosXHi = abs(deltaPosXHi);
            if (absDeltaPosXHi < 0x80)
                self->step++;
        }
        break;
    case 2:
        if (--self->ext.et_801B3F30.unk80 == 0) {
            for (i = 0; i < 7; i++) {
                curPal = D_us_80180B84[i];
                for (j = 1; j < 16; j++) {
                    color = g_Clut[0][0x400 + curPal * COLORS_PER_PAL + j];
                    colorLo = color & 0x1F;
                    colorLo++;
                    if (colorLo > 0x1F) {
                        colorLo = 0x1F;
                    }
                    g_Clut[0][0x400 + curPal * COLORS_PER_PAL + j] =
                        (color & ~0x1F) + colorLo;
                }
            }
            LoadClut((void*)&(g_Clut[0][0x400]), 0x200, 0xF4);
            self->ext.et_801B3F30.unk80 = 0x10;
        }
        break;
    }
    if (--self->ext.et_801B3F30.unk7C == 0) {
        self->ext.et_801B3F30.unk7E++;
        self->ext.et_801B3F30.unk7C = 2;
    }
    if (self->ext.et_801B3F30.unk7E > 6) {
        self->ext.et_801B3F30.unk7E = 0;
    }
    self->palette = D_us_80180B74[self->ext.et_801B3F30.unk7E];
}

void func_us_801B4148_from_bo0(Entity* self) {
    if (self->step == 0) {
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(2);
        self->animCurFrame = 1;
        self->zPriority = 0xA0;
    }
}

// background color
void func_us_801B41A4_from_bo0(Entity* self) {
    if (g_CurrentEntity->step == 0) {
        g_CurrentEntity->step++;
    }
    g_GpuBuffers[0].draw.r0 = 0x20;
    g_GpuBuffers[0].draw.g0 = 0x18;
    g_GpuBuffers[0].draw.b0 = 0x28;
    g_GpuBuffers[1].draw.r0 = 0x20;
    g_GpuBuffers[1].draw.g0 = 0x18;
    g_GpuBuffers[1].draw.b0 = 0x28;
}

void func_us_801B4210_from_bo0(Entity* self) {
    Entity* entity;
    bool flag;
    s32 i;

    flag = false;
    if ((g_Entities[self->params + 0x40].entityId) != 1) {
        flag = true;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        self->animSet = ANIMSET_OVL(2);
        self->zPriority = 0x80;
        break;
    case 1:
        if (self->ext.et_801B4210.unk7C == 0 && flag) {
            self->pose = self->poseTimer = 0;
            for (i = 0; i < 5; i++) {
                entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, entity);
                    entity->posX.i.hi += (rand() & 0xF) - 8;
                    entity->posY.i.hi += (rand() & 0xF) - 8;
                    entity->params = 0x10;
                }
            }
        }
        break;
    }
    if (!flag) {
        AnimateEntity(D_us_80180BA0, self);
    } else {
        AnimateEntity(D_us_80180BAC, self);
    }
    self->ext.et_801B4210.unk7C = flag;
}
