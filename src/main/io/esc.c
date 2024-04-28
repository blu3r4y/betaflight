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

#include "drivers/io.h"
#include "pg/esc.h"
#include "drivers/io_impl.h"

#include "common/utils.h"

#include "esc.h"

static volatile bool toggle = false;
static IO_t escIO;


static void pinSet(IO_t led, bool on)
{
    IOWrite(led, on);
}

static void escToggleInit(const ioTag_t tag) {
    escIO = IOGetByTag(tag);

    if (escIO) {
        IOInit(escIO, OWNER_ESC, 0);
        IOConfigGPIO(escIO, IOCFG_OUT_PP);
        ledSet(0, false);
    }
}

void escInit(const escConfig_t *config) {
    escToggleInit(config->ioTag);
}

void escTogglePin(timeUs_t currentTimeUs)
{
    UNUSED(currentTimeUs);

    toggle = !toggle;
    pinSet(escIO, toggle);
}
