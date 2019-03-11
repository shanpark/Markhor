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

typedef struct {
    Uint32 size;
    Uint32 code;
    Uint32 tags[1];
} MailboxPropertyBuffer;

typedef struct {
    Uint32 id;
    Uint32 size;
    Uint32 code;
    union {
        Uint8 uint8Values[4];
        Uint32 uint32Values[1];
        Uint64 uint64Values[0];
    };
} MailboxPropertyTag;

typedef enum {
    End                 = 0x00000000,
    
    GetFirmwareRevision = 0x00000001,

    GetBoardModel       = 0x00010001,
    GetBoardRevision    = 0x00010002,
    GetBoardMacAddress  = 0x00010003,
    GetBoardSerial      = 0x00010004,
    
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
    SetPalette          = 0x0004800b
} MailboxPropertyTagId;

typedef enum {
    RequestCode             = 0x00000000,
    ResponseCodeBit         = 0x80000000,
    ResponseCodeErrorBit    = 0x00000001
} MailboxPropertyTagCode;

extern Uint32 bufferForMailbox[1024] __attribute__((aligned(16)));

extern Uint32 getFirmwareRevision();
extern Uint32 getBoardModel();
extern Uint32 getBoardRevision();
extern int getBoardMacAddress(Uint8 bufForMac[6]);
extern Uint64 getBoardSerial();

extern int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]);

#endif /* __MAILBOX_PROPERTY_H */
