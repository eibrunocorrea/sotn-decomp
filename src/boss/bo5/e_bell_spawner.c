// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

extern EInit g_EInitInteractable;

// Two sets of x, y, bell_params index
static s16 bell_spawner_params[][3] = {{512, 456, 0}, {512, 589, 1}};

void EntityBellSpawner(Entity* self) {
    Entity* bell;
    s32 count;
    s16* ptr = *bell_spawner_params;

    if (!self->step) {
        InitializeEntity(g_EInitInteractable);
        for (bell = self + 1, count = 0; count < 2; count++, bell++) {
            CreateEntityFromCurrentEntity(E_ID(BELL), bell);
            bell->posX.i.hi = *ptr++ - g_Tilemap.scrollX.i.hi;
            bell->posY.i.hi = *ptr++ - g_Tilemap.scrollY.i.hi;
            bell->params = *ptr++;
        }
    }
}
