// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

void CutsceneCameraPan(s16 target) {
    s16 newTarget = 0x180 - target;
    s16 delta = newTarget - g_unkGraphicsStruct.unkC;

    if (delta > 1) {
        g_unkGraphicsStruct.unkC++;
    } else if (delta < -1) {
        g_unkGraphicsStruct.unkC--;
    } else {
        g_unkGraphicsStruct.unkC = newTarget;
    }
}
