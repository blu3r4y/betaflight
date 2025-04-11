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

#define DRV8311_PIN_COUNT 6

#define DRV8311_AH 3
#define DRV8311_AL 2
#define DRV8311_BH 0
#define DRV8311_BL 1
#define DRV8311_CH 5
#define DRV8311_CL 4

typedef struct drv8311Config_s {
    ioTag_t ioTags[DRV8311_PIN_COUNT];
    uint16_t frequency;
} drv8311Config_t;

PG_DECLARE(drv8311Config_t, drv8311Config);

void drv8311Init(const drv8311Config_t *config);
void drv8311Loop(timeUs_t currentTimeUs);
