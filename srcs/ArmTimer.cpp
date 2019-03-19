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

/* BCM2385 ARM Peripherals */
#define ARM_TIMER_BASE   (IO_PERIPHERALS_BASE + 0xb400UL)

/* Arm Timer register offsets */
#define LOAD                    0
#define VALUE                   1
#define CONTROL                 2
#define IRQ_CLEAR               3
#define RAW_IRQ                 4
#define MASKED_IRQ              5
#define RELOAD                  6
#define PRE_DIVIDER             7
#define FREE_RUNNING_COUNTER    8

/* Control register bits */
#define TIMER_23BIT                         0x00000002
#define PRESCALE16_BIT                      0x00000004
#define PRESCALE256_BIT                     0x00000008
#define INTERRUPT_ENABLE_BIT                0x00000020
#define ENABLE_BIT                          0x00000080
#define DEBUG_HALT_BIT                      0x00000100
#define FREE_RUNNING_COUNTER_ENABLE_BIT     0x00000200
#define FREE_RUNNING_COUNTER_PRESCALER_BITS 0x00FF0000

typedef Uint32 ArmTimerRegister;

ArmTimer armTimer;

void ArmTimer::setLoad(Uint32 load) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    armTimerRegister[LOAD] = load;
}

Uint32 ArmTimer::getValue() {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    return armTimerRegister[VALUE];
}

void ArmTimer::setTimerBits(TimerBits bits) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (bits == TimerBits::Bit23)
        armTimerRegister[CONTROL] |= TIMER_23BIT;
    else
        armTimerRegister[CONTROL] &= ~TIMER_23BIT;
}

void ArmTimer::setPrescale(Prescale prescale) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (prescale == Prescale::Prescale16)
        armTimerRegister[CONTROL] |= PRESCALE16_BIT;
    else if (prescale == Prescale::Prescale256)
        armTimerRegister[CONTROL] |= PRESCALE256_BIT;
    else
        armTimerRegister[CONTROL] &= ~(PRESCALE16_BIT | PRESCALE256_BIT);
}

void ArmTimer::enableInterrupt(bool enable) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (enable)
        armTimerRegister[CONTROL] |= INTERRUPT_ENABLE_BIT;
    else
        armTimerRegister[CONTROL] &= ~INTERRUPT_ENABLE_BIT;
}

void ArmTimer::enable(bool enable) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (enable) {
        interrupt.enableBasicIrq(IRQ_TIMER); // enable timer irq of the interrupt controller.
        armTimerRegister[CONTROL] |= ENABLE_BIT; // enable timer.
    } else {
        interrupt.disableBasicIrq(IRQ_TIMER); // disable
        armTimerRegister[CONTROL] &= ~ENABLE_BIT;
    }
}

void ArmTimer::haltDebugMode(bool halt) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (halt)
        armTimerRegister[CONTROL] |= DEBUG_HALT_BIT;
    else
        armTimerRegister[CONTROL] &= ~DEBUG_HALT_BIT;
}

void ArmTimer::enableFreeRunningCounter(bool enable) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    if (enable)
        armTimerRegister[CONTROL] |= FREE_RUNNING_COUNTER_ENABLE_BIT;
    else
        armTimerRegister[CONTROL] &= ~FREE_RUNNING_COUNTER_ENABLE_BIT;
}

void ArmTimer::setFreeRunningCounterScaler(Uint8 scaler) {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    armTimerRegister[CONTROL] &= 0xff00ffff;
    armTimerRegister[CONTROL] |= (((Uint32)scaler) << 16);
}

void ArmTimer::clearIrq() {
    volatile ArmTimerRegister * armTimerRegister = (ArmTimerRegister *)ARM_TIMER_BASE;
    armTimerRegister[IRQ_CLEAR] = 1; // 
}
