// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

// Matched source exists (see docs/analysis/bugs-port-nativo/
// rno2-801b4da4-rect-template.c.txt): IDENTICO at function level via
// src/st/no2_unk_34DA4.h, but the RECT initializer rodata template is
// SHARED with e_spikes in the original overlay (single 8-byte template
// at 0x31C4C serves both objects), which our per-TU build cannot
// reproduce without upstream guidance. Reverted to keep checksum green.
INCLUDE_ASM("st/rno2/nonmatchings/func_us_801b4da4_from_bo0", func_us_801B4DA4_from_bo0);
