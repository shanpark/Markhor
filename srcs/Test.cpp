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
#include "Test.h"

// #define WIDTH   1680
// #define HEIGHT  1050
#define WIDTH   1920
#define HEIGHT  1080
#define DEPTH   8
// #define COLOR   0x00ffffff // 0xaarrggbb. for BGR mode
#define COLOR   0x07 // 0xaarrggbb. BGRA for little endian.

static Uint32 palette[16] = { // format = 0x00bbggrr
    0x00000000, 0x00000080, 0x00008000, 0x00008080, 0x00800000, 0x00800080, 0x00808000, 0x00c0c0c0,
    0x00808080, 0x000000ff, 0x0000ff00, 0x0000ffff, 0x00ff0000, 0x00ff00ff, 0x00ffff00, 0x00ffffff };

static int y = 0;
static Uint32 data;
static char buf[512];
static Uint32 temp[256];

void setupDisplay() {
    Gpio gpio;
    
    gpio.selectFunction(16, GpioPinFunction::Output);

    if (frameBuffer.setupFrameBuffer(WIDTH, HEIGHT, WIDTH, HEIGHT, DEPTH) == 0)
        gpio.clearOutputPin(16);
    else
        gpio.setOutputPin(16);
}

// void testPalette() {
//     if (DEPTH == 8) {
//         data = setPalette(0, sizeof(palette) / sizeof(Uint32), palette);
//         if (data >= 0)
//             sprintf(buf, "Set Palette: %u", data); // 0=valid, 1=invalid
//         else
//             sprintf(buf, "Set Palette: fail");
//         putString(0, y++, buf, COLOR);

//         if (getPalette(temp) == 0) {
//             if ((palette[6] == temp[6]) && (palette[7] == temp[7]))
//                 sprintf(buf, "Get Palette: OK");
//             else
//                 sprintf(buf, "Get Palette: NOK [%08X, %08X, %08X, %08X, %08X, %08X, %08X, %08X]", temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15]);
//         } else
//             sprintf(buf, "Get Palette: fail");
//         putString(0, y++, buf, COLOR);
//     }
// }

// void printFrameBufferInfo() {
//     sprintf(buf, "Frame Buffer Base:%08X, End:%08X, Size:%08X, Bpp:%d, Pitch:%d", frameBuffer.base, frameBuffer.end, frameBuffer.end - frameBuffer.base, frameBuffer.bpp, frameBuffer.pitch);
//     putString(0, y++, buf, COLOR);

//     sprintf(buf, "Frame Buffer Physical:[%d, %d], Virtual:[%d, %d]", frameBuffer.physicalWidth, frameBuffer.physicalHeight, frameBuffer.virtualWidth, frameBuffer.virtualHeight);
//     putString(0, y++, buf, COLOR);
// }

// void printHardwareInfo() {
//     data = getFirmwareRevision();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "Firmware Revision: %08X", data);
//     else
//         sprintf(buf, "Firmware Revision: fail");
//     putString(0, y++, buf, COLOR);

//     data = getBoardModel();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "Board Model: %08X", data);
//     else
//         sprintf(buf, "Board Model: fail");
//     putString(0, y++, buf, COLOR);

//     data = getBoardRevision();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "Board Revision: %08X", data);
//     else
//         sprintf(buf, "Board Revision: fail");
//     putString(0, y++, buf, COLOR);

//     Uint8 mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//     if (getBoardMacAddress(mac) == 0)
//         sprintf(buf, "Board MAC Address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//     else
//         sprintf(buf, "Board MAC Address: fail");
//     putString(0, y++, buf, COLOR);

//     Uint64 data64 = getBoardSerial();
//     if (data64 != (Uint64)(-1))
//         sprintf(buf, "Board Serial: %llu", data64);
//     else
//         sprintf(buf, "Board Serial: fail");
//     putString(0, y++, buf, COLOR);
// }

// void printHwMemoryInfo() {
//     Address base;
//     Uint64 size;

//     if (getArmMemory(&base, &size) == 0)
//         sprintf(buf, "ARM Mem Base: 0x%016llX, Size: 0x%016llX", (Uint64)base, size);
//     else
//         sprintf(buf, "ARM Mem Base: fail, Size: fail");
//     putString(0, y++, buf, COLOR);

//     if (getVcMemory(&base, &size) == 0)
//         sprintf(buf, "VC Mem Base: 0x%016llX, Size: 0x%016llX", (Uint64)base, size);
//     else
//         sprintf(buf, "VC Mem Base: fail, Size: fail");
//     putString(0, y++, buf, COLOR);
// }

// void testClock() {
//     int clockCount = 20;
//     Clock clocks[20];
//     int ret = getClocks(&clockCount, clocks);
//     if (ret == 0) {
//         int clockIndex = 0;
//         for (int jnx = 0 ; clockIndex < clockCount ; jnx++) {
//             for (int inx = 0 ; (inx < 5) && (clockIndex < clockCount) ; inx++) {
//                 sprintf(buf, "Clock %02d: [0x%08X, 0x%08X]", clockIndex, clocks[clockIndex].parentId, clocks[clockIndex].id);
//                 putString(40 * inx, y, buf, COLOR);
//                 clockIndex++;
//             }
//             y++;
//         }
//     } else if (ret == -2) { // not enough buffer space
//         sprintf(buf, "Get Clocks: least buffer size: %d", clockCount);
//         putString(0, y++, buf, COLOR);
//     } else {
//         sprintf(buf, "Get Clocks: fail");
//         putString(0, y++, buf, COLOR);
//     }

//     const ClockId clockId = CIPwm; // PWM clock test
//     Uint32 state = getClockState(clockId);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Get State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
//     else
//         sprintf(buf, "Clock[%d] Get State: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     state = setClockState(clockId, CSOnBit);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Set State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
//     else
//         sprintf(buf, "Clock[%d] Set State: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     Uint32 rate = getClockRate(clockId);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Get Rate: %u", clockId, rate);
//     else
//         sprintf(buf, "Clock[%d] Get Rate: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     rate = setClockRate(clockId, 1000, 0);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Set Rate: %u", clockId, rate);
//     else
//         sprintf(buf, "Clock[%d] Set Rate: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     rate = getMaxClockRate(clockId);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Max Rate: %u", clockId, rate);
//     else
//         sprintf(buf, "Clock[%d] Max Rate: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     rate = getMinClockRate(clockId);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Min Rate: %u", clockId, rate);
//     else
//         sprintf(buf, "Clock[%d] Min Rate: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     state = setClockState(clockId, 0);
//     if (state != (Uint32)(-1))
//         sprintf(buf, "Clock[%d] Set State: %s, %s", clockId, (state & CSOnBit) ? "On" : "Off", (state & CSNotExistBit) ? "Not Exist" : "Exist");
//     else
//         sprintf(buf, "Clock[%d] Set State: fail", clockId);
//     putString(0, y++, buf, COLOR);

//     Uint32 level = getTurbo(0);
//     if (level != (Uint32)(-1))
//         sprintf(buf, "Turbo Level: %u", level);
//     else
//         sprintf(buf, "Turbo Level: fail");
//     putString(0, y++, buf, COLOR);

//     // level = setTurbo(0, 1);
//     // if (state != (Uint32)(-1)) {
//     //     sprintf(buf, "Set Turbo Level: %u", level);
//     //     putString(0, y++, buf, COLOR);
//     // } else {
//     //     sprintf(buf, "Set Turbo Level: fail");
//     //     putString(0, y++, buf, COLOR);
//     // }
// }

// void printCommandLine() {
//     int bufLen = 300;
//     char cmdLine[300 + 1];
//     int ret = getCommandLine(&bufLen, cmdLine);
//     if (ret == 0) {
//         cmdLine[bufLen] = 0; // null
//         sprintf(buf, "Command Line: %s", cmdLine);
//         putString(0, y++, buf, COLOR);
//         y++; // 2줄이 더라구...
//     } else if (ret == -2) { // not enough buffer space
//         sprintf(buf, "Command Line: least buffer size: %d", bufLen);
//         putString(0, y++, buf, COLOR);
//     } else {
//         sprintf(buf, "Command Line: fail");
//         putString(0, y++, buf, COLOR);
//     }
// }

// void printDmaState() {
//     data = getDmaChannels();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "DMA Channels: %08X", data);
//     else 
//         sprintf(buf, "DMA Channels: fail");
//     putString(0, y++, buf, COLOR);
// }

// void testDevicePower() {
//     // power on/ff 동작은 UsbHcd만 동작한다. 
//     // 게다가 set을 할 때 on/off 비트의 값은 문서와 반대다. 즉 1이면 꺼지고 0이면 켜진다. ???
//     const DeviceId device = DIUsbHcd; 
//     data = getPowerState(device);
//     if (data != (Uint32)(-1)) {
//         sprintf(buf, "Divice[%d] Power State: %s, %s", device, (data & PSOnBit) ? "On" : "Off", (data & PSNotExistBit) ? "Not Exist" : "Exist");
//         putString(0, y++, buf, COLOR);
//     } else {
//         sprintf(buf, "Divice[%d] Power State: fail", device);
//         putString(0, y++, buf, COLOR);
//     }

//     if (data & PSOnBit) {
//         data = setPowerState(device, PSOnBit & PSWaitBit); // off
//         if (data != (Uint32)(-1)) {
//             sprintf(buf, "Divice[%d] Power State: %s, %s", device, (data & PSOnBit) ? "On" : "Off", (data & PSNotExistBit) ? "Not Exist" : "Exist");
//             putString(0, y++, buf, COLOR);
//         } else {
//             sprintf(buf, "Divice[%d] Power State: fail", device);
//             putString(0, y++, buf, COLOR);
//             return;
//         }

//         data = setPowerState(device, PSWaitBit); // on
//         if (data != (Uint32)(-1)) {
//             sprintf(buf, "Divice[%d] Power State: %s, %s", device, (data & PSOnBit) ? "On" : "Off", (data & PSNotExistBit) ? "Not Exist" : "Exist");
//             putString(0, y++, buf, COLOR);
//         } else {
//             sprintf(buf, "Divice[%d] Power State: fail", device);
//             putString(0, y++, buf, COLOR);
//             return;
//         }
//     } else {
//         data = setPowerState(device, PSWaitBit); // on. 문서와 반대로 움직인다. bit0을 0으로 줘야 on 상태로 되는데??
//         if (data != (Uint32)(-1)) {
//             sprintf(buf, "Divice[%d] Power State: %s, %s", device, (data & PSOnBit) ? "On" : "Off", (data & PSNotExistBit) ? "Not Exist" : "Exist");
//             putString(0, y++, buf, COLOR);
//         } else {
//             sprintf(buf, "Divice[%d] Power State: fail", device);
//             putString(0, y++, buf, COLOR);
//             return;
//         }

//         data = setPowerState(device, PSOnBit & PSWaitBit); // off
//         if (data != (Uint32)(-1)) {
//             sprintf(buf, "Divice[%d] Power State: %s, %s", device, (data & PSOnBit) ? "On" : "Off", (data & PSNotExistBit) ? "Not Exist" : "Exist");
//             putString(0, y++, buf, COLOR);
//         } else {
//             sprintf(buf, "Divice[%d] Power State: fail", device);
//             putString(0, y++, buf, COLOR);
//             return;
//         }
//     }
// }

// void printVoltageInfo() {
//     VoltageId voltageId = VISdramC;
//     Uint32 voltage = getVoltage(voltageId);
//     if (voltage != (Uint32)(-1))
//         sprintf(buf, "Voltage[%d]: %u", voltageId, voltage);
//     else
//         sprintf(buf, "Voltage[%d]: fail", voltageId);
//     putString(0, y++, buf, COLOR);

//     voltage = getMaxVoltage(voltageId);
//     if (voltage != (Uint32)(-1))
//         sprintf(buf, "Voltage[%d] Max: %u", voltageId, voltage);
//     else
//         sprintf(buf, "Voltage[%d] Max: fail", voltageId);
//     putString(0, y++, buf, COLOR);

//     voltage = getMinVoltage(voltageId);
//     if (voltage != (Uint32)(-1))
//         sprintf(buf, "Voltage[%d] Min: %u", voltageId, voltage);
//     else
//         sprintf(buf, "Voltage[%d] Min: fail", voltageId);
//     putString(0, y++, buf, COLOR);
// }

// void printTemperature() {
//     Uint32 temperature = getTemperature(0);
//     if (temperature != (Uint32)(-1))
//         sprintf(buf, "Temperatore: %u", temperature);
//     else
//         sprintf(buf, "Temperatore]: fail");
//     putString(0, y++, buf, COLOR);

//     temperature = getMaxTemperature(0);
//     if (temperature != (Uint32)(-1))
//         sprintf(buf, "Temperatore Max: %u", temperature);
//     else
//         sprintf(buf, "Temperatore Max: fail");
//     putString(0, y++, buf, COLOR);
// }

// void testDisplayInfo() {
//     // blankScreen(1);

//     Uint32 width = 0, height = 0;
//     if (getPhysicalWH(&width, &height) == 0)
//         sprintf(buf, "Get Physical WH: [%d, %d]", width, height);
//     else
//         sprintf(buf, "Get Physical WH: fail");
//     putString(0, y++, buf, COLOR);

//     width = 640, height = 480;
//     if (testPhysicalWH(&width, &height) == 0)
//         sprintf(buf, "Test Physical WH: [640, 480] -> [%d, %d]", width, height);
//     else
//         sprintf(buf, "Test Physical WH: fail");
//     putString(0, y++, buf, COLOR);

//     width = 0, height = 0;
//     if (getVirtualWH(&width, &height) == 0)
//         sprintf(buf, "Get Virtual WH: [%d, %d]", width, height);
//     else
//         sprintf(buf, "Get Virtual WH: fail");
//     putString(0, y++, buf, COLOR);

//     width = 640, height = 480;
//     if (testVirtualWH(&width, &height) == 0)
//         sprintf(buf, "Test Virtual WH: [640, 480] -> [%d, %d]", width, height);
//     else
//         sprintf(buf, "Test Virtual WH: fail");
//     putString(0, y++, buf, COLOR);

//     Uint32 depth = 0;
//     if (getDepth(&depth) == 0)
//         sprintf(buf, "Get Depth: %d", depth);
//     else
//         sprintf(buf, "Get Depth: fail");
//     putString(0, y++, buf, COLOR);

//     depth = 16;
//     if (testDepth(&depth) == 0)
//         sprintf(buf, "Test Depth: 16 -> %d", depth);
//     else
//         sprintf(buf, "Test Depth: fail");
//     putString(0, y++, buf, COLOR);

//     Uint32 pixelOrder = getPixelOrder();
//     if (pixelOrder != (Uint32)(-1))
//         sprintf(buf, "Pixel Order: %s", pixelOrder == 0 ? "BGR" : "RGB");
//     else
//         sprintf(buf, "Pixel Order: fail");
//     putString(0, y++, buf, COLOR);

//     data = getAlphaMode();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "Alpha Mode: %u", data);
//     else
//         sprintf(buf, "Alpha Mode: fail");
//     putString(0, y++, buf, COLOR);

//     data = getPitch();
//     if (data != (Uint32)(-1))
//         sprintf(buf, "Pitch: %u", data);
//     else
//         sprintf(buf, "Pitch: fail");
//     putString(0, y++, buf, COLOR);

//     Uint32 offX = 0, offY = 0;
//     if (getVirtualOffset(&offX, &offY) == 0)
//         sprintf(buf, "Get Virtual Offset: [%d, %d]", offX, offY);
//     else
//         sprintf(buf, "Get Virtual Offset: fail");
//     putString(0, y++, buf, COLOR);

//     offX = 16, offY = 16;
//     if (setVirtualOffset(&offX, &offY) == 0)
//         sprintf(buf, "Set Virtual Offset: [16, 16] -> [%d, %d]", offX, offY);
//     else
//         sprintf(buf, "Set Virtual Offset: fail");
//     putString(0, y++, buf, COLOR);
// }