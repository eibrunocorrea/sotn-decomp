// SPDX-License-Identifier: AGPL-3.0-or-later

void MarDisableAfterImage(s32 resetAnims, s32 arg1) {
    Primitive* prim;

    if (resetAnims) {
        MARIA_AFTERIMAGE_1.ext.disableAfterImage.resetFlag = 1;
        MARIA_AFTERIMAGE_1.animCurFrame = MARIA_AFTERIMAGE_2.animCurFrame =
            MARIA_AFTERIMAGE_3.animCurFrame = 0;
        prim = &g_PrimBuf[MARIA_AFTERIMAGE_1.primIndex];

        while (prim != NULL) {
            prim->x1 = 0;
            prim = prim->next;
        }
    }
    MARIA_AFTERIMAGE_1.ext.disableAfterImage.disableFlag = 1;
    MARIA_AFTERIMAGE_1.ext.disableAfterImage.index = MaxAfterImageIndex;
    if (arg1) {
        g_Maria.timers[PL_T_AFTERIMAGE_DISABLE] = 4;
    }
}
