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
#include "ArmTimer.h"

/* BCM2385 ARM Peripherals document */
#define ARM_TIMER_BASE   (IO_PERIPHERALS_BASE + 0xb400UL)

/* Control register bits */
#define TIMER_23BIT                         0x00000002
#define PRESCALE16_BIT                      0x00000004
#define PRESCALE256_BIT                     0x00000008
#define INTERRUPT_ENABLE_BIT                0x00000020
#define ENABLE_BIT                          0x00000080
#define DEBUG_HALT_BIT                      0x00000100
#define FREE_RUNNING_COUNTER_ENABLE_BIT     0x00000200
#define FREE_RUNNING_COUNTER_PRESCALER_BITS 0x00FF0000

ArmTimer armTimer;

volatile ArmTimer::Register * const ArmTimer::armTimerRegister = (ArmTimer::Register *)ARM_TIMER_BASE;

void ArmTimer::setTimerBits(TimerBits bits) { 
    if (bits == TimerBits::Bit23)
        armTimerRegister[CONTROL] |= TIMER_23BIT;
    else
        armTimerRegister[CONTROL] &= ~TIMER_23BIT;
}

void ArmTimer::setPrescale(Prescale prescale) {
    if (prescale == Prescale::Prescale16)
        armTimerRegister[CONTROL] |= PRESCALE16_BIT;
    else if (prescale == Prescale::Prescale256)
        armTimerRegister[CONTROL] |= PRESCALE256_BIT;
    else
        armTimerRegister[CONTROL] &= ~(PRESCALE16_BIT | PRESCALE256_BIT);
}

void ArmTimer::enableInterrupt(bool enable, bool fast) {
    if (enable) {
        if (fast) {
            interrupt.setFastInterrupt(true, Interrupt::Source::Timer);
            interrupt.disableBasicIrq(IRQ_TIMER);
        } else {
            interrupt.setFastInterrupt(false, Interrupt::Source::Timer);
            interrupt.enableBasicIrq(IRQ_TIMER);
        }
        armTimerRegister[CONTROL] |= INTERRUPT_ENABLE_BIT;
    } else {
        interrupt.setFastInterrupt(false, Interrupt::Source::Timer); // disable if Timer is fast interrupt.
        interrupt.disableBasicIrq(IRQ_TIMER); // disable
        armTimerRegister[CONTROL] &= ~INTERRUPT_ENABLE_BIT;
    }
}

/**
 * enable timer.
 */
void ArmTimer::enable(bool enable) {
    if (enable)
        armTimerRegister[CONTROL] |= ENABLE_BIT; // enable timer.
    else
        armTimerRegister[CONTROL] &= ~ENABLE_BIT;
}

void ArmTimer::haltDebugMode(bool halt) {
    if (halt)
        armTimerRegister[CONTROL] |= DEBUG_HALT_BIT;
    else
        armTimerRegister[CONTROL] &= ~DEBUG_HALT_BIT;
}

void ArmTimer::enableFreeRunningCounter(bool enable) {
    if (enable)
        armTimerRegister[CONTROL] |= FREE_RUNNING_COUNTER_ENABLE_BIT;
    else
        armTimerRegister[CONTROL] &= ~FREE_RUNNING_COUNTER_ENABLE_BIT;
}

void ArmTimer::setFreeRunningCounterScaler(Uint8 scaler) {
    armTimerRegister[CONTROL] &= 0xff00ffff;
    armTimerRegister[CONTROL] |= (((Uint32)scaler) << 16);
}
