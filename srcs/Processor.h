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

#ifndef __PROCESSOR_H
#define __PROCESSOR_H

class Processor {
public:
    enum class Mode {
        User        = 0x10,
        Fiq         = 0x11,
        Irq         = 0x12,
        Supervisor  = 0x13,
        Monitor     = 0x16,
        Abort       = 0x17,
        Hyp         = 0x1a,
        Undefined   = 0x1b,
        System      = 0x1f
    };

    void changeMode(Mode mode);
};

#endif /* __PROCESSOR_H */
