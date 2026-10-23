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
#include "../e_imp.h"

#define BALLOON_POD_DEATH_PARTICLE E_IMP_DEATH_PARTICLE
#include "../e_balloon_pod.h"

#include "../e_imp_death_particle.h"
