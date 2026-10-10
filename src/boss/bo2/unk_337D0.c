// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo2.h"

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B37D0);

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B385C);

#include "e_werewolf_attack_hitbox.h"

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B503C);

#include "e_werewolf_after_image.h"

INCLUDE_ASM("boss/bo2/nonmatchings/unk_337D0", func_us_801B52FC);

extern EInit g_EInitInteractable;
// Both tables already live in bo2's own raw .data blob (see
// asm/us/boss/bo2/data/1850.data.s) - extern, never re-declare with values
// even though the bytes happen to match the donor's (lesson 3).
extern SVECTOR D_us_80181ACC[];
extern s16* D_us_801A16F0[];

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
