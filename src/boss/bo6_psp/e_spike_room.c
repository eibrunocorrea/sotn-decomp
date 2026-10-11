// SPDX-License-Identifier: AGPL-3.0-or-later
#include "../bo6/bo6.h"

u8 IncreaseBrightness(Primitive* prim, u8 amount) {
    s32 i;
    s32 j;
    u8* colorPtr;
    u8* channelPtr;
    u8 isEnd;

    isEnd = 0;
    colorPtr = &prim->r0;
    for (i = 0; i < 4; colorPtr += OFF(Primitive, r1) - OFF(Primitive, r0),
        i++) {
        for (j = 0; j < 3; j++) {
            channelPtr = &colorPtr[j];
            *channelPtr -= amount;

            if (*channelPtr < 16) {
                *channelPtr = 16;
            } else {
                isEnd |= 1;
            }
        }
    }
    return isEnd;
}
