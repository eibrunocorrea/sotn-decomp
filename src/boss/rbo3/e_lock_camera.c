// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rbo3.h"

#ifdef VERSION_PSP
// RBO3 on PSP links two EInitLockCamera copies: the shared framework one owns
// the g_EInitLockCamera name (symexport, 0x092666A8), while the lock camera
// uses the overlay-local copy (RBO3EInitLockCamera, 0x0924A610).
#define g_EInitLockCamera RBO3EInitLockCamera
#endif

#include "../../st/entity_lock_camera.h"
