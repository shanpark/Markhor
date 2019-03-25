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

#include "TranslationTable.h"

void TranslationTable::map(Address va, Address pa, AccessPermission ap, MemoryType cb, bool nG) {
    Uint32 entry;
    entry = pa & 0xfff00000; // Section Base Address
    entry |= static_cast<Uint32>(ap);
    entry |= static_cast<Uint32>(cb);
    if (nG)
        entry |= 0x00020000; // nG

    int inx = (va & 0xfff00000) >> 20;
    base[inx] = entry;
}
