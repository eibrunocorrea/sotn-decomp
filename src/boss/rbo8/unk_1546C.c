// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo8.h"

void func_801CE1E8(s32 step) {
    s32 i;

    g_CurrentEntity->step = step;
    g_CurrentEntity->step_s = 0;
    g_CurrentEntity->pose = 0;
    g_CurrentEntity->poseTimer = 0;
    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

void func_801CE228() {
    s32 i;

    for (i = 0; i < 4; i++) {
        g_CurrentEntity->ext.GH_Props.unkB0[i] = 0;
        g_CurrentEntity->ext.GH_Props.unkB4[i] = 0;
    }
}

void polarPlacePartsList(s16* offsets) {
    Entity* entity;

    while (*offsets) {
        entity = g_CurrentEntity + *offsets;
        if (!entity->ext.GH_Props.unkA8) {
            polarPlacePart(entity);
        }
        offsets++;
    }
}
