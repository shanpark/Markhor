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

extern int __bss_start__;
extern int __bss_end__;

extern void Markhor(void);

__attribute__((naked))  void StartUp(void) {
    int* pos = &__bss_start__;
    int* end = &__bss_end__;

    /* clear .bss section */
    while (pos < end)
        *pos++ = 0;

    /* kernel main */
    Markhor();

    while (1)
        ;
}
