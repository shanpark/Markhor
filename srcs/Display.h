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

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "MarkhorTypes.h"

typedef struct {
    Address frameBuffer;
    Address frameBufferEnd;
    Uint32 physicalWidth;
    Uint32 physicalHeight;
    Uint32 virtualWidth;
    Uint32 virtualHeight;
    Uint32 bpp;
    Uint32 pitch;
} DisplayInfo;

extern DisplayInfo displayInfo;

extern int setDisplay(Uint32 width, Uint32 height, Uint32 bpp);
extern void setPixel(Uint32 x, Uint32 y, Uint32 color);
extern void putChar(Uint32 x, Uint32 y, Uint32 ascii, Uint32 color);
extern void putString(Uint32 x, Uint32 y, char *str, Uint32 color);

#endif /* __DISPLAY_H */
