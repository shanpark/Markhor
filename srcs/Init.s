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

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.section ".init"

/**
 * entry point 
 */
.global _start
_start:
    /* initialize interrupt vector table */
    mov r0, #0x8000 
    add r0, #_interruptVectorTable
    mov r1, #0x0000
    ldmia r0!, { r2, r3, r4, r5, r6, r7, r8, r9 } /* copy vector entries: 8 words */
    stmia r1!, { r2, r3, r4, r5, r6, r7, r8, r9 }
    ldmia r0!, { r2, r3, r4, r5, r6, r7, r8 } /* copy handler addresses: 7 words */
    stmia r1!, { r2, r3, r4, r5, r6, r7, r8 }

    /* initialize stack */
    /* change mode to interrupt mode */
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0

    /* set interrupt mode stack pointer */
    mov sp, #0x4000 

    /* change mode to supervisor mode */
    mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0

    mov sp, #0x8000

    /* branch to StartUp */
    b   StartUp

/* Interrupt Vector Table Entries */
_interruptVectorTable:
    ldr pc, resetHandlerPointer
    ldr pc, undefinedInstructionHandlerPointer
    ldr pc, softwareInterruptHandlerPointer
    ldr pc, prefetchAbortHandlerPointer
    ldr pc, dataAbortHandlerPointer
    nop
    ldr pc, interruptRequestHandlerPointer
    ldr pc, fastInterruptRequestHandlerPointer

/* Interrupt Handler Addresses */
resetHandlerPointer:                    .word resetHandler
undefinedInstructionHandlerPointer:     .word undefinedInstructionHandler
softwareInterruptHandlerPointer:        .word softwareInterruptHandler
prefetchAbortHandlerPointer:            .word prefetchAbortHandler
dataAbortHandlerPointer:                .word dataAbortHandler
interruptRequestHandlerPointer:         .word interruptRequestHandler
fastInterruptRequestHandlerPointer:     .word fastInterruptRequestHandler
