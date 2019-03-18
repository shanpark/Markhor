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

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

extern void enableInterruptRequest(void);
extern void enableFastInterruptRequest(void);

extern "C" {
    void reset(void) __attribute__((interrupt("ABORT")));
    void undefinedInstruction(void) __attribute__((interrupt("UNDEF")));
    void software(void) __attribute__((interrupt("SWI")));
    void prefetchAbort(void) __attribute__((interrupt("ABORT")));
    void dataAbort(void) __attribute__((interrupt("ABORT")));
    void interruptRequest(void) __attribute__((interrupt("IRQ")));
    void fastInterruptRequest(void) __attribute__((interrupt("FIQ")));
}

#endif /* __INTERRUPT_H */