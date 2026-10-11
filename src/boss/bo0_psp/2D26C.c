// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo0.h"

extern EInit g_EInitOlroxAfterImage;
extern u8 D_pspeu_0928FAC0[];
extern s32 E_ID(UNK_2F);
extern EInit D_pspeu_09275F60;
extern EInit D_pspeu_09275F80;
extern EInit D_pspeu_09275F50;
extern u8 D_pspeu_0928FA78[];
extern u8 D_pspeu_0928FAA8[];
extern u8 D_pspeu_0928FAB8[];
extern s32 D_pspeu_09290798;
extern s16 D_pspeu_0928FAF0[];
extern s32 E_ID(ID_2A);

s16 func_pspeu_09258DE0(s16 delta, s16 current, s16 target) {
    s16 diff;

    current &= 0xFFF;
    target &= 0xFFF;
    diff = target - current;
    if (diff > ROT(180)) {
        diff = diff - ROT(360);
    }
    if (diff < ROT(-180)) {
        diff = diff + ROT(360);
    }
    if (diff < -delta) {
        diff = -delta;
    }
    if (diff > delta) {
        diff = delta;
    }
    target = current + diff;
    target &= 0xFFF;
    return target;
}

s32 func_pspeu_09258EF0(s16 x, s16 y) {
    Collider collider;

    g_api.CheckCollision(x, y, &collider, 0);
    if (collider.effects & EFFECT_SOLID) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_us_801AD338);

INCLUDE_ASM("boss/bo0_psp/nonmatchings/bo0_psp/2D26C", func_pspeu_0925B1B0);

void func_pspeu_0925C178(Entity* self) {
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F80);
        if (self->facingLeft) {
            self->posX.i.hi += 0x1C;
        } else {
            self->posX.i.hi -= 0x1C;
        }
        self->posY.i.hi -= 0xC;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 1);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.et_801AF31C.prim = prim;
            prim->tpage = 0x12;
            prim->clut = 0x212;
            prim->u0 = prim->u2 = 0xA0;
            prim->u1 = prim->u3 = 0xD0;
            prim->v0 = prim->v1 = 0;
            prim->v2 = prim->v3 = 0x10;
            prim->x0 = prim->x2 = self->posX.i.hi;
            prim->x1 = prim->x3 = prim->x0;
            prim->y0 = prim->y1 = self->posY.i.hi;
            prim->y2 = prim->y3 = prim->y0;
            PGREY(prim, 0) = 0;
            LOW(prim->r2) = LOW(prim->r0);
            PGREY(prim, 1) = 0xA0;
            LOW(prim->r3) = LOW(prim->r1);
            prim->priority = self->zPriority + 4;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
        } else {
            DestroyEntity(self);
            return;
        }
        self->velocityY = 0;
        if (self->facingLeft) {
            self->velocityX = FIX(-0.5);
            self->ext.et_801AF31C.unkA8.i.hi = -0x30;
        } else {
            self->velocityX = FIX(0.5);
            self->ext.et_801AF31C.unkA8.i.hi = 0x30;
        }
        self->hitboxWidth = 0;
        self->hitboxHeight = 5;
        PlaySfxPositional(SFX_SCIFI_BLAST);
        break;
    case 1:
        MoveEntity();
        if (self->facingLeft) {
            self->velocityX -= FIX(0.125);
            self->ext.et_801AF31C.unkA8.val += FIX(-5);
        } else {
            self->velocityX += FIX(0.125);
            self->ext.et_801AF31C.unkA8.val += FIX(5);
        }
        prim = self->ext.et_801AF31C.prim;
        prim->x1 = prim->x3 =
            self->posX.i.hi + self->ext.et_801AF31C.unkA8.i.hi;
        prim->x0 = prim->x2 = self->posX.i.hi;
        prim->y0 = prim->y1 = self->posY.i.hi - 8;
        prim->y2 = prim->y3 = prim->y0 + 0x10;
        self->hitboxWidth = abs(self->ext.et_801AF31C.unkA8.i.hi) / 4;
        self->hitboxOffX =
            abs(self->ext.et_801AF31C.unkA8.i.hi) - self->hitboxWidth - 4;
        break;
    }
    if (abs(self->posX.i.hi) > 0x280) {
        DestroyEntity(self);
    }
}

void func_pspeu_0925C580(Entity* self) {
    Entity* entity;
    s16 angle;
    Entity* player;
    s16 x;
    s16 y;

    if (D_pspeu_09290798) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        PlaySfxPositional(SFX_EXPLODE_B);
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity) {
            CreateEntityFromEntity(E_ID(LASER_EXPLOSION), self, entity);
            entity->params = 2;
        }
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F60);
        self->hitboxState = 0;
        self->drawFlags |= ENTITY_OPACITY;
        self->hitboxOffY = -1;
        self->ext.et_801AF8C0.unkAA = ((Random() & 0x3F) * 0x10) + ROT(45);
        if (self->params) {
            self->step = 2;
            self->zPriority--;
            self->hitboxState = 0;
            self->blendMode |= BLEND_TRANSP | BLEND_ADD;
            break;
        }
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity) {
            CreateEntityFromEntity(E_ID(UNK_2F), self, entity);
            entity->params = 1;
            entity->ext.et_801AF8C0.parent = self;
        }
        break;
    case 1:
        if (self->opacity < 0x80) {
            self->opacity += 4;
            self->hitboxState = 0;
        } else {
            self->hitboxState = 3;
        }
        AnimateEntity(D_pspeu_0928FAB8, self);
        MoveEntity();
        if (self->velocityX > 0) {
            self->facingLeft = 1;
        } else {
            self->facingLeft = 0;
        }
        player = &PLAYER;
        x = player->posX.i.hi - self->posX.i.hi;
        y = player->posY.i.hi - self->posY.i.hi;
        y -= 0x20;
        angle = ratan2(-y, x);
        angle = func_pspeu_09258DE0(8, self->ext.et_801AF8C0.unkAA, angle);
        self->velocityX = rcos(angle) * 16;
        self->velocityY = -rsin(angle) * 16;
        self->ext.et_801AF8C0.unkAA = angle;
        break;
    case 2:
        AnimateEntity(D_pspeu_0928FAA8, self);
        entity = self->ext.et_801AF8C0.parent;
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        self->opacity = entity->opacity;
        if (!entity->entityId) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_pspeu_0925C938(Entity* self) {
    Entity* entity;

    entity = self->ext.et_801AF8C0.parent;
    if (entity->flags & FLAG_DEAD) {
        self->flags |= FLAG_DEAD;
        self->step = 3;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitOlroxAfterImage);
        self->animCurFrame = 0x54;
        self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0;
        self->zPriority--;
        self->hitboxState = 0;
        break;
    case 1:
        AnimateEntity(D_pspeu_0928FAC0, self);
        if (self->scaleX < 0x100) {
            self->scaleX = self->scaleY += 8;
        } else {
            self->scaleX = self->scaleY = 0x100;
            self->step++;
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_0928FAC0, self);
        if (!(g_Timer % 12)) {
            entity = AllocEntity(&g_Entities[160], &g_Entities[168]);
            if (entity) {
                CreateEntityFromEntity(E_ID(UNK_2F), self, entity);
                entity->ext.et_801AF8C0.parent = self;
                PlaySfxPositional(SFX_SMALL_FLAME_IGNITE);
            }
        }
        if (g_Timer % 5) {
            self->palette = PAL_FLAG(0x213);
        } else {
            self->palette = PAL_FLAG(0x215);
        }
        entity = self->ext.et_801AF8C0.parent;
        if (entity->ext.et_801AD338.unk85) {
            self->step++;
        }
        break;
    case 3:
        AnimateEntity(D_pspeu_0928FAC0, self);
        self->scaleX = self->scaleY -= 8;
        if (self->scaleX < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_pspeu_0925CBE8(Entity* self) {
    Entity* entity;
    s16 y;
    s32 radius;
    s16 x;
    s16 angle;
    u8 angle2;
    s32 dx;
    s32 dy;
    s32 oldX;

    entity = self->ext.et_801AF8C0.parent;
    if (entity->flags & FLAG_DEAD) {
        self->flags |= FLAG_DEAD;
    }
    if (self->flags & FLAG_DEAD) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 2;
        }
        DestroyEntity(self);
        PlaySfxPositional(SFX_BAT_SCREECH);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_pspeu_09275F50);
        self->hitboxState = 0;
        self->hitboxOffX = 1;
        self->hitboxOffY = 1;
        entity = self->ext.et_801AF8C0.parent;
        self->zPriority = entity->zPriority + 2;
        self->palette = PAL_BREAKABLE;
        self->pose = (Random() & 3) * 2;
        self->ext.et_801AF8C0.unk80 = (self->params + 1) * 14;
        self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY;
        self->params = Random() & 3;
        break;
    case 1:
        AnimateEntity(D_pspeu_0928FA78, self);
        oldX = self->posX.i.hi;
        entity = self->ext.et_801AF8C0.parent;
        x = entity->posX.i.hi;
        y = entity->posY.i.hi;
        y -= 0x20;
        dx = self->posX.i.hi - x;
        dy = self->posY.i.hi - y;
        angle = ratan2(-dy, dx);
        if (entity->facingLeft) {
            angle += ROT(1.40625);
        } else {
            angle -= ROT(1.40625);
        }
        radius = dx * dx + dy * dy;
        radius = SquareRoot0(radius);
        radius -= 2;
        self->posX.i.hi = x + radius * rcos(angle) / 4096;
        self->posY.i.hi = y - radius * rsin(angle) / 4096;
        self->scaleX = radius * self->params + 0x100;
        if (self->scaleX > 0x300) {
            self->scaleX = 0x300;
        }
        self->scaleY = self->scaleX;
        oldX = self->posX.i.hi - oldX;
        if (oldX > 0) {
            self->facingLeft = 0;
        } else {
            self->facingLeft = 1;
        }
        if (radius < 8) {
            self->ext.et_801AF8C0.unkA8 = Random();
            self->palette = 0x209;
            self->hitboxState = 3;
            self->drawFlags = ENTITY_DEFAULT;
            self->step++;
        }
        break;
    case 2:
        entity = self->ext.et_801AF8C0.parent;
        AnimateEntity(D_pspeu_0928FA78, self);
        MoveEntity();
        if (self->velocityX > 0) {
            self->facingLeft = 1;
            self->zPriority = entity->zPriority + 2;
        } else {
            self->facingLeft = 0;
            self->zPriority = entity->zPriority - 2;
        }
        x = entity->posX.i.hi;
        y = entity->posY.i.hi;
        x = x - self->posX.i.hi;
        y = y - self->posY.i.hi;
        angle2 = Ratan2Shifted(x, y);
        self->ext.et_801AF8C0.unkA8 =
            AdjustValueWithinThreshold(2, self->ext.et_801AF8C0.unkA8, angle2);
        SetEntityVelocityFromAngle(self->ext.et_801AF8C0.unkA8, 0x10);
        if (entity->ext.et_801AD338.unk84) {
            if (!--self->ext.et_801AF8C0.unk80) {
                entity = &PLAYER;
                angle2 = GetAngleBetweenEntitiesShifted(self, entity);
                SetEntityVelocityFromAngle(angle2, 0x20);
                self->step++;
            }
        }
        break;
    case 3:
        AnimateEntity(D_pspeu_0928FA78, self);
        MoveEntity();
        if (self->velocityX > 0) {
            self->facingLeft = 0;
        } else {
            self->facingLeft = 1;
        }
        if (abs(self->posX.i.hi) > 0x200 || abs(self->posY.i.hi) > 0x200) {
            DestroyEntity(self);
        }
        break;
    }
}

void EntityOlroxAfterImage(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitOlroxAfterImage);
        self->hitboxState = 0;
        self->palette = 0x217;
        self->animCurFrame = self->params;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0x80;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
        self->zPriority -= 2;
    }
    self->opacity -= 2;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

void func_us_801B001C(Entity* self) {
    Entity* entity;
    s32 i;
    s32 x;
    s16* data;
    s32 y;
    s32 params;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->animSet = ANIMSET_OVL(6);
        self->unk5A = 0x48;
        self->palette = 0x209;
        self->hitboxState = 2;
        if (self->params & 0x100) {
            self->facingLeft = 1;
        }
        self->zPriority -= 4;
        if (self->params == 1) {
            D_pspeu_09290798 = 0;
        }
        params = self->params & 0xFF;
        self->animCurFrame = params;
        if (params < 5) {
            data = D_pspeu_0928FAF0;
            data += (params - 1) * 6;
            self->hitboxWidth = *data++;
            self->hitboxHeight = *data++;
            self->hitboxOffX = *data++;
            self->hitboxOffY = *data++;
            self->hitPoints = *data++;
            self->zPriority = *data++;
        } else {
            self->step += params;
        }
        break;
    case 1:
        if (self->flags & FLAG_DEAD) {
            self->hitboxState = 0;
            self->step += (self->params & 0xFF);
        }
        break;
    case 2:
        self->animCurFrame = 10;
        if (GetSideToPlayer() & 1) {
            self->velocityX = FIX(1);
        } else {
            self->velocityX = FIX(-1);
        }
        self->step = 11;
        break;
    case 3:
        for (i = 0; i < 4; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity) {
                CreateEntityFromEntity(E_ID(ID_2A), self, entity);
                entity->params = (i + 5);
                entity->posX.i.hi += (i * -0x1E + 0x30);
            }
        }
        if (entity) {
            entity->params = 0x105;
        }
        for (i = 0; i < 4; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity) {
                CreateEntityFromEntity(E_EXPLOSION, self, entity);
                entity->params = 0x13;
                entity->posX.i.hi += (i * -0x1E + 0x30);
            }
        }
        entity = self;
        entity++;
        for (i = 0; i < 3; i++, entity++) {
            entity->flags |= FLAG_DEAD;
        }
        self->animCurFrame = 0;
        self->step = 32;
        break;
    case 4:
        for (i = 0; i < 2; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity) {
                CreateEntityFromEntity(E_ID(ID_2A), self, entity);
                entity->params = (i + 8);
                entity->posY.i.hi += (i * 8);
                if (i) {
                    if (GetSideToPlayer() & 1) {
                        entity->velocityX = FIX(0.5);
                    } else {
                        entity->velocityX = FIX(-0.5);
                    }
                    entity->velocityY = FIX(-1);
                }
            }
        }
        PlaySfxPositional(SFX_SMALL_FLAME_IGNITE);
        self->animCurFrame = 0;
        self->step = 32;
        break;
    case 5:
        for (i = 0; i < 3; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity) {
                CreateEntityFromEntity(E_ID(ID_2A), self, entity);
                entity->params = (i + 11);
                entity->posY.i.hi += (i * 0x1C - 0x1C);
            }
        }
        PlaySfxPositional(SFX_CANDLE_HIT_WHOOSH_B);
        self->animCurFrame = 0;
        self->step = 32;
        break;
    case 6:
        self->zPriority = 0x6A;
        break;
    case 7:
    case 8:
        self->zPriority = 0x6A;
        MoveEntity();
        self->velocityY += FIX(0.0625);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        y += 8;
        if (func_pspeu_09258EF0(x, y)) {
            self->step = 32;
        }
        break;
    case 9:
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate -= ROT(1.40625);
        self->zPriority = 0x6C;
        MoveEntity();
        self->velocityY += FIX(0.09375);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        y += 4;
        if (func_pspeu_09258EF0(x, y)) {
            self->entityId = E_EXPLOSION;
            self->pfnUpdate = EntityExplosion;
            self->params = 0;
            self->step = 0;
        }
        break;
    case 10:
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate += ROT(11.25);
        self->zPriority = 0x6C;
        MoveEntity();
        self->velocityY += FIX(0.125);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        if (func_pspeu_09258EF0(x, y)) {
            self->entityId = E_EXPLOSION;
            self->pfnUpdate = EntityExplosion;
            self->step = 0;
            self->params = 0;
        }
        break;
    case 11:
        self->zPriority = 0x6A;
        MoveEntity();
        self->velocityY += FIX(0.0625);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        y += 10;
        if (func_pspeu_09258EF0(x, y)) {
            self->step = 32;
        }
        break;
    case 12:
        self->rotate -= ROT(4.21875);
    case 13:
        self->drawFlags |= ENTITY_ROTATE;
        self->rotate += ROT(3.515625);
        self->zPriority = 0x6B;
        MoveEntity();
        self->velocityY += FIX(0.09375);
        x = self->posX.i.hi;
        y = self->posY.i.hi;
        y += 6;
        if (func_pspeu_09258EF0(x, y)) {
            self->entityId = E_EXPLOSION;
            self->pfnUpdate = EntityExplosion;
            self->params = 0;
            self->step = 0;
        }
        break;
    case 14:
        self->zPriority = 0x6A;
        break;
    }
}
