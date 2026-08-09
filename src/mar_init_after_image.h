// SPDX-License-Identifier: AGPL-3.0-or-later

// Same function in DRA is func_8010D59C (InitPlayerAfterImage)
static u8 mar_801545B0[] = {
    10, 8, 8, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, -1, -1, 0, 0};
void InitMariaAfterImage(void) {
    Primitive* prim;
    s32 i;

    if (MARIA_AFTERIMAGE_1.ext.afterImage.disableFlag) {
        return;
    }
    if (g_Maria.padTapped & GAMEBUTTONS ||
        (g_Maria.padHeld & (g_Maria.padHeld ^ g_Maria.padPressed) &
         GAMEBUTTONS) ||
        MARIA.velocityY > FIX(0.5)) {
        MARIA_AFTERIMAGE_1.ext.afterImage.index = 0;
        MARIA_AFTERIMAGE_1.ext.afterImage.timer = 0;
    } else if (MARIA_AFTERIMAGE_1.ext.afterImage.index >= MaxAfterImageIndex) {
        return;
    } else {
        if (MARIA_AFTERIMAGE_1.ext.afterImage.timer == 0) {
            MARIA_AFTERIMAGE_1.ext.afterImage.timer =
                mar_801545B0[MARIA_AFTERIMAGE_1.ext.afterImage.index];
        }

        if (--MARIA_AFTERIMAGE_1.ext.afterImage.timer == 0) {
            MARIA_AFTERIMAGE_1.ext.afterImage.index++;
            MARIA_AFTERIMAGE_1.ext.afterImage.timer =
                mar_801545B0[MARIA_AFTERIMAGE_1.ext.afterImage.index];
        }
    }

    if (MARIA_AFTERIMAGE_1.pose) {
        MARIA_AFTERIMAGE_1.pose--;
        return;
    }

    for (prim = &g_PrimBuf[MARIA_AFTERIMAGE_1.primIndex], i = 0; prim != NULL;
         i++, prim = prim->next) {
        if (i == MARIA_AFTERIMAGE_1.entityId) {
            PGREY(prim, 0) = 0x80;
            prim->x0 = MARIA.posX.i.hi;
            prim->y0 = MARIA.posY.i.hi;
            prim->x1 = MARIA.animCurFrame;
            prim->y1 = 0;
            prim->x2 = MARIA.facingLeft;
            prim->y2 = MARIA.palette;
        }
    }

    MARIA_AFTERIMAGE_1.pose = 2;
    MARIA_AFTERIMAGE_1.entityId++;
    if (MARIA_AFTERIMAGE_1.entityId >= MaxAfterImages) {
        MARIA_AFTERIMAGE_1.entityId = 0;
    }
}
