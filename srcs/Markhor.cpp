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

#include <cstring>
#include "Interrupt.h"
#include "Gpio.h"
#include "Console.h"
#include "CLib.h"
#include "Test.h"
#include "SystemTimer.h"
#include "Mmu.h"

extern char buf[512];

#define NUM_PAGE_TABLE_ENTRIES 4096 /* 1 entry per 1MB, so this covers 4G address space */
#define SDRAM_START       0
#define SDRAM_END         400
#define CACHE_DISABLED    0x12
#define CACHE_WRITEBACK   0x1e

static Uint32 page_table[NUM_PAGE_TABLE_ENTRIES] __attribute__((aligned(16384)));

Mmu mmu;

void Markhor(void) {
    Uint32 reg;
    int inx;    
    for (inx = SDRAM_END ; inx < NUM_PAGE_TABLE_ENTRIES ; inx++)
        page_table[inx] = inx << 20 | (3 << 10) | CACHE_DISABLED;
    for (inx = SDRAM_START ; inx < SDRAM_END ; inx++)
        page_table[inx] = inx << 20 | (3 << 10) | CACHE_WRITEBACK;

    mmu.setTtbr0((Address)page_table);
    mmu.setAllDomainToClient();
    mmu.enable();

    gpio.selectFunction(16, Gpio::PinFunction::Output);

    if (console.init() == ResultCode::Success) {
        console.write("Console OK\n");
        gpio.clearOutputPin(16); // turn LED on.
    }

    interrupt.enableInterruptRequest();
    interrupt.enableFastInterruptRequest();
    console.write("Interrupt enabled.\n");

    sprintf(buf, "DACR: %x\n", mmu.getDacr());
    console.write(buf);

    // testPalette();
    // testConsole();
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
    // testItoa();
    // testSprintf();
    // dumpIVT();
    // testArmTimer();
    testSystemTimer();
}

/**
 * NOT complete function. must be rewritten.
 */
// extern char _end;
// extern "C" void * _sbrk(int incr) {
//     static char * heap_end = &_end;
//     char * prev_heap_end;

//     // if (heap_end == 0)
//     //     heap_end = &_end;

//      prev_heap_end = heap_end;
//      heap_end += incr;

//      return (void *)prev_heap_end;
// }
