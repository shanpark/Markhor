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

#ifndef __SYSTEM_TIMER_H
#define __SYSTEM_TIMER_H

#include "MarkhorTypes.h"

/* System timer registers offsets */
#define CS  0
#define CLO 1
#define CHI 2
#define C0  3
#define C1  4
#define C2  5
#define C3  6

#define C0_IRQ  (1 << 0)
#define C1_IRQ  (1 << 1)
#define C2_IRQ  (1 << 2)
#define C3_IRQ  (1 << 3)

class SystemTimer {
    typedef Uint32 SystemTimerRegister;

public:
    enum class Channel {
        Channel0    = (1 << 0),
        Channel1    = (1 << 1),
        Channel2    = (1 << 2),
        Channel3    = (1 << 3)
    };

    Uint32 getCounterLo() { return systemTimerRegister[CLO]; }

    void enableInterrupt(Channel channel, bool enable, bool fast);

    void clearChannel0Irq() { systemTimerRegister[CS] = C0_IRQ; }
    void setChannel0Match(Uint32 match) { systemTimerRegister[C0] = match; }
    void clearChannel1Irq() { systemTimerRegister[CS] = C1_IRQ; }
    void setChannel1Match(Uint32 match) { systemTimerRegister[C1] = match; }
    void clearChannel2Irq() { systemTimerRegister[CS] = C2_IRQ; }
    void setChannel2Match(Uint32 match) { systemTimerRegister[C2] = match; }
    void clearChannel3Irq() { systemTimerRegister[CS] = C3_IRQ; }
    void setChannel3Match(Uint32 match) { systemTimerRegister[C3] = match; }

private:
    static volatile SystemTimerRegister * systemTimerRegister;
};

extern SystemTimer systemTimer;

#endif /* __SYSTEM_TIMER_H */