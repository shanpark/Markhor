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
#include "Console.h"
#include "Test.h"

Console console;
Gpio gpio;

void Markhor(void) {
    gpio.selectFunction(16, GpioPinFunction::Output);

    if (console.init() == 0)
        gpio.clearOutputPin(16);

    for (int inx = 0 ; inx < 67 ; inx++) {
        for (int jnx = 0 ; jnx < ((inx + 1) % 12) ; jnx++)
            console.write((char *)"Hello\tWorld\t", 12);

        console.write((char *)"New\n", 4);

        for (int d = 0 ; d < 1000000 ; d++)
            asm ( "NOP" );
    }

    // testPalette();
    // printFrameBufferInfo();
    // printHardwareInfo();
    // printHwMemoryInfo();
    // testClock();
    // printCommandLine();
    // printDmaState();
    // testDevicePower();
    // printVoltageInfo();
    // printTemperature();
    // testDisplayInfo();
}

/**
 * NOT complete function. must be rewritten.
 */
extern char _end;
extern "C" void * _sbrk(int incr) {
    static char * heap_end = &_end;
    char * prev_heap_end;

    // if (heap_end == 0)
    //     heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (void *)prev_heap_end;
}

extern "C" void abort(void) {
}