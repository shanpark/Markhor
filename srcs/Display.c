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
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = 0x00000000; // request

    // AllocateFrameBuffer tag
    params[0] = 0x10; // frame buffer alignment in bytes. //??? 어디에도 적당한 값이 없다. va_arg()가 오동작할 때 나오는 값을 쓴다. ;; 0인 것 같다. 일단 4를 쓰자.
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), AllocateFrameBuffer, 8, 1, params);

    // SetPhysicalWH tag
    params[0] = width;
    params[1] = height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), SetPhysicalWH, 8, 2, params);

    // SetVirtualWH tag
    params[0] = width;
    params[1] = height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), SetVirtualWH, 8, 2, params);

    // SetDepth tag
    params[0] = bpp;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), SetDepth, 4, 1, params);

    // GetPitch tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetPitch, 4, 0, 0);

    // End tag
    mbpb->tags[index] = 0x00000000;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & 0x80000000) && ((mbpb->code & 0x00000001) == 0)) {
        int end = (mbpb->size >> 2) - 2; // convert into Uint32 unit. and exclude header size.
        for (index = 0 ; index < end ; ) {
            MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[index]));
            switch (tag->id) {
                case AllocateFrameBuffer:
                    displayInfo.frameBuffer = (Address)tag->uint32Values[0];
                    displayInfo.frameBufferEnd = displayInfo.frameBuffer + tag->uint32Values[1];
                    index += (3 + 2);
                    break;
                case SetPhysicalWH:
                    displayInfo.physicalWidth = tag->uint32Values[0];
                    displayInfo.physicalHeight = tag->uint32Values[1];
                    index += (3 + 2);
                    break;
                case SetVirtualWH:
                    displayInfo.virtualWidth = tag->uint32Values[0];
                    displayInfo.virtualHeight = tag->uint32Values[1];
                    index += (3 + 2);
                    break;
                case SetDepth:
                    displayInfo.bpp = tag->uint32Values[0];
                    index += (3 + 1);
                    break;
                case GetPitch:
                    displayInfo.pitch = tag->uint32Values[0];
                    index += (3 + 1);
                    break;
                case 0x00000000: // End tag.
                    goto END_HANDLE_TAGS; // for safe.
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
