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

MailboxProperty mailboxProperty;

ResultCode MailboxProperty::getFirmwareRevision(Uint32 * firmwareVersion) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetFirmwareRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetFirmwareRevision, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetFirmwareRevision) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4))
            *firmwareVersion = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getBoardModel(Uint32 * boardModel) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardModel tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetBoardModel, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetBoardModel) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4))
            *boardModel = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getBoardRevision(Uint32 * boardRevision) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetBoardRevision, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetBoardRevision) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4))
            *boardRevision = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getBoardMacAddress(Uint8 bufForMac[6]) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetMacAddress tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetBoardMacAddress, 6, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetBoardMacAddress) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 6)) {
            bufForMac[0] = tag->uint8Values[0];
            bufForMac[1] = tag->uint8Values[1];
            bufForMac[2] = tag->uint8Values[2];
            bufForMac[3] = tag->uint8Values[3];
            bufForMac[4] = tag->uint8Values[4];
            bufForMac[5] = tag->uint8Values[5];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getBoardSerial(Uint64 * boardSerial) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetBoardSerial, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetBoardSerial) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *boardSerial = tag->uint64Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getArmMemory(Address * base, Uint64 * size) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetArmMemory, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetArmMemory) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *base = (Address)tag->uint32Values[0];
            *size = (Uint64)tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getVcMemory(Address * base, Uint64 * size) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetVcMemory, 8, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetVcMemory) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *base = (Address)tag->uint32Values[0];
            *size = (Uint64)tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getClocks(int * clockCount, Clock clocks[]) {
    int index = 0;

    // too big buffer size. adjust to inner buffer size. 6 = buffer header + tag header + null tag
    if (*clockCount > ((BUFFER_SIZE - (6 * sizeof(Uint32))) / sizeof(Clock)))
        *clockCount = ((BUFFER_SIZE - (6 * sizeof(Uint32))) / sizeof(Clock));

    int bufSize = (*clockCount << 3);
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetClocks, bufSize, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetClocks) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit)) {
            Uint32 valueSize = tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits;
            *clockCount = (valueSize >> 3);                
            if (valueSize <= bufSize) {
                for (int valueIndex = 0, clockIndex = 0 ; clockIndex < *clockCount ; clockIndex++) {
                    clocks[clockIndex].parentId = static_cast<ClockId>(tag->uint32Values[valueIndex++]);
                    clocks[clockIndex].id = static_cast<ClockId>(tag->uint32Values[valueIndex++]);
                }
                mailbox.unlockBuffer();
                return ResultCode::Success;
            } else {
                mailbox.unlockBuffer();
                return ResultCode::AlreadyInUse; // Not enough buffer space
            }
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getCommandLine(int * bufLen, char buffer[]) {
    int index = 0;

    // too big buffer size. adjust to inner buffer size. 6 = buffer header + tag header + null tag
    if (*bufLen > (BUFFER_SIZE - (6 * sizeof(Uint32))))
        *bufLen = (BUFFER_SIZE - (6 * sizeof(Uint32)));

    int bufSize = *bufLen & 0xfffffffc;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetCommandLine, bufSize, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetCommandLine) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit)) {
            Uint32 valueSize = tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits;
            *bufLen = valueSize;
            if (valueSize <= bufSize) {
                memcpy(buffer, (char *)tag->uint8Values, valueSize);
                mailbox.unlockBuffer();
                return ResultCode::Success;
            } else {
                *bufLen = ((valueSize + 3) & 0xfffffffc); // align 32bit.
                mailbox.unlockBuffer();
                return ResultCode::AlreadyInUse; // Not enough buffer space
            }
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

ResultCode MailboxProperty::getDmaChannels(Uint32 & dmaChannel) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetFirmwareRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetDmaChannels, 4, 0, 0);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetDmaChannels) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 4)) {
            dmaChannel = tag->uint32Values[0];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return device power state. -1 for failure.
 */
ResultCode MailboxProperty::getPowerState(DeviceId deviceId, Uint32 * powerState) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(deviceId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetPowerState, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetPowerState) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *powerState = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param deviceId
 * @return wait time required after turning a device on before power is stable. Returns 0 wait time if the device does not exist.
 *         -1 for failure.
 */
ResultCode MailboxProperty::getEnableWaitTime(DeviceId deviceId, Uint32 * enableWaitTime) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetEnableWaitTime tag
    params[0] = static_cast<Uint32>(deviceId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetEnableWaitTime, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetEnableWaitTime) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *enableWaitTime = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * UsbHcd를 제외하고 대부분 장치가 지원하지 않음. 게다가 UsbHcd 장치로 테스트해 보면 
 * 문서와 달리 request state값에서 Bit0의 값이 0일 때 On이 되고 1일 때 Off가 됨.
 * 
 * @param deviceId
 * @param state Bit 0: 0=off, 1=on. (unlike the document, zero make it turn on.)
 *              Bit 1: 0=do not wait, 1=wait
 *              Bits 2-31: reserved for future use (set to 0)
 * @return new state, with/without waiting for the power to become stable.
 */
ResultCode MailboxProperty::setPowerState(DeviceId deviceId, Uint32 * state) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(deviceId);
    params[1] = *state;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetPowerState, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetPowerState) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *state = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return clock state. -1 for failure.
 */
ResultCode MailboxProperty::getClockState(ClockId clockId, Uint32 * clockState) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetClockState, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetClockState) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *clockState = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param clockId
 * @param state Bit 0: 0=off, 1=on. 
 *              Bit 1: 0=exists, 1=not exists
 *              Bits 2-31: reserved for future use (set to 0)
 * @return new clock state
 */
ResultCode MailboxProperty::setClockState(ClockId clockId, Uint32 * state) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    params[1] = *state;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetClockState, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetClockState) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *state = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return clock rate in Hz. 0 if clock does not exist. -1 for failure.
 */
ResultCode MailboxProperty::getClockRate(ClockId clockId, Uint32 * clockRate) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetClockRate) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *clockRate =  tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param clockId
 * @param rate clock rate in hz
 * @param skipSettingTurbo By default when setting arm freq above default, other turbo settings 
 *        will be enabled (e.g. voltage, sdram and gpu frequencies). You can disable this effect
 *        by setting "skip setting turbo" to 1.
 * @return clock rate in hz.
 */
ResultCode MailboxProperty::setClockRate(ClockId clockId, Uint32 * rate, Uint32 skipSettingTurbo) {
    int index = 0;
    Uint32 params[3];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    params[1] = *rate;
    params[2] = skipSettingTurbo;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetClockRate, 12, 3, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetClockRate) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *rate = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return the maximum supported clock rate for the given clock. -1 for failure.
 */
ResultCode MailboxProperty::getMaxClockRate(ClockId clockId, Uint32 * maxClockRate) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetMaxClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetMaxClockRate) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *maxClockRate = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return the minimum supported clock rate for the given clock. -1 for failure.
 */
ResultCode MailboxProperty::getMinClockRate(ClockId clockId, Uint32 * minClockRate) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(clockId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetMinClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetMinClockRate) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *minClockRate = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return the turbo state for index id. level will be 0 for non-turbo and 1 for turbo. -1 for failure.
 */
ResultCode MailboxProperty::getTurbo(Uint32 id, Uint32 * level) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = id;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetTurbo, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetTurbo) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *level = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param id should be zero.
 * @param level 0 for non-turbo and 1 for turbo.
 * @return the turbo state for index id. level will be 0 for non-turbo and 1 for turbo. -1 for failure.
 */
ResultCode MailboxProperty::setTurbo(Uint32 id, Uint32 * level) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = id;
    params[1] = *level;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetTurbo, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetTurbo) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *level = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return offset from 1.2V in units of 0.025V. 0x80000000 for invalid voltage id. -1 for failure.
 *         실제 테스트 결과. offset이 아니라 micro volt 단위의 값이 나오는 것으로 판단됨.
 */
ResultCode MailboxProperty::getVoltage(VoltageId voltageId, Uint32 * voltage) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(voltageId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetVoltage, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetVoltage) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *voltage = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param voltageId
 * @param voltage offset from 1.2V in units of 0.025V.
 * @return offset from 1.2V in units of 0.025V. 0x80000000 for invalid voltage id. -1 for failure.
 */
ResultCode MailboxProperty::setVoltage(VoltageId voltageId, Uint32 * voltage) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = static_cast<Uint32>(voltageId);
    params[1] = *voltage;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::SetVoltage, 8, 2, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::SetVoltage) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *voltage = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return the maximum supported voltage rate for the given id. -1 for failure.
 *         실제 테스트 결과. offset이 아니라 micro volt 단위의 값이 나오는 것으로 판단됨.
 */
ResultCode MailboxProperty::getMaxVoltage(VoltageId voltageId, Uint32 * maxVoltage) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetMaxVoltage tag
    params[0] = static_cast<Uint32>(voltageId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetMaxVoltage, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetMaxVoltage) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *maxVoltage = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @return the minimum supported voltage rate for the given id. -1 for failure.
 *         실제 테스트 결과. offset이 아니라 micro volt 단위의 값이 나오는 것으로 판단됨.
 */
ResultCode MailboxProperty::getMinVoltage(VoltageId voltageId, Uint32 * minVoltage) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetMinVoltage tag
    params[0] = static_cast<Uint32>(voltageId);
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetMinVoltage, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetMinVoltage) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *minVoltage = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * the temperature of the SoC
 * 
 * @param temperatureId should be zero.
 * @param temperature the temperature of the SoC. (degree * 1000)
 * @return Return the temperature of the SoC in thousandths of a degree C (degree * 1000). -1 for failure.
 */
ResultCode MailboxProperty::getTemperature(Uint32 temperatureId, Uint32 * temperature) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetPowerState tag
    params[0] = temperatureId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetTemperature, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetTemperature) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *temperature = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

/**
 * @param temperatureId should be zero.
 * @return Return the temperature of the SoC in thousandths of a degree C (degree * 1000). -1 for failure.
 */
ResultCode MailboxProperty::getMaxTemperature(Uint32 temperatureId, Uint32 * maxTemperature) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)mailbox.lockBuffer();
    if (mpb == nullptr)
        return ResultCode::AlreadyInUse;
    mpb->code = MailboxPropertyInterfaceCode::RequestCode;

    // GetMaxVoltage tag
    params[0] = temperatureId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MailboxPropertyTagId::GetMaxTemperature, 8, 1, params);

    // End tag
    mpb->tags[index] = static_cast<Uint32>(MailboxPropertyTagId::End);
    index++;

    mpb->size = ((index + 2) << 2);

    mailbox.write(MailboxChannel::PropertyTagsArmToVc, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)mailbox.read(MailboxChannel::PropertyTagsArmToVc);

    if ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeBit) && ((mpb->code & MailboxPropertyInterfaceCode::ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MailboxPropertyTagId::GetMaxTemperature) && 
            (tag->code & MailboxPropertyTagCode::ResponseTagCodeBit) && ((tag->code & MailboxPropertyTagCode::ResponseTagCodeSizeBits) >= 8)) {
            *maxTemperature = tag->uint32Values[1];
            mailbox.unlockBuffer();
            return ResultCode::Success;
        }
    }

    mailbox.unlockBuffer();
    return ResultCode::Fail;
}

// following functions not implemented.
// Allocate Memory
// Lock memory
// Unlock memory
// Release Memory
// Execute Code
// Get Dispmanx Resource mem handle 
// Get EDID block

/**
 * Fill Mailbox request tag structure. Support only tags with 32 bit data values.
 * @return Total count of the filled 32bit data
 */
int MailboxProperty::fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, MailboxPropertyTagId id, Uint32 size, Uint32 paramCount, Uint32 param[]) {
    tagBuf->id = id;
    tagBuf->size = (size + 3) & 0xfffffffcUL; // align size to 32bits.
    tagBuf->code = MailboxPropertyTagCode::RequestTagCode;
    for (int inx = 0 ; inx < paramCount ; inx++)
        tagBuf->uint32Values[inx] = param[inx];

    return (3 + (tagBuf->size >> 2));
}
