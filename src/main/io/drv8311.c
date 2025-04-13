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

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#include "pg/drv8311.h"

#include "common/utils.h"

#include "drv8311.h"


void drv8311Loop(timeUs_t currentTimeUs)
{
    // TODO: this is just for debugging the chip,
    // currently we are called once per second, e.g.,
    // to read diagnostics, set test rpm speeds, etc.
    // (remove this function when done with testing)
    UNUSED(currentTimeUs);
}
