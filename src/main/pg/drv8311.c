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

#include "platform.h"

#ifdef USE_DRV8311

#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "drivers/io.h"
#include "drivers/bus_spi.h"

#include "common/utils.h"

#include "drv8311.h"

PG_REGISTER_WITH_RESET_FN(drv8311Config_t, drv8311Config, PG_DRV8311_CONFIG, 0);

#ifndef DRV8311_SPI_INSTANCE
#define DRV8311_SPI_INSTANCE NULL
#endif

#ifndef DRV8311_FAULT_PIN0
#define DRV8311_FAULT_PIN0 PC7
#endif

#ifndef DRV8311_FAULT_PIN1
#define DRV8311_FAULT_PIN1 PC9
#endif

#ifndef DRV8311_FAULT_PIN2
#define DRV8311_FAULT_PIN2 PE6
#endif

#ifndef DRV8311_FAULT_PIN3
#define DRV8311_FAULT_PIN3 PE5
#endif

#ifndef DRV8311_CS_PIN0
#define DRV8311_CS_PIN0 PC6
#endif

#ifndef DRV8311_CS_PIN1
#define DRV8311_CS_PIN1 PC8
#endif

#ifndef DRV8311_CS_PIN2
#define DRV8311_CS_PIN2 PA8
#endif

#ifndef DRV8311_CS_PIN3
#define DRV8311_CS_PIN3 PE4
#endif

#ifndef DRV8311_SLEEP_PIN
#define DRV8311_SLEEP_PIN PD10
#endif

#ifndef DRV8311_PWM_SYNC_PIN
#define DRV8311_PWM_SYNC_PIN PD11
#endif

#ifndef DRV8311_CURRENT_ADC_PIN0
#define DRV8311_CURRENT_ADC_PIN0 PA2
#endif

#ifndef DRV8311_CURRENT_ADC_PIN1
#define DRV8311_CURRENT_ADC_PIN1 PA3
#endif

#ifndef DRV8311_CURRENT_ADC_PIN2
#define DRV8311_CURRENT_ADC_PIN2 PB3
#endif

void pgResetFn_drv8311Config(drv8311Config_t *drv8311Config)
{
    drv8311Config->spiDevice = SPI_DEV_TO_CFG(spiDeviceByInstance(DRV8311_SPI_INSTANCE));

    drv8311Config->faultTags[0] = IO_TAG(DRV8311_FAULT_PIN0);
    drv8311Config->faultTags[1] = IO_TAG(DRV8311_FAULT_PIN1);
    drv8311Config->faultTags[2] = IO_TAG(DRV8311_FAULT_PIN2);
    drv8311Config->faultTags[3] = IO_TAG(DRV8311_FAULT_PIN3);

    drv8311Config->csTags[0] = IO_TAG(DRV8311_CS_PIN0);
    drv8311Config->csTags[1] = IO_TAG(DRV8311_CS_PIN1);
    drv8311Config->csTags[2] = IO_TAG(DRV8311_CS_PIN2);
    drv8311Config->csTags[3] = IO_TAG(DRV8311_CS_PIN3);

    drv8311Config->sleepTag = IO_TAG(DRV8311_SLEEP_PIN);
    drv8311Config->pwnSyncTag = IO_TAG(DRV8311_PWM_SYNC_PIN);

    drv8311Config->currentAdcTags[0] = IO_TAG(DRV8311_CURRENT_ADC_PIN0);
    drv8311Config->currentAdcTags[1] = IO_TAG(DRV8311_CURRENT_ADC_PIN1);
    drv8311Config->currentAdcTags[2] = IO_TAG(DRV8311_CURRENT_ADC_PIN2);
}
#endif // USE_DRV8311
