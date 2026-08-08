// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

// Donor: src/st/rare/e_werewolf.c (matched). EInit lives in this overlay's
// own e_init.c under its raw D_us_ name - extern only. bo2 has no local
// name for the main werewolf boss entity (its update function,
// func_us_801B385C, isn't decompiled yet), so the entityId check below
// uses E_UNK_1B, the local enum slot at the matching numeric index (0x1B),
// same as the donor's E_WEREWOLF.
extern EInit D_us_8018056C;
extern EInit g_EInitWerewolfARE;
extern EInit g_EInitInteractable;
// Both tables already live in bo2's own raw .data blob (see
// asm/us/boss/bo2/data/1850.data.s) - extern, never re-declare with values
// even though the bytes happen to match the donor's (lesson 3).
extern SVECTOR D_us_80181ACC[];
extern s16* D_us_801A16F0[];

Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);

// { hitboxOffX, hitboxOffY, hitboxWidth, hitboxHeight }
static s8 attack_hitboxes[][4] = {
    {0, 0, 0, 0},       {-38, -18, 17, 12}, {-34, -16, 17, 12},
    {-35, -15, 17, 10}, {-37, -15, 17, 10}, {-39, -16, 17, 12},
    {-36, -17, 17, 10}, {-8, -52, 11, 21},  {33, -31, 12, 11},
    {50, 7, 10, 11},    {33, -8, 14, 12},   {27, -36, 14, 12},
    {-5, -56, 10, 26},  {-44, -25, 24, 40}, {-34, 30, 17, 11},
    {-46, 11, 28, 10},  {-36, -5, 17, 11},  {-33, -14, 13, 13},
    {-40, -26, 12, 11}, {-30, -30, 11, 11}, {-23, -31, 12, 13},
    {-8, -29, 12, 16},  {-9, -25, 11, 18},  {-7, -27, 11, 17},
    {13, -21, 11, 11},  {20, -22, 11, 11},  {-21, -31, 11, 13},
    {-35, -6, 17, 11},  {-19, 20, 12, 12},  {-64, -88, 0, 0},
    {-26, -30, 13, 13}, {-38, -17, 13, 13}, {-25, -30, 13, 13},
};
// Indexed based off animCurFrame
static u8 attack_hitbox_indices[] = {
    0,  1,  1,  1,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,
    12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 29, 3,  29, 29, 29, 29, 29, 29, 29, 29, 29,
    1,  9,  9,  1,  30, 31, 17, 10, 14, 25, 15, 32, 29, 29, 0,
};

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B37D0);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B385C);

void EntityWerewolfAttackHitbox(Entity* self) {
    s32 animCurFrame;
    s8* hitboxPtr;
    Entity* werewolf;

    if (!self->step) {
        InitializeEntity(D_us_8018056C);
        self->animCurFrame = 0;
    }

    werewolf = self - 1;
    if (werewolf->entityId != E_UNK_1B) {
        DestroyEntity(self);
        return;
    }

    animCurFrame = werewolf->animCurFrame;
    self->facingLeft = werewolf->facingLeft;
    self->posX.val = werewolf->posX.val;
    self->posY.val = werewolf->posY.val;

    hitboxPtr = *attack_hitboxes;
    hitboxPtr += attack_hitbox_indices[animCurFrame] * 4;
    self->hitboxOffX = *hitboxPtr++;
    self->hitboxOffY = *hitboxPtr++;
    self->hitboxWidth = *hitboxPtr++;
    self->hitboxHeight = *hitboxPtr++;
}

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B503C);

// Displays a trailing opaque effect behind the arcing spin jump attack
// Params here is the animCurFrame of the main Werewolf entity
void EntityWerewolfSpinAttackAfterImage(Entity* self) {
    if (!self->step) {
        InitializeEntity(g_EInitWerewolfARE);
        self->palette = g_EInitWerewolfARE[3] + PAL_FLAG(9);
        self->animCurFrame = self->params;
        self->hitboxState = 0;
        self->flags |= FLAG_UNK_00200000 | FLAG_UNK_2000;
        self->blendMode = BLEND_TRANSP | BLEND_ADD;
        self->drawFlags = ENTITY_OPACITY | ENTITY_ROTATE;
        self->opacity = 0x60;
    }

    self->ext.werewolf.timer++;
    if ((!self->ext.werewolf.timer) & 1) {
        self->animCurFrame = 0;
    } else {
        self->animCurFrame = self->params;
    }

    self->opacity -= 8;
    if (!self->opacity) {
        DestroyEntity(self);
    }
}

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B52FC);

void EntityWerewolfDeathFlames(Entity* self) {
    long otz;
    SVECTOR svecTwo;
    VECTOR vec;
    MATRIX matrix;

    Primitive* prim;
    s16* ptr;
    s32 posY;
    s32 i;
    SVECTOR* sVec;
    s32 color;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x80);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.werewolf.prim = prim;
        while (prim != NULL) {
            prim->tpage = 0x17;
            // BUG: this is immediately overwritten
            prim->clut = 0x222;
            prim->clut = 0x223;
            prim->priority = self->zPriority;
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        self->ext.werewolf.timer = 0x140;
    case 1:
        if (!--self->ext.werewolf.timer) {
            DestroyEntity(self);
            return;
        }

        if (!(self->ext.werewolf.timer & 0x1F)) {
            PlaySfxPositional(SFX_FIREBALL_SHOT_B);
        }

        if (!(self->ext.werewolf.timer & 3)) {
            self->ext.werewolf.unk9C -= 0x20;
            color = self->ext.werewolf.timer;
            if (color > 0x80) {
                color = 0x80;
            }

            for (i = 0; i < 3; i++) {
                prim = self->ext.werewolf.prim;
                prim = FindFirstUnkPrim2(prim, 2);
                if (prim != NULL) {
                    UnkPolyFunc2(prim);
                    prim->next->x2 = 0;
                    prim->next->y2 = 0;
                    prim->next->x1 =
                        self->ext.werewolf.unk9C + (i * 0x555) + Random();
                    prim->next->y1 = 0;
                    PGREY(prim, 0) = PGREY(prim, 1) = PGREY(prim, 2) =
                        PGREY(prim, 3) = color;

                    prim = prim->next;
                    prim->drawMode = DRAW_HIDE;
                }
            }
        }

        SetGeomScreen(0x200);
        SetGeomOffset(self->posX.i.hi, self->posY.i.hi);
        prim = self->ext.werewolf.prim;
        sVec = D_us_80181ACC;
        posY = self->posY.i.hi + 8;
        while (prim != NULL) {
            if (prim->p3 == 8) {
                vec.vx = 0;
                vec.vy = prim->next->y1;
                vec.vz = 0x200;
                TransMatrix(&matrix, &vec);
                SetTransMatrix(&matrix);
                svecTwo.vx = 0;
                svecTwo.vy = prim->next->x1;
                svecTwo.vz = 0x180;
                RotMatrix(&svecTwo, &matrix);
                SetRotMatrix(&matrix);
                gte_ldv3c(sVec);
                gte_rtpt();
                gte_stsxy3_gt3(prim);
                gte_ldv0(&sVec[3]);
                gte_rtps();
                gte_stsxy((long*)&prim->x3);
                gte_avsz4();
#ifdef VERSION_US
                gte_stszotz(otz);
#else
                gte_stszotz(&otz);
#endif

                if (otz > 0x80) {
                    prim->priority = self->zPriority - 1;
                } else {
                    prim->priority = self->zPriority + 1;
                }

                if (posY < prim->y2) {
                    prim->y2 = posY;
                }

                if (posY < prim->y3) {
                    prim->y3 = posY;
                }
                prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }

        prim = self->ext.werewolf.prim;
        while (prim != NULL) {
            if (prim->p3 == 8) {
                prim->next->y1 -= 3;
                prim->next->x1 += 0x30;
                prim->r0 -= 2;
                if (!prim->r0) {
                    UnkPolyFunc0(prim);
                    continue;
                }

                prim->g0 = prim->b0 = prim->r0;
                PGREY(prim, 1) = PGREY(prim, 2) = PGREY(prim, 3) = prim->r0;
                if (!prim->next->y2) {
                    prim->next->x2++;
                    if (prim->next->x2 > 0xD) {
                        UnkPolyFunc0(prim);
                        continue;
                    }

                    ptr = D_us_801A16F0[prim->next->x2];
                    ptr += 8;
                    prim->u0 = prim->u2 = *ptr++;
                    prim->v0 = prim->v1 = *ptr++;
                    prim->u1 = prim->u3 = *ptr++;
                    prim->v2 = prim->v3 = *ptr++;
                    prim->next->y2 = 3;
                } else {
                    prim->next->y2--;
                }
            }
            prim = prim->next;
        }
        break;
    }
}
