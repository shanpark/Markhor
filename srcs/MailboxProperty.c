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

#define BUFFER_SIZE  (4 * 1024)

Uint32 bufferForMailbox[BUFFER_SIZE >> 2] __attribute__((aligned(16)));

Uint32 getFirmwareRevision() {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetFirmwareRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetFirmwareRevision, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetFirmwareRevision) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4))
            return tag->uint32Values[0];
    }

    return -1;
}

Uint32 getBoardModel() {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardModel tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetBoardModel, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetBoardModel) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4))
            return tag->uint32Values[0];
    }

    return -1;
}

Uint32 getBoardRevision() {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetBoardRevision, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetBoardRevision) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4))
            return tag->uint32Values[0];
    }

    return -1;
}

int getBoardMacAddress(Uint8 bufForMac[6]) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetMacAddress tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetBoardMacAddress, 6, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetBoardMacAddress) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 6)) {
            bufForMac[0] = tag->uint8Values[0];
            bufForMac[1] = tag->uint8Values[1];
            bufForMac[2] = tag->uint8Values[2];
            bufForMac[3] = tag->uint8Values[3];
            bufForMac[4] = tag->uint8Values[4];
            bufForMac[5] = tag->uint8Values[5];
            return 0;
        }
    }

    return -1;
}

Uint64 getBoardSerial() {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetBoardSerial, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetBoardSerial) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8))
            return tag->uint64Values[0];
    }

    return -1;
}

int getArmMemory(Address * base, Uint64 * size) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetArmMemory, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetArmMemory) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *base = (Address)tag->uint32Values[0];
            *size = (Uint64)tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

int getVcMemory(Address * base, Uint64 * size) {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetVcMemory, 8, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetVcMemory) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            *base = (Address)tag->uint32Values[0];
            *size = (Uint64)tag->uint32Values[1];
            return 0;
        }
    }

    return -1;
}

int getClocks(int * clockCount, Clock clocks[]) {
    int index = 0;

    // too big buffer size. adjust to inner buffer size. 6 = buffer header + tag header + null tag
    if (*clockCount > ((BUFFER_SIZE - (6 * sizeof(Uint32))) / sizeof(Clock)))
        *clockCount = ((BUFFER_SIZE - (6 * sizeof(Uint32))) / sizeof(Clock));

    int bufSize = (*clockCount << 3);
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetClocks, bufSize, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetClocks) && 
            (tag->code & MPTCResponseTagCodeBit)) {
            Uint32 valueSize = tag->code & MPTCResponseTagCodeSizeBits;
            *clockCount = (valueSize >> 3);                
            if (valueSize <= bufSize) {
                for (int valueIndex = 0, clockIndex = 0 ; clockIndex < *clockCount ; clockIndex++) {
                    clocks[clockIndex].parentId = tag->uint32Values[valueIndex++];
                    clocks[clockIndex].id = tag->uint32Values[valueIndex++];
                }
                return 0;
            } else {
                return -2; // Not enough buffer space
            }
        }
    }

    return -1;
}

int getCommandLine(int * bufLen, char buffer[]) {
    int index = 0;

    // too big buffer size. adjust to inner buffer size. 6 = buffer header + tag header + null tag
    if (*bufLen > (BUFFER_SIZE - (6 * sizeof(Uint32))))
        *bufLen = (BUFFER_SIZE - (6 * sizeof(Uint32)));

    int bufSize = *bufLen & 0xfffffffc;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetCommandLine, bufSize, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetCommandLine) && 
            (tag->code & MPTCResponseTagCodeBit)) {
            Uint32 valueSize = tag->code & MPTCResponseTagCodeSizeBits;
            *bufLen = valueSize;
            if (valueSize <= bufSize) {
                memcpy(buffer, tag->uint8Values, valueSize);
                return 0;
            } else {
                *bufLen = ((valueSize + 3) & 0xfffffffc); // align 32bit.
                return -2; // Not enough buffer space
            }
        }
    }

    return -1;
}

Uint32 getDmaChannels() {
    int index = 0;
    
    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetFirmwareRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetDmaChannels, 4, 0, 0);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetDmaChannels) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 4))
            return tag->uint32Values[0];
    }

    return -1;
}

/**
 * @return device power state. -1 for failure.
 */
Uint32 getPowerState(DeviceId deviceId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = deviceId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetPowerState, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetPowerState) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @param deviceId
 * @return wait time required after turning a device on before power is stable. Returns 0 wait time if the device does not exist.
 *         -1 for failure.
 */
Uint32 getEnableWaitTime(DeviceId deviceId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetEnableWaitTime tag
    params[0] = deviceId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetEnableWaitTime, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetEnableWaitTime) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8))
            return tag->uint32Values[1];
    }

    return -1;
}

/**
 * UsbHcd를 제외하고 대부분 장치가 지원하지 않음. 게다가 UsbHcd 장치로 테스트해 보면 
 * 문서와 달리 request state값에서 Bit0의 값이 0일 때 On이 되고 1일 때 Off가 됨.
 * 
 * @param deviceId
 * @param state Bit 0: 0=off, 1=on. 문서에는 이렇게 되어있으나 0을 줘야 on상태로 바뀐다.???
 *              Bit 1: 0=do not wait, 1=wait
 *              Bits 2-31: reserved for future use (set to 0)
 * @return new state, with/without waiting for the power to become stable.
 */
Uint32 setPowerState(DeviceId deviceId, Uint32 state) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = deviceId;
    params[1] = state;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetPowerState, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetPowerState) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @return clock state. -1 for failure.
 */
Uint32 getClockState(ClockId clockId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetClockState, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetClockState) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @param clockId
 * @param state Bit 0: 0=off, 1=on. 
 *              Bit 1: 0=exists, 1=not exists
 *              Bits 2-31: reserved for future use (set to 0)
 * @return new clock state
 */
Uint32 setClockState(ClockId clockId, Uint32 state) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    params[1] = state;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetClockState, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetClockState) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @return clock rate in Hz. 0 if clock does not exist. -1 for failure.
 */
Uint32 getClockRate(ClockId clockId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetClockRate) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @param clockId
 * @param rate clock rate in hz
 * @param skipSettingTurbo By default when setting arm freq above default, other turbo settings 
 *        will be enabled (e.g. voltage, sdram and gpu frequencies). You can disable this effect
 *        by setting "skip setting turbo" to 1.
 * @return clock rate in hz.
 */
Uint32 setClockRate(ClockId clockId, Uint32 rate, Uint32 skipSettingTurbo) {
    int index = 0;
    Uint32 params[3];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    params[1] = rate;
    params[2] = skipSettingTurbo;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetClockRate, 12, 3, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetClockRate) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @return the maximum supported clock rate for the given clock. -1 for failure.
 */
Uint32 getMaxClockRate(ClockId clockId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetMaxClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetMaxClockRate) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @return the minimum supported clock rate for the given clock. -1 for failure.
 */
Uint32 getMinClockRate(ClockId clockId) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = clockId;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetMinClockRate, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetMinClockRate) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @return the turbo state for index id. level will be 0 for non-turbo and 1 for turbo. -1 for failure.
 */
Uint32 getTurbo(Uint32 id) {
    int index = 0;
    Uint32 params[1];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = id;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTIGetTurbo, 8, 1, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTIGetTurbo) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * @param id should be zero.
 * @param level 0 for non-turbo and 1 for turbo.
 * @return the turbo state for index id. level will be 0 for non-turbo and 1 for turbo. -1 for failure.
 */
Uint32 setTurbo(Uint32 id, Uint32 level) {
    int index = 0;
    Uint32 params[2];

    MailboxPropertyBuffer * mpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mpb->code = MPICRequestCode;

    // GetPowerState tag
    params[0] = id;
    params[1] = level;
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mpb->tags[index]), MPTISetTurbo, 8, 2, params);

    // End tag
    mpb->tags[index] = MPTIEnd;
    index++;

    mpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mpb);
    mpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mpb->code & MPICResponseCodeBit) && ((mpb->code & MPICResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mpb->tags[0]));
        if ((tag->id == MPTISetTurbo) && 
            (tag->code & MPTCResponseTagCodeBit) && ((tag->code & MPTCResponseTagCodeSizeBits) >= 8)) {
            return tag->uint32Values[1];
        }
    }

    return -1;
}

/**
 * Fill Mailbox request tag structure. Support only tags with 32 bit data values.
 * @return Total count of the filled 32bit data
 */
int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]) {
    tagBuf->id = id;
    tagBuf->size = (size + 3) & 0xfffffffcUL; // align size to 32bits.
    tagBuf->code = MPTCRequestTagCode;
    for (int inx = 0 ; inx < paramCount ; inx++)
        tagBuf->uint32Values[inx] = param[inx];

    return (3 + (tagBuf->size >> 2));
}
