// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

// TU histórico da Konami (e_imp.c do rno4): reconstruído pela cicatriz de
// alinhamento — a jtbl do EntityImp abre a .rodata deste objeto (offset 0,
// 801C4844) e a string charal da RdaiUnk33 é a SEGUNDA cópia do overlay
// (D_us_801C4874): TU separado não fazia pool com o D_us_801C47D8 do
// unk_58A30. Ordem histórica: Imp -> Smoke -> RdaiUnk33 -> DeathParticle.

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
#define anim_imp D_us_80182B78
#include "../e_imp.h"

#define g_EInitImpSmoke D_us_80180C38
#define anim_imp_smoke D_us_80182B90
#include "../e_imp_smoke.h"

// NOTE: not #include "../e_rdai_unk33.h" (the shared header) — its body uses
// the placeholder entity id E_UNK_34, which is a per-overlay enum slot. In
// rdai's own enum that slot lines up with rdai's death-particle entity, but
// in rno4.h it lands on an unrelated unmatched function's slot (0x34). The
// real target here is rno4's own E_IMP_DEATH_PARTICLE slot (0x5D), confirmed
// by strict-fn-diff against asm/us/st/rno4/nonmatchings/unk_58A30/EntityRdaiUnk33.s.
// Editing the shared header to fix this would need touching rdai's copy too
// (out of this territory), so the body is copied locally instead.
extern EInit g_EInitBalloonpod;
extern u8 D_us_80182BAC[];
extern u8 D_us_80182BC0[];
extern u8 D_us_80182BD0[];
extern u8 D_us_80182BE4[];
#if defined(VERSION_PSP)
extern char g_RdaiUnk33DebugText[];
#endif

#define RDAI_UNK33_DEATH_STARTED(self) (((u8*)&(self)->ext)[8])
#if defined(VERSION_PSP)
#define RDAI_UNK33_PAD_PRESSED g_pads[1].pressed
#else
extern u16 g_pads_1_pressed;
#define RDAI_UNK33_PAD_PRESSED g_pads_1_pressed
#endif

void EntityRdaiUnk33(Entity* self) {
    Entity* entity;
    s32 i;

    if ((self->flags & FLAG_DEAD) && !RDAI_UNK33_DEATH_STARTED(self)) {
        PlaySfxPositional(SFX_EXPLODE_B);
        if (self->params) {
            SetStep(5);
        } else {
            SetStep(3);
        }
        RDAI_UNK33_DEATH_STARTED(self) = 1;
    }

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitBalloonpod);
        if (self->params) {
            SetStep(4);
            break;
        }
        self->hitboxOffY = -4;
        SetStep(2);
        break;

    case 2:
        AnimateEntity(D_us_80182BAC, self);
        break;

    case 3:
        if (!AnimateEntity(D_us_80182BC0, self)) {
            for (i = 0; i < 16; i++) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_IMP_DEATH_PARTICLE, self, entity);
                }
            }
            DestroyEntity(self);
        }
        break;

    case 4:
        AnimateEntity(D_us_80182BD0, self);
        break;

    case 5:
        if (!AnimateEntity(D_us_80182BE4, self)) {
            for (i = 0; i < 16; i++) {
                entity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (entity != NULL) {
                    CreateEntityFromEntity(E_IMP_DEATH_PARTICLE, self, entity);
                }
            }
            DestroyEntity(self);
        }
        break;

    case 0xFF:
#if defined(VERSION_PSP)
        FntPrint(g_RdaiUnk33DebugText, self->animCurFrame);
#else
        FntPrint("charal %x\n", self->animCurFrame);
#endif
        if (RDAI_UNK33_PAD_PRESSED & PAD_SQUARE) {
            if (self->params) {
                break;
            }
            self->animCurFrame++;
            self->params |= 1;
        } else {
            self->params = 0;
        }
        if (RDAI_UNK33_PAD_PRESSED & PAD_CIRCLE) {
            if (self->step_s) {
                break;
            }
            self->animCurFrame--;
            self->step_s |= 1;
        } else {
            self->step_s = 0;
        }
        break;
    }
}

#undef RDAI_UNK33_DEATH_STARTED
#undef RDAI_UNK33_PAD_PRESSED

#define g_EInitImpDeathParticle D_us_80180C50
#define g_ImpDeathParticleAnim D_us_80182BF4
#include "../e_imp_death_particle.h"
