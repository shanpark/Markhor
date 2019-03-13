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

#include <string.h>
#include "Mailbox.h"
#include "MailboxProperty.h"
#include "FrameBuffer.h"

FrameBuffer frameBuffer;

/**
 * Allocate frame buffer for display.
 * 
 * @return 0 for off, 1 for on, -1 for failure.
 */
int setFrameBuffer(Uint32 width, Uint32 height, Uint32 viewWidth, Uint32 viewHeight, Uint32 bpp) {
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
    params[0] = viewWidth;
    params[1] = viewHeight;
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
                        if (size >= 2) {
                            frameBuffer.base = (Address)tag->uint32Values[0];
                            frameBuffer.end = frameBuffer.base + tag->uint32Values[1];
                        }
                        break;
                    case MPTISetPhysicalWH:
                        if (size >= 2) {
                            frameBuffer.physicalWidth = tag->uint32Values[0];
                            frameBuffer.physicalHeight = tag->uint32Values[1];
                        }
                        break;
                    case MPTISetVirtualWH:
                        if (size >= 2) {
                            frameBuffer.virtualWidth = tag->uint32Values[0];
                            frameBuffer.virtualHeight = tag->uint32Values[1];
                        }
                        break;
                    case MPTISetDepth:
                        if (size >= 1) {
                            frameBuffer.bpp = tag->uint32Values[0];
                        }
                        break;
                    case MPTIGetPitch:
                        if (size >= 1) {
                            frameBuffer.pitch = tag->uint32Values[0];
                        }
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

/**
 * blank screen.
 * 테스트 결과 1일 때 화면을 지우고 0일 때는 아무 일도 하지 않음.
 * 
 * @param state 0 for off, 1 for on.
 * @return 0 for off, 1 for on, -1 for failure.
 */
Uint32 blankScreen(Uint32 state) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // BlankScreen tag
    params[0] = state;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIBlankScreen, 4, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIBlankScreen) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            return tag->uint32Values[0];
        }
    }

    return -1;
}

/**
 * @param width 
 * @param height
 * @return 0 if successful, -1 for failure.
 */
int getPhysicalWH(Uint32 * width, Uint32 * height) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // TestPhysicalWH tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPhysicalWH, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetPhysicalWH) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 해상도를 반환한다. 즉, getPhysicalWH()와 같다.
 * 
 * @param width
 * @param height
 * @return 0 if successful, -1 for failure.
 */
int testPhysicalWH(Uint32 * width, Uint32 * height) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // TestPhysicalWH tag
    params[0] = *width;
    params[1] = *height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTITestPhysicalWH, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTITestPhysicalWH) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * @param width 
 * @param height
 * @return 0 if successful, -1 for failure.
 */
int getVirtualWH(Uint32 * width, Uint32 * height) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetVirtualWH tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetVirtualWH, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetVirtualWH) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 값을 반환한다. 즉, getVirtualWH()와 같다.
 * 
 * @param width
 * @param height
 * @return 0 if successful, -1 for failure.
 */
int testVirtualWH(Uint32 * width, Uint32 * height) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // TestVirtualWH tag
    params[0] = *width;
    params[1] = *height;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTITestVirtualWH, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTITestVirtualWH) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * @param depth 
 * @return 0 if successful, -1 for failure.
 */
int getDepth(Uint32 * depth) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetDepth tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetDepth, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetDepth) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            *depth = tag->uint32Values[0];
            return 0;
        }
    }

    return -1;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 값을 반환한다. 즉, getDepth()와 같다.

 * @param depth
 * @return 0 if successful, -1 for failure.
 */
int testDepth(Uint32 * depth) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // TestDepth tag
    params[0] = *depth;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTITestDepth, 4, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTITestDepth) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            *depth = tag->uint32Values[0];
            return 0;
        }
    }

    return -1;
}

/**
 * @return 0 if BGR, 1 if RGB, -1 for failure.
 */
Uint32 getPixelOrder() {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPixelOrder tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPixelOrder, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetPixelOrder) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            return tag->uint32Values[0];
        }
    }

    return -1;
}

/**
 * @return 0x0: Alpha channel enabled (0 = fully opaque)
 *         0x1: Alpha channel reversed (0 = fully transparent)
 *         0x2: Alpha channel ignored.
 *         -1 for failure.
 */
Uint32 getAlphaMode() {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetAlphaMode tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetAlphaMode, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetAlphaMode) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            return tag->uint32Values[0];
        }
    }

    return -1;
}

/**
 * @return bytes per line, -1 for failure.
 */
Uint32 getPitch() {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPitch tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPitch, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetPitch) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            return tag->uint32Values[0];
        }
    }

    return -1;
}

/**
 * @param x
 * @param y
 * @return 0 if successful, -1 for failure.
 */
int getVirtualOffset(Uint32 * x, Uint32 * y) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetVirtualOffset tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetVirtualOffset, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetVirtualOffset) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *x = tag->uint32Values[0];
            *y = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * 테스트 결과 값은 반영되나 실제로 화면에 보이는 상태는 바뀌지 않는다.
 * @param x
 * @param y
 * @return 0 if successful, -1 for failure.
 */
int setVirtualOffset(Uint32 * x, Uint32 * y) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // SetVirtualOffset tag
    params[0] = *x;
    params[1] = *y;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetVirtualOffset, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetVirtualOffset) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *x = tag->uint32Values[0];
            *y = tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

/**
 * 바로 전에 설정한 팔레트와 다른 값이 나온다. 화면에 표시된 색으로 봐서는 설정이 제대로 된 것 같고 
 * 이 함수가 오동작하는 것으로 판단됨.
 * @param palette must be Uint32[256] size or more.
 * @return 0 if success, -1 for failure.
 */
int getPalette(Uint32 * palette) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPalette tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPalette, 1024, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetPalette) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 1024)) {
            memcpy(palette, tag->uint32Values, 1024);
            return 0;
        }
    }

    return -1;
}

/**
 * @return 0 if valid, 1 if invalid, -1 for failure.
 */
int setPalette(Uint32 offset, Uint32 length, Uint32 table[]) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // SetPalette tag
    params[0] = offset;
    params[1] = length;
    MailboxPropertyTag * tag = (MailboxPropertyTag *)&(mpb->tags[index]);
    index += fillMailboxRequestTagInfo(tag, MPTISetPalette, 8 + (length << 2), 2, params);
    memcpy(&(tag->uint32Values[2]), table, length << 2);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetPalette) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4)) {
            return tag->uint32Values[0];
        }
    }

    return -1;
}
