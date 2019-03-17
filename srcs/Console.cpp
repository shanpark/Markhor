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

#include "TextEmul.h"
#include "Console.h"

#define COLOR   0x07

Console console;

ResultCode Console::init() {
    ResultCode resultCode = textEmul.setupTextMode();
    if (resultCode != ResultCode::Success)
        return resultCode;

    width = textEmul.getWidth();
    height = textEmul.getHeight();

    return ResultCode::Success;
}

int Console::write(const char * str, int length) {
    int inx;
    for (inx = 0 ; inx < length ; inx++) {
        if (str[inx] == '\t') {
            cursor.x = (cursor.x + 8) & 0xfffffff8;
            if (cursor.x >= width)
                gotoNewLine();
        } else if (str[inx] == '\n') {
                gotoNewLine();
        } else {
            textEmul.printCharAt(cursor.x, cursor.y, str[inx], COLOR);
            cursor.x++;
            if (cursor.x >= width)
                gotoNewLine();
        }
    }

    return inx;
}

void Console::gotoNewLine() {
    cursor.x = 0;
    if (cursor.y == height - 1)
        textEmul.scrollUp(1);
    else
        cursor.y++;
}
