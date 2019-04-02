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

#ifndef __EMMC_H
#define __EMMC_H

#include "MarkhorTypes.h"
#include "ResultCode.h"

class Emmc {
    typedef Uint32 Register;

public:
    ResultCode init();

private:
    Uint32 getBaseClock();
    void issueCommand(Uint32 command, Uint32 argument, Uint32 timeout);
    void issueCommandInt(Uint32 cmd_reg, Uint32 argument, Uint32 timeout);
    void handleInterrupts();
    void handleCardInterrupt();
    int resetDat();
    int resetCmd();
    int switchClockRate(Uint32 base_clock, Uint32 target_rate);
    Uint32 getClockDivider(Uint32 base_clock, Uint32 target_rate);


    ResultCode powerOnSdCard();
    ResultCode powerOffSdCard();
    ResultCode getPowerState(Uint32 * state);

private:
    static volatile Register * const emmcRegister;
    static Uint8 internalBuf[1024];

	Uint32 card_supports_sdhc;
	Uint32 card_supports_18v;
	Uint32 card_ocr;
    Uint32 card_rca;
    Uint32 last_cmd_reg;
    Uint32 last_cmd;
    Uint32 last_cmd_success;
    Uint32 last_r0;
	Uint32 last_r1;
	Uint32 last_r2;
	Uint32 last_r3;

	// int failed_voltage_switch;

	Uint32 last_interrupt;
	Uint32 last_error;
    int use_sdma;
	int card_removal;
	int blocks_to_transfer;
	Uint32 block_size;

    Uint8 vendor;
    Uint8 sdVersion;
    Uint8 slotStatus;
    Uint32 baseClock;
};

extern Emmc emmc;

#endif /* __EMMC_H */