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
/**
 * Laspberry Pi는 텍스트 모드가 따로 없기 때문에 적당한 그래픽 모드에서 텍스트 모드를 
 * 에뮬레이트한다. 
 * 8 depth로 emulate 하도록 한다.
 */

#include <memory.h>
#include "FrameBuffer.h"
#include "TextEmul.h"

#define SCREEN_DEPTH    8

#define FONT_WIDTH      8   // do not change.
#define FONT_HEIGHT     16  // do not change. 

extern Uint8 font[2048];

typedef struct {
    int width;
    int height;
} TextModeInfo;

// static const int screenWidth = 1920, screenHeight = 1080; // 16:9
static const int screenWidth = 1680, screenHeight = 1050; // 16:10
static TextModeInfo textModeInfo;
static Uint32 palette[16] = { // format = 0x00bbggrr
    0x00000000, 0x00000080, 0x00008000, 0x00008080, 0x00800000, 0x00800080, 0x00808000, 0x00c0c0c0,
    0x00808080, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff, 0x00ffff00, 0x00ffffff };

#define BACK_COLOR  0x00
#define FORE_COLOR  0x07

int setupTextMode() {
    if (setupFrameBuffer(screenWidth, screenHeight, screenWidth, screenHeight, SCREEN_DEPTH) != 0)
        return -1;

    setPalette(0, sizeof(palette) / sizeof(palette[0]), palette);

    textModeInfo.width = screenWidth / FONT_WIDTH;
    textModeInfo.height = screenHeight / FONT_HEIGHT;

    return 0;
}

int getTextModeWidth() {
    return textModeInfo.width;
}

int getTextModeHeight() {
    return textModeInfo.height;
}

void printCharAt(Uint32 x, Uint32 y, char ascii, Uint32 color) {
    if ((x >= textModeInfo.width) || (y >= textModeInfo.height) || (ascii < 0))
        return; // do nothing.

    int fontOffset = ascii * FONT_HEIGHT;
    int bytePerPixel = (frameBuffer.bpp >> 3);
    int screenOffset = (frameBuffer.pitch * (y << 4)) + ((x << 3) * bytePerPixel); // = (frameBuffer.pitch * (y * FONT_HEIGHT)) + ((x * FONT_WIDTH) * bytePerPixel);
    Address dest = frameBuffer.base + screenOffset;

    for (int fy = 0 ; fy < FONT_HEIGHT ; fy++) {
        for (int bit = 0x01 ; bit <= 0x80 ; bit <<= 1) {
            if (font[fontOffset] & bit) {
                switch (frameBuffer.bpp) {
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
            dest += bytePerPixel;
        }
        fontOffset++;
        dest += (frameBuffer.pitch - frameBuffer.bpp); // frameBuffer.bpp = (FONT_WIDTH * bytePerPixel)
    }
}

void printStringAt(Uint32 x, Uint32 y, char * str, int length, Uint32 color) {
    char * pos = str;
    char * end = pos + length;
    for (int inx = x ; (inx < textModeInfo.width) && (pos < end) ; inx++)
        printCharAt(inx, y, *pos++, color);
}

void scrollUp(int lines) {
    memcpy((void *)frameBuffer.base, 
        (void *)(frameBuffer.base + (frameBuffer.pitch * FONT_HEIGHT * lines)), 
        (textModeInfo.height - lines) * (frameBuffer.pitch * FONT_HEIGHT));

    memset((void *)(frameBuffer.base + ((textModeInfo.height - lines) * (frameBuffer.pitch * FONT_HEIGHT))), 
        BACK_COLOR, 
        (frameBuffer.pitch * FONT_HEIGHT) * lines);
}
