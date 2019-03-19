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
#include "CLib.h"
#include "Console.h"
#include "Gpio.h"
#include "Interrupt.h"
#include "InterruptHandlers.h"

void __attribute__((interrupt("ABORT"))) resetHandler(void) {
    while (true)
        ;
}

void __attribute__((interrupt("UNDEF"))) undefinedInstructionHandler(void) {
    while (true)
        ;
}

void __attribute__((interrupt("SWI"))) softwareInterruptHandler(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) prefetchAbortHandler(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) dataAbortHandler(void) {
    while (true)
        ;
}

void __attribute__((interrupt("IRQ"))) interruptRequestHandler(void) {
    static int on = 0;
    char buf[32];

    if (interrupt.getBasicIrq() & IRQ_TIMER) {
        sprintf(buf, "Basic IRQ:%x\n", interrupt.getBasicIrq());
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
}

void __attribute__((interrupt("FIQ"))) fastInterruptRequestHandler(void) {
    while (true)
        ;
}
