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

#ifndef __ARMTIMER_H
#define __ARMTIMER_H

#include "MarkhorTypes.h"

class ArmTimer {
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

    void setLoad(Uint32 load);
    Uint32 getValue();
    void setTimerBits(TimerBits bits);
    void setPrescale(Prescale prescale);
    void enableInterrupt(bool enable);
    void enable(bool enable);
    void haltDebugMode(bool halt);
    void enableFreeRunningCounter(bool enable);
    void setFreeRunningCounterScaler(Uint8 scaler);
    void clearIrq();
};

extern ArmTimer armTimer;

#endif /* __ARMTIMER_H */
