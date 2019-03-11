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
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = RequestCode; // request

    // GetFirmwareRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetFirmwareRevision, 4, 0, 0);

    // End tag
    mbpb->tags[index] = End;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & ResponseCodeBit) && ((mbpb->code & ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[0]));
        if (tag->id == GetFirmwareRevision)
            return tag->uint32Values[0];
    }

    return -1;
}

Uint32 getBoardModel() {
    int index = 0;
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = RequestCode; // request

    // GetBoardModel tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetBoardModel, 4, 0, 0);

    // End tag
    mbpb->tags[index] = End;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & ResponseCodeBit) && ((mbpb->code & ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[0]));
        if (tag->id == GetBoardModel)
            return tag->uint32Values[0];
    }

    return -1;
}

Uint32 getBoardRevision() {
    int index = 0;
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = RequestCode; // request

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetBoardRevision, 4, 0, 0);

    // End tag
    mbpb->tags[index] = End;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & ResponseCodeBit) && ((mbpb->code & ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[0]));
        if (tag->id == GetBoardRevision)
            return tag->uint32Values[0];
    }

    return -1;
}

int getBoardMacAddress(Uint8 bufForMac[6]) {
    int index = 0;
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = RequestCode; // request

    // GetMacAddress tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetBoardMacAddress, 6, 0, 0);

    // End tag
    mbpb->tags[index] = End;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & ResponseCodeBit) && ((mbpb->code & ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[0]));
        if (tag->id == GetBoardMacAddress) {
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
    
    MailboxPropertyBuffer * mbpb = (MailboxPropertyBuffer *)bufferForMailbox;
    mbpb->code = RequestCode; // request

    // GetBoardRevision tag
    index += fillMailboxRequestTagInfo((MailboxPropertyTag *)&(mbpb->tags[index]), GetBoardSerial, 8, 0, 0);

    // End tag
    mbpb->tags[index] = End;
    index++;

    mbpb->size = ((index + 2) << 2);

    writeToMailbox(CH_PROPERTY_TAGS_ARM_TO_VC, (Uint32)mbpb);
    mbpb = (MailboxPropertyBuffer *)readFromMailbox(CH_PROPERTY_TAGS_ARM_TO_VC);

    if ((mbpb->code & ResponseCodeBit) && ((mbpb->code & ResponseCodeErrorBit) == 0)) {
        MailboxPropertyTag * tag = ((MailboxPropertyTag *)&(mbpb->tags[0]));
        if (tag->id == GetBoardSerial)
            return tag->uint64Values[0];
    }

    return -1;
}

/**
 * Fill Mailbox request tag structure. Support only tags with 32 bit data values.
 * @return Total count of the filled 32bit data
 */
int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]) {
    int padding = (4 - (size & 0x03)) & 0x03;
    tagBuf->id = id;
    tagBuf->size = size + padding;
    tagBuf->code = 0x00000000; // request
    for (int inx = 0 ; inx < paramCount ; inx++)
        tagBuf->uint32Values[inx] = param[inx];

    return (3 + (tagBuf->size >> 2));
}
