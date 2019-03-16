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

#include "IoPeripherals.h"
#include "Mailbox.h"

#define MAILBOX_BASE   (IO_PERIPHERALS_BASE + 0xb880UL)

/* https://github.com/raspberrypi/firmware/wiki/Mailboxes */
#define READ_DATA       0
#define READ_PEEK       4
#define READ_SENDER     5
#define READ_STATUS     6
#define READ_CONFIG     7
#define WRITE_DATA      8
#define WRITE_PEEK      12
#define WRITE_SENDER    13
#define WRITE_STATUS    14
#define WRITE_CONFIG    15

#define MAIL_FULL_BIT   0x80000000
#define MAIL_EMPTY_BIT  0x40000000

typedef Uint32 MailboxRegister;

Uint32 Mailbox::buffer[BUFFER_SIZE >> 2] __attribute__((aligned(16)));

Mailbox mailbox;

/**
 * Read data from Mailbox.
 * https://github.com/raspberrypi/firmware/wiki/Accessing-mailboxes
 * 
 * @param channel target channel
 * @return read data if successful, else -1.
 */
Uint32 Mailbox::read(MailboxChannel channel) {
    volatile MailboxRegister * mailboxRegister = (MailboxRegister *)MAILBOX_BASE;

    while (1) {
        // wait untile mailbox becomes available
        while ((mailboxRegister[READ_STATUS] & MAIL_EMPTY_BIT) != 0)
            ;

        // read the value
		Uint32 data = mailboxRegister[READ_DATA];
		int readChannel = data & 0x0f;

		if (readChannel == static_cast<int>(channel))
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
void Mailbox::write(MailboxChannel channel, Uint32 data) {
    volatile MailboxRegister * mailboxRegister = (MailboxRegister *)MAILBOX_BASE;

    // set channel
    data &= ~(0x0f);
    data |= static_cast<int>(channel);

    // wait until the mailbox becomes available
    while ((mailboxRegister[WRITE_STATUS] & MAIL_FULL_BIT) != 0)
        ;

    // write the value to the requested channel
    mailboxRegister[WRITE_DATA] = data;
}
