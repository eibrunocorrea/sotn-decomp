// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo6.h"

#include "../../st/player_is_within_hitbox.h"

// Donor: src/st/rcen/e_lock_camera.c (matched). The 3 tables below live in
// rbo6's own raw .data blob (asm/us/boss/rbo6/data/5D4.data.s) at their own
// addresses - extern, never re-declare with the donor's copied values
// (lesson 3; that duplication is exactly what got a previous attempt here
// reverted).
extern EInit g_EInitLockCamera;

extern u8 D_us_801805E4[];
extern u8 D_us_801805E8[];
extern u16 D_us_801805EC[];

void EntityLockCamera(Entity* self) {
    Tilemap* tilemap = &g_Tilemap;
    u16* dataPtr;
    u16 facingLeft;
    u16 params;
    s16 var_s4;

    params = self->params;
    if (!self->step) {
        InitializeEntity(g_EInitLockCamera);
        self->hitboxState = 1;
        facingLeft = self->ext.lockCamera.unk7C = D_us_801805E8[params];
        if (facingLeft) {
            self->hitboxWidth = D_us_801805E4[params];
            self->hitboxHeight = 0x14;
        } else {
            self->hitboxWidth = 0x14;
            self->hitboxHeight = D_us_801805E4[params];
        }
        self->ext.lockCamera.unk88 = 2;
    }

    switch (params) {
        // nb. needed for PSP but unused
    }

    if (PlayerIsWithinHitbox(self)) {
        facingLeft = GetSideToPlayer();
        if (self->ext.lockCamera.unk7C) {
            facingLeft &= 2;
            facingLeft *= 2;
        } else {
            facingLeft &= 1;
            facingLeft *= 4;
        }
        if (facingLeft != self->ext.lockCamera.unk88) {
            self->ext.lockCamera.unk88 = facingLeft;
            params = (params << 3) + facingLeft;
            dataPtr = &D_us_801805EC[params];
            self->ext.lockCamera.unk7E = 0;
            self->ext.lockCamera.unk8A = 0x10;
            params = tilemap->scrollX.i.hi;
            if (params != *dataPtr && self->ext.lockCamera.unk7C) {
                self->ext.lockCamera.unk7E = 1;
                tilemap->x = params;
            } else {
                tilemap->x = *dataPtr;
            }
            self->ext.lockCamera.unk80 = *dataPtr++;
            facingLeft = tilemap->scrollY.i.hi - 4;
            if (facingLeft != *dataPtr && !self->ext.lockCamera.unk7C) {
                self->ext.lockCamera.unk7E |= 2;
                tilemap->y = facingLeft;
            } else {
                tilemap->y = *dataPtr;
            }
            self->ext.lockCamera.unk82 = *dataPtr++;
            params += 0x100;
            if (params != *dataPtr && self->ext.lockCamera.unk7C) {
                self->ext.lockCamera.unk7E |= 4;
                tilemap->width = params;
            } else {
                tilemap->width = *dataPtr;
            }
            self->ext.lockCamera.unk84 = *dataPtr++;
            facingLeft += 0x100;
            if (facingLeft != *dataPtr && !self->ext.lockCamera.unk7C) {
                self->ext.lockCamera.unk7E |= 8;
                tilemap->height = facingLeft;
            } else {
                tilemap->height = *dataPtr;
            }
            self->ext.lockCamera.unk86 = *dataPtr;
        }
    } else {
        self->ext.lockCamera.unk88 = 2;
    }

    // nb. needed for PSP but unused
    switch (self->params) {
    case 4:
        break;
    }

    if (self->ext.lockCamera.unk7E) {
        if (!(--self->ext.lockCamera.unk8A)) {
            tilemap->x = self->ext.lockCamera.unk80;
            tilemap->y = self->ext.lockCamera.unk82;
            tilemap->width = self->ext.lockCamera.unk84;
            tilemap->height = self->ext.lockCamera.unk86;
            self->ext.lockCamera.unk7E = 0;
            return;
        }
        var_s4 = (self->ext.lockCamera.unk80 - tilemap->x) / 2;
        if (var_s4) {
            if (var_s4 > 0) {
                tilemap->x += 2;
            } else {
                tilemap->x -= 2;
            }
        } else {
            tilemap->x = self->ext.lockCamera.unk80;
            self->ext.lockCamera.unk7E &= ~1;
        }
        var_s4 = (self->ext.lockCamera.unk82 - tilemap->y) / 2;
        if (var_s4) {
            if (var_s4 > 0) {
                tilemap->y += 2;
            } else {
                tilemap->y -= 2;
            }
        } else {
            tilemap->y = self->ext.lockCamera.unk82;
            self->ext.lockCamera.unk7E &= ~2;
        }
        var_s4 = (self->ext.lockCamera.unk84 - tilemap->width) / 2;
        if (var_s4) {
            if (var_s4 > 0) {
                tilemap->width += 2;
            } else {
                tilemap->width -= 2;
            }
        } else {
            tilemap->width = self->ext.lockCamera.unk84;
            self->ext.lockCamera.unk7E &= ~4;
        }
        var_s4 = (self->ext.lockCamera.unk86 - tilemap->height) / 2;
        if (var_s4) {
            if (var_s4 > 0) {
                tilemap->height += 2;
            } else {
                tilemap->height -= 2;
            }
        } else {
            tilemap->height = self->ext.lockCamera.unk86;
            self->ext.lockCamera.unk7E &= ~8;
        }
    }
}
