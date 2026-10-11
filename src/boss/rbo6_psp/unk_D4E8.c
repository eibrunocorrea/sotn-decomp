// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo6/rbo6.h"

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

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A37B4);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A399C);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A3BE0);

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

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A9208_from_bo6);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019DB9C);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_8019EADC);

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

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A0710);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A0860);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A0AB4);

INCLUDE_ASM("boss/rbo6_psp/nonmatchings/rbo6_psp/unk_D4E8", func_us_801A0DC0);

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
