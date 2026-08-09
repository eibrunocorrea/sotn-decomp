// SPDX-License-Identifier: AGPL-3.0-or-later

s32 MarCheckFacing(void) {
    if (g_Maria.unk44 & 2) {
        return 0;
    }

    if (MARIA.facingLeft == 1) {
        if (g_Maria.padPressed & PAD_RIGHT) {
            MARIA.facingLeft = 0;
            g_Maria.unk4C = 1;
            return -1;
        } else if (g_Maria.padPressed & PAD_LEFT) {
            return 1;
        }
    } else {
        if (g_Maria.padPressed & PAD_RIGHT) {
            return 1;
        }

        if (g_Maria.padPressed & PAD_LEFT) {
            MARIA.facingLeft = 1;
            g_Maria.unk4C = 1;
            return -1;
        }
    }

    return 0;
}
