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

enum class PixelOrder {
    BGR = 0x00,
    RGB = 0x01
};

class FrameBuffer {
private:
    Address base;
    Address end;
    Uint32 physicalWidth;
    Uint32 physicalHeight;
    Uint32 virtualWidth;
    Uint32 virtualHeight;
    Uint32 bpp;
    Uint32 pitch;
    PixelOrder pixelOrder;
    Uint32 alphaMode;;

public:
    FrameBuffer() : base(0), end(0), physicalWidth(0), physicalHeight(0), virtualWidth(0), virtualHeight(0), bpp(0), pitch(0), pixelOrder(PixelOrder::BGR), alphaMode(0) {}

    int setupFrameBuffer(Uint32 width, Uint32 height, Uint32 viewWidth, Uint32 viewHeight, Uint32 bpp);

    Address getBase() { return base; }
    Address getEnd() { return end; }
    Uint64 getSize() { return end - base; }
    Uint32 getPhysicalWidth() { return physicalWidth; }
    Uint32 getPhysicalHeight() { return physicalHeight; }
    Uint32 getVirtualWidth() { return virtualWidth; }
    Uint32 getVirtualHeight() { return virtualHeight; }
    Uint32 getBpp() { return bpp; }
    Uint32 getPitch() { return pitch; }
    PixelOrder getPixelOrder() { return pixelOrder; }
    Uint32 getAlphaMode() { return alphaMode; }

    Uint32 blankScreen(Uint32 state);

    int getPhysicalWH(Uint32 * width, Uint32 * height);
    int testPhysicalWH(Uint32 * width, Uint32 * height);

    int getVirtualWH(Uint32 * width, Uint32 * height);
    int testVirtualWH(Uint32 * width, Uint32 * height);

    int getDepth(Uint32 * depth);
    int testDepth(Uint32 * depth);

    int getPixelOrder(PixelOrder * pixelOrder);
    int getAlphaMode(Uint32 * alphaMode);

    int getPitch(Uint32 * pitch);

    int getVirtualOffset(Uint32 * x, Uint32 * y);
    int setVirtualOffset(Uint32 * x, Uint32 * y);

    int getPalette(Uint32 * palette);
    int setPalette(Uint32 offset, Uint32 length, Uint32 palette[]);
};

extern FrameBuffer frameBuffer;

#endif /* __FRAME_BUFFER_H */
