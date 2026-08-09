// SPDX-License-Identifier: AGPL-3.0-or-later

// Syncs two of the blade boss body parts to a base position read
// from the parts table; indices and offsets vary per overlay.
#ifndef BLADE_SYNC_PART_A
#define BLADE_SYNC_PART_A 15
#define BLADE_SYNC_PART_B 16
#define BLADE_SYNC_OFFSET_A -0x600
#define BLADE_SYNC_OFFSET_B -0x600
#endif

static void func_801D0B40(void) {
    Entity* ent;
    s16* unk88;

    ent = g_CurrentEntity + BLADE_SYNC_PART_A;
    unk88 = ent->ext.et_801D0B40.unk88;
    ent->ext.et_801D0B40.unk84 = unk88[0x4E] + BLADE_SYNC_OFFSET_A;

    ent = g_CurrentEntity + BLADE_SYNC_PART_B;
    unk88 = ent->ext.et_801D0B40.unk88;
    ent->ext.et_801D0B40.unk84 = unk88[0x4E] + BLADE_SYNC_OFFSET_B;
}
