// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno0.h"

// RNO0's "Guardian" is a renamed copy of ARE's "Armor Lord" mid-boss
// (see src/st/e_armor_lord.h, src/st/are/e_armor_lord.c). ARE is fully
// matched, so its code is transplanted here verbatim; only the palette
// id (clut) and the local data-table addresses differ for this overlay.
// Static helper names keep the func_us_..._from_are convention already
// established by the stub scaffolding / config/symbols.us.strno0.txt.

#define PAL_GUARDIAN_UNK 0x20A // ARE's PAL_ARMOR_LORD_UNK is 0x21A; RNO0 uses a different clut id here

extern EInit OVL_EXPORT(EInitInteractable);
extern EInit g_EInitGuardian;
extern EInit D_us_80180AD4; // ARE's D_us_80180AE8 equivalent (func_us_801D348C_from_are)
extern EInit D_us_80180AE0; // ARE's D_us_80180AF4 equivalent (func_us_801D3700_from_are)

extern Primitive* FindFirstUnkPrim2(Primitive* prim, u8 index);

extern u8 D_us_80181B00[];     // ARE's D_us_80182D50 equivalent (SetStep random table)
extern s16 D_us_80181B08[];    // ARE's D_us_80182D58 equivalent (UnkCollisionFunc3 table)
extern s16 D_us_80181B18[];    // ARE's D_us_80182D68 equivalent (UnkCollisionFunc2 table)
extern s16 D_us_80181B20[];    // ARE's D_us_80182D70 equivalent (unk80 countdown table)
extern u8 D_us_80181B28[];     // anim0
extern u8 D_us_80181B34[];     // anim1
extern u8 D_us_80181B4C[];     // anim2
extern u8 D_us_80181B54[];     // anim3
extern u8 D_us_80181B5C[];     // anim4
extern u8 D_us_80181B74[];     // anim5
extern u8 D_us_80181B90[];     // anim6
extern u8 D_us_80181B9C[];     // anim7
extern MATRIX D_us_80181BA4;   // armorLordColorMatrix
extern SVECTOR D_us_80181BC4;  // armorLordColNormVec1
extern SVECTOR D_us_80181BCC;  // armorLordColNormVec2
extern SVECTOR D_us_80181BD4;  // armorLordRotVec
extern s16 D_us_80181BDC[][2]; // hitboxWidthHeights
extern u16 D_us_80181C48[][2]; // hitboxOffXYs

// Guardian fire wave helper
static void func_us_801D1184_from_are(Primitive* prim) {
    switch (prim->next->u2) {
    case 0:
        prim->tpage = 0x1A;
        prim->clut = PAL_CC_FIRE_EFFECT;
        prim->u0 = 0xF0;
        prim->u1 = 0xFF;
        prim->u2 = prim->u0;
        prim->u3 = prim->u1;
        if (prim->next->r3) {
            prim->v0 = 0;
            prim->v1 = prim->v0;
            prim->v2 = 0xF;
            prim->v3 = prim->v2;
        } else {
            prim->v0 = 0x28;
            prim->v1 = prim->v0;
            prim->v2 = 0x37;
            prim->v3 = prim->v2;
        }
        prim->priority = g_CurrentEntity->zPriority + 2;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->x0 -= 8;
        prim->x1 = prim->x0 + 16;
        prim->x2 = prim->x0;
        prim->x3 = prim->x1;
        prim->y0 -= 8;
        prim->y1 = prim->y0;
        prim->y2 = prim->y0 + 0x10;
        prim->y3 = prim->y2;
        PGREY(prim, 0) = 0xA0;
        PGREY(prim, 1) = 0xA0;
        PGREY(prim, 2) = 0xA0;
        PGREY(prim, 3) = 0xA0;
        prim->next->u2++;
        break;

    case 1:
        if (g_Timer % 4 == 0) {
            prim->y0++;
            prim->y1 = prim->y0;
            prim->y2 = prim->y0 + 0x10;
            prim->y3 = prim->y2;
        }
        prim->r0 -= 2;
        prim->g0 = prim->b0 = prim->r0;
        prim->r1 = prim->g1 = prim->b1 = prim->r0;
        prim->r2 = prim->g2 = prim->b2 = prim->r0;
        prim->r3 = prim->g3 = prim->b3 = prim->r0;
        if (prim->r0 < 0x10) {
            UnkPolyFunc0(prim);
            prim->next->u2 = 0;
        }
        break;
    }
}

// Guardian
static void func_us_801D1388_from_are(Primitive* prim) {
    Collider collider;
    Primitive* otherPrim;
    Entity* tempEntity;
    s16 dx;
    s16 posX, posY;

    if (g_Timer % 3 == 0) {
        u8 temp = prim->u0;
        prim->u0 = prim->u1;
        prim->u1 = temp;
        prim->u2 = prim->u0;
        prim->u3 = prim->u1;
    }
    switch (prim->next->u2) {
    case 0:
        prim->x0 = prim->x2;
        prim->x1 = prim->x3;
        prim->y0 = prim->y2;
        prim->y1 = prim->y3;
        prim->tpage = 0x14;
        prim->clut = PAL_GUARDIAN_UNK;
        prim->u0 = 0xE0;
        prim->u1 = 0xEF;
        prim->u2 = prim->u0;
        prim->u3 = prim->u1;
        prim->v0 = 0xD0;
        prim->v1 = prim->v0;
        prim->v2 = 0xD0;
        prim->v3 = prim->v2;
        PGREY(prim, 0) = 0x70;
        PGREY(prim, 1) = 0x70;
        PGREY(prim, 2) = 0x70;
        PGREY(prim, 3) = 0x70;
        prim->priority = g_CurrentEntity->zPriority + 2;
        prim->drawMode =
            DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS | DRAW_UNK02 | DRAW_TRANSP;
        prim->next->u2++;
        break;

    case 1:
        prim->v2 += 2;
        prim->v3 = prim->v2;
        prim->y0 -= 2;
        prim->y1 -= 2;
        if (g_CurrentEntity->facingLeft) {
            prim->x3++;
        } else {
            prim->x3--;
        }
        prim->x1 = prim->x3;
        posX = prim->x3;
        posY = prim->y3 + 1;
        g_api.CheckCollision(posX, posY, &collider, 0);
        if ((collider.effects & EFFECT_SOLID) == 0) {
            prim->next->r3 = 0;
            prim->next->u2 += 1;
            break;
        }
        if ((prim->v2 == 0xE0) && prim->next->r3) {
            otherPrim = g_CurrentEntity->ext.armorLord.prim;
            otherPrim = FindFirstUnkPrim2(otherPrim, 2);
            if (otherPrim != NULL) {
                UnkPolyFunc2(otherPrim);
                otherPrim->next->r3 = prim->next->r3 - 1;
                if (g_CurrentEntity->facingLeft) {
                    otherPrim->x2 = prim->x3 - 8;
                    otherPrim->x3 = otherPrim->x2 + 16;
                } else {
                    otherPrim->x2 = prim->x3 + 8;
                    otherPrim->x3 = otherPrim->x2 - 16;
                }
                otherPrim->y2 = prim->y3;
                otherPrim->y3 = otherPrim->y2;
            }
        }
        if (prim->v2 > 0xFD) {
            otherPrim = g_CurrentEntity->ext.armorLord.prim;
            otherPrim = FindFirstUnkPrim2(otherPrim, 2);
            if (otherPrim != NULL) {
                UnkPolyFunc2(otherPrim);
                otherPrim->next->g3 = 1;
                otherPrim->next->r3 = Random() & 1;
                if (g_CurrentEntity->facingLeft) {
                    otherPrim->x0 = prim->x1 - 0x10;
                } else {
                    otherPrim->x0 = prim->x1 + 0x10;
                }
                otherPrim->y0 = prim->y1 + 0x10;
            }
            prim->next->u2++;
        }
        break;

    case 2:
        prim->v2--;
        prim->v3 = prim->v2;
        prim->y0++;
        prim->y1++;
        prim->g0 -= 4;
        prim->b0 -= 2;
        prim->r1 = prim->r2 = prim->r3 = prim->r0;
        prim->g1 = prim->g2 = prim->g3 = prim->g0;
        prim->b1 = prim->b2 = prim->b3 = prim->b0;
        if (prim->v2 < 0xD2) {
            UnkPolyFunc0(prim);
            prim->next->u2 = 0;
        }
        break;
    }
    tempEntity = &PLAYER;
    if (g_CurrentEntity->facingLeft) {
        dx = prim->x3 - tempEntity->posX.i.hi;
    } else {
        dx = tempEntity->posX.i.hi - prim->x3;
    }
    if ((dx > 0) && !prim->next->v2 && (dx < 0x10)) {
        tempEntity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (tempEntity != NULL) {
            CreateEntityFromCurrentEntity(E_UNK_24, tempEntity);
            tempEntity->posX.i.hi = prim->x3;
            tempEntity->posY.i.hi = prim->y0;
            tempEntity->facingLeft = g_CurrentEntity->facingLeft;
            tempEntity->hitboxHeight = (prim->y2 - prim->y0) / 2;
            tempEntity->hitboxOffY = tempEntity->hitboxHeight + 8;
            tempEntity->ext.armorLord.prim = prim;
            prim->next->v2 = 1;
        }
    }
}

void EntityGuardianFireWave(Entity* self) {
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(OVL_EXPORT(EInitInteractable));
        self->ext.armorLord.unk80 = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x1A);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.armorLord.prim = prim;
            while (prim != NULL) {
                prim->drawMode = DRAW_HIDE;
                prim->p3 = 0;
                prim = prim->next;
            }
        } else {
            DestroyEntity(self);
            return;
        }
        prim = self->ext.armorLord.prim;
        prim = FindFirstUnkPrim2(prim, 2);
        if (prim != NULL) {
            UnkPolyFunc2(prim);
            prim->x2 = self->posX.i.hi;
            if (self->facingLeft) {
                prim->x3 = prim->x2 + 0x20;
            } else {
                prim->x3 = prim->x2 - 0x20;
            }
            prim->y2 = self->posY.i.hi + 0x28;
            prim->y3 = prim->y2;
            prim->next->r3 = 5;
            prim->next->g3 = 0;
        }

    case 1:
        prim = self->ext.armorLord.prim;
        while (prim != NULL) {
            if (prim->p3 & 8) {
                if (prim->next->g3) {
                    func_us_801D1184_from_are(prim);
                } else {
                    func_us_801D1388_from_are(prim);
                }
            }
            prim = prim->next;
        }
        if (self->ext.armorLord.unk80++ > 0x100) {
            DestroyEntity(self);
            return;
        }
    }
}

void RNO0_Unused801C2C50(void) {}

// Guardian stone-statue transformation helper
static void func_us_801D1A9C_from_are(void) {
    Primitive* prim;
    s32 primIndex;

    switch (g_CurrentEntity->step_s) {
    case 0:
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 2);
        if (primIndex != -1) {
            g_CurrentEntity->flags |= FLAG_HAS_PRIMS;
            g_CurrentEntity->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            g_CurrentEntity->ext.armorLord.prim = prim;
            UnkPolyFunc2(prim);
            prim->tpage = 0x1A;
            prim->clut = PAL_CC_STONE_EFFECT;
            prim->u0 = 0x14;
            prim->u1 = 0x2C;
            prim->u2 = prim->u0;
            prim->u3 = prim->u1;
            prim->v0 = 0xC0;
            prim->v1 = prim->v0;
            prim->v2 = 0xFF;
            prim->v3 = prim->v2;
            prim->priority = g_CurrentEntity->zPriority + 2;
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim->p3 = 8;
            if (g_CurrentEntity->facingLeft) {
                prim->next->x1 = g_CurrentEntity->posX.i.hi + 0x16;
            } else {
                prim->next->x1 = g_CurrentEntity->posX.i.hi - 0x16;
            }
            prim->next->y0 = g_CurrentEntity->posY.i.hi - 4;
            LOH(prim->next->r2) = 0;
            LOH(prim->next->b2) = 0;
            prim->next->b3 = 0x80;
        } else {
            g_CurrentEntity->step_s = 4;
            break;
        }
        g_CurrentEntity->hitboxState = 1;
        g_CurrentEntity->ext.armorLord.unk8C = 0;
        PlaySfxPositional(SFX_MAGIC_NOISE_SWEEP);
        g_CurrentEntity->step_s++;
        break;

    case 1:
        prim = g_CurrentEntity->ext.armorLord.prim;
        LOH(prim->next->r2)++;
        LOH(prim->next->b2) += 8;
        UnkPrimHelper(prim);
        if (g_CurrentEntity->ext.armorLord.unk8C++ > 8) {
            g_CurrentEntity->ext.armorLord.unk8C = 0;
            g_CurrentEntity->step_s++;
        }
        break;

    case 2:
        break;

    case 3:
        prim = g_CurrentEntity->ext.armorLord.prim;
        prim->next->b3 -= 8;
        UnkPrimHelper(prim);
        if (g_CurrentEntity->ext.armorLord.unk8C++ > 15) {
            primIndex = g_CurrentEntity->primIndex;
            g_api.FreePrimitives(primIndex);
            g_CurrentEntity->flags &= ~FLAG_HAS_PRIMS;
        }
        break;
    }
}

// Guardian death effect (rotating fire ring + stone tile crumble)
static s32 func_us_801D1DAC_from_are(void) {
    long unusedA, unusedB;
    SVECTOR rotA, rotB, rotC;
    VECTOR trans;
    MATRIX m;
    MATRIX lightMatrix;
    CVECTOR color;

    s16 posX, posY;
    Primitive* prim;
    Primitive* prim2;
    s16 z;
    s32 primIndex;
    s32 i;
    u8 temp;
    s32 unused;

    switch (g_CurrentEntity->step_s) {
    case 0:
        g_CurrentEntity->ext.armorLord.unk8D = 0;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 0xE);
        if (primIndex != -1) {
            g_CurrentEntity->flags |= FLAG_HAS_PRIMS;
            g_CurrentEntity->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            g_CurrentEntity->ext.armorLord.prim = prim;
            prim = g_CurrentEntity->ext.armorLord.prim;
            if (g_CurrentEntity->facingLeft) {
                prim->u0 = 0xFF;
                prim->u1 = 0xD8;
                prim->x0 = g_CurrentEntity->posX.i.hi - 0x1A;
            } else {
                prim->u0 = 0xD8;
                prim->u1 = 0xFF;
                prim->x0 = g_CurrentEntity->posX.i.hi - 0xD;
            }
            prim->x1 = prim->x0 + 0x28;
            prim->r0 = prim->g0 = prim->b0 = 0;
            LOW(prim->r1) = LOW(prim->r0);
            LOW(prim->r2) = LOW(prim->r0);
            LOW(prim->r3) = LOW(prim->r0);
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;

            prim2 = prim;
            prim = prim->next;
            prim->u0 = prim2->u0;
            prim->u1 = prim->u0;
            prim->x0 = prim2->x0;
            prim->x1 = prim->x0;
            prim->r0 = prim->g0 = prim->b0 = 0x20;
            LOW(prim->r2) = LOW(prim->r0);
            prim->r1 = prim->g1 = prim->b1 = 0x80;
            LOW(prim->r3) = LOW(prim->r1);
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;

            prim = prim->next;
            prim->u0 = prim2->u1;
            prim->u1 = prim->u0;
            prim->x0 = prim2->x1;
            prim->x1 = prim->x0;
            prim->r0 = prim->g0 = prim->b0 = 0x80;
            LOW(prim->r2) = LOW(prim->r0);
            prim->r1 = prim->g1 = prim->b1 = 0x20;
            LOW(prim->r3) = LOW(prim->r1);
            prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                             DRAW_UNK02 | DRAW_TRANSP;
            prim = g_CurrentEntity->ext.armorLord.prim;
            for (i = 0; i < 3; i++) {
                prim->tpage = 0x15;
                prim->clut = PAL_CC_FIRE_EFFECT;
                prim->u2 = prim->u0;
                prim->u3 = prim->u1;
                prim->v0 = 0;
                prim->v1 = prim->v0;
                prim->v2 = 0x37;
                prim->v3 = prim->v2;
                prim->x2 = prim->x0;
                prim->x3 = prim->x1;
                prim->y0 = g_CurrentEntity->posY.i.hi - 0x10;
                prim->y1 = prim->y0;
                prim->y2 = prim->y1 + 0x38;
                prim->y3 = prim->y2;
                prim->priority = g_CurrentEntity->zPriority + 1;
                prim = prim->next;
            }
            g_CurrentEntity->ext.armorLord.unk90 = prim;
            while (prim != NULL) {
                prim->tpage = 0x14;
                prim->clut = PAL_GUARDIAN_UNK;
                prim->u0 = 0xE2;
                prim->u1 = 0xEC;
                prim->u2 = prim->u0;
                prim->u3 = prim->u1;
                prim->v0 = 0xD0;
                prim->v1 = prim->v0;
                prim->v2 = 0xD0;
                prim->v3 = prim->v2;
                prim->r0 = prim->g0 = prim->b0 = 0x40;
                LOW(prim->r1) = LOW(prim->r0);
                LOW(prim->r2) = LOW(prim->r0);
                LOW(prim->r3) = LOW(prim->r0);
                prim->priority = g_CurrentEntity->zPriority + 2;
                prim->drawMode = DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                 DRAW_UNK02 | DRAW_TRANSP;
                prim = prim->next;
            }
        } else {
            return 1;
        }

        g_CurrentEntity->ext.armorLord.unk86 = 0x20;
        g_CurrentEntity->ext.armorLord.unk8A = 0;
        g_CurrentEntity->ext.armorLord.unk88 = 0;
        g_CurrentEntity->ext.armorLord.unk8C = 0;
        g_CurrentEntity->drawFlags |= ENTITY_OPACITY;
        g_CurrentEntity->opacity = 0x80;
        g_CurrentEntity->step_s++;
        break;

    case 1:
        prim = g_CurrentEntity->ext.armorLord.prim;
        prim->r0 += 2;
        prim->g0 = prim->b0 = prim->r0;
        LOW(prim->r1) = LOW(prim->r0);
        LOW(prim->r2) = LOW(prim->r0);
        LOW(prim->r3) = LOW(prim->r0);
        g_CurrentEntity->opacity -= 2;
        if (!g_CurrentEntity->opacity) {
            g_CurrentEntity->animCurFrame = 0;
            prim->drawMode = DRAW_COLORS | DRAW_UNK02;
            g_CurrentEntity->ext.armorLord.unk8D += 1;
            PlaySfxPositional(SFX_FIREBALL_SHOT_A);
            g_CurrentEntity->step_s++;
        }
        break;

    case 2:
        g_CurrentEntity->step_s++;
        break;

    case 3:
        if (g_Timer % 8 == 0) {
            prim = g_CurrentEntity->ext.armorLord.prim;
            if (g_CurrentEntity->facingLeft) {
                prim->u0--;
                prim->u1++;
            } else {
                prim->u0++;
                prim->u1--;
            }
            prim->u2 = prim->u0;
            prim->u3 = prim->u1;
            prim->x0++;
            prim->x2 = prim->x0;
            prim->x1--;
            prim->x3 = prim->x1;

            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u1--;
            } else {
                prim->u1++;
            }
            prim->u3 = prim->u1;
            prim->x1++;
            prim->x3 = prim->x1;
            prim->y0 -= 3;
            prim->y2 -= 1;

            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u0++;
            } else {
                prim->u0--;
            }
            prim->u2 = prim->u0;
            prim->x0--;
            prim->x2 = prim->x0;
            prim->y1 -= 3;
            prim->y3 -= 1;
            g_CurrentEntity->ext.armorLord.unk8C += 1;
            if (g_CurrentEntity->ext.armorLord.unk8C > 5) {
                g_CurrentEntity->step_s++;
            }
        }
        break;

    case 5:
        if (g_Timer % 4 == 0) {
            prim = g_CurrentEntity->ext.armorLord.prim;
            if (g_CurrentEntity->facingLeft) {
                prim->u0--;
                prim->u1++;
            } else {
                prim->u0++;
                prim->u1--;
            }
            prim->u2 = prim->u0;
            prim->u3 = prim->u1;
            prim->x0++;
            prim->x2 = prim->x0;
            prim->x1--;
            prim->x3 = prim->x1;

            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u0--;
                prim->u1--;
            } else {
                prim->u0++;
                prim->u1++;
            }
            prim->u2 = prim->u0;
            prim->u3 = prim->u1;
            prim->x0++;
            prim->x2 = prim->x0;
            prim->x1++;
            prim->x3 = prim->x1;

            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u1++;
                prim->u0++;
            } else {
                prim->u1--;
                prim->u0--;
            }
            prim->u3 = prim->u1;
            prim->u2 = prim->u0;
            prim->x0--;
            prim->x2 = prim->x0;
            prim->x1--;
            prim->x3 = prim->x1;
            g_CurrentEntity->ext.armorLord.unk8C++;
            if (g_CurrentEntity->ext.armorLord.unk8C > 0x14) {
                g_CurrentEntity->ext.armorLord.unk8D = 2;
                g_CurrentEntity->step_s++;
            }
        }
        break;

    case 6:
        if (g_Timer % 2 == 0) {
            prim = g_CurrentEntity->ext.armorLord.prim;
            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u0--;
            } else {
                prim->u0++;
            }
            prim->u2 = prim->u0;
            prim->x0++;
            prim->x2 = prim->x0;

            prim = prim->next;
            if (g_CurrentEntity->facingLeft) {
                prim->u1++;
            } else {
                prim->u1--;
            }
            prim->u3 = prim->u1;
            prim->x1--;
            prim->x3 = prim->x1;
            g_CurrentEntity->ext.armorLord.unk8C++;
            if (g_CurrentEntity->ext.armorLord.unk8C > 0x18) {
                g_CurrentEntity->step_s++;
            }
        }
        break;

    case 8:
        prim = g_CurrentEntity->ext.armorLord.prim;
        while (prim != NULL) {
            if (g_Timer % prim->p2 == 0) {
                prim->y0--;
            }
            prim->r0 -= 2;
            prim->g0 -= 2;
            prim->b0 -= 2;
            prim = prim->next;
        }
        if (g_CurrentEntity->ext.armorLord.unk8C++ > 0x40) {
            return 1;
        }
        break;
    }

    if (g_CurrentEntity->ext.armorLord.unk8D) {
        prim = g_CurrentEntity->ext.armorLord.unk90;
        prim2 = prim;
        unused = g_CurrentEntity->ext.armorLord.unk8A;
        for (i = 0; i < 8; i++) {
            if (g_CurrentEntity->ext.armorLord.unk88 < 0x5C) {
                prim->v2++;
                prim->v3 = prim->v2;
            }
            if (g_Timer % 3 == 0) {
                temp = prim->u0;
                prim->u0 = prim->u1;
                prim->u1 = temp;
                prim->u2 = prim->u0;
                prim->u3 = prim->u1;
            }
            SetGeomScreen(0x400);
            rotC.vx = 0;
            rotC.vy = (i * 512) + g_CurrentEntity->ext.armorLord.unk8A;
            rotC.vz = 0;
            RotMatrix(&D_us_80181BD4, &m);
            RotMatrixY(rotC.vy, &m);
            SetRotMatrix(&m);
            trans.vx = 0;
            trans.vy = 0;
            trans.vz = 0x400;
            TransMatrix(&m, &trans);
            SetTransMatrix(&m);
            SetBackColor(0x20, 0x20, 0x20);
            color.r = 0x80;
            color.g = 0x60;
            color.b = 0x60;
            color.cd = prim->type;
            RotMatrix(&rotC, &lightMatrix);
            SetColorMatrix(&D_us_80181BA4);
            SetLightMatrix(&lightMatrix);
            if (g_CurrentEntity->facingLeft) {
                posX = g_CurrentEntity->posX.i.hi - 5;
            } else {
                posX = g_CurrentEntity->posX.i.hi + 8;
            }
            posY = g_CurrentEntity->posY.i.hi + 0x29;
            SetGeomOffset(posX, posY);
            rotA.vx = 0;
            rotA.vy = 0;
            rotA.vz = -g_CurrentEntity->ext.armorLord.unk86;
            rotB.vx = 0;
            rotB.vy = -g_CurrentEntity->ext.armorLord.unk88;
            rotB.vz = -g_CurrentEntity->ext.armorLord.unk86;
            prim->x0 = prim2->x1;
            prim->y0 = prim2->y1;
            prim->x2 = prim2->x3;
            prim->y2 = prim2->y3;
            prim->r0 = prim2->r1;
            prim->g0 = prim2->g1;
            prim->b0 = prim2->b1;
            prim->r2 = prim2->r3;
            prim->g2 = prim2->g3;
            prim->b2 = prim2->b3;
            z = RotTransPers(&rotA, (long*)(&prim->x3), &unusedA, &unusedB);
            z += RotTransPers(&rotB, (long*)(&prim->x1), &unusedA, &unusedB);
            z /= 2;
            NormalColorCol(
                &D_us_80181BC4, &color, (CVECTOR*)(&prim->r3));
            NormalColorCol(
                &D_us_80181BCC, &color, (CVECTOR*)(&prim->r1));
            prim->priority = g_CurrentEntity->zPriority + (0x101 - z);
            prim2 = prim;
            prim = prim->next;
        }
        prim = g_CurrentEntity->ext.armorLord.unk90;
        prim->x0 = prim2->x1;
        prim->y0 = prim2->y1;
        prim->x2 = prim2->x3;
        prim->y2 = prim2->y3;
        prim->r0 = prim2->r1;
        prim->g0 = prim2->g1;
        prim->b0 = prim2->b1;
        prim->r2 = prim2->r3;
        prim->g2 = prim2->g3;
        prim->b2 = prim2->b3;
        g_CurrentEntity->ext.armorLord.unk8A += 4;
        if (g_CurrentEntity->ext.armorLord.unk88 < 0x68) {
            g_CurrentEntity->ext.armorLord.unk88 += 2;
        } else if (g_Timer % 4 == 0) {
            g_CurrentEntity->ext.armorLord.unk86 -= 1;
            if (g_CurrentEntity->ext.armorLord.unk86 == 0x19) {
                g_CurrentEntity->step_s++;
            }
        }
        if (g_CurrentEntity->ext.armorLord.unk8D == 2) {
            g_CurrentEntity->ext.armorLord.unk88 += 8;
        }
        if (!g_CurrentEntity->ext.armorLord.unk86) {
            primIndex = g_CurrentEntity->primIndex;
            g_api.FreePrimitives(primIndex);
            primIndex = g_api.AllocPrimitives(PRIM_TILE, 0x14);
            if (primIndex != -1) {
                g_CurrentEntity->primIndex = primIndex;
                prim = &g_PrimBuf[primIndex];
                g_CurrentEntity->ext.armorLord.prim = prim;
                while (prim != NULL) {
                    prim->x0 = (posX + (Random() & 3)) - 2;
                    prim->y0 = posY - 0x48 + (Random() & 0x3F);
                    prim->u0 = 1;
                    prim->v0 = 1;
                    prim->r0 = 0xE0;
                    prim->b0 = 0x88;
                    prim->g0 = 0xA0;
                    prim->p2 = (Random() & 7) + 1;
                    prim->priority = g_CurrentEntity->zPriority + 1;
                    prim->drawMode =
                        DRAW_TPAGE2 | DRAW_TPAGE | DRAW_UNK02 | DRAW_TRANSP;
                    prim = prim->next;
                }
            }
            g_CurrentEntity->ext.armorLord.unk8D = 0;
            g_CurrentEntity->ext.armorLord.unk8C = 0;
            g_CurrentEntity->step_s++;
        }
    }
    return 0;
}

// NOTE: EntityGuardian is NOT a verbatim transplant of ARE's EntityArmorLord.
// Its case 3 uses a different xDistance threshold (0x40 vs ARE's 0x50) and,
// right after the "if (!--unk80) SetStep(D_us_80181B00[Random()&7])" block,
// the target has an *extra* ~30-instruction chunk (starting around
// 0x801C4108, a GetSideToPlayer() call feeding a comparison against the
// entity's current facingLeft, guarding a g_Player + 0x35C status check
// with mask 0x1C00) that has no counterpart at all in are/e_armor_lord.h.
// This looks like genuine reversed-castle-specific behavior, not just an
// immediate swap, and needs to be worked out from the raw instructions
// rather than reused from the donor. Left as INCLUDE_ASM (travada).
INCLUDE_ASM("st/rno0/nonmatchings/e_armor_lord_guardian", EntityGuardian);

// Some kind of helper for the Guardian
void func_us_801D348C_from_are(Entity* self) {
    Entity* parent;
    u8 animCurFrame;

    parent = self - 1;

    self->facingLeft = parent->facingLeft;
    self->posX.i.hi = parent->posX.i.hi;
    self->posY.i.hi = parent->posY.i.hi;

    switch (self->step) {
    case 0:
        InitializeEntity(D_us_80180AD4);
        self->blendMode |= BLEND_TRANSP | BLEND_ADD;
        self->drawFlags |= ENTITY_OPACITY;
        self->animCurFrame = 0;
        break;
    case 1:
        if (parent->animCurFrame == 0x10 && parent->step == 6) {
            self->step = 2;
            self->animCurFrame = 0x20;
            self->opacity = 0x60;
            self->drawFlags = ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
            self->scaleX = 0x1C8;
            self->scaleY = 0x1C8;
        }
        if (parent->animCurFrame == 0x15) {
            self->step = 3;
            self->animCurFrame = 0x21;
            self->opacity = 0x60;
            self->drawFlags = ENTITY_OPACITY | ENTITY_SCALEY | ENTITY_SCALEX;
            self->scaleX = 0x1B8;
            self->scaleY = 0x1B8;
        }
        self->ext.armorLord.unk80 = 3;
        break;
    case 2:
        if (!--self->ext.armorLord.unk80) {
            self->animCurFrame = 0;
        } else {
            self->opacity -= 0x20;
        }
        if (parent->animCurFrame != 0x10) {
            self->step = 1;
        }
        break;
    case 3:
        if (!--self->ext.armorLord.unk80) {
            self->animCurFrame = 0;
        } else {
            self->opacity -= 0x20;
        }
        if (parent->animCurFrame != 0x15) {
            self->step = 1;
        }
        break;
    }

    animCurFrame = parent->animCurFrame;
    if (animCurFrame == 0x1E) {
        self->hitboxState = 3;
    } else {
        self->hitboxState = 1;
    }

    if (animCurFrame < 5 || animCurFrame > 30) {
        animCurFrame = 0;
    } else {
        animCurFrame -= 4;
    }

    self->hitboxOffX = D_us_80181C48[animCurFrame][0];
    self->hitboxOffY = D_us_80181C48[animCurFrame][1];
    self->hitboxWidth = D_us_80181BDC[animCurFrame][0];
    self->hitboxHeight = D_us_80181BDC[animCurFrame][1];

    if (parent->entityId != E_GUARDIAN) {
        DestroyEntity(self);
    }
}

// Another wave attack helper
void func_us_801D3700_from_are(Entity* self) {
    Primitive* prim;
    s32 height;
    s32 offsetY;

    if (!self->step) {
        height = self->hitboxHeight;
        offsetY = self->hitboxOffY;
        InitializeEntity(D_us_80180AE0);
        self->hitboxWidth = 8;
        self->hitboxOffX = 8;
        self->hitboxHeight = height;
        self->hitboxOffY = offsetY;
    }

    if (self->step++ > 5) {
        prim = self->ext.armorLord.prim;
        prim->next->v2 = 0;
        DestroyEntity(self);
    }
}
