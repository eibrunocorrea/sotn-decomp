// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rnz1.h"

/*
 * RNZ1's secret walls mirror NZ1's e_secrets for the inverted castle: tiles
 * are written right-to-left with negative row steps, the debris kick angles
 * keep the same quadrants, and the reverse castle plays no positional sound
 * effects when the walls crack.
 */

extern EInit g_EInitInteractable;
extern EInit g_EInitEnvironment;

extern s16 wall_1_tile_layout[];
extern s16 wall_2_tile_layout[];
extern u16 partial_wall_tiles[];
extern s32 item_drops[];

void EntityBreakableWall(Entity* self) {
    Entity* tempEntity;
    s32 tileIdx;
    s32 i;
    s32 castleFlag;
    s16* ptr;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        self->hitboxWidth = 0x10;
        self->hitboxHeight = 0x20;
        self->hitboxState = 2;
        castleFlag = RNZ1_LOWER_WALL_OPEN;
        if (!self->params) {
            castleFlag = RNZ1_UPPER_WALL_OPEN;
        }
        if (g_CastleFlags[castleFlag]) {
            i = 0x18;
        } else {
            i = 0;
        }
        if (!self->params) {
            ptr = &wall_1_tile_layout[i];
            tileIdx = 0x45F;
        } else {
            ptr = &wall_2_tile_layout[i];
            tileIdx = 0x391;
        }
        for (i = 0; i < 4; i++, ptr += 2) {
            *(&g_Tilemap.fg[tileIdx] + 0) = ptr[0];
            *(&g_Tilemap.fg[tileIdx] - 1) = ptr[1];
            if (!self->params) {
                tileIdx -= 0x70;
            } else {
                tileIdx -= 0x10;
            }
        }
        if (g_CastleFlags[castleFlag]) {
            DestroyEntity(self);
            return;
        }
        /* fallthrough */
    case 1:
        if (self->hitFlags) {
            g_api.PlaySfx(SFX_WALL_DEBRIS_B);
            self->step++;
        }
        break;

    case 2:
        self->ext.breakable.breakCount++;
        if (!self->params) {
            ptr = &wall_1_tile_layout[self->ext.breakable.breakCount * 8];
            tileIdx = 0x45F;
        } else {
            ptr = &wall_2_tile_layout[self->ext.breakable.breakCount * 8];
            tileIdx = 0x391;
        }
        for (i = 0; i < 4; i++, ptr += 2) {
            *(&g_Tilemap.fg[tileIdx] + 0) = ptr[0];
            *(&g_Tilemap.fg[tileIdx] - 1) = ptr[1];
            if (!self->params) {
                tileIdx -= 0x70;
            } else {
                tileIdx -= 0x10;
            }
        }
        tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (tempEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
            tempEntity->params = 0x13;
        }
        self->ext.breakable.resetTimer = 0x20;
        self->step++;
        if (self->ext.breakable.breakCount == 3) {
            if (!self->params) {
                g_CastleFlags[RNZ1_UPPER_WALL_OPEN] = 1;
                g_api.RevealSecretPassageAtPlayerPositionOnMap(
                    RNZ1_UPPER_WALL_OPEN);
            } else {
                g_CastleFlags[RNZ1_LOWER_WALL_OPEN] = 1;
                g_api.RevealSecretPassageAtPlayerPositionOnMap(
                    RNZ1_LOWER_WALL_OPEN);
            }
            for (i = 0; i < 8; i++) {
                tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (tempEntity != NULL) {
                    CreateEntityFromEntity(
                        E_ID(SECRET_WALL_DEBRIS), self, tempEntity);
                    tempEntity->posX.i.hi += (Random() & 0xF);
                    tempEntity->posY.i.hi += (Random() & 0x3F) - 0x20;
                    tempEntity->params = self->params ^ 1;
                }
            }
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

void EntityBreakableWallPartial(Entity* self) {
    Entity* tempEntity;
    s32 i;
    s32 tileX, tileY;
    s32 tileIdx;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitInteractable);
        if ((g_CastleFlags[RNZ1_STATUE_ROOM_BREAKABLE_WALLS] >> self->params) &
            1) {
            tileX = self->posX.i.hi;
            tileY = self->posY.i.hi + 0x10;
            tileX += g_Tilemap.scrollX.i.hi;
            tileY += g_Tilemap.scrollY.i.hi;
            for (i = 0; i < 3; i++) {
                tileIdx =
                    (tileX >> 4) + ((tileY >> 4) * g_Tilemap.hSize * 0x10);
                g_Tilemap.fg[tileIdx] = partial_wall_tiles[i + 3];
                tileY -= 0x10;
            }
            self->step = 0x10;
            return;
        }
        self->hitboxState = 2;
        self->hitPoints = 0x7FFF;
        self->hitboxWidth = 8;
        self->hitboxHeight = 0x18;
        /* fallthrough */
    case 1:
        if (self->hitFlags) {
            self->step++;
        }
        break;

    case 2:
        tileX = self->posX.i.hi;
        tileY = self->posY.i.hi + 0x10;
        tileX += g_Tilemap.scrollX.i.hi;
        tileY += g_Tilemap.scrollY.i.hi;
        for (i = 0; i < 3; i++) {
            tileIdx = (tileX >> 4) + ((tileY >> 4) * g_Tilemap.hSize * 0x10);
            g_Tilemap.fg[tileIdx] = partial_wall_tiles[i + 3];
            tileY -= 0x10;
        }
        self->hitboxState = 0;
        tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
        if (tempEntity != NULL) {
            CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
            tempEntity->params = 0x13;
        }
        for (i = 0; i < 3; i++) {
            tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(
                    E_ID(SECRET_WALL_DEBRIS), self, tempEntity);
                tempEntity->posY.i.hi += (Random() & 0x1F) - 0x10;
                tempEntity->params = 1;
            }
        }
        g_CastleFlags[RNZ1_STATUE_ROOM_BREAKABLE_WALLS] |= (1 << self->params);
        tempEntity = AllocEntity(&g_Entities[160], &g_Entities[192]);
        if (tempEntity != NULL) {
            CreateEntityFromEntity(E_EQUIP_ITEM_DROP, self, tempEntity);
            tempEntity->params = item_drops[self->params];
        }
        self->step++;
        break;
    }
}

void EntitySecretWallDebris(Entity* self) {
    Collider collider;
    Entity* tempEntity;
    s16 angle;
    s32 i;
    s32 speed;
    s32 x, y;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitEnvironment);
        self->drawFlags = ENTITY_ROTATE;
        self->animCurFrame = 8;
        speed = (Random() & 0x1F) + 0x10;
        if (self->params) {
            angle = ((Random() & 0x3F) * 0x10) + ROT(90);
        } else {
            angle = ((Random() & 0x3F) * 0x10) + ROT(270);
        }
        if (self->params & 0x10) {
            self->animCurFrame = 9;
            speed = (Random() & 0x1F) + 0x10;
            angle = (Random() * 6) + ROT(202.5);
        }
        self->velocityX = speed * rcos(angle);
        self->velocityY = speed * rsin(angle);
        if (self->velocityX < 0) {
            self->facingLeft = 1;
        }
        /* fallthrough */
    case 1:
        MoveEntity();
        self->rotate += 0x20;
        if (self->params) {
            self->rotate += 0x20;
        }
        self->velocityY += FIX(0.125);
        x = self->posX.i.hi;
        y = self->posY.i.hi + 6;
        g_api.CheckCollision(x, y, &collider, 0);
        if ((collider.effects & EFFECT_SOLID) == 0) {
            return;
        }
        self->posY.i.hi += collider.unk18;
        if (!(self->params & 0x10)) {
            g_api.PlaySfx(SFX_WALL_DEBRIS_B);
            for (i = 0; i < 2; i++) {
                tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (tempEntity != NULL) {
                    CreateEntityFromEntity(
                        E_ID(SECRET_WALL_DEBRIS), self, tempEntity);
                    tempEntity->params = 0x10;
                }
            }
            DestroyEntity(self);
            return;
        }
        if (self->velocityY < FIX(0.5)) {
            tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(E_INTENSE_EXPLOSION, self, tempEntity);
                tempEntity->params = 0x10;
            }
            DestroyEntity(self);
            return;
        }
        self->velocityY = -self->velocityY * 2 / 3;
        break;
    }
}
