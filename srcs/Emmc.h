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
    ResultCode powerOnSdCard();
    ResultCode powerOffSdCard();
    ResultCode getPowerState(Uint32 * state);

private:
    static volatile Register * const emmcRegister;

    Uint8 vendor;
    Uint8 sdVersion;
    Uint8 slotStatus;
    Uint32 baseClock;
};

extern Emmc emmc;

#endif /* __EMMC_H */