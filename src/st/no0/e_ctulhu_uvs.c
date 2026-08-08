// SPDX-License-Identifier: AGPL-3.0-or-later
#include "no0.h"

// UV tables for EntityCtulhuIceShockwave, self-contained data window at 0x411B0.
// The NULL first entry is original: index 0 is never drawn.

static s16 D_us_801C11E8[];
static s16 D_us_801C1204[];
static s16 D_us_801C1220[];
static s16 D_us_801C123C[];
static s16 D_us_801C1258[];
static s16 D_us_801C1274[];
static s16 D_us_801C1290[];
static s16 D_us_801C12AC[];
static s16 D_us_801C12C8[];
static s16 D_us_801C12E4[];
static s16 D_us_801C1300[];
static s16 D_us_801C1330[];
static s16 D_us_801C134C[];

s16* ctulhu_shockwave_uvs[] = {
    NULL,
    D_us_801C11E8,
    D_us_801C1204,
    D_us_801C1220,
    D_us_801C123C,
    D_us_801C1258,
    D_us_801C1274,
    D_us_801C1290,
    D_us_801C12AC,
    D_us_801C12C8,
    D_us_801C12E4,
    D_us_801C1300,
    D_us_801C1330,
    D_us_801C134C,
};

static s16 D_us_801C11E8[] = {
    1, 0x18, -0xB, -0xC, 0x18, 0x18, 0, 0,
    0, 0x68, 0x18, 0x80, 0, 0,
};

static s16 D_us_801C1204[] = {
    1, 0x30, -0x10, -0x13, 0x28, 0x20, 0, 0,
    0, 0, 0x28, 0x20, 0, 0,
};

static s16 D_us_801C1220[] = {
    1, 0x20, -0x10, -0x15, 0x28, 0x20, 0, 0,
    0x28, 0, 0x50, 0x20, 0, 0,
};

static s16 D_us_801C123C[] = {
    1, 0x20, -0xF, -0x15, 0x28, 0x20, 0, 0,
    0x50, 0, 0x78, 0x20, 0, 0,
};

static s16 D_us_801C1258[] = {
    1, 0x10, -0x10, -0x19, 0x28, 0x28, 0, 0,
    0, 0x20, 0x28, 0x48, 0, 0,
};

static s16 D_us_801C1274[] = {
    1, 0, -0x11, -0x10, 0x28, 0x28, 0, 0,
    0x28, 0x20, 0x50, 0x48, 0, 0,
};

static s16 D_us_801C1290[] = {
    1, 0, -0x11, -0x11, 0x28, 0x28, 0, 0,
    0x50, 0x20, 0x78, 0x48, 0, 0,
};

static s16 D_us_801C12AC[] = {
    1, 0x10, -0x13, -0xF, 0x28, 0x20, 0, 0,
    0, 0x48, 0x28, 0x68, 0, 0,
};

static s16 D_us_801C12C8[] = {
    1, 0, -0x11, -0x10, 0x20, 0x20, 0, 0,
    0x28, 0x48, 0x48, 0x68, 0, 0,
};

static s16 D_us_801C12E4[] = {
    1, 0, -0xF, -0x10, 0x20, 0x20, 0, 0,
    0x48, 0x48, 0x68, 0x68, 0, 0,
};

static s16 D_us_801C1300[] = {
    2, 4, -0x13, -0x9, 0x10, 0x10, 0, 0,
    0x70, 0x68, 0x80, 0x78, 4, -0x3, -0x11, 0x10,
    0x20, 0, 0, 0x70, 0x48, 0x80, 0x68, 0,
};

static s16 D_us_801C1330[] = {
    1, 8, -0x1, -0x11, 0x10, 0x18, 0, 0,
    0x18, 0x68, 0x28, 0x80, 0, 0,
};

static s16 D_us_801C134C[] = {
    1, 8, 3, -0x11, 0x10, 0x18, 0, 0,
    0x28, 0x68, 0x38, 0x80, 0, 0,
};
