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

#pragma pack(push, 4)

typedef struct {
    Uint32 size;
    Uint32 code;
    Uint32 tags[0];
} MailboxPropertyBuffer;

typedef struct {
    Uint32 id;
    Uint32 size;
    Uint32 code;
    union {
        Uint8 uint8Values[0];
        Uint32 uint32Values[0];
        Uint64 uint64Values[0];
    };
} MailboxPropertyTag;

#pragma pack(pop)

typedef enum {
    MPTIGetFirmwareRevision = 0x00000001,

    MPTIGetBoardModel       = 0x00010001,
    MPTIGetBoardRevision    = 0x00010002,
    MPTIGetBoardMacAddress  = 0x00010003,
    MPTIGetBoardSerial      = 0x00010004,
    MPTIGetArmMemory        = 0x00010005,
    MPTIGetVcMemory         = 0x00010006,
    MPTIGetClocks           = 0x00010007,

    MPTIGetPowerState       = 0x00020001,
    MPTIGetEnableWaitTime   = 0x00020002,
    MPTISetPowerState       = 0x00028001,

    MPTIGetClockState       = 0x00030001,
    MPTIGetClockRate        = 0x00030002,
    MPTIGetVoltage          = 0x00030003,
    MPTIGetMaxClockRate     = 0x00030004,
    MPTIGetMaxVoltage       = 0x00030005,
    MPTIGetTemperature      = 0x00030006,
    MPTIGetMinClockRate     = 0x00030007,
    MPTIGetMinVoltage       = 0x00030008,
    MPTIGetTurbo            = 0x00030009,
    MPTIGetMaxTemperature   = 0x0003000a,
    MPTISetClockState       = 0x00038001,
    MPTISetClockRate        = 0x00038002,
    MPTISetVoltage          = 0x00038003,
    MPTISetTurbo            = 0x00038009,

    MPTIGetCommandLine      = 0x00050001,

    MPTIGetDmaChannels      = 0x00060001,
    
    MPTISetCursorInfo       = 0x00008010,
    MPTISetCursorState      = 0x00008011,

    MPTIAllocateFrameBuffer = 0x00040001,
    MPTIBlankScreen         = 0x00040002,
    MPTIGetPhysicalWH       = 0x00040003,
    MPTIGetVirtualWH        = 0x00040004,
    MPTIGetDepth            = 0x00040005,
    MPTIGetPixelOrder       = 0x00040006,
    MPTIGetAlphaMode        = 0x00040007,
    MPTIGetPitch            = 0x00040008,
    MPTIGetVirtualOffset    = 0x00040009,
    MPTIGetOverscan         = 0x0004000a,
    MPTIGetPalette          = 0x0004000b,

    MPTITestPhysicalWH      = 0x00044003,
    MPTITestVirtualWH       = 0x00044004,
    MPTITestDepth           = 0x00044005,
    MPTITestPixelOrder      = 0x00044006,
    MPTITestAlphaMode       = 0x00044007,
    MPTITestVirtualOffset   = 0x00044009,
    MPTITestOverscan        = 0x0004400a,
    MPTITestPalette         = 0x0004400b,

    MPTIReleaseFrameBuffer  = 0x00048001,
    MPTISetPhysicalWH       = 0x00048003,
    MPTISetVirtualWH        = 0x00048004,
    MPTISetDepth            = 0x00048005,
    MPTISetPixelOrder       = 0x00048006,
    MPTISetAlphaMode        = 0x00048007,
    MPTISetVirtualOffset    = 0x00048009,
    MPTISetOverscan         = 0x0004800a,
    MPTISetPalette          = 0x0004800b,

    MPTIEnd                 = 0x00000000
} MailboxPropertyTagId;

typedef enum {
    MPICRequestCode             = 0x00000000,
    MPICResponseCodeBit         = 0x80000000,
    MPICResponseCodeErrorBit    = 0x00000001
} MailboxPropertyInterfaceCode;

typedef enum {
    MPTCRequestTagCode          = 0x00000000,
    MPTCResponseTagCodeBit      = 0x80000000,
    MPTCResponseTagCodeSizeBits = 0x7fffffff
} MailboxPropertyTagCode;

typedef enum {
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
} ClockId;

typedef enum {
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
} DMAChannel;

typedef enum {
    DISdCard  = 0x00000000,
    DIUart0   = 0x00000001,
    DIUart1   = 0x00000002,
    DIUsbHcd  = 0x00000003,
    DII2c0    = 0x00000004,
    DII2c1    = 0x00000005,
    DII2c2    = 0x00000006,
    DISpi     = 0x00000007,
    DICcp2tx  = 0x00000008,
} DeviceId;

typedef enum {
    PSOnBit         = 0x00000001,
    PSNotExistBit   = 0x00000002,
    PSWaitBit       = 0x00000002
} PowerState;

typedef enum {
    CSOnBit         = 0x00000001,
    CSNotExistBit   = 0x00000002
} ClockState;

typedef enum {
    VICore      = 0x00000001,
    VISdramC    = 0x00000002,
    VISdramP    = 0x00000003,
    VISdramI    = 0x00000004,
} VoltageId;

typedef struct {
    Uint32 parentId;
    Uint32 id;
} Clock;

extern Uint32 bufferForMailbox[1024] __attribute__((aligned(16)));

#ifdef __cplusplus
extern "C"
{
#endif

extern Uint32 getFirmwareRevision();
extern Uint32 getBoardModel();
extern Uint32 getBoardRevision();
extern int getBoardMacAddress(Uint8 bufForMac[6]);
extern Uint64 getBoardSerial();
extern int getArmMemory(Address * base, Uint64 * size);
extern int getVcMemory(Address * base, Uint64 * size);
extern int getClocks(int * clockCount, Clock clocks[]);
extern int getCommandLine(int * bufLen, char buffer[]);
extern Uint32 getDmaChannels();
extern Uint32 getPowerState(DeviceId deviceId);
extern Uint32 getEnableWaitTime(DeviceId deviceId);
extern Uint32 setPowerState(DeviceId deviceId, Uint32 state);
extern Uint32 getClockState(ClockId clockId);
extern Uint32 setClockState(ClockId clockId, Uint32 state);
extern Uint32 getClockRate(ClockId clockId);
extern Uint32 setClockRate(ClockId clockId, Uint32 rate, Uint32 skipSettingTurbo);
extern Uint32 getMaxClockRate(ClockId clockId);
extern Uint32 getMinClockRate(ClockId clockId);
extern Uint32 getTurbo(Uint32 id);
extern Uint32 setTurbo(Uint32 id, Uint32 level);
extern Uint32 getVoltage(VoltageId voltageId);
extern Uint32 setVoltage(VoltageId voltageId, Uint32 voltage);
extern Uint32 getMaxVoltage(VoltageId voltageId);
extern Uint32 getMinVoltage(VoltageId voltageId);
extern Uint32 getTemperature(Uint32 temperatureId);
extern Uint32 getMaxTemperature(Uint32 temperatureId);

extern int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]);

#ifdef __cplusplus
}
#endif

#endif /* __MAILBOX_PROPERTY_H */
