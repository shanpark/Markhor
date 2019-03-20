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

#include <cstring>
#include "Mailbox.h"
#include "MailboxProperty.h"
#include "FrameBuffer.h"

FrameBuffer frameBuffer;

/**
 * Set screen resolution and allocate frame buffers.
 * should be called before any other method.
 * 
 * @param width physical width of frame buffer.
 * @param height physical height of frame buffer.
 * @param width of display. may be smaller than width of frame buffer.
 * @param height of display. may be smaller than height of frame buffer.
 * @param bpp bit per pixel.
 */
ResultCode FrameBuffer::setupFrameBuffer(Uint32 width, Uint32 height, Uint32 viewWidth, Uint32 viewHeight, Uint32 bpp) {
    int index = 0;
    Uint32 params[2];
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // AllocateFrameBuffer tag
    params[0] = 0x10; // frame buffer alignment in bytes. don't know what is correct value.
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::AllocateFrameBuffer, 8, 1, params);

    // SetPhysicalWH tag
    params[0] = width;
    params[1] = height;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetPhysicalWH, 8, 2, params);

    // SetVirtualWH tag
    params[0] = viewWidth;
    params[1] = viewHeight;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetVirtualWH, 8, 2, params);

    // SetDepth tag
    params[0] = bpp;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetDepth, 4, 1, params);

    // GetPitch tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPitch, 4, 0, 0);

    // GetPixelOrder tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPixelOrder, 4, 0, 0);

    // GetAlphaMode tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetAlphaMode, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit)) == 0) {
        int result = 0; 
        int end = (mpb->size >> 2) - 2; // convert into Uint32 unit. and exclude header size.
        for (index = 0 ; index < end ; ) {
            MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[index]));
            if (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) {
                int size = ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) + 3) >> 2; // Uint32 unit.
                switch (static_cast<MailboxPropertyTagId>(tag->id)) {
                    case MailboxPropertyTagId::AllocateFrameBuffer:
                        if (size >= 2) {
                            frameBuffer.base = (Address)tag->uint32Values[0];
                            frameBuffer.end = frameBuffer.base + tag->uint32Values[1];
                        } else {
                            goto ERROR;
                        }
                        break;
                    case MailboxPropertyTagId::SetPhysicalWH:
                        if (size >= 2) {
                            frameBuffer.physicalWidth = tag->uint32Values[0];
                            frameBuffer.physicalHeight = tag->uint32Values[1];
                        } else {
                            goto ERROR;
                        }
                        break;
                    case MailboxPropertyTagId::SetVirtualWH:
                        if (size >= 2) {
                            frameBuffer.virtualWidth = tag->uint32Values[0];
                            frameBuffer.virtualHeight = tag->uint32Values[1];
                        } else {
                            goto ERROR;
                        }
                        break;
                    case MailboxPropertyTagId::SetDepth:
                        if (size >= 1)
                            frameBuffer.bpp = tag->uint32Values[0];
                        else
                            goto ERROR;
                        break;
                    case MailboxPropertyTagId::GetPitch:
                        if (size >= 1)
                            frameBuffer.pitch = tag->uint32Values[0];
                        else
                            goto ERROR;
                        break;
                    case MailboxPropertyTagId::GetPixelOrder:
                        if (size >= 1)
                            frameBuffer.pixelOrder = static_cast<PixelOrder>(tag->uint32Values[0]);
                        else
                            goto ERROR;
                        break;
                    case MailboxPropertyTagId::GetAlphaMode:
                        if (size >= 1)
                            frameBuffer.alphaMode = tag->uint32Values[0];
                        else
                            goto ERROR;
                        break;
                    case MailboxPropertyTagId::End: // End tag.
                        goto END_HANDLE_TAGS; // for safe.
                }
                index += (3 + size);
            } else {
                break;
            }
        }

END_HANDLE_TAGS:
        mailbox.unlockBuffer();
        return ResultCode::Success;
    }

ERROR:
    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * blank screen.
 * 
 * @param state 0 for off, 1 for on. 1 blanks screen.
 * @return ResultCode.
 */
ResultCode FrameBuffer::blankScreen(Uint32 * state) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // BlankScreen tag
    params[0] = *state;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::BlankScreen, 4, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::BlankScreen) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *state = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param width 
 * @param height
 * @return ResultCode
 */
ResultCode FrameBuffer::getPhysicalWH(Uint32 * width, Uint32 * height) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // TestPhysicalWH tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPhysicalWH, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetPhysicalWH) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 해상도를 반환한다. 즉, getPhysicalWH()와 같다.
 * 
 * @param width
 * @param height
 * @return ResultCode
 */
ResultCode FrameBuffer::testPhysicalWH(Uint32 * width, Uint32 * height) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // TestPhysicalWH tag
    params[0] = *width;
    params[1] = *height;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::TestPhysicalWH, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::TestPhysicalWH) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param width 
 * @param height
 * @return ResultCode
 */
ResultCode FrameBuffer::getVirtualWH(Uint32 * width, Uint32 * height) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetVirtualWH tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetVirtualWH, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetVirtualWH) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 값을 반환한다. 즉, getVirtualWH()와 같다.
 * 
 * @param width
 * @param height
 * @return ResultCode
 */
ResultCode FrameBuffer::testVirtualWH(Uint32 * width, Uint32 * height) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // TestVirtualWH tag
    params[0] = *width;
    params[1] = *height;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::TestVirtualWH, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::TestVirtualWH) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *width = tag->uint32Values[0];
            *height = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param depth 
 * @return ResultCode
 */
ResultCode FrameBuffer::getDepth(Uint32 * depth) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetDepth tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetDepth, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetDepth) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *depth = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 테스트 결과 어떤 값을 전달하든 현재 값을 반환한다. 즉, getDepth()와 같다.

 * @param depth
 * @return ResultCode
 */
ResultCode FrameBuffer::testDepth(Uint32 * depth) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // TestDepth tag
    params[0] = *depth;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::TestDepth, 4, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::TestDepth) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *depth = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return ResultCode
 */
ResultCode FrameBuffer::getPixelOrder(PixelOrder * pixelOrder) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPixelOrder tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPixelOrder, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetPixelOrder) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *pixelOrder = static_cast<PixelOrder>(tag->uint32Values[0]);
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 
 * @return ResultCode
 */
ResultCode FrameBuffer::getAlphaMode(Uint32 * alphaMode) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetAlphaMode tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetAlphaMode, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetAlphaMode) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *alphaMode = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return ResultCode
 */
ResultCode FrameBuffer::getPitch(Uint32 * pitch) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPitch tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPitch, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetPitch) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            *pitch = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param x
 * @param y
 * @return ResultCode
 */
ResultCode FrameBuffer::getVirtualOffset(Uint32 * x, Uint32 * y) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetVirtualOffset tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetVirtualOffset, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetVirtualOffset) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *x = tag->uint32Values[0];
            *y = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 테스트 결과 값은 반영되나 실제로 화면에 보이는 상태는 바뀌지 않는다.
 * @param x
 * @param y
 * @return ResultCode
 */
ResultCode FrameBuffer::setVirtualOffset(Uint32 * x, Uint32 * y) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // SetVirtualOffset tag
    params[0] = *x;
    params[1] = *y;
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetVirtualOffset, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetVirtualOffset) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *x = tag->uint32Values[0];
            *y = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * 설정할 때 넘기는 값과 다른 값이 나온다. 설정한 대로 화면에 나오는 것으로 볼 때 설정값이 틀린 것 같지는 않다.
 * 바로 전에 설정한 팔레트와 다른 값이 나온다. 하지만 일정한 값이 나오는 걸로 볼 때 가져올 때 포맷이 달라지는 것 같은데
 * 참고할 만한 문서가 없다.
 * 추리해보건데 R bit는 6 bit right shift 되고, G bit는 4 bit right shift되고 B bit는 상위 5비트만 남고 하위 3비트는 0으로 바뀐다.
 * 
 * @param palette must be Uint32[256] size or more.
 * @return ResultCode
 */
ResultCode FrameBuffer::getPalette(Uint32 * palette) {
    int index = 0;

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPalette tag
    index += mailboxProperty.fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPalette, 1024, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetPalette) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 1024)) {
            memcpy(palette, tag->uint32Values, 1024);
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * depth가 8인 경우에 사용되는 컬러 palette를 설정한다. 총 256개의 color가 있으며 
 * offset 부터 length개 만큼 설정해준다.
 * 
 * @param offset 설정할 palette의 offset.
 * @param length palette의 원소의 갯수.
 * @param palette 0x00RRGGBB 포맷의 color값의 배열이다.
 * @return ResultCode
 */
ResultCode FrameBuffer::setPalette(Uint32 offset, Uint32 length, Uint32 palette[]) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // SetPalette tag
    params[0] = offset;
    params[1] = length;
    MailboxPropertyTag * tag = (MailboxPropertyTag *)&(mpb->tags[index]);
    index += mailboxProperty.fillMailboxRequestTagInfo(tag, MailboxPropertyTagId::SetPalette, 8 + (length << 2), 2, params);
    memcpy(&(tag->uint32Values[2]), palette, length << 2);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(Mailbox::Channel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(Mailbox::Channel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetPalette) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && 
            ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            ResultCode result;
            if (tag->uint32Values[0] == 0) {
                mailbox.unlockBuffer();
                return ResultCode::Success;
            } else if (tag->uint32Values[0] == 1) {
                mailbox.unlockBuffer();
                return ResultCode::InvalidParameter;
            }
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}
