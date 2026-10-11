// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

extern s32 D_pspeu_09265CC8;
extern EInit D_us_801804C4;

extern s32 E_ID(UNK_17);

void func_pspeu_09246448(Entity* self) {
    s16 angle = self->ext.GH_Props.rotate;
    Entity* src;

    angle += self->ext.GH_Props.unkAC;
    if (g_CurrentEntity->facingLeft) {
        angle = -angle;
    }
    src = self->ext.GH_Props.parent;
    self->posX.val = src->posX.val;
    self->posY.val = src->posY.val;
    self->posX.val -= self->ext.GH_Props.length * rsin(angle) * 16;
    self->posY.val += self->ext.GH_Props.length * rcos(angle) * 16;
    self->ext.GH_Props.unkA8 |= 1;
}

void polarPlacePartsWithAngvel(s16* entOffsets) {
    Entity* ent;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            ent = g_CurrentEntity + *entOffsets;
            ent->ext.GH_Props.rotate += ent->ext.GH_Props.rotVel;
            func_pspeu_09246448(ent);
        }
        entOffsets++;
    }
}

void func_801CDD00(Entity* entity, s16 arg1, s16 arg2) {
    s16 temp_t0 = arg1 - entity->ext.GH_Props.rotate;

    if (temp_t0 > ROT(180)) {
        temp_t0 = temp_t0 - ROT(360);
    }

    if (temp_t0 < ROT(-180)) {
        temp_t0 = temp_t0 + ROT(360);
    }

    temp_t0 = temp_t0 / arg2;
    entity->ext.GH_Props.rotVel = temp_t0;
    entity->ext.GH_Props.unkA4 = arg1;
}

void func_801CDD80(s16* entOffsets, unkStr_801CDD80* arg1) {
    Entity* var_s1;
    s16* ptr = arg1->unk4;

    while (*entOffsets) {
        if (*entOffsets != 0xFF) {
            var_s1 = g_CurrentEntity + *entOffsets;
            func_801CDD00(var_s1, *ptr, arg1->unk0);
        }
        ptr++;
        entOffsets++;
    }
}

void func_801CDF1C(s16 entIndices[], unkStr_801CDD80* arg1, s32 arg2) {

    arg1 += g_CurrentEntity->ext.GH_Props.unkB0[arg2];

    if (!g_CurrentEntity->ext.GH_Props.unkB4[arg2]) {
        func_801CDD80(entIndices, arg1);
        g_CurrentEntity->ext.GH_Props.unkB4[arg2] = arg1->unk0;
    }
    if (!--g_CurrentEntity->ext.GH_Props.unkB4[arg2]) {
        arg1++;
        if (!arg1->unk0) {
            g_CurrentEntity->ext.GH_Props.unkB0[arg2] = 0;
        } else {
            ++g_CurrentEntity->ext.GH_Props.unkB0[arg2];
        }
    }
}

void func_801CE1E8(s32 step) {
    s32 i;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!entity->ext.GH_Props.unkA8) {
            func_pspeu_09246448(entity);
        }
        offsets++;
    }
}

s16 func_us_8019A98C(s16 arg0, s16 arg1, s16 arg2) {
    s16 v_s1;
    s16 v_s0;

    arg1 &= 0xFFF;

    v_s1 = arg2 - arg1;
    v_s0 = v_s1;

    if (v_s1 > ROT(180)) {
        v_s0 = v_s1 - ROT(360);
    }
    if (v_s1 < ROT(-180)) {
        v_s0 = v_s1 + ROT(360);
    }

    if (abs(v_s0) > arg0) {
        if (v_s1 < 0) {
            v_s0 = arg1 - arg0;
        } else {
            v_s0 = arg1 + arg0;
        }
        return v_s0;
    }

    return arg2;
}

void func_pspeu_09246AE8(void) {
    Entity* entity;
    s32 posX;
    s32 posY;

    if (!(g_Timer & 0xF)) {
        entity = AllocEntity(&g_Entities[160], &g_Entities[168]);
        if (entity != NULL) {
            CreateEntityFromCurrentEntity(E_ID(UNK_17), entity);
            posX = rand() & 0xFF;
            posY = (rand() & 0x3F) + 0x20;
            entity->posX.i.hi = posX;
            entity->posY.i.hi = posY;
        }
    }
}

#include "../../st/approach_s16.h"

INCLUDE_ASM("boss/rbo2_psp/nonmatchings/rbo2_psp/unk_EDC8", func_us_8019B52C);

extern EInit D_us_801804A0;
extern u8 D_pspeu_09265000[];
extern u8 D_pspeu_09265018[];

void func_us_8019C718(Entity* self) {
    s16 angle;
    s32 posX;
    s32 posY;
    Entity* entity;

    if (D_pspeu_09265CC8 & 8) {
        self->flags |= FLAG_DEAD;
    }
    if ((self->flags & FLAG_DEAD) && self->step != 3) {
        PlaySfxPositional(SFX_METAL_CLANG_C);
        self->hitboxState = 0;
        SetStep(3);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804A0);
        self->facingLeft = Random() & 1;
        self->hitboxState = 0;
        // fallthrough
    case 1:
        if (!AnimateEntity(D_pspeu_09265000, self)) {
            self->hitboxState = 3;
            self->animCurFrame = 0x4F;
            self->drawFlags = ENTITY_ROTATE;
            SetStep(2);
        }
        break;
    case 2:
        if (!self->step_s) {
            entity = &PLAYER;
            posX = entity->posX.i.hi;
            posY = entity->posY.i.hi;
            posX += (Random() & 0x3F) - 0x20;
            posY += (Random() & 0x3F) - 0x20;
            posX -= self->posX.i.hi;
            posY -= self->posY.i.hi;
            angle = ratan2(posY, posX);
            self->velocityX = (rcos(angle) << 16) >> 12;
            self->velocityY = (rsin(angle) << 16) >> 12;
            PlaySfxPositional(SFX_WEAPON_SCRAPE_ECHO);
            self->step_s++;
        }
        MoveEntity();
        self->rotate += ROT(16.875);
        break;
    case 3:
        if (!AnimateEntity(D_pspeu_09265018, self)) {
            DestroyEntity(self);
        }
        break;
    }
}

extern EInit D_us_801804AC;
extern u8 D_pspeu_09265028[];
extern u8 D_pspeu_09265038[24];
extern u8 D_pspeu_09265050[];
extern u8 D_pspeu_09265068[];
extern u8 D_pspeu_09265080[];
extern s32 E_ID(UNK_19);
extern s32 E_ID(UNK_1A);

void func_us_8019C924(Entity* self) {
    Entity* entity;
    s32 posX;
    s32 posY;
    s16 angle;
    s32 distance;
    s32 animResult;
    EnemyDef* enemy;

    FntPrint("ball_step %x\n", self->step);
    if (D_pspeu_09265CC8 & 8) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 3;
        }
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804AC);
        self->hitboxState = 0;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0x80;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        PlaySfxPositional(SFX_RAPID_SYNTH_BUBBLE_SHORT);
        // fallthrough
    case 1:
        if (!AnimateEntity(D_pspeu_09265028, self)) {
            self->step++;
        }
        // fallthrough
    case 2:
        self->scaleX += 4;
        if (self->scaleX > 0x100) {
            self->hitboxState = 1;
            self->drawFlags = ENTITY_DEFAULT;
            SetStep(3);
        }
        self->scaleY = self->scaleX;
        break;
    case 3:
        AnimateEntity(D_pspeu_09265038, self);
        entity = self->ext.et_8019C924.parent;
        if (entity->ext.et_8019B52C.unk88) {
            self->ext.et_8019C924.unk82 = ROT(90);
            SetStep(4);
        }
        break;
    case 4:
        AnimateEntity(D_pspeu_09265038, self);
        MoveEntity();
        entity = &PLAYER;
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
        posX += (Random() & 0x3F) - 0x20;
        posY += (Random() & 0x3F) - 0x20;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        angle =
            LimitAngleChange(ROT(5.625), self->ext.et_8019C924.unk82, angle);
        self->velocityX = (rcos(angle) * FIX(0.4375)) >> 12;
        self->velocityY = (rsin(angle) * FIX(0.4375)) >> 12;
        self->ext.et_8019C924.unk82 = angle;
        entity = self->ext.et_8019C924.parent;
        if (entity->ext.et_8019B52C.unk89) {
            SetStep(5);
        }
        break;
    case 5:
        AnimateEntity(D_pspeu_09265038, self);
        MoveEntity();
        entity = self->ext.et_8019C924.parent;
        if (!self->params) {
            posX = entity->posX.i.hi - 0x20;
        } else {
            posX = entity->posX.i.hi + 0x20;
        }
        posY = entity->posY.i.hi - 0x1C;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        self->velocityX = (rcos(angle) * FIX(1.75)) >> 12;
        self->velocityY = (rsin(angle) * FIX(1.75)) >> 12;
        distance = SquareRoot0(posX * posX + posY * posY);
        if (distance < 4) {
            self->ext.et_8019C924.unk8A = 1;
            self->step++;
        }
        break;
    case 6:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            AnimateEntity(D_pspeu_09265038, self);
            entity = self->ext.et_8019C924.parent;
            if (entity->ext.et_8019B52C.unk8B) {
                self->ext.et_8019C924.unk8A = 0;
                SetSubStep(1);
            }
            break;
        case 1:
            if (!AnimateEntity(D_pspeu_09265050, self)) {
                SetStep(7);
            }
            break;
        }
        break;
    case 7:
        switch (self->step_s) {
        case 0:
            entity = AllocEntity(&g_Entities[176], &g_Entities[184]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_19), self, entity);
                entity->ext.et_8019D4CC.parent = self;
                if (!self->params) {
                    entity->ext.et_8019D4CC.unkA0 = self + 1;
                } else {
                    entity->ext.et_8019D4CC.unkA0 = self - 1;
                }
                entity->facingLeft = self->params;
                entity->zPriority = self->zPriority + 1;
                self->ext.et_8019C924.unk8C = 0;
                PlaySfxPositional(SFX_LOW_SYNTH_BUBBLES);
                self->step_s++;
            }
            break;
        case 1:
            self->ext.et_8019C924.unk80 = 0x60;
            self->step_s++;
            break;
        case 2:
            if (!--self->ext.et_8019C924.unk80) {
                self->step_s = 0;
            }
            break;
        }
        if (g_Timer & 1) {
            self->animCurFrame = 0x39;
        } else {
            self->animCurFrame = 0;
        }
        MoveEntity();
        entity = self->ext.et_8019C924.parent;
        posY = entity->posY.i.hi - 0x1C;
        if (!self->params) {
            posX = entity->posX.i.hi - 0x50;
        } else {
            posX = entity->posX.i.hi + 0x50;
        }
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        self->velocityX = (rcos(angle) * FIX(0.875)) >> 12;
        self->velocityY = (rsin(angle) * FIX(0.875)) >> 12;
        distance = SquareRoot0(posX * posX + posY * posY);
        if (distance < 3) {
            self->velocityX = 0;
            self->velocityY = 0;
        }
        if (D_pspeu_09265CC8 & 4) {
            SetStep(8);
        }
        break;
    case 8:
        switch (self->step_s) {
        case 0:
            animResult = AnimateEntity(D_pspeu_09265068, self);
            MoveEntity();
            entity = self->ext.et_8019C924.parent;
            if (!self->params) {
                posX = entity->posX.i.hi - 0x20;
            } else {
                posX = entity->posX.i.hi + 0x20;
            }
            posY = entity->posY.i.hi - 0x1C;
            posX -= self->posX.i.hi;
            posY -= self->posY.i.hi;
            angle = ratan2(posY, posX);
            self->velocityX = (rcos(angle) * FIX(1.75)) >> 12;
            self->velocityY = (rsin(angle) * FIX(1.75)) >> 12;
            distance = SquareRoot0(posX * posX + posY * posY);
            if (distance < 4 && !animResult) {
                self->ext.et_8019C924.unk8A = 1;
                SetSubStep(1);
            }
            break;
        case 1:
            AnimateEntity(D_pspeu_09265038, self);
            entity = self->ext.et_8019C924.parent;
            if (entity->ext.et_8019B52C.unk8C) {
                self->facingLeft = self->params;
                SetSubStep(2);
            }
            break;
        case 2:
            if (!AnimateEntity(D_pspeu_09265080, self)) {
                enemy = &g_api.enemyDefs[ENEMY_DEATH_UNK_168];
                self->attackElement = enemy->attackElement;
                self->attack = enemy->attack;
                self->hitboxWidth = enemy->hitboxWidth;
                self->hitboxHeight = enemy->hitboxHeight;
                self->hitboxState = 1;
                self->blendMode = BLEND_NO;
                self->drawFlags = ENTITY_ROTATE;
                SetStep(9);
            }
            break;
        }
        break;
    case 9:
        switch (self->step_s) {
        case 0:
            self->ext.et_8019C924.unk8A = 1;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            func_801CDC80(&self->rotate, 0, ROT(5.625));
            entity = self->ext.et_8019C924.parent;
            posY = entity->posY.i.hi - 0x10;
            if (!self->params) {
                posX = entity->posX.i.hi - 0x20;
            } else {
                posX = entity->posX.i.hi + 0x20;
            }
            posX -= self->posX.i.hi;
            entity = self;
            posY -= entity->posY.i.hi;
            angle = ratan2(posY, posX);
            self->velocityX = (rcos(angle) * FIX(0.875)) >> 12;
            self->velocityY = (rsin(angle) * FIX(0.875)) >> 12;
            distance = SquareRoot0(posX * posX + posY * posY);
            if (distance < 3) {
                self->velocityX = 0;
                self->velocityY = 0;
            }
            entity = self->ext.et_8019C924.parent;
            if (entity->ext.et_8019B52C.unk8D) {
                self->ext.et_8019C924.unk8A = 0;
                self->step_s++;
            }
            break;
        case 2:
            entity = &PLAYER;
            posX = entity->posX.i.hi - self->posX.i.hi;
            posY = entity->posY.i.hi - self->posY.i.hi;
            angle = ratan2(posY, posX);
            self->ext.et_8019C924.unk82 = angle;
            self->ext.et_8019C924.unk84 = FIX(6);
            self->step_s++;
            // fallthrough
        case 3:
            MoveEntity();
            self->rotate -= ROT(22.5);
            if (!(self->rotate & 0xF00)) {
                PlaySfxPositional(SFX_DEATH_SCYTHE_SWISH);
            }
            angle = self->ext.et_8019C924.unk82;
            self->velocityX = (self->ext.et_8019C924.unk84 * rcos(angle)) >> 12;
            self->velocityY = (self->ext.et_8019C924.unk84 * rsin(angle)) >> 12;
            self->ext.et_8019C924.unk84 -= self->ext.et_8019C924.unk84 >> 4;
            if (self->ext.et_8019C924.unk84 < FIX(0.25)) {
                self->ext.et_8019C924.unk80 = 0x20;
                self->step_s++;
            }
            if (!(g_Timer & 3)) {
                entity = AllocEntity(&g_Entities[208], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_ID(UNK_1A), self, entity);
                    entity->params = self->animCurFrame;
                    entity->facingLeft = self->facingLeft;
                    entity->rotate = self->rotate;
                }
            }
            break;
        case 4:
            self->rotate -= ROT(45);
            self->rotate &= ROT(360) - 1;
            if (!(self->rotate & 0xF00)) {
                PlaySfxPositional(SFX_DEATH_SCYTHE_SWISH);
            }
            if (!--self->ext.et_8019C924.unk80) {
                self->step_s++;
            }
            break;
        case 5:
            MoveEntity();
            func_801CDC80(&self->rotate, 0, ROT(5.625));
            entity = self->ext.et_8019C924.parent;
            posY = entity->posY.i.hi - 0x10;
            if (!self->params) {
                posX = entity->posX.i.hi - 0x20;
            } else {
                posX = entity->posX.i.hi + 0x20;
            }
            posX -= self->posX.i.hi;
            posY -= self->posY.i.hi;
            angle = ratan2(posY, posX);
            distance = SquareRoot0(posX * posX + posY * posY);
            if (distance > 32) {
                angle = LimitAngleChange(
                    ROT(8.4375), self->ext.et_8019C924.unk82, angle);
            }
            self->velocityX = (rcos(angle) * FIX(3)) >> 12;
            self->velocityY = (rsin(angle) * FIX(3)) >> 12;
            self->ext.et_8019C924.unk82 = angle;
            if (distance < 8) {
                self->step_s = 0;
            }
            break;
        }
        break;
    }
}

extern EInit D_us_801804B8;
extern u8 D_pspeu_09265098[];

void func_us_8019D4CC(Entity* self) {
    Entity* entity;
    s32 posX;
    s16 angle;
    s32 posY;
    s32 distance;

    FntPrint("skull_step %x\n", self->step);
    if (D_pspeu_09265CC8 & 8) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 1;
        }
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_801804B8);
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->scaleX = self->scaleY = 0;
        // fallthrough
    case 1:
        entity = self->ext.et_8019D4CC.parent;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        AnimateEntity(D_pspeu_09265098, self);
        self->scaleX += 12;
        if (self->scaleX > 0x100) {
            self->hitboxState = 1;
            self->drawFlags = ENTITY_DEFAULT;
            self->ext.et_8019D4CC.unk80 = 0x80;
            self->ext.et_8019D4CC.unk82 = ROT(90);
            self->step++;
        }
        self->scaleY = self->scaleX;
        if (D_pspeu_09265CC8 & 4) {
            DestroyEntity(self);
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_09265098, self);
        MoveEntity();
        entity = &PLAYER;
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        angle =
            LimitAngleChange(ROT(2.8125), self->ext.et_8019D4CC.unk82, angle);
        self->velocityX = (rcos(angle) * FIX(3)) >> 12;
        self->velocityY = (rsin(angle) * FIX(3)) >> 12;
        self->ext.et_8019D4CC.unk82 = angle;
        self->ext.et_8019D4CC.unk80--;
        posX = entity->posX.i.hi - self->posX.i.hi;
        if (self->facingLeft) {
            posX = -posX;
        }
        if (!self->ext.et_8019D4CC.unk80 || posX < 0 ||
            (self->hitFlags & 0x80)) {
            self->step++;
        }
        if (!(g_Timer & 3)) {
            entity = AllocEntity(&g_Entities[208], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1A), self, entity);
                entity->params = self->animCurFrame;
                entity->facingLeft = self->facingLeft;
            }
        }
        if (D_pspeu_09265CC8 & 4) {
            SetStep(5);
        }
        break;
    case 3:
        AnimateEntity(D_pspeu_09265098, self);
        MoveEntity();
        entity = self->ext.et_8019D4CC.unkA0;
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
        posX -= self->posX.i.hi;
        posY -= self->posY.i.hi;
        angle = ratan2(posY, posX);
        distance = SquareRoot0(posX * posX + posY * posY);
        if (distance > 32) {
            angle = LimitAngleChange(
                ROT(8.4375), self->ext.et_8019D4CC.unk82, angle);
        }
        self->velocityX = (rcos(angle) * FIX(3)) >> 12;
        self->velocityY = (rsin(angle) * FIX(3)) >> 12;
        self->ext.et_8019D4CC.unk82 = angle;
        if (distance < 4) {
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->scaleX = self->scaleY = 0x100;
            self->hitboxState = 0;
            self->step++;
        }
        if (!(g_Timer & 3)) {
            entity = AllocEntity(&g_Entities[208], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_1A), self, entity);
                entity->params = self->animCurFrame;
                entity->facingLeft = self->facingLeft;
            }
        }
        if (D_pspeu_09265CC8 & 4) {
            SetStep(5);
        }
        break;
    case 4:
        AnimateEntity(D_pspeu_09265098, self);
        entity = self->ext.et_8019D4CC.unkA0;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        self->scaleY = self->scaleX -= 16;
        if (!self->scaleX) {
            entity->ext.et_8019C924.unk8C = 1;
            DestroyEntity(self);
            break;
        }
        if (D_pspeu_09265CC8 & 4) {
            DestroyEntity(self);
        }
        break;
    case 5:
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        MoveEntity();
        break;
    }
}

void func_us_8019D950(Entity* self) {
    if (D_pspeu_09265CC8 & 0x8) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(D_us_801804C4);
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
