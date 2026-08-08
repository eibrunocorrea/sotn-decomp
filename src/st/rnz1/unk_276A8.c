// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rnz1.h"
extern u8 D_us_80180F4C[];

extern EInit g_EInitInteractable;
extern EInit g_EInitEnvironment;


void EntityBackgroundGears(Entity* self) {
    Primitive* prim;
    s32 primIndex;
    s32 i, j;
    s32 posX;
    s32 posY;
    s32 u;
    s32 v;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->animSet = 0;
        self->posX.i.hi = 0;
        self->posY.i.hi = 0;
        self->unk68 = 0x80;
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 9);
        if (primIndex == -1) {
            DestroyEntity(self);
            return;
        }
        self->flags |= FLAG_HAS_PRIMS;
        self->primIndex = primIndex;
        prim = &g_PrimBuf[primIndex];
        self->ext.prim = prim;
        while (prim != NULL) {
            prim->tpage = 0xF;
            prim->clut = 8;
            prim->priority = 0x20;
            prim->drawMode = DRAW_UNK02;
            prim = prim->next;
        }
        // fallthrough

    case 1:
        AnimateEntity(D_us_80180F4C, self);

        posX = self->posX.i.hi;
        posX &= 0x7F;
        posX -= 128;
        posY = self->posY.i.hi;
        posY &= 0x7F;
        posY -= 128;
        u = 0;
        v = 0;
        if (self->animCurFrame == 2) {
            u = 128;
        }
        if (self->animCurFrame == 3) {
            v = 128;
        }

        prim = self->ext.prim;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                prim->x0 = prim->x2 = posX + (j * 128);
                prim->x1 = prim->x3 = posX + (j * 128) + 128;
                prim->y0 = prim->y1 = posY + (i * 128);
                prim->y2 = prim->y3 = posY + (i * 128) + 128;
                prim->u0 = prim->u2 = u;
                prim->u1 = prim->u3 = u + 127;
                prim->v0 = prim->v1 = v;
                prim->v2 = prim->v3 = v + 127;
                prim->drawMode = DRAW_DEFAULT;
                prim = prim->next;
            }
        }
        while (prim != NULL) {
            prim->drawMode = DRAW_HIDE;
            prim = prim->next;
        }
        break;
    }
}

void EntityGearSidewaysLarge(Entity* self) {
    Entity* player;
    s16 angle;
    s32 offsetX;
    s32 offsetY;
    s32 collision;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x6C;
        self->hitboxState = 1;
        self->hitboxWidth = 8;
        self->hitboxHeight = 3;
        self->animCurFrame = 3;
        self->drawFlags = ENTITY_ROTATE;
        self->ext.gearPuzzle.cooldownTimer = 0x80;
        // fallthrough

    case 1:
        self->rotate += 8;
        if (!--self->ext.gearPuzzle.cooldownTimer) {
            self->ext.gearPuzzle.cooldownTimer = 0x80;
            PlaySfxPositional(SFX_CLOCK_TOWER_GEAR);
        }

        player = &PLAYER;
        offsetX = player->posX.i.hi;
        offsetY = player->posY.i.hi + 26;
        offsetX -= self->posX.i.hi;
        offsetY -= self->posY.i.hi;
        angle = ratan2(offsetY, offsetX);
        if (angle <= 0) {
            offsetX = rcos(angle) * 54 * 16;
            offsetY = rsin(angle) * 54 * 16;
            self->hitboxOffX = offsetX >> 16;
            self->hitboxOffY = (offsetY >> 16) - 1;

            if (g_Player.status &
                (PLAYER_STATUS_MIST_FORM | PLAYER_STATUS_BAT_FORM)) {
                collision = 0;
            } else {
                collision = GetPlayerCollisionWith(self, 8, 3, 4);
            }

            if (collision & 4) {
                angle += 8;
                offsetX = (rcos(angle) * 54 * 16) - offsetX;
                offsetY = (rsin(angle) * 54 * 16) - offsetY;
                D_8006C38C.x = offsetY;

                player = &PLAYER;
                if (!(g_Player.vram_flag & TOUCHING_R_WALL)) {

                    player->posX.val += offsetX;
                    g_unkGraphicsStruct.shoveX.val += offsetX;
                }
                player->posY.val += offsetY + FIX(3);
                g_unkGraphicsStruct.shoveY.val += offsetY + FIX(3);
                self->ext.gearPuzzle.offsetX = angle;
            }
            self->ext.gearPuzzle.collision = collision;
        }
        break;
    }
}

#define D_us_80180FB8 D_us_80180F54
extern u8 D_us_80180FB8[];

void EntityGearHorizontal(Entity* self) {
    Entity* player;
    s32 collision;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x6C;
        // fallthrough

    case 1:
        AnimateEntity(D_us_80180FB8, self);
        collision = GetPlayerCollisionWith(self, 0x20, 8, 4);
        if (collision != 0) {
            player = &PLAYER;
            if (!self->params) {
                if (!(g_Player.vram_flag & TOUCHING_R_WALL)) {
                    player->posX.val += FIX(0.25);
                    g_unkGraphicsStruct.shoveX.val += FIX(0.25);
                }
            } else {
                if (!(g_Player.vram_flag & TOUCHING_L_WALL)) {
                    player->posX.val -= FIX(0.25);
                    g_unkGraphicsStruct.shoveX.val -= FIX(0.25);
                }
            }
        }
        break;
    }
}

#define D_us_80180FC0 D_us_80180F5C
extern AnimationFrame D_us_80180FC0[];

void EntityGearVertical(Entity* self) {
    Entity* player;
    s32 collision;
    s32 posY;
    s32 offsetY;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x6C;
        self->drawFlags = ENTITY_ROTATE;
        self->rotate = 0x400;
        // fallthrough

    case 1:
        AnimateEntity(D_us_80180FC0, self);
        if (g_Player.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) {
            collision = 4;
        } else {
            collision = 5;
        }
        collision = GetPlayerCollisionWith(self, 8, 0x20, collision | 0x8);
        self->ext.gearPuzzle.cooldownTimer = 0x20;
        if (collision & 4) {
            self->step++;
        }
        break;

    case 2:
        AnimateEntity(&D_us_80180FC0, self);
        collision = 0;
        self->ext.gearPuzzle.cooldownTimer--;
        if (!self->ext.gearPuzzle.cooldownTimer) {
            self->ext.gearPuzzle.timer2 = 0x20;
            self->step = 3;
        } else {
            player = &PLAYER;
            if (self->ext.gearPuzzle.collision & 4) {
                posY = self->posY.i.hi + g_Tilemap.scrollY.i.hi -
                       self->ext.gearPuzzle.cooldownTimer;
                offsetY = posY - self->ext.gearPuzzle.offsetY;
                player->posY.i.hi += offsetY;
                g_unkGraphicsStruct.shoveY.val += offsetY;
            }
            if (g_Player.vram_flag & (TOUCHING_L_WALL | TOUCHING_R_WALL)) {
                collision = 4;
            } else {
                collision = 5;
            }
            collision = GetPlayerCollisionWith(
                self, 8, self->ext.gearPuzzle.cooldownTimer, collision | 0x8);
            if (!(collision & 4)) {
                self->ext.gearPuzzle.timer2 = 0x10;
                self->step = 3;
            }
        }
        break;

    case 3:
        AnimateEntity(&D_us_80180FC0, self);
        collision = 0;

        if (!--self->ext.gearPuzzle.timer2) {
            self->step = 1;
        }
        break;
    }
    self->ext.gearPuzzle.collision = collision;
    self->ext.gearPuzzle.offsetY = self->posY.i.hi + g_Tilemap.scrollY.i.hi -
                                   self->ext.gearPuzzle.cooldownTimer;
}

#define D_us_80180FC8 D_us_80180F64
extern Point16 D_us_80180FC8[];

void EntityGearSidewaysSmall(Entity* self) {
    Entity* player;
    s16 angle;
    s32 offsetX;
    s32 offsetY;
    s32 collision;
    s32 params; // used for params and y-offset

    switch (self->step) {
    case 0x0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x6C;
        self->animCurFrame = 0xC;
        self->drawFlags = ENTITY_ROTATE;
        self->velocityY = FIX(0.5);
        // fallthrough

    case 0x1:

        player = &PLAYER;
        if (self->ext.gearPuzzle.collision & 4) {
            offsetY = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
            params = offsetY - self->ext.gearPuzzle.offsetY;
            player->posY.i.hi += params;
            g_unkGraphicsStruct.shoveY.i.hi += params;
        }

        self->rotate += 64;

        MoveEntity();
        offsetY = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
        params = self->params;
        if (offsetY < D_us_80180FC8[params].x) {
            self->velocityY = FIX(0.5);
        }
        if (D_us_80180FC8[params].y < offsetY) {
            self->velocityY = -FIX(0.5);
        }

        offsetX = player->posX.i.hi;
        offsetY = player->posY.i.hi + 25;
        offsetX -= self->posX.i.hi;
        offsetY -= self->posY.i.hi;

        angle = ratan2(offsetY, offsetX);
        if (angle <= 0) {
            offsetX = (14 * rcos(angle)) << 4;
            offsetY = (14 * rsin(angle)) << 4;
            self->hitboxOffX = (s16)(offsetX >> 0x10);
            self->hitboxOffY = (s16)(offsetY >> 0x10);
            collision = GetPlayerCollisionWith(self, 6, 2, 4);
            if (collision & 4) {
                angle += 64;
                offsetX = (rcos(angle) * 14 * 16) - offsetX;
                offsetY = (rsin(angle) * 14 * 16) - offsetY;

                player = &PLAYER;
                player->posX.val += offsetX;
                player->posY.val += FIX(1) + offsetY;
                g_unkGraphicsStruct.shoveX.val += offsetX;
                g_unkGraphicsStruct.shoveY.val += offsetY;
            }
        }
        break;
    case 0xFF:
#include "../pad2_anim_debug.h"
        break;
    }
    self->ext.gearPuzzle.collision = collision;
    self->ext.gearPuzzle.offsetY = self->posY.i.hi + g_Tilemap.scrollY.i.hi;
}

#define D_us_80180FD0 D_us_80180F6C
extern s32 D_us_80180FD0;
#define D_us_80180FD4 D_us_80180F70
extern s16 D_us_80180FD4[];
extern s16 D_us_801BEE2C[4];

void EntityWallGear(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->zPriority = 0x6C;
        self->animCurFrame = 0xC;
        self->drawFlags = ENTITY_ROTATE;
        self->velocityY = FIX(0.5);
        self->hitboxState = 2;
        self->hitPoints = S16_MAX;
        self->hitboxWidth = self->hitboxHeight = 0x10;
        self->rotate = D_us_801BEE2C[self->params];
        self->rotate &= 0xF00;
        // fallthrough

    case 1:
        self->hitboxState = 2;
        if (self->hitFlags) {
            self->ext.gearPuzzle.cooldownTimer = 16;
            self->step++;
        }
        D_us_801BEE2C[self->params] = self->rotate;
        break;

    case 2:
        self->hitboxState = 0;
        self->rotate += 16;
        if (!--self->ext.gearPuzzle.cooldownTimer) {
            self->rotate &= 0xFFF;
            if (self->rotate == D_us_80180FD4[self->params]) {
                D_us_80180FD0 |= 1 << self->params;
                g_api.PlaySfx(SFX_SWITCH_CLICK);
            } else {
                D_us_80180FD0 &= 0xFF - (1 << self->params);
                g_api.PlaySfx(SFX_LEVER_METAL_BANG);
            }
            self->step = 1;
        }
        break;
    }
}

INCLUDE_ASM("st/rnz1/nonmatchings/unk_276A8", EntitySecretAreaDoor);
