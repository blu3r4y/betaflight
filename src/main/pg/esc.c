#include "platform.h"

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

#include "drivers/io.h"
#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "esc.h"

PG_REGISTER_WITH_RESET_TEMPLATE(escConfig_t, escConfig, PG_ESC_CONFIG, 0);

#ifndef ESC_HZ
#define ESC_HZ   100
#endif

#ifndef BEEPER_PIN
#define BEEPER_PIN      NONE
#endif

PG_RESET_TEMPLATE(escConfig_t, escConfig,
    .ioTag = IO_TAG(BEEPER_PIN),
    .frequency = ESC_HZ
);
