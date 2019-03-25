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

#include "ArmTimer.h"
#include "SystemTimer.h"
#include "CLib.h"
#include "TranslationTable.h"
#include "Console.h"
#include "Gpio.h"
#include "Interrupt.h"
#include "InterruptHandlers.h"

static char buf[256];

void __attribute__((interrupt("ABORT"))) resetHandler(void) {
    sprintf(buf, "Reset !\n");
    console.write(buf);
}

void __attribute__((interrupt("UNDEF"))) undefinedInstructionHandler(void) {
    sprintf(buf, "Undefined Instruction !\n");
    console.write(buf);
}

void __attribute__((interrupt("SWI"))) softwareInterruptHandler(void) {
    sprintf(buf, "Software Interrupt !\n");
    console.write(buf);
}

void __attribute__((interrupt("ABORT"))) prefetchAbortHandler(void) {
    sprintf(buf, "Prefetch Abort !\n");
    console.write(buf);
}

extern Uint32 page_table[];

void __attribute__((interrupt("ABORT"))) dataAbortHandler(void) {
    // Return address is 'LR - #8' in Data Abort Exception. 
    // C Stub subtract 4 from LR. So we should subtract 4 more.
    // LR is stored in [sp, #44].
    asm volatile ("ldr r0, [sp, #44] \n"
                  "sub r0, r0, #4 \n"
                  "str r0, [sp, #44]");

    Uint32 dfsr = -1, dfar = -1, contextId = -1;

    asm volatile ("mrc p15, 0, %0, c5, c0, 0" : "=r" (dfsr));
    asm volatile ("mrc p15, 0, %0, c6, c0, 0" : "=r" (dfar));
    asm volatile ("mrc p15, 0, %0, c13, c0, 1" : "=r" (contextId));

    sprintf(buf, "Data Abort[%c]. DFSR:%x, DFAR:%x, PID:%x, ASID:%x\n", dfsr & 0x800 ? 'W' : 'R', dfsr, dfar, contextId >> 8, contextId & 0xff);
    console.write(buf);

    TranslationTable tt(page_table);
    tt.map(0x00100000, 0x00100000, AccessPermission::Unprivileged, MemoryType::Normal, false);

    gpio.setOutputPin(16);
}

void __attribute__((interrupt("IRQ"))) interruptRequestHandler(void) {
    static int on = 0;

    if (interrupt.getBasicIrqPending() & IRQ_TIMER) {
        sprintf(buf, "Basic IRQ:%x\n", interrupt.getBasicIrqPending());
        console.write(buf);

        /* Flip the LED */
        if (on) {
            gpio.setOutputPin(16);
            on = 0;
        } else {
            gpio.clearOutputPin(16);
            on = 1;
        }

        armTimer.clearIrq();
    } 

    if (interrupt.getBasicIrqPending() & IRQ_IRQ1_REGISTER_PENDING) {
        if (interrupt.getIrqPending1() & GPU0_IRQ3) {
            Uint32 counter = systemTimer.getCounterLo();
            sprintf(buf, "System Timer ch3:%d (%x)\n", counter, interrupt.getIrqPending1());
            console.write(buf);

            systemTimer.setChannel3Match(counter + 1000000);
            systemTimer.clearChannel3Irq();
        }
    } 

    if (interrupt.getBasicIrqPending() & IRQ_IRQ2_REGISTER_PENDING) {
    }
}

void __attribute__((interrupt("FIQ"))) fastInterruptRequestHandler(void) {
    static int on = 0;

    // Arm Timer test code
    // sprintf(buf, "FIQ ArmTimer Handler:%u \n", armTimer.getFreeRunningCounter());
    // console.write(buf);

    // System Timer test code
    Uint32 counter = systemTimer.getCounterLo();
    sprintf(buf, "FIQ System Timer ch3:%u (%x)\n", counter, interrupt.getIrqPending1());
    console.write(buf);
    systemTimer.setChannel3Match(counter + 1000000);
    systemTimer.clearChannel3Irq();

    /* Flip the LED */
    if (on) {
        gpio.setOutputPin(16);
        on = 0;
    } else {
        gpio.clearOutputPin(16);
        on = 1;
    }

    armTimer.clearIrq(); // has effect in fiq.
}
