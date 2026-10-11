// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"

extern EInit g_EInitBreakable;
extern u8* D_pspeu_09256C38[];
extern u8 D_pspeu_09256C58[];
extern u16 D_pspeu_09256C60[];
extern u8 D_pspeu_0926C040[];
extern u8 D_pspeu_09256C70[];
extern EInit g_EInitParticle;
extern s32 D_pspeu_0926C048;
extern EInit g_EInitInteractable;
extern u32 g_CutsceneFlags;
extern EInit D_us_80180540;
extern u8 D_pspeu_0926B438[];
extern u8 D_pspeu_0926B448[];
extern u8 D_pspeu_0926B458[];
extern u8 D_pspeu_0926B468[];
extern s32 E_ID(UNK_1B);
extern s32 D_pspeu_0926C060;
extern s32 D_pspeu_0926C068;
extern s32 D_pspeu_0926C070;
extern s32 D_pspeu_0926C088;
extern s32 D_pspeu_0926C0A0;
extern EInit D_us_80180558;
extern u8 D_pspeu_0926B418[];
extern u8 D_pspeu_0926B420[];
extern u8 D_pspeu_0926B430[];
extern Point16 D_pspeu_0926B518[];
extern s8 D_pspeu_0926B550[];
extern u8 D_pspeu_0926B570[];
extern s32 E_ID(UNK_1F);
extern u8* D_pspeu_0926B500[];
extern Entity g_Entities[256];
extern u8 D_pspeu_0926B4A8[];
extern s32 E_ID(UNK_21);
extern s32 D_pspeu_0926C078;
extern s32 E_ID(UNK_22);
extern u8 D_pspeu_0926B4E8[];
extern EInit D_us_80180564;
extern u8 D_pspeu_0926B408[];
extern s32 D_pspeu_0926C080;
extern s32 E_ID(UNK_26);
extern u8 D_us_80180724;
extern u8 D_pspeu_0926B410;
extern s32 D_pspeu_0926C098;
extern u8 D_pspeu_0926B4F8[];

extern EInit D_us_8018054C;
extern u8 D_pspeu_0926B478[];
extern u8 D_pspeu_0926B480[];
extern u8 D_pspeu_0926B490[];
extern Point16 D_pspeu_0926B530[];
extern s16 D_pspeu_0926B540[];
extern s32 D_pspeu_0926C090;
extern s32 E_ID(UNK_1D);
extern s32 E_ID(UNK_25);

void func_us_801A4F14(Entity* self) {
    s32 primIndex;
    s32 size;
    s32 y;
    s32 x;
    s32 width;
    s16* positions;
    s32 i;
    Primitive* prim;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->animCurFrame = 0;
        size = Random() % D_pspeu_0926C048;
        self->hitboxWidth = size + 2;
        size++;
        self->velocityY = FIX(-1.5) - ((Random() % (size + 2)) << 17);
        self->ext.et_801A4F14.accelerationY =
            -0x1000 - (Random() % (size + 1)) * 0x3000;
        self->opacity = (Random() % size) * 8 + 0x20;
        if (size < D_pspeu_0926C048 / 2) {
            self->zPriority = 0x54;
        } else {
            self->zPriority = 0x90;
        }
        primIndex = (s16)g_api.AllocPrimitives(PRIM_GT4, 7);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_801A4F14.prim = prim;
        prim->tpage = 0x1A;
        prim->clut = 0x2EC;
        prim->u0 = prim->u2 = 0;
        prim->u1 = prim->u3 = 0x40;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xE0;
        PGREY(prim, 0) = self->opacity;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->priority = self->zPriority;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim = prim->next;
        for (i = 0; i < 4; i++) {
            prim->tpage = 0x1A;
            prim->clut = 0x2EC;
            prim->u0 = prim->u2 = 0;
            prim->u1 = prim->u3 = 0x40;
            prim->v0 = prim->v1 = 0xE0;
            prim->v2 = prim->v3 = 0xE2;
            PGREY(prim, 0) = self->opacity;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->priority = self->zPriority + 1;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim = prim->next;
        }
        prim->tpage = 0x1A;
        prim->clut = 0x2EC;
        prim->u0 = prim->u2 = 0;
        prim->u1 = prim->u3 = 0x40;
        prim->v0 = prim->v1 = 0xE0;
        prim->v2 = prim->v3 = 0xFF;
        PGREY(prim, 0) = self->opacity;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->priority = self->zPriority;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim = prim->next;
        self->ext.et_801A4F14.unk84 = prim;
        prim->tpage = 0x1A;
        prim->clut = 0x15F;
        prim->clut = 0x2EC;
        prim->u0 = prim->u2 = 0;
        prim->u1 = prim->u3 = 0x3F;
        prim->v0 = prim->v1 = 0xC0;
        prim->v2 = prim->v3 = 0xFF;
        PGREY(prim, 0) = self->opacity + 0x20;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->priority = self->zPriority;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 |
                         DRAW_TRANSP | DRAW_UNK_40;
        if (self->hitboxWidth < 4) {
            prim->drawMode = DRAW_HIDE;
        }
        positions = self->ext.et_801A4F14.yPositions;
        for (i = 0; i < 6; i++) {
            *positions = self->posY.i.hi;
            positions++;
        }
        break;
    case 1:
        MoveEntity();
        self->velocityY += (s32)self->ext.et_801A4F14.accelerationY;
        break;
    }
    positions = &self->ext.et_801A4F14.yPositions[5];
    for (i = 0; i < 5; i++) {
        *positions = positions[-1];
        positions--;
    }
    positions = self->ext.et_801A4F14.yPositions;
    *positions = self->posY.i.hi;
    width = self->hitboxWidth;
    prim = self->ext.et_801A4F14.prim;
    x = self->posX.i.hi;
    y = self->posY.i.hi - self->hitboxWidth;
    prim->x0 = x - width;
    prim->x1 = x + width;
    prim->y0 = prim->y1 = y;
    prim = self->ext.et_801A4F14.unk84;
    prim->x0 = x - width * 2;
    prim->x1 = x + width * 2;
    prim->y0 = prim->y1 = y - width;
    prim = self->ext.et_801A4F14.prim;
    positions = self->ext.et_801A4F14.yPositions;
    for (i = 0; i < 5; i++) {
        y = *positions;
        prim->x2 = x - width;
        prim->x3 = x + width;
        prim->y2 = prim->y3 = y;
        prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
        prim = prim->next;
        prim->x0 = x - width;
        prim->x1 = x + width;
        prim->y0 = prim->y1 = y;
        positions++;
    }
    y = y + self->hitboxWidth;
    prim->x2 = x - width;
    prim->x3 = x + width;
    prim->y2 = prim->y3 = y;
    prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
    prim = prim->next;
    prim = self->ext.et_801A4F14.unk84;
    prim->x2 = x - width * 2;
    prim->x3 = x + width * 2;
    prim->y2 = prim->y3 = y + self->hitboxWidth * 2;
    if (y < 0x10) {
        DestroyEntity(self);
    }
}

void func_us_801A367C(Entity* self) {
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
            prim->drawMode =
                DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
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
            prim->drawMode =
                DRAW_UNK_40 | DRAW_TPAGE | DRAW_COLORS | DRAW_TRANSP;
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

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_E3B8", func_us_8019DB9C);

void func_us_8019EADC(Entity* self) {
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
        CreateEntityFromCurrentEntity(E_ID(UNK_1B), entity);
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
                PlaySfxPositional(SFX_FIREBALL_SHOT_C);
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

void func_us_8019EE30(Entity* self) {
    s32 index;
    s8* hitbox;
    s32 i;
    Entity* entity;

    self->palette = D_pspeu_0926C0A0;
    if ((D_pspeu_0926C060 & 1) && self->step != 16) {
        SetStep(16);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180558);
        self->facingLeft = self->params;
        self->parent = &g_Entities[80];
        if (!self->params) {
            self->nextPart = self + 2;
        } else {
            self->nextPart = &g_Entities[80];
        }
        // Fallthrough
    case 1:
        AnimateEntity(D_pspeu_0926B418, self);
        if (D_pspeu_0926C070 == 1) {
            SetStep(2);
        }
        if (D_pspeu_0926C088 == self->params + 1) {
            self->animCurFrame = 0xA;
        }
        break;
    case 2:
        switch (self->step_s) {
        case 0:
            for (i = 0; i < 2; i++) {
                entity = AllocEntity(&g_Entities[144], &g_Entities[152]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_ID(UNK_1F), entity);
                    entity->params = self->params;
                    entity->zPriority = self->zPriority + 4;
                }
            }
            self->animCurFrame = 9;
            self->step_s++;
            // Fallthrough
        case 1:
            if (D_pspeu_0926C070 == 3) {
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_pspeu_0926B420, self);
            if (D_pspeu_0926C070 == 5) {
                SetSubStep(3);
            }
            break;
        case 3:
            if (!AnimateEntity(D_pspeu_0926B430, self)) {
                SetStep(1);
            }
            break;
        }
        break;
    case 16:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->animCurFrame = 9;
            self->step_s++;
            break;
        case 1:
            break;
        }
        break;
    }
    entity = self - 1;
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi;
    index = entity->animCurFrame - 0xC;
    if (index < 0) {
        index = 0;
    }
    if (index > 4) {
        index = 4;
    }
    if (!self->params) {
        self->posX.i.hi += D_pspeu_0926B518[index].x;
    } else {
        self->posX.i.hi -= D_pspeu_0926B518[index].x;
    }
    self->posY.i.hi += D_pspeu_0926B518[index].y;
    hitbox = D_pspeu_0926B550;
    hitbox += D_pspeu_0926B570[self->animCurFrame] * 4;
    self->hitboxOffX = *hitbox++;
    self->hitboxOffY = *hitbox++;
    self->hitboxWidth = *hitbox++;
    self->hitboxHeight = *hitbox++;
    if (D_pspeu_0926C068) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    } else {
        self->hitboxState = 3;
    }
}

void func_us_8019F1CC(Entity* self) {
    Entity* entity;
    long diffX;
    long diffY;
    long posX;
    s32 index;
    register long posY;
    s16 angle;
    Entity* previous;
    s32 distance;

    self->palette = D_pspeu_0926C0A0;
    if ((D_pspeu_0926C060 & 1) && self->step != 16) {
        SetStep(16);
    }
    distance = self->step;
    switch (distance) {
    case 0:
        InitializeEntity(D_us_8018054C);
        entity = self + 1;
        for (index = 0; index < 5; index++, entity++) {
            CreateEntityFromCurrentEntity(E_ID(UNK_1D), entity);
            entity->zPriority = self->zPriority - index;
        }
        // fallthrough
    case 1:
        AnimateEntity(D_pspeu_0926B478, self);
        entity = &g_Entities[80];
        diffX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
        diffY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
        self->posX.i.hi = diffX;
        self->posY.i.hi = diffY;
        self->zPriority = entity->zPriority + 2;
        self->hitboxState = 0;
        if (D_pspeu_0926C090 == self->params + 1) {
            self->zPriority = entity->zPriority + 6;
            SetStep(2);
        }
        if (D_pspeu_0926C098 == 1) {
            self->zPriority = entity->zPriority + 6;
            SetStep(3);
        }
        break;
    case 2:
        self->hitboxState = 3;
        switch (self->step_s) {
        case 0:
            entity = &PLAYER;
            diffX = entity->posX.i.hi - self->posX.i.hi;
            diffY = entity->posY.i.hi - self->posY.i.hi;
            angle = ratan2(diffY, diffX);
            self->velocityX = (rcos(angle) << 18) >> 12;
            self->velocityY = (rsin(angle) << 18) >> 12;
            PlaySfxPositional(SFX_DRACULA_MONSTER_ROAR);
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            if (!AnimateEntity(D_pspeu_0926B480, self)) {
                SetSubStep(2);
            }
            break;
        case 2:
            AnimateEntity(D_pspeu_0926B490, self);
            MoveEntity();
            entity = &g_Entities[80];
            diffX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
            diffY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
            diffX -= self->posX.i.hi;
            diffY -= self->posY.i.hi;
            angle = ratan2(diffY, diffX);
            self->velocityX = rcos(angle) << 5;
            self->velocityY = rsin(angle) << 5;
            if (abs(diffX) < 3 && abs(diffY) < 3) {
                D_pspeu_0926C090++;
                SetStep(1);
            }
            break;
        }
        break;
    case 3:
        switch (self->step_s) {
        case 0:
            angle = D_pspeu_0926B540[self->params];
            self->velocityX = (rcos(angle) << 18) >> 12;
            self->velocityY = (rsin(angle) << 18) >> 12;
            self->step_s++;
            // fallthrough
        case 1:
            MoveEntity();
            if (!AnimateEntity(D_pspeu_0926B480, self)) {
                g_api.PlaySfx(SFX_WEAPON_APPEAR);
                entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
                if (entity != NULL) {
                    CreateEntityFromCurrentEntity(E_ID(UNK_25), entity);
                    entity->ext.succubus.real = self;
                    entity->zPriority = self->zPriority + 1;
                }
                D_pspeu_0926C098++;
                self->step_s++;
            }
            break;
        case 2:
            if (D_pspeu_0926C098 == 5) {
                self->step_s++;
            }
            break;
        case 3:
            AnimateEntity(D_pspeu_0926B490, self);
            MoveEntity();
            entity = &g_Entities[80];
            diffX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
            diffY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
            diffX -= self->posX.i.hi;
            diffY -= self->posY.i.hi;
            angle = ratan2(diffY, diffX);
            self->velocityX = rcos(angle) * 40;
            self->velocityY = rsin(angle) * 40;
            if (abs(diffX) < 3 && abs(diffY) < 3) {
                SetStep(1);
            }
            break;
        }
        break;
    case 16:
        switch (self->step_s) {
        case 0:
            self->hitboxState = 0;
            self->animCurFrame = 18;
            MoveEntity();
            entity = &g_Entities[80];
            diffX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
            diffY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
            diffX -= self->posX.i.hi;
            diffY -= self->posY.i.hi;
            angle = ratan2(diffY, diffX);
            self->velocityX = rcos(angle) * 40;
            self->velocityY = rsin(angle) * 40;
            if (abs(diffX) < 3 && abs(diffY) < 3) {
                self->zPriority = entity->zPriority + 2;
                self->step_s++;
            }
            break;
        case 1:
            entity = &g_Entities[80];
            diffX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
            diffY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
            self->posX.i.hi = diffX;
            self->posY.i.hi = diffY;
            break;
        }
        break;
    }
    entity = &g_Entities[80];
    posX = entity->posX.val + (D_pspeu_0926B530[self->params].x << 16);
    posY = entity->posY.val + (D_pspeu_0926B530[self->params].y << 16);
    entity = self + 5;
    for (index = 0; index < 5; index++) {
        previous = entity - 1;
        posX = (posX + previous->posX.val) >> 1;
        posY = (posY + previous->posY.val) >> 1;
        diffX = posX - entity->posX.val;
        diffY = posY - entity->posY.val;
        posX = (diffX >> 2) + posX;
        posY = (diffY >> 2) + posY;
        entity->posX.val = posX;
        entity->posY.val = posY;
        entity->zPriority = self->zPriority - 5 + index;
        entity = previous;
    }
    entity = &g_Entities[80];
    posX = entity->posX.i.hi + D_pspeu_0926B530[self->params].x;
    posY = entity->posY.i.hi + D_pspeu_0926B530[self->params].y;
    entity = self + 5;
    for (index = 0; index < 6; index++, entity--) {
        diffX = entity->posX.i.hi - posX;
        diffY = entity->posY.i.hi - posY;
        angle = ratan2(diffY, diffX);
        distance = diffX * diffX + diffY * diffY;
        if (distance > 144) {
            entity->posX.i.hi = posX + ((rcos(angle) * 12) >> 12);
            entity->posY.i.hi = posY + ((rsin(angle) * 12) >> 12);
        }
        posX = entity->posX.i.hi;
        posY = entity->posY.i.hi;
    }
    if (D_pspeu_0926C068) {
        self->animCurFrame = 0;
        self->hitboxState = 0;
    }
}

void func_us_8019FA78(Entity* self) {

    self->palette = (u16)D_pspeu_0926C0A0;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
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

    u8* anim;
    Entity* source;

    self->palette = D_pspeu_0926C0A0;
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
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
    Entity* source = &g_Entities[80];

    self->posX.i.hi = source->posX.i.hi;
    self->posY.i.hi = source->posY.i.hi;
    self->palette = D_pspeu_0926C0A0;
    if ((D_pspeu_0926C060 & 1) && self->step != 16) {
        SetStep(16);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180540);
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

void func_us_8019FCB4(Entity* self) {
    s32 primIndex;
    Entity* entity;
    s32 palette;
    s32 i;
    s16 angle;
    s16 y;
    s16 x;
    Primitive* prim;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = (s16)g_api.AllocPrimitives(PRIM_GT4, 4);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.et_8019FCB4.prim = prim;
        while (prim != NULL) {
            prim->tpage = 0x14;
            prim->clut = 0x201;
            prim->priority = self->zPriority - 1;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        prim = self->ext.et_8019FCB4.prim;
        for (i = 0; i < 2; i++) {
            prim->u0 = 0xC1;
            prim->u1 = 0xFF;
            prim->u2 = 0xC1;
            prim->u3 = 0xFF;
            prim->v0 = 1;
            prim->v1 = 1;
            prim->v2 = 0x47;
            prim->v3 = 0x47;
            prim->priority = self->zPriority - 2;
            prim = prim->next;
            prim->u0 = 1;
            prim->u1 = 0xBF;
            prim->u2 = 1;
            prim->u3 = 0xBF;
            prim->v0 = 1;
            prim->v1 = 1;
            prim->v2 = 0xB4;
            prim->v3 = 0xB4;
            prim = prim->next;
        }
        // Fallthrough
    case 1:
        entity = &g_Entities[80];
        self->posX.i.hi = entity->posX.i.hi;
        self->posY.i.hi = entity->posY.i.hi;
        self->ext.et_8019FCB4.angle += 0x10;
        angle = self->rotate = (rsin(self->ext.et_8019FCB4.angle) << 7) >> 12;
        prim = self->ext.et_8019FCB4.prim;
        for (i = 0; i < 2; i++) {
            if (!i) {
                x = self->posX.i.hi - 8;
            } else {
                x = self->posX.i.hi + 8;
            }
            y = self->posY.i.hi - 0x34;
            prim->x1 = x;
            prim->y1 = y;
            prim->x3 = x;
            prim->y3 = y + 0x46;
            if (!i) {
                x = x - ((rcos(angle) * 0x3E) >> 12);
            } else {
                x = x + ((rcos(angle) * 0x3E) >> 12);
            }
            y = y + ((rsin(angle) * 0x3E) >> 12);
            prim->x0 = x;
            prim->y0 = y;
            if (!i) {
                prim->x2 = x + ((rsin(angle) * 0x46) >> 12);
            } else {
                prim->x2 = x - ((rsin(angle) * 0x46) >> 12);
            }
            prim->y2 = y + ((rcos(angle) * 0x46) >> 12);
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
            if (!i) {
                x = x + 0x18;
            } else {
                x = x - 0x18;
            }
            y = y - 0x78;
            prim->x1 = x;
            prim->y1 = y;
            prim->x3 = x;
            prim->y3 = y + 0xB3;
            if (!i) {
                x = x - ((rcos(angle) * 0xBE) >> 12);
            } else {
                x = x + ((rcos(angle) * 0xBE) >> 12);
            }
            y = y + ((rsin(angle) * 0xBE) >> 12);
            prim->x0 = x;
            prim->y0 = y;
            if (!i) {
                prim->x2 = x + ((rsin(angle) * 0xB3) >> 12);
            } else {
                prim->x2 = x - ((rsin(angle) * 0xB3) >> 12);
            }
            prim->y2 = y + ((rcos(angle) * 0xB3) >> 12);
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        }
        break;
    }
    prim = self->ext.et_8019FCB4.prim;
    palette = D_pspeu_0926C0A0 & 0xFFF;
    if (palette == 0x200) {
        palette = 0x201;
    }
    while (prim != NULL) {
        prim->clut = palette;
        if (D_pspeu_0926C068) {
            prim->drawMode |= DRAW_HIDE;
        }
        prim = prim->next;
    }
    FntPrint("color %x\n", D_pspeu_0926C0A0);
}

void func_us_801A01A4(Entity* self) {
    Primitive* prim;
    Primitive* primTwo;
    s32 posX;
    Entity* entity;
    s32 posY;
    s16 tempAngle;
    s16 boltAngle;
    s32 i;
    u8 closeToTarget;
    s16 angle;
    s32 primIndex;
    s32 sp34;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }

    if (D_pspeu_0926C070 == 2) {
        DestroyEntity(self);
        return;
    }
    if (!self->params) {
        self->ext.shaftLightning.sourceOrb = &g_Entities[86];
        self->ext.shaftLightning.targetOrb = &g_Entities[88];
    } else {
        self->ext.shaftLightning.targetOrb = &g_Entities[86];
        self->ext.shaftLightning.sourceOrb = &g_Entities[88];
    }
    entity = self->ext.shaftLightning.sourceOrb;
    self->posX.i.hi = entity->posX.i.hi;
    self->posY.i.hi = entity->posY.i.hi;
    if (!(g_Timer & 0xF)) {
        // The PSP call passes volume/pan through the PlaySfx slot.
        ((void (*)(s32, s32, s32))g_api.PlaySfx)(
            SFX_DRACULA_ELECTRICITY, 0x70, 0);
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = (s16)g_api.AllocPrimitives(PRIM_GT4, 0x14);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.shaftLightning.prim = prim;
        self->ext.shaftLightning.primTwo = prim;

        i = 0;
        while (prim != NULL) {
            prim->tpage = 0x1A;
            prim->clut = 0x194;
            prim->u0 = prim->u1 = (i * 0x10) + 0x90;
            prim->u2 = prim->u3 = prim->u0 + 0x10;
            prim->v0 = prim->v2 = 0xD0;
            prim->v1 = prim->v3 = 0xC0;
            PGREY(prim, 0) = 0x50;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->priority = self->zPriority + 1;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;

            if (++i > 5) {
                i = 0;
            }
        }
    case 1:
        prim = self->ext.shaftLightning.prim;
        prim->x0 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        prim->x1 = prim->x0;
        prim->y1 = prim->y1 - 0x10;
        prim->x2 = self->posX.i.hi;
        prim->y2 = self->posY.i.hi;
        prim->x3 = prim->x2;
        prim->y3 = prim->y2 - 0x10;
        self->ext.shaftLightning.primTwo = prim;

        self->ext.shaftLightning.boltAngle = Random() * 0x10;

        while (prim != NULL) {
            PGREY(prim, 0) = 0x50;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->ext.shaftLightning.segmentsBeforeAim = 0;
        self->step++;
        // fallthrough
    case 2:
        // nb. loop indefinitely until break
        for (i = 0;; i = i + 1) {
            prim = self->ext.shaftLightning.primTwo;
            boltAngle = self->ext.shaftLightning.boltAngle;
            entity = self->ext.shaftLightning.targetOrb;
            posX = entity->posX.i.hi;
            posY = entity->posY.i.hi;
            posX = posX - prim->x2;
            posY = posY - prim->y2;

            if (abs(posX) < 0x10 && abs(posY) < 0x10) {
                self->step = 1;
                return;
            }

            if (abs(posX) < 0x20 && abs(posY) < 0x20) {
                closeToTarget = true;
            } else {
                closeToTarget = false;
            }

            if (!self->ext.shaftLightning.segmentsBeforeAim) {
                self->ext.shaftLightning.segmentsBeforeAim = 4;

                if (closeToTarget) {
                    self->ext.shaftLightning.segmentsBeforeAim = 2;
                }

                angle = ratan2(-posY, posX);
                tempAngle = angle - boltAngle;
                if (tempAngle > ROT(180)) {
                    tempAngle = tempAngle - ROT(360);
                }

                if (tempAngle < ROT(-180)) {
                    tempAngle = tempAngle + ROT(360);
                }

                // If the bolt is getting close to the target orb we turn more
                // sharply to hit it
                if (!closeToTarget) {
                    tempAngle /= 4;
                } else {
                    tempAngle /= 2;
                }
                self->ext.shaftLightning.angleStep = tempAngle;
            }

            boltAngle += self->ext.shaftLightning.angleStep;
            if (!closeToTarget) {
                // Add some jitter to the bolt if we're not close to our target
                // orb yet
                boltAngle += (0x60 - ((Random() & 3) << 6));
            }
            boltAngle &= 0xFFF;

            primTwo = prim->next;
            if (primTwo == NULL) {
                self->step = 1;
                return;
            }

            LOW(primTwo->x0) = LOW(prim->x2);
            LOW(primTwo->x1) = LOW(prim->x3);
            self->ext.shaftLightning.boltAngle = boltAngle;
            self->ext.shaftLightning.primTwo = primTwo;
            posX = (rcos(boltAngle) * 0xC) >> 0xC;
            posY = -((rsin(boltAngle) * 0xC) >> 0xC);
            primTwo->x2 = primTwo->x0 + posX;
            primTwo->y2 = primTwo->y0 + posY;

            tempAngle = boltAngle - ROT(90);
            sp34 = 0x10;
            posX = (sp34 * rcos(tempAngle)) >> 0xC;
            posY = -((sp34 * rsin(tempAngle)) >> 0xC);
            posX = primTwo->x3 = primTwo->x2 + posX;
            posY = primTwo->y3 = primTwo->y2 + posY;
            primTwo->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                DRAW_UNK02 | DRAW_TRANSP | DRAW_UNK_40;
            self->ext.shaftLightning.segmentsBeforeAim--;
        }
        // fallthrough
    case 3:
        prim = self->ext.shaftLightning.prim;
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->step = 1;
        break;
    }
}

void func_us_801A0710(Entity* self) {
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
        InitializeEntity(D_us_80180540);
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
        CreateEntityFromCurrentEntity(E_ID(UNK_21), entity);
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
            g_api.PlaySfx(SFX_DRACULA_KARASUMAN_SQUISH);
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
                CreateEntityFromEntity(E_ID(UNK_22), self, entity);
                entity->zPriority = self->zPriority + 1;
            }
        }
        if (!scale) {
            self->animCurFrame = 0;
            self->ext.et_8019FCB4.timer = 0x50;
            self->step++;
        }
        break;
    case 5:
        if (!--self->ext.et_8019FCB4.timer) {
            g_api.PlaySfx(SFX_NOISE_SWEEP_DOWN_A);
            DestroyEntity(self);
        }
        break;
    }
}

void func_us_801A0AB4(Entity* self) {
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
        self->ext.et_8019FCB4.timer = (Random() & 0x1F) + 0x10;
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
        if (!--self->ext.et_8019FCB4.timer) {
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
        if (self->ext.et_8019FCB4.speed < FIX(3)) {
            self->ext.et_8019FCB4.speed += FIX(0.0625);
        }
        speed = self->ext.et_8019FCB4.speed;
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
            self->ext.et_8019FCB4.timer = 0x60;
            entity = &PLAYER;
            deltaX = entity->posX.val - self->posX.val;
            deltaY = entity->posY.val - self->posY.val;
            self->velocityX = deltaX / 96;
            self->velocityY = deltaY / 96;
            self->step++;
        }
        if (!(g_Timer & 0x1F)) {
            g_api.PlaySfxVolPan(SFX_RAPID_SYNTH_BUBBLE_SHORT, 0x50, 0);
        }
        break;
    case 2:
        AnimateEntity(D_pspeu_0926B408, self);
        MoveEntity();
        scale = 0x6000 / (self->ext.et_8019FCB4.timer + 0x18);
        self->scaleX = self->scaleY = scale;
        if (!(self->ext.et_8019FCB4.timer & 3)) {
            entity = AllocEntity(&g_Entities[192], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_ID(UNK_26), self, entity);
                entity->scaleX = entity->scaleY = self->scaleX;
                entity->zPriority = self->zPriority;
                entity->opacity = self->opacity;
            }
        }
        if (!(self->ext.et_8019FCB4.timer & 0xF)) {
            g_api.PlaySfxVolPan(
                SFX_RIC_SUC_REVIVE, 0x80 - self->ext.et_8019FCB4.timer / 2, 0);
        }
        if (!--self->ext.et_8019FCB4.timer) {
            g_api.PlaySfx(SFX_FIREBALL_SHOT_B);
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
    s32 var_v0;

    if (!self->step) {
        InitializeEntity(D_us_80180540);
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

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_E3B8", func_us_801A11DC);

void func_us_801A1B38(Entity* self) {
    Entity* real;

    if (D_pspeu_0926C060 & 1) {
        DestroyEntity(self);
        return;
    }

    if (!self->step) {
        InitializeEntity(D_us_80180540);
        self->blendMode = BLEND_ADD | BLEND_TRANSP;
    }

    AnimateEntity(D_pspeu_0926B4F8, self);

    real = self->ext.succubus.real;
    self->posX.i.hi = real->posX.i.hi;
    self->posY.i.hi = real->posY.i.hi + 0xC;

    if (D_pspeu_0926C098 == 5) {
        DestroyEntity(self);
    }
}
