// SPDX-License-Identifier: AGPL-3.0-or-later

// Shared by are/e_blade_soldier.c and the psp nova skeleton TUs (rno1/rno4),
// where the nova skeleton spawns blade-soldier style death parts.

#ifndef BLADE_DEATH_ANIM_FRAME_BASE
#define BLADE_DEATH_ANIM_FRAME_BASE 0x23
#endif

#ifndef BLADE_DEATH_FALL_DURATION_FIELD
#define BLADE_DEATH_FALL_DURATION_FIELD deathPartFallDuration
#endif

#ifndef BLADE_DEATH_ROTATION_TABLE
#define BLADE_DEATH_ROTATION_TABLE death_parts_rotation
#endif

#ifndef BLADE_DEATH_PARTS_NO_ROTATION_TABLE
static u16 death_parts_rotation[] = {
    ROT(22.5),  ROT(11.25),   ROT(6.328125), ROT(2.8125),
    ROT(5.625), ROT(1.40625), ROT(2.8125),   ROT(-2.8125)};
#endif

void EntityBladeSoldierDeathParts(Entity* self) {
    if (self->step) {
        if (--self->ext.bladeSoldier.BLADE_DEATH_FALL_DURATION_FIELD) {
            self->rotate += BLADE_DEATH_ROTATION_TABLE[self->params];
            FallEntity();
            MoveEntity();
            return;
        }

        self->entityId = E_EXPLOSION;
        self->pfnUpdate = EntityExplosion;
        self->params = EXPLOSION_SMALL;
        self->step = 0;
        return;
    }

    InitializeEntity(g_EInitBladeSoldier);
    self->hitboxState = 0;
    self->flags |=
        FLAG_DESTROY_IF_OUT_OF_CAMERA | FLAG_DESTROY_IF_BARELY_OUT_OF_CAMERA |
        FLAG_UNK_00200000 | FLAG_UNK_2000;
    self->animCurFrame = self->params + BLADE_DEATH_ANIM_FRAME_BASE;
    self->drawFlags = ENTITY_ROTATE;
    if (self->facingLeft) {
        self->velocityX = -self->velocityX;
    }
}
