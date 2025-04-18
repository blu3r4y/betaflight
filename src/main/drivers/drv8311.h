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

typedef enum {
    // Return status for OK
    DRV8311_OK = 0,
    // One of the input parameters was invalid
    DRV8311_INPUT_PARAM_CHECK_FAILED = -1,
} drv8311RetStatus_e;

drv8311InitStatus_e drvInit(const drv8311Config_t *config);

/**
 * @brief Enable all motors.
 *
 * This function powers up or activates all necessary components
 * to allow the motors to spin.
 */
void drvEnable(void);

/**
 * @brief Disable all motors.
 *
 * This function powers down all motors immediately
 */
void drvDisable(void);

/**
 * @brief Set the motor speed as a percentage-based PWM signal.
 *
 * Writes the desired motor speed in percent (0 to 100) to each channel.
 *
 * @param[in] rpm  Array of PWM values in percent (0–100) for each motor channel.
 *                 The length of the array depends on the number of connected motors.
 */
void drvWriteRpm(uint8_t const rpm[]);

/**
 * @brief Read and process diagnostics from the motor driver.
 *
 * This function reads diagnostic information (e.g., faults, status flags)
 * from the driver and stores them to an internal struct.
 */
void drvReadDiagnostics(void);
