// SPDX-License-Identifier: AGPL-3.0-or-later
#include "bo4.h"

#ifdef VERSION_PSP
// BO4 on PSP links two EInitLockCamera copies: the shared framework one owns
// the g_EInitLockCamera name (symexport, 0x092666A8), while the lock camera
// uses the overlay-local copy (BO4EInitLockCamera, 0x09263378).
#define g_EInitLockCamera BO4EInitLockCamera
#endif

#include "../../st/entity_lock_camera.h"
