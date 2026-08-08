// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rnz1.h"
extern u8 D_us_80181F94[];

extern EInit g_EInitMedusaHeadBlue;
extern EInit g_EInitMedusaHeadYellow;

void EntityMedusaHeadYellow(Entity* self);
void EntityMedusaHeadBlue(Entity* self);

typedef struct {
    s16 yMax;
    s16 yMin;
    s16 spawnDelay;
    s16 spawnCount;
    s16 yellowChance; // chance out of 16 that medusa head will be yellow
    s16 zPriority;
} MedusaHeadSpawnerParams;

// clang-format off
#define medusaHeadSpawnerParams D_us_80181F34
extern MedusaHeadSpawnerParams medusaHeadSpawnerParams[];
// clang-format on

void EntityMedusaHeadSpawner(Entity* self) {
    Entity* tempEntity;

    u8 index = self->params;
    MedusaHeadSpawnerParams* params = medusaHeadSpawnerParams;
    params += index;
    FntPrint("y:%02x\n", g_Tilemap.scrollY.i.hi);
    if (self->flags & FLAG_DEAD) {
        DestroyEntity(self);
        return;
    }
    if (!self->step) {
        InitializeEntity(g_EInitSpawner);
        self->flags &= ~FLAG_UNK_2000;
    }
    if ((g_Tilemap.scrollY.i.hi >= params->yMax) &&
        (g_Tilemap.scrollY.i.hi <= params->yMin) &&
        (LOH(PLAYER.posY.i.hi) >= 0x20) && (PLAYER.posY.i.hi < 0xC1)) {
        if (self->ext.medusaHead.timer) {
            self->ext.medusaHead.timer--;
            return;
        }
        tempEntity = AllocEntity(
            &g_Entities[128], &g_Entities[128 + params->spawnCount]);
        if (tempEntity != NULL) {
            DestroyEntity(tempEntity);
            if ((rand() & 0xF) < params->yellowChance) {
                tempEntity->entityId = E_MEDUSA_HEAD_YELLOW;
                tempEntity->pfnUpdate = EntityMedusaHeadYellow;
            } else {
                tempEntity->entityId = E_MEDUSA_HEAD_BLUE;
                tempEntity->pfnUpdate = EntityMedusaHeadBlue;
            }
            tempEntity->zPriority = params->zPriority;
            self->ext.medusaHead.timer = params->spawnDelay;
            return;
        }
        self->ext.medusaHead.timer++;
    }
}


typedef struct {
    s32 velocityX;
    s16 posX;
    s16 facingLeft;
} MedusaHeadInitParams;

#define medusaHeadInitParams D_us_80181F9C
extern MedusaHeadInitParams medusaHeadInitParams[];

void EntityMedusaHeadYellow(Entity* self) {
    self->params = 1;
    EntityMedusaHeadBlue(self);
}

void EntityMedusaHeadBlue(Entity* self) {
    s32 side;
    Entity* player = &PLAYER;

    if (self->flags & FLAG_DEAD) {
        EntityExplosionSpawn(0, 0);
        return;
    }
    if (self->step) {
        AnimateEntity(D_us_80181F94, self);
        if (self->velocityY > 0) {
            self->animCurFrame += 2;
        }
        self->velocityY += self->ext.medusaHead.accelY;
        side = self->velocityY;
        if (side < 0) {
            side = -side;
        }
        if (side >= FIX(2.5)) {
            self->ext.medusaHead.accelY = -self->ext.medusaHead.accelY;
        }
        MoveEntity();
        return;
    }

    if (!self->params) {
        InitializeEntity(g_EInitMedusaHeadBlue);
    } else {
        InitializeEntity(g_EInitMedusaHeadYellow);
    }

    self->posY.i.hi = player->posY.i.hi;
    self->posY.i.hi = (Random() & 0x7F) + 0x40;
    side = 0;
    if (player->posX.i.hi < 0x50) {
        side = 1;
    } else if (player->posX.i.hi < 0xB1) {
        if ((rand() & 3) == 0) {
            side = player->facingLeft;
        } else {
            side = ((player->facingLeft + 1) & 1);
        }
    }
    side = Random() & 1;
    self->posX.i.hi = medusaHeadInitParams[side].posX;
    self->velocityX = medusaHeadInitParams[side].velocityX;
    self->facingLeft = medusaHeadInitParams[side].facingLeft;
    self->velocityY = FIX(2.5) - ((Random() & 0xF) * FIX(2.5) >> 3);
    if (self->velocityY > 0) {
        self->ext.medusaHead.accelY = FIX(-5.0 / 32);
    } else {
        self->ext.medusaHead.accelY = FIX(5.0 / 32);
    }
}
