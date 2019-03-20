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

#ifndef __ARM_TIMER_H
#define __ARM_TIMER_H

#include "MarkhorTypes.h"

/* ARM timer registers offsets */
#define LOAD                    0
#define VALUE                   1
#define CONTROL                 2
#define IRQ_CLEAR               3
#define RAW_IRQ                 4
#define MASKED_IRQ              5
#define RELOAD                  6
#define PREDIVIDER              7
#define FREE_RUNNING_COUNTER    8

class ArmTimer {
    typedef Uint32 Register;

public:
    enum class TimerBits {
        Bit23,
        Bit16
    };

    enum class Prescale {
        NoPrescale,
        Prescale16,
        Prescale256
    };

    void setLoad(Uint32 load) { armTimerRegister[LOAD] = load; }
    Uint32 getValue() { return armTimerRegister[VALUE]; }
    void setTimerBits(TimerBits bits);
    void setPrescale(Prescale prescale);
    void enableInterrupt(bool enable, bool fast);
    void enable(bool enable);
    void haltDebugMode(bool halt);
    void clearIrq() { armTimerRegister[IRQ_CLEAR] = 1; }
    void setReload(Uint32 reload) { armTimerRegister[RELOAD] = reload; }
    Uint32 getPredivider() { return (armTimerRegister[PREDIVIDER] & 0x03ff); } // only 10 bits used 
    void setPredivider(Uint32 predivider) { armTimerRegister[PREDIVIDER] = (predivider & 0x03ff); } // only 10 bits used
    void enableFreeRunningCounter(bool enable);
    void setFreeRunningCounterScaler(Uint8 scaler);
    Uint32 getFreeRunningCounter() { return armTimerRegister[FREE_RUNNING_COUNTER]; }

private:
    static volatile Register * const armTimerRegister;
};

extern ArmTimer armTimer;

#endif /* __ARM_TIMER_H */

