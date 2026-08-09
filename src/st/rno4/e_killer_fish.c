// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"
// positions table lives inside the D_us_8018174C anim blob (reloc in the
// original asm resolves to 0x80181758 = D_us_8018174C + 0xC)
#define DEATH_PUFF_POSITIONS ((s16(*)[2]) & D_us_8018174C[0xC])

extern u8 D_us_8018172C[];
extern u8 D_us_80181738[];
extern u8 D_us_8018174C[];
extern u8 D_us_8018176C[];

extern EInit g_EInitKillerFish;
extern EInit g_EInitParticle;

#ifdef VERSION_PSP
extern s32 D_psp_E_KILLER_FISH_DEATH_PUFF;
#endif


// { posX.i.hi, posY.i.hi }

void EntityKillerFish(Entity* self) {
    Entity* entity;
    s16* ptr;
    s32 i;
    u16 params;

    if (self->flags & FLAG_DEAD && self->step != 4) {
        SetStep(4);
    }

    params = self->params;
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitKillerFish);
        self->facingLeft = params & 1;
        break;
    case 1:
        // Idle
        // nb. Interesting this is using the in-built rand() and not Random()
        if (!AnimateEntity(D_us_8018172C, self) && !(rand() & 3)) {
            SetStep(2);
            self->ext.killerFish.swimTimer = 0x100;
        }
        break;
    case 2:
        // Swim
        if (AnimateEntity(D_us_80181738, self) & 0x80 &&
            (self->pose == 3 || self->pose == 7)) {
            if (self->facingLeft) {
                self->velocityX = FIX(-1.5);
            } else {
                self->velocityX = FIX(1.5);
            }
        }
        if (self->velocityX != 0) {
            if (self->facingLeft) {
                self->velocityX += FIX(0.015625);
            } else {
                self->velocityX -= FIX(0.015625);
            }
        }
        MoveEntity();
        if (!--self->ext.killerFish.swimTimer) {
            self->velocityX = 0;
            SetStep(3);
        }
        break;
    case 3:
        // Finished swimming one direction
        if (!AnimateEntity(D_us_8018174C, self)) {
            if (self->ext.killerFish.swimCount++ & 1) {
                // On even numbered trips (left side), wait idle for a bit
                SetStep(1);
            } else {
                // Otherwise swim back the other direction
                self->ext.killerFish.swimTimer = 0x100;
                SetStep(2);
            }
            self->animCurFrame = 1;
            self->facingLeft ^= 1;
            if (self->facingLeft) {
                self->posX.i.hi -= 8;
            } else {
                self->posX.i.hi += 8;
            }
        }
        break;
    case 4:
        // Death
        PlaySfxPositional(SFX_EXPLODE_B);
        ptr = DEATH_PUFF_POSITIONS[0];

        // donor no4 has a real death_puff_positions[][2] array (LEN == 5);
        // here the table lives inside the D_us_8018176C blob behind a pointer
        // cast, where LEN() would degenerate to 1 — spell the count out
        for (i = 0; i < 5; i++) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity == NULL) {
                break;
            }
            CreateEntityFromCurrentEntity(E_ID(KILLER_FISH_DEATH_PUFF), entity);
            if (self->facingLeft) {
                entity->posX.i.hi += *ptr++;
            } else {
                entity->posX.i.hi -= *ptr++;
            }
            entity->posY.i.hi += *ptr++;
        }

        DestroyEntity(self);
        return;
    }

    params = self->animCurFrame;
    if (params == 9) {
        // Hitbox shrinks slightly in the middle of changing directions
        self->hitboxWidth = 6;
        self->hitboxOffX = -0xA;
    } else {
        self->hitboxWidth = 0x14;
        self->hitboxOffX = 0;
        if (params >= 10 && params < 13) {
            self->hitboxWidth = 0x10;
            self->hitboxOffX = 4;
        }
    }
    self->hitboxOffY = 2;
    self->hitboxHeight = 8;
}


void EntityKillerFishDeathPuff(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitParticle);
        self->pose = 0;
        self->poseTimer = 0;
        self->animSet = 0xE;
        self->unk5A = 0x79;
        self->palette = 0x2E8;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_OPACITY;
        self->opacity = 0x60;
        if (self->params & 0xFF00) {
            self->zPriority = (self->params & 0xFF00) >> 8;
        }
        self->velocityY += -0x8000 - 0x8000;
        return;
    }

    self->posY.val += self->velocityY;
    if (!AnimateEntity(D_us_8018176C, self)) {
        DestroyEntity(self);
    }
}
