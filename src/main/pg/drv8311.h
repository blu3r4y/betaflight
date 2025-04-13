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

#include "drivers/io_types.h"
#include "pg/pg.h"

#include "common/time.h"

// number of chips to be driven by this driver
#define DRV8311_DEVICE_COUNT 4

// number of output phases per chip
#define DRV8311_PHASE_COUNT 3

typedef struct drv8311Config_s {
    uint8_t spiDevice;

    ioTag_t faultTags[DRV8311_DEVICE_COUNT];
    ioTag_t csTags[DRV8311_DEVICE_COUNT];
    ioTag_t sleepTag;
    ioTag_t pwnSyncTag;

    // only supports measurements for one chip
    ioTag_t currentAdcTags[DRV8311_PHASE_COUNT];
} drv8311Config_t;

PG_DECLARE(drv8311Config_t, drv8311Config);

void drv8311Loop(timeUs_t currentTimeUs);
