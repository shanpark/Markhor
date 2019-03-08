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

#include "Gpio.h"

#define NOP() asm("nop")

int Markhor(void) {

    GpioSelectFunction(16, 1);

    while (1) {
        if (GpioGetPinValue(16) == GPV_HI)
            GpioClearOutputPin(16);
        else
            GpioSetOutputPin(16);

        for (int inx = 0 ; inx < 500000 ; inx++)
            NOP();
    }

    return 0;
}