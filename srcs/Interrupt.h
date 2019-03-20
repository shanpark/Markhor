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

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "MarkhorTypes.h"

/* Interrupt registers offsets */
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

/* Basic IRQ Pending register bits */
#define IRQ_TIMER                   (0x01 << 0)
#define IRQ_MAILBOX                 (0x01 << 1)
#define IRQ_DOORBELL0               (0x01 << 2)
#define IRQ_DOORBELL1               (0x01 << 3)
#define IRQ_GPU0_HALTED             (0x01 << 4)
#define IRQ_GPU1_HALTED             (0x01 << 5)
#define IRQ_ILLEGAL_ACCESS_TYPE1    (0x01 << 6)
#define IRQ_ILLEGAL_ACCESS_TYPE0    (0x01 << 7)
#define IRQ_IRQ1_REGISTER_PENDING   (0x01 << 8)
#define IRQ_IRQ2_REGISTER_PENDING   (0x01 << 9)
#define IRQ_GPU_IRQ7                (0x01 << 10)
#define IRQ_GPU_IRQ9                (0x01 << 11)
#define IRQ_GPU_IRQ10               (0x01 << 12)
#define IRQ_GPU_IRQ18               (0x01 << 13)
#define IRQ_GPU_IRQ19               (0x01 << 14)
#define IRQ_GPU_IRQ53               (0x01 << 15)
#define IRQ_GPU_IRQ54               (0x01 << 16)
#define IRQ_GPU_IRQ55               (0x01 << 17)
#define IRQ_GPU_IRQ56               (0x01 << 18)
#define IRQ_GPU_IRQ57               (0x01 << 19)
#define IRQ_GPU_IRQ62               (0x01 << 20)

/* IRQ1 Pending register bits */
#define GPU0_IRQ0   (1 << 0)
#define GPU0_IRQ1   (1 << 1)
#define GPU0_IRQ2   (1 << 2)
#define GPU0_IRQ3   (1 << 3)
#define GPU0_IRQ4   (1 << 4)
#define GPU0_IRQ5   (1 << 5)
#define GPU0_IRQ6   (1 << 6)
#define GPU0_IRQ7   (1 << 7)
#define GPU0_IRQ8   (1 << 8)
#define GPU0_IRQ9   (1 << 9)
#define GPU0_IRQ10  (1 << 10)
#define GPU0_IRQ11  (1 << 11)
#define GPU0_IRQ12  (1 << 12)
#define GPU0_IRQ13  (1 << 13)
#define GPU0_IRQ14  (1 << 14)
#define GPU0_IRQ15  (1 << 15)
#define GPU0_IRQ16  (1 << 16)
#define GPU0_IRQ17  (1 << 17)
#define GPU0_IRQ18  (1 << 18)
#define GPU0_IRQ19  (1 << 19)
#define GPU0_IRQ20  (1 << 20)
#define GPU0_IRQ21  (1 << 21)
#define GPU0_IRQ22  (1 << 22)
#define GPU0_IRQ23  (1 << 23)
#define GPU0_IRQ24  (1 << 24)
#define GPU0_IRQ25  (1 << 25)
#define GPU0_IRQ26  (1 << 26)
#define GPU0_IRQ27  (1 << 27)
#define GPU0_IRQ28  (1 << 28)
#define GPU0_IRQ29  (1 << 29)
#define GPU0_IRQ30  (1 << 30)
#define GPU0_IRQ31  (1 << 31)

/* IRQ2 Pending register bits */
#define GPU1_IRQ0   (1 << 0)
#define GPU1_IRQ1   (1 << 1)
#define GPU1_IRQ2   (1 << 2)
#define GPU1_IRQ3   (1 << 3)
#define GPU1_IRQ4   (1 << 4)
#define GPU1_IRQ5   (1 << 5)
#define GPU1_IRQ6   (1 << 6)
#define GPU1_IRQ7   (1 << 7)
#define GPU1_IRQ8   (1 << 8)
#define GPU1_IRQ9   (1 << 9)
#define GPU1_IRQ10  (1 << 10)
#define GPU1_IRQ11  (1 << 11)
#define GPU1_IRQ12  (1 << 12)
#define GPU1_IRQ13  (1 << 13)
#define GPU1_IRQ14  (1 << 14)
#define GPU1_IRQ15  (1 << 15)
#define GPU1_IRQ16  (1 << 16)
#define GPU1_IRQ17  (1 << 17)
#define GPU1_IRQ18  (1 << 18)
#define GPU1_IRQ19  (1 << 19)
#define GPU1_IRQ20  (1 << 20)
#define GPU1_IRQ21  (1 << 21)
#define GPU1_IRQ22  (1 << 22)
#define GPU1_IRQ23  (1 << 23)
#define GPU1_IRQ24  (1 << 24)
#define GPU1_IRQ25  (1 << 25)
#define GPU1_IRQ26  (1 << 26)
#define GPU1_IRQ27  (1 << 27)
#define GPU1_IRQ28  (1 << 28)
#define GPU1_IRQ29  (1 << 29)
#define GPU1_IRQ30  (1 << 30)
#define GPU1_IRQ31  (1 << 31)

class Interrupt {
    typedef Uint32 Register;

public:
    /* for fast interrupt setting */
    enum class Source : Uint32 {
        GpuIrq0 = 0,
        GpuIrq1 = 1,
        GpuIrq2 = 2,
        GpuIrq3 = 3,
        GpuIrq4 = 4,
        GpuIrq5 = 5,
        GpuIrq6 = 6,
        GpuIrq7 = 7,
        GpuIrq8 = 8,
        GpuIrq9 = 9,
        GpuIrq10 = 10,
        GpuIrq11 = 11,
        GpuIrq12 = 12,
        GpuIrq13 = 13,
        GpuIrq14 = 14,
        GpuIrq15 = 15,
        GpuIrq16 = 16,
        GpuIrq17 = 17,
        GpuIrq18 = 18,
        GpuIrq19 = 19,
        GpuIrq20 = 20,
        GpuIrq21 = 21,
        GpuIrq22 = 22,
        GpuIrq23 = 23,
        GpuIrq24 = 24,
        GpuIrq25 = 25,
        GpuIrq26 = 26,
        GpuIrq27 = 27,
        GpuIrq28 = 28,
        GpuIrq29 = 29,
        GpuIrq30 = 30,
        GpuIrq31 = 31,
        GpuIrq32 = 32,
        GpuIrq33 = 33,
        GpuIrq34 = 34,
        GpuIrq35 = 35,
        GpuIrq36 = 36,
        GpuIrq37 = 37,
        GpuIrq38 = 38,
        GpuIrq39 = 39,
        GpuIrq40 = 40,
        GpuIrq41 = 41,
        GpuIrq42 = 42,
        GpuIrq43 = 43,
        GpuIrq44 = 44,
        GpuIrq45 = 45,
        GpuIrq46 = 46,
        GpuIrq47 = 47,
        GpuIrq48 = 48,
        GpuIrq49 = 49,
        GpuIrq50 = 50,
        GpuIrq51 = 51,
        GpuIrq52 = 52,
        GpuIrq53 = 53,
        GpuIrq54 = 54,
        GpuIrq55 = 55,
        GpuIrq56 = 56,
        GpuIrq57 = 57,
        GpuIrq58 = 58,
        GpuIrq59 = 59,
        GpuIrq60 = 60,
        GpuIrq61 = 61,
        GpuIrq62 = 62,
        GpuIrq63 = 63,
        Timer = 64,
        Mailbox = 65,
        Doorbell0 = 66,
        Doorbell1 = 67,
        Gpu0Halted = 68,
        Gpu1Halted = 69,
        IllegalAccessType1 = 70,
        IllegalAccessType0 = 71
    };

    void enableInterruptRequest(void);
    void enableFastInterruptRequest(void);

    Uint32 getIrqPending1() { return interruptRegister[IRQ_PENDIGN_1]; }
    Uint32 getIrqPending2() { return interruptRegister[IRQ_PENDIGN_2]; }
    Uint32 getBasicIrqPending()  { return interruptRegister[IRQ_BASIC_PENDING]; }
    void setFastInterrupt(bool enable, Source source);
    void enableIrq1(Uint32 bits) { interruptRegister[ENABLE_IRQ_1] = bits; } // Only '1' bits enable IRQs. '0' bits have no effect.
    void enableIrq2(Uint32 bits) { interruptRegister[ENABLE_IRQ_2] = bits; } // Only '1' bits enable IRQs. '0' bits have no effect.
    void enableBasicIrq(Uint32 bits) { interruptRegister[ENABLE_BASIC_IRQ] = bits; } // Only '1' bits enable IRQs. '0' bits have no effect.
    void disableIrq1(Uint32 bits) { interruptRegister[DISABLE_IRQ_1] = bits; } // Only '1' bits disable IRQs. '0' bits have no effect.
    void disableIrq2(Uint32 bits) { interruptRegister[DISABLE_IRQ_2] = bits; } // Only '1' bits disable IRQs. '0' bits have no effect.
    void disableBasicIrq(Uint32 bits) { interruptRegister[DISABLE_BASIC_IRQ] = bits; } // Only '1' bits disable IRQs. '0' bits have no effect.

private:
    static volatile Register * const interruptRegister;
};

extern Interrupt interrupt;

#endif /* __INTERRUPT_H */
