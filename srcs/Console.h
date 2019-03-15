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

#ifndef __CONSOLE_H
#define __CONSOLE_H

class Console {
private:
    class Cursor {
    public:
        Cursor() : x(0), y(0) {};

        int x;
        int y;
    };

    Cursor cursor;
    int width, height;

public:
    int init();
    int write(char * str, int length);

private:
    void gotoNewLine();
};

extern Console console;

#endif /* __CONSOLE_H */
