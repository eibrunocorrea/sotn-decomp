// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

// Donors: src/st/rare/e_minotaur.c and src/st/are/e_boss_torch.c (matched).
// EInits already live in this overlay's own e_init.c under their raw
// D_us_ names (not yet semantically renamed - that's the batizador's job,
// not mine) - extern those directly rather than inventing new names.
extern EInit D_us_80180524;
extern EInit D_us_80180548;
extern EInit D_us_80180554;
extern EInit g_EInitParticle;

// { velocity, angle } - table lives in bo2's own raw .data, extern only.
extern s32 D_us_80180A5C[][2];

#define anim_spit D_us_80180834
extern AnimateEntityFrame anim_spit[];
#define anim_death_puff D_us_80180864
extern AnimateEntityFrame anim_death_puff[];

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A2610);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A269C);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A3818);

// This entity appears unused in game as the entity step
// which triggers it to spawn is never called.
void EntityMinotaurFireball(Entity* self) {
    s32 angle;
    s32 speed;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180548);
        // Fireballs fly out in a sort of clock hands arrangement
        speed = D_us_80180A5C[self->params][0];
        angle = D_us_80180A5C[self->params][1];
        self->velocityX = (-speed * rcos(angle)) >> 0xC;
        self->velocityY = (-speed * rsin(angle)) >> 0xC;
        if (self->facingLeft) {
            self->velocityX = -self->velocityX;
        }
        // fallthrough
    case 1:
        MoveEntity();
        // Fireballs slowly fall over time
        self->velocityY += FIX(0.125);
        break;
    }
}

void EntityMinotaurSpitLiquid(Entity* self) {
    s16 angle;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180554);
        self->palette = PAL_FLAG(0x16B);
        if (self->facingLeft) {
            self->rotate = -self->rotate;
        }
        self->facingLeft = 0;
        // Spit flies slightly up and down randomly
        self->rotate += ROT(11.25) - Random();
        angle = self->rotate;
        self->velocityX = rsin(angle) * 0x20;
        self->velocityY = rcos(angle) * -0x20;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags =
            ENTITY_OPACITY | ENTITY_ROTATE | ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = 0x40;
        self->scaleY = 0x80;
        self->opacity = 0x80;
        break;
    case 1:
        MoveEntity();
        self->scaleX += 0x10;
        self->scaleY += 0xE;
        self->opacity -= 1;
        if (!AnimateEntity(anim_spit, self)) {
            DestroyEntity(self);
        }
        break;
    }
}

// A purple cloud which rises and fades away over time
void EntityMinotaurDeathPuff(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        self->animSet = ANIMSET_OVL(4);
        self->unk5A = 0x5C;
        self->palette = 0x21E;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleY = 0x180;
        self->scaleX = 0xA0;
        self->opacity = -0x80 - (self->params * 2);
        self->velocityY = FIX(-3.0);
        self->facingLeft = Random() & 1;
        // fallthrough
    case 1:
        MoveEntity();
        self->velocityY -= FIX(0.0625);
        self->opacity -= 2;
        self->scaleX += 8;
        self->scaleY -= 4;

        if (!self->opacity) {
            DestroyEntity(self);
        } else if (!AnimateEntity(anim_death_puff, self)) {
            DestroyEntity(self);
        }
        break;
    }
}

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A3E04);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A460C);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", func_us_801A4BA4);

#define anim_boss_torch D_us_80180B60
extern AnimateEntityFrame anim_boss_torch[];
// Torches are scaled slightly smaller based on params
// to appear further in the background.
extern s16 D_us_80180B6C[];

void EntityBossTorch(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180524);
        self->drawFlags = ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = D_us_80180B6C[self->params];
        // fallthrough
    case 1:
        AnimateEntity(anim_boss_torch, self);
        if (g_Timer & 4) {
            self->palette = (self->params * 2) + PAL_FLAG(0x5A);
        } else {
            self->palette = (self->params * 2) + PAL_FLAG(0x5B);
        }
        break;
    case 255:
#include "../../st/pad2_anim_debug.h"
    }
}

INCLUDE_ASM("boss/bo2/nonmatchings/unk_224DC", EntityBossDoors);
