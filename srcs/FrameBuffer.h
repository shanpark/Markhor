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
#include "ResultCode.h"

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

    ResultCode setupFrameBuffer(Uint32 width, Uint32 height, Uint32 viewWidth, Uint32 viewHeight, Uint32 bpp);

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

    ResultCode blankScreen(Uint32 * state);
    ResultCode getPhysicalWH(Uint32 * width, Uint32 * height);
    ResultCode testPhysicalWH(Uint32 * width, Uint32 * height);
    ResultCode getVirtualWH(Uint32 * width, Uint32 * height);
    ResultCode testVirtualWH(Uint32 * width, Uint32 * height);
    ResultCode getDepth(Uint32 * depth);
    ResultCode testDepth(Uint32 * depth);
    ResultCode getPixelOrder(PixelOrder * pixelOrder);
    ResultCode getAlphaMode(Uint32 * alphaMode);
    ResultCode getPitch(Uint32 * pitch);
    ResultCode getVirtualOffset(Uint32 * x, Uint32 * y);
    ResultCode setVirtualOffset(Uint32 * x, Uint32 * y);
    ResultCode getPalette(Uint32 * palette);
    ResultCode setPalette(Uint32 offset, Uint32 length, Uint32 palette[]);
};

extern FrameBuffer frameBuffer;

#endif /* __FRAME_BUFFER_H */
