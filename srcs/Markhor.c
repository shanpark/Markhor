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

#include <stdio.h>
#include "Console.h"

void Markhor(void) {
    consoleInit();

    for (int inx = 0 ; inx < 67 ; inx++) {

        for (int jnx = 0 ; jnx < ((inx + 1) % 12) ; jnx++)
            consoleWrite("Hello\tWorld\t", 12);

        consoleWrite("New\n", 4);

        for (int d = 0 ; d < 1000000 ; d++)
            asm ( "NOP" );
    }

    // setupDisplay();
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
void * _sbrk(int incr) {
    static char * heap_end = &_end;
    char * prev_heap_end;

    // if (heap_end == 0)
    //     heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (void *)prev_heap_end;
}
