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

#ifndef ESC_PINAH
#define ESC_PINAH NONE
#endif

#ifndef ESC_PINAL
#define ESC_PINAL NONE
#endif

#ifndef ESC_PINBH
#define ESC_PINBH NONE
#endif

#ifndef ESC_PINBL
#define ESC_PINBL NONE
#endif

#ifndef ESC_PINCH
#define ESC_PINCH NONE
#endif

#ifndef ESC_PINCL
#define ESC_PINCL NONE
#endif


void pgResetFn_escConfig(escConfig_t *escConfig)
{
    escConfig->ioTags[ESC_AH] = IO_TAG(ESC_PINAH);
    escConfig->ioTags[ESC_AL] = IO_TAG(ESC_PINAL);
    escConfig->ioTags[ESC_BH] = IO_TAG(ESC_PINBH);
    escConfig->ioTags[ESC_BL] = IO_TAG(ESC_PINBL);
    escConfig->ioTags[ESC_CH] = IO_TAG(ESC_PINCH);
    escConfig->ioTags[ESC_CL] = IO_TAG(ESC_PINCL);

    escConfig->frequency = ESC_HZ;
}
