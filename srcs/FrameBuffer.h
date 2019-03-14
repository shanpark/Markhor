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

#ifndef __FRAME_BUFFER_H
#define __FRAME_BUFFER_H

#include "MarkhorTypes.h"

typedef struct {
    Address base;
    Address end;
    Uint32 physicalWidth;
    Uint32 physicalHeight;
    Uint32 virtualWidth;
    Uint32 virtualHeight;
    Uint32 bpp;
    Uint32 pitch;
} FrameBuffer;

extern FrameBuffer frameBuffer;

extern int setupFrameBuffer(Uint32 width, Uint32 height, Uint32 viewWidth, Uint32 viewHeight, Uint32 bpp);
extern Uint32 blankScreen(Uint32 state);
extern int getPhysicalWH(Uint32 * width, Uint32 * height);
extern int testPhysicalWH(Uint32 * width, Uint32 * height);
extern int getVirtualWH(Uint32 * width, Uint32 * height);
extern int testVirtualWH(Uint32 * width, Uint32 * height);
extern int getDepth(Uint32 * depth);
extern int testDepth(Uint32 * depth);
extern Uint32 getPixelOrder();
extern Uint32 getAlphaMode();
extern Uint32 getPitch();
extern int getVirtualOffset(Uint32 * x, Uint32 * y);
extern int setVirtualOffset(Uint32 * x, Uint32 * y);
extern int getPalette(Uint32 * palette);
extern int setPalette(Uint32 offset, Uint32 length, Uint32 palette[]);

#endif /* __FRAME_BUFFER_H */
