// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

extern u8 D_us_801829C4[][4];
extern SVECTOR D_us_80182B4C;
extern SVECTOR D_us_80182B54;
extern SVECTOR D_us_80182B5C;
extern SVECTOR D_us_80182B64;
extern u16 D_us_80182AC0[];

extern s16 D_us_801829CC[];
extern s16 D_us_801829E4[];
extern s16 D_us_801829F0[];
extern s16 D_us_80182A00[];
extern u16 D_us_801829DC[];
extern u8 D_us_80182904[];
extern u8 D_us_80182914[];
extern u8 D_us_80182924[];
extern u8 D_us_8018293C[];
extern u8 D_us_80182948[];
extern u8 D_us_80182A08[];
extern u8 D_us_80182A18[];
extern u8 D_us_80182A40[];
extern u8 D_us_80182A4C[];
extern u8 D_us_80182A90[];
extern u8 D_us_80182B6C[];

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BBE58_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BC650_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BCA5C_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BCB9C_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BCD80_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BCE4C_from_rnz1);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", func_us_801BCFC8_from_rnz1);

// Donor: src/st/e_jack_o_bones.h (shared header, used verbatim by
// src/st/rno3/e_jack_o_bones.c). Not #include-d directly: the header calls
// CreateEntityFromCurrentEntity(E_JACKO_JACK, ...) / E_JACKO_DEATH_PARTS,
// but rno4.h's enum (already semantically renamed) calls those slots
// E_JACKO_BONES_JACK / E_JACKO_BONES_DEATH_PARTS. Editing the shared header
// would ripple into rno3, so the body is copied locally with the id names
// swapped to rno4's.
extern EInit g_EInitJackOBones;
// g_EInitJackOBones2 / g_EInitJackOBones3 are the death-parts and
// jack-projectile init structs, right after g_EInitJackOBones in the same
// ANIMSET_OVL(10) group (same bank, enemyID pattern 0x074/0x002/0x075
// matches main/particle/projectile).
extern EInit g_EInitJackOBones2;
extern EInit g_EInitJackOBones3;


typedef enum {
    JACKO_INIT,
    JACKO_1,
    JACKO_WALK_FWD,
    JACKO_WALK_BACK,
    JACKO_THROW,
    JACKO_JUMP,
    JACKO_DEAD
} JackOBonesSteps;

typedef enum {
    JACKO_JUMP_WINDUP,
    JACKO_JUMP_MIDAIR,
    JACKO_JUMP_LANDING
} JumpSubsteps;

static void TryThrow(void) {
    s32 temp_s1;
    u16 temp_s0;

    temp_s1 = UnkCollisionFunc2(D_us_801829DC);
    temp_s0 = UnkCollisionFunc(D_us_801829E4, 3);
    if ((temp_s1 == 0x80) || (temp_s0 & 2)) {
        SetStep(JACKO_JUMP);
        return;
    }
    if (!g_CurrentEntity->ext.jackoBones.throwTimer) {
        SetStep(JACKO_THROW);
        return;
    }
    g_CurrentEntity->ext.jackoBones.throwTimer--;
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", EntityJackOBones);

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", EntityJackOBonesDeathParts);

void EntityJackOBonesJack(Entity* self) {
    Collider sp10;
    s32 temp;
    s32 yVar;
    s32 xVar;

    if (!self->step) {
        InitializeEntity(g_EInitJackOBones3);
        if (self->params) {
            self->palette += 1;
        }
        self->animCurFrame = 0x15;
        self->drawFlags |= ENTITY_ROTATE;
        if (self->params) {
            yVar = FIX(4);
            xVar = FIX(1);
        } else {
            yVar = FIX(-1);
            xVar = FIX(2.5);
        }
        if (self->facingLeft) {
            self->velocityX = xVar;
        } else {
            self->velocityX = -xVar;
        }
        self->velocityY = yVar;
    }
    MoveEntity();
    self->velocityY += FIX(0.1875);
    self->rotate -= 0x40;
    xVar = self->posX.i.hi;
    yVar = self->posY.i.hi + 5;
    g_api.CheckCollision(xVar, yVar, &sp10, 0);
    if (sp10.effects & EFFECT_SOLID) {
        PlaySfxPositional(SFX_SKULL_KNOCK_A);
        self->ext.jackoBones.bouncesDone += 1;
        temp = sp10.unk18;
#if defined(VERSION_PSP)
        xVar = self->posX.i.hi;
        yVar = self->posY.i.hi - 3;
        g_api.CheckCollision(xVar, yVar, &sp10, 0);
        if (sp10.effects & EFFECT_SOLID) {
            self->velocityX = -self->velocityX;
        } else {
#else
        if (1) {
#endif
            self->posY.i.hi += temp;
#if defined(VERSION_PSP)
            self->velocityY = -self->velocityY;
#else
            self->velocityY =
                -((self->velocityY < 0) ? -self->velocityY : self->velocityY);
#endif
            if (self->params) {
                self->velocityY = FIX(-7) / self->ext.jackoBones.bouncesDone;
            } else {
                self->velocityY -= self->velocityY / 16;
            }
        }
        xVar = self->posX.i.hi + self->velocityX;
        yVar = self->posY.i.hi;
    }
#if defined(VERSION_PSP)
    if (self->params) {
#else
    if (1) {
#endif
        xVar = self->posX.i.hi;
        yVar = self->posY.i.hi - 5;
        g_api.CheckCollision(xVar, yVar, &sp10, 0);
        if (sp10.effects & EFFECT_SOLID) {
            self->posY.i.hi += sp10.unk20;
            self->velocityY = abs(self->velocityY);
        }
#if !defined(VERSION_PSP)
        xVar = self->posX.i.hi;
        yVar = self->posY.i.hi;
        if (self->velocityX > 0) {
            xVar += 5;
        } else {
            xVar -= 5;
        }
        g_api.CheckCollision(xVar, yVar, &sp10, 0);
        if (sp10.effects & EFFECT_SOLID) {
            self->velocityX = -self->velocityX;
        }
        if (self->params)
#endif
            if (self->ext.jackoBones.bouncesDone > 8) {
                self->flags |= FLAG_DEAD;
            }
    }

    if (self->flags & FLAG_DEAD) {
        self->drawFlags = ENTITY_DEFAULT;
        self->entityId = E_EXPLOSION;
        self->pfnUpdate = EntityExplosion;
        self->params = 0;
        self->step = 0;
    }
}

// Donor: src/st/e_nova_skeleton.h (shared header, used verbatim by
// src/st/rno3/e_nova_skeleton.c). Same situation as jack-o-bones above:
// the header's CreateEntityFromEntity(E_NOVA_PULSE, ...) needs to become
// E_NOVA_LASER_PULSE for rno4.h's enum, so the body is copied locally.
extern EInit g_EInitNovaSkeleton;
// g_EInitNovaSkeleton2 is the laser/pulse init struct, right after
// g_EInitNovaSkeleton in the ANIMSET_OVL(11) group.
extern EInit g_EInitNovaSkeleton2;

// Not the laser itself - just nova skeleton standing there, holding arms up
// menacingly
// laser cooldown
// vectors
// uv data

typedef enum {
    NOVA_INIT,
    NOVA_1,
    NOVA_IDLE,
    NOVA_WALK_FWD,
    NOVA_WALK_BACK,
    NOVA_5,
    NOVA_CHARGE,
    NOVA_SHOOT,
    NOVA_DEAD
} NovaSkeletonSteps;

static void TryShoot(void) {
    // return value not used, but function has side effects
    s32 unused = UnkCollisionFunc2(&D_us_80182A00);
    // if cooldown has expired...
    if (!g_CurrentEntity->ext.nova.cooldown) {
        if (GetDistanceToPlayerX() >= 0x80) {
            return;
        }
        if ((g_CurrentEntity->facingLeft) ^ (GetSideToPlayer() & 1)) {
            SetStep(NOVA_CHARGE);
        }
    } else {
        g_CurrentEntity->ext.nova.cooldown--;
    }
}

static void DrawLaserRing(void) {
    s32 p;
    s32 flag;
    SVECTOR sp60;
    VECTOR sp50;
    MATRIX sp30;
    SVECTOR sp28 = {0};
    s32 yVar;
    s32 xVar;
    Primitive* prim;
    // Ring state: 0 = init, 1 = ongoing/growing
    switch (g_CurrentEntity->ext.nova.ringState) {
    case 0:
        g_CurrentEntity->ext.nova.ringSize = 0;
        prim = g_CurrentEntity->ext.nova.prim;
        prim->r0 = prim->g0 = prim->b0 = 0xC0;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        g_CurrentEntity->ext.nova.ringState = 1;
        break;
    case 1:
        g_CurrentEntity->ext.nova.ringRot += 0x100;
        g_CurrentEntity->ext.nova.ringSize += 0x200;
        break;
    }
    SetGeomScreen(0x200);
    xVar = g_CurrentEntity->posX.i.hi;
    yVar = g_CurrentEntity->posY.i.hi;
    if (g_CurrentEntity->facingLeft) {
        xVar += 10;
    } else {
        xVar -= 10;
    }
    yVar -= 2;
    SetGeomOffset(xVar, yVar);
    sp60.vx = 0;
    if (g_CurrentEntity->facingLeft) {
        sp60.vy = -0x2E0;
    } else {
        sp60.vy = 0x2E0;
    }
    sp60.vz = g_CurrentEntity->ext.nova.ringRot;
    RotMatrix(&sp28, &sp30);
    RotMatrixZ(sp60.vz, &sp30);
    RotMatrixY(sp60.vy, &sp30);
    sp50.vx = 0;
    sp50.vy = 0;
    sp50.vz = 0x200;
    TransMatrix(&sp30, &sp50);
    sp50.vx = g_CurrentEntity->ext.nova.ringSize;
    sp50.vy = g_CurrentEntity->ext.nova.ringSize;
    sp50.vz = 0x1000;
    ScaleMatrix(&sp30, &sp50);
    SetRotMatrix(&sp30);
    SetTransMatrix(&sp30);
    prim = g_CurrentEntity->ext.nova.prim;
    RotTransPers4(&D_us_80182B4C, &D_us_80182B54, &D_us_80182B5C, &D_us_80182B64,
                  (long*)&prim->x0, (long*)&prim->x1, (long*)&prim->x2,
                  (long*)&prim->x3, (long*)&p, (long*)&flag);
}

INCLUDE_ASM("st/rno4/nonmatchings/unk_58A30", EntityNovaSkeleton);

// Donor: src/st/dai/e_bone_halberd.c (EntityBoneHalberdParts), matched.
// Only the piece EntityBoneHalberdParts actually uses (D_us_80182AC0[])
// is reproduced; the rest of that file's tables belong to functions rno4
// doesn't stub (EntityBoneHalberd / EntityBoneHalberdAttack).

void EntityBladeSoldierDeathParts(Entity* self) {
    if (self->step) {
        if (--self->ext.nova.deathPartLife) {
            self->rotate += D_us_80182AC0[self->params];
            FallEntity();
            MoveEntity();
            return;
        }
        self->entityId = E_EXPLOSION;
        self->pfnUpdate = EntityExplosion;
        self->params = EXPLOSION_SMALL;
        self->step = 0;
        return;
    }
    InitializeEntity(g_EInitNovaSkeleton);
    self->hitboxState = 0;
    self->flags |=
        FLAG_DESTROY_IF_OUT_OF_CAMERA | FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA |
        FLAG_UNK_00200000 | FLAG_UNK_2000;
    self->animCurFrame = self->params + 0x1D;
    self->drawFlags = ENTITY_ROTATE;

    if (self->facingLeft) {
        self->velocityX = -self->velocityX;
    }
}

typedef enum { LASER_INIT, LASER_1, LASER_2, LASER_3, LASER_4 } NovaLaserSteps;

void EntityNovaLaser(Entity* self) {
    s32 centerX;
    s32 primIndex;
    s32 centerY;
    Entity* other;
    s32 primX;
    s32 var_s2;
    u8* var_s1;
    Primitive* prim;

    switch (self->step) {
    case LASER_INIT:
        InitializeEntity(g_EInitNovaSkeleton2);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 3);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.nova.prim = prim;
        var_s1 = &D_us_80182B6C[0];
        for (var_s2 = 0; var_s2 < 3; prim = prim->next, var_s2++) {
            prim->tpage = 0x12;
            prim->clut = 0x216;
            prim->u0 = prim->u2 = *var_s1++ + 0x80;
            prim->u1 = prim->u3 = *var_s1++ + 0x80;
            prim->v0 = prim->v1 = 0x40;
            prim->v2 = prim->v3 = 0x5F;
            prim->r0 = prim->g0 = prim->b0 = *var_s1++;
            LOW(prim->r2) = LOW(prim->r0);
            prim->r1 = prim->g1 = prim->b1 = *var_s1++;
            LOW(prim->r3) = LOW(prim->r1);
            prim->priority = self->zPriority + 2;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
        }
        self->ext.nova.laserTimer = 0x60;
        self->ext.nova.laserLength = 0;
    case LASER_1:
        self->ext.nova.laserFadeTimer = 0x10;
        if (self->ext.nova.laserLength < 0x80) {
            self->ext.nova.laserLength += 0x10;
        } else {
            self->ext.nova.laserLength = 0x80;
            self->hitboxState = 1;
            self->step++;
        }
    case LASER_2:
        if (!(self->ext.nova.laserTimer & 3)) {
            other = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (other != NULL) {
                CreateEntityFromEntity(E_NOVA_LASER_PULSE, self, other);
                other->zPriority = self->zPriority - 1;
                other->ext.nova.laserLength = self->ext.nova.laserLength;
                other->facingLeft = self->facingLeft;
            }
        }
        if (!(self->ext.nova.laserTimer & 0xF)) {
            PlaySfxPositional(SFX_BAT_ECHO_A);
        }
        if (self->ext.nova.laserTimer < 0x10) {
            PlaySfxPositional(SFX_BAT_ECHO_D);
            self->step++;
        }
    case LASER_3:
        if (Random() & 1) {
            if (self->ext.nova.laserLength < 0x88) {
                self->ext.nova.laserLength++;
            } else if (self->ext.nova.laserLength > 0x78) {
                self->ext.nova.laserLength--;
            }
        }
        self->hitboxWidth = self->ext.nova.laserLength / 2 + 0x10;
        self->hitboxOffX = -self->ext.nova.laserLength / 2 - 0x10;
        self->hitboxHeight = 8;
        other = self - 1;
        if (other->entityId != E_NOVA_SKELETON) {
            self->ext.nova.laserTimer = 1;
        }
        if (!--self->ext.nova.laserTimer) {
            self->hitboxState = 0;
            self->step++;
        }
        break;
    case LASER_4:
        self->ext.nova.laserFadeTimer--;
        if (!self->ext.nova.laserFadeTimer) {
            DestroyEntity(self);
            return;
        }
        break;
    }
    centerX = self->posX.i.hi;
    centerY = self->posY.i.hi;
    prim = self->ext.nova.prim;
    for (var_s2 = 0; var_s2 < 3; prim = prim->next, var_s2++) {
        prim->y0 = prim->y1 = centerY - self->ext.nova.laserFadeTimer;
        prim->y2 = prim->y3 = centerY + self->ext.nova.laserFadeTimer;
        if (g_Timer & 1) {
            prim->clut = 0x216;
        } else {
            prim->clut = 0x217;
        }
    }
    prim = self->ext.nova.prim;
    primX = centerX;
    if (self->facingLeft) {
        primX -= 0x10;
    } else {
        primX += 0x10;
    }

    prim->x1 = prim->x3 = primX;
    if (self->facingLeft) {
        primX += 0x20;
    } else {
        primX -= 0x20;
    }

    prim->x0 = prim->x2 = primX;
    prim = prim->next;
    prim->x1 = prim->x3 = primX;
    if (self->facingLeft) {
        primX += self->ext.nova.laserLength;
    } else {
        primX -= self->ext.nova.laserLength;
    }

    prim->x0 = prim->x2 = primX;
    prim = prim->next;
    prim->x1 = prim->x3 = primX;
    if (self->facingLeft) {
        primX += 0x20;
    } else {
        primX -= 0x20;
    }
    prim->x0 = prim->x2 = primX;
    prim = prim->next;
}

void EntityNovaLaserPulse(Entity* self) {
    s32 temp_s0;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitNovaSkeleton2);
        self->hitboxState = 0;
        self->animCurFrame = 0x24;
        self->drawFlags |= ENTITY_SCALEY | ENTITY_SCALEX;
        self->scaleX = self->scaleY = 0x10;
        if (self->facingLeft) {
            self->velocityX = FIX(8.0);
        } else {
            self->velocityX = FIX(-8.0);
        }
        /* fallthrough */
    case 1:
        MoveEntity();
        self->ext.nova.laserPulseDist += abs(self->velocityX);
        self->scaleX = self->scaleY += 0x40;
        if (self->scaleX < 0x100) {
            return;
        }
        self->step++;
        return;
    case 2:
        MoveEntity();
        self->ext.nova.laserPulseDist += abs(self->velocityX);
        temp_s0 = (self->ext.nova.laserLength + 0x20) << 0x10;
        temp_s0 -= self->ext.nova.laserPulseDist;
        if (temp_s0 < 0) {
            DestroyEntity(self);
            return;
        }
        // okay now let's just play with this number. we're not going to use
        // it but math is fun, I guess? Everyone likes some nice bit shifts.
        temp_s0 >>= 0x10;
        temp_s0 <<= 3;
        if (temp_s0 > 0x100) {
            temp_s0 = 0x100;
        }
        break;
    }
}
