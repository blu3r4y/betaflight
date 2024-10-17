/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/time.h"

#include "drivers/io_types.h"

#include "pg/pg.h"

#define ESC_PIN_COUNT 6

#define ESC_AH 3
#define ESC_AL 2
#define ESC_BH 0
#define ESC_BL 1
#define ESC_CH 5
#define ESC_CL 4

typedef struct escConfig_s {
    ioTag_t ioTags[ESC_PIN_COUNT];
    uint16_t frequency;
} escConfig_t;

PG_DECLARE(escConfig_t, escConfig);

void escInit(const escConfig_t *config);
void escLoop(timeUs_t currentTimeUs);
