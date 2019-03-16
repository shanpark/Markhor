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

#ifndef __GPIO_H
#define __GPIO_H

#include "MarkhorTypes.h"
#include "ResultCode.h"

enum class GpioPinFunction {
    Input    = 0,
    Output   = 1,
    Alt0     = 4,
    Alt1     = 5,
    Alt2     = 6,
    Alt3     = 7,
    Alt4     = 3,
    Alt5     = 2
};

enum class GpioPinValue {
    Err  = -1,
    Lo   = 0,
    Hi   = 1
};

class Gpio {
public:
    ResultCode selectFunction(Uint32 pin, GpioPinFunction func);
    ResultCode setOutputPin(Uint32 pin);
    ResultCode clearOutputPin(Uint32 pin);
    GpioPinValue getPinValue(Uint32 pin);
};

extern Gpio gpio;

#endif /* __GPIO_H */