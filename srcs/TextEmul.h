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

#ifndef __TEXT_EMUL_H
#define __TEXT_EMUL_H

#include "MarkhorTypes.h"

extern int setupTextMode();
extern int getTextModeWidth();
extern int getTextModeHeight();
extern void printCharAt(Uint32 x, Uint32 y, char ascii, Uint32 color);
extern void printStringAt(Uint32 x, Uint32 y, char * str, int length, Uint32 color);
extern void scrollUp(int lines);

#endif /* __TEXT_EMUL_H */