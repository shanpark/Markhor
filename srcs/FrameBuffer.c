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
#include "FrameBuffer.h"

FrameBuffer frameBuffer;

int setFrameBuffer(Uint32 width, Uint32 height, Uint32 bpp) {
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
                        frameBuffer.base = (Address)tag->uint32Values[0];
                        frameBuffer.end = frameBuffer.base + tag->uint32Values[1];
                        break;
                    case MPTISetPhysicalWH:
                        frameBuffer.physicalWidth = tag->uint32Values[0];
                        frameBuffer.physicalHeight = tag->uint32Values[1];
                        break;
                    case MPTISetVirtualWH:
                        frameBuffer.virtualWidth = tag->uint32Values[0];
                        frameBuffer.virtualHeight = tag->uint32Values[1];
                        break;
                    case MPTISetDepth:
                        frameBuffer.bpp = tag->uint32Values[0];
                        break;
                    case MPTIGetPitch:
                        frameBuffer.pitch = tag->uint32Values[0];
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
