// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno4.h"

typedef enum {
    BREAKABLE_CRYSTAL_FLOOR_INIT,
    BREAKABLE_CRYSTAL_FLOOR_IDLE,
    BREAKABLE_CRYSTAL_FLOOR_BREAK,
    BREAKABLE_CRYSTAL_FLOOR_CHECK
} BREAKABLE_CRYSTAL_FLOOR_STEPS;

// clang-format off
static s16 crystal_floor_tile_layout[] = {
    // Phase 0
    0x054C, 0x054D, 0x054E,
    0x0180, 0x0551, 0x0552,
    // Phase 1
    0x054C, 0x0740, 0x0748,
    0x0180, 0x0551, 0x0552,
    // Phase 2
    0x054C, 0x074D, 0x074E,
    0x0180, 0x0551, 0x0552,
    // Phase 3
    0x055E, 0x0000, 0x0000,
    0x055F, 0x0000, 0x0000
};

static s16 breakable_wall_tile_layout[] = {
    // Phase 0
    0x030E, 0x0310, 0x0766, 0x0312,
    0x076B, 0x0319, 0x0323, 0x0310,
    // Phase 1
    0x076D, 0x0310, 0x076E, 0x0312,
    0x076F, 0x0319, 0x0770, 0x0310,
    // Phase 2
    0x0771, 0x0310, 0x0772, 0x0312,
    0x0773, 0x0319, 0x0774, 0x0310,
    // Phase 3
    0x030F, 0x0310, 0x0334, 0x0314,
    0x0327, 0x031B, 0x0351, 0x0352};
// clang-format on

extern EInit g_EInitInteractable;

void EntityBreakableCrystalFloor(Entity* self) {
    Entity* newEntity;
    s16* tileLayoutPtr;
    s32 tilePos;
    s32 i;

    switch (self->step) {
    case BREAKABLE_CRYSTAL_FLOOR_INIT:
        InitializeEntity(g_EInitInteractable);
        self->hitboxWidth = 16;
        self->hitboxHeight = 16;
        self->hitboxState = 2;
        if (g_CastleFlags[RNO4_SECRET_CEILING_OPEN]) {
            i = 0x12;
        } else {
            i = 0;
        }
        tileLayoutPtr = &crystal_floor_tile_layout[i];
        tilePos = 0x2C;
        (&g_BgLayers[0].layout[tilePos])[-1] = 0x3FE;
        (&g_BgLayers[0].layout[tilePos])[-2] = 0x3FF;
        (&g_BgLayers[0].layout[tilePos])[-17] = 0x401;
        (&g_BgLayers[0].layout[tilePos])[-18] = 0x402;
        for (i = 0; i < 2; i++, tileLayoutPtr += 3) {
            (&g_Tilemap.fg[tilePos])[0] = tileLayoutPtr[0];
            (&g_Tilemap.fg[tilePos])[-1] = tileLayoutPtr[1];
            (&g_Tilemap.fg[tilePos])[-2] = tileLayoutPtr[2];
            tilePos -= 0x10;
        }
        if (g_CastleFlags[RNO4_SECRET_CEILING_OPEN]) {
            DestroyEntity(self);
            return;
        }
        // fallthrough
    case BREAKABLE_CRYSTAL_FLOOR_IDLE:
        if (self->hitFlags) {
            PlaySfxPositional(SFX_WALL_DEBRIS_B);
            self->step++;
        }
        break;
    case BREAKABLE_CRYSTAL_FLOOR_BREAK:
        self->ext.breakable.breakCount++;
        tileLayoutPtr =
            &crystal_floor_tile_layout[self->ext.breakable.breakCount * 6];
        tilePos = 0x2C;
        for (i = 0; i < 2; i++, tileLayoutPtr += 3) {
            (&g_Tilemap.fg[tilePos])[0] = tileLayoutPtr[0];
            (&g_Tilemap.fg[tilePos])[-1] = tileLayoutPtr[1];
            (&g_Tilemap.fg[tilePos])[-2] = tileLayoutPtr[2];
            tilePos -= 0x10;
        }
        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, newEntity);
            newEntity->params = 0x11;
        }
        self->ext.breakable.resetTimer = 32;
        self->step += 1;
        if (self->ext.breakable.breakCount == 3) {
            newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, newEntity);
                newEntity->params = 0x13;
            }
            g_CastleFlags[RNO4_SECRET_CEILING_OPEN] = 1;
            g_api.RevealSecretPassageAtPlayerPositionOnMap(
                RNO4_SECRET_CEILING_OPEN);
            DestroyEntity(self);
            return;
        }
        break;
    case BREAKABLE_CRYSTAL_FLOOR_CHECK:
        if (!--self->ext.breakable.resetTimer) {
            self->step = BREAKABLE_CRYSTAL_FLOOR_IDLE;
            return;
        }
        break;
    }
}

void EntityBreakableWall(Entity* self) {
    Entity* newEntity;
    s32 i;
    s32 tilePos;
    s16* tileLayoutPtr;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->hitboxWidth = 8;
        self->hitboxHeight = 0x20;
        self->hitboxState = 2;
        if (g_CastleFlags[RNO4_SECRET_WALL_OPEN]) {
            i = 0x18;
        } else {
            i = 0;
        }
        tileLayoutPtr = &breakable_wall_tile_layout[i];
        tilePos = 0x9F;

        for (i = 0; i < 4; tilePos -= 0x10, i++, tileLayoutPtr += 2) {
            (&g_Tilemap.fg[tilePos])[0] = tileLayoutPtr[0];
        }

        if (g_CastleFlags[RNO4_SECRET_WALL_OPEN]) {
            DestroyEntity(self);
            break;
        }
        // fallthrough
    case 1:
        if (self->hitFlags) {
            PlaySfxPositional(SFX_WALL_DEBRIS_B);
            self->step++;
        }
        break;
    case 2:
        tilePos = 0x9F;
        self->ext.breakable.breakCount++;
        tileLayoutPtr =
            &breakable_wall_tile_layout[self->ext.breakable.breakCount * 8];

        for (i = 0; i < 4; tilePos -= 0x10, i++, tileLayoutPtr += 2) {
            (&g_Tilemap.fg[tilePos])[0] = tileLayoutPtr[0];
        }
        newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (newEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, newEntity);
            newEntity->params = 0x11;
        }
        self->ext.breakable.resetTimer = 0x14;
        self->step++;
        if (self->ext.breakable.breakCount == 3) {
            for (i = 0; i < 0x10; i++) {
                newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (newEntity != NULL) {
                    CreateEntityFromEntity(
                        E_BREAKABLE_WALL_DEBRIS, self, newEntity);
                    newEntity->posX.i.hi += 4 - (Random() & 0xF);
                    newEntity->posY.i.hi += 0x1F - (i * 4);
                    newEntity->velocityX = FIX(0.5) - (Random() << 8);
                    newEntity->params = 1;
                }
            }

            g_api.PlaySfx(SFX_WALL_DEBRIS_B);
            g_CastleFlags[RNO4_SECRET_WALL_OPEN] = 1;
            g_api.RevealSecretPassageAtPlayerPositionOnMap(
                RNO4_SECRET_WALL_OPEN);
            DestroyEntity(self);
        }
        break;
    case 3:
        if (!--self->ext.breakable.resetTimer) {
            self->step = 1;
        }
        break;
    }
}
