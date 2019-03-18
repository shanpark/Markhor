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
    mov sp, #0x8000

    /* branch to StartUp */
    b   StartUp

/* Interrupt Vector Table Entries */
_interruptVectorTable:
    ldr pc, resetAddr
    ldr pc, undefinedInstructionAddr
    ldr pc, softwareAddr
    ldr pc, prefetchAbortAddr
    ldr pc, dataAbortAddr
    nop
    ldr pc, interruptRequestAddr
    ldr pc, fastInterruptRequestAddr

/* Interrupt Handler Addresses */
resetAddr:                   .word reset
undefinedInstructionAddr:    .word undefinedInstruction
softwareAddr:                .word software
prefetchAbortAddr:           .word prefetchAbort
dataAbortAddr:               .word dataAbort
interruptRequestAddr:        .word interruptRequest
fastInterruptRequestAddr:    .word fastInterruptRequest

@ /**
@  * enableInterrupts() function definition
@  */
@ .global _enableInterruptRequest
@ _enableInterruptRequest:
@     mrs r0, cpsr
@     bic r0, r0, #0x80
@     msr cpsr_c, r0

@     mov pc, lr
