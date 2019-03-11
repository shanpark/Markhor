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
#include "Gpio.h"
#include "MailboxProperty.h"
#include "Display.h"

#define WIDTH   1680
#define HEIGHT  1050
#define DEPTH   32
#define COLOR   0x00ffffff // 0xaarrggbb

int Markhor(void) {
    GpioSelectFunction(16, 1);

    if (setDisplay(WIDTH, HEIGHT, DEPTH) == 0)
        GpioClearOutputPin(16);
    else
        GpioSetOutputPin(16);

    setPixel(0, 0, COLOR);
    setPixel(1, 1, COLOR);
    setPixel(WIDTH - 2, HEIGHT - 2, COLOR);
    setPixel(WIDTH - 1, HEIGHT - 1, COLOR);

    // char buf[256];

    // sprintf(buf, "Firmware Revision: %08X", getFirmwareRevision());
    // putString(0, 0, buf, COLOR);

    // sprintf(buf, "Board Model: %08X", getBoardModel());
    // putString(0, 1, buf, COLOR);

    // sprintf(buf, "Board Revision: %08X", getBoardRevision());
    // putString(0, 2, buf, COLOR);

    // Uint8 mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    // if (getBoardMacAddress(mac) == 0)
    //     sprintf(buf, "Board MAC Address: %02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // else
    //     sprintf(buf, "Board MAC Address: %08X", -1);
    // putString(0, 3, buf, COLOR);

    // sprintf(buf, "Board Serial: %ld", getBoardSerial());
    // putString(0, 4, buf, COLOR);

    return 0;
}

/**
 * NOT complete function. must be rewritten.
 */
extern char _end;
void * _sbrk(int incr) {
    static char* heap_end = &_end;
    char* prev_heap_end;

    // if (heap_end == 0)
    //     heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (void *)prev_heap_end;
}
