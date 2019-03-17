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

#ifndef __CLIB_H
#define __CLIB_H

#include <cstdarg>
#include "MarkhorTypes.h"

inline bool isdigit(int n) {
    return ((n >= '0') && (n <= '9')) ? true : false;
}

/**
 * This is for signed integer type.
 * The result is right aligned in the buffer. And 
 * the address of the first character is returned.
 *
 * @param n
 * @param buf
 * @param bufLen
 * @param base
 * @return pointer to converted string. nullptr if failed.
 */
template<typename T>
char * itoa(T n, char * buf, int bufLen, int base) {
    int digit;
    char * p = buf + bufLen - 1;
    *p = '\0'; // points to terminating '\0'
    if (n >= 0) {
        do {
            digit = static_cast<int>((n % static_cast<T>(base)));
            if (digit < 0xA)
			    *--p = '0' + digit;
    		else
    			*--p = 'a' + digit - 0xA;
            n /= base;
        } while ((n != 0) && (p > buf));
    } else { // i < 0
        do {
            digit = (n % base);
            if (digit < 0xA)
			    *--p = '0' - digit;
    		else
    			*--p = 'a' - digit - 0xA;
            n /= base;
        } while ((n != 0) && (p > (buf + 1)));
        *--p = '-';
    }

    return (n == 0) ? p : nullptr;
}

/**
 * This is for unsigned integer type.
 * The result is right aligned in the buffer. And 
 * the address of the first character is returned.
 *
 * @param n
 * @param buf
 * @param bufLen
 * @param base
 * @return pointer to converted string. nullptr if failed.
 */
template<typename T>
char * utoa(T n, char * buf, int bufLen, int base) {
    int digit;
    char * p = buf + bufLen - 1;
    *p = '\0'; // points to terminating '\0'
    do {
        digit = static_cast<int>((n % static_cast<T>(base)));
        if (digit < 0xA)
            *--p = '0' + digit;
        else
            *--p = 'a' + digit - 0xA;
        n /= base;
    } while ((n != 0) && (p > buf));

    return (n == 0) ? p : nullptr;
}

extern int vsprintf(char * buf, const char * format, va_list ap);
extern int sprintf(char * buf, const char * format, ...);

#endif /* __CLIB_H */
