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

#include "FrameBuffer.h"
#include "Display.h"

void setPixel(Uint32 x, Uint32 y, Uint32 color) {
    if ((x < frameBuffer.getPhysicalWidth()) && (y < frameBuffer.getPhysicalHeight())) {
        Address dest = frameBuffer.getBase() + (y * frameBuffer.getPitch()) + (x * (frameBuffer.getBpp() >> 3));
        switch (frameBuffer.getBpp()) {
            case 8:
                *((Uint8 *)dest) = (Uint8)(color & 0x000000ff);
                break;
            case 16:
                *((Uint16 *)dest) = (Uint16)(color & 0x0000ffff);
                break;
            case 24:
                *((Uint8 *)dest) = (Uint8)(color & 0x000000ff);
                *((Uint8 *)dest + 1) = (Uint8)((color >> 8) & 0x000000ff);
                *((Uint8 *)dest + 2) = (Uint8)((color >> 16) & 0x000000ff);
                break;
            case 32:
                *((Uint32 *)dest) = color;
                break;
        }
    }
}

extern Uint8 font[2048];

void putChar(Uint32 x, Uint32 y, Uint32 ascii, Uint32 color) {
    int fontOffset = ascii * 16;
    int screenOffset = (frameBuffer.getPitch() * (y << 4)) + ((x << 3) * (frameBuffer.getBpp() >> 3));
    Address dest = frameBuffer.getBase() + screenOffset;

    for (int y = 0 ; y < 16 ; y++) {
        for (int x = 0x01 ; x <= 0x80 ; x <<= 1) {
            if (font[fontOffset] & x) {
                switch (frameBuffer.getBpp()) {
                    case 8:
                        *((Uint8 *)dest) = (Uint8)(color & 0x000000ff);
                        break;
                    case 16:
                        *((Uint16 *)dest) = (Uint16)(color & 0x0000ffff);
                        break;
                    case 24:
                        *((Uint8 *)dest) = (Uint8)(color & 0x000000ff);
                        *((Uint8 *)dest + 1) = (Uint8)((color >> 8) & 0x000000ff);
                        *((Uint8 *)dest + 2) = (Uint8)((color >> 16) & 0x000000ff);
                        break;
                    case 32:
                        *((Uint32 *)dest) = color;
                        break;
                }
            }
            dest += (frameBuffer.getBpp() >> 3);
        }
        fontOffset++;
        dest += (frameBuffer.getPitch() - frameBuffer.getBpp());
    }
}

void putString(Uint32 x, Uint32 y, char *str, Uint32 color) {
    while (*str) {
        putChar(x, y, *str, color);
        if (((++x) * 8) >= frameBuffer.getPhysicalWidth()) {
            x = 0;
            y++;
        }
        str++;
    }
}
