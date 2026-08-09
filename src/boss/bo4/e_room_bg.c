// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo4.h"

#ifdef VERSION_PSP
// The boss-rush symexport hands g_EInitCommon to the shared framework copy;
// this overlay's own copy keeps the prefixed name.
#define g_EInitCommon BO4EInitCommon
#endif

#include "../../st/e_room_bg.h"
