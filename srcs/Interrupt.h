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

class Interrupt {
public:
    enum class Source : Uint32 {
        Gpu0 = 0,
        Gpu1 = 1,
        Gpu2 = 2,
        Gpu3 = 3,
        Gpu4 = 4,
        Gpu5 = 5,
        Gpu6 = 6,
        Gpu7 = 7,
        Gpu8 = 8,
        Gpu9 = 9,
        Gpu10 = 10,
        Gpu11 = 11,
        Gpu12 = 12,
        Gpu13 = 13,
        Gpu14 = 14,
        Gpu15 = 15,
        Gpu16 = 16,
        Gpu17 = 17,
        Gpu18 = 18,
        Gpu19 = 19,
        Gpu20 = 20,
        Gpu21 = 21,
        Gpu22 = 22,
        Gpu23 = 23,
        Gpu24 = 24,
        Gpu25 = 25,
        Gpu26 = 26,
        Gpu27 = 27,
        Gpu28 = 28,
        Gpu29 = 29,
        Gpu30 = 30,
        Gpu31 = 31,
        Gpu32 = 32,
        Gpu33 = 33,
        Gpu34 = 34,
        Gpu35 = 35,
        Gpu36 = 36,
        Gpu37 = 37,
        Gpu38 = 38,
        Gpu39 = 39,
        Gpu40 = 40,
        Gpu41 = 41,
        Gpu42 = 42,
        Gpu43 = 43,
        Gpu44 = 44,
        Gpu45 = 45,
        Gpu46 = 46,
        Gpu47 = 47,
        Gpu48 = 48,
        Gpu49 = 49,
        Gpu50 = 50,
        Gpu51 = 51,
        Gpu52 = 52,
        Gpu53 = 53,
        Gpu54 = 54,
        Gpu55 = 55,
        Gpu56 = 56,
        Gpu57 = 57,
        Gpu58 = 58,
        Gpu59 = 59,
        Gpu60 = 60,
        Gpu61 = 61,
        Gpu62 = 62,
        Gpu63 = 63,
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

    Uint32 getIrq1();
    Uint32 getIrq2();
    Uint32 getBasicIrq();
    void setFastInterrupt(bool enable, Source source);
    void enableIrq1(Uint32 bits);
    void enableIrq2(Uint32 bits);
    void enableBasicIrq(Uint32 bits);
    void disableIrq1(Uint32 bits);
    void disableIrq2(Uint32 bits);
    void disableBasicIrq(Uint32 bits);
};

extern Interrupt interrupt;

#endif /* __INTERRUPT_H */
