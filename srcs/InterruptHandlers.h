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

#ifndef __INTERRUPT_HANDLERS_H
#define __INTERRUPT_HANDLERS_H

extern "C" {
    void resetHandler(void) __attribute__((interrupt("ABORT")));
    void undefinedInstructionHandler(void) __attribute__((interrupt("UNDEF")));
    void softwareInterruptHandler(void) __attribute__((interrupt("SWI")));
    void prefetchAbortHandler(void) __attribute__((interrupt("ABORT")));
    void dataAbortHandler(void) __attribute__((interrupt("ABORT")));
    void interruptRequestHandler(void) __attribute__((interrupt("IRQ")));
    void fastInterruptRequestHandler(void) __attribute__((interrupt("FIQ")));
}

#endif /* __INTERRUPT_HANDLERS_H */