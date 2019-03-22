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

#ifndef __MMU_H
#define __MMU_H

#include "MarkhorTypes.h"

class Mmu {
public:
    /* enable MMU */
    void enable(void) {
        Uint32 temp;
        asm volatile("mrc p15, 0, %0, c1, c0, 0": "=r" (temp));
        temp |= 0x01;
        asm volatile("mcr p15, 0, %0, c1, c0, 0": : "r" (temp));
    }

    /* CP15 - c2, TTBR0, TTBR1, TTBCR */
    void setTtbr0(Address table) { asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r" (table)); }
    void setTtbr1(Address table) { asm volatile("mcr p15, 0, %0, c2, c0, 1" : : "r" (table)); }
    
    /* CP15 - c3, Domain Access Control Register. DACR
     * The use of domains is deprecated in the ARMv7 architecture. */
    void setAllDomainToClient() { setDacr(0x55555555); }
    void setAllDomainToManager() { setDacr(0xffffffff); }
    void setDacr(Uint32 bits) { asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (bits)); }
    Uint32 getDacr() { Uint32 bits; asm volatile("mrc p15, 0, %0, c3, c0, 0" : "=r" (bits)); return bits; }
};

#endif /* __MMU_H */