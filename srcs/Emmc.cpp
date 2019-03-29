/**
  Markhor
  Copyright (C) 2019  Sunghan Park

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* TODO just for debugging */
#include "Console.h"
#include "CLib.h"
extern char buf[512];
/* End: just for debugging */

#include "IoPeripherals.h"
#include "MailboxProperty.h"
#include "SystemTimer.h"
#include "Emmc.h"

/* BCM2385 ARM Peripherals document */
#define EMMC_BASE   (IO_PERIPHERALS_BASE + 0x300000UL)

/* EMMC registers offsets */
#define ARG2            0
#define BLKSIZECNT      1
#define ARG1            2
#define CMDTM           3
#define RESP0           4
#define RESP1           5
#define RESP2           6
#define RESP3           7
#define DATA            8
#define STATUS          9
#define CONTROL0        10
#define CONTROL1        11
#define INTERRUPT       12
#define IRPT_MASK       13
#define IRPT_EN         14
#define CONTROL2        15
#define FORCE_IRPT      20
#define BOOT_TIMEOUT    28
#define DBG_SEL         29
#define EXRDFIFO_CFG    32
#define EXRDFIFO_EN     33
#define TUNE_STEP       34
#define TUNE_STEPS_STD  35
#define TUNE_STEPS_DDR  36
#define SPI_INT_SPT     60
#define SLOTISR_VER     63

Emmc emmc;

volatile Emmc::Register * const Emmc::emmcRegister = (Emmc::Register *)EMMC_BASE;

ResultCode Emmc::init() {
    // Sdcard Power Off, On operation needed?
    if (powerOffSdCard() != ResultCode::Success) {
        console.write("SD Card Power off failed.\n");
        return ResultCode::Fail;
    }

    Uint32 state;
    state = 0;
    getPowerState(&state);
    if (state | static_cast<Uint32>(PowerState::PSOnBit)) {
        sprintf(buf, "SD Card Power is ON. [%u]\n", state);
        console.write(buf);
    } else {
        sprintf(buf, "SD Card Power is OFF. [%u]\n", state);
        console.write(buf);
    }

    if (powerOnSdCard() != ResultCode::Success) {
        console.write("SD Card Power on failed.\n");
        return ResultCode::Fail;
    }

    state = 0;
    getPowerState(&state);
    if (state | static_cast<Uint32>(PowerState::PSOnBit)) {
        sprintf(buf, "SD Card Power is ON. [%u]\n", state);
        console.write(buf);
    } else {
        sprintf(buf, "SD Card Power is OFF. [%u]\n", state);
        console.write(buf);
    }

    // Read SD Card Info.
    Register ver = emmcRegister[SLOTISR_VER];
    vendor = static_cast<Uint8>(ver >> 24);
    sdVersion = static_cast<Uint8>((ver >> 16) & 0xff);
    slotStatus = static_cast<Uint8>(ver & 0xff);
    sprintf(buf, "Vender:%u, SD Version:%u, Slot Status:%u\n", vendor, sdVersion, slotStatus);
    console.write(buf);

    // reset host controller. disable clocks
    // 값을 찍어보니 읽기 전이나 후나 항상 0으로 찍힌다. 즉, 실행을 하던 안하던 변화가 없다는 것이다.
    Register control1 = emmcRegister[CONTROL1];
	control1 |= (1 << 24); // reset the complete host circuit
	control1 &= ~(1 << 2); // disable SD clock
	control1 &= ~(1 << 0); // disable internal EMMC clocks for power saving
    emmcRegister[CONTROL1], control1;

    systemTimer.waitUntil([]() { return ((emmcRegister[CONTROL1] & (0x7 << 24)) == 0); }, 1000000);
	if((emmcRegister[CONTROL1] & (0x7 << 24)) != 0)	{
		sprintf(buf, "EMMC: controller did not reset properly\n");
        console.write(buf);
		return ResultCode::Fail;
	}

    // check valid card
    // 문서에는 16번 비트는 Reserved이다. 테스트 결과 카드가 있으나 없으나 항상 16번 비트는 1이었다.
    Register status = emmcRegister[STATUS];
    systemTimer.waitUntil([]() { return ((emmcRegister[STATUS] & (1 << 16)) != 0); }, 500000);
	if ((emmcRegister[STATUS] & (1 << 16)) == 0) {
		sprintf(buf, "EMMC: no card inserted\n");
        console.write(buf);
		return ResultCode::Fail;
	}

    // clear CONTROL2
    emmcRegister[CONTROL2] = 0;

    // Get the base clock rate
	baseClock = getBaseClock();
	if (baseClock == 0) {
	    sprintf(buf, "EMMC: assuming clock rate to be 100MHz\n");
        console.write(buf);
	    baseClock = 100000000;
	}

    // config clock. CONTROL1 register.
    // TODO : 적절한 clock 설정을 찾아야 한다. SD 카드에 따라서 값이 달라지는 것 같다. SD 카드의 속도가 빠르면 더 빠르게 설정할 수 있는 듯...
    //        지금은 400Khz를 타겟으로 설정해서 값을 하드코딩한 것이다.
    control1 = emmcRegister[CONTROL1];
    control1 |= 0x01; // enable clock.    
    control1 |= 0x4000; // 0x40 << 8. set clock divisor for sd clock. 0x40 = 258Khz (for 400Khz). refer to PartA2-300.pdf page 46.
	control1 |= 0x00070000;	// 0x07 << 16. data timeout interval. data timeout = TMCLK * 2^(7+13)
    emmcRegister[CONTROL1] = control1;

    // wait until SD Clock is stable.
    systemTimer.waitUntil([]() { return (emmcRegister[CONTROL1] & (0x01 << 1)) != 0; }, 1000000);

    return ResultCode::Success;
}

Uint32 Emmc::getBaseClock() {
    Uint32 clockRate = 0;
    if (mailboxProperty.getClockRate(ClockId::CIEmmc, &clockRate) != ResultCode::Success) {
        return 0;
    }

    return clockRate;
}

ResultCode Emmc::powerOnSdCard() {
    Uint32 state = static_cast<Uint32>(PowerState::PSOnBit) | static_cast<Uint32>(PowerState::PSWaitBit);
    return mailboxProperty.setPowerState(DeviceId::DISdCard, &state);
}

ResultCode Emmc::powerOffSdCard() {
    Uint32 state = static_cast<Uint32>(PowerState::PSWaitBit);
    return mailboxProperty.setPowerState(DeviceId::DISdCard, &state);
}

ResultCode Emmc::getPowerState(Uint32 * state) {
    return mailboxProperty.getPowerState(DeviceId::DISdCard, state);
}
