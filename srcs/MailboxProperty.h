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

extern Uint32 bufferForMailbox[1024] __attribute__((aligned(16)));

extern Uint32 getFirmwareRevision();
extern Uint32 getBoardModel();
extern Uint32 getBoardRevision();
extern int getBoardMacAddress(Uint8 bufForMac[6]);
extern Uint64 getBoardSerial();

extern int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]);

#endif /* __MAILBOX_PROPERTY_H */
