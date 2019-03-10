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

Uint32 bufferForMailbox[1024] __attribute__((aligned(16)));

/**
 * Read data from Mailbox.
 * https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * 
 * @param channel target channel
 * @return read data if successful, else -1.
 */
Uint32 readFromMailbox(MailboxChannel channel) {
    if (channel > 0x0f)
        return -1; // 0xffffffff

    volatile MailboxRegister * mailboxRegister = (MailboxRegister *)MAILBOX_BASE;

    while (1) {
        // wait untile mailbox becomes available
        while ((mailboxRegister[READ_STATUS] & MAIL_EMPTY) != 0)
            ;

        // read the value
		Uint32 data = mailboxRegister[READ_DATA];
		int readChannel = data & 0x0f;

		if (readChannel == channel)
			return data & 0xfffffff0UL;
    }
}

/**
 * Write data to Mailbox.
 * https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * 
 * @param channel target channel
 * @param data data to write.
 */
void writeToMailbox(MailboxChannel channel, Uint32 data) {
    if (channel > 0x0f)
        return;

    volatile MailboxRegister * mailboxRegister = (MailboxRegister *)MAILBOX_BASE;

    // set channel
    data &= ~(0x0f);
    data |= channel;

    // wait until the mailbox becomes available
    while ((mailboxRegister[WRITE_STATUS] & MAIL_FULL) != 0)
        ;

    // write the value to the requested channel
    mailboxRegister[WRITE_DATA] = data;
}

/**
 * Fill Mailbox request tag structure. Support only tags with 32 bit data values.
 * @return Total count of the filled 32bit data
 */
int fillMailboxRequestTagInfo(MailboxPropertyTag *tagBuf, Uint32 id, Uint32 size, Uint32 paramCount, Uint32 param[]) {
    tagBuf->id = id;
    tagBuf->size = size;
    tagBuf->code = 0x00000000; // request
    for (int inx = 0 ; inx < paramCount ; inx++)
        tagBuf->uint32Values[inx] = param[inx];

    return (3 + (size >> 2));
}
