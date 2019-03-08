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

#include "MailBox.h"

/**
 * Read data from MailBox.
 * https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * 
 * @param channel target channel
 * @return read data if successful, else -1.
 */
Uint32 MailBoxRead(MailBoxChannel channel) {
    if (channel > 0x0f)
        return -1; // 0xffffffff

    MailBoxRegister * mailBoxRegister = (MailBoxRegister *)MAIL_BOX_BASE;

    while (1) {
        while ((mailBoxRegister[READ_STATUS] & MAIL_EMPTY) != 0)
            ;

        // Read the data
		Uint32 data = mailBoxRegister[READ_DATA];
		int readChannel = data & 0xF;

        // Return it straight away if it's for the requested channel
		if (readChannel == channel)
			return data & 0xfffffff0UL;
    }
}

/**
 * Write data to MailBox.
 * https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * 
 * @param channel target channel
 * @param data data to write.
 */
void MailBoxWrite(MailBoxChannel channel, Uint32 data) {
    if (channel > 0x0f)
        return;

    MailBoxRegister * mailBoxRegister = (MailBoxRegister *)MAIL_BOX_BASE;

    /* Add the channel number into the lower 4 bits */
    data &= ~(0x0f);
    data |= channel;

    /* Wait until the mailbox becomes available and then write to the mailbox channel */
    while ((mailBoxRegister[WRITE_STATUS] & MAIL_FULL) != 0)
        ;

    // Write the value to the requested channel
    mailBoxRegister[WRITE_DATA] = data;
}