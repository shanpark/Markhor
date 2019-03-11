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

#include "Mailbox.h"
#include "MailboxProperty.h"
#include "Display.h"

DisplayInfo displayInfo;

int setDisplay(Uint32 width, Uint32 height, Uint32 bpp) {
    int index = 0;
    Uint32 params[2];
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // AllocateFrameBuffer tag
    params[0] = 0x10; // frame buffer alignment in bytes. //??? 어디에도 적당한 값이 없다. va_arg()가 오동작할 때 나오는 값을 쓴다. ;; 0인 것 같다. 일단 4를 쓰자.
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIAllocateFrameBuffer, 8, 1, params);

    // SetPhysicalWH tag
    params[0] = width;
    params[1] = height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetPhysicalWH, 8, 2, params);

    // SetVirtualWH tag
    params[0] = width;
    params[1] = height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetVirtualWH, 8, 2, params);

    // SetDepth tag
    params[0] = bpp;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetDepth, 4, 1, params);

    // GetPitch tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPitch, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        int end = (mpb->size >> 2) - 2; // convert into Uint32 unit. and exclude header size.
        for (index = 0 ; index < end ; ) {
            MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[index]));
            if (tag->code & MPTCResponseTagCodeBit) {
                int size = ((tag->code & MPTCResponseTagCodeSizeBits) + 3) >> 2; // Uint32 unit.
                switch (tag->id) {
                    case MPTIAllocateFrameBuffer:
                        displayInfo.frameBuffer = (Address)tag->uint32Values[0];
                        displayInfo.frameBufferEnd = displayInfo.frameBuffer + tag->uint32Values[1];
                        break;
                    case MPTISetPhysicalWH:
                        displayInfo.physicalWidth = tag->uint32Values[0];
                        displayInfo.physicalHeight = tag->uint32Values[1];
                        break;
                    case MPTISetVirtualWH:
                        displayInfo.virtualWidth = tag->uint32Values[0];
                        displayInfo.virtualHeight = tag->uint32Values[1];
                        break;
                    case MPTISetDepth:
                        displayInfo.bpp = tag->uint32Values[0];
                        break;
                    case MPTIGetPitch:
                        displayInfo.pitch = tag->uint32Values[0];
                        break;
                    case MPTIEnd: // End tag.
                        goto END_HANDLE_TAGS; // for safe.
                }
                index += (3 + size);
            } else {
                break;
            }
        }

END_HANDLE_TAGS:
        return 0;
    }

    return -1;
}

void setPixel(Uint32 x, Uint32 y, Uint32 color) {
    if ((x < displayInfo.physicalWidth) && (y < displayInfo.physicalHeight)) {
        Address dest = displayInfo.frameBuffer + (y * displayInfo.pitch) + (x * (displayInfo.bpp >> 3));
        switch (displayInfo.bpp) {
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
    int screenOffset = (displayInfo.pitch * (y << 4)) + ((x << 3) * (displayInfo.bpp >> 3));
    Address dest = displayInfo.frameBuffer + screenOffset;

    for (int y = 0 ; y < 16 ; y++) {
        for (int x = 0x01 ; x <= 0x80 ; x <<= 1) {
            if (font[fontOffset] & x) {
                switch (displayInfo.bpp) {
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
            dest += (displayInfo.bpp >> 3);
        }
        fontOffset++;
        dest += (displayInfo.pitch - displayInfo.bpp);
    }
}

void putString(Uint32 x, Uint32 y, char *str, Uint32 color) {
    while (*str) {
        putChar(x, y, *str, color);
        if (((++x) * 8) >= displayInfo.physicalWidth) {
            x = 0;
            y++;
        }
        str++;
    }
}
