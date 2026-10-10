// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../rbo2/rbo2.h"

void CutsceneCameraPan(s16 target) {
    s16 delta = target - g_unkGraphicsStruct.unk14;

    if (delta > 1) {
        g_unkGraphicsStruct.unk14++;
    } else if (delta < -1) {
        g_unkGraphicsStruct.unk14--;
    } else {
        g_unkGraphicsStruct.unk14 = target;
    }
}
