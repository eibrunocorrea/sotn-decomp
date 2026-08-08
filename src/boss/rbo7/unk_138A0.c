// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo7.h"

INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", func_us_801BAB18_from_bo0);

// Donor: src/boss/rbo3/rbo3.c (matched; nome confirma doador rbo3).
// D_us_801805C8/D_us_80180564 are plain externs in the donor too (not
// defined there either -- global scratch flags shared across overlays),
// so they're declared the same way here.
extern EInit g_EInitInteractable;
extern s32 D_us_801805C8;
extern s32 D_us_80180564;

void func_us_80192B38_from_rbo3(Entity* self) {
    Entity* entity;
    s32 x;
    s32 y;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        // fallthrough
    case 1:
        entity = &PLAYER;
        x = entity->posX.i.hi + g_Tilemap.scrollX.i.hi;
        if (x > 128 && x < 384) {
            D_us_80180564 = 1;
            D_us_801805C8 = 1;
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_AKMODAN_II_DEFEAT, TIMEATTACK_SET_VISITED);
            stopMusicFlag = true;
            currentMusicId = MU_FESTIVAL_OF_SERVANTS;
            self->step++;
        }
        break;
    case 2:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;

    case 3:
        if (D_us_801805C8 & 2) {
            g_api.TimeAttackController(
                TIMEATTACK_EVENT_AKMODAN_II_DEFEAT, TIMEATTACK_SET_RECORD);
            g_api.PlaySfx(SET_UNK_90);
            currentMusicId = MU_FINAL_TOCATTA;
            self->step++;
        }
        break;

    case 4:
        if (D_us_801805C8 & 4) {
            self->step++;
        }
        break;

    case 5:
        // rbo7 fix: donor's y-position constant (0x80) and reward params
        // (0x11) are rbo3-specific; rbo7 wants 0x180 / 0x13 (confirmed via
        // strict-fn-diff).
        x = 256 - g_Tilemap.scrollX.i.hi;
        y = 0x180 - g_Tilemap.scrollY.i.hi;
        entity = AllocEntity(&g_Entities[0xA0], &g_Entities[0xC0]);
        if (entity == NULL) {
            break;
        }
        // rbo7 fix: donor's UNK_ENTITY_30 (rbo3's own enum slot 0x1E) is
        // rbo7's E_LIFE_UPSPAWN slot (0x18), confirmed via strict-fn-diff
        // (target loads "ori $a0, $zero, 0x18" right before the
        // CreateEntityFromEntity call).
        CreateEntityFromEntity(E_LIFE_UPSPAWN, self, entity);
        entity->posX.i.hi = x;
        entity->posY.i.hi = y;
        entity->params = 0x13;
        D_us_80180564 = 0;
        stopMusicFlag = true;
        currentMusicId = MU_FINAL_TOCATTA;
        self->step++;
        break;

    case 6:
        if (g_api.func_80131F68() == false) {
            stopMusicFlag = false;
            g_api.PlaySfx(currentMusicId);
            self->step++;
        }
        break;
    }
}

INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", func_us_801940B4);

INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", func_us_801957C0);

// TRAVADA: nz1's EntityHarpyKick (e_harpy.h) is only 89.5% mnemonic-similar
// (classe B). Transplant attempted; strict-fn-diff shows a real structural
// gap, not just constant drift -- rbo7's target has an extra 3-way branch
// on self->step (step 0 inits, step 1 jumps to a distinct label, step>=2
// skips straight to the shared harpy-tracking code) that the donor's plain
// "if (!self->step) { InitializeEntity(...); }" doesn't have, plus a
// different animFrame bounds check (target subtracts 0x23 and branches on
// sign instead of "> 13 then = 0"). Needs new C, not just re-parametrized
// donor. Reverted; see relatório final.
INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", EntityHarpyKick);

INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", func_us_80195A8C);

INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", func_us_80195D04);

// TRAVADA: no0's EntityCtulhuDeath (e_ctulhu.h) is 90% mnemonic-similar
// (classe B). Got very close: after reshaping the if(params)/else block to
// match rbo7's real branch structure (rbo7 sets a DIFFERENT palette/opacity
// per branch -- 0x2EE/8 vs 0x2E4/0x14 -- and only touches drawFlags in the
// if-branch, unlike no0's unconditional-before-the-if layout), strict-fn-
// diff got to compiled=61/original=61 (exact instruction count, all values
// correct) with only a 3-instruction scheduling reorder left: rbo7 loads
// self->params right after InitializeEntity, this build loaded it after
// the animSet/unk5A stores instead. Tried swapping animSet/unk5A order,
// made it worse (reverted). Full working reconstruction (verified via
// asm/us/boss/rbo7/nonmatchings/unk_138A0/EntityCtulhuDeath.s) for whoever
// picks this up:
//   if (self->params) {
//       self->drawFlags = ENTITY_OPACITY;
//       self->palette = 0x2EE;      // rbo7.h has no PAL_CTULHU_DEATH yet
//       self->opacity = 8;
//       self->blendMode = BLEND_TRANSP | BLEND_SUB;
//       self->flags &= ~FLAG_POS_CAMERA_LOCKED;
//   } else {
//       self->palette = 0x2E4;
//       self->opacity = 0x14;
//       self->blendMode = BLEND_TRANSP | BLEND_ADD;
//       self->zPriority += 2;
//   }
// self->animSet = 14; self->unk5A = 121; still need to land so the params
// load (lhu $v0, 0x30($s0)) schedules AFTER both, not right after
// InitializeEntity.
INCLUDE_ASM("boss/rbo7/nonmatchings/unk_138A0", EntityCtulhuDeath);
