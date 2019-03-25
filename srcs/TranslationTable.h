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

#ifndef __TRANSLATION_TABLE_H
#define __TRANSLATION_TABLE_H

#include <string.h>
#include "MarkhorTypes.h"

#define NUM_OF_ENTRY    4096

enum class AccessPermission {
    Privileged      = 0x00000400,
    Unprivileged    = 0x00000c00
};

enum class MemoryType {
    StronglyOrdered = 0x2,
    Device          = 0x6,
    Normal          = 0xe
};

class TranslationTable {
public:
    TranslationTable(Uint32 * base) : base(base) {}

    void init() { memset(base, 0, NUM_OF_ENTRY * sizeof(Uint32)); }
    /**
     * Access Permission : APX, AP
     * Memory Type : TEX, C, B
     * non-Global, ASID
     */
    void map(Address va, Address pa, AccessPermission ap, MemoryType cb, bool nG);

    Uint32 * getBase() { return base; }

private:
    Uint32 * base;
};

#endif  /* __TRANSLATION_TABLE_H */
