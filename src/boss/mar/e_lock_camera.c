// SPDX-License-Identifier: AGPL-3.0-or-later
#include "mar.h"

#ifdef VERSION_PSP
// MAR on PSP links two EInitLockCamera copies: the shared framework one owns
// the g_EInitLockCamera name (symexport, 0x092666A8), while the lock camera
// uses the overlay-local copy (MAREInitLockCamera, 0x0924BF10).
#define g_EInitLockCamera MAREInitLockCamera
#endif

#include "../../st/entity_lock_camera.h"
