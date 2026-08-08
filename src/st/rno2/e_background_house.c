// SPDX-License-Identifier: AGPL-3.0-or-later
#include "rno2.h"

// reverse castle: houses hang upside down from the top of the room and the
// model is projected with a much smaller vertical offset than the normal
// castle
#define HOUSE_GEOM_OFFSET_Y 0x30
#define HOUSE_POS_Y_BIAS 0x30
#define HOUSE_ROT_Z ROT(180)

#include "../e_background_house.h"
