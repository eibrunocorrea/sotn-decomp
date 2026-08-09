// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

extern u8 D_us_80182518[];

extern s16 D_us_801823FC[];
extern u8 D_us_801823EC[];
extern u8 D_us_80182404[];
extern u8 D_us_8018241C[];
extern u8 D_us_80182428[];
extern u8 D_us_80182430[];
extern u8 D_us_8018244C[];
extern u8 D_us_80182458[];
extern u8 D_us_80182468[];
extern u8 D_us_80182480[];
extern u8 D_us_8018249C[];
extern u8 D_us_801824E0[];
extern u8 D_us_80182500[];
extern u8 D_us_8018250C[];

// Donor: src/st/e_venus_weed.h (shared header, used verbatim -- with a
// "BLUE" define set -- by src/st/rno3/e_blue_venus_weed.c, matched). Not
// #include-d directly: EntityVenusWeedFlower's death-clut clamp is a raw
// literal (0x24f) in the header, not macro-driven, and rno4's palette
// layout needs 0x243 there (same -0xC offset confirmed on PLANT_CLUT/
// DEATH_CLUT below). Editing the shared header would change that literal
// for rno3/chi too, so the whole file is copied locally instead, with that
// one spot corrected.
#define BLUE
// rno4 has no gen/us/sprite_banks.h yet (sprite tables not extracted for
// this overlay); SPIKE_SPRITES is declared extern under a guessed name
// mirroring rno3's numbering. Harmless for strict-fn-diff: it only ever
// appears behind a %hi/%lo reloc.
#define SPIKE_SPRITES D_us_801BE9C4
#define TENDRIL_COUNT 8
// rno4's palette layout is rno3's minus 0xC across the board (confirmed via
// strict-fn-diff CONST feedback on EntityVenusWeed's PLANT_CLUT/DEATH_CLUT
// uses, and reused below for the DART/SPIKE_CLUT bounds + the death-clut
// literal fixed directly in EntityVenusWeedFlower).
#define DART_CLUT_START 0x234
#define DEATH_CLUT 0x23A
#define PLANT_CLUT 0x244
#define SPIKE_CLUT_START 0x24E
#define SPIKE_CLUT_END 0x25C
#define GROWTH_SPEED 2

extern EInit g_EInitBlueVenusWeed2;
extern EInit D_us_80180B9C;
extern EInit D_us_80180BA8;
extern EInit g_EInitBlueVenusWeed1;

extern signed short* SPIKE_SPRITES[];

static Primitive* SetupPrimsForEntitySpriteParts(
    Entity* entity, Primitive* prim) {
    s16 y;
    s32 spritePartCount;
    s16 x;
    u8 spriteU0;
    u8 spriteV0;
    s16* spriteData;
    s32 i;
    u8 spriteU1;
    s16 spriteDestX;
    s16 spriteDestY;
    s16 spriteDestW;
    s16 spriteDestH;
    s16 spriteFlags;
    u8 spriteV1;
    s32 xFlip;

    spriteData = SPIKE_SPRITES[entity->animCurFrame];
    spritePartCount = *spriteData;
    spriteData++;

    for (i = 0; i < spritePartCount; i++, spriteData += 11) {
        spriteFlags = spriteData[0];
        spriteDestX = spriteData[1];
        spriteDestY = spriteData[2];
        spriteDestW = spriteData[3];
        spriteDestH = spriteData[4];

        // Adjust sprite position to respect sprite flags
        if (spriteFlags & 4) {
            spriteDestW -= 1;
            if (spriteFlags & 2) {
                spriteDestX += 1;
            }
        }
        if (spriteFlags & 8) {
            spriteDestH -= 1;
            if (spriteFlags & 1) {
                spriteDestY += 1;
            }
        }
        if (spriteFlags & 0x10) {
            spriteDestW -= 1;
            if (!(spriteFlags & 2)) {
                spriteDestX += 1;
            }
        }
        if (spriteFlags & 0x20) {
            spriteDestH -= 1;
            if (!(spriteFlags & 1)) {
                spriteDestY += 1;
            }
        }

        // Calculate sprite position to respect facing
        x = entity->posX.i.hi;
        y = entity->posY.i.hi;
        if (entity->facingLeft) {
            x -= spriteDestX;
        } else {
            x += spriteDestX;
        }
        y += spriteDestY;

        // Set sprite position to respect the above, plus sprite dimensions
        if (entity->facingLeft) {
            LOH(prim->x0) = x - spriteDestW + 1;
            LOH(prim->y0) = y;
            LOH(prim->x1) = x + 1;
            LOH(prim->y1) = y;
            LOH(prim->x2) = x - spriteDestW + 1;
            LOH(prim->y2) = y + spriteDestH;
            LOH(prim->x3) = x + 1;
            LOH(prim->y3) = y + spriteDestH;
        } else {
            LOH(prim->x0) = x;
            LOH(prim->y0) = y;
            LOH(prim->x1) = x + spriteDestW;
            LOH(prim->y1) = y;
            LOH(prim->x2) = x;
            LOH(prim->y2) = y + spriteDestH;
            LOH(prim->x3) = x + spriteDestW;
            LOH(prim->y3) = y + spriteDestH;
        }

        // Entity-relative clut
        prim->clut = entity->palette + spriteData[5];

        spriteU0 = spriteData[7];
        spriteV0 = spriteData[8];
        spriteU1 = spriteData[9];
        spriteV1 = spriteData[10];

        // Adjust sprite UVs to respect sprite flags
        if (spriteFlags & 4) {
            spriteU1--;
        }
        if (spriteFlags & 8) {
            spriteV1--;
        }
        if (spriteFlags & 0x10) {
            spriteU0++;
        }
        if (spriteFlags & 0x20) {
            spriteV0++;
        }

        // Set sprite UVs to respect the above, plus facing
        xFlip = (spriteFlags & 2) ^ entity->facingLeft;
        if (!xFlip) {
            if (!(spriteFlags & 1)) {
                prim->u0 = spriteU0;
                prim->v0 = spriteV0;
                prim->u1 = spriteU1;
                prim->v1 = spriteV0;
                prim->u2 = spriteU0;
                prim->v2 = spriteV1;
                prim->u3 = spriteU1;
                prim->v3 = spriteV1;
            } else {
                prim->u0 = spriteU0;
                prim->v0 = spriteV1 - 1;
                prim->u1 = spriteU1;
                prim->v1 = spriteV1 - 1;
                prim->u2 = spriteU0;
                prim->v2 = spriteV0 - 1;
                prim->u3 = spriteU1;
                prim->v3 = spriteV0 - 1;
            }
        } else {
            if (!(spriteFlags & 1)) {
                prim->u0 = spriteU1 - 1;
                prim->v0 = spriteV0;
                prim->u1 = spriteU0 - 1;
                prim->v1 = spriteV0;
                prim->u2 = spriteU1 - 1;
                prim->v2 = spriteV1;
                prim->u3 = spriteU0 - 1;
                prim->v3 = spriteV1;
            } else {
                prim->u0 = spriteU1 - 1;
                prim->v0 = spriteV1 - 1;
                prim->u1 = spriteU0 - 1;
                prim->v1 = spriteV1 - 1;
                prim->u2 = spriteU1 - 1;
                prim->v2 = spriteV0 - 1;
                prim->u3 = spriteU0 - 1;
                prim->v3 = spriteV0 - 1;
            }
        }

        prim->tpage = 0x14;
        // Entity-relative z priority
        prim->priority = entity->zPriority + 1;

        // Next!
        prim = prim->next;
    }
    return prim;
}

typedef enum VenusWeedStep {
    VENUS_WEED_INIT,
    VENUS_WEED_DROP_TO_GROUND,
    VENUS_WEED_THORNWEED_DISGUISE,
    VENUS_WEED_GROW,
    VENUS_WEED_IDLE,
    VENUS_WEED_ATTACK,
    VENUS_WEED_DEATH = 8,
};

typedef enum VenusWeedTendrilStep {
    VENUS_WEED_TENDRIL_INIT,
    VENUS_WEED_TENDRIL_DROP_TO_GROUND,
    VENUS_WEED_TENDRIL_MOVE_TO_RANDOM_POSITION,
    VENUS_WEED_TENDRIL_ATTACK,
    VENUS_WEED_TENDRIL_UNUSED4,
    VENUS_WEED_TENDRIL_STEP5,
    VENUS_WEED_TENDRIL_DEATH = 8,
};

typedef enum VenusWeedTendrilAttack_Substep {
    VENUS_WEED_TENDRIL_ATTACK_INIT,
#if !defined(BLUE)
    VENUS_WEED_TENDRIL_ATTACK_DELAY,
#endif
    VENUS_WEED_TENDRIL_ATTACK_CHARGE,
    VENUS_WEED_TENDRIL_ATTACK_LAUNCH,
};

// clang-format off


// Whips her hair around as she comes out of the flower







# if defined(BLUE)
#endif








// clang-format on

void EntityVenusWeed(Entity* self) {
    // Sprites
    const int SpriteLeavesX = 0x48;
    const int SpriteLeavesY = 0x00;
    const int SpriteLeavesW = 0x38;
    const int SpriteLeavesH = 0x22;
    const int SpriteStemX = 0x00;
    const int SpriteStemY = 0x30;
    const int SpriteStemW = 0x18;
    const int SpriteStemH = 0x22;
    // Behaviour
#if defined(BLUE)
    const int ActivateDistanceX = 0xA0;
#else
    const int ActivateDistanceX = 0x70;
#endif
    const int LeavesWidthMax = 0x38;
    const int LeavesHeightMax = 0x22;
    const int StemWidthMax = 0xC;
    const int StemHeightMax = 0x22;
    const int FlowerOffsetY = 0x1B;
    const int WiggleLeavesSpeed = 0x180;
    const int AttackDuration = 0x30;
    const int DeathFinalClut = DEATH_CLUT;

    typedef enum Grow_Substep {
        GROW_LEAVES = 0,
        GROW_STEM = 1,
        GROW_FLOWER = 2,
        GROW_TENDRILS = 3,
        GROW_DONE = 4,
    };

    typedef enum Death_Substep {
        DEATH_INIT = 0,
        DEATH_COLOR_CYCLE = 1,
        DEATH_SHRINK = 2,
        DEATH_DONE = 3,
    };

    Entity* entity;
    s32 x;
    s32 primIdx;
    s32 y;
    Primitive* prim;
    s32 checkCount;
    s32 i;
    s16 rot;

    // Death check
    if ((self->flags & FLAG_DEAD) && (self->step < VENUS_WEED_DEATH)) {
        SetStep(VENUS_WEED_DEATH);
    }

    switch (self->step) {
    case VENUS_WEED_INIT:
        InitializeEntity(g_EInitBlueVenusWeed1);
        self->hitboxOffX = 1;
        self->hitboxOffY = -7;
#if defined(BLUE)
        self->zPriority -= 8;
#endif
        // 3 Prims: 2x Leaves (left/right) + Stem
        primIdx = g_api.AllocPrimitives(PRIM_GT4, 3);
        if (primIdx == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIdx;
        prim = &g_PrimBuf[primIdx];
        self->ext.prim = prim;

        // Leaves
        for (i = 0; i < 2; i++) {
            prim->tpage = 0x14;
            prim->clut = PLANT_CLUT;
            prim->u0 = prim->u2 = SpriteLeavesX;
            prim->u1 = prim->u3 = SpriteLeavesX + SpriteLeavesW;
            prim->v0 = prim->v1 = SpriteLeavesY;
            prim->v2 = prim->v3 = SpriteLeavesY + SpriteLeavesH;
            prim->priority = self->zPriority - 1;
            prim->drawMode = DRAW_HIDE;

            prim = prim->next;
        }

        // Stem
        self->ext.venusWeed.stemPrim = prim;
        prim->tpage = 0x14;
        prim->clut = PLANT_CLUT;
        prim->u0 = prim->u2 = SpriteStemX;
        prim->u1 = prim->u3 = SpriteStemX + SpriteStemW;
        prim->v0 = prim->v1 = SpriteStemY;
        prim->v2 = prim->v3 = SpriteStemY + SpriteStemH;
        prim->priority = self->zPriority - 2;
        prim->drawMode = DRAW_HIDE;

        prim = prim->next;
        break;

    case VENUS_WEED_DROP_TO_GROUND:
        if (UnkCollisionFunc3(&D_us_801823EC) & 1) {
            SetStep(VENUS_WEED_THORNWEED_DISGUISE);
        }
        break;

    case VENUS_WEED_THORNWEED_DISGUISE:
        AnimateEntity(&D_us_8018250C, self);
        if (GetDistanceToPlayerX() < ActivateDistanceX) {
            self->hitboxState = 0;
            SetStep(VENUS_WEED_GROW);
        }

        // Death check
        if (self->flags & FLAG_DEAD) {
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, entity);
                entity->posY.i.hi -= 4;
                entity->params = 0;
            }

            PlaySfxPositional(SFX_STUTTER_EXPLODE_LOW);
            DestroyEntity(self);
            return;
        }
        break;

    case VENUS_WEED_GROW:
        AnimateEntity(&D_us_80182518, self);

        checkCount = 0;
        switch (self->step_s) {
        case GROW_LEAVES:
            // Update leaves width
            self->ext.venusWeed.leavesWidth += GROWTH_SPEED;
            if (self->ext.venusWeed.leavesWidth > LeavesWidthMax) {
                self->ext.venusWeed.leavesWidth = LeavesWidthMax;
                checkCount += 1;
            }

            // Update leaves height
            self->ext.venusWeed.leavesHeight += GROWTH_SPEED;
            if (self->ext.venusWeed.leavesHeight > LeavesHeightMax) {
                self->ext.venusWeed.leavesHeight = LeavesHeightMax;
                checkCount += 1;
            }

            // Update prims to match
            prim = self->ext.prim;
            x = self->posX.i.hi;
            y = self->posY.i.hi;
            y -= self->ext.venusWeed.leavesHeight;
            // 2 primitives: One for left and one for right
            for (i = -1; i < 2; i += 2) {
                prim->x0 = prim->x2 = x;
                prim->x1 = prim->x3 = x + self->ext.venusWeed.leavesWidth * i;
                prim->y0 = prim->y1 = y;
                prim->y2 = prim->y3 = self->posY.i.hi;
                prim->drawMode = DRAW_UNK02;

                prim = prim->next;
            }

            // Check for completion
            if (checkCount == 2) {
                self->step_s++;
            }
            break;

        case GROW_STEM:
            // Update stem width
            self->ext.venusWeed.stemWidth += GROWTH_SPEED;
            if (self->ext.venusWeed.stemWidth > StemWidthMax) {
                self->ext.venusWeed.stemWidth = StemWidthMax;
                checkCount += 1;
            }

            // Update stem height
            self->ext.venusWeed.stemHeight += GROWTH_SPEED;
            if (self->ext.venusWeed.stemHeight > StemHeightMax) {
                self->ext.venusWeed.stemHeight = StemHeightMax;
                checkCount += 1;
            }

            // Update prim to match
            prim = self->ext.venusWeed.stemPrim;
            x = self->posX.i.hi;
            y = self->posY.i.hi - self->ext.venusWeed.stemHeight;
            prim->x0 = prim->x2 = x - self->ext.venusWeed.stemWidth;
            prim->x1 = prim->x3 = x + self->ext.venusWeed.stemWidth;
            prim->y0 = prim->y1 = y;
            prim->y2 = prim->y3 = self->posY.i.hi;
            prim->drawMode = DRAW_UNK02;

            // Check for completion
            if (checkCount == 2) {
                self->step_s++;
            }
            break;

        case GROW_FLOWER:
            entity = self + 1; // Flower

            // Spawn flower
            CreateEntityFromCurrentEntity(E_VENUS_WEED_FLOWER, entity);
            entity->posX.i.hi = self->posX.i.hi;
            entity->posY.i.hi = self->posY.i.hi - FlowerOffsetY;

            // Face the player
            entity->facingLeft = GetSideToPlayer() & 1;
            entity->zPriority = (s32)self->zPriority;

            self->step_s++;
            break;

        case GROW_TENDRILS:
            entity = self + 2; // Tendrils start
            for (i = 0; i < TENDRIL_COUNT; i++, entity++) {
                CreateEntityFromCurrentEntity(E_VENUS_WEED_TENDRIL, entity);
                entity->params = i;
                entity->zPriority = self->zPriority + 1;
            }

            self->step_s++;
            break;

        case GROW_DONE:
            break;
        }
        break;

    case VENUS_WEED_IDLE: // Set by the flower entity (self + 1)
        AnimateEntity(&D_us_8018250C, self);
        break;

    case VENUS_WEED_ATTACK: // Set by the flower entity (self + 1)
        if (self->ext.venusWeed.triggerAttack) {
            self->ext.venusWeed.triggerAttack = false;
            self->ext.venusWeed.timer = AttackDuration;
        }
        if (self->ext.venusWeed.timer) {
            AnimateEntity(&D_us_80182518, self);
            self->ext.venusWeed.timer--;
        }
        break;

    case VENUS_WEED_DEATH:
        switch (self->step_s) {
        case DEATH_INIT:
            self->ext.venusWeed.wiggleT = 0;
            self->step_s++;
            // fallthrough
        case DEATH_COLOR_CYCLE:
            // Cycle thru cluts
            if (!(g_Timer & 7)) {
                // Switch to next clut
                self->palette += 1;
                // For primitives too
                prim = self->ext.prim;
                while (prim != NULL) {
                    prim->clut += 1;
                    prim = prim->next;
                }
                if (self->palette == DeathFinalClut) {
                    self->step_s++;
                }
            }
            break;

        case DEATH_SHRINK:
            checkCount = 0;
            self->ext.venusWeed.timer++;
            // Every other frame
            if (self->ext.venusWeed.timer & 1) {
                prim = self->ext.prim;
                x = self->posX.i.hi;

                // Shrink leaves
                if (self->ext.venusWeed.leavesWidth) {
                    self->ext.venusWeed.leavesWidth--;
                    if (self->ext.venusWeed.leavesWidth < 0) {
                        self->ext.venusWeed.leavesWidth = 0;
                    }
                }
                // Update leaves sprites
                for (i = -1; i < 2; i += 2) {
                    // 0x38 is LeavesWidthMax
                    // Using a const here changes the registers on PSP
                    prim->x1 =
                        x + (self->ext.venusWeed.leavesWidth + 0x38) / 2 * i;
                    prim->x3 = x + (self->ext.venusWeed.leavesWidth * i);
                    prim->y0++;
                    prim->y1++;
                    if (prim->y1 > prim->y2) {
                        prim->drawMode = DRAW_HIDE;
                        checkCount += 1;
                    }
                    prim = prim->next;
                }
            }

            // Collapse stem
            prim = self->ext.venusWeed.stemPrim;
            prim->y0 = ++prim->y1;
            if (prim->y0 > prim->y2) {
                prim->drawMode = DRAW_HIDE;
                checkCount += 1;
            }

            // Check for completion
            if (checkCount == 3) {
                self->step_s += 1;
            }
            break;

        case DEATH_DONE:
            // Spawn explosion
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, entity);
                entity->params = 2;
                entity->posY.i.hi -= 0xC;
            }

            PlaySfxPositional(SFX_EXPLODE_B);

            // Destroy
            PreventEntityFromRespawning(self);
            DestroyEntity(self);
            return;
        }
    }

    // Update wiggle
    if (self->ext.venusWeed.wiggleT) {
        rot = self->rotate;
        self->rotate += WiggleLeavesSpeed;
        x = rcos(rot) * 3 >> 0xC;
        y = rsin(rot) * 3 >> 0xC;
        prim = self->ext.prim;

        // Update leaves
        for (i = -1; i < 2; i += 2) {
            // 0x38 is LeavesWidthMax
            // Using a const here changes the registers on PSP
            prim->x1 = self->posX.i.hi + (x + 0x38) * i;
            prim->y1 = self->posY.i.hi - LeavesHeightMax + y * i;

            prim = prim->next;
        }

        // Update stem and flower
        x /= 2;
        entity = self + 1; // Flower
        prim = self->ext.venusWeed.stemPrim;
        self->ext.venusWeed.wiggleT--;
        if (!self->ext.venusWeed.wiggleT) {
            entity->posX.i.hi = self->posX.i.hi;
            prim->x0 = self->posX.i.hi - StemWidthMax;
            prim->x1 = self->posX.i.hi + StemWidthMax;
        } else {
            // 0xC is StemWidthMax
            // Using a const here changes the registers on PSP
            prim->x0 = self->posX.i.hi - 0xC + x;
            prim->x1 = self->posX.i.hi + 0xC + x;
            entity->posX.i.hi = self->posX.i.hi + x;
        }
    }
}

void EntityVenusWeedFlower(Entity* self) {
    const int HitboxOffsetX = 6;
    const int HitboxOffsetY = -16;
    const int HitboxWidth = 14;
    const int HitboxHeight = 14;
    const int AnimFrameInit = 1;
    const int GrowSpeed = 6;
    const int GrowLimit = 0x100;
    const int DartsSfxpose = 3;
    const int DartsLaunchPosOffsetX = 0x18;
    const int DartsLaunchPosOffsetY = 0x18;
    const int DartsAngleLeft = 0x800;
    const int DartsAngleMaxUp = 0x380;
    const int DartsAngleMaxDown = 0x300;
    const int DartsAngleDelta = 0x60;
    const int DartsCount = 5;

    typedef enum Step {
        INIT,
        GROW,
        REVEAL,
        IDLE,
        SPIKES,
        DARTS,
        DEATH = 8,
    };

    typedef enum Spikes_Substep {
        SPIKES_INIT,
#if defined(BLUE)
        SPIKES_1,
#endif
        SPIKES_CHARGE,
        SPIKES_SPAWN,
        SPIKES_LAUNCH,
        SPIKES_ANIM_RESET,
        SPIKES_RESET_TO_IDLE,
    };

    typedef enum Darts_Substep {
        DARTS_INIT,
        DARTS_DELAY,
        DARTS_CHARGE,
        DARTS_LAUNCH,
        DARTS_RESET_TO_IDLE,
    };

    Entity* entity;
    s32 x;
    s16 rot;
    s32 i;
    s32 rotDelta;
    s32 spikeStartTimeOffsetIndex;
    s32 y;

#if defined(BLUE)
    FntPrint("arla_step %x\n", self->step);
    FntPrint("arla_color %x\n", self->palette);
#endif

    // Hurt check
    if (self->hitFlags & 3) {
        PlaySfxPositional(SFX_VENUS_WEED_HURT);

        // Tell root to wiggle for a bit
        entity = self - 1; // Root
        entity->ext.venusWeed.wiggleT = 0x40;
    }
    // Death check
    if ((self->flags & FLAG_DEAD) && (self->step < DEATH)) {
        PlaySfxPositional(SFX_VENUS_WEED_DEATH);
        SetStep(DEATH);
    }

    switch (self->step) {
    case INIT:
        InitializeEntity(g_EInitBlueVenusWeed2);
        self->hitboxOffX = HitboxOffsetX;
        self->hitboxOffY = HitboxOffsetY;
        self->hitboxWidth = HitboxWidth;
        self->hitboxHeight = HitboxHeight;
        self->animCurFrame = AnimFrameInit;
        self->drawFlags |= ENTITY_SCALEX | ENTITY_SCALEY;
        self->scaleX = self->scaleY = 0;
        self->hitboxState = 0;
        break;

    case GROW:
        self->scaleX = self->scaleY += GrowSpeed;
        if (self->scaleX >= GrowLimit) {
            self->drawFlags = ENTITY_DEFAULT;
            self->hitboxState = 3;

            PlaySfxPositional(SFX_MAGIC_WEAPON_APPEAR_A);
            SetStep(REVEAL);
        }
        break;

    case REVEAL:
        if (AnimateEntity(D_us_80182404, self) == 0) {
            // Tell root to idle
            entity = self - 1; // Root
            entity->step = VENUS_WEED_IDLE;
            entity->step_s = 0;

            SetStep(IDLE);
        }
        break;

    case IDLE:
        // Init
        if (!self->step_s) {
            self->ext.venusWeedFlower.triggerAttack = 1;
            self->step_s++;
        }

        // Animate, occasionally turning to face player
        if (AnimateEntity(D_us_8018244C, self) == 0) {
            self->facingLeft = GetSideToPlayer() & 1;
        }

        // Only once, when entering IDLE state
        if (!--self->ext.venusWeedFlower.triggerAttack) {
            // Face player
            self->facingLeft = GetSideToPlayer() & 1;
            // Blue chooses by distance, non-blue alternates
#if defined(BLUE)
            SetStep(DARTS);
            if (GetDistanceToPlayerX() < 64) {
                SetStep(SPIKES);
            }
#else
            if (self->ext.venusWeedFlower.nextAttackIsDarts) {
                SetStep(DARTS);
            } else {
                SetStep(SPIKES);
            }
            // Toggle between darts and tendril spikes attacks
            self->ext.venusWeedFlower.nextAttackIsDarts ^= 1;
#endif
        }
        break;

    case SPIKES:
        switch (self->step_s) {
        case SPIKES_INIT:

#if defined(BLUE)
            self->ext.venusWeedFlower.unk93 = 0;
#else
            // Set root entity to attack
            entity = self - 1; // Root
            entity->step = VENUS_WEED_ATTACK;
            entity->step_s = 0;
#endif

            // Set tendrils to attack
            entity = self + 1; // Tendrils start
            for (i = 0; i < TENDRIL_COUNT; i++, entity++) {
#if defined(BLUE)
                entity->ext.venusWeedFlower.unk93 = 1;
#else
                entity->step = VENUS_WEED_TENDRIL_ATTACK;
                entity->step_s = VENUS_WEED_TENDRIL_ATTACK_INIT;
#endif
            }

            self->step_s++;
            // fallthrough
#if defined(BLUE)
        case SPIKES_1:
            AnimateEntity(D_us_8018244C, self);
            if (self->ext.venusWeedFlower.unk93 == 8) {
                entity = self - 1;
                entity->step = 5;
                entity->step_s = 0;
                SetSubStep(SPIKES_CHARGE);
            }
            break;
#endif
        case SPIKES_CHARGE:
            if (!AnimateEntity(D_us_8018241C, self)) {
                SetSubStep(SPIKES_SPAWN);
            }
            break;
        case SPIKES_SPAWN:
            PlaySfxPositional(SFX_GLASS_SHARDS);

            // Spawn spikes
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_VENUS_WEED_SPIKE, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->ext.venusWeedSpike.flower = self;
            }
            self->step_s++;
            // fallthrough
        case SPIKES_LAUNCH:
            if (AnimateEntity(D_us_80182428, self) == 0) {
                entity = self + 1; // Tendrils start
#if !defined(BLUE)
                if (g_Timer & 1) {
                    spikeStartTimeOffsetIndex = 0;
                } else {
                    spikeStartTimeOffsetIndex = 4;
                }
#endif
                for (i = 0; i < TENDRIL_COUNT; i++, entity++) {
#if defined(BLUE)
                    entity->ext.venusWeedTendril.spikeStartTimeOffsetIndex = 1;
#else
                    entity->ext.venusWeedTendril.spikeStartTimeOffsetIndex =
                        spikeStartTimeOffsetIndex + 1;
                    spikeStartTimeOffsetIndex++;
                    spikeStartTimeOffsetIndex &= 0x7;
#endif
                }
                SetSubStep(SPIKES_ANIM_RESET);
            }
            break;

        case SPIKES_ANIM_RESET: // Anim: Reset to idle
            if (AnimateEntity(D_us_80182430, self) == 0) {
                SetSubStep(SPIKES_RESET_TO_IDLE);
            }
            break;

        case SPIKES_RESET_TO_IDLE:
            // Tell root to idle
            entity = self - 1; // Root
            entity->step = VENUS_WEED_IDLE;

            SetStep(IDLE);
            break;
        }
#if !defined(BLUE)
        // Cycle clut; this block is in different places on the two versions
        if (self->ext.venusWeedFlower.clutOffset) {
            entity = self - 1; // Root
            entity->ext.venusWeed.triggerAttack = true;
            if (!(self->palette & PAL_UNK_FLAG)) {
                self->palette += self->ext.venusWeedFlower.clutOffset;
                if (self->palette > 0x219) {
                    self->palette = 0x219;
                }
                self->ext.venusWeedFlower.clutOffset = 0;
                return;
            }
        }
#endif
        break;

    case DARTS:
        switch (self->step_s) {
        case DARTS_INIT:
            entity = self - 1; // Root
            entity->step = 6;  // Non-existent state: "Do nothing"
            entity->step_s = 0;

            self->step_s += 1;
            // fallthrough
        case DARTS_DELAY:
            if (AnimateEntity(D_us_80182458, self) == 0) {
                SetSubStep(DARTS_CHARGE);
            }
            break;

        case DARTS_CHARGE:
            PlaySfxPositional(SFX_GLASS_SHARDS);
            entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (entity != NULL) {
                CreateEntityFromEntity(E_VENUS_WEED_SPIKE, self, entity);
                entity->facingLeft = self->facingLeft;
                entity->ext.venusWeedSpike.flower = self;
            }
            self->step_s++;
            // fallthrough
        case DARTS_LAUNCH:
            if (AnimateEntity(D_us_80182468, self) == 0) {
                self->step_s++;
            }
            if (!self->poseTimer && self->pose == DartsSfxpose) {
                PlaySfxPositional(SFX_ARROW_SHOT_B);

                // Calculate launch start pos
                if (self->facingLeft) {
                    x = self->posX.i.hi - DartsLaunchPosOffsetX;
                } else {
                    x = self->posX.i.hi + DartsLaunchPosOffsetX;
                }
                y = self->posY.i.hi - DartsLaunchPosOffsetY;

                // Calculate launch angle and delta between darts
                entity = &PLAYER;
                rot = ratan2(entity->posY.i.hi - y, entity->posX.i.hi - x);
                if (self->facingLeft) {
                    if (rot < 0) { // Angled up
                        if (rot > -DartsAngleLeft + DartsAngleMaxUp) {
                            rot = -DartsAngleLeft + DartsAngleMaxUp;
                        }
                        rotDelta = -DartsAngleDelta; // More up
                    } else {                         // Angled down
                        if (rot < DartsAngleLeft - DartsAngleMaxDown) {
                            rot = DartsAngleLeft - DartsAngleMaxDown;
                        }
                        rotDelta = DartsAngleDelta; // More down
                    }
                } else if (rot < 0) { // Angled up
                    if (rot < -DartsAngleMaxUp) {
                        rot = -DartsAngleMaxUp;
                    }
                    rotDelta = DartsAngleDelta; // More up
                } else {                        // Angled down
                    if (rot > DartsAngleMaxDown) {
                        rot = DartsAngleMaxDown;
                    }
                    rotDelta = -DartsAngleDelta; // More down
                }

                // Spawn darts
                for (i = 0; i < DartsCount; i++) {
                    entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                    if (entity != NULL) {
                        CreateEntityFromEntity(E_VENUS_WEED_DART, self, entity);
                        entity->rotate = rot;
                        entity->params = i;
                        entity->posX.i.hi = x;
                        entity->posY.i.hi -= DartsLaunchPosOffsetY;
                    }
                    rot += rotDelta;
                }
            }
            break;

        case DARTS_RESET_TO_IDLE:
            entity = self - 1; // Root
            entity->step = VENUS_WEED_IDLE;
            SetStep(IDLE);
        }
        break;

    case DEATH:
        // Kill tendrils
        entity = self + 1; // Tendrils start
        for (i = 0; i < TENDRIL_COUNT; i++, entity++) {
            entity->flags |= FLAG_DEAD;
        }

        PlaySfxPositional(SFX_FM_EXPLODE_B);
        self->hitboxState = 0;

        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 3;
        }

        // Kill root
        entity = self - 1; // Root
        entity->flags |= FLAG_DEAD;

        DestroyEntity(self);
        return;
    }
#if defined(BLUE)
    // Cycle clut; this block is in different places on the two versions
    if (self->ext.venusWeedFlower.clutOffset) {
        entity = self - 1; // Root
        entity->ext.venusWeed.triggerAttack = true;
        if (!(self->palette & PAL_UNK_FLAG)) {
            self->palette += self->ext.venusWeedFlower.clutOffset >> 4;
            self->ext.venusWeedFlower.clutOffset &= 0xF;
            // rno4 fix: donor (rno3) hardcodes 0x24f here; rno4's palette
            // layout needs 0x243 (confirmed via strict-fn-diff), matching
            // the same -0xC offset as PLANT_CLUT/DEATH_CLUT above.
            if (self->palette > 0x243) {
                self->palette = 0x243;
            }
        }
    }
#endif
}

INCLUDE_ASM("st/rno4/nonmatchings/e_blue_venus_weed", EntityVenusWeedTendril);

void EntityVenusWeedDart(Entity* self) {
    const int AnimFrameIndexInit = 0x37;
    const int StartSpeed = 0x8000;
    const int SpeedMax = 0x60000;
    const int AccelInc = 0x800;
    const int AccelMax = 0x10000;
    const int ClutIdxWallHit = 0x20;
    const int ClutIdxPlayerHit = 0x00;
    const int ClutIdxMax = 0x30;

    typedef enum Step {
        INIT = 0,
        FLY = 1,
        DECAY = 2,
        DEATH = 3,
    };

    Collider collider;
    Entity* entity;
    s16 rot;
    s32 x;
    s32 speed;
    s32 y;

    switch (self->step) {
    case INIT:
        InitializeEntity(D_us_80180BA8);
        self->animCurFrame = AnimFrameIndexInit;
        self->drawFlags = ENTITY_ROTATE;
        rot = self->rotate;
        self->hitboxOffX = (rcos(rot) * 6) >> 0xC;
        self->hitboxOffY = (rsin(rot) * 6) >> 0xC;
        self->ext.venusWeedDart.nextPosDeltaX = rcos(rot) << 3 >> 0xC;
        self->ext.venusWeedDart.nextPosDeltaY = rsin(rot) << 3 >> 0xC;
        self->ext.venusWeedDart.speed = StartSpeed;
        // fallthrough
    case FLY:
        MoveEntity();

        rot = self->rotate;
        speed = self->ext.venusWeedDart.speed;
        self->velocityX = (speed * rcos(rot)) >> 0xC;
        self->velocityY = (speed * rsin(rot)) >> 0xC;
        self->ext.venusWeedDart.speed += self->ext.venusWeedDart.accel;
        self->ext.venusWeedDart.accel += (self->params + 1) * AccelInc;
        if (self->ext.venusWeedDart.accel > AccelMax) {
            self->ext.venusWeedDart.accel = AccelMax;
        }
        if (self->ext.venusWeedDart.speed > SpeedMax) {
            self->ext.venusWeedDart.speed = SpeedMax;
        }

        x = self->posX.i.hi + self->ext.venusWeedDart.nextPosDeltaX;
        y = self->posY.i.hi + self->ext.venusWeedDart.nextPosDeltaY;
        g_api.CheckCollision(x, y, &collider, 0);
        if (collider.effects & EFFECT_SOLID) {
            PlaySfxPositional(SFX_STOMP_HARD_E);
            // Correct position to be against the edge
            if (self->velocityY > 0) {
                self->posY.i.hi += collider.unk18;
            }
            if (self->velocityY < 0) {
                self->posY.i.hi += collider.unk20;
            }
            self->hitboxState = 0;
            self->ext.venusWeedDart.clutIndex = ClutIdxWallHit;
            SetStep(DEATH);
        }
        if (self->hitFlags & 0x80) {
            entity = &PLAYER;
            self->ext.venusWeedDart.nextPosDeltaX =
                entity->posX.i.hi - self->posX.i.hi;
            self->ext.venusWeedDart.nextPosDeltaY =
                entity->posY.i.hi - self->posY.i.hi;
            self->ext.venusWeedDart.clutIndex = ClutIdxPlayerHit;
            self->hitboxState = 0;
            SetStep(DECAY);
            break;
        }
        if (self->hitParams) {
            self->flags & FLAG_DEAD; // Weird?
        }
        break;

    case DECAY:
        if (!(self->palette & PAL_UNK_FLAG)) {
            self->ext.venusWeedDart.clutIndex++;
            self->palette = self->ext.venusWeedDart.clutIndex + DART_CLUT_START;

            if (self->palette > PLANT_CLUT - 1) {
                self->palette = PLANT_CLUT - 1;
            }
        }
        if (self->ext.venusWeedDart.clutIndex > ClutIdxMax) {
            self->flags |= FLAG_DEAD;
        }

        // Stick to player
        entity = &PLAYER;
        self->posX.i.hi =
            entity->posX.i.hi - self->ext.venusWeedDart.nextPosDeltaX;
        self->posY.i.hi =
            entity->posY.i.hi - self->ext.venusWeedDart.nextPosDeltaY;
        break;

    case DEATH:
        if (!--self->ext.venusWeedDart.clutIndex) {
            self->flags |= FLAG_DEAD;
        }
        break;
    }

    // Death check
    if (self->flags & FLAG_DEAD) {
        entity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (entity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, entity);
            entity->params = 0;
        }
        DestroyEntity(self);
    }
}

void EntityVenusWeedSpike(Entity* self) {
    const int SpikeParts = 5;

    typedef enum Step {
        INIT = 0,
        EXTEND = 1,
    };

    Primitive* prim;
    Primitive* primItr;
    Primitive* primNext;
    s32 primIdx;
    Entity* entity;
    s16 clut;

    switch (self->step) {
    case INIT:
        InitializeEntity(g_EInitBlueVenusWeed2);

        self->flags |= FLAG_UNK_2000 | FLAG_UNK_00200000;
        self->hitboxState = 0;
        self->palette = PAL_FLAG(SPIKE_CLUT_START);

        primIdx = g_api.AllocPrimitives(PRIM_GT4, SpikeParts);
        if (primIdx == -1) {
            DestroyEntity(self);
            break;
        } else {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIdx;
            prim = &g_PrimBuf[primIdx];
            self->ext.venusWeedSpike.firstPart = prim;

            entity = self->ext.venusWeedSpike.flower;
            entity--; // Root
            prim = self->ext.venusWeedSpike.firstPart;

            // Draw sprite parts
            prim = SetupPrimsForEntitySpriteParts(entity, prim);
            // Above returns the following prim

            // Copy prims to a later index (while maintaining linked list order)
            for (primItr = entity->ext.venusWeedSpike.firstPart;
                 primItr != NULL; primItr = primItr->next, prim = primNext) {
                primNext = prim->next;

                *prim = *primItr;
                prim->next = primNext;
                prim->priority = primItr->priority + 1;
            }
        }

        // Update to match flower
        entity = entity + 1; // Flower
        self->animCurFrame = entity->animCurFrame;
        self->zPriority = entity->zPriority + 1;
        // Fallthrough
    case EXTEND:
        clut = self->palette & 0xFFF;
#if defined(BLUE)
        FntPrint("color %x\n", self->palette);
#endif
        prim = self->ext.venusWeedSpike.firstPart;
        while (prim != NULL) {
            prim->clut = clut;
            prim->drawMode = DRAW_UNK02;

            prim = prim->next;
        }

        // Update to match flower
        entity = self->ext.venusWeedSpike.flower;
        self->animCurFrame = entity->animCurFrame;
        self->palette++;
        clut = self->palette & 0xFFF;
        if (clut > SPIKE_CLUT_END) {
            DestroyEntity(self);
        } else {
            if (entity->entityId != E_VENUS_WEED_FLOWER) {
                DestroyEntity(self);
            }
        }
        break;
    }
}
