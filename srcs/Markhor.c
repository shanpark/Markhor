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

#include <stdio.h>
#include "Gpio.h"
#include "MailboxProperty.h"
#include "FrameBuffer.h"
#include "Display.h"

#define WIDTH   1680
#define HEIGHT  1050
// #define WIDTH   1920
// #define HEIGHT  1080
#define DEPTH   8
// #define COLOR   0x00ffffff // 0xaarrggbb. for BGR mode
#define COLOR   0x07 // 0xaarrggbb. BGRA for little endian.

// format = 0x00bbggrr
Uint32 palette[16] = { 
    0x00000000, 0x00000080, 0x00008000, 0x00008080, 0x00800000, 0x00800080, 0x00808000, 0x00c0c0c0,
    0x00808080, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff, 0x00ffff00, 0x00ffffff };

Uint32 temp[256];

int Markhor(void) {
    GpioSelectFunction(16, 1);

    if (setFrameBuffer(WIDTH, HEIGHT, WIDTH, HEIGHT, DEPTH) == 0)
        GpioClearOutputPin(16);
    else
        GpioSetOutputPin(16);

    // setPixel(0, 0, COLOR);
    // setPixel(1, 1, COLOR);
    // setPixel(WIDTH - 2, HEIGHT - 2, COLOR);
    // setPixel(WIDTH - 1, HEIGHT - 1, COLOR);

    char buf[512];
    int y;
    Uint32 data;

    if (DEPTH == 8) {
        data = setPalette(0, sizeof(palette) / sizeof(Uint32), palette);
        if (data >= 0)
            sprintf(buf, "Set Palette: %u", data);
        else
            sprintf(buf, "Set Palette: fail");
        putString(0, y++, buf, COLOR);

        if (getPalette(temp) == 0) {
            if ((palette[6] == temp[6]) && (palette[7] == temp[7]))
                sprintf(buf, "Get Palette: OK");
            else
                sprintf(buf, "Get Palette: NOK [%08X, %08X]", palette[7], temp[7]);
        } else
            sprintf(buf, "Get Palette: fail");
        putString(0, y++, buf, COLOR);
    }

    sprintf(buf, "Frame Buffer Base:%08X, End:%08X, Size:%08X, Pitch:%d", frameBuffer.base, frameBuffer.end, frameBuffer.end - frameBuffer.base, frameBuffer.pitch);
    putString(0, y++, buf, COLOR);

    sprintf(buf, "Firmware Revision: %08X", getFirmwareRevision());
    putString(0, y++, buf, COLOR);

    sprintf(buf, "Board Model: %08X", getBoardModel());
    putString(0, y++, buf, COLOR);

    sprintf(buf, "Board Revision: %08X", getBoardRevision());
    putString(0, y++, buf, COLOR);

    Uint8 mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    if (getBoardMacAddress(mac) == 0)
        sprintf(buf, "Board MAC Address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    else
        sprintf(buf, "Board MAC Address: fail");
    putString(0, y++, buf, COLOR);

    sprintf(buf, "Board Serial: %llu", getBoardSerial());
    putString(0, y++, buf, COLOR);

    Address base;
    Uint64 size;
    if (getArmMemory(&base, &size) == 0)
        sprintf(buf, "ARM Mem Base: 0x%08X, Size: 0x%016llX", base, size);
    else
        sprintf(buf, "ARM Mem Base: fail, Size: fail");
    putString(0, y++, buf, COLOR);

    if (getVcMemory(&base, &size) == 0)
        sprintf(buf, "VC Mem Base: 0x%08X, Size: 0x%016llX", base, size);
    else
        sprintf(buf, "VC Mem Base: fail, Size: fail");
    putString(0, y++, buf, COLOR);

    int clockCount = 10;
    Clock clocks[10];
    int ret = getClocks(&clockCount, clocks);
    if (ret == 0) {
        int clockIndex = 0;
        for (int jnx = 0 ; (jnx < 10) && (clockIndex < clockCount) ; jnx++) {
            for (int inx = 0 ; (inx < 5) && (clockIndex < clockCount) ; inx++) {
                sprintf(buf, "Clock %02d: [0x%08X, 0x%08X]", clockIndex, clocks[clockIndex].parentId, clocks[clockIndex].id);
                putString(40 * inx, y, buf, COLOR);
                clockIndex++;
            }
            y++;
        }
    } else if (ret == -2) { // not enough buffer space
        sprintf(buf, "Get Clocks: least buffer size: %d", clockCount);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Get Clocks: fail");
        putString(0, y++, buf, COLOR);
    }

    int bufLen = 268;
    char cmdLine[268 + 1];
    ret = getCommandLine(&bufLen, cmdLine);
    if (ret == 0) {
        cmdLine[bufLen] = 0; // null
        sprintf(buf, "Command Line: %s", cmdLine);
        putString(0, y++, buf, COLOR);
        y++; // 2줄이 더라구...
    } else if (ret == -2) { // not enough buffer space
        sprintf(buf, "Command Line: least buffer size: %d", bufLen);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Command Line: fail");
        putString(0, y++, buf, COLOR);
    }

    sprintf(buf, "DMA Channels: %08X", getDmaChannels());
    putString(0, y++, buf, COLOR);

    const DeviceId device = DIUsbHcd; // power on/ff 동작은 이 장치만 동작한다. 게다가 set을 할 때 on/off 비트의 값은 문서와 반대다. ???
    Uint32 state = getPowerState(device);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Divice[%d] Power State: %s, %s", device, (state & PSOnBit) ? "On" : "Off", (state & PSNotExistBit) ? "Not Exist" : "Exist");
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Divice[%d] Power State: fail", device);
        putString(0, y++, buf, COLOR);
    }

    if (state & PSOnBit) {
        state = setPowerState(device, PSOnBit & PSWaitBit); // off
        if (state != (Uint32)(-1)) {
            sprintf(buf, "Divice[%d] Power State: %s, %s", device, (state & PSOnBit) ? "On" : "Off", (state & PSNotExistBit) ? "Not Exist" : "Exist");
            putString(0, y++, buf, COLOR);
        } else {
            sprintf(buf, "Divice[%d] Power State: fail", device);
            putString(0, y++, buf, COLOR);
            goto NEXT0;
        }

        state = setPowerState(device, PSWaitBit); // on
        if (state != (Uint32)(-1)) {
            sprintf(buf, "Divice[%d] Power State: %s, %s", device, (state & PSOnBit) ? "On" : "Off", (state & PSNotExistBit) ? "Not Exist" : "Exist");
            putString(0, y++, buf, COLOR);
        } else {
            sprintf(buf, "Divice[%d] Power State: fail", device);
            putString(0, y++, buf, COLOR);
            goto NEXT0;
        }
    } else {
        state = setPowerState(device, PSWaitBit); // on. 문서와 반대로 움직인다. bit0을 0으로 줘야 on 상태로 되는데??
        if (state != (Uint32)(-1)) {
            sprintf(buf, "Divice[%d] Power State: %s, %s", device, (state & PSOnBit) ? "On" : "Off", (state & PSNotExistBit) ? "Not Exist" : "Exist");
            putString(0, y++, buf, COLOR);
        } else {
            sprintf(buf, "Divice[%d] Power State: fail", device);
            putString(0, y++, buf, COLOR);
            goto NEXT0;
        }

        state = setPowerState(device, PSOnBit & PSWaitBit); // off
        if (state != (Uint32)(-1)) {
            sprintf(buf, "Divice[%d] Power State: %s, %s", device, (state & PSOnBit) ? "On" : "Off", (state & PSNotExistBit) ? "Not Exist" : "Exist");
            putString(0, y++, buf, COLOR);
        } else {
            sprintf(buf, "Divice[%d] Power State: fail", device);
            putString(0, y++, buf, COLOR);
            goto NEXT0;
        }
    }

    const ClockId clockId = CIPwm;
NEXT0:
    state = getClockState(clockId);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Get State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Get State: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    state = setClockState(clockId, CSOnBit);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Set State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Set State: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    Uint32 rate = getClockRate(clockId);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Get Rate: %u", clockId, rate);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Get Rate: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    rate = setClockRate(clockId, 1000, 0);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Set Rate: %u", clockId, rate);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Set Rate: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    rate = getMaxClockRate(clockId);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Max Rate: %u", clockId, rate);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Max Rate: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    rate = getMinClockRate(clockId);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Min Rate: %u", clockId, rate);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Min Rate: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    state = setClockState(clockId, 0);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Clock[%d] Set State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Clock[%d] Set State: fail", clockId);
        putString(0, y++, buf, COLOR);
    }

    Uint32 level = getTurbo(0);
    if (state != (Uint32)(-1)) {
        sprintf(buf, "Turbo Level: %u", level);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Turbo Level: fail");
        putString(0, y++, buf, COLOR);
    }

    // level = setTurbo(0, 1);
    // if (state != (Uint32)(-1)) {
    //     sprintf(buf, "Set Turbo Level: %u", level);
    //     putString(0, y++, buf, COLOR);
    // } else {
    //     sprintf(buf, "Set Turbo Level: fail");
    //     putString(0, y++, buf, COLOR);
    // }

    VoltageId voltageId = VISdramC;
    Uint32 voltage = getVoltage(voltageId);
    if (voltage != (Uint32)(-1)) {
        sprintf(buf, "Voltage[%d]: %u", voltageId, voltage);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Voltage[%d]: fail", voltageId);
        putString(0, y++, buf, COLOR);
    }

    voltage = getMaxVoltage(voltageId);
    if (voltage != (Uint32)(-1)) {
        sprintf(buf, "Voltage[%d] Max: %u", voltageId, voltage);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Voltage[%d] Max: fail", voltageId);
        putString(0, y++, buf, COLOR);
    }

    voltage = getMinVoltage(voltageId);
    if (voltage != (Uint32)(-1)) {
        sprintf(buf, "Voltage[%d] Min: %u", voltageId, voltage);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Voltage[%d] Min: fail", voltageId);
        putString(0, y++, buf, COLOR);
    }

    Uint32 temperature = getTemperature(0);
    if (temperature != (Uint32)(-1)) {
        sprintf(buf, "Temperatore: %u", temperature);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Temperatore]: fail");
        putString(0, y++, buf, COLOR);
    }

    temperature = getMaxTemperature(0);
    if (temperature != (Uint32)(-1)) {
        sprintf(buf, "Temperatore Max: %u", temperature);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Temperatore Max: fail");
        putString(0, y++, buf, COLOR);
    }

    // blankScreen(1);

    Uint32 width = 1233, height = 233;
    if (getPhysicalWH(&width, &height) == 0) {
        sprintf(buf, "Get Physical WH: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Get Physical WH: fail");
        putString(0, y++, buf, COLOR);
    }

    width = 1233, height = 233;
    if (testPhysicalWH(&width, &height) == 0) {
        sprintf(buf, "Test Physical WH: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Test Physical WH: fail");
        putString(0, y++, buf, COLOR);
    }

    width = 1233, height = 233;
    if (getVirtualWH(&width, &height) == 0) {
        sprintf(buf, "Get Virtual WH: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Get Virtual WH: fail");
        putString(0, y++, buf, COLOR);
    }

    width = 1233, height = 233;
    if (testVirtualWH(&width, &height) == 0) {
        sprintf(buf, "Test Virtual WH: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Test Virtual WH: fail");
        putString(0, y++, buf, COLOR);
    }

    Uint32 depth = 8;
    if (getDepth(&depth) == 0) {
        sprintf(buf, "Get Depth: %d", depth);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Get Depth: fail");
        putString(0, y++, buf, COLOR);
    }

    depth = 8;
    if (testDepth(&depth) == 0) {
        sprintf(buf, "Test Depth: %d", depth);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Test Depth: fail");
        putString(0, y++, buf, COLOR);
    }

    Uint32 pixelOrder = getPixelOrder();
    if (pixelOrder != (Uint32)(-1))
        sprintf(buf, "Pixel Order: %s", pixelOrder == 0 ? "BGR" : "RGB");
    else
        sprintf(buf, "Pixel Order: fail");
    putString(0, y++, buf, COLOR);

    data = getAlphaMode();
    if (data != (Uint32)(-1))
        sprintf(buf, "Alpha Mode: %u", data);
    else
        sprintf(buf, "Alpha Mode: fail");
    putString(0, y++, buf, COLOR);

    data = getPitch();
    if (data != (Uint32)(-1))
        sprintf(buf, "Pitch: %u", data);
    else
        sprintf(buf, "Pitch: fail");
    putString(0, y++, buf, COLOR);

    width = 1, height = 1;
    if (getVirtualOffset(&width, &height) == 0) {
        sprintf(buf, "Get Virtual Offset: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Get Virtual Offset: fail");
        putString(0, y++, buf, COLOR);
    }

    width = 8, height = 16;
    if (setVirtualOffset(&width, &height) == 0) {
        sprintf(buf, "Set Virtual Offset: [%d, %d]", width, height);
        putString(0, y++, buf, COLOR);
    } else {
        sprintf(buf, "Set Virtual Offset: fail");
        putString(0, y++, buf, COLOR);
    }

    return 0;
}

/**
 * NOT complete function. must be rewritten.
 */
extern char _end;
void * _sbrk(int incr) {
    static char * heap_end = &_end;
    char * prev_heap_end;

    // if (heap_end == 0)
    //     heap_end = &_end;

     prev_heap_end = heap_end;
     heap_end += incr;

     return (void *)prev_heap_end;
}
