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
#include "Display.h"

int Markhor(void) {
    GpioSelectFunction(16, 1);

    if (setDisplay(1920, 1080, 32) == 0)
        GpioClearOutputPin(16);
    else
        GpioSetOutputPin(16);

    setPixel(0, 0, 0x00ffffff);
    setPixel(1920 - 1, 1080 -1, 0x00ffffff);

    putChar(0, 0, 'A');
    putChar(1, 0, 'B');
    putChar(2, 0, 'C');
    putChar(3, 0, 'D');
    putChar(4, 0, 'E');
    putChar(5, 0, 'F');
    putChar(6, 0, 'G');
    putChar(7, 0, 'H');
    putChar(8, 0, 'I');
    putChar(9, 0, 'J');

    putChar(0, 1, 'a');
    putChar(1, 1, 'b');
    putChar(2, 1, 'c');
    putChar(3, 1, 'd');
    putChar(4, 1, 'e');
    putChar(5, 1, 'f');
    putChar(6, 1, 'g');
    putChar(7, 1, 'h');
    putChar(8, 1, 'i');
    putChar(9, 1, 'j');

    // while (1) {
    //     if (GpioGetPinValue(16) == GPV_HI)
    //         GpioClearOutputPin(16);
    //     else
    //         GpioSetOutputPin(16);

    //     for (int inx = 0 ; inx < 500000 ; inx++)
    //         NOP();
    // }

    return 0;
}