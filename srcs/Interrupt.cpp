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

#include "IoPeripherals.h"
#include "Interrupt.h"

#define INTERRUPT_CONTROLLER_BASE   ( IO_PERIPHERALS_BASE + 0xB200 )

Interrupt interrupt;

volatile Interrupt::Register * const Interrupt::interruptRegister = (Interrupt::Register *)INTERRUPT_CONTROLLER_BASE;

void Interrupt::enableInterruptRequest(void) {
    asm ("mrs r0, cpsr");
    asm ("bic r0, r0, #0x80");
    asm ("msr cpsr_c, r0");
}

void Interrupt::enableFastInterruptRequest(void) {
    asm ("mrs r0, cpsr");
    asm ("bic r0, r0, #0x40");
    asm ("msr cpsr_c, r0");
}

/**
 * disable has effect only if 'source' is the same as the current value.
 */
void Interrupt::setFastInterrupt(bool enable, Source source) {
    if (enable) {
        interruptRegister[FIQ_CONTROL] &= ~0xff;
        interruptRegister[FIQ_CONTROL] |= (0x80 | static_cast<Uint32>(source)); // sources bits

        // interruptRegister[FIQ_CONTROL] |= 0x80; // enable
    } else {
        if ((interruptRegister[FIQ_CONTROL] & 0x7f) == static_cast<Uint32>(source)) { // if current selected interrupt is 'source'
            interruptRegister[FIQ_CONTROL] &= ~0xff; // disable
        }
    }
}
