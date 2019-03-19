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

.equ    STACK_FIQ,              0x1000
.equ    STACK_IRQ,              0x2000
.equ    STACK_SVR,              0x8000
.equ    STACK_ABORT,            0x3000
.equ    STACK_UNDEFINED,        0x4000
.equ    STACK_SYSTEM,           0x5000

.section ".init"

/**
 * entry point 
 */
.global _start
_start:
    /**
     * initialize interrupt vector table
     */
    mov r0, #0x8000 
    add r0, #_interruptVectorTable
    mov r1, #0x0000
    ldmia r0!, { r2, r3, r4, r5, r6, r7, r8, r9 } /* copy vector entries: 8 words */
    stmia r1!, { r2, r3, r4, r5, r6, r7, r8, r9 }
    ldmia r0!, { r2, r3, r4, r5, r6, r7, r8 } /* copy handler pointers: 7 words */
    stmia r1!, { r2, r3, r4, r5, r6, r7, r8 }

    /**
     * initialize stack
     */
    /* set SVC mode stack pointer */
    mov sp, #STACK_SVR

    /* save previous mode */
    mrs r0, cpsr
    and r1, r0, #0x1f // r1 = previous mode.

    /* change mode to IRQ mode */
    bic r0, r0, #0x1f
    orr r0, r0, #CPSR_MODE_IRQ
    msr cpsr_c, r0

    /* set IRQ mode stack pointer */
    mov sp, #STACK_IRQ 

    /* change mode to FIQ mode */
    bic r0, r0, #0x1f
    orr r0, r0, #CPSR_MODE_FIQ
    msr cpsr_c, r0

    /* set FIQ mode stack pointer */
    mov sp, #STACK_FIQ 

    /* restore previous mode */
    bic r0, r0, #0x1f
    orr r0, r0, r1 // r1 = previous mode.
    msr cpsr_c, r0

    /**
     * branch to StartUp
     */
    b StartUp

/**
 * Interrupt Vector Table Entries
 */
_interruptVectorTable:
    ldr pc, resetHandlerPointer
    ldr pc, undefinedInstructionHandlerPointer
    ldr pc, softwareInterruptHandlerPointer
    ldr pc, prefetchAbortHandlerPointer
    ldr pc, dataAbortHandlerPointer
    nop
    ldr pc, interruptRequestHandlerPointer
    ldr pc, fastInterruptRequestHandlerPointer

/**
 * Interrupt Handler Pointers
 */
resetHandlerPointer:                    .word resetHandler
undefinedInstructionHandlerPointer:     .word undefinedInstructionHandler
softwareInterruptHandlerPointer:        .word softwareInterruptHandler
prefetchAbortHandlerPointer:            .word prefetchAbortHandler
dataAbortHandlerPointer:                .word dataAbortHandler
interruptRequestHandlerPointer:         .word interruptRequestHandler
fastInterruptRequestHandlerPointer:     .word fastInterruptRequestHandler
