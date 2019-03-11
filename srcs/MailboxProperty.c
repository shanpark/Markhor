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

Uint32 bufferForMailbox[1024] __attribute__((aligned(16)));

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
