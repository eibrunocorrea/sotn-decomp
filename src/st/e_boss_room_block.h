// SPDX-License-Identifier: AGPL-3.0-or-later

extern s32 g_BossFlag;
extern EInit g_EInitBossDoor;

#ifndef BOSS_ROOM_BLOCK_FLAG_CLOSE
#define BOSS_ROOM_BLOCK_FLAG_CLOSE 1
#endif
#ifndef BOSS_ROOM_BLOCK_FLAG_OPEN
#define BOSS_ROOM_BLOCK_FLAG_OPEN BOSS_FLAG_DOORS_OPEN
#endif

void EntityBossRoomBlock(Entity* self) {
    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitBossDoor);
        self->animCurFrame = 8;

    case 1:
        if (g_BossFlag & BOSS_ROOM_BLOCK_FLAG_CLOSE) {
            self->ext.GS_Props.timer = 16;
            self->step++;
        }
        break;

    case 2:
        if (self->params) {
            self->velocityX = FIX(-1);
        } else {
            self->velocityX = FIX(1);
        }
        MoveEntity();
        GetPlayerCollisionWith(self, 8, 8, 5);
        if (!(g_Timer & 3)) {
            g_api.PlaySfx(SFX_STONE_MOVE_B);
        }
        if (--self->ext.GS_Props.timer) {
            break;
        }
        self->step++;
        break;

    case 3:
        GetPlayerCollisionWith(self, 8, 8, 5);
        if (g_BossFlag & BOSS_ROOM_BLOCK_FLAG_OPEN) {
            self->step++;
        }
        break;

    case 4:
        self->flags |= FLAG_DESTROY_IF_OUT_OF_CAMERA;
        if (self->params) {
            self->velocityX = FIX(1);
        } else {
            self->velocityX = FIX(-1);
        }
        MoveEntity();
        break;
    }
}
