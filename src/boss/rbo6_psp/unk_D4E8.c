// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"

// Extension used by the transformation effects; offsets verified in PSP ASM.
typedef struct {
    /* 0x7C */ s32 : 32;
    /* 0x80 */ s16 timer;
    /* 0x82 */ s16 : 16;
    /* 0x84 */ s32 : 32;
    /* 0x88 */ s32 : 32;
    /* 0x8C */ s32 speed;
} Rbo6TransformationEffect;

extern EInit g_EInitBreakable;
extern u8* D_pspeu_09256C38[];
extern u8 D_pspeu_09256C58[];
extern u16 D_pspeu_09256C60[];
extern u8 D_pspeu_0926C040[];
extern u8 D_pspeu_09256C70[];

void EntityBreakable(Entity* self) {
    Entity* entity;
    u16 breakableType;

    breakableType = self->params >> 12;
    if (self->step) {
        AnimateEntity(D_pspeu_09256C38[breakableType], self);
        if (self->hitParams) {
            g_api.PlaySfx(SFX_CANDLE_HIT);
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromCurrentEntity(E_EXPLOSION, entity);
                entity->params = D_pspeu_0926C040[breakableType];
            }
            ReplaceBreakableWithItemDrop(self);
        }
    } else {
        InitializeEntity(g_EInitBreakable);
        self->zPriority = g_unkGraphicsStruct.g_zEntityCenter - 20;
        self->blendMode = D_pspeu_09256C70[breakableType];
        self->hitboxHeight = D_pspeu_09256C58[breakableType];
        self->animSet = D_pspeu_09256C60[breakableType];
    }
}

s32 DecreaseBrightness(Primitive* prim, s32 brightnessOffset) {
    s32 newColor;
    s32 i;
    s32 j;
    u8* rgbVal;
    u8* rPtr;
    s32 ret;

    ret = 0;
    rPtr = &prim->r0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            rgbVal = &rPtr[j];
            newColor = *rgbVal;
            newColor += brightnessOffset;
            if (newColor > 0xFF) {
                newColor = 0xFF;
            } else {
                ret |= 1;
            }
            *rgbVal = newColor;
        }
        rPtr += 0xC;
    }
    return ret;
}

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A4594);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A4F14);

void func_us_801A367C(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;
    Tilemap* tilemap = &g_Tilemap;
    Entity* player = &PLAYER;
    s16 playerX = player->posX.i.hi + g_Tilemap.scrollX.i.hi;
    s16 posY = self->posY.i.hi + g_Tilemap.scrollY.i.hi;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        g_PauseAllowed = false;
        g_unkGraphicsStruct.pauseEnemies = true;
        player->facingLeft = 0;
        if (g_Status.wornEquipment[RIGHT_HAND_SLOT] != ITEM_MAGIC_MISSILE) {
            player->animCurFrame = 0xBF;
        }
        player->posX.i.hi = 0x70;
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;
    case 1:
        if (g_Status.wornEquipment[RIGHT_HAND_SLOT] != ITEM_MAGIC_MISSILE) {
            player->animCurFrame = 0xBF;
        }
        player->posX.i.hi = 0x70;
        if (g_CutsceneFlags & 2) {
            self->step++;
        }
        g_Player.padSim = PAD_NONE;
        g_Player.demo_timer = 1;
        break;
    case 2:
        g_PauseAllowed = true;
        g_unkGraphicsStruct.pauseEnemies = false;
        g_Entities[E_AFTERIMAGE_1].ext.afterImage.disableFlag = 1;
        DestroyEntity(self);
        break;
    }
}

void func_us_801A37B4(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        primIndex = (s16)g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            prim->x0 = prim->x2 = 0;
            prim->x1 = prim->x3 = 0x100;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0xF0;
            PCOL(prim) = 0xFF;
            prim->priority = 0x1F8;
            prim->drawMode =
                DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 1:
        self->step++;
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) -= 4;
        if (prim->r0 < 4) {
            DestroyEntity(self);
            g_CutsceneFlags |= 1;
        }
        break;
    }
}

void func_us_801A399C(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        primIndex = (s16)g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            prim->x0 = prim->x2 = 0;
            prim->x1 = prim->x3 = 0x100;
            prim->y0 = prim->y1 = 0;
            prim->y2 = prim->y3 = 0xF0;
            PCOL(prim) = 0;
            prim->priority = 0x80;
            prim->drawMode = DRAW_DEFAULT;
        }
        break;
    case 1:
        if (g_CutsceneFlags & 0x100) {
            prim = &g_PrimBuf[self->primIndex];
            PCOL(prim) = 0xFF;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            self->step++;
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) -= 4;
        if (prim->r0 < 4) {
            DestroyEntity(self);
        }
        break;
    }
}


void func_us_801A3BE0(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        primIndex = (s16)g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            prim->x0 = prim->x2 = 0x60;
            prim->x1 = prim->x3 = 0xA0;
            prim->y0 = prim->y1 = 0x40;
            prim->y2 = prim->y3 = 0xC0;
            PCOL(prim) = 0xFF;
            prim->drawMode = DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            prim->priority = 0x82;
        }
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        if (g_CutsceneFlags & 4) {
            PCOL(prim) -= 1;
            if (prim->r0 <= 0) {
                PCOL(prim) = 0;
                g_CutsceneFlags |= 0x200;
                self->step++;
            }
        }
        break;
    case 2:
        prim = &g_PrimBuf[self->primIndex];
        if (g_CutsceneFlags & 0x80) {
            PCOL(prim)++;
            if (prim->r0 > 0xC0) {
                g_CutsceneFlags |= 0x100;
            }
            if (prim->r0 > 0xFD) {
                PCOL(prim) = 0;
                prim->drawMode = DRAW_DEFAULT;
                SetStep(3);
                g_api.PlaySfx(SFX_WEAPON_APPEAR);
            }
        }
        break;
    case 3:
        prim = &g_PrimBuf[self->primIndex];
        prim->type = PRIM_G4;
        prim->x0 = prim->x2 = 0;
        prim->x1 = prim->x3 = 0x100;
        prim->y0 = prim->y1 = 0x10;
        prim->y2 = prim->y3 = 0xF0;
        prim->priority = 0x7F;
        prim->drawMode = DRAW_DEFAULT;
        if (self->poseTimer & 1) {
            prim->r0 = 0x80;
            prim->g0 = 0x80;
            prim->b0 = 0x80;
            prim->r1 = 0x80;
            prim->g1 = 0x80;
            prim->b1 = 0x80;
            prim->r2 = 0x80;
            prim->g2 = 0x80;
            prim->b2 = 0x80;
            prim->r3 = 0x80;
            prim->g3 = 0x80;
            prim->b3 = 0x80;
        } else {
            prim->r0 = 0;
            prim->g0 = 0;
            prim->b0 = 0;
            prim->r1 = 0;
            prim->g1 = 0;
            prim->b1 = 0;
            prim->r2 = 0;
            prim->g2 = 0;
            prim->b2 = 0;
            prim->r3 = 0;
            prim->g3 = 0;
            prim->b3 = 0;
        }
        self->poseTimer++;
        if (self->poseTimer > 8) {
            DestroyEntity(self);
            g_CutsceneFlags |= 0x20;
        }
        break;
    }
}

void func_us_801A4028(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->animSet = ANIMSET_OVL(1);
        self->palette = 0x200;
        self->unk5A = 0x48;
        self->animCurFrame = 0x35;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = 0x100;
        self->scaleY = 0x100;
        self->zPriority = 0x81;
        break;
    case 1:
        if (g_CutsceneFlags & 0x80) {
            self->step++;
            self->velocityY = FIX(0.5);
        }
        break;
    case 2:
        self->velocityY -= FIX(0.005859375);
        MoveEntity();
        self->scaleX--;
        self->scaleY--;
        if (g_CutsceneFlags & 0x100) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801A9208_from_bo6(Entity* self) {
    extern EInit g_EInitInteractable;
    extern u32 g_CutsceneFlags;
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        primIndex = (s16)g_api.AllocPrimitives(PRIM_G4, 1);
        if (primIndex != -1) {
            InitializeEntity(g_EInitInteractable);
            g_PauseAllowed = false;
            g_unkGraphicsStruct.pauseEnemies = true;
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            self->animSet = 0;
            prim = &g_PrimBuf[primIndex];

            prim->x0 = prim->x2 = 0;
            prim->x1 = prim->x3 = 0x100;
            prim->y0 = prim->y1 = 4;
            prim->y2 = prim->y3 = 0xE8;

            PCOL(prim) = 0x80;

            prim->priority = 0x1F8;
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
            self->ext.utimer.t = 0;
            g_api.PlaySfx(SFX_WEAPON_APPEAR);
        }
        self->flags |= FLAG_UNK_10000;
        break;
    case 1:
        prim = &g_PrimBuf[self->primIndex];
        self->ext.utimer.t++;
        if (self->ext.utimer.t > 8) {
            g_unkGraphicsStruct.unk28 = 0xFF;
            self->step++;
            prim->drawMode = DRAW_HIDE;
        } else if (self->ext.utimer.t & 1) {
            prim->drawMode = DRAW_HIDE;
        } else {
            prim->drawMode =
                DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        }
        break;
    case 2:
        if (g_CutsceneFlags & 0x100) {
            g_unkGraphicsStruct.unk28 = 0;
            self->step++;
            self->ext.utimer.t = 0;
        }
        break;
    case 3:
        prim = &g_PrimBuf[self->primIndex];
        self->step++;
        PCOL(prim) = 0;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        break;
    case 4:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) += 1;

        if (prim->r0 == 0xFF) {
            prim->drawMode = DRAW_COLORS;
            self->step++;
        }
        break;
    case 5:
        prim = &g_PrimBuf[self->primIndex];
        PCOL(prim) -= 4;

        if (prim->r0 < 5) {
            g_PauseAllowed = true;
            g_unkGraphicsStruct.pauseEnemies = false;
            if (g_RoomCount > 0x736) {
                D_800978B4 = 3;
            } else {
                D_800978B4 = 5;
            }
            g_GameState = Game_Ending;
            g_GameStep = Play_Reset;
            self->step++;
        }
        break;
    }
}


INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019DB9C);

void func_us_8019EADC(Entity* self) {
    extern EInit D_us_80180540;
    extern u8 D_pspeu_0926B438[];
    extern u8 D_pspeu_0926B448[];
    extern u8 D_pspeu_0926B458[];
    extern u8 D_pspeu_0926B468[];
    extern s32 D_pspeu_0926BFE8;
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C068;
    extern s32 D_pspeu_0926C070;
    extern s32 D_pspeu_0926C088;
    extern s32 D_pspeu_0926C0A0;
    Entity* entity;

    entity = &g_Entities[80];
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi;
    if (!self->params) {
        self->posX.i.hi -= 0x34;
        self->posY.i.hi += 0x10;
    } else {
        self->posX.i.hi += 0x3B;
        self->posY.i.hi += 0x18;
    }
    self->palette = D_pspeu_0926C0A0;
    if ((D_pspeu_0926C060 & 1) && self->step != 16) {
        SetStep(16);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
        self->facingLeft = self->params;
        entity = self + 1;
        CreateEntityFromCurrentEntity(D_pspeu_0926BFE8, entity);
        entity->params = self->params;
        entity->zPriority = self->zPriority + 1;
        // Fallthrough
    case 1:
        self->animCurFrame = 0xC;
        if (D_pspeu_0926C070 == 1) {
            SetStep(2);
        }
        if (D_pspeu_0926C088 == self->params + 1) {
            SetStep(3);
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            self->animCurFrame = 0xC;
            if (D_pspeu_0926C070 == 3) {
                self->step_s++;
            }
            break;
        case 1:
            if (!self->poseTimer && self->pose == 3) {
                D_pspeu_0926C070 = 4;
            }
            if (!AnimateEntity(D_pspeu_0926B438, self)) {
                D_pspeu_0926C070 = 5;
                SetSubStep(2);
            }
            break;
        case 2:
            if (!AnimateEntity(D_pspeu_0926B468, self)) {
                SetStep(1);
            }
            break;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            if (!AnimateEntity(D_pspeu_0926B448, self)) {
                SetSubStep(1);
            }
            if (!self->poseTimer && self->pose == 2) {
                PlaySfxPositional(0x662);
            }
            break;
        case 1:
            if (!AnimateEntity(D_pspeu_0926B468, self)) {
                D_pspeu_0926C088 = 3;
                SetStep(1);
            }
            break;
        }
        break;
    case 8:
        if (!AnimateEntity(D_pspeu_0926B458, self)) {
            self->pose = 0;
            self->poseTimer = 0;
        }
        break;
    case 16:
        switch (self->step_s) {
        case 0:
            self->pose = 0x10 - self->animCurFrame;
            self->step_s++;
            // Fallthrough
        case 1:
            if (!AnimateEntity(D_pspeu_0926B468, self)) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s++;
            }
            break;
        }
        break;
    }
    if (D_pspeu_0926C068) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019EE30);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019F1CC);

void func_us_8019FA78(Entity* self) {
    extern u16 D_us_80180540;
    extern u32 D_pspeu_0926C0A0;
    extern s32 D_pspeu_0926C068;

    self->palette = (u16)D_pspeu_0926C0A0;

    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->animCurFrame = 0x11;
        /* fall through */
    case 1:
        self->animCurFrame = 0x11;
        break;
    }

    if (D_pspeu_0926C068 != 0) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

void func_us_8019FB04(Entity* self) {
    extern s32 D_pspeu_0926C0A0;
    extern u8* D_pspeu_0926B500[];
    extern s32 D_pspeu_0926C068;
    extern Entity g_Entities[256];
    extern u16 D_us_80180540;
    extern void InitializeEntity(u16*);

    u8* anim;
    Entity* source;

    self->palette = D_pspeu_0926C0A0;
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        /* fall through */
    case 1:
        anim = D_pspeu_0926B500[self->params];
        AnimateEntity(anim, self);
        source = &g_Entities[80];
        self->posX.i.hi = source->posX.i.hi;
        self->posY.i.hi = source->posY.i.hi;
        break;
    }

    if (D_pspeu_0926C068 != 0) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

void func_us_8019FBC0(Entity* self) {
    extern u16 D_us_80180540;
    extern u8 D_pspeu_0926B4A8[];
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C068;
    extern s32 D_pspeu_0926C0A0;
    Entity* source = &g_Entities[80];

    self->posX.i.hi = source->posX.i.hi;
    self->posY.i.hi = source->posY.i.hi;
    self->palette = D_pspeu_0926C0A0;
    if ((D_pspeu_0926C060 & 1) && self->step != 16) {
        SetStep(16);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        /* fall through */
    case 1:
        AnimateEntity(D_pspeu_0926B4A8, self);
        break;
    case 16:
        self->animCurFrame = 0x1B;
        break;
    }
    if (D_pspeu_0926C068) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019FCB4);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A01A4);

void func_us_801A0710(Entity* self) {
    extern u16 D_us_80180540;
    extern s32 D_pspeu_0926BFB8;
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C070;
    Entity* entity;
    s32 scale;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }
    entity = &g_Entities[80];
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi + 0x10;
    switch (self->step) {
    case 0:
        InitializeEntity(&D_us_80180540);
        self->animCurFrame = 0x2A;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0;
        /* fall through */
    case 1:
        self->scaleX = self->scaleY += 0x10;
        if (self->scaleX > 0x180) {
            self->step++;
        }
        break;
    case 2:
        entity = &g_Entities[130];
        CreateEntityFromCurrentEntity(D_pspeu_0926BFB8, entity);
        entity->zPriority = self->zPriority + 1;
        self->step++;
        break;
    case 3:
        if (D_pspeu_0926C070 == 2) {
            self->step++;
        }
        break;
    case 4:
        scale = self->scaleX;
        scale -= 0x20;
        self->scaleX = self->scaleY = scale;
        if (scale < 0) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801A0860(Entity* self) {
    extern EInit D_us_80180540;
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C070;
    extern s32 D_pspeu_0926C078;
    extern s32 D_pspeu_0926BFB0;
    Entity* entity;
    s32 i;
    s32 scale;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }
    entity = &g_Entities[80];
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi + 0x10;
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
        self->animCurFrame = D_pspeu_0926C078 + 0x2D;
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0;
        self->drawFlags |= ENTITY_OPACITY;
        self->opacity = 0;
        // Fallthrough
    case 1:
        self->scaleX = self->scaleY += 8;
        if (self->scaleX >= 0x100) {
            self->scaleX = self->scaleY = 0x100;
            self->step++;
        }
        // Fallthrough
    case 2:
        self->opacity += 2;
        if (self->opacity >= 0x80) {
            D_pspeu_0926C070 = 2;
            self->step = 3;
        }
        break;
    case 3:
        if (D_pspeu_0926C070 == 4) {
            g_api.PlaySfx(0x868);
            self->step++;
        }
        break;
    case 4:
        self->scaleX += 8;
        scale = self->scaleY;
        scale -= 0x18;
        if (scale < 0) {
            scale = 0;
        }
        self->scaleY = scale;
        for (i = 0; i < 16; i++) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(D_pspeu_0926BFB0, self, entity);
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (!scale) {
            self->animCurFrame = 0;
            ((Rbo6TransformationEffect*)&self->ext)->timer = 0x50;
            self->step++;
        }
        break;
    case 5:
        if (!--((Rbo6TransformationEffect*)&self->ext)->timer) {
            g_api.PlaySfx(0x672);
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801A0AB4(Entity* self) {
    extern EInit D_us_80180540;
    extern u8 D_pspeu_0926B4E8[];
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C070;
    s32 deltaX;
    s32 deltaY;
    Entity* entity;
    s16 angle;
    s32 speed;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = (Random() & 0x7F) + 0x80;
        self->blendMode = BLEND_TRANSP;
        angle = rand() & 0xFFF;
        speed = (Random() & 0x1F) + 0x10;
        self->velocityX = speed * rcos(angle);
        self->velocityY = speed * rsin(angle);
        self->posX.val += self->velocityX * 4;
        self->posY.val += self->velocityY * 4;
        ((Rbo6TransformationEffect*)&self->ext)->timer = (Random() & 0x1F) + 0x10;
        // Fallthrough
    case 1:
        MoveEntity();
        AnimateEntity(D_pspeu_0926B4E8, self);
        self->velocityX -= self->velocityX >> 5;
        self->velocityY -= self->velocityY >> 5;
        if (D_pspeu_0926C070 == 5) {
            self->velocityX = 0;
            self->velocityY = 0;
            self->step++;
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_0926B4E8, self);
        if (!--((Rbo6TransformationEffect*)&self->ext)->timer) {
            self->step++;
        }
        if (!(g_Timer & 7)) {
            g_api.PlaySfx(SFX_NOISE_SWEEP_DOWN_B);
        }
        break;
    case 3:
        MoveEntity();
        if (self->scaleX > 0x20) {
            self->scaleX = self->scaleY -= 2;
        }
        entity = &g_Entities[80];
        deltaX = entity->posX.i.hi - self->posX.i.hi;
        deltaY = entity->posY.i.hi - 0x18 - self->posY.i.hi;
        angle = ratan2(deltaY, deltaX);
        if (((Rbo6TransformationEffect*)&self->ext)->speed < FIX(3)) {
            ((Rbo6TransformationEffect*)&self->ext)->speed += FIX(0.0625);
        }
        speed = ((Rbo6TransformationEffect*)&self->ext)->speed;
        self->velocityX = (speed * rcos(angle)) >> 12;
        self->velocityY = (speed * rsin(angle)) >> 12;
        speed = deltaX * deltaX + deltaY * deltaY;
        if (speed < 0x40) {
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801A0DC0(Entity* self) {
    extern EInit D_us_80180564;
    extern u8 D_pspeu_0926B408[];
    extern s32 D_pspeu_0926C060;
    extern s32 D_pspeu_0926C080;
    extern s32 D_pspeu_0926BF90;
    s32 deltaX;
    s32 deltaY;
    s32 scale;
    Entity* entity;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180564);
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0;
        self->blendMode |= BLEND_TRANSP | BLEND_ADD;
        self->drawFlags |= ENTITY_OPACITY;
        self->opacity = 0x60;
        self->hitboxState = 0;
        // Fallthrough
    case 1:
        AnimateEntity(D_pspeu_0926B408, self);
        scale = self->scaleX;
        scale += 0x10;
        if (scale > 0xCC) {
            scale = 0xCC;
        }
        self->scaleX = self->scaleY = scale;
        if (D_pspeu_0926C080) {
            ((Rbo6TransformationEffect*)&self->ext)->timer = 0x60;
            entity = &PLAYER;
            deltaX = entity->posX.val - self->posX.val;
            deltaY = entity->posY.val - self->posY.val;
            self->velocityX = deltaX / 96;
            self->velocityY = deltaY / 96;
            self->step++;
        }
        if (!(g_Timer & 0x1F)) {
            g_api.PlaySfxVolPan(0x6DE, 0x50, 0);
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_0926B408, self);
        MoveEntity();
        scale = 0x6000 / (((Rbo6TransformationEffect*)&self->ext)->timer + 0x18);
        self->scaleX = self->scaleY = scale;
        if (!(((Rbo6TransformationEffect*)&self->ext)->timer & 3)) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(D_pspeu_0926BF90, self, entity);
                entity->scaleX = entity->scaleY = self->scaleX;
                entity->zPriority = self->zPriority;
                entity->opacity = self->opacity;
            }
        }
        if (!(((Rbo6TransformationEffect*)&self->ext)->timer & 0xF)) {
            g_api.PlaySfxVolPan(0x6E2, 0x80 - ((Rbo6TransformationEffect*)&self->ext)->timer / 2, 0);
        }
        if (!--((Rbo6TransformationEffect*)&self->ext)->timer) {
            g_api.PlaySfx(0x661);
            self->zPriority = g_unkGraphicsStruct.g_zEntityCenter + 8;
            entity = &PLAYER;
            deltaX = entity->posX.i.hi - self->posX.i.hi;
            deltaY = entity->posY.i.hi - self->posY.i.hi;
            scale = self->scaleX * 20 / 256;
            if (deltaX * deltaX + deltaY * deltaY < scale * scale) {
                self->hitboxOffX = deltaX;
                self->hitboxOffY = deltaY;
                self->hitboxState = 1;
            }
            self->step++;
        }
        break;
    case 3:
        DestroyEntity(self);
        break;
    }
}

void func_us_801A1150(Entity* self) {
    extern u16 D_us_80180540;
    extern u8 D_us_80180724;
    extern u8 D_pspeu_0926B410;
    s32 var_v0;

    if (!self->step) {
        InitializeEntity(&D_us_80180540);
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->drawFlags |= ENTITY_OPACITY;
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }
    AnimateEntity(&D_pspeu_0926B410, self);
    var_v0 = self->opacity;
    var_v0 -= 2;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    self->opacity = (u8)var_v0;
    if (var_v0 == 0) {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A11DC);

void func_us_801A1B38(Entity* self) {
    Entity* real;

    if (*(s32*)0x0926C060 & 1) {
        DestroyEntity(self);
        return;
    }

    if (!self->step) {
        InitializeEntity((u16*)((u8*)(void*)func_us_801A1B38 + 0x80F8));
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }

    AnimateEntity((u8*)((u8*)(void*)func_us_801A1B38 + 0x1E620), self);

    real = self->ext.succubus.real;
    self->posX.i.hi = real->posX.i.hi;
    self->posY.i.hi = real->posY.i.hi + 0xC;

    if (*(s32*)0x0926C098 == 5) {
        DestroyEntity(self);
    }
}
