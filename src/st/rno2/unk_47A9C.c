// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"
extern s16 D_us_80181F38[];
extern s16 D_us_80181FF4[];
extern s16 D_us_8018207C[][2];
extern s16 D_us_801820A0[][2];
extern s16 D_us_801820B4[][2];
extern s16 D_us_801820C8[][2];
extern s16 D_us_801820E0[][2];
extern s16 D_us_801820F8[][2];
extern s16 D_us_80182114[][2];
extern u8 D_us_80181F64[];
extern u8 D_us_80181FA0[];

extern EInit g_EInitFlyingZombieHalf2;
extern EInit g_EInitFlyingZombieHalf1;
extern void EntityBloodDrips(Entity*);

// float/bob offsets applied to the severed half while it hovers
extern s16 D_us_80181FBC[24];

INCLUDE_ASM("st/rno2/nonmatchings/unk_47A9C", EntityFlyingZombie2);

void EntityFlyingZombie1(Entity* self) {
    Entity* tempEntity;
    s16 angle;
    s32 dx, dy;

    if ((self->flags & FLAG_DEAD) && self->step > 5) {
        self->step = 5;
        self->step_s = 0;
        self->hitboxState = 0;
        self->ext.flyingZombie.unk7E = 0;
        self->ext.flyingZombie.unk80 = 0;
        PlaySfxPositional(SFX_FLYING_ZOMBIE_DEATH);
        PlaySfxPositional(SFX_SMALL_FLAME_IGNITE);
    }
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitFlyingZombieHalf1);
        self->hitboxOffX = -4;
        self->hitboxOffY = -14;
        self->ext.flyingZombie.unk80 = 0;
        break;

    case 1:
        self->facingLeft = (self - 1)->facingLeft;
        if ((self->hitFlags & 3) || (self->flags & FLAG_DEAD)) {
            self->hitboxState = 0;
            self->flags &= ~FLAG_DEAD;
            self->hitPoints = g_api.enemyDefs[14].hitPoints;
            (self - 1)->step = 4;
            (self - 1)->step_s = 0;
            (self - 1)->pose = 0;
            (self - 1)->poseTimer = 0;
            self->step++;
        }
        break;

    case 3:
        self->hitboxState = 3;
        if (!self->step_s) {
            self->velocityY = FIX(-13.0 / 16);
            self->step_s++;
        }
        MoveEntity();
        self->velocityY += FIX(0.5 / 16);
        if (!AnimateEntity(D_us_80181FA0, self)) {
            self->step_s = 0;
            self->step = 6;
        }
        break;

    case 5:
        switch (self->step_s) {
        case 0:
            tempEntity = &PLAYER;
            dx = self->posX.val - tempEntity->posX.val;
            dy = self->posY.val - tempEntity->posY.val;
            angle = ratan2(dx, -dy);
            self->rotate = angle;
            self->velocityX = rsin(angle) * 40;
            self->velocityY = rcos(angle) * -40;
            self->ext.flyingZombie.unk7C = 2;
            self->step_s++;
            break;

        case 1:
            if (UnkCollisionFunc3(D_us_80181F38) & 1) {
                tempEntity = AllocEntity(&g_Entities[160], &g_Entities[192]);
                if (tempEntity != NULL) {
                    CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
                    tempEntity->params = 3;
                }
                DestroyEntity(self);
            } else {
                if (!--self->ext.flyingZombie.unk7C) {
                    tempEntity =
                        AllocEntity(&g_Entities[160], &g_Entities[192]);
                    if (tempEntity != NULL) {
                        CreateEntityFromEntity(E_EXPLOSION, self, tempEntity);
                        tempEntity->params = 1;
                        tempEntity->drawFlags = ENTITY_ROTATE;
                        tempEntity->rotate = self->rotate;
                    }
                    self->ext.flyingZombie.unk7C = 6;
                }
            }
            break;
        }
        break;

    case 6:
        MoveEntity();
        self->velocityY += FIX(0.5 / 16);
        if (self->velocityY > FIX(0.5)) {
            self->velocityY = 0;
            self->step = 7;
        }
        break;

    case 7:
        if ((Random() & 0x7F) == 0) {
            self->ext.flyingZombie.unk80 = 1;
        }
        if ((Random() & 0x1F) == 0) {
            tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (tempEntity != NULL) {
                CreateEntityFromEntity(E_BLOOD_DRIPS, self, tempEntity);
                tempEntity->posX.i.hi += (Random() & 8) - 4;
                tempEntity->posY.i.hi += 13;
                tempEntity->pfnUpdate = EntityBloodDrips;
            }
        }
        if (!self->step_s) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
            if (self->facingLeft) {
                self->velocityX = FIX(0.5);
            } else {
                self->velocityX = FIX(-0.5);
            }
            self->ext.flyingZombie.unk7C = 0x60;
            self->step_s++;
        } else {
            MoveEntity();
            AnimateEntity(D_us_80181F64, self);
            if (!--self->ext.flyingZombie.unk7C) {
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 0;
                self->step++;
            }
        }
        break;

    case 8:
        tempEntity = &PLAYER;
        if (!self->step_s) {
            self->ext.flyingZombie.unk7C = 0x40;
            angle = GetAngleBetweenEntitiesShifted(self, tempEntity);
            SetEntityVelocityFromAngle(angle, 6);
            self->step_s++;
        } else {
            if ((Random() & 0x7F) == 0) {
                self->ext.flyingZombie.unk80 = 1;
            }
            if ((Random() & 0x1F) == 0) {
                tempEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (tempEntity != NULL) {
                    CreateEntityFromEntity(E_BLOOD_DRIPS, self, tempEntity);
                    tempEntity->posX.i.hi += (Random() & 8) - 4;
                    tempEntity->posY.i.hi += 13;
                    tempEntity->pfnUpdate = EntityBloodDrips;
                }
            }
            MoveEntity();
            AnimateEntity(D_us_80181F64, self);
            if (!--self->ext.flyingZombie.unk7C) {
                self->velocityY = 0;
                self->pose = 0;
                self->poseTimer = 0;
                self->step_s = 0;
                self->step--;
            }
        }
        break;
    }
    if (self->step < 2) {
        self->posX.i.hi = (self - 1)->posX.i.hi;
        self->posY.i.hi = (self - 1)->posY.i.hi;
    }
    if (self->ext.flyingZombie.unk80 && g_Timer % 3 == 0) {
        self->posY.i.hi += D_us_80181FBC[self->ext.flyingZombie.unk7E];
        self->ext.flyingZombie.unk7E++;
        if (self->ext.flyingZombie.unk7E > LEN(D_us_80181FBC) - 1) {
            self->ext.flyingZombie.unk7E = 0;
            self->ext.flyingZombie.unk80 = 0;
        }
    }
}
