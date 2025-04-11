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

#include "drv8311.h"

PG_REGISTER_WITH_RESET_FN(drv8311Config_t, drv8311Config, PG_DRV8311_CONFIG, 0);

#ifndef DRV8311_HZ
#define DRV8311_HZ 1
#endif

#ifndef DRV8311_PINAH
#define DRV8311_PINAH NONE
#endif

#ifndef DRV8311_PINAL
#define DRV8311_PINAL NONE
#endif

#ifndef DRV8311_PINBH
#define DRV8311_PINBH NONE
#endif

#ifndef DRV8311_PINBL
#define DRV8311_PINBL NONE
#endif

#ifndef DRV8311_PINCH
#define DRV8311_PINCH NONE
#endif

#ifndef DRV8311_PINCL
#define DRV8311_PINCL NONE
#endif


void pgResetFn_drv8311Config(drv8311Config_t *drv8311Config)
{
    drv8311Config->ioTags[DRV8311_AH] = IO_TAG(DRV8311_PINAH);
    drv8311Config->ioTags[DRV8311_AL] = IO_TAG(DRV8311_PINAL);
    drv8311Config->ioTags[DRV8311_BH] = IO_TAG(DRV8311_PINBH);
    drv8311Config->ioTags[DRV8311_BL] = IO_TAG(DRV8311_PINBL);
    drv8311Config->ioTags[DRV8311_CH] = IO_TAG(DRV8311_PINCH);
    drv8311Config->ioTags[DRV8311_CL] = IO_TAG(DRV8311_PINCL);

    drv8311Config->frequency = DRV8311_HZ;
}
