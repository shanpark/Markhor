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

#include "Interrupt.h"

void enableInterruptRequest(void) {
    asm ("mrs r0, cpsr");
    asm ("bic r0, r0, #0x80");
    asm ("msr cpsr_c, r0");
}

void enableFastInterruptRequest(void) {
    asm ("mrs r0, cpsr");
    asm ("bic r0, r0, #0x40");
    asm ("msr cpsr_c, r0");
}

void __attribute__((interrupt("ABORT"))) reset(void) {
    while (true)
        ;
}

void __attribute__((interrupt("UNDEF"))) undefinedInstruction(void) {
    while (true)
        ;
}

void __attribute__((interrupt("SWI"))) software(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) prefetchAbort(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) dataAbort(void) {
    while (true)
        ;
}

void __attribute__((interrupt("IRQ"))) interruptRequest(void) {
    while (true)
        ;
}

void __attribute__((interrupt("FIQ"))) fastInterruptRequest(void) {
    while (true)
        ;
}
