// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo5.h"

extern EInit g_EInitCommon;

void func_801A8620(Entity* entity) {
    s16 dist;
    s16 params = entity->params;

    switch (entity->step) {
    case 0:
        InitializeEntity(g_EInitCommon);
        break;

    case 1:
        dist = entity->posY.i.hi - PLAYER.posY.i.hi;
        dist = abs(dist);

        if (dist < 0x20) {
            switch (params) {
            case 0:
                if (g_PlayerX > 0x280) {
                    g_Tilemap.width = 0x280;
                    g_Tilemap.right--;
                    entity->step++;
                }
                break;

            case 1:
                if (g_PlayerX < 0x180) {
                    g_Tilemap.x = 0x180;
                    g_Tilemap.left++;
                    entity->step++;
                }
                break;

            case 3:
                if (g_PlayerX < 0x100) {
                    g_Tilemap.x = 0x100;
                    g_Tilemap.left++;
                    entity->step++;
                }
                break;

            case 5:
                if (g_PlayerX < 0x80) {
                    g_Tilemap.x = 0x80;
                    entity->step++;
                }
                break;

            case 6:
                if (g_PlayerX > 0x480) {
                    g_Tilemap.width = 0x480;
                    entity->step++;
                }
                break;

            case 7:
                if (g_PlayerX > 0x480) {
                    g_Tilemap.width = 0x480;
                    entity->step++;
                }
                break;

            case 8:
                if (g_PlayerX < 0x80) {
                    g_Tilemap.x = 0x80;
                    entity->step++;
                }
                break;

            case 9:
                if (g_PlayerX > 0x280) {
                    g_Tilemap.width = 0x280;
                    entity->step++;
                }
                break;

            case 10:
                if (g_PlayerX < 0x180) {
                    g_Tilemap.x = 0x180;
                    g_Tilemap.left++;
                    entity->step++;
                }
                break;

            case 11:
                if (g_PlayerX > 0x280) {
                    g_Tilemap.width = 0x280;
                    g_Tilemap.right--;
                    entity->step++;
                }
                break;

            case 12:
                if (g_PlayerX < 0x180) {
                    g_Tilemap.x = 0x180;
                    g_Tilemap.left++;
                    entity->step++;
                }
                break;

            case 2:
            case 4:
            case 13:
            case 14:
                if (g_PlayerX > 0x300) {
                    g_Tilemap.width = 0x300;
                    g_Tilemap.right--;
                    entity->step++;
                }
                break;
            }
        }
        break;
    }
}
