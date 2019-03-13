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

#include "Gpio.h"

/**
 * selects a function of a GPIO pins.
 * 
 * @param pin Pin number. (0 ~ 53)
 * @param func Function to seelct (0 ~ 7)
 * @return 0 if successful. else -1.
 */
int gpioSelectFunction(Uint32 pin, GpioPinFunction func) {
    if ((pin > 54) || (func > 8))
        return -1;

    GpioRegister * gpfselRegister = &((GpioRegister *)GPFSEL_BASE)[pin / 10];
    GpioRegister temp = *gpfselRegister;
    temp &= ~(0x00000007 << ((pin % 10) * 3));
    temp |= (func << ((pin % 10) * 3));
    *gpfselRegister = temp;

    return 0;
}

/**
 * set a GPIO pin.
 * 
 * @param pin Pin number. (0 ~ 53)
 * @return 0 if successful. else -1.
 */
int gpioSetOutputPin(Uint32 pin) {
    if (pin > 54)
        return -1;

    GpioRegister * gpsetRegister = &((GpioRegister *)GPSET_BASE)[pin / 32];
    *gpsetRegister = 1 << (pin % 32);

    return 0;
}

/**
 * clear a GPIO pin.
 * 
 * @param pin Pin number. (0 ~ 53)
 * @return 0 if successful. else -1.
 */
int gpioClearOutputPin(Uint32 pin) {
    if (pin > 54)
        return -1;

    GpioRegister * gpclrRegister = &((GpioRegister *)GPCLR_BASE)[pin / 32];
    *gpclrRegister = 1 << (pin % 32);

    return 0;
}

/**
 * return the actual value of the GPIO pin.
 * 
 * @param pin Pin number. (0 ~ 53)
 * @return 1 = high, 0 = low. -1 = failure.
 */
GpioPinValue gpioGetPinValue(Uint32 pin) {
    if (pin > 54)
        return GPVErr;

    GpioRegister * gplevRegister = &((GpioRegister *)GPLEV_BASE)[pin / 32];
    if (*gplevRegister & (1 << (pin % 32)))
        return GPVHi;

    return GPVLo;
}
