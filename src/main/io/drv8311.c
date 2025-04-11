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
#include "pg/drv8311.h"
#include "drivers/io_impl.h"

#include "common/utils.h"

#include "drv8311.h"

#define ESC_GND 0
#define ESC_VDD 1
#define ESC_FLT 2

#define ESC_H_ON false
#define ESC_H_OFF true
#define ESC_L_ON true
#define ESC_L_OFF false

static IO_t drvIO[DRV8311_PIN_COUNT];
static uint8_t drvState = 1;

static inline void pinSet(IO_t pin, bool on)
{
    IOWrite(pin, on);
}

/** Pull one MOSFET stage to VDD, GND, or have them floating */
static inline void drvPinSet(IO_t pinHigh, IO_t pinLow, int mode)
{
    switch (mode)
    {
    case ESC_GND:
        pinSet(pinHigh, ESC_H_OFF);
        pinSet(pinLow, ESC_L_ON);
        break;
    case ESC_VDD:
        pinSet(pinHigh, ESC_H_ON);
        pinSet(pinLow, ESC_L_OFF);
        break;
    case ESC_FLT:
        pinSet(pinHigh, ESC_H_OFF);
        pinSet(pinLow, ESC_L_OFF);
    }
}

/** Set the mode (VDD, GND, Z) of all three MOSFET stages at once */
static inline void drvSetModes(int modeA, int modeB, int modeC)
{
    drvPinSet(drvIO[DRV8311_AH], drvIO[DRV8311_AL], modeA);
    drvPinSet(drvIO[DRV8311_BH], drvIO[DRV8311_BL], modeB);
    drvPinSet(drvIO[DRV8311_CH], drvIO[DRV8311_CL], modeC);
}

void drv8311Init(const drv8311Config_t *config)
{
    for (int i = 0; i < DRV8311_PIN_COUNT; i++)
    {
        drvIO[i] = IOGetByTag(config->ioTags[i]);
        if (drvIO[i])
        {
            IOInit(drvIO[i], OWNER_DRV8311, 0);
            IOConfigGPIO(drvIO[i], IOCFG_OUT_PP);
            pinSet(drvIO[i], false);
        }
    }
}

void drv8311Loop(timeUs_t currentTimeUs)
{
    UNUSED(currentTimeUs);

    switch (drvState)
    {
    default:
    case 1:
        drvSetModes(ESC_GND, ESC_VDD, ESC_FLT);
        drvState = 2;
        break;
    case 2:
        drvSetModes(ESC_FLT, ESC_VDD, ESC_GND);
        drvState = 3;
        break;
    case 3:
        drvSetModes(ESC_VDD, ESC_FLT, ESC_GND);
        drvState = 4;
        break;
    case 4:
        drvSetModes(ESC_VDD, ESC_GND, ESC_FLT);
        drvState = 5;
        break;
    case 5:
        drvSetModes(ESC_FLT, ESC_GND, ESC_VDD);
        drvState = 6;
        break;
    case 6:
        drvSetModes(ESC_GND, ESC_FLT, ESC_VDD);
        drvState = 1;
        break;
    }
}
