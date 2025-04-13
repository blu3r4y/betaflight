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

#include "drivers/io.h"
#include "drivers/bus_spi.h"
// #include "drivers/io_impl.h"

#include "drivers/drv8311.h"

// clock speed 1MHz (TODO: make this ~ 10MHz later)
#define DRV8311_SPI_CLK_HZ 1000000

extDevice_t drv8311Device;
extDevice_t *motorDev = &drv8311Device;

static IO_t faultIO[DRV8311_DEVICE_COUNT];
static IO_t csIO[DRV8311_DEVICE_COUNT];
static IO_t sleepIO;
static IO_t pwmSyncIO;

static IO_t currentAdcIO[DRV8311_PHASE_COUNT];

drv8311InitStatus_e drvInit(const drv8311Config_t *config)
{
    // TODO: check if pins are free before setting owners, see also
    // if (!IOIsFreeOrPreinit(motorDev->busType_u.spi.csnPin)) ...

    for (int i = 0; i < DRV8311_DEVICE_COUNT; i++)
    {
        // fault pins
        faultIO[i] = IOGetByTag(config->faultTags[i]);
        if (faultIO[i])
        {
            IOInit(faultIO[i], OWNER_DRV8311, 0);
            IOConfigGPIO(faultIO[i], IOCFG_IPU); // input with pull-ups
        }

        // chip-select pins
        csIO[i] = IOGetByTag(config->csTags[i]);
        if (csIO[i])
        {
            IOInit(csIO[i], OWNER_DRV8311, 0);
            IOLo(csIO[i]); // start with not selected
            IOConfigGPIO(csIO[i], IOCFG_OUT_PP); // push-pull output
        }
    }

    // sleep pin
    sleepIO = IOGetByTag(config->sleepTag);
    if (sleepIO)
    {
        IOInit(sleepIO, OWNER_DRV8311, 0);
        IOLo(sleepIO); // start in sleep mode
        IOConfigGPIO(sleepIO, IOCFG_OUT_PP); // push-pull output
    }

    // pwm sync pin
    pwmSyncIO = IOGetByTag(config->pwnSyncTag);
    if (pwmSyncIO)
    {
        // TODO: should this be connected to a timer?
        IOInit(pwmSyncIO, OWNER_DRV8311, 0);
        IOLo(pwmSyncIO); // do not use now
        IOConfigGPIO(pwmSyncIO, IOCFG_OUT_PP); // push-pull output
    }

    for (int i = 0; i < DRV8311_PHASE_COUNT; i++)
    {
        // current measurement pins
        currentAdcIO[i] = IOGetByTag(config->currentAdcTags[i]);
        if (currentAdcIO[i])
        {
            IOInit(currentAdcIO[i], OWNER_DRV8311, 0);
            IOConfigGPIO(currentAdcIO[i], IOCFG_AF_OD); // open-drain input
        }
    }

    // TODO: maybe think about following max7456HardwareReset to awake from sleep mode
    // drv8311HardwareReset();
    
    // initialize SPI bus
    if (!spiSetBusInstance(motorDev, config->spiDevice)) {
        return DRV8311_INIT_NOT_CONFIGURED;
    }

    // TODO: make this work with multiple chip-select or just short them on board
    motorDev->busType_u.spi.csnPin = IOGetByTag(config->csTags[0]);
    if (!IOIsFreeOrPreinit(motorDev->busType_u.spi.csnPin)) {
        return DRV8311_INIT_NOT_CONFIGURED;
    }

    // TODO: debug what frequency this will be later
    // TODO: maybe there is a way to make this some even number??
    spiSetClkDivisor(motorDev, spiCalculateDivider(DRV8311_SPI_CLK_HZ));

    // TODO: maybe we also need this?
    // IOInit(motorDev->busType_u.spi.csnPin, OWNER_OSD_CS, 0);
    // IOConfigGPIO(motorDev->busType_u.spi.csnPin, SPI_IO_CS_CFG);
    // IOHi(motorDev->busType_u.spi.csnPin);

    // TODO: spi write tests, see also
    // max7456.c:376 ... spiWrite(dev, 0xff);
    spiWrite(motorDev, 0xff);

    // TODO: check if this is correct
    return DRV8311_INIT_OK;
}

void drvEnable(void)
{

}

void drvDisable(void)
{

}

void drvWriteRpm(float const rpm[])
{
    UNUSED(rpm);
}

void drvReadDiagnostics(void)
{

}

#endif // USE_DRV8311
