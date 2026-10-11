// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

extern EInit D_pspeu_0926EC18;
extern u8 D_pspeu_0927B490[46][5];
extern SVECTOR* D_pspeu_0927B1B0[46][4];

void func_pspeu_0926A738(Entity* self) {
    MATRIX m;
    SVECTOR rot;
    VECTOR trans1;
    SVECTOR pos = {0, 0, 0};
    SVECTOR sp50 = {-96, 0, 0};
    s32 z;
    s32 nclip;
    Primitive* prim;
    s32 temp_v1;
    s32 i;
    u8* primUVCoords;
    SVECTOR** vectors_ptr;
    SVECTOR* psp_s7;
    SVECTOR* psp_s6;
    SVECTOR* temp_a3;
    SVECTOR* psp_s4;
    if (self->step == 0) {
        self->ext.giantcross.subweaponId = PL_W_CRASH_CROSS_BEAM;
        InitializeEntity(D_pspeu_0926EC18);
        self->primIndex = g_api.func_800EDB58(PRIM_GT4, LEN(D_pspeu_0927B490));
        if (self->primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_UNK_10000000 | FLAG_HAS_PRIMS;
        prim = &g_PrimBuf[self->primIndex];
        while (prim) {
            prim->tpage = 0x1C;
            prim->drawMode = DRAW_UNK_100 | DRAW_HIDE;
            prim = prim->next;
        }
        self->hitboxHeight = 0x50;
        self->hitboxWidth = 0xC;
        self->facingLeft = 0;
        self->posY.i.hi = 0x160;
        self->velocityY = FIX(-6.0);
        self->ext.giantcross.unk7C = 0;
        self->ext.giantcross.unk7E = 0x400;
        g_api.PlaySfx(SFX_FIREBALL_SHOT_B);
        prim = &g_PrimBuf[self->primIndex];
        primUVCoords = D_pspeu_0927B490[0];
        for (i = 0; i < LEN(D_pspeu_0927B490); i++, prim = prim->next,
            primUVCoords += 5) {
            prim->clut = (primUVCoords[4] & 0xF) + 0x1A0;
            switch (primUVCoords[4] & 0xF0) {
            case 0x10:
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x20:
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1];
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1];
                break;
            case 0x30:
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1];
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1];
                break;
            case 0x40:
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x50:
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1] + primUVCoords[3];
                break;
            case 0x60:
                prim->u3 = primUVCoords[0];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1];
                prim->u0 = primUVCoords[0] + primUVCoords[2];
                prim->v0 = primUVCoords[1];
                break;
            case 0x70:
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                prim->u1 = primUVCoords[0];
                prim->v1 = primUVCoords[1] + primUVCoords[3];
                prim->u2 = primUVCoords[0] + primUVCoords[2];
                prim->v2 = primUVCoords[1];
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1];
                break;
            default:
                prim->u0 = primUVCoords[0];
                prim->v0 = primUVCoords[1];
                prim->u1 = primUVCoords[0] + primUVCoords[2];
                prim->v1 = primUVCoords[1];
                prim->u2 = primUVCoords[0];
                prim->v2 = primUVCoords[1] + primUVCoords[3];
                prim->u3 = primUVCoords[0] + primUVCoords[2];
                prim->v3 = primUVCoords[1] + primUVCoords[3];
                break;
            }
        }
        self->step = 1;
    } else {
        if (self->posY.i.hi < -0x40) {
            DestroyEntity(self);
            return;
        }
        if (self->posY.i.hi < 0) {
            self->velocityY -= FIX(0.5);
        }
        self->posY.val += self->velocityY;
        self->ext.giantcross.unk7C += 0x60;
        self->ext.giantcross.unk7E += 0x60;
        trans1.vx = -(rcos(self->ext.giantcross.unk7C) * 0x60) >> 0xC;
        trans1.vy = self->posY.i.hi - 0x80;
        trans1.vz = ((rsin(self->ext.giantcross.unk7C) * 0x60) >> 0xC) + 0x180;
        rot.vy = self->ext.giantcross.unk7E;
        rot.vz = 0x40;
        rot.vx = 0;
        SetGeomOffset(self->posX.i.hi, 120);
        gte_SetGeomScreen(320);
        RotMatrix(&rot, &m);
        TransMatrix(&m, &trans1);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&pos);
        gte_rtps();
        prim = &g_PrimBuf[self->primIndex];
        vectors_ptr = D_pspeu_0927B1B0[0];
        gte_stsxy((long*)&prim->x0);
        gte_stszotz((long*)&z);
        self->hitboxOffX = prim->x0 - self->posX.i.hi;
        self->hitboxOffY = prim->y0 - self->posY.i.hi;
        for (i = 0; i < LEN(D_pspeu_0927B490); i++, prim = prim->next,
            vectors_ptr += 4) {
            psp_s7 = vectors_ptr[0];
            psp_s6 = vectors_ptr[1];
            psp_s4 = vectors_ptr[3];
            gte_ldv3(psp_s7, psp_s6, psp_s4);
            gte_rtpt();
            temp_a3 = vectors_ptr[2];
            prim->type = PRIM_GT4;
            gte_nclip();
            prim->drawMode = DRAW_HIDE;
            gte_stopz(&nclip);
            if (nclip < 0) {
                continue;
            }
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_ldv0(temp_a3);
            gte_rtps();
            prim->drawMode = DRAW_DEFAULT;
            if (z < 16) {
                prim->priority = 0x1F6;
            } else if (z > 998) {
                prim->priority = 0x10;
            } else {
                prim->priority = 0xD0 - (z - 0x50);
            }
            gte_stsxy((long*)&prim->x3);
        }
    }
    FntPrint("judge:%02x\n", self->hitboxState);
}
