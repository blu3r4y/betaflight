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

#define ESC_GND 0
#define ESC_VDD 1
#define ESC_FLT 2

#define ESC_H_ON false
#define ESC_H_OFF true
#define ESC_L_ON true
#define ESC_L_OFF false

static IO_t escIO[ESC_PIN_COUNT];
static uint8_t escState = 1;

static inline void pinSet(IO_t pin, bool on)
{
    IOWrite(pin, on);
}

/** Pull one MOSFET stage to VDD, GND, or have them floating */
static inline void escPinSet(IO_t pinHigh, IO_t pinLow, int mode)
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
static inline void escSetModes(int modeA, int modeB, int modeC)
{
    escPinSet(escIO[ESC_AH], escIO[ESC_AL], modeA);
    escPinSet(escIO[ESC_BH], escIO[ESC_BL], modeB);
    escPinSet(escIO[ESC_CH], escIO[ESC_CL], modeC);
}

void escInit(const escConfig_t *config)
{
    for (int i = 0; i < ESC_PIN_COUNT; i++)
    {
        escIO[i] = IOGetByTag(config->ioTags[i]);
        if (escIO[i])
        {
            IOInit(escIO[i], OWNER_ESC, 0);
            IOConfigGPIO(escIO[i], IOCFG_OUT_PP);
            pinSet(escIO[i], false);
        }
    }
}

void escLoop(timeUs_t currentTimeUs)
{
    UNUSED(currentTimeUs);

    switch (escState)
    {
    default:
    case 1:
        escSetModes(ESC_GND, ESC_VDD, ESC_FLT);
        escState = 2;
        break;
    case 2:
        escSetModes(ESC_FLT, ESC_VDD, ESC_GND);
        escState = 3;
        break;
    case 3:
        escSetModes(ESC_VDD, ESC_FLT, ESC_GND);
        escState = 4;
        break;
    case 4:
        escSetModes(ESC_VDD, ESC_GND, ESC_FLT);
        escState = 5;
        break;
    case 5:
        escSetModes(ESC_FLT, ESC_GND, ESC_VDD);
        escState = 6;
        break;
    case 6:
        escSetModes(ESC_GND, ESC_FLT, ESC_VDD);
        escState = 1;
        break;
    }
}
