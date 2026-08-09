// SPDX-License-Identifier: AGPL-3.0-or-later

void MarSetSpeedX(s32 speed) {
    if (g_CurrentEntity->facingLeft == 1)
        speed = -speed;
    g_CurrentEntity->velocityX = speed;
}
