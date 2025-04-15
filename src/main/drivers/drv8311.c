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

 /*******************************************************************************
 * Includes
 */
#include "platform.h"

#ifdef USE_DRV8311

#include <string.h>
#include "drivers/io.h"
#include "drivers/bus_spi.h"
#include "drivers/time.h"
// #include "drivers/io_impl.h"

#include "drivers/drv8311.h"

/*******************************************************************************
 * Macros
*/

/*******************************************************************************
 * Defines
*/

// clock speed 1MHz (TODO: make this ~ 10MHz later)
#define DRV8311_SPI_CLK_HZ 1000000

// SPI defines
#define DRV8311_SPIBUF_LEN_SINGLEREG 4
#define DRV8311_SPI_READ 1
#define DRV8311_SPI_WRITE 0

// TSPI_Defines
#define DRV8311_MAX_CNT_TSPI_DEVICES 4
#define DRV8311_TSPI_BROADCAST 0x0F
#define DRV8311_TSPI_MOTOR1 0x00
#define DRV8311_TSPI_MOTOR2 0x01
#define DRV8311_TSPI_MOTOR3 0x02
#define DRV8311_TSPI_MOTOR4 0x03


// register adress defines
#define DRV8311_REG_ADDR_DEV_STS1      0x00  // Device Status 1 Register (R)
#define DRV8311_REG_ADDR_OT_STS        0x04  // Over Temperature Status Register (R)
#define DRV8311_REG_ADDR_SUP_STS       0x05  // Supply Status Register (R)
#define DRV8311_REG_ADDR_DRV_STS       0x06  // Driver Status Register (R)
#define DRV8311_REG_ADDR_SYS_STS       0x07  // System Status Register (R)
#define DRV8311_REG_ADDR_PWM_SYNC_PRD  0x0C  // PWM Sync Period Register (R)
#define DRV8311_REG_ADDR_FLT_MODE      0x10  // Fault Mode Register (R/W)
#define DRV8311_REG_ADDR_SYSF_CTRL     0x12  // System Fault Control Register (R/W)
#define DRV8311_REG_ADDR_DRVF_CTRL     0x13  // Driver Fault Control Register (R/W)
#define DRV8311_REG_ADDR_FLT_TCTRL     0x16  // Fault Timing Control Register (R/W)
#define DRV8311_REG_ADDR_FLT_CLR       0x17  // Fault Clear Register (W)
#define DRV8311_REG_ADDR_PWMG_PERIOD   0x18  // PWM_GEN Period Register (W)
#define DRV8311_REG_ADDR_PWMG_A_DUTY   0x19  // PWM_GEN A Duty Register (W)
#define DRV8311_REG_ADDR_PWMG_B_DUTY   0x1A  // PWM_GEN B Duty Register (W)
#define DRV8311_REG_ADDR_PWMG_C_DUTY   0x1B  // PWM_GEN C Duty Register (W)
#define DRV8311_REG_ADDR_PWM_STATE     0x1C  // PWM State Register (W)
#define DRV8311_REG_ADDR_PWMG_CTRL     0x1D  // PWM_GEN Control Register (R/W) -> Done
#define DRV8311_REG_ADDR_PWM_CTRL1     0x20  // PWM Control Register 1 (R/W) -> Done
#define DRV8311_REG_ADDR_DRV_CTRL      0x22  // Predriver Control Register (R/W) -> Done
#define DRV8311_REG_ADDR_CSA_CTRL      0x23  // CSA Control Register (R/W)
#define DRV8311_REG_ADDR_SYS_CTRL      0x3F  // System Control Register (R/W) --> Done



// DRV8311_REG_ADDR_FLT_CLR
#define DRV8311_REG_ADDR_FLT_CLR_FLT_CLR 0x0001

// DRV8311_REG_ADDR_PWMG_CTRL
#define DRV8311_REG_ADDR_PWMG_CTRL_PWM_EN 0x0400
#define DRV8311_REG_ADDR_PWMG_CTRL_PWMCNTR_MODE_UP 0x0100

// DRV8311_REG_ADDR_DRV_CTRL
#define DRV8311_REG_ADDR_DRV_CTRL_DLYCMP_EN 0x0080
#define DRV8311_REG_ADDR_DRV_CTRL_TDEAD_CTRL_600ns 0x0030

// DRV8311_REG_ADDR_SYS_CTRL
#define DRV_8311_SYS_CTRL_REG_LOCK 0x0080
#define DRV_8311_SYS_CTRL_SPI_PEN 0x0040



/*******************************************************************************
 * Definitions
*/

/*******************************************************************************
 * Local Variables
*/

extDevice_t drv8311Device;
extDevice_t *motorDev = &drv8311Device;

static IO_t faultIO[DRV8311_DEVICE_COUNT];
static IO_t csIO[DRV8311_DEVICE_COUNT];
static IO_t sleepIO;
static IO_t pwmSyncIO;

static IO_t currentAdcIO[DRV8311_PHASE_COUNT];

/*******************************************************************************
 * Local Functions - Prototypes
*/

static void InitPins(const drv8311Config_t *config);
static drv8311RetStatus_e FillSPIBufferSingleRegAccess(uint8_t * const pBuffer, bool const readNotWrite,
                                                       uint8_t const deviceIdx, uint8_t const regAddr, 
                                                       uint16_t const data);
static bool CalculateEvenParity15FromArray(const uint8_t *pData);

/*******************************************************************************
 * API Variables
*/

/*******************************************************************************
 * API Functions
 */

drv8311InitStatus_e drvInit(const drv8311Config_t *config)
{
    // Initialize STM Hardware peripherals for DRV Pins
    InitPins(config);
    
    // initialize SPI bus
    if (!spiSetBusInstance(motorDev, config->spiDevice)) {
        return DRV8311_INIT_NOT_CONFIGURED;
    }

    // TODO: make this work with multiple chip-select or just short them on board
    motorDev->busType_u.spi.csnPin = IOGetByTag(config->csTags[0]);

    // TODO: debug what frequency this will be later
    // TODO: maybe there is a way to make this some even number??
    spiSetClkDivisor(motorDev, spiCalculateDivider(DRV8311_SPI_CLK_HZ));


    uint8_t spiBuf[4] = { 0 };

    // Initialize all motor drivers
    drvDisable();
    delay(1);

    // Enable parity and lock control reg
    FillSPIBufferSingleRegAccess(spiBuf, DRV8311_SPI_WRITE, DRV8311_TSPI_BROADCAST, 
                                 DRV8311_REG_ADDR_SYS_CTRL, 
                                 (DRV_8311_SYS_CTRL_SPI_PEN | DRV_8311_SYS_CTRL_REG_LOCK));
    spiWrite32Bit(motorDev, spiBuf);

    // Clear all faults
    FillSPIBufferSingleRegAccess(spiBuf, DRV8311_SPI_WRITE, DRV8311_TSPI_BROADCAST, 
        DRV8311_REG_ADDR_FLT_CLR, (DRV8311_REG_ADDR_FLT_CLR_FLT_CLR));
    spiWrite32Bit(motorDev, spiBuf);
    
    // Enable internal PWM Generarion (UP Counter, No Synchronization)
    // TODO: Maybe we need synchronization here
    FillSPIBufferSingleRegAccess(spiBuf, DRV8311_SPI_WRITE, DRV8311_TSPI_BROADCAST, 
        DRV8311_REG_ADDR_PWMG_CTRL, 
        (DRV8311_REG_ADDR_PWMG_CTRL_PWM_EN |DRV8311_REG_ADDR_PWMG_CTRL_PWMCNTR_MODE_UP));
    spiWrite32Bit(motorDev, spiBuf);
    
    // Configure Deadtime and Slewrate
    //TODO: Maybe change these parameters (maybe delay compensation or higher slew rate needed?)
    FillSPIBufferSingleRegAccess(spiBuf, DRV8311_SPI_WRITE, DRV8311_TSPI_BROADCAST, 
        DRV8311_REG_ADDR_DRV_CTRL, 
        (DRV8311_REG_ADDR_DRV_CTRL_TDEAD_CTRL_600ns));
    spiWrite32Bit(motorDev, spiBuf);

    drvEnable();

    return DRV8311_INIT_OK;
}

void drvEnable(void)
{
    IOHi(sleepIO);
}

void drvDisable(void)
{
    IOLo(sleepIO);
}

void drvWriteRpm(float const rpm[])
{
    UNUSED(rpm);
}

void drvReadDiagnostics(void)
{

}

/*******************************************************************************
 * Local Functions - Implementation
*/

/**
 * @brief Initializes STMs GPIO pins for the DRV8311 driver.
 *
 * @param[in] config Pointer to pin configuration.
 */
static void InitPins(const drv8311Config_t *config)
{
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
}

/**
 * @brief Fills a 4 byte SPI buffer with a command frame.
 *
 * Prepares a SPI frame for reading or writing one register of the DRV8311 including parity bits.
 *
 * @param[out] pBuffer       Pointer to the buffer to fill 
 *                           (must be a pointer to a free memory of at least 4 byte).
 * @param[in]  readNotWrite  True for read, false for write
 * @param[in]  deviceIdx     Index of the target device (0 - 3 possible).
 * @param[in]  regAddr       Register address to access.
 * @param[in]  data          Data to write.
 */
static drv8311RetStatus_e FillSPIBufferSingleRegAccess(uint8_t * const pBuffer, bool const readNotWrite,
    uint8_t const deviceIdx, uint8_t const regAddr, uint16_t const data)
{
    if(pBuffer == NULL || (deviceIdx >= DRV8311_MAX_CNT_TSPI_DEVICES && deviceIdx != DRV8311_TSPI_BROADCAST))
    {
        return DRV8311_INPUT_PARAM_CHECK_FAILED;
    }

    // reset the buffer for a clean start condition
    memset(pBuffer, 0, DRV8311_SPIBUF_LEN_SINGLEREG);

    // fill header
    pBuffer[0] |= (readNotWrite & 0x01 << 7); // Read/write bit
    pBuffer[0] |= ((deviceIdx & 0x0F) << 3); // 2 Bit device index
    pBuffer[0] |= ((regAddr & 0xE0) >> 5); // 3 MSBs of Address

    pBuffer[1] |= ((regAddr & 0x1F) << 3); //5 LSBs of Address

    bool parity_header = CalculateEvenParity15FromArray(&(pBuffer[0]));
    pBuffer[1] |= ((parity_header & 0x01) << 0); // Parity bit for header

    // fill data if this is a write command
    if(!readNotWrite)
    {
        pBuffer[2] |= ((data & 0xEF00) >> 8); // High Data bits
        pBuffer[3] |= ((data & 0x00FF) << 0); // Low Databits
        
        bool parity_data = CalculateEvenParity15FromArray(&(pBuffer[2]));
        pBuffer[2] |= ((parity_data & 0x01) << 7); // Parity bit for data
    }
    return DRV8311_OK;
}

/**
 * @brief Calculates the even parity bit for a 16-bit value stored in a uint8_t[2] array.
 *
 * Computes parity over the lower 15 bits (MSB is reserved for the parity bit).
 * Returns true if parity bit should be 1 (to make the total number of 1s even).
 *
 * @param pData Pointer to a 2-byte array, where pData[0] is MSB and pData[1] is LSB.
 * @return true  Parity bit should be high (1).
 * @return false Parity bit should be low (0).
 */
static bool CalculateEvenParity15FromArray(const uint8_t *pData)
{
    // Combine bytes into 16-bit value (big-endian: pData[0] = MSB, pData[1] = LSB)
    uint16_t value = ((uint16_t)pData[0] << 8) | pData[1];

    bool parity = false;
    for (uint8_t i = 0; i < 15; ++i)
    {
        parity ^= (value >> i) & 0x01;
    }
    return parity;
}


#endif // USE_DRV8311
