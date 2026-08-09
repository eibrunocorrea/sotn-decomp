// SPDX-License-Identifier: AGPL-3.0-or-later

// Extremely similar to func_8010D800 (DrawPlayerAfterImage)

// which g_PlayerDraw slots render Maria's afterimages
#ifndef MAR_DRAW_AFTER_IMAGE_SLOT
#define MAR_DRAW_AFTER_IMAGE_SLOT 6
#endif

static u8 mar_801545C4[] = {
    4, 4, 4, 4, 6, 6, 6, 6, 8, 8, 16, 16, 16, 16, 16, 16};
static u8 mar_801545D4[] = {
    8, 12, 16, 20, 24, 28, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
void DrawMariaAfterImage(void) {
    PlayerDraw* draw;
    Primitive* prim;
    s32 i;
    u8 var_s5;
    u8 resetAnim;
    u8 var_s3;

    resetAnim = MARIA_AFTERIMAGE_1.ext.afterImage.resetFlag;
    prim = &g_PrimBuf[MARIA_AFTERIMAGE_1.primIndex];
    i = 0;
    draw = &g_PlayerDraw[MAR_DRAW_AFTER_IMAGE_SLOT];
    var_s5 = mar_801545C4[MARIA_AFTERIMAGE_1.ext.afterImage.index];
    var_s3 = mar_801545D4[MARIA_AFTERIMAGE_1.ext.afterImage.index];

    while (prim != NULL) {
        if (prim->r0 > var_s3) {
            prim->r0 -= var_s5;
        }

        if (prim->r0 < 112 && prim->b0 < 240) {
            prim->b0 += 6;
        }

        if (prim->r0 < 88) {
            prim->y1 = 16;
        } else {
            prim->y1 = 0;
        }

        if (prim->r0 <= var_s3) {
            prim->x1 = 0;
        }

        if ((i ^ g_Timer) & 1) {
            g_Entities[(i / 2) + 65].posX.i.hi = prim->x0;
            g_Entities[(i / 2) + 65].posY.i.hi = prim->y0;
            g_Entities[(i / 2) + 65].animCurFrame = prim->x1;
            g_Entities[(i / 2) + 65].blendMode = prim->y1;
            g_Entities[(i / 2) + 65].facingLeft = prim->x2;
            g_Entities[(i / 2) + 65].palette = prim->y2;
            g_Entities[(i / 2) + 65].zPriority = MARIA.zPriority - 2;
            if (resetAnim) {
                g_Entities[(i / 2) + 65].animCurFrame = 0;
                prim->x1 = 0;
            }

            draw->r0 = draw->r1 = draw->r2 = draw->r3 = draw->g0 = draw->g1 =
                draw->g2 = draw->g3 = prim->r0;
            draw->b0 = draw->b1 = draw->b2 = draw->b3 = prim->b0;
            draw->enableColorBlend = true;
            draw++;
        }

        i++;
        prim = prim->next;
    }
}
