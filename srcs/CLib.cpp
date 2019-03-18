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

#include <cstring> // library code size = 820 bytes. And the provided functions are great.
#include "CLib.h"

/**
 * Simple version of vsprintf()
 * Following format specifiers are supported.
 * format specifier: %, c, s, d, i, o, X, u
 * 
 * @param buf
 * @param format
 * @param vlist
 * @return the number of written bytes.
 */
int vsprintf(char * buf, const char * format, va_list vlist) {
	const char * t;
	char temp[30]; // 30 = enough space.

	char * d = buf;
	while (*format) {
		if (*format == '%') {
			format++;
            if (*format == '%') {
                *d++ = *format++;
            } else if (*format == 'c') {
				*d++ = va_arg(vlist, int);
                format++;
            } else if (*format == 'o') {
                t = utoa(va_arg(vlist, unsigned int), temp, sizeof(temp), 8);
                strcpy(d, t);
                d += sizeof(temp) - (t - temp) - 1;
                format++;
            } else if ((*format == 'i') || (*format == 'd')) {
                t = itoa(va_arg(vlist, int), temp, sizeof(temp), 10);
                strcpy(d, t);
                d += sizeof(temp) - (t - temp) - 1;
                format++;
            } else if (*format == 'u') {
                t = utoa(va_arg(vlist, unsigned int), temp, sizeof(temp), 10);
                strcpy(d, t);
                d += sizeof(temp) - (t - temp) - 1;
                format++;
            } else if ((*format == 'x') || (*format == 'X')) {
                t = utoa(va_arg(vlist, unsigned int), temp, sizeof(temp), 16);
                strcpy(d, t);
                d += sizeof(temp) - (t - temp) - 1;
                format++;
            } else if (*format == 's') {
                strcpy(d, va_arg(vlist, char *));
                d += strlen(d);
                format++;
            }
		} else {
			*d++ = *format++;
        }
	}

	*d = '\0';
	return (d - buf);
}

/**
 * Simple version of sprintf()
 * Following format specifiers are supported.
 * format specifier: %, c, s, d, i, o, X, u
 * 
 * @param buf
 * @param format
 * @return the number of written bytes.
 */
int sprintf(char * buf, const char * format, ...) {
    va_list arg;
    
    va_start(arg, format);
    int written = vsprintf(buf, format, arg);
    va_end(arg);

    return written;
}
