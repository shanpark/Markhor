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

#ifndef __MAILBOX_H
#define __MAILBOX_H

#include "MarkhorTypes.h"
#include "IoPeripherals.h"

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

// This bit is set in the status register if there is no space to write into the mailbox
#define MAIL_FULL       0x80000000
// This bit is set in the status register if there is nothing to read from the mailbox
#define MAIL_EMPTY      0x40000000

typedef enum {
    CH_POWER_MANAGEMENT = 0,
    CH_FRAMEBUFFER = 1, // deprecated.
    CH_VIRTUAL_UART = 2,
    CH_VCHIQ = 3,
    CH_LEDS = 4,
    CH_BUTTONS = 5,
    CH_TOUCHSCREEN = 6,
    CH_UNUSED = 7,
    CH_PROPERTY_TAGS_ARM_TO_VC = 8,
    CH_PROPERTY_TAGS_VC_TO_ARM = 9
} MailboxChannel;

typedef Uint32 MailboxRegister;

extern Uint32 readFromMailbox(MailboxChannel channel);
extern void writeToMailbox(MailboxChannel channel, Uint32 data);

#endif /* __MAILBOX_H */