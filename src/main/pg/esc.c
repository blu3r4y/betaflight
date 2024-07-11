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

PG_REGISTER_WITH_RESET_FN(escConfig_t, escConfig, PG_ESC_CONFIG, 0);

#ifndef ESC_HZ
#define ESC_HZ 100
#endif

#ifndef ESC_PIN0
#define ESC_PIN0 NONE
#endif

#ifndef ESC_PIN1
#define ESC_PIN1 NONE
#endif

#ifndef ESC_PIN2
#define ESC_PIN2 NONE
#endif

void pgResetFn_escConfig(escConfig_t *escConfig)
{
    escConfig->ioTags[0] = IO_TAG(ESC_PIN0);
    escConfig->ioTags[1] = IO_TAG(ESC_PIN1);
    escConfig->ioTags[2] = IO_TAG(ESC_PIN2);

    escConfig->frequency = ESC_HZ;
}
