// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

extern s32 D_pspeu_09265CC8;
extern EInit D_us_801804DC;

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12BA0", func_us_801A17C8);

typedef struct {
    s16 eArrayOffset;
    s16 eArrayParentOffset;
    u16 length;
    u16 animCurFrame;
    u16 zOffset;
    u16 rotate;
} unkStr_092650E0;

extern EInit g_EInitDeath2;
extern unkStr_092650E0 D_pspeu_092650E0[];
extern s32 D_pspeu_09265CD8;
extern s32 D_pspeu_09265CE0;
extern s32 E_ID(UNK_1D);
extern EInit D_us_801804E8;
extern s32 E_ID(UNK_1F);
extern s32 E_ID(UNK_1E);

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_12BA0", func_us_8019DA04);

void func_us_8019E558(Entity* self) {
    Entity* entity;
    s16 angle;
    unkStr_092650E0* parts;

    switch (self->step) {
    case 0:
        if (self->params == 3 || self->params == 6) {
            InitializeEntity(D_us_801804E8);
        } else {
            InitializeEntity(g_EInitDeath2);
        }
        parts = &D_pspeu_092650E0[self->params];
        self->animCurFrame = parts->animCurFrame;
        self->drawFlags |= ENTITY_ROTATE;
        // fallthrough
    case 1:
        self->rotate = self->ext.et_8019E558.rotate;
        if (D_pspeu_09265CC8 & 0x10) {
            self->step = 2;
        }
        break;
    case 2:
        self->rotate = self->ext.et_8019E558.rotate;
        if (!(Random() & 0x3F)) {
            PlaySfxPositional(SFX_EXPLODE_A);
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1F), self, entity);
                entity->params = 0;
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (D_pspeu_09265CC8 & 0x40) {
            self->animCurFrame = 0;
            self->step = 3;
        }
        break;
    case 3:
        self->animCurFrame = 0;
        self->hitboxState = 0;
        return;
    }
    switch (self->params) {
    case 3:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 10;
            if (D_pspeu_09265CD8) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_ID(UNK_1D), self, entity);
                    entity->facingLeft = self->facingLeft;
                    entity->ext.et_8019E920.parent = self;
                    self->ext.et_8019E558.unk90 = 0;
                    self->step_s++;
                } else {
                    D_pspeu_09265CD8 = 0;
                }
            }
            break;
        case 1:
            self->animCurFrame = 9;
            if (self->ext.et_8019E558.unk90) {
                D_pspeu_09265CD8 = 0;
                self->step_s = 0;
            }
            break;
        }
        if ((D_pspeu_09265CC8 & 0x20) && self->animCurFrame == 10) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1D), self, entity);
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
                entity->zPriority = self->zPriority;
                self->animCurFrame = 9;
                self->step_s = 8;
            }
        }
        // fallthrough
    case 6:
        angle = self->rotate;
        self->hitboxOffX = (rsin(angle) * -12) >> 12;
        self->hitboxOffY = (rcos(angle) * 12) >> 12;
        if (D_pspeu_09265CE0) {
            self->hitboxState = 1;
        } else {
            self->hitboxState = 0;
        }
        if ((D_pspeu_09265CC8 & 0x20) && self->animCurFrame == 14) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1D), self, entity);
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
                entity->zPriority = self->zPriority;
                self->animCurFrame = 13;
            }
        }
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        self->hitboxState = 0;
        break;
    }
}

void func_us_8019E920(Entity* self) {
    Entity* entity;
    s16 angle;
    s32 posX;
    s32 posY;
    s32 distance;

    if ((D_pspeu_09265CC8 & 0x10) && self->step != 3) {
        self->hitboxState = 0;
        if (self->step) {
            SetStep(3);
        }
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804E8);
        self->animCurFrame = 15;
        self->drawFlags = ENTITY_ROTATE;
        if (self->facingLeft) {
            self->ext.et_8019E920.unk82 = 0;
        } else {
            self->ext.et_8019E920.unk82 = ROT(180);
        }
        self->ext.et_8019E920.unk8C = FIX(8);
        // fallthrough
    case 1:
        MoveEntity();
        self->rotate -= ROT(22.5);
        if (!(g_Timer & 7)) {
            PlaySfxPositional(SFX_BONE_SWORD_SWISH_C);
        }
        entity = &PLAYER;
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        angle =
            LimitAngleChange(ROT(1.40625), self->ext.et_8019E920.unk82, angle);
        self->velocityX = (self->ext.et_8019E920.unk8C * rcos(angle)) >> 12;
        self->velocityY = (self->ext.et_8019E920.unk8C * rsin(angle)) >> 12;
        angle = self->ext.et_8019E920.unk82;
        if (!(g_Timer & 3)) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1E), self, entity);
                entity->params = self->animCurFrame;
                entity->facingLeft = self->facingLeft;
                entity->rotate = self->rotate;
            }
        }
        self->ext.et_8019E920.unk8C -= self->ext.et_8019E920.unk8C >> 4;
        if (self->ext.et_8019E920.unk8C < FIX(0.25)) {
            self->ext.et_8019E920.unk80 = 0x20;
            self->step++;
        }
        break;
    case 2:
        MoveEntity();
        self->rotate -= ROT(22.5);
        if (!(g_Timer & 7)) {
            PlaySfxPositional(SFX_BONE_SWORD_SWISH_C);
        }
        entity = self->ext.et_8019E920.parent;
        posY = entity->posY.i.hi;
        posX = entity->posX.i.hi;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        distance = SquareRoot0(posX * posX + posY * posY);
        if (distance > 32) {
            angle = LimitAngleChange(
                ROT(8.4375), self->ext.et_8019E920.unk82, angle);
        } else {
            angle = LimitAngleChange(
                ROT(4.21875), self->ext.et_8019E920.unk82, angle);
        }
        self->velocityX = (rcos(angle) << 18) >> 12;
        self->velocityY = (rsin(angle) << 18) >> 12;
        self->ext.et_8019E920.unk82 = angle;
        if (distance < 8) {
            PlaySfxPositional(SFX_SWITCH_CLICK);
            entity->ext.et_8019E558.unk90 = 1;
            DestroyEntity(self);
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->velocityX = 0;
            self->velocityY = 0;
            self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            self->velocityY += FIX(0.25);
            self->rotate -= ROT(11.25);
            break;
        }
        break;
    }
}

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
