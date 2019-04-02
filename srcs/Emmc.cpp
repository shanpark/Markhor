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

/* TODO just for debugging */
#include "Console.h"
#include "CLib.h"
extern char buf[512];
/* End: just for debugging */

#include <cstring>
#include "IoPeripherals.h"
#include "MailboxProperty.h"
#include "SystemTimer.h"
#include "Emmc.h"

/* BCM2385 ARM Peripherals document */
#define EMMC_BASE   (IO_PERIPHERALS_BASE + 0x300000UL)

// SDMA buffer address
#define SDMA_BUFFER     0x6000
#define SDMA_BUFFER_PA  (SDMA_BUFFER + 0xC0000000)

// SD Clock Frequencies (in Hz)
#define SD_CLOCK_ID         400000
#define SD_CLOCK_NORMAL     25000000
#define SD_CLOCK_HIGH       50000000
#define SD_CLOCK_100        100000000
#define SD_CLOCK_208        208000000

/* EMMC registers offsets */
#define ARG2            0
#define BLKSIZECNT      1
#define ARG1            2
#define CMDTM           3
#define RESP0           4
#define RESP1           5
#define RESP2           6
#define RESP3           7
#define DATA            8
#define STATUS          9
#define CONTROL0        10
#define CONTROL1        11
#define INTERRUPT       12
#define IRPT_MASK       13
#define IRPT_EN         14
#define CONTROL2        15
#define FORCE_IRPT      20
#define BOOT_TIMEOUT    28
#define DBG_SEL         29
#define EXRDFIFO_CFG    32
#define EXRDFIFO_EN     33
#define TUNE_STEP       34
#define TUNE_STEPS_STD  35
#define TUNE_STEPS_DDR  36
#define SPI_INT_SPT     60
#define SLOTISR_VER     63

#define SD_COMMAND_COMPLETE     (1 << 0)
#define SD_TRANSFER_COMPLETE    (1 << 1)
#define SD_BLOCK_GAP_EVENT      (1 << 2)
#define SD_DMA_INTERRUPT        (1 << 3)
#define SD_BUFFER_WRITE_READY   (1 << 4)
#define SD_BUFFER_READ_READY    (1 << 5)
#define SD_CARD_INSERTION       (1 << 6)
#define SD_CARD_REMOVAL         (1 << 7)
#define SD_CARD_INTERRUPT       (1 << 8)

// The actual command indices
#define GO_IDLE_STATE           0
#define ALL_SEND_CID            2
#define SEND_RELATIVE_ADDR      3
#define SET_DSR                 4
#define IO_SET_OP_COND          5
#define SWITCH_FUNC             6
#define SELECT_CARD             7
#define DESELECT_CARD           7
#define SELECT_DESELECT_CARD    7
#define SEND_IF_COND            8
#define SEND_CSD                9
#define SEND_CID                10
#define VOLTAGE_SWITCH          11
#define STOP_TRANSMISSION       12
#define SEND_STATUS             13
#define GO_INACTIVE_STATE       15
#define SET_BLOCKLEN            16
#define READ_SINGLE_BLOCK       17
#define READ_MULTIPLE_BLOCK     18
#define SEND_TUNING_BLOCK       19
#define SPEED_CLASS_CONTROL     20
#define SET_BLOCK_COUNT         23
#define WRITE_BLOCK             24
#define WRITE_MULTIPLE_BLOCK    25
#define PROGRAM_CSD             27
#define SET_WRITE_PROT          28
#define CLR_WRITE_PROT          29
#define SEND_WRITE_PROT         30
#define ERASE_WR_BLK_START      32
#define ERASE_WR_BLK_END        33
#define ERASE                   38
#define LOCK_UNLOCK             42
#define APP_CMD                 55
#define GEN_CMD                 56

#define IS_APP_CMD              0x80000000
#define ACMD(a)                 (a | IS_APP_CMD)
#define SET_BUS_WIDTH           (6 | IS_APP_CMD)
#define SD_STATUS               (13 | IS_APP_CMD)
#define SEND_NUM_WR_BLOCKS      (22 | IS_APP_CMD)
#define SET_WR_BLK_ERASE_COUNT  (23 | IS_APP_CMD)
#define SD_SEND_OP_COND         (41 | IS_APP_CMD)
#define SET_CLR_CARD_DETECT     (42 | IS_APP_CMD)
#define SEND_SCR                (51 | IS_APP_CMD)

#define SD_RESET_ALL            (1 << 24)
#define SD_RESET_CMD            (1 << 25)
#define SD_RESET_DAT            (1 << 26)

#define SD_CMD_INDEX(a)		((a) << 24)
#define SD_CMD_TYPE_NORMAL	0x0
#define SD_CMD_TYPE_SUSPEND	(1 << 22)
#define SD_CMD_TYPE_RESUME	(2 << 22)
#define SD_CMD_TYPE_ABORT	(3 << 22)
#define SD_CMD_TYPE_MASK    (3 << 22)
#define SD_CMD_ISDATA		(1 << 21)
#define SD_CMD_IXCHK_EN		(1 << 20)
#define SD_CMD_CRCCHK_EN	(1 << 19)
#define SD_CMD_RSPNS_TYPE_NONE	0			// For no response
#define SD_CMD_RSPNS_TYPE_136	(1 << 16)		// For response R2 (with CRC), R3,4 (no CRC)
#define SD_CMD_RSPNS_TYPE_48	(2 << 16)		// For responses R1, R5, R6, R7 (with CRC)
#define SD_CMD_RSPNS_TYPE_48B	(3 << 16)		// For responses R1b, R5b (with CRC)
#define SD_CMD_RSPNS_TYPE_MASK  (3 << 16)
#define SD_CMD_MULTI_BLOCK	(1 << 5)
#define SD_CMD_DAT_DIR_HC	0
#define SD_CMD_DAT_DIR_CH	(1 << 4)
#define SD_CMD_AUTO_CMD_EN_NONE	0
#define SD_CMD_AUTO_CMD_EN_CMD12	(1 << 2)
#define SD_CMD_AUTO_CMD_EN_CMD23	(2 << 2)
#define SD_CMD_BLKCNT_EN		(1 << 1)
#define SD_CMD_DMA          1

#define SD_ERR_CMD_TIMEOUT	0
#define SD_ERR_CMD_CRC		1
#define SD_ERR_CMD_END_BIT	2
#define SD_ERR_CMD_INDEX	3
#define SD_ERR_DATA_TIMEOUT	4
#define SD_ERR_DATA_CRC		5
#define SD_ERR_DATA_END_BIT	6
#define SD_ERR_CURRENT_LIMIT	7
#define SD_ERR_AUTO_CMD12	8
#define SD_ERR_ADMA		9
#define SD_ERR_TUNING		10
#define SD_ERR_RSVD		11

#define SD_ERR_MASK_CMD_TIMEOUT		(1 << (16 + SD_ERR_CMD_TIMEOUT))
#define SD_ERR_MASK_CMD_CRC		(1 << (16 + SD_ERR_CMD_CRC))
#define SD_ERR_MASK_CMD_END_BIT		(1 << (16 + SD_ERR_CMD_END_BIT))
#define SD_ERR_MASK_CMD_INDEX		(1 << (16 + SD_ERR_CMD_INDEX))
#define SD_ERR_MASK_DATA_TIMEOUT	(1 << (16 + SD_ERR_CMD_TIMEOUT))
#define SD_ERR_MASK_DATA_CRC		(1 << (16 + SD_ERR_CMD_CRC))
#define SD_ERR_MASK_DATA_END_BIT	(1 << (16 + SD_ERR_CMD_END_BIT))
#define SD_ERR_MASK_CURRENT_LIMIT	(1 << (16 + SD_ERR_CMD_CURRENT_LIMIT))
#define SD_ERR_MASK_AUTO_CMD12		(1 << (16 + SD_ERR_CMD_AUTO_CMD12))
#define SD_ERR_MASK_ADMA		(1 << (16 + SD_ERR_CMD_ADMA))
#define SD_ERR_MASK_TUNING		(1 << (16 + SD_ERR_CMD_TUNING))

#define SD_RESP_NONE        SD_CMD_RSPNS_TYPE_NONE
#define SD_RESP_R1          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R1b         (SD_CMD_RSPNS_TYPE_48B | SD_CMD_CRCCHK_EN)
#define SD_RESP_R2          (SD_CMD_RSPNS_TYPE_136 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R3          SD_CMD_RSPNS_TYPE_48
#define SD_RESP_R4          SD_CMD_RSPNS_TYPE_136
#define SD_RESP_R5          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R5b         (SD_CMD_RSPNS_TYPE_48B | SD_CMD_CRCCHK_EN)
#define SD_RESP_R6          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)
#define SD_RESP_R7          (SD_CMD_RSPNS_TYPE_48 | SD_CMD_CRCCHK_EN)

#define SD_DATA_READ        (SD_CMD_ISDATA | SD_CMD_DAT_DIR_CH)
#define SD_DATA_WRITE       (SD_CMD_ISDATA | SD_CMD_DAT_DIR_HC)

#define SD_CMD_RESERVED(a)  0xffffffff

static uint32_t sd_commands[] = {
    SD_CMD_INDEX(0),
    SD_CMD_RESERVED(1),
    SD_CMD_INDEX(2) | SD_RESP_R2,
    SD_CMD_INDEX(3) | SD_RESP_R6,
    SD_CMD_INDEX(4),
    SD_CMD_INDEX(5) | SD_RESP_R4,
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_INDEX(7) | SD_RESP_R1b,
    SD_CMD_INDEX(8) | SD_RESP_R7,
    SD_CMD_INDEX(9) | SD_RESP_R2,
    SD_CMD_INDEX(10) | SD_RESP_R2,
    SD_CMD_INDEX(11) | SD_RESP_R1,
    SD_CMD_INDEX(12) | SD_RESP_R1b | SD_CMD_TYPE_ABORT,
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_INDEX(15),
    SD_CMD_INDEX(16) | SD_RESP_R1,
    SD_CMD_INDEX(17) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(18) | SD_RESP_R1 | SD_DATA_READ | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_INDEX(19) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(20) | SD_RESP_R1b,
    SD_CMD_RESERVED(21),
    SD_CMD_RESERVED(22),
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_INDEX(24) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(25) | SD_RESP_R1 | SD_DATA_WRITE | SD_CMD_MULTI_BLOCK | SD_CMD_BLKCNT_EN,
    SD_CMD_RESERVED(26),
    SD_CMD_INDEX(27) | SD_RESP_R1 | SD_DATA_WRITE,
    SD_CMD_INDEX(28) | SD_RESP_R1b,
    SD_CMD_INDEX(29) | SD_RESP_R1b,
    SD_CMD_INDEX(30) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(31),
    SD_CMD_INDEX(32) | SD_RESP_R1,
    SD_CMD_INDEX(33) | SD_RESP_R1,
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_INDEX(38) | SD_RESP_R1b,
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_RESERVED(41),
    SD_CMD_RESERVED(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_RESERVED(51),
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_INDEX(55) | SD_RESP_R1,
    SD_CMD_INDEX(56) | SD_RESP_R1 | SD_CMD_ISDATA,
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

static uint32_t sd_acommands[] = {
    SD_CMD_RESERVED(0),
    SD_CMD_RESERVED(1),
    SD_CMD_RESERVED(2),
    SD_CMD_RESERVED(3),
    SD_CMD_RESERVED(4),
    SD_CMD_RESERVED(5),
    SD_CMD_INDEX(6) | SD_RESP_R1,
    SD_CMD_RESERVED(7),
    SD_CMD_RESERVED(8),
    SD_CMD_RESERVED(9),
    SD_CMD_RESERVED(10),
    SD_CMD_RESERVED(11),
    SD_CMD_RESERVED(12),
    SD_CMD_INDEX(13) | SD_RESP_R1,
    SD_CMD_RESERVED(14),
    SD_CMD_RESERVED(15),
    SD_CMD_RESERVED(16),
    SD_CMD_RESERVED(17),
    SD_CMD_RESERVED(18),
    SD_CMD_RESERVED(19),
    SD_CMD_RESERVED(20),
    SD_CMD_RESERVED(21),
    SD_CMD_INDEX(22) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_INDEX(23) | SD_RESP_R1,
    SD_CMD_RESERVED(24),
    SD_CMD_RESERVED(25),
    SD_CMD_RESERVED(26),
    SD_CMD_RESERVED(27),
    SD_CMD_RESERVED(28),
    SD_CMD_RESERVED(29),
    SD_CMD_RESERVED(30),
    SD_CMD_RESERVED(31),
    SD_CMD_RESERVED(32),
    SD_CMD_RESERVED(33),
    SD_CMD_RESERVED(34),
    SD_CMD_RESERVED(35),
    SD_CMD_RESERVED(36),
    SD_CMD_RESERVED(37),
    SD_CMD_RESERVED(38),
    SD_CMD_RESERVED(39),
    SD_CMD_RESERVED(40),
    SD_CMD_INDEX(41) | SD_RESP_R3,
    SD_CMD_INDEX(42) | SD_RESP_R1,
    SD_CMD_RESERVED(43),
    SD_CMD_RESERVED(44),
    SD_CMD_RESERVED(45),
    SD_CMD_RESERVED(46),
    SD_CMD_RESERVED(47),
    SD_CMD_RESERVED(48),
    SD_CMD_RESERVED(49),
    SD_CMD_RESERVED(50),
    SD_CMD_INDEX(51) | SD_RESP_R1 | SD_DATA_READ,
    SD_CMD_RESERVED(52),
    SD_CMD_RESERVED(53),
    SD_CMD_RESERVED(54),
    SD_CMD_RESERVED(55),
    SD_CMD_RESERVED(56),
    SD_CMD_RESERVED(57),
    SD_CMD_RESERVED(58),
    SD_CMD_RESERVED(59),
    SD_CMD_RESERVED(60),
    SD_CMD_RESERVED(61),
    SD_CMD_RESERVED(62),
    SD_CMD_RESERVED(63)
};

#define SD_GET_CLOCK_DIVIDER_FAIL	0xffffffff

Emmc emmc;

volatile Emmc::Register * const Emmc::emmcRegister = (Emmc::Register *)EMMC_BASE;
Uint8 Emmc::internalBuf[1024] __attribute__((aligned(4)));

#define FAIL()             (last_cmd_success == 0)
#define TIMEOUT()         (FAIL() && (last_error == 0))
#define CMD_TIMEOUT()      (FAIL() && (last_error & (1 << 16)))

ResultCode Emmc::init() {
    // Sdcard Power Off, On operation needed?
    if (powerOffSdCard() != ResultCode::Success) {
        console.write("SD Card Power off failed.\n");
        return ResultCode::Fail;
    }

    Uint32 state;
    state = 0;
    getPowerState(&state);
    if (state | static_cast<Uint32>(PowerState::PSOnBit)) {
        sprintf(buf, "SD Card Power is ON. [%u]\n", state);
        console.write(buf);
    } else {
        sprintf(buf, "SD Card Power is OFF. [%u]\n", state);
        console.write(buf);
    }

    // should we wait 5000us ???

    if (powerOnSdCard() != ResultCode::Success) {
        console.write("SD Card Power on failed.\n");
        return ResultCode::Fail;
    }

    state = 0;
    getPowerState(&state);
    if (state | static_cast<Uint32>(PowerState::PSOnBit)) {
        sprintf(buf, "SD Card Power is ON. [%u]\n", state);
        console.write(buf);
    } else {
        sprintf(buf, "SD Card Power is OFF. [%u]\n", state);
        console.write(buf);
    }

    // Read SD Card Info.
    Register ver = emmcRegister[SLOTISR_VER];
    vendor = static_cast<Uint8>(ver >> 24);
    sdVersion = static_cast<Uint8>((ver >> 16) & 0xff);
    slotStatus = static_cast<Uint8>(ver & 0xff);
    sprintf(buf, "Vender:%u, SD Version:%u, Slot Status:%u\n", vendor, sdVersion, slotStatus);
    console.write(buf);

    // reset host controller. disable clocks
    // 값을 찍어보니 읽기 전이나 후나 항상 0으로 찍힌다. 즉, 실행을 하던 안하던 변화가 없다는 것이다.
    Register control1 = emmcRegister[CONTROL1];
	control1 |= (1 << 24); // reset the complete host circuit
	control1 &= ~(1 << 2); // disable SD clock
	control1 &= ~(1 << 0); // disable internal EMMC clocks for power saving
    emmcRegister[CONTROL1], control1;

    systemTimer.waitUntil([]() { return ((emmcRegister[CONTROL1] & (0x7 << 24)) == 0); }, 1000000);
	if((emmcRegister[CONTROL1] & (0x7 << 24)) != 0)	{
		sprintf(buf, "EMMC: controller did not reset properly\n");
        console.write(buf);
		return ResultCode::Fail;
	}

    // check valid card
    // 문서에는 16번 비트는 Reserved이다. 테스트 결과 카드가 있으나 없으나 항상 16번 비트는 1이었다.
    Register status = emmcRegister[STATUS];
    systemTimer.waitUntil([]() { return ((emmcRegister[STATUS] & (1 << 16)) != 0); }, 500000);
	if ((emmcRegister[STATUS] & (1 << 16)) == 0) {
		sprintf(buf, "EMMC: no card inserted\n");
        console.write(buf);
		return ResultCode::Fail;
	}

    // clear CONTROL2
    emmcRegister[CONTROL2] = 0;

    // Get the base clock rate
	baseClock = getBaseClock();
	if (baseClock == 0) {
	    sprintf(buf, "EMMC: assuming clock rate to be 100MHz\n");
        console.write(buf);
	    baseClock = 100000000;
	}

    sprintf(buf, "Base clock: %d\n", baseClock);
    console.write(buf);

    // config clock. CONTROL1 register.
    // TODO : 적절한 clock 설정을 찾아야 한다. SD 카드에 따라서 값이 달라지는 것 같다. SD 카드의 속도가 빠르면 더 빠르게 설정할 수 있는 듯...
    //        지금은 400Khz를 타겟으로 설정해서 값을 하드코딩한 것이다.
    control1 = emmcRegister[CONTROL1];
    control1 |= 0x01; // enable clock.    
    control1 |= 0x4000; // 0x40 << 8. set clock divisor for sd clock. 0x40 = 258Khz (for 400Khz). refer to PartA2-300.pdf page 46.
	control1 |= 0x00070000;	// 0x07 << 16. data timeout interval. data timeout = TMCLK * 2^(7+13)
    emmcRegister[CONTROL1] = control1;

    // wait until SD Clock is stable.
    systemTimer.waitUntil([]() { return (emmcRegister[CONTROL1] & (0x01 << 1)) != 0; }, 1000000);

    // enable the SD Clock
    systemTimer.wait(2000);
    control1 = emmcRegister[CONTROL1];
	control1 |= 0x04;
    emmcRegister[CONTROL1] = control1;
    systemTimer.wait(2000);

	// Mask off sending interrupts to the ARM
    emmcRegister[IRPT_EN] = 0; // 모든 interrupt(status, error)를 disable 시킨다. 
	// Reset interrupt status
    emmcRegister[INTERRUPT] = 0xffffffff; // 각 bit에 1을 write하면 필요에 따라 상태가 clear되거나 기존 값을 유지하도록 되어있다. 
	// Have all interrupts sent to the INTERRUPT register
    emmcRegister[IRPT_MASK] = ~SD_CARD_INTERRUPT; // 8번 bit를 0으로 설정함으로써 host controller각 모든 interrupt request를 clear한다.
    systemTimer.wait(2000);

    console.write("All Interrupt disabled.\n");

    issueCommand(GO_IDLE_STATE, 0, 500000);
	if (FAIL()) {
        sprintf(buf, "SD: no CMD0 response\n");
		return ResultCode::Fail;
	}

	// Send CMD8 to the card
	// Voltage supplied = 0x1 = 2.7-3.6V (standard)
	// Check pattern = 10101010b (as per PLSS 4.3.13) = 0xAA
// #ifdef EMMC_DEBUG
//     printf("SD: note a timeout error on the following command (CMD8) is normal "
//            "and expected if the SD card version is less than 2.0\n");
// #endif
	issueCommand(SEND_IF_COND, 0x1aa, 500000);
	int v2_later = 0;
	if (TIMEOUT()) {
        v2_later = 0;
        sprintf(buf, "SD: Old version detected.\n");
        console.write(buf);
    }
    else if(CMD_TIMEOUT()) {
        if (resetCmd() == -1)
    		return ResultCode::Fail;
        emmcRegister[INTERRUPT] = SD_ERR_MASK_CMD_TIMEOUT;
        v2_later = 0;
    } else if (FAIL()) {
        sprintf(buf, "SD: failure sending CMD8 (%x)\n", last_interrupt);
        console.write(buf);
        return ResultCode::Fail;
    }
    else {
        if ((last_r0 & 0xfff) != 0x1aa) {
            sprintf(buf, "SD: unusable card\n");
            console.write(buf);
            sprintf(buf, "SD: CMD8 response %08x\n", last_r0);
            console.write(buf);
    		return ResultCode::Fail;
        }
        else
            v2_later = 1;
    }

    // Here we are supposed to check the response to CMD5 (HCSS 3.6)
    // It only returns if the card is a SDIO card
// #ifdef EMMC_DEBUG
//     printf("SD: note that a timeout error on the following command (CMD5) is "
//            "normal and expected if the card is not a SDIO card.\n");
// #endif
    issueCommand(IO_SET_OP_COND, 0, 10000);
    if (!TIMEOUT()) {
        if (CMD_TIMEOUT()) {
            if (resetCmd() == -1)
        		return ResultCode::Fail;
            emmcRegister[INTERRUPT] = SD_ERR_MASK_CMD_TIMEOUT; // 주석으로 봤을 때 타임아웃이 나서 여기로 들어와야 맞는 듯...
        }
        else {
            sprintf(buf, "SD: SDIO card detected - not currently supported\n");
            console.write(buf);
            sprintf(buf, "SD: CMD5 returned %x\n", last_r0);
            console.write(buf);
    		return ResultCode::Fail;
        }
    }

    // Call an inquiry ACMD41 (voltage window = 0) to get the OCR
// #ifdef EMMC_DEBUG
//     printf("SD: sending inquiry ACMD41\n");
// #endif
    issueCommand(ACMD(41), 0, 500000);
    if (FAIL()) {
        sprintf(buf, "SD: inquiry ACMD41 failed\n");
        console.write(buf);
        return ResultCode::Fail;
    }
    sprintf(buf, "SD: inquiry ACMD41 returned %x\n", last_r0);
    console.write(buf);


	// Call initialization ACMD41
	int card_is_busy = 1;
	while (card_is_busy) {
	    Uint32 v2_flags = 0;
	    if (v2_later) {
	        // Set SDHC support
	        v2_flags |= (1 << 30);
	    }

	    issueCommand(ACMD(41), 0x00ff8000 | v2_flags, 500000);
	    if (FAIL()) {
	        sprintf(buf, "SD: error issuing ACMD41\n");
            console.write(buf);
            return ResultCode::Fail;
	    }

	    if ((last_r0 >> 31) & 0x1) {
	        // Initialization is complete
	        card_ocr = (last_r0 >> 8) & 0xffff;
	        card_supports_sdhc = (last_r0 >> 30) & 0x1;

	        card_is_busy = 0;
	    }
	    else {
	        // Card is still busy
            sprintf(buf, "SD: card is busy, retrying\n");
            console.write(buf);

            systemTimer.wait(500000); // 500ms
	    }
	}
	sprintf(buf, "SD: card identified: OCR: %x, 1.8v support: %i, SDHC support: %i\n",
			card_ocr, card_supports_18v, card_supports_sdhc);
    console.write(buf);


    // At this point, we know the card is definitely an SD card, so will definitely
	//  support SDR12 mode which runs at 25 MHz
    switchClockRate(baseClock, SD_CLOCK_NORMAL);

	// A small wait before the voltage switch
    systemTimer.wait(5000);

    return ResultCode::Success;
}

Uint32 Emmc::getBaseClock() {
    Uint32 clockRate = 0;
    if (mailboxProperty.getClockRate(ClockId::CIEmmc, &clockRate) != ResultCode::Success)
        return 0; // fail.

    return clockRate;
}

void Emmc::issueCommand(Uint32 command, Uint32 argument, Uint32 timeout) {
    // First, handle any pending interrupts
    handleInterrupts();

    // Stop the command issue if it was the card remove interrupt that was handled
    // if (dev->card_removal)
    // {
    //     dev->last_cmd_success = 0;
    //     return;
    // }

    // Now run the appropriate commands by calling sd_issue_command_int()
    if (command & IS_APP_CMD) {
        command &= 0xff;

        // if (sd_acommands[command] == SD_CMD_RESERVED(0)) {
        //     printf("SD: invalid command ACMD%i\n", command);
        //     dev->last_cmd_success = 0;
        //     return;
        // }
        last_cmd = APP_CMD;

        Uint32 rca = 0;
        if (card_rca)
            rca = card_rca << 16;
        issueCommandInt(sd_commands[APP_CMD], rca, timeout);
        if (last_cmd_success) {
            last_cmd = command | IS_APP_CMD;
            issueCommandInt(sd_acommands[command], argument, timeout);
        }
    }
    else {
        if(sd_commands[command] == SD_CMD_RESERVED(0)) {
            sprintf(buf, "SD: invalid command CMD%i\n", command);
            console.write(buf);
            last_cmd_success = 0;
            return;
        }

        last_cmd = command;
        issueCommandInt(sd_commands[command], argument, timeout);
    }
}

void Emmc::issueCommandInt(Uint32 cmd_reg, Uint32 argument, Uint32 timeout) {
    last_cmd_reg = cmd_reg;
    last_cmd_success = 0;

    // This is as per HCSS 3.7.1.1/3.7.2.2

    // Check Command Inhibit
    while (emmcRegister[STATUS] & 0x1) // Command line이 아직 사용중인지 체크한다.
        systemTimer.wait(1000);

    // Is the command with busy?
    if ((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) {
        // With busy

        // Is is an abort command?
        if((cmd_reg & SD_CMD_TYPE_MASK) != SD_CMD_TYPE_ABORT) {
            // Not an abort command

            // Wait for the data line to be free
            while(emmcRegister[STATUS] & 0x2)
                systemTimer.wait(1000);
        }
    }

    // Is this a DMA transfer?
    int is_sdma = 0;
    if((cmd_reg & SD_CMD_ISDATA) && (use_sdma)) {
        is_sdma = 1;
    }

    if (is_sdma) {
        // Set system address register (ARGUMENT2 in RPi)

        // We need to define a 4 kiB aligned buffer to use here
        // Then convert its virtual address to a bus address
        emmcRegister[ARG2] = SDMA_BUFFER_PA;
    }

    // Set block size and block count
    // For now, block size = 512 bytes, block count = 1,
    //  host SDMA buffer boundary = 4 kiB
    if (blocks_to_transfer > 0xffff) {
        sprintf(buf, "SD: blocks_to_transfer too great (%i)\n", blocks_to_transfer);
        last_cmd_success = 0;
        return;
    }

    Uint32 blksizecnt = block_size | (blocks_to_transfer << 16);
    emmcRegister[BLKSIZECNT], blksizecnt;

    // Set argument 1 reg
    emmcRegister[ARG1] = argument;

    if (is_sdma) {
        // Set Transfer mode register
        cmd_reg |= SD_CMD_DMA;
    }

    // Set command reg
    emmcRegister[CMDTM] = cmd_reg;

    systemTimer.wait(2000);

    // Wait for command complete interrupt
    systemTimer.waitUntil([]() { return (emmcRegister[INTERRUPT] & 0x8001) != 0; }, timeout);
    Uint32 irpts = emmcRegister[INTERRUPT];

    // Clear command complete status
    emmcRegister[INTERRUPT] = 0xffff0001;

    // Test for errors
    if ((irpts & 0xffff0001) != 0x1) {
        last_error = irpts & 0xffff0000;
        last_interrupt = irpts;
        return;
    }

    systemTimer.wait(2000);

    // Get response data
    switch(cmd_reg & SD_CMD_RSPNS_TYPE_MASK) {
        case SD_CMD_RSPNS_TYPE_48:
        case SD_CMD_RSPNS_TYPE_48B:
            last_r0 = emmcRegister[RESP0];
            break;

        case SD_CMD_RSPNS_TYPE_136:
            last_r0 = emmcRegister[RESP0];
            last_r1 = emmcRegister[RESP1];
            last_r2 = emmcRegister[RESP2];
            last_r3 = emmcRegister[RESP3];
            break;
    }

    // If with data, wait for the appropriate interrupt
    if ((cmd_reg & SD_CMD_ISDATA) && (is_sdma == 0)) {
        Uint32 wr_irpt;
        int is_write = 0;
        if (cmd_reg & SD_CMD_DAT_DIR_CH)
            wr_irpt = (1 << 5);     // read
        else {
            is_write = 1;
            wr_irpt = (1 << 4);     // write
        }

        int cur_block = 0;
        uint32_t *cur_buf_addr = (uint32_t *)internalBuf;
        while (cur_block < blocks_to_transfer) {
            // systemTimer.waitUntil([wr_irpt]() { return (emmcRegister[INTERRUPT] & (wr_irpt | 0x8000)) != 0; }, timeout);
            Uint32 hi = systemTimer.getCounterHi();
            Uint32 lo = systemTimer.getCounterLo() + timeout;
            if (lo < systemTimer.getCounterLo())
                hi++;
            do {
                if (emmcRegister[INTERRUPT] & (wr_irpt | 0x8000))
                    break;
            } while ((systemTimer.getCounterHi() < hi) || (systemTimer.getCounterLo() < lo));

            irpts = emmcRegister[INTERRUPT];
            emmcRegister[INTERRUPT] = 0xffff0000 | wr_irpt;

            if ((irpts & (0xffff0000 | wr_irpt)) != wr_irpt) {
//                printf("SD: error occured whilst waiting for data ready interrupt\n");
                last_error = irpts & 0xffff0000;
                last_interrupt = irpts;
                return;
            }

            // Transfer the block
            Uint32 cur_byte_no = 0;
            while (cur_byte_no < block_size) {
                if (is_write) {
					// uint32_t data = read_word((uint8_t *)cur_buf_addr, 0);
                    Uint32 data = *((Uint32 *)cur_buf_addr);
                    emmcRegister[DATA] = data;
				}
                else {
					uint32_t data = emmcRegister[DATA];
                    *((Uint32 *)cur_buf_addr) = data;
					// write_word(data, (uint8_t *)cur_buf_addr, 0);
				}
                cur_byte_no += 4;
                cur_buf_addr++;
            }

//			printf("SD: block %i transfer complete\n", cur_block);
            cur_block++;
        }
    }

    // Wait for transfer complete (set if read/write transfer or with busy)
    if ((((cmd_reg & SD_CMD_RSPNS_TYPE_MASK) == SD_CMD_RSPNS_TYPE_48B) ||
       (cmd_reg & SD_CMD_ISDATA)) && (is_sdma == 0)) {
        // First check command inhibit (DAT) is not already 0
        if ((emmcRegister[STATUS] & 0x2) == 0)
            emmcRegister[INTERRUPT] = 0xffff0002;
        else {
            systemTimer.waitUntil([]() { return (emmcRegister[INTERRUPT] & 0x8002) != 0; }, timeout);
            irpts = emmcRegister[INTERRUPT];
            emmcRegister[INTERRUPT] = 0xffff0002;

            // Handle the case where both data timeout and transfer complete
            //  are set - transfer complete overrides data timeout: HCSS 2.2.17
            if (((irpts & 0xffff0002) != 0x2) && ((irpts & 0xffff0002) != 0x100002)) {
//                printf("SD: error occured whilst waiting for transfer complete interrupt\n");
                last_error = irpts & 0xffff0000;
                last_interrupt = irpts;
                return;
            }

            emmcRegister[INTERRUPT] = 0xffff0002;
        }
    }
    else if (is_sdma) {
        // For SDMA transfers, we have to wait for either transfer complete,
        //  DMA int or an error

        // First check command inhibit (DAT) is not already 0
        if((emmcRegister[STATUS] & 0x2) == 0)
            emmcRegister[INTERRUPT] = 0xffff000a;
        else {
            systemTimer.waitUntil([]() { return (emmcRegister[INTERRUPT] & 0x800a) != 0; }, timeout);
            irpts = emmcRegister[INTERRUPT];
            emmcRegister[INTERRUPT] = 0xffff000a;

            // Detect errors
            if ((irpts & 0x8000) && ((irpts & 0x2) != 0x2)) {
//                printf("SD: error occured whilst waiting for transfer complete interrupt\n");
                last_error = irpts & 0xffff0000;
                last_interrupt = irpts;
                return;
            }

            // Detect DMA interrupt without transfer complete
            // Currently not supported - all block sizes should fit in the
            //  buffer
            if ((irpts & 0x8) && ((irpts & 0x2) != 0x2)) {
//                printf("SD: error: DMA interrupt occured without transfer complete\n");
                last_error = irpts & 0xffff0000;
                last_interrupt = irpts;
                return;
            }

            // Detect transfer complete
            if (irpts & 0x2) {
//                printf("SD: SDMA transfer complete");
                // Transfer the data to the user buffer
                memcpy(internalBuf, (const void *)SDMA_BUFFER, block_size);
            }
            else {
                // Unknown error
// #ifdef EMMC_DEBUG
//                 if (irpts == 0)
//                     printf("SD: timeout waiting for SDMA transfer to complete\n");
//                 else
//                     printf("SD: unknown SDMA transfer error\n");
//                 printf("SD: INTERRUPT: %08x, STATUS %08x\n", irpts,
//                        mmio_read(emmc_base + EMMC_STATUS));
// #endif

                if ((irpts == 0) && ((emmcRegister[STATUS] & 0x3) == 0x2)) {
                    // The data transfer is ongoing, we should attempt to stop it
//                    printf("SD: aborting transfer\n");
                    emmcRegister[CMDTM] = sd_commands[STOP_TRANSMISSION];
                }
                last_error = irpts & 0xffff0000;
                last_interrupt = irpts;
                return;
            }
        }
    }

    // Return success
    last_cmd_success = 1;
}

void Emmc::handleInterrupts() {
    /**
     * 로그를 찍는 것 말고는 그냥 전체 bit를 리셋하는 것과 별 다른 작업이 없는 것 같은데..???
     */
    Register interrupt = emmcRegister[INTERRUPT];
    Register reset_mask = 0;

    if (interrupt & SD_COMMAND_COMPLETE) {
        sprintf(buf, "SD: spurious command complete interrupt\n");
        console.write(buf);
        reset_mask |= SD_COMMAND_COMPLETE;
    }

    if (interrupt & SD_TRANSFER_COMPLETE) {
        sprintf(buf, "SD: spurious transfer complete interrupt\n");
        console.write(buf);
        reset_mask |= SD_TRANSFER_COMPLETE;
    }

    if (interrupt & SD_BLOCK_GAP_EVENT) {
        sprintf(buf, "SD: spurious block gap event interrupt\n");
        console.write(buf);
        reset_mask |= SD_BLOCK_GAP_EVENT;
    }

    if (interrupt & SD_DMA_INTERRUPT) {
        sprintf(buf, "SD: spurious DMA interrupt\n");
        console.write(buf);
        reset_mask |= SD_DMA_INTERRUPT;
    }

    if (interrupt & SD_BUFFER_WRITE_READY) {
        sprintf(buf, "SD: spurious buffer write ready interrupt\n");
        console.write(buf);
        reset_mask |= SD_BUFFER_WRITE_READY;
        resetDat();
    }

    if(interrupt & SD_BUFFER_READ_READY) {
        sprintf(buf, "SD: spurious buffer read ready interrupt\n");
        console.write(buf);
        reset_mask |= SD_BUFFER_READ_READY;
        resetDat();
    }

    if (interrupt & SD_CARD_INSERTION) {
        sprintf(buf, "SD: card insertion detected\n");
        console.write(buf);
        reset_mask |= SD_CARD_INSERTION;
    }

    if (interrupt & SD_CARD_REMOVAL) {
        sprintf(buf, "SD: card removal detected\n");
        console.write(buf);
        reset_mask |= SD_CARD_REMOVAL;
        card_removal = 1;
    }

    if (interrupt & SD_CARD_INTERRUPT) {
        sprintf(buf, "SD: card interrupt detected\n");
        console.write(buf);
        handleCardInterrupt();
        reset_mask |= SD_CARD_INTERRUPT;
    }

    if (interrupt & 0x8000)
    {
        sprintf(buf, "SD: spurious error interrupt: %x\n", interrupt);
        console.write(buf);
        reset_mask |= 0xffff0000;
    }

    emmcRegister[INTERRUPT] = reset_mask;
}

void Emmc::handleCardInterrupt() {
    // Handle a card interrupt

    // Get the card status
    if (card_rca) {
        issueCommandInt(sd_commands[SEND_STATUS], card_rca << 16, 500000);
        if(FAIL()) {
            sprintf(buf, "SD: unable to get card status\n");
            console.write(buf);
        }
        else {
            sprintf(buf, "SD: card status: %x\n", last_r0);
            console.write(buf);
        }
    }
    else {
        sprintf(buf, "SD: no card currently selected\n");
        console.write(buf);
    }
}

// Reset the DATA line
int Emmc::resetDat() {
    Register control1 = emmcRegister[CONTROL1];
	control1 |= SD_RESET_DAT;
    emmcRegister[CONTROL1] = control1;

	systemTimer.waitUntil([]() { return ((emmcRegister[CONTROL1] & SD_RESET_DAT) == 0); }, 1000000);
	if ((emmcRegister[CONTROL1] & SD_RESET_DAT) != 0) {
		sprintf(buf, "EMMC: DAT line did not reset properly\n");
        console.write(buf);
		return -1;
	}

	return 0;
}

int Emmc::resetCmd() {
    Uint32 control1 = emmcRegister[CONTROL1];
	control1 |= SD_RESET_CMD;
	emmcRegister[CONTROL1] = control1;

    systemTimer.waitUntil([]() { return (emmcRegister[CONTROL1] & SD_RESET_CMD) == 0; }, 1000000);
	if((emmcRegister[CONTROL1] & SD_RESET_CMD) != 0) {
		sprintf(buf, "EMMC: CMD line did not reset properly\n");
        console.write(buf);
		return -1;
	}

	return 0;
}

// Switch the clock rate whilst running
int Emmc::switchClockRate(Uint32 base_clock, Uint32 target_rate) {
    // Decide on an appropriate divider
    Uint32 divider = getClockDivider(base_clock, target_rate);
    if (divider == SD_GET_CLOCK_DIVIDER_FAIL) {
        sprintf(buf, "EMMC: couldn't get a valid divider for target rate %i Hz\n", target_rate);
        console.write(buf);
        return -1;
    }

    // Wait for the command inhibit (CMD and DAT) bits to clear
    while (emmcRegister[STATUS] & 0x3)
        systemTimer.wait(1000);

    // Set the SD clock off
    Register control1 = emmcRegister[CONTROL1];
    control1 &= ~(1 << 2);
    emmcRegister[CONTROL1] = control1;
    systemTimer.wait(2000);

    // Write the new divider
	control1 &= ~0xffe0;		// Clear old setting + clock generator select
    control1 |= divider;
    emmcRegister[CONTROL1] = control1;
    systemTimer.wait(2000);

    // Enable the SD clock
    control1 |= (1 << 2);
    emmcRegister[CONTROL1] = control1;
    systemTimer.wait(2000);

    sprintf(buf, "EMMC: successfully set clock rate to %i Hz\n", target_rate);
    console.write(buf);
    return 0;
}

// Set the clock dividers to generate a target value
Uint32 Emmc::getClockDivider(Uint32 base_clock, Uint32 target_rate)
{
    // TODO: implement use of preset value registers

    Uint32 targetted_divisor = 0;
    if (target_rate > base_clock)
        targetted_divisor = 1;
    else {
        targetted_divisor = base_clock / target_rate;
        uint32_t mod = base_clock % target_rate;
        if(mod)
            targetted_divisor--;
    }

    // Decide on the clock mode to use

    // Currently only 10-bit divided clock mode is supported

// #ifndef EMMC_ALLOW_OLD_SDHCI
//     if(hci_ver >= 2)
//     {
// #endif
        // HCI version 3 or greater supports 10-bit divided clock mode
        // This requires a power-of-two divider

        // Find the first bit set
        int divisor = -1;
        for (int first_bit = 31; first_bit >= 0; first_bit--)
        {
            uint32_t bit_test = (1 << first_bit);
            if(targetted_divisor & bit_test)
            {
                divisor = first_bit;
                targetted_divisor &= ~bit_test;
                if(targetted_divisor)
                {
                    // The divisor is not a power-of-two, increase it
                    divisor++;
                }
                break;
            }
        }

        if(divisor == -1)
            divisor = 31;
        if(divisor >= 32)
            divisor = 31;

        if(divisor != 0)
            divisor = (1 << (divisor - 1));

        if(divisor >= 0x400)
            divisor = 0x3ff;

        uint32_t freq_select = divisor & 0xff;
        uint32_t upper_bits = (divisor >> 8) & 0x3;
        uint32_t ret = (freq_select << 8) | (upper_bits << 6) | (0 << 5);

        int denominator = 1;
        if(divisor != 0)
            denominator = divisor * 2;
        int actual_clock = base_clock / denominator;
        sprintf(buf, "EMMC: base_clock: %i, target_rate: %i, divisor: %08x, "
               "actual_clock: %i, ret: %08x\n", base_clock, target_rate,
               divisor, actual_clock, ret);
        console.write(buf);

        return ret;
// #ifndef EMMC_ALLOW_OLD_SDHCI
//     }
//     else
//     {
//         sprintf(buf, "EMMC: unsupported host version\n");
//         console.write(buf);
//         return SD_GET_CLOCK_DIVIDER_FAIL;
//     }
// #endif
}

ResultCode Emmc::powerOnSdCard() {
    Uint32 state = static_cast<Uint32>(PowerState::PSOnBit) | static_cast<Uint32>(PowerState::PSWaitBit);
    return mailboxProperty.setPowerState(DeviceId::DISdCard, &state);
}

ResultCode Emmc::powerOffSdCard() {
    Uint32 state = static_cast<Uint32>(PowerState::PSWaitBit);
    return mailboxProperty.setPowerState(DeviceId::DISdCard, &state);
}

ResultCode Emmc::getPowerState(Uint32 * state) {
    return mailboxProperty.getPowerState(DeviceId::DISdCard, state);
}
