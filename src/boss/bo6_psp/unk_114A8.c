// SPDX-License-Identifier: AGPL-3.0-or-later
#include "player_copy.h"

extern PlayerState g_Maria;

extern EInit D_pspeu_0926EB98;            // slide kick hitbox entity init
extern AnimationFrame D_pspeu_0927A418[]; // rising sparkle anim
extern AnimationFrame D_pspeu_0927A5C0[]; // Maria's stop-run anim

Entity* MarCreateEntFactoryFromEntity(Entity* entity, u32 arg1, s32 arg2);
void MarSetBladeDash(void);
void MarSetAnimation(AnimationFrame* anim);
void MarDecelerateX(s32 speed);
void MarSetSpeedX(s32 speed);
s32 MarCheckFacing(void);
bool func_pspeu_0925B680(s32 checks);
void func_pspeu_09259BB0(s32 arg0);
void func_pspeu_09259AD8(s32 velocityX);
void func_pspeu_09259D48(void);
void func_pspeu_09259C58(void);
void func_pspeu_0925AA50(void);
void MarSetStep(s16 step);
void MarSetCrouch(s32 kind, s32 velocityX);
void func_maria_8015CC28(void);
void func_pspeu_0924EA50_from_rbo5(s32 velocityX);
s32 func_pspeu_09248D20(
    SubweaponDef* subweapon, s32 isItemCrash, s32 useHearts);
s32 MarCheckSubwpnChainLimit(s16 subwpnId, s16 limit);

// Maria's anim tables live in the raw data blobs of the overlay
extern AnimationFrame D_pspeu_0927A5E0[]; // stand
extern AnimationFrame D_pspeu_0927A6B0[]; // walk
extern AnimationFrame D_pspeu_0927A6D8[]; // crouch from stand 2
extern AnimationFrame D_pspeu_0927A6E8[]; // crouch
extern AnimationFrame D_pspeu_0927A6F0[]; // land from air run
extern AnimationFrame D_pspeu_0927A6F8[]; // crouch from stand
extern AnimationFrame D_pspeu_0927A718[]; // jump neutral
extern AnimationFrame D_pspeu_0927A738[]; // jump moving
extern AnimationFrame D_pspeu_0927A768[]; // fall
extern AnimationFrame D_pspeu_0927A790[]; // slide kick escape jump
extern AnimationFrame D_pspeu_0927A820[]; // subweapon stand
extern AnimationFrame D_pspeu_0927A870[]; // subweapon jump
extern AnimationFrame D_pspeu_0927A8A8[]; // run
extern AnimationFrame D_pspeu_0927A8D0[]; // death
extern AnimationFrame D_pspeu_0927A988[]; // slide
extern AnimationFrame D_pspeu_0927A9B8[]; // slide kick
extern AnimationFrame D_pspeu_0927A9C8[]; // slide kick bounce
extern AnimationFrame D_pspeu_0927A9E0[]; // high jump
extern AnimationFrame D_pspeu_0927AA20[]; // blade dash
extern s32 D_pspeu_0927C760;              // throw dagger timer (raw bss)

// local copy of RicGetFreeEntity (see us_39144.c)
Entity* MarGetFreeEntity(s16 start, s16 end) {
    Entity* entity = &g_Entities[start];
    s16 i;

    for (i = start; i < end; i++, entity++) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

// local copy of RicGetFreeEntityReverse (see us_39144.c)
Entity* MarGetFreeEntityReverse(s16 start, s16 end) {
    Entity* entity = &g_Entities[end - 1];
    s16 i;
    for (i = end - 1; i >= start; i--, entity--) {
        if (entity->entityId == E_NONE) {
            return entity;
        }
    }
    return NULL;
}

extern u8 D_pspeu_092797D0[][4];
extern u8 D_pspeu_0927C230;
extern u8 D_pspeu_0927C228;
extern u8 D_pspeu_0927C220;
extern u8 D_pspeu_0927C218;
// local copy of func_us_801BB314 (see bo6/us_39144.c)
void func_pspeu_09252480_from_rbo5(s32 arg0) {
    D_pspeu_0927C230 = D_pspeu_092797D0[arg0][0];
    D_pspeu_0927C228 = D_pspeu_092797D0[arg0][1];
    D_pspeu_0927C220 = D_pspeu_092797D0[arg0][2];
    D_pspeu_0927C218 = D_pspeu_092797D0[arg0][3];
}

extern SubweaponDef D_pspeu_092797E8[];
s32 func_pspeu_09248D20(
    SubweaponDef* actualSubwpn, s32 isItemCrash, s32 useHearts) {
    SubweaponDef* subwpn;
    SubweaponDef* wpn;
    s32 i;
    s32 subweaponId;
    s32 distanceX;

    distanceX = abs(MARIA.posX.i.hi - PLAYER.posX.i.hi);

    if (isItemCrash == 0) {
        if (distanceX < 0x50) {
            subweaponId = 3;
        } else {
            subweaponId = 4;
        }
        if ((MARIA.posY.i.hi - PLAYER.posY.i.hi) > 0x18) {
            subweaponId = 2;
        }
        if ((MARIA.posY.i.hi - PLAYER.posY.i.hi) < -0x18) {
            subweaponId = 3;
        }
        if (g_Player.status & PLAYER_STATUS_BAT_FORM) {
            subweaponId = 2;
        }

        *actualSubwpn = D_pspeu_092797E8[subweaponId];
    } else {
        subweaponId = 4;
        if (g_Maria.padPressed & PAD_L1) {
            subweaponId = 2;
        } else {
            if (g_Maria.padPressed & PAD_L2) {
                subweaponId = 3;
            }
            if (g_Maria.padPressed & PAD_START) {
                subweaponId = 9;
            }
            if (g_Maria.padPressed & PAD_SELECT) {
                subweaponId = 5;
            }
        }

        subwpn = &D_pspeu_092797E8[subweaponId];
        *actualSubwpn = D_pspeu_092797E8[subwpn->crashId];
    }
    return subweaponId;
}


extern u8 D_pspeu_09279A40[][8];
s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY) {
    s16 offset;
    s32 ret;
    u8* uvAnim;

    ret = 0;
    uvAnim = D_pspeu_09279A40[0];
    if (prim->b0 >= 6) {
        prim->b0 = 0;
        ret = -1;
    }

    uvAnim = &uvAnim[prim->b0 * 8];

    if (prim->b0 >= 3) {
        offset = 4;
    } else {
        offset = 6;
    }

    prim->x0 = posX - offset;
    prim->y0 = posY - offset;
    prim->x1 = posX + offset;
    prim->y1 = posY - offset;
    prim->x2 = posX - offset;
    prim->y2 = posY + offset;
    prim->x3 = posX + offset;
    prim->y3 = posY + offset;
    prim->u0 = *uvAnim++;
    prim->v0 = *uvAnim++;
    prim->u1 = *uvAnim++;
    prim->v1 = *uvAnim++;
    prim->u2 = *uvAnim++;
    prim->v2 = *uvAnim++;
    prim->u3 = *uvAnim++;
    prim->v3 = *uvAnim;

    prim->b1++;
    if (!(prim->b1 & 1)) {
        prim->b0++;
    }
    return ret;
}

s32 func_8015FDB0(Primitive* prim, s16 posX, s16 posY);
extern Point16 D_pspeu_0927C1D8[16];
// local copy of RicEntityHitByHoly (see ric/pl_blueprints.c); the boss
// walls track the doppleganger in slot 64 and camera-lock the effect
void func_pspeu_09252768_from_rbo5(Entity* entity) {
    Primitive* prim;
    s32 i;
    s32 temp;
    s16 hitboxX;
    s16 hitboxY;
    s16 temp_xRand;
    s16 temp_yRand;

    switch (entity->step) {
    case 0:
        entity->primIndex =
            (s16)g_api.AllocPrimitives(PRIM_GT4, LEN(D_pspeu_0927C1D8));
        if (entity->primIndex == -1) {
            DestroyEntity(entity);
            return;
        }
        entity->flags = FLAG_POS_CAMERA_LOCKED | FLAG_HAS_PRIMS;
        hitboxX = MARIA.posX.i.hi + MARIA.hitboxOffX;
        hitboxY = MARIA.posY.i.hi + MARIA.hitboxOffY;
        prim = &g_PrimBuf[entity->primIndex];
        for (i = 0; i < LEN(D_pspeu_0927C1D8); i++) {
            temp_xRand = hitboxX + rand() % 24 - 12;
            temp_yRand = hitboxY + rand() % 48 - 24;
            D_pspeu_0927C1D8[i].x = temp_xRand;
            D_pspeu_0927C1D8[i].y = temp_yRand;
            prim->clut = PAL_UNK_1B2;
            prim->tpage = 0x1A;
            prim->b0 = 0;
            prim->b1 = 0;
            prim->g0 = 0;
            prim->g1 = (rand() & 7) + 1;
            prim->g2 = 0;
            prim->priority = MARIA.zPriority + 4;
            prim->drawMode = DRAW_UNK_100 | DRAW_TPAGE | DRAW_HIDE |
                             DRAW_UNK02 | DRAW_TRANSP;
            if (rand() & 1) {
                prim->drawMode =
                    DRAW_UNK_100 | DRAW_UNK_40 | DRAW_TPAGE2 | DRAW_TPAGE |
                    DRAW_HIDE | DRAW_UNK02 | DRAW_TRANSP;
            }
            prim = prim->next;
        }
        entity->step++;
        break;
    case 1:
        if (!(g_Maria.status & PLAYER_STATUS_UNK10000)) {
            DestroyEntity(entity);
            return;
        }
        break;
    }

    prim = &g_PrimBuf[entity->primIndex];
    for (i = 0; i < 16; i++) {
        switch (prim->g0) {
        case 0:
            if (--prim->g1 == 0) {
                prim->g0++;
            }
            break;
        case 1:
            hitboxX = D_pspeu_0927C1D8[i].x;
            hitboxY = D_pspeu_0927C1D8[i].y;
            temp = func_8015FDB0(prim, hitboxX, hitboxY);
            D_pspeu_0927C1D8[i].y--;
            if (temp < 0) {
                prim->drawMode |= DRAW_HIDE;
                prim->g0++;
            } else {
                prim->drawMode &= ~DRAW_HIDE;
            }
            break;
        }
        prim = prim->next;
    }
}

extern AnimationFrame D_pspeu_09279A70[];
extern s32 D_pspeu_0927C1D0;
// local copy of RicEntityHitByDark (see ric/pl_blueprints.c); the boss
// walls use different flags and track the doppleganger's z-priority
void func_pspeu_09253EA0_from_rbo5(Entity* entity) {
    s16 x, y;

    switch (entity->step) {
    case 0:
        entity->flags = FLAG_UNK_20000000 | FLAG_POS_CAMERA_LOCKED;
        entity->unk5A = 0x79;
        entity->animSet = ANIMSET_DRA(14);
        entity->zPriority = MARIA.zPriority + 2;
        entity->palette = PAL_FLAG(PAL_UNK_19F);
        if (D_pspeu_0927C1D0 & 1) {
            entity->blendMode = BLEND_TRANSP | BLEND_QUARTER;
        } else {
            entity->blendMode = BLEND_TRANSP;
        }
        D_pspeu_0927C1D0++;
        entity->opacity = 0xFF;
        entity->drawFlags =
            ENTITY_SCALEX | ENTITY_SCALEY | ENTITY_MASK_R | ENTITY_MASK_G;
        entity->scaleX = entity->scaleY = 0x40;
        entity->anim = D_pspeu_09279A70;
        entity->posY.i.hi += (rand() % 35) - 15;
        entity->posX.i.hi += (rand() % 20) - 10;
        entity->velocityY = -0x6000 - (rand() & 0x3FFF);
        entity->step++;
        break;
    case 1:
        if (entity->opacity > 16) {
            entity->opacity -= 8;
        }
        entity->posY.val += entity->velocityY;
        entity->scaleX += 8;
        entity->scaleY += 8;
        if (entity->poseTimer < 0) {
            DestroyEntity(entity);
        }
        break;
    }
}

// declaring the ignored arg reproduces mwcc's 0x10 stack frame
void Unused09249778(s32 arg0) {}

