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

enum class MailboxChannel {
    PowerManagement     = 0,
    FrameBuffer         = 1, // deprecated.
    VirtualUart         = 2,
    Vchiq               = 3,
    Leds                = 4,
    Buttons             = 5,
    TouchScreen         = 6,
    Unused              = 7,
    PropertyTagsArmToVc = 8,
    PropertyTagsVcToArm = 9
};

class Mailbox {
private:
#define BUFFER_SIZE  (4 * 512)
    static Uint32 buffer[BUFFER_SIZE >> 2];

public:
    Uint32 *lockBuffer() { return buffer; /* TODO should implement lock system. */ }
    void unlockBuffer() { /* TODO should implement lock system. */ }

    Uint32 read(MailboxChannel channel);
    void write(MailboxChannel channel, Uint32 data);
};

extern Mailbox mailbox;

#endif /* __MAILBOX_H */
