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

#include "pg/drv8311.h"

typedef enum {
    // IO defined and DRV8311 was detected
    DRV8311_INIT_OK = 0,
    // IO defined, but DRV8311 could not be detected
    DRV8311_INIT_NOT_FOUND = -1,
    // No DRV8311 IO defined, which means either the
    // we don't have it or it's not properly configured
    DRV8311_INIT_NOT_CONFIGURED = -2,
} drv8311InitStatus_e;

drv8311InitStatus_e drvInit(const drv8311Config_t *config);

void drvEnable(void);
void drvDisable(void);

void drvWriteRpm(float const rpm[]);
void drvReadDiagnostics(void);
