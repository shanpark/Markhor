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

void __attribute__((interrupt("ABORT"))) Interrupt::reset(void) {
    while (true)
        ;
}

void __attribute__((interrupt("UNDEF"))) Interrupt::undefinedInstruction(void) {
    while (true)
        ;
}

void __attribute__((interrupt("SWI"))) Interrupt::software(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) Interrupt::prefetchAbort(void) {
    while (true)
        ;
}

void __attribute__((interrupt("ABORT"))) Interrupt::dataAbort(void) {
    while (true)
        ;
}

void __attribute__((interrupt("IRQ"))) Interrupt::interruptRequest(void) {
    while (true)
        ;
}

void __attribute__((interrupt("FIQ"))) Interrupt::fastInterruptRequest(void) {
    while (true)
        ;
}
