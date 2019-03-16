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

#ifndef __MAILBOX_PROPERTY_H
#define __MAILBOX_PROPERTY_H

#include "MarkhorTypes.h"
#include "ResultCode.h"

enum class MailboxPropertyTagId : Uint32 {
    GetFirmwareRevision = 0x00000001,

    GetBoardModel       = 0x00010001,
    GetBoardRevision    = 0x00010002,
    GetBoardMacAddress  = 0x00010003,
    GetBoardSerial      = 0x00010004,
    GetArmMemory        = 0x00010005,
    GetVcMemory         = 0x00010006,
    GetClocks           = 0x00010007,

    GetPowerState       = 0x00020001,
    GetEnableWaitTime   = 0x00020002,
    SetPowerState       = 0x00028001,

    GetClockState       = 0x00030001,
    GetClockRate        = 0x00030002,
    GetVoltage          = 0x00030003,
    GetMaxClockRate     = 0x00030004,
    GetMaxVoltage       = 0x00030005,
    GetTemperature      = 0x00030006,
    GetMinClockRate     = 0x00030007,
    GetMinVoltage       = 0x00030008,
    GetTurbo            = 0x00030009,
    GetMaxTemperature   = 0x0003000a,
    SetClockState       = 0x00038001,
    SetClockRate        = 0x00038002,
    SetVoltage          = 0x00038003,
    SetTurbo            = 0x00038009,

    GetCommandLine      = 0x00050001,

    GetDmaChannels      = 0x00060001,
    
    SetCursorInfo       = 0x00008010,
    SetCursorState      = 0x00008011,

    AllocateFrameBuffer = 0x00040001,
    BlankScreen         = 0x00040002,
    GetPhysicalWH       = 0x00040003,
    GetVirtualWH        = 0x00040004,
    GetDepth            = 0x00040005,
    GetPixelOrder       = 0x00040006,
    GetAlphaMode        = 0x00040007,
    GetPitch            = 0x00040008,
    GetVirtualOffset    = 0x00040009,
    GetOverscan         = 0x0004000a,
    GetPalette          = 0x0004000b,

    TestPhysicalWH      = 0x00044003,
    TestVirtualWH       = 0x00044004,
    TestDepth           = 0x00044005,
    TestPixelOrder      = 0x00044006,
    TestAlphaMode       = 0x00044007,
    TestVirtualOffset   = 0x00044009,
    TestOverscan        = 0x0004400a,
    TestPalette         = 0x0004400b,

    ReleaseFrameBuffer  = 0x00048001,
    SetPhysicalWH       = 0x00048003,
    SetVirtualWH        = 0x00048004,
    SetDepth            = 0x00048005,
    SetPixelOrder       = 0x00048006,
    SetAlphaMode        = 0x00048007,
    SetVirtualOffset    = 0x00048009,
    SetOverscan         = 0x0004800a,
    SetPalette          = 0x0004800b,

    End                 = 0x00000000
};

enum class MailboxPropertyInterfaceCode : Uint32 {
    RequestCode             = 0x00000000,
    ResponseCodeBit         = 0x80000000,
    ResponseCodeErrorBit    = 0x00000001
};

enum class MailboxPropertyTagCode : Uint32 {
    RequestTagCode          = 0x00000000,
    ResponseTagCodeBit      = 0x80000000,
    ResponseTagCodeSizeBits = 0x7fffffff
};

enum class ClockId : Uint32 {
    CIRoot   = 0x00000000, /* Not exists */
    CIEmmc   = 0x00000001,
    CIUart   = 0x00000002,
    CIArm    = 0x00000003,
    CICore   = 0x00000004,
    CIV3d    = 0x00000005,
    CIH264   = 0x00000006,
    CIIsp    = 0x00000007,
    CISdram  = 0x00000008,
    CIPixel  = 0x00000009,
    CIPwm    = 0x0000000a,
};

enum class DMAChannel : Uint32 {
    DMA0    = 0x00000001 << 0,
    DMA1    = 0x00000001 << 1,
    DMA2    = 0x00000001 << 2,
    DMA3    = 0x00000001 << 3,
    DMA4    = 0x00000001 << 4,
    DMA5    = 0x00000001 << 5,
    DMA6    = 0x00000001 << 6,
    DMA7    = 0x00000001 << 7,
    DMA8    = 0x00000001 << 8,
    DMA9    = 0x00000001 << 9,
    DMA10   = 0x00000001 << 10,
    DMA11   = 0x00000001 << 11,
    DMA12   = 0x00000001 << 12,
    DMA13   = 0x00000001 << 13,
    DMA14   = 0x00000001 << 14,
    DMA15   = 0x00000001 << 15
};

enum class DeviceId : Uint32 {
    DISdCard  = 0x00000000,
    DIUart0   = 0x00000001,
    DIUart1   = 0x00000002,
    DIUsbHcd  = 0x00000003,
    DII2c0    = 0x00000004,
    DII2c1    = 0x00000005,
    DII2c2    = 0x00000006,
    DISpi     = 0x00000007,
    DICcp2tx  = 0x00000008,
};

enum class PowerState : Uint32 {
    PSOnBit         = 0x00000001,
    PSNotExistBit   = 0x00000002,
    PSWaitBit       = 0x00000002
} ;

enum class ClockState : Uint32 {
    CSOnBit         = 0x00000001,
    CSNotExistBit   = 0x00000002
};

enum class VoltageId : Uint32 {
    VICore      = 0x00000001,
    VISdramC    = 0x00000002,
    VISdramP    = 0x00000003,
    VISdramI    = 0x00000004
};

#pragma pack(push, 4)

typedef struct {
    ClockId parentId;
    ClockId id;
} Clock;

typedef struct {
    Uint32 size;
    MailboxPropertyInterfaceCode code;
    Uint32 tags[0]; // all tags are aligned to 4 bytes.
} MailboxPropertyBuffer;

typedef struct {
    MailboxPropertyTagId id;
    Uint32 size;
    MailboxPropertyTagCode code;
    union {
        Uint8 uint8Values[0];
        Uint32 uint32Values[0];
        Uint64 uint64Values[0];
    };
} MailboxPropertyTag;

#pragma pack(pop)

inline Uint32 operator&(const MailboxPropertyInterfaceCode & lhs, const MailboxPropertyInterfaceCode & rhs) {
    return (static_cast<Uint32>(lhs) & static_cast<Uint32>(rhs));
}

inline Uint32 operator&(const MailboxPropertyTagCode & lhs, const MailboxPropertyTagCode & rhs) {
    return (static_cast<Uint32>(lhs) & static_cast<Uint32>(rhs));
}

class MailboxProperty {
public:
    ResultCode getFirmwareRevision(Uint32 * firmwareVersion);
    ResultCode getBoardModel(Uint32 * boardModel);
    ResultCode getBoardRevision(Uint32 * boardRevision);
    ResultCode getBoardMacAddress(Uint8 bufForMac[6]);
    ResultCode getBoardSerial(Uint64 * boardSerial);
    ResultCode getArmMemory(Address * base, Uint64 * size);
    ResultCode getVcMemory(Address * base, Uint64 * size);
    ResultCode getClocks(int * clockCount, Clock clocks[]);
    ResultCode getCommandLine(int * bufLen, char buffer[]);
    ResultCode getDmaChannels(Uint32 & dmaChannel);
    ResultCode getPowerState(DeviceId deviceId, Uint32 * powerState);
    ResultCode getEnableWaitTime(DeviceId deviceId, Uint32 * enableWaitTime);
    ResultCode setPowerState(DeviceId deviceId, Uint32 * state);
    ResultCode getClockState(ClockId clockId, Uint32 * clockState);
    ResultCode setClockState(ClockId clockId, Uint32 * state);
    ResultCode getClockRate(ClockId clockId, Uint32 * clockRate);
    ResultCode setClockRate(ClockId clockId, Uint32 * rate, Uint32 skipSettingTurbo);
    ResultCode getMaxClockRate(ClockId clockId, Uint32 * maxClockRate);
    ResultCode getMinClockRate(ClockId clockId, Uint32 * minClockRate);
    ResultCode getTurbo(Uint32 id, Uint32 * level);
    ResultCode setTurbo(Uint32 id, Uint32 * level);
    ResultCode getVoltage(VoltageId voltageId, Uint32 * voltage);
    ResultCode setVoltage(VoltageId voltageId, Uint32 * voltage);
    ResultCode getMaxVoltage(VoltageId voltageId, Uint32 * maxVoltage);
    ResultCode getMinVoltage(VoltageId voltageId, Uint32 * minVoltage);
    ResultCode getTemperature(Uint32 temperatureId, Uint32 * temperature);
    ResultCode getMaxTemperature(Uint32 temperatureId, Uint32 * maxTemperature);

    static int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, MailboxPropertyTagId id, Uint32 size, Uint32 paramCount, Uint32 param[]);
};

extern MailboxProperty mailboxProperty;

#endif /* __MAILBOX_PROPERTY_H */
