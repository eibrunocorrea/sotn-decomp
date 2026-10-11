// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

typedef struct {
    u16 animSet;
    u16 unk5A;
    u16 palette;
    u16 blendMode;
    AnimateEntityFrame* anim;
} EInitUnk22;

extern EInitUnk22 D_pspeu_092658D8[];
extern EInit g_EInitParticle;
extern s32 E_ID(UNK_1F);

void func_us_8019F260(Entity* self) {
    EInitUnk22* initEntry;
    Entity* newEntity;
    s32 params;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        params = self->params & 0xF;
        initEntry = &D_pspeu_092658D8[params];
        self->palette = initEntry->palette + PAL_EXPLOSION_FLAME;
        self->blendMode = initEntry->blendMode;
        self->animSet = initEntry->animSet;
        self->unk5A = initEntry->unk5A;
        self->ext.et_801A518C.anim = (u8*)initEntry->anim;
        self->step = params + 1;
        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        if (self->params & 0xF0) {
            self->palette = PAL_FLAG(PAL_UNK_19F);
            self->blendMode = BLEND_TRANSP;
            self->facingLeft = 1;
        }
        break;
    case 1:
        if (!self->step_s) {
            self->velocityY = FIX(-0.25);
            self->step_s++;
        }
        MoveEntity();
        if (!AnimateEntity(self->ext.et_801A518C.anim, self)) {
            DestroyEntity(self);
        }
        break;
    case 2:
        if (!self->step_s) {
            self->drawFlags = ENTITY_OPACITY;
            self->opacity = 0x60;
            self->facingLeft = Random() & 1;
            self->velocityX = (Random() << 8) - FIX(0.5);
            self->velocityY = FIX(-2.5);
            self->ext.et_801A518C.acceleration = -(Random() << 4) - FIX(0.0625);
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += (s32)self->ext.et_801A518C.acceleration;
        self->opacity--;
        if (self->pose == 8 && !self->poseTimer) {
            newEntity = AllocEntity(&g_Entities[64], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1F), self, newEntity);
                newEntity->params = 1;
                newEntity->zPriority = self->zPriority - 1;
            }
        }
        if (!AnimateEntity(self->ext.et_801A518C.anim, self)) {
            DestroyEntity(self);
        }
        break;
    }
}
