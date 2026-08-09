// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo6.h"

#ifdef VERSION_PSP
// BO6 on PSP links two EInitLockCamera copies: the shared framework one owns
// the g_EInitLockCamera name (symexport, 0x092666A8), while the lock camera
// uses the overlay-local copy (BO6EInitLockCamera, 0x0926EB08).
#define g_EInitLockCamera BO6EInitLockCamera
#endif

#include "../../st/entity_lock_camera.h"
