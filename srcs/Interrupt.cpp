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

#define IRQ_BASIC_PENDING   0
#define IRQ_PENDIGN_1       1
#define IRQ_PENDIGN_2       2
#define FIQ_CONTROL         3
#define ENABLE_IRQ_1        4
#define ENABLE_IRQ_2        5
#define ENABLE_BASIC_IRQ    6
#define DISABLE_IRQ_1       7
#define DISABLE_IRQ_2       8
#define DISABLE_BASIC_IRQ   9

typedef Uint32 InterruptRegister;

Interrupt interrupt;

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

Uint32 Interrupt::getIrq1() {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    return interrupRegister[IRQ_PENDIGN_1];
}

Uint32 Interrupt::getIrq2() {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    return interrupRegister[IRQ_PENDIGN_2];
}

Uint32 Interrupt::getBasicIrq() {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    return interrupRegister[IRQ_BASIC_PENDING];
}

/**
 * Only '1' bits are enabled and '0' bits have no effect.
 */
void Interrupt::enableIrq1(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[ENABLE_IRQ_1] = bits;
}

/**
 * Only '1' bits are enabled and '0' bits have no effect.
 */
void Interrupt::enableIrq2(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[ENABLE_IRQ_2] = bits;
}

/**
 * Only '1' bits are enabled and '0' bits have no effect.
 */
void Interrupt::enableBasicIrq(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[ENABLE_BASIC_IRQ] = bits;
}

/**
 * Only '1' bits are disabled and '0' bits have no effect.
 */
void Interrupt::disableIrq1(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[DISABLE_IRQ_1] = bits;
}

/**
 * Only '1' bits are disabled and '0' bits have no effect.
 */
void Interrupt::disableIrq2(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[DISABLE_IRQ_2] = bits;
}

/**
 * Only '1' bits are disabled and '0' bits have no effect.
 */
void Interrupt::disableBasicIrq(Uint32 bits) {
    volatile InterruptRegister * interrupRegister = (InterruptRegister *)INTERRUPT_CONTROLLER_BASE;
    interrupRegister[DISABLE_BASIC_IRQ] = bits;
}
