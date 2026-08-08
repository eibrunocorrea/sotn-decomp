// SPDX-License-Identifier: AGPL-3.0-or-later
#include "no0.h"

// UV tables for EntityCtulhuIceShockwave, self-contained data window at 0x411B0.
// The NULL first entry is original: index 0 is never drawn.

static s16 s_CtulhuShockwaveUv1[];
static s16 s_CtulhuShockwaveUv2[];
static s16 s_CtulhuShockwaveUv3[];
static s16 s_CtulhuShockwaveUv4[];
static s16 s_CtulhuShockwaveUv5[];
static s16 s_CtulhuShockwaveUv6[];
static s16 s_CtulhuShockwaveUv7[];
static s16 s_CtulhuShockwaveUv8[];
static s16 s_CtulhuShockwaveUv9[];
static s16 s_CtulhuShockwaveUv10[];
static s16 s_CtulhuShockwaveUv11[];
static s16 s_CtulhuShockwaveUv12[];
static s16 s_CtulhuShockwaveUv13[];

s16* ctulhu_shockwave_uvs[] = {
    NULL,
    s_CtulhuShockwaveUv1,
    s_CtulhuShockwaveUv2,
    s_CtulhuShockwaveUv3,
    s_CtulhuShockwaveUv4,
    s_CtulhuShockwaveUv5,
    s_CtulhuShockwaveUv6,
    s_CtulhuShockwaveUv7,
    s_CtulhuShockwaveUv8,
    s_CtulhuShockwaveUv9,
    s_CtulhuShockwaveUv10,
    s_CtulhuShockwaveUv11,
    s_CtulhuShockwaveUv12,
    s_CtulhuShockwaveUv13,
};

static s16 s_CtulhuShockwaveUv1[] = {
    1, 0x18, -0xB, -0xC, 0x18, 0x18, 0, 0,
    0, 0x68, 0x18, 0x80, 0, 0,
};

static s16 s_CtulhuShockwaveUv2[] = {
    1, 0x30, -0x10, -0x13, 0x28, 0x20, 0, 0,
    0, 0, 0x28, 0x20, 0, 0,
};

static s16 s_CtulhuShockwaveUv3[] = {
    1, 0x20, -0x10, -0x15, 0x28, 0x20, 0, 0,
    0x28, 0, 0x50, 0x20, 0, 0,
};

static s16 s_CtulhuShockwaveUv4[] = {
    1, 0x20, -0xF, -0x15, 0x28, 0x20, 0, 0,
    0x50, 0, 0x78, 0x20, 0, 0,
};

static s16 s_CtulhuShockwaveUv5[] = {
    1, 0x10, -0x10, -0x19, 0x28, 0x28, 0, 0,
    0, 0x20, 0x28, 0x48, 0, 0,
};

static s16 s_CtulhuShockwaveUv6[] = {
    1, 0, -0x11, -0x10, 0x28, 0x28, 0, 0,
    0x28, 0x20, 0x50, 0x48, 0, 0,
};

static s16 s_CtulhuShockwaveUv7[] = {
    1, 0, -0x11, -0x11, 0x28, 0x28, 0, 0,
    0x50, 0x20, 0x78, 0x48, 0, 0,
};

static s16 s_CtulhuShockwaveUv8[] = {
    1, 0x10, -0x13, -0xF, 0x28, 0x20, 0, 0,
    0, 0x48, 0x28, 0x68, 0, 0,
};

static s16 s_CtulhuShockwaveUv9[] = {
    1, 0, -0x11, -0x10, 0x20, 0x20, 0, 0,
    0x28, 0x48, 0x48, 0x68, 0, 0,
};

static s16 s_CtulhuShockwaveUv10[] = {
    1, 0, -0xF, -0x10, 0x20, 0x20, 0, 0,
    0x48, 0x48, 0x68, 0x68, 0, 0,
};

static s16 s_CtulhuShockwaveUv11[] = {
    2, 4, -0x13, -0x9, 0x10, 0x10, 0, 0,
    0x70, 0x68, 0x80, 0x78, 4, -0x3, -0x11, 0x10,
    0x20, 0, 0, 0x70, 0x48, 0x80, 0x68, 0,
};

static s16 s_CtulhuShockwaveUv12[] = {
    1, 8, -0x1, -0x11, 0x10, 0x18, 0, 0,
    0x18, 0x68, 0x28, 0x80, 0, 0,
};

static s16 s_CtulhuShockwaveUv13[] = {
    1, 8, 3, -0x11, 0x10, 0x18, 0, 0,
    0x28, 0x68, 0x38, 0x80, 0, 0,
};
