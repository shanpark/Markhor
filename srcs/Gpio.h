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

#ifndef __GPIO_H
#define __GPIO_H

#include "MarkhorTypes.h"
#include "IoPeripherals.h"

#define GPIO_BASE       (IO_PERIPHERALS_BASE + 0x200000UL)

#define GPFSEL_BASE     (GPIO_BASE) // GPIO Function Select

#define GPSET_BASE      (GPIO_BASE + 0x1cUL) // GPIO Pin Output Set
#define GPCLR_BASE      (GPIO_BASE + 0x28UL) // GPIO Pin Output Clear
#define GPLEV_BASE      (GPIO_BASE + 0x34UL) // GPIO Pin Level
#define GPEDS_BASE      (GPIO_BASE + 0x40UL) // GPIO Pin Event Detect Status
#define GPREN_BASE      (GPIO_BASE + 0x4cUL) // GPIO Pin Rising Edge Detect Enable
#define GPFEN_BASE      (GPIO_BASE + 0x58UL) // GPIO Pin Falling Edge Detect Enable
#define GPHEN_BASE      (GPIO_BASE + 0x64UL) // GPIO Pin High Detect Enable
#define GPLEN_BASE      (GPIO_BASE + 0x70UL) // GPIO Pin Low Detect Enable
#define GPAREN_BASE     (GPIO_BASE + 0x7cUL) // GPIO Pin Async. Rising Edge Detect
#define GPAFEN_BASE     (GPIO_BASE + 0x88UL) // GPIO Pin Async. Falling Edge Detect
#define GPPUD_BASE      (GPIO_BASE + 0x94UL) // GPIO Pin Pull-up/down Enable
#define GPPUDCLK_BASE   (GPIO_BASE + 0x98UL) // GPIO Pin Pull-up/down Enable Clock

typedef Uint32 GpioRegister;

extern int GpioSelectFunction(Uint32 pin, Uint32 func);
extern int GpioSetOutputPin(Uint32 pin);
extern int GpioClearOutputPin(Uint32 pin);
extern int GpioGetPinValue(Uint32 pin);

#endif /* __GPIO_H */