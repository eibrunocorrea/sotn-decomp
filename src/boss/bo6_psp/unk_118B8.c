// SPDX-License-Identifier: AGPL-3.0-or-later
#include "player_copy.h"

extern PfnEntityUpdate D_pspeu_09279AA8[];
extern AnimationFrame* D_pspeu_0927C158[];
void func_pspeu_09252B48_from_rbo5(void) {
    Entity* entity;
    PfnEntityUpdate entityUpdate;
    s32 i;
    s32 isPrologueTimeStopped;

    isPrologueTimeStopped = g_unkGraphicsStruct.unk28;
    entity = g_CurrentEntity = &g_Entities[E_ID_44];

    for (i = E_ID_44; i < E_ID_90; i++, g_CurrentEntity++, entity++) {
        if (entity->entityId != 0) {
            entityUpdate = D_pspeu_09279AA8[entity->entityId];
            entityUpdate(entity);
            entity = g_CurrentEntity;

            if (entity->entityId != 0) {
                if (!(entity->flags & FLAG_UNK_10000000) &&
                    (entity->posX.i.hi > 288 || entity->posX.i.hi < -32 ||
                     entity->posY.i.hi > 256 || entity->posY.i.hi < -16)) {
                    DestroyEntity(g_CurrentEntity);
                } else {
                    if (entity->flags & FLAG_UNK_20000000) {
                        g_api.UpdateAnim(NULL, (AnimationFrame**)D_pspeu_0927C158);
                    }
                    entity->flags |= FLAG_NOT_AN_ENEMY;
                }
            }
        }
    }

    if (D_pspeu_0927C230) {
        D_pspeu_0927C230--;
        if (D_pspeu_0927C230 & 1) {
            g_api.func_800EA5AC(1, D_pspeu_0927C228, D_pspeu_0927C220, D_pspeu_0927C218);
        }
    }
    if ((MARIA.step == 0x11) || (MARIA.step == 0x60) || (MARIA.step == 0x70)) {
        FntPrint("dead boss\n");
        entity = &g_Entities[E_ID_44];
        for (i = E_ID_44; i < E_ID_90; i++, entity++) {
            entity->hitboxState = 0;
        };
    }
}


// local copy of RicCreateEntFactoryFromEntity (see us_39144.c); note the
// (68, 80) slot range - cen's Maria uses (72, 80)
Entity* MarCreateEntFactoryFromEntity(
    Entity* source, u32 factoryParams, s32 arg2) {
    Entity* entity = MarGetFreeEntity(68, 80);
    if (!entity) {
        return NULL;
    }
    DestroyEntity(entity);
    entity->entityId = E_FACTORY;
    // the parent pointer must align for anything the factory creates
    entity->ext.factory.parent = source;
    entity->posX.val = source->posX.val;
    entity->posY.val = source->posY.val;
    entity->facingLeft = source->facingLeft;
    entity->zPriority = source->zPriority;
    entity->params = factoryParams & 0xFFF;
    entity->ext.factory.paramsBase = (factoryParams & 0xFF0000) >> 8;
    return entity;
}

extern FactoryBlueprint D_pspeu_09279BD0[];
extern u8 D_pspeu_09279D98[][2];
void MarEntityFactory(Entity* self) {
    Entity* newEntity;
    s16 nPerCycle;
    s16 i;
    s16 startIndex;
    s16 endIndex;
    u8* data;

    if (self->step == 0) {
        data = (u8*)&D_pspeu_09279BD0[self->params];
        self->ext.factory.newEntityId = *data++;
        self->ext.factory.amount = *data++;
        self->ext.factory.nPerCycle = *data & 0x3F;
        self->ext.factory.isNonCritical = (s16)(*data >> 7) & 1;
        self->ext.factory.incParamsKind = (s16)(*data++ >> 6) & 1;
        self->ext.factory.tCycle = *data++;
        self->ext.factory.kind = *data & 0x7;
        self->ext.factory.origin = (s16)(*data++ >> 3) & 0x1F;
        self->ext.factory.delay = *data;
        self->flags |= FLAG_UNK_10000000;
        self->step++;
        switch (self->ext.factory.origin) {
        case 0:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        case 4:
            self->flags |= 0;
        case 2:
        case 9:
            self->flags |= FLAG_POS_CAMERA_LOCKED;
        case 3:
        case 7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case 8:
            if (self->ext.factory.parent) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
            }
            self->flags |= FLAG_POS_CAMERA_LOCKED;
            break;
        }
    } else {
        switch (self->ext.factory.origin) {
        case 0:
            break;
        case 9:
            if (g_Maria.unk4E) {
                DestroyEntity(self);
                return;
            }
        case 2:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            break;
        case 4:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != PL_S_RUN) {
                self->entityId = 0;
                return;
            }
            break;
        case 3:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step == PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 7:
            self->posX.val = MARIA.posX.val;
            self->posY.val = MARIA.posY.val;
            if (MARIA.step != PL_S_HIT) {
                self->entityId = 0;
                return;
            }
            break;
        case 8:
            if (self->ext.factory.parent) {
                self->posX.val = self->ext.factory.parent->posX.val;
                self->posY.val = self->ext.factory.parent->posY.val;
            }
            break;
        }
    }
    if (self->ext.factory.delay) {
        if (--self->ext.factory.delay) {
            return;
        }
        self->ext.factory.delay = self->ext.factory.tCycle;
    }
    nPerCycle = self->ext.factory.nPerCycle;
    for (i = 0; i < nPerCycle; i++) {
        data = D_pspeu_09279D98[0];
        data += self->ext.factory.kind * 2;
        startIndex = *data++;
        endIndex = *data;
        if (self->ext.factory.kind == 0) {
            newEntity = MarGetFreeEntityReverse(startIndex, endIndex + 1);
        } else if (self->ext.factory.kind == 4) {
            newEntity = &g_Entities[95];
        } else if (self->ext.factory.kind == 5) {
            newEntity = &g_Entities[112];
        } else {
            newEntity = MarGetFreeEntity(startIndex, endIndex + 1);
        }
        if (newEntity == NULL) {
            if (self->ext.factory.isNonCritical == 1) {
                self->entityId = 0;
            } else {
                self->ext.factory.delay = self->ext.factory.tCycle;
            }
            return;
        }
        DestroyEntity(newEntity);
        newEntity->entityId =
            self->ext.factory.newEntityId + self->ext.factory.entityIdMod;
        newEntity->params = self->ext.factory.paramsBase;
        // The child  (newEntity) is not an ent factory, but because the
        // factory creates many entities, we can't pick a particular extension.
        // But we're not allowed to use generic, so i'll just reuse entFactory.
        newEntity->ext.factory.parent = self->ext.factory.parent;
        newEntity->posX.val = self->posX.val;
        newEntity->posY.val = self->posY.val;
        newEntity->facingLeft = self->facingLeft;
        newEntity->zPriority = self->zPriority;
        if (self->ext.factory.incParamsKind) {
            newEntity->params += self->ext.factory.spawnIndex;
        } else {
            newEntity->params += i;
        }
        self->ext.factory.spawnIndex++;
        if (self->ext.factory.spawnIndex == self->ext.factory.amount) {
            self->entityId = 0;
            return;
        }
    }
    self->ext.factory.delay = self->ext.factory.tCycle;
}

extern EInit D_pspeu_0926EB88;
void MarEntitySlideKick(Entity* entity) {
    if (MARIA.step != PL_S_SLIDE) {
        DestroyEntity(entity);
    } else {
        entity->posX.i.hi = MARIA.posX.i.hi;
        entity->posY.i.hi = MARIA.posY.i.hi;
        entity->facingLeft = MARIA.facingLeft;
        if (entity->step == 0) {
            InitializeEntity(D_pspeu_0926EB88);
            entity->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
            entity->hitboxOffX = 0x14;
            entity->hitboxOffY = 0xC;
            entity->hitboxWidth = entity->hitboxHeight = 9;
            entity->ext.subweapon.timer = entity->hitboxState;
            entity->step = 1;
        }
        entity->hitboxState = entity->ext.subweapon.timer;
        if (MARIA.pose < 2) {
            entity->hitboxState = 0;
        }
        if (MARIA.pose > 7) {
            DestroyEntity(entity);
        }
    }
}

// local copy of func_us_801BC3E0 (see us_39144.c) - slide kick hitbox
void func_pspeu_0924A2E0(Entity* self) {
    if (MARIA.step != PL_S_SLIDE_KICK) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->facingLeft = MARIA.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_pspeu_0926EB98);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0x14;
        self->hitboxWidth = self->hitboxHeight = 9;
        self->step = 1;
    }

    if (MARIA.animCurFrame == 140) {
        self->hitboxOffY = 0;
    }

    if (MARIA.animCurFrame == 141) {
        self->hitboxOffY = 12;
    }

    if (self->hitFlags) {
        g_Maria.unk44 |= 0x80;
    } else {
        g_Maria.unk44 &= ~0x80;
    }
    self->hitFlags = 0;
}

extern EInit D_pspeu_0926EB68;
void func_pspeu_0924A448(Entity* self) {
    if (MARIA.step != PL_S_BLADEDASH) {
        DestroyEntity(self);
    } else {
        self->posX.i.hi = MARIA.posX.i.hi;
        self->posY.i.hi = MARIA.posY.i.hi;
        self->facingLeft = MARIA.facingLeft;
        if (self->step == 0) {
            InitializeEntity(D_pspeu_0926EB68);
            self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
            self->hitboxOffX = self->hitboxOffY = 0;
            self->hitboxWidth = self->hitboxHeight = 20;
            self->hitboxWidth = self->hitboxHeight = 12;
            self->ext.subweapon.subweaponId = 0x11;
            self->step++;
        }
        if (MARIA.pose > 0x12) {
            DestroyEntity(self);
        }
    }
}

extern EInit D_pspeu_0926EB78;
void func_pspeu_0924A578(Entity* self) {
    if (MARIA.step != PL_S_HIGHJUMP) {
        DestroyEntity(self);
        return;
    }
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    self->facingLeft = MARIA.facingLeft;
    if (self->step == 0) {
        InitializeEntity(D_pspeu_0926EB78);
        self->flags = FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED;
        self->hitboxOffX = 0xC;
        self->hitboxOffY = -0x1A;
        self->hitboxWidth = 12;
        self->hitboxHeight = 12;
        self->step = 1;
    }
}

extern s16 D_pspeu_09279DA8[];
extern s32 D_pspeu_09279DB8[];
extern s16 D_pspeu_09279DD0[];
extern AnimationFrame D_pspeu_09279DE0[];
void func_pspeu_0924A660(Entity* self) {
    s16 posX;

    s16 paramsHi = self->params >> 8;
    s16 paramsLo = self->params & 0xFF;

    switch (self->step) {
    case 0:
        self->animSet = ANIMSET_DRA(5);
        self->anim = D_pspeu_09279DE0;
        self->zPriority = MARIA.zPriority + 2;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_OPACITY;
        self->opacity = 0x60;
        posX = D_pspeu_09279DA8[paramsLo];
        if (paramsHi == 0) {
            posX += 6;
        }
        if (paramsHi == 1) {
            posX -= 8;
        }
        if (paramsHi == 2) {
            posX -= 6;
        }
        if (paramsHi == 5) {
            posX = -6;
        }
        if (paramsHi == 3) {
            self->posY.i.hi -= 8;
        }
        if (paramsHi == 4) {
            DestroyEntity(self);
            return;
        }
        if (paramsHi == 1) {
            if (g_Maria.vram_flag & TOUCHING_ANY_SLOPE) {
                posX /= 2;
            }
        }
        if (self->facingLeft) {
            posX = -posX;
        }
        self->posX.i.hi += posX;
        self->posY.i.hi += 0x18;
        self->scaleX = D_pspeu_09279DD0[paramsLo] + 0x40;
        self->velocityY = D_pspeu_09279DB8[paramsLo];
        if (paramsHi == 1) {
            self->velocityY = FIX(-0.25);
            MarSetSpeedX(FIX(-0.1875));
            self->scaleX = D_pspeu_09279DD0[1] + 0x40;
        }
        if (paramsHi == 5) {
            self->velocityY = D_pspeu_09279DB8[4 - paramsLo * 2];
        }
        if (paramsHi == 2) {
            self->velocityY = FIX(-0.5);
            MarSetSpeedX(FIX(-0.1875));
            self->scaleX = D_pspeu_09279DD0[1] + 0x40;
        }
        self->scaleY = self->scaleX;
        self->step++;
        break;
    case 1:
        self->opacity -= 2;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
}


extern unkStr_8011E4BC* D_pspeu_09279ED8[];
extern s32 D_pspeu_0927C308[];
extern s32 D_pspeu_0927C288[];
void MarEntityHitByCutBlood(Entity* self) {
    byte stackpad[0x28];
    u8 thickness;
    s16 variant;
    s16 playerX;
    s16 playerY;
    s16 velocityScale;
    s16 velocityRnd;
    s32 timer;
    s16 x;
    s16 y;
    s16 rnd;
    s32 i;
    unkStr_8011E4BC* props;
    FakePrim* tilePrim;

    variant = (self->params) >> 8;
    props = D_pspeu_09279ED8[variant];
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    playerX = MARIA.posX.i.hi;
    playerY = MARIA.posY.i.hi;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.func_800EDB58(PRIM_TILE_ALT, props->count + 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = props->flags;
        switch (props->unkA) {
        case 8:
            self->ext.et_8011E4BC.unk7C = 0x100;
            break;
        case 7:
            self->ext.et_8011E4BC.unk7C = 0x3F;
            break;
        case 4:
            self->ext.et_8011E4BC.unk7C = 0x2F;
            break;
        case 3:
        case 6:
            self->ext.et_8011E4BC.unk7C = 0x1F;
            break;
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (1) {
            tilePrim->drawMode = props->drawMode;
            tilePrim->priority = MARIA.zPriority + props->priority;
            if (tilePrim->next == NULL) {
                tilePrim->drawMode &= ~DRAW_HIDE;
                tilePrim->y0 = tilePrim->x0 = tilePrim->w = 0;
                break;
            }
            tilePrim->posX.i.hi = x;
            tilePrim->posY.i.hi = y;
            tilePrim->posX.i.lo = tilePrim->posY.i.lo = 0;
            switch (props->unkA) {
            case 0:
                velocityRnd = rand() & PSP_RANDMASK;
                velocityScale = (velocityRnd & 1) + 2;
                tilePrim->velocityX.val = (rcos(velocityRnd) << velocityScale);
                tilePrim->velocityY.val = -(rsin(velocityRnd) << velocityScale);
                break;
            case 1:
                tilePrim->velocityX.val = ((rand() & 0x1FF) - 0x100) << 8;
                tilePrim->velocityY.val = ((rand() & 0x1FF) - 0x100) << 8;
                break;
            case 3:
                tilePrim->posX.i.hi = x + (rand() & 0xF) - 7;
                tilePrim->posY.i.hi = y - (rand() & 0xF);
                tilePrim->velocityY.val = FIX(-0.75) - (rand() & 0x7FFF);
                if (self->ext.et_8011E4BC.parent != NULL) {
                    tilePrim->velocityX.val =
                        self->ext.et_8011E4BC.parent->velocityX;
                }
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 4:
                rnd = rand() & 0x1F;
                tilePrim->posX.i.hi = x + rnd - 0x10;
                rnd = rand() & 0x1F;
                tilePrim->posY.i.hi = y + rnd - 0x14;
                rnd = rand() & 0x1F;
                tilePrim->velocityX.val = D_pspeu_0927C308[rnd];
                tilePrim->velocityY.val = D_pspeu_0927C288[rnd];
                break;
            case 6:
                tilePrim->posX.i.hi = self->posX.i.hi + (rand() & 0xF) - 8;
                tilePrim->posY.i.hi = self->posY.i.hi + (rand() & 0xF) - 4;
                tilePrim->velocityY.val = (rand() & PSP_RANDMASK) + FIX(0.5);
                tilePrim->delay = (rand() & 0xF) + 0x10;
                break;
            case 7:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                if (i < 10) {
                    tilePrim->velocityY.val =
                        -((i * i * FIX(0.09375)) + FIX(0.125));
                } else {
                    tilePrim->velocityY.val =
                        (i - 10) * (i - 10) * FIX(0.09375) + FIX(0.125);
                }
                tilePrim->delay = 0x3F;
                break;
            case 8:
                tilePrim->posX.i.hi = x;
                tilePrim->posY.i.hi = y;
                tilePrim->velocityX.val = ((rand() & 0x7FFF) - FIX(0.25)) >> 1;

                tilePrim->velocityY.val = -((rand() & 0x1FFF) + FIX(0.375));
                tilePrim->timer = (i * 4);
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            tilePrim->r0 = props->r;
            tilePrim->g0 = props->g;
            tilePrim->b0 = props->b;
            tilePrim->w = props->w;
            tilePrim->h = props->h;
            i++;
            tilePrim = tilePrim->next;
        }
        self->step++;
        break;
    case 1:
        switch (props->unkA) {
        case 3:
        case 6:
        case 7:
        case 8:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
        case 4:
            if (--self->ext.et_8011E4BC.unk7C == 0) {
                DestroyEntity(self);
                return;
            }
            thickness = 3;
            timer = self->ext.et_8011E4BC.unk7C;
            if (timer < 9) {
                thickness--;
            }
            if (timer < 4) {
                thickness--;
            }
        }
        tilePrim = (FakePrim*)&g_PrimBuf[self->primIndex];
        i = 0;
        while (1) {
            if (tilePrim->next == NULL) {
                tilePrim->drawMode &= ~DRAW_HIDE;
                tilePrim->y0 = tilePrim->x0 = tilePrim->w = 0;
                return;
            }
            tilePrim->posX.i.hi = tilePrim->x0;
            tilePrim->posY.i.hi = tilePrim->y0;
            switch (props->unkA) {
            case 0:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->r0 -= 6;
                tilePrim->g0 -= 6;
                tilePrim->b0 -= 6;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 1:
                tilePrim->posY.val += tilePrim->velocityY.val;
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->velocityY.val =
                    (tilePrim->velocityY.val + FIX(0.15625));
                tilePrim->r0 -= 3;
                tilePrim->g0 -= 3;
                tilePrim->b0 -= 3;
                if (tilePrim->r0 < 8) {
                    DestroyEntity(self);
                    return;
                }
                break;
            case 3:
            case 6:
            case 7:
                tilePrim->posY.val += tilePrim->velocityY.val;
                if (--tilePrim->delay < 0) {
                    tilePrim->drawMode |= DRAW_HIDE;
                }
                break;
            case 4:
                tilePrim->posX.val += tilePrim->velocityX.val;
                tilePrim->posY.val += tilePrim->velocityY.val;
                // There is probably a clever way to write this
                tilePrim->velocityY.val -= tilePrim->velocityY.val >> 5;
                if (!(timer & 7)) {
                    tilePrim->velocityX.val >>= 1;
                    tilePrim->velocityY.val >>= 1;
                    if (timer & 0x20) {
                        tilePrim->velocityY.val >>= 1;
                    }
                    if (timer == 0x18) {
                        tilePrim->drawMode = DRAW_UNK02;
                    }
                }
                tilePrim->w = tilePrim->h = thickness;
                break;
            case 8:
                if (tilePrim->timer == 0) {
                    tilePrim->drawMode &= ~DRAW_HIDE;
                    tilePrim->r0 -= 1;
                    tilePrim->g0 -= 1;
                    tilePrim->b0 -= 1;
                    tilePrim->posY.val += tilePrim->velocityY.val;
                    tilePrim->posX.val += tilePrim->velocityX.val;
                    if (g_unkGraphicsStruct.D_80097448 == 0 ||
                        !(tilePrim->posY.i.hi >
                          (MARIA.posY.i.hi - g_unkGraphicsStruct.D_80097448 +
                           0x19))) {
                        tilePrim->drawMode |= DRAW_HIDE;
                    }
                } else {
                    tilePrim->timer--;
                }
                break;
            }
            tilePrim->x0 = tilePrim->posX.i.hi;
            tilePrim->y0 = tilePrim->posY.i.hi;
            i++;
            tilePrim = tilePrim->next;
        }
    }
}

extern AnimationFrame D_pspeu_09279F40[];
extern AnimationFrame D_pspeu_09279F00[];
extern AnimationFrame D_pspeu_09279DE0[];
void func_maria_80161C2C(Entity* self) {
    s16 paramsHi;
    s16 paramsLo;

    paramsLo = self->params & 0xFF;
    paramsHi = (self->params >> 8) & 0xFF;
    switch (self->step) {
    case 0:
        if (paramsHi == 1) {
            self->scaleX = 0xC0;
            self->scaleY = 0xC0;
            self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
            self->animSet = ANIMSET_DRA(2);
            self->anim = D_pspeu_09279F40;
        }
        if (paramsHi == 0 || paramsHi == 2) {
            if (paramsLo & 3) {
                self->anim = D_pspeu_09279F00;
                self->scaleX = 0x120;
                self->scaleY = 0x120;
                self->drawFlags = ENTITY_SCALEX | ENTITY_SCALEY;
                self->animSet = ANIMSET_DRA(2);
            } else {
                self->animSet = ANIMSET_DRA(5);
                self->anim = D_pspeu_09279DE0;
                self->palette = PAL_FLAG(PAL_UNK_170);
            }
        }
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        if (rand() & 3) {
            self->zPriority = MARIA.zPriority + 2;
        } else {
            self->zPriority = MARIA.zPriority - 2;
        }
        if (paramsHi == 2) {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() % 44) - 22;
        } else {
            self->posX.i.hi = MARIA.posX.i.hi + (rand() & 15) - 8;
        }
        self->posY.i.hi =
            MARIA.posY.i.hi + MARIA.hitboxOffY + (rand() & 31) - 16;
        self->velocityY = FIX(-0.5);
        self->velocityX = MARIA.velocityX >> 2;
        self->step++;
        break;
    case 1:
        self->scaleX -= 4;
        self->scaleY -= 4;
        self->posY.val += self->velocityY;
        self->posX.val += self->velocityX;
        if ((self->pose == 8) && (self->anim != D_pspeu_09279DE0)) {
            self->blendMode = BLEND_TRANSP;
            if (!(paramsLo & 1) && self->poseTimer == 1) {
                MarCreateEntFactoryFromEntity(self, FACTORY(4, 4), 0);
            }
        }
        if (self->pose == 16 && self->anim == D_pspeu_09279DE0) {
            self->blendMode = BLEND_TRANSP;
        }
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

extern AnimationFrame D_pspeu_09279F78[];
void func_pspeu_0924B8D0(Entity* self) {
    switch (self->step) {
    case 0:
        self->animSet = ANIMSET_DRA(2);
        self->anim = D_pspeu_09279F78;
        self->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        self->zPriority = MARIA.zPriority + 4;
        self->velocityY = (rand() & 0x3FFF) - 0x10000;
        self->step++;
        break;
    case 1:
        if (self->pose == 6) {
            if (self->poseTimer == 1 && (rand() & 1)) {
                MarCreateEntFactoryFromEntity(self, 4, 0);
            }
        }
        self->posY.val += self->velocityY;
        if (self->poseTimer < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

// local copy of func_us_801BD47C (see us_39144.c) - true when another
// live entity shares this id and params
bool func_maria_80162E9C(Entity* entity) {
    Entity* e;
    s32 i;
    s16 objId;
    s16 params;

    objId = entity->entityId;
    params = entity->params;
    for (e = &g_Entities[0x50], i = 0x50; i < 0x90; e++, i++) {
        if (objId == e->entityId && params == e->params && e != entity) {
            return true;
        }
    }
    return false;
}

extern s16* D_916F800[];
extern s16* D_pspeu_0926D6F8[];
extern u8** D_pspeu_092703C0;
extern s16 D_pspeu_09279FA0[][10];
extern s32 D_pspeu_0927C160[];
void func_pspeu_0925B4E0(s32 kind, s16 invincibilityFrames);
void MarEntityPlayerBlinkWhite(Entity* self) {
    u8 xMargin;
    u8 yMargin;
    s16 angle;
    u8 wSprite;
    u8 hSprite;
    s16 width;
    s16 height;
    s16 selfX;
    s16 selfY;
    s32 i;
    Primitive* prim;
    s16 xPivot;
    s16 yPivot;
    s16 plSpriteIndex;
    s16 upperParams;
    s16 angleRedIndex;
    s16* sp44;
    u8* plSprite;
    s16* dataPtr;
    s16 angleGreenIndex;
    s16 angleBlueIndex;
    s16 redDivide;
    s16 blueDivide;
    s16 greenDivide;
    void* dummy48;

    if (!MARIA.animSet || !(MARIA.animCurFrame & 0x7FFF)) {
        DestroyEntity(self);
        return;
    }
    self->posY.i.hi = MARIA.posY.i.hi;
    self->posX.i.hi = MARIA.posX.i.hi;
    self->facingLeft = MARIA.facingLeft;
    sp44 = D_916F800[MARIA.animCurFrame & 0x7FFF];
    sp44 = D_pspeu_0926D6F8[MARIA.animCurFrame & 0x7FFF];
    plSpriteIndex = *sp44++;
    plSpriteIndex &= 0x7FFF;
    selfX = self->posX.i.hi;
    selfY = self->posY.i.hi;
    plSprite = D_pspeu_092703C0[plSpriteIndex];
    xMargin = 4;
    yMargin = 1;
    wSprite = xMargin + plSprite[0];
    hSprite = yMargin + plSprite[1];
    width = wSprite - xMargin;
    height = hSprite - yMargin;
    xPivot = sp44[0] + plSprite[2];
    yPivot = sp44[1] + plSprite[3];

    self->rotate = MARIA.rotate;
    self->drawFlags = MARIA.drawFlags;
    self->scaleX = MARIA.scaleX;
    self->scaleY = MARIA.scaleY;
    self->rotPivotY = MARIA.rotPivotY;
    self->rotPivotX = MARIA.rotPivotX;
    upperParams = (self->params & 0x7F00) >> 8;
    dataPtr = D_pspeu_09279FA0[upperParams & 0x3F];
    switch (self->step) {
    case 0:
        if (func_maria_80162E9C(self) != 0) {
            DestroyEntity(self);
            return;
        }
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        prim = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 8; i++) {
            D_pspeu_0927C160[i] = i << 9;
            prim->tpage = 0x10;
            prim->clut = dataPtr[3];
            prim->r0 = prim->b0 = prim->g0 = prim->r1 = prim->b1 = prim->g1 =
                prim->r2 = prim->b2 = prim->g2 = prim->r3 = prim->b3 =
                    prim->g3 = 0x80;
            prim->priority = MARIA.zPriority + 2;
            prim->drawMode = dataPtr[8] + DRAW_UNK_100 + DRAW_COLORS;
            prim = prim->next;
        }
        self->ext.playerBlink.unk8A = dataPtr[9];
        self->ext.playerBlink.unk90 = 0;
        self->step += 1;
        break;
    case 1:
        self->ext.playerBlink.unk90 += 0xA;
        if (self->ext.playerBlink.unk90 > 0x100) {
            self->ext.playerBlink.unk90 = 0x100;
            self->ext.playerBlink.unk80 = dataPtr[7];
            self->step += 1;
        }
        break;
    case 2:
        if (dataPtr[7] >= 0x7000) {
            switch ((u32)dataPtr[7]) {
            case 0x7000:
                if (g_Maria.timers[PL_T_POISON] == 0 ||
                    MARIA.step == PL_S_DEAD || MARIA.step == 0x60 ||
                    MARIA.step == 0x70) {
                    self->step++;
                }
                break;
            case 0x7001:
                if (g_Maria.timers[PL_T_INVINCIBLE_SCENE] == 0) {
                    self->step++;
                }
                break;
            case 0x7007:
            case 0x7002:
                if (MARIA.step != PL_S_HIT) {
                    self->step++;
                }
                break;
            case 0x7005:
            case 0x7006:
                if (MARIA.step_s == 3) {
                    self->step++;
                }
                break;
            case 0x7003:
            case 0x7004:
            case 0x7008:

                break;
            }
            self->ext.playerBlink.unk80 = 8;
        }
        if (--self->ext.playerBlink.unk80 == 0) {
            self->step++;
        }
        break;
    case 3:
        self->ext.playerBlink.unk90 -= 10;
        if (self->ext.playerBlink.unk90 < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    self->ext.playerBlink.unk82 += self->ext.playerBlink.unk8A;
    if (self->facingLeft) {
        selfX = selfX - xPivot;
    } else {
        selfX = selfX + xPivot;
    }
    selfY = selfY + yPivot;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 8; i++) {
        if (upperParams & 0x40) {
            switch (i) {
            case 0:
                if (self->facingLeft) {
                    prim->x0 = selfX;
                    prim->x1 = selfX - width / 2;
                    prim->u0 = xMargin;
                    prim->u1 = xMargin + width / 2;
                } else {
                    prim->x0 = selfX;
                    prim->x1 = selfX + width / 2;
                    prim->u0 = xMargin;
                    prim->u1 = xMargin + width / 2;
                }
                prim->y0 = prim->y1 = selfY;
                prim->v0 = prim->v1 = yMargin;
                break;
            case 1:
                if (self->facingLeft) {
                    prim->x0 = selfX - width / 2;
                    prim->x1 = selfX - width;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin + width;
                } else {
                    prim->x0 = selfX + width / 2;
                    prim->x1 = selfX + width;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin + width;
                }
                prim->y0 = prim->y1 = selfY;
                prim->v0 = prim->v1 = yMargin;
                break;
            case 2:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX - width;
                    prim->u0 = prim->u1 = xMargin + width;
                } else {
                    prim->x0 = prim->x1 = selfX + width;
                    prim->u0 = prim->u1 = xMargin + width;
                }
                prim->y0 = selfY;
                prim->y1 = selfY + height / 2;
                prim->v0 = yMargin;
                prim->v1 = yMargin + height / 2;
                break;
            case 3:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX - width;
                    prim->u0 = prim->u1 = xMargin + width;
                } else {
                    prim->x0 = prim->x1 = selfX + width;
                    prim->u0 = prim->u1 = xMargin + width;
                }

                prim->y0 = selfY + height / 2;
                prim->y1 = selfY + height;
                prim->v0 = yMargin + height / 2;
                prim->v1 = yMargin + height;
                break;
            case 4:
                if (self->facingLeft) {
                    prim->x0 = selfX - width;
                    prim->x1 = selfX - width / 2;
                    prim->u0 = xMargin + width;
                    prim->u1 = xMargin + width / 2;
                } else {
                    prim->x0 = selfX + width;
                    prim->x1 = selfX + width / 2;
                    prim->u0 = xMargin + width;
                    prim->u1 = xMargin + width / 2;
                }
                prim->y0 = prim->y1 = selfY + height;
                prim->v0 = prim->v1 = yMargin + height;
                break;
            case 5:
                if (self->facingLeft) {
                    prim->x0 = selfX - width / 2;
                    prim->x1 = selfX;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin;
                } else {
                    prim->x0 = selfX + width / 2;
                    prim->x1 = selfX;
                    prim->u0 = xMargin + width / 2;
                    prim->u1 = xMargin;
                }
                prim->y0 = prim->y1 = selfY + height;
                prim->v0 = prim->v1 = yMargin + height;
                break;
            case 6:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                } else {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                }
                prim->y0 = selfY + height;
                prim->y1 = selfY + height / 2;
                prim->v0 = yMargin + height;
                prim->v1 = yMargin + height / 2;
                break;
            case 7:
                if (self->facingLeft) {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                } else {
                    prim->x0 = prim->x1 = selfX;
                    prim->u0 = prim->u1 = xMargin;
                }
                prim->y0 = selfY + height / 2;
                prim->y1 = selfY;
                prim->v0 = yMargin + height / 2;
                prim->v1 = yMargin;
                break;
            }
            if (self->facingLeft) {
                prim->x2 = prim->x3 =
                    selfX - width / 2 +
                    (((rcos(self->ext.playerBlink.unk82) >> 4) * 3) >> 0xC);
            } else {
                prim->x2 = prim->x3 =
                    selfX + width / 2 +
                    (((rcos(self->ext.playerBlink.unk82) >> 4) * 3) >> 0xC);
            }
            prim->y2 = prim->y3 =
                (selfY + height / 2) -
                ((rsin(self->ext.playerBlink.unk82) >> 4) * 3 << 1 >> 8);
            prim->u2 = prim->u3 = xMargin + width / 2;
            prim->v2 = prim->v3 = yMargin + height / 2;
        } else {
            if (self->facingLeft) {
                prim->x0 = prim->x2 = (selfX - width) + 1;
                prim->x1 = prim->x3 = selfX + 1;
            } else {
                prim->x0 = prim->x2 = selfX;
                prim->x1 = prim->x3 = selfX + width;
            }
            prim->y0 = prim->y1 = selfY + height * i / 8;
            prim->y2 = prim->y3 = selfY + height * (i + 1) / 8;
            if (self->facingLeft) {
                prim->u0 = prim->u2 = wSprite - 1;
                prim->u1 = prim->u3 = xMargin - 1;
            } else {
                prim->u0 = prim->u2 = xMargin;
                prim->u1 = prim->u3 = wSprite;
            }
            prim->v0 = prim->v1 = yMargin + height * i / 8;
            prim->v2 = prim->v3 = yMargin + height * (i + 1) / 8;
        }
        angleRedIndex = dataPtr[0];
        angleGreenIndex = dataPtr[2];
        angleBlueIndex = dataPtr[1];
        redDivide = dataPtr[4];
        greenDivide = dataPtr[6];
        blueDivide = dataPtr[5];
        if (upperParams & 0x40) {
            angle = D_pspeu_0927C160[(i + angleRedIndex) % 8];
            prim->r0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / redDivide;
            angle = D_pspeu_0927C160[(i + angleGreenIndex) % 8];
            prim->g0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / greenDivide;
            angle = D_pspeu_0927C160[(i + angleBlueIndex) % 8];
            prim->b0 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / blueDivide;
            angle = D_pspeu_0927C160[(i + angleRedIndex + 1) % 8];
            prim->r1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / redDivide;
            angle = D_pspeu_0927C160[(i + angleGreenIndex + 1) % 8];
            prim->g1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / greenDivide;
            angle = D_pspeu_0927C160[(i + angleBlueIndex + 1) % 8];
            prim->b1 = ((rsin(angle) + 0x1000) >> 6) *
                       self->ext.playerBlink.unk90 / blueDivide;
            prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 = prim->b3 = 0;
            D_pspeu_0927C160[i] += self->ext.playerBlink.unk8A;
        } else {
            angle = D_pspeu_0927C160[(i + angleRedIndex) % 8];
            prim->r0 = prim->r1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / redDivide);
            angle = D_pspeu_0927C160[(i + angleGreenIndex) % 8];
            prim->g0 = prim->g1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / greenDivide);
            angle = D_pspeu_0927C160[(i + angleBlueIndex) % 8];
            prim->b0 = prim->b1 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / blueDivide);
            angle = D_pspeu_0927C160[(i + angleRedIndex + 1) % 8];
            prim->r2 = prim->r3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / redDivide);
            angle = D_pspeu_0927C160[(i + angleGreenIndex + 1) % 8];
            prim->g2 = prim->g3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / greenDivide);
            angle = D_pspeu_0927C160[(i + angleBlueIndex + 1) % 8];
            prim->b2 = prim->b3 = (((rsin(angle) + 0x1000) >> 6) *
                                   self->ext.playerBlink.unk90 / blueDivide);
            D_pspeu_0927C160[i] += self->ext.playerBlink.unk8A;
        }
        prim->priority = MARIA.zPriority + 2;
        prim = prim->next;
    }
    if ((upperParams & 0x3F) == 0 || (upperParams & 0x3F) == 7) {
        func_pspeu_0925B4E0(1, 10);
    }
}

void func_pspeu_0924D7F0(Entity* self) {
    Primitive* prim;
    s16 primIndex;

    self->posX.i.hi = MARIA.posX.i.hi - 0;
    self->posY.i.hi = MARIA.posY.i.hi - 8;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->ext.circleExpand.width = 16;
        self->ext.circleExpand.height = 12;
        prim = &g_PrimBuf[self->primIndex];
        prim->u0 = prim->u2 = 64;
        prim->v0 = prim->v1 = 192;
        prim->u1 = prim->u3 = 127;
        prim->v2 = prim->v3 = 255;
        PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) = 128;
        prim->tpage = 0x1A;
        prim->clut = PAL_CC_FIRE_EFFECT;
        prim->priority = MARIA.zPriority + 8;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        self->flags =
            FLAG_UNK_10000000 | FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->step++;
        break;

    case 1:
        self->ext.circleExpand.width += 2;
        self->ext.circleExpand.height += 2;
        if (self->ext.circleExpand.width > 56) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[self->primIndex];
    prim->x0 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y0 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x1 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y1 = self->posY.i.hi - self->ext.circleExpand.height;
    prim->x2 = self->posX.i.hi - self->ext.circleExpand.width;
    prim->y2 = self->posY.i.hi + self->ext.circleExpand.height;
    prim->x3 = self->posX.i.hi + self->ext.circleExpand.width;
    prim->y3 = self->posY.i.hi + self->ext.circleExpand.height;
    if (prim->b3 >= 12) {
        prim->b3 -= 12;
    }
    // remember last element of PGREY(prim,3) is prim->b3
    PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3);
}

extern s16 D_pspeu_0927A1A8[][6];
void MarEntityShrinkingPowerUpRing(Entity* self) {
    s16 variant;
    s16 rMod;
    s16* loadedParams;
    s16 x;
    s16 gMod;
    s16 bMod;
    s16 gOffset;
    s16 bOffset;
    s16 angleStart;
    s16 y;
    s16 baseAngle;
    s32 i;
    s16 angle;
    Primitive* prim1;
    Primitive* prim2;

    variant = (self->params & 0x7F00) >> 8;
    loadedParams = D_pspeu_0927A1A8[variant];
    rMod = loadedParams[2];
    gMod = loadedParams[3];
    bMod = loadedParams[4];
    gOffset = loadedParams[0];
    bOffset = loadedParams[1];
    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 32);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags =
            FLAG_UNK_10000000 | FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        prim2 = prim1 = &g_PrimBuf[self->primIndex];
        for (i = 0; i < 16; i++) {
            prim1 = prim1->next;
        }
        for (i = 0; i < 16; i++) {
            angle = i << 8;
            prim2->u0 = (rcos(angle) >> 4 << 5 >> 8) + 0x20;
            prim2->v0 = -(rsin(angle) >> 4 << 5 >> 8) + 0xDF;
            angle = (i + 1) << 8;
            prim2->u1 = (rcos(angle) >> 4 << 5 >> 8) + 0x20;
            prim2->v1 = -(rsin(angle) >> 4 << 5 >> 8) + 0xDF;
            prim1->u2 = prim1->u3 = 0x20;
            prim1->v2 = prim1->v3 = 0xDF;
            prim2->u2 = prim1->u0 = (prim2->u0 + prim1->u2) / 2;
            prim2->v2 = prim1->v0 = (prim2->v0 + prim1->v2) / 2;
            prim2->u3 = prim1->u1 = (prim2->u1 + prim1->u3) / 2;
            prim2->v3 = prim1->v1 = (prim2->v1 + prim1->v3) / 2;
            prim1->tpage = prim2->tpage = 0x1A;
            prim1->clut = prim2->clut = PAL_FILL_WHITE;
            prim1->priority = prim2->priority = MARIA.zPriority + 2;
            prim1->drawMode = prim2->drawMode =
                DRAW_UNK02 | DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                DRAW_TRANSP;
            prim2 = prim2->next;
            prim1 = prim1->next;
        }
        self->ext.ricPowerRing.unk80 = self->ext.ricPowerRing.unk82 = 0x280;
        self->ext.ricPowerRing.unk84 = self->ext.ricPowerRing.unk86 = 0x240;
        self->ext.ricPowerRing.unk8A = loadedParams[5];
        self->ext.ricPowerRing.unk88 = 0xC0;
        self->step++;
        break;
    case 1:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk86 -= 10;
        if (self->ext.ricPowerRing.unk86 < 0) {
            self->ext.ricPowerRing.unk86 = 0;
            self->ext.ricPowerRing.unk7C = 0x20;
            self->step++;
        }
        self->ext.ricPowerRing.unk84 = self->ext.ricPowerRing.unk86;
        self->ext.ricPowerRing.unk82 -= 5;
        self->ext.ricPowerRing.unk80 -= 5;
        break;
    case 2:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk82 -= 3;
        self->ext.ricPowerRing.unk80 -= 6;
        if (--self->ext.ricPowerRing.unk7C == 0) {
            self->step++;
        }
        break;
    case 3:
        self->ext.ricPowerRing.unk7E += 0x40;
        self->ext.ricPowerRing.unk82 -= 3;
        self->ext.ricPowerRing.unk80 -= 6;
        self->ext.ricPowerRing.unk88 -= 12;
        if (self->ext.ricPowerRing.unk88 < 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    angleStart = self->ext.ricPowerRing.unk8A;
    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim1 = prim2 = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 16; i++) {
        prim1 = prim1->next;
    }
    for (i = 0; i < 16; i++) {
        prim2->x0 =
            x + (prim2->u0 - 0x20) * self->ext.ricPowerRing.unk80 / 0x100;
        prim2->y0 =
            y + (prim2->v0 - 0xE0) * self->ext.ricPowerRing.unk82 / 0x100;
        prim2->x1 =
            x + (prim2->u1 - 0x20) * self->ext.ricPowerRing.unk80 / 0x100;
        prim2->y1 =
            y + (prim2->v1 - 0xE0) * self->ext.ricPowerRing.unk82 / 0x100;
        prim1->x2 = x + (rcos((i + 1) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk84 / 0x100;
        prim1->y2 = y - (rsin((i + 1) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk86 / 0x100;
        prim1->x3 = x + (rcos((i + 2) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk84 / 0x100;
        prim1->y3 = y - (rsin((i + 2) << 8) >> 4 << 5 >> 8) *
                            self->ext.ricPowerRing.unk86 / 0x100;
        prim2->x2 = prim1->x0 = (prim2->x0 + prim1->x2) / 2;
        prim2->y2 = prim1->y0 = (prim2->y0 + prim1->y2) / 2;
        prim2->x3 = prim1->x1 = (prim2->x1 + prim1->x3) / 2;
        prim2->y3 = prim1->y1 = (prim2->y1 + prim1->y3) / 2;
        baseAngle = i * angleStart;
        angle = self->ext.ricPowerRing.unk7E + baseAngle;
        prim1->r0 = prim2->r2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / rMod;
        angle = self->ext.ricPowerRing.unk7E + gOffset + baseAngle;
        prim1->g0 = prim2->g2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / gMod;
        angle = self->ext.ricPowerRing.unk7E + bOffset + baseAngle;
        prim1->b0 = prim2->b2 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / bMod;
        angle = self->ext.ricPowerRing.unk7E + angleStart + baseAngle;
        prim1->r1 = prim2->r3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / rMod;
        angle = self->ext.ricPowerRing.unk7E + gOffset + angleStart + baseAngle;
        prim1->g1 = prim2->g3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / gMod;
        angle = self->ext.ricPowerRing.unk7E + bOffset + angleStart + baseAngle;
        prim1->b1 = prim2->b3 =
            ((rsin(angle) + 0x1000) >> 7) * self->ext.ricPowerRing.unk88 / bMod;
        prim2->r0 = prim2->g0 = prim2->b0 = prim2->r1 = prim2->g1 = prim2->b1 =
            0;
        prim1->r2 = prim1->g2 = prim1->b2 = prim1->r3 = prim1->g3 = prim1->b3 =
            0;
        prim2 = prim2->next;
        prim1 = prim1->next;
    }
}

extern Point16* D_pspeu_0927A298[72];
void MarEntityHitByIce(Entity* self) {
    const int PrimCount = LEN(D_pspeu_0927A298) / 3;
    s16 x;
    s16 y;
    s32 i;
    bool terminateFlag;
    s16 deltaX;
    s16 deltaY;
    s16 yMod;
    s32 distance;
    s16 angle;
    Point16* pos;
    Primitive* prim;

    self->posX.i.hi = MARIA.posX.i.hi;
    self->posY.i.hi = MARIA.posY.i.hi;
    terminateFlag = 0;
    if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
        terminateFlag = 1;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT3, PrimCount);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags = FLAG_HAS_PRIMS | FLAG_POS_CAMERA_LOCKED;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = (rand() & 0x0F) + 0x30;
            prim->b0 = prim->b1 = prim->b2 = prim->b3 = (rand() & 0x7F) + 0x80;
            prim->g0 = prim->g1 = prim->g2 = prim->g3 = (rand() & 0x1F) + 0x30;
            if (rand() & 1) {
                prim->drawMode = DRAW_UNK02 | DRAW_UNK_100 | DRAW_TPAGE2 |
                                 DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            } else {
                prim->drawMode = DRAW_UNK02 | DRAW_UNK_100 | DRAW_TPAGE |
                                 DRAW_COLORS | DRAW_TRANSP;
            }
            prim->type = PRIM_G4;
            prim->priority = MARIA.zPriority + 2;
            prim = prim->next;
        }
        // Weird repeated conditional
        if (MARIA.velocityY != 0) {
            self->ext.hitbyice.unk7E = 1;
        }
        if (MARIA.velocityY != 0) {
            if (MARIA.facingLeft) {
                self->rotate = ROT(22.5);
            } else {
                self->rotate = ROT(-22.5);
            }
        } else {
            if (MARIA.velocityX > 0) {
                self->rotate = ROT(11.25);
            } else {
                self->rotate = ROT(348.75);
            }
        }
        if (MARIA.step == PL_S_DEAD) {
            if (MARIA.facingLeft) {
                self->rotate = ROT(33.75);
            } else {
                self->rotate = ROT(-33.75);
            }
            self->ext.hitbyice.unk80 = 1;
            self->ext.hitbyice.unk82 = 0x3C;
            if (self->params & 0x7F00) {
                self->ext.hitbyice.unk82 = 0x14;
            }
            self->ext.hitbyice.unk7E = 0;
        }
        self->step++;
        break;
    case 1:
        if (MARIA.step == PL_S_DEAD) {
            if ((MARIA.animCurFrame & 0x7FFF) == 0x21) {
                if (MARIA.facingLeft) {
                    self->rotate = ROT(56.25);
                } else {
                    self->rotate = ROT(-56.25);
                }
            }
            if ((MARIA.animCurFrame & 0x7FFF) == 0x22) {
                if (MARIA.facingLeft) {
                    self->rotate = ROT(78.75);
                } else {
                    self->rotate = ROT(-78.75);
                }
            }
            if ((MARIA.animCurFrame & 0x7FFF) == 0x20) {
                if (MARIA.facingLeft) {
                    self->rotate = ROT(33.75);
                } else {
                    self->rotate = ROT(-33.75);
                }
            }
        }
        if (self->ext.hitbyice.unk80 && --self->ext.hitbyice.unk82 == 0) {
            terminateFlag = true;
        }
        if (self->ext.hitbyice.unk7E &&
            g_Maria.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) {
            terminateFlag = true;
        }
        if (terminateFlag) {
            self->ext.hitbyice.unk7C = 0x40;
            if (self->ext.hitbyice.unk80) {
                self->ext.hitbyice.unk7C = 0x80;
            }
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbyice.unk7C == 0) {
            DestroyEntity(self);
            return;
        }
        break;
    }

    x = self->posX.i.hi;
    y = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < PrimCount; i++) {
        pos = D_pspeu_0927A298[i * 3];
        if (prim->u0 < 2) {
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x0 = x + deltaX;
            prim->y0 = y + deltaY;

            pos = D_pspeu_0927A298[i * 3 + 1];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x1 = x + deltaX;
            prim->y1 = y + deltaY;

            pos = D_pspeu_0927A298[i * 3 + 2];
            distance = SquareRoot12((pos->x * pos->x + pos->y * pos->y) << 0xC);
            angle = self->rotate + ratan2(pos->y, pos->x);
            deltaX = (((rcos(angle) >> 4) * distance) + 0x80000) >> 0x14;
            deltaY = (((rsin(angle) >> 4) * distance) + 0x80000) >> 0x14;
            prim->x2 = prim->x3 = x + deltaX;
            prim->y2 = prim->y3 = y + deltaY;
        }
        if (prim->u0 == 0 && terminateFlag) {
            prim->u0++;
            prim->v0 = (rand() & 15) + 1;
            if (self->ext.hitbyice.unk80) {
                prim->v0 = (rand() % 60) + 1;
            }
        }
        if (prim->u0 == 1) {
            if (--prim->v0 == 0) {
                prim->u0++;
                prim->v0 = 0x20;
                prim->u2 = 0xF0;
                if (self->ext.hitbyice.unk80) {
                    prim->v0 = (rand() & 31) + 0x28;
                }
            }
        }
        if (prim->u0 == 2) {
            if (prim->u2 < 0x70 || prim->u2 > 0xD0) {
                prim->u2 += 4;
            }
            yMod = (s8)prim->u2 >> 4;
            if (self->ext.hitbyice.unk80) {
                yMod = yMod >> 1;
            }
            prim->y0 += yMod;
            prim->y1 += yMod;
            prim->y2 += yMod;
            prim->y3 += yMod;
            if (prim->r3 < 4) {
                prim->r3 -= 4;
            }
            if (prim->g3 < 4) {
                prim->g3 -= 4;
            }
            if (prim->b3 < 4) {
                prim->b3 -= 4;
            }
            prim->r0 = prim->r1 = prim->r2 = prim->r3;
            prim->b0 = prim->b1 = prim->b2 = prim->b3;
            prim->g0 = prim->g1 = prim->g2 = prim->g3;
            prim->drawMode |= DRAW_UNK02;
            prim->drawMode &= ~(DRAW_UNK02 | DRAW_UNK_100);
            if (--prim->v0 == 0) {
                prim->drawMode |= DRAW_HIDE;
            }
        }
        prim = prim->next;
    }
}

extern u16 D_pspeu_0927A3B8[];
void MarEntityHitByLightning(Entity* self) {
    s16 x;
    s16 y;
    s16 mul2;
    s32 i;
    s32 xMod;
    s32 yMod;
    bool terminate;
    s32 mul;
    s16 angle;
    Primitive* primPrev;
    Primitive* prim;

    terminate = false;
    if ((self->params & 0xFF00) == 0x100) {
        if ((++self->ext.hitbylightning.unk9C) > 0xA8) {
            terminate = true;
        }
    } else if ((self->params & 0xFF00) == 0x200) {
        if (++self->ext.hitbylightning.unk9C > 0x90) {
            terminate = true;
        }
    } else if (MARIA.step != PL_S_HIT) {
        terminate = true;
    }
    switch (self->step) {
    case 0:
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 6);
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }

        self->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x3F);
        self->ext.hitbylightning.unk80 = (rand() & PSP_RANDMASK);
        self->ext.hitbylightning.unk82 = (rand() & 0x1FF) + 0x100;
        prim = &g_PrimBuf[self->primIndex];
        while (prim != NULL) {
            prim->x0 = prim->x1 = prim->x2 = prim->x3 = self->posX.i.hi;
            prim->y0 = prim->y1 = prim->y2 = prim->y3 = self->posY.i.hi;
            prim->tpage = 0x1A;
            prim->clut = D_pspeu_0927A3B8[rand() & 1];
            prim->priority = MARIA.zPriority - 2;
            prim->r0 = prim->g0 = prim->b0 = prim->r1 = prim->g1 = prim->b1 =
                prim->r2 = prim->g2 = prim->b2 = prim->r3 = prim->g3 =
                    prim->b3 = 0x80;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim = prim->next;
        }
        if ((MARIA.velocityY != 0) && (MARIA.step != PL_S_DEAD)) {
            self->ext.hitbylightning.unk92 = 1;
        }
        self->ext.hitbylightning.unk94 = 0x10;
        g_api.PlaySfx(SFX_PENTAGRAM_ATTACK);
        self->step++;
        break;
    case 1:
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0x1FF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) * 12;
        yMod = (-((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) * 7) << 1;
        self->posX.val = xMod + MARIA.posX.val;
        self->posY.val = yMod + MARIA.posY.val;
        if (self->ext.hitbylightning.unk92 &&
            g_Maria.vram_flag &
                (TOUCHING_L_WALL | TOUCHING_R_WALL | TOUCHING_CEILING)) {
            terminate = true;
        }
        if (terminate) {
            self->ext.hitbylightning.unk90 = (rand() & 0xF) + 0x10;
            self->step++;
        }
        break;
    case 2:
        if (--self->ext.hitbylightning.unk90 == 0) {
            DestroyEntity(self);
            return;
        }
        if (self->ext.hitbylightning.unk94 > 0) {
            self->ext.hitbylightning.unk94--;
        }
        self->ext.hitbylightning.unk7C =
            ((self->params & 0xF) << 9) + (rand() & 0xFF);
        mul = rsin(self->ext.hitbylightning.unk80);
        self->ext.hitbylightning.unk80 += self->ext.hitbylightning.unk82;
        xMod = ((rcos(self->ext.hitbylightning.unk7C) * mul) >> 7) *
               ((rand() % 8) + 8);
        yMod = -((rsin(self->ext.hitbylightning.unk7C) * mul) >> 7) *
                   ((rand() % 8) + 10) +
               self->ext.hitbylightning.unk98;
        self->posX.val = MARIA.posX.val + xMod;
        self->posY.val = MARIA.posY.val + yMod;
        self->ext.hitbylightning.unk98 -= 0x8000;
        prim = &g_PrimBuf[self->primIndex];
        break;
    }

    x = (self->posX.i.hi + (rand() & 7)) - 4;
    y = (self->posY.i.hi + (rand() & 0x1F)) - 0x18;
    mul2 = self->ext.hitbylightning.unk94;
    mul2 = (mul2 * rsin(self->ext.hitbylightning.unk80)) >> 0xC;
    prim = &g_PrimBuf[self->primIndex];
    for (i = 0; i < 5; i++) {
        primPrev = prim;
        prim = prim->next;
        *primPrev = *prim;
        primPrev->next = prim;
        primPrev->u0 = primPrev->u2 = (i * 0x10) + 0x90;
        primPrev->u1 = primPrev->u3 = ((i + 1) * 0x10) + 0x90;
        primPrev->v0 = primPrev->v1 = 0xC0;
        primPrev->v2 = primPrev->v3 = 0xCF;
    }
    prim->x0 = prim->x1;
    prim->y0 = prim->y1;
    prim->x2 = prim->x3;
    prim->y2 = prim->y3;
    angle = self->ext.hitbylightning.unk7C + 0x400;
    prim->x1 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y1 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk7C - 0x400;
    prim->x3 = x + (((rcos(angle) >> 4) * mul2) >> 8);
    prim->y3 = y - (((rsin(angle) >> 4) * mul2) >> 8);
    angle = self->ext.hitbylightning.unk80 & 0xFFF;
    if (angle < 0x400) {
        prim->priority = MARIA.zPriority + 2;
    } else if (angle < 0xC00) {
        prim->priority = MARIA.zPriority - 2;
    } else {
        prim->priority = MARIA.zPriority + 2;
    }
    prim->u0 = prim->u2 = (i << 4) + 0x90;
    prim->u1 = prim->u3 = ((i + 1) << 4) + 0x90;
    prim->v0 = prim->v1 = 0xC0;
    prim->v2 = prim->v3 = 0xCF;
}

extern EInit D_pspeu_0926EC28;
extern AnimationFrame D_pspeu_0927A3C0[];
extern s32 D_pspeu_0927C1C8;
extern s32 D_pspeu_0927C1C0;
extern s32 D_pspeu_0927C1A0[8];
void func_pspeu_0924FFB8(Entity* self) {
    Primitive* prim; // s0
    s32 i;           // s1

    u16 palette;   // 0x6E(sp)
    s32 temp_s0_5; // 0x68(sp)
    s32 temp_v0_7; // 0x64(sp)
    s32 var_v1_2;  // 0x60(sp)
    s32 var_a0_2;  // 0x5C(sp)

    s32 distanceX; // 0x58(sp)
    s32 distanceY; // 0x54(sp)

    s32 scale;   // 0x50(sp)
    s32 posX;    // 0x4C(sp)
    s32 posY;    // 0x48(sp)
    s32 ricPosX; // 0x44(sp)
    s32 ricPosY; // s8

    long anotherX;
    s32 anotherY;

    s32 var_s4;    // 0x38(sp)
    s32 sp30;      // 0x34(sp)
    s32 j;         // 0x30(sp)
    s32 angle;     // s7
    s32 distance;  // s6
    s32 direction; // s5
    s32 primX;     // s4
    s32 primY;     // s3
    s32 temp_s0_2; // s2

    scale = 4;
    D_pspeu_0927C1C8 = 0;
    var_s4 = 0;
    sp30 = 0;

    if (self->flags & FLAG_DEAD) {
        if (self->step < 0x14) {
            D_pspeu_0927C1C8 = 1;
            self->step = 0x14;
        }
    } else {
        if ((self->hitFlags) && (self->step != 10)) {
            self->ext.et_801C03E8.hitTimer = 10;
            self->step = 0xA;
        }
        self->hitFlags = 0;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_0926EC28);
        self->primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x20);

        if (self->primIndex == -1) {
            self->step = 0;
            return;
        }
        prim = &g_PrimBuf[self->primIndex];

        for (i = 0; i < 8; i++) {
            prim->clut = 0x252;
            prim->tpage = 0x12;
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x1F;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0x1F;
            prim->priority = MARIA.zPriority + 4;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                             DRAW_HIDE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
            D_pspeu_0927C1A0[i] = 0;
            prim = prim->next;
        }

        for (i = 0; i < 24; i++) {
            prim->priority = MARIA.zPriority - 2;
            prim->r0 = prim->g0 = prim->r1 = prim->g1 = 0x3F;
            prim->b0 = prim->b1 = 0x7F;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            prim->type = PRIM_LINE_G2;
            prim = prim->next;
        }

        self->flags |= FLAG_UNK_20000000 | FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        self->posX.i.hi = 0x80;
        self->posY.i.hi = 0x30;
        self->ext.et_801C03E8.orbitAngle = 0x400;
        self->ext.et_801C03E8.orbitAngleStep = 0x10;
        self->ext.et_801C03E8.maxTurnRate = 0x30;
        self->ext.et_801C03E8.moveAngle = 0xC00;
        self->animSet = ANIMSET_OVL(5);
        self->animCurFrame = 0;
        self->unk5A = 0x48;
        self->palette = PAL_FLAG(0x252);
        self->ext.et_801C03E8.savedHitboxState = self->hitboxState;
        self->anim = D_pspeu_0927A3C0;
        self->zPriority = MARIA.zPriority + 4;
        self->step = 1;
        self->opacity = 0;
        self->drawFlags = ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        break;

    case 1:
        self->opacity++;
        if (self->opacity >= 0x80) {
            self->drawFlags = ENTITY_DEFAULT;
            self->blendMode = BLEND_NO;
            self->step++;
        }
        break;

    case 2:
        distanceX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        distanceY = (MARIA.posY.i.hi + MARIA.hitboxOffY) - 0x40;

        angle = self->ext.et_801C03E8.orbitAngle;

        distanceX += (((rcos(angle) >> 4) * scale) >> 8);
        distanceY -= (((rsin(angle) >> 4) * scale) >> 8);
        self->ext.et_801C03E8.orbitAngle +=
            self->ext.et_801C03E8.orbitAngleStep;

        posX = distanceX - self->posX.i.hi;
        posY = distanceY - self->posY.i.hi;
        angle = ratan2(-posY, posX) & 0xFFF;

        temp_s0_2 = (self->ext.et_801C03E8.moveAngle & 0xFFF);
        var_v1_2 = abs(temp_s0_2 - angle);

        var_a0_2 = self->ext.et_801C03E8.maxTurnRate;
        if (self->ext.et_801C03E8.maxTurnRate > var_v1_2) {
            var_a0_2 = var_v1_2;
        }

        if (temp_s0_2 < angle) {
            if (var_v1_2 < 0x800) {
                temp_s0_2 += var_a0_2;
            } else {
                temp_s0_2 -= var_a0_2;
            }
        } else {
            if (var_v1_2 < 0x800) {
                temp_s0_2 -= var_a0_2;
            } else {
                distance = var_a0_2;
                temp_s0_2 += distance;
            }
        }
        self->ext.et_801C03E8.moveAngle = temp_s0_2 & 0xFFF;
        temp_s0_5 = rcos(temp_s0_2) * 0x10;
        temp_v0_7 = rsin(temp_s0_2) * 0x10;
        self->posX.val += (long)temp_s0_5;
        self->posY.val -= temp_v0_7;
        break;
    case 10:
        if (g_Timer & 1) {
            self->palette = PAL_FLAG(PAL_FILL_WHITE);
        } else {
            self->palette = PAL_FLAG(PAL_CC_GREEN_EFFECT_A);
        }

        self->poseTimer++;
        if (--self->ext.et_801C03E8.hitTimer == 0) {
            self->step = 2;
        }

        break;
    case 20:
        MarCreateEntFactoryFromEntity(self, 0x49, 0);
        MarCreateEntFactoryFromEntity(self, 0x4B, 0);
        self->step++;
        break;
    case 21:
        DestroyEntity(self);
        return;
    }

    if (g_api.CheckEquipmentItemCount(0x22U, 1U) != 0) {
        palette = PAL_FLAG(0x252);
        self->hitboxState = self->ext.et_801C03E8.savedHitboxState;
        self->ext.et_801C03E8.showPrims = 1;
    } else {
        palette = PAL_FLAG(PAL_PLAYER_HIDDEN);
        self->hitboxState = 0;
        self->ext.et_801C03E8.showPrims = 0;
    }
    if (MARIA.step == PL_S_DEAD || MARIA.step == PL_S_ENDING_1) {
        self->hitboxState = 0;
    }
    if (self->step != PL_S_9) {
        self->palette = palette;
    }
    if (!(g_Timer % 4) && (self->step == 2)) {
        D_pspeu_0927C1C0++;
        D_pspeu_0927C1C0 %= 8;
        var_s4 = 1;
    }

    if (g_Timer % 0x100 == 0) {
        if (self->step == 2) {
            if ((abs(self->posX.i.hi - MARIA.posX.i.hi) < 0x20) &&
                (MARIA.step != PL_S_DEAD)) {
                sp30 = 1;
                if (self->ext.et_801C03E8.showPrims) {
                    MarCreateEntFactoryFromEntity(self, 0x590021, 0);
                }
            }
        }
    }

    posX = self->posX.i.hi;
    posY = self->posY.i.hi;
    prim = &g_PrimBuf[self->primIndex];

    for (i = 0; i < 8; i++) {
        if (D_pspeu_0927C1A0[i] == 0) {
            if ((var_s4 != 0) && (D_pspeu_0927C1C0 == i)) {
                prim->x0 = prim->x2 = posX - 0x10;
                prim->x1 = prim->x3 = posX + 0xF;
                prim->y0 = prim->y1 = posY - 0x10;
                prim->y2 = prim->y3 = posY + 0xF;
                prim->drawMode &= ~DRAW_HIDE;
                prim->r0 = prim->r1 = prim->r2 = prim->r3 = prim->g0 =
                    prim->g1 = prim->g2 = prim->g3 = prim->b0 = prim->b1 =
                        prim->b2 = prim->b3 = 0x80;
                D_pspeu_0927C1A0[i] += 1;
            }
        } else {
            prim->b3 -= 4;
            if (prim->b3 < 0x10) {
                D_pspeu_0927C1A0[i] = 0;
            }
            prim->r0 = prim->r1 = prim->r2 = prim->r3 = prim->g0 = prim->g1 =
                prim->g2 = prim->g3 = prim->b0 = prim->b1 = prim->b2 = prim->b3;
        }
        if (!self->ext.et_801C03E8.showPrims) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = prim->next;
    }

    ricPosX = MARIA.posX.i.hi;
    ricPosY = MARIA.posY.i.hi;

    if (g_Timer & 1) {
        direction = -1;
    } else {
        direction = 1;
    }
    posX -= 3;

    for (j = 0; j < 3; j++, posX += 3) {
        primX = posX;
        primY = posY;
        distance = 3;
        // > lw a1, 0x48(sp)
        anotherY = ricPosY - posY;
        anotherX = ricPosX - posX;
        angle = ratan2(-anotherY, anotherX);
        distance = (SquareRoot12(I_TO_FLT(
                        (anotherX * anotherX) + (anotherY * anotherY))) /
                    7);
        distance = FLT_TO_I(distance);

        for (i = 0; i < 8; i++) {
            direction = -direction;
            if (prim->r2 == 0) {
                if (sp30 != 0) {
                    prim->r2++;
                    prim->b2 = 0xC;
                    prim->drawMode &= ~DRAW_HIDE;
                }
            } else if (--prim->b2 == 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->r2 = 0;
            }
            prim->x0 = primX;
            prim->y0 = primY;
            temp_s0_2 = angle + (rand() & 0x1FF) * direction;
            prim->x1 = (((rcos(temp_s0_2) >> 4) * distance) >> 8) + primX;
            prim->y1 = -(((rsin(temp_s0_2) >> 4) * distance) >> 8) + primY;
            primX = prim->x1;
            primY = prim->y1;

            if (i == 7) {
                prim->x1 = ricPosX;
                prim->y1 = ricPosY;
            }
            if (!self->ext.et_801C03E8.showPrims) {
                prim->drawMode |= DRAW_HIDE;
            }
            prim = prim->next;
        }
    }
}


