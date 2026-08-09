// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rchi.h"

// Donor: src/st/rcat/e_bat.c, via the shared header src/st/e_bat.h
// (matched). That header already has a dedicated "STAGE_IS_RCHI nested
// inside STAGE_IS_RCAT" branch (rchi being the same cave family as rcat),
// but rchi.h only defines STAGE_IS_RCHI on its own. STAGE_IS_RCAT is
// defined locally here instead of adding it to rchi.h, so this doesn't
// risk changing behavior for any other already-matched rchi source file
// that might also check it.
#define STAGE_IS_RCAT

#include "../e_bat.h"
