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
#include "SystemTimer.h"

/* BCM2385 ARM Peripherals document */
#define SYSTEM_TIMER_BASE   (IO_PERIPHERALS_BASE + 0x3000UL)

SystemTimer systemTimer;

volatile SystemTimer::Register * const SystemTimer::systemTimerRegister = (SystemTimer::Register *)SYSTEM_TIMER_BASE;

void SystemTimer::enableInterrupt(Channel channel, bool enable, bool fast) {
    Interrupt::Source source;
    Uint32 bits;

    switch (channel) {
        case Channel::Channel0:
            source = Interrupt::Source::GpuIrq0;
            bits = C0_IRQ;
            break;
        case Channel::Channel1:
            source = Interrupt::Source::GpuIrq1;
            bits = C1_IRQ;
            break;
        case Channel::Channel2:
            source = Interrupt::Source::GpuIrq2;
            bits = C2_IRQ;
            break;
        case Channel::Channel3:
            source = Interrupt::Source::GpuIrq3;
            bits = C3_IRQ;
            break;
    }

    if (enable) {
        if (fast) {
            interrupt.setFastInterrupt(true, source);
            interrupt.disableIrq1(bits);
        } else {
            interrupt.setFastInterrupt(false, source);
            interrupt.enableIrq1(bits);
        }
    } else {
        interrupt.setFastInterrupt(false, source);
        interrupt.disableIrq1(bits);
    }
}

/**
 * 
 */
void SystemTimer::wait(Uint32 timeout) {
    Uint32 hi = getCounterHi();
    Uint32 lo = getCounterLo() + timeout;
    if (lo < getCounterLo())
        hi++;
    while ((getCounterHi() < hi) || (getCounterLo() < lo))
        ;
}

/**
 * 
 */
void SystemTimer::waitUntil(Condition cond, Uint32 timeout) {
    Uint32 hi = getCounterHi();
    Uint32 lo = getCounterLo() + timeout;
    if (lo < getCounterLo())
        hi++;

    do {
        if (cond())
            break;
    } while ((getCounterHi() < hi) || (getCounterLo() < lo));
}
