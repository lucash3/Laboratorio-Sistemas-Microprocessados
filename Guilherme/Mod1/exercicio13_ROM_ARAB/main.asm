;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

			mov		#NUM_ROM, R6
			mov		#0x202C, R7
			call    #ROM_ARAB
			jmp     $

;-------------------------------------------------------------------------------

ROM_ARAB:

			push		R6
			push		R7
			push 		R8

			mov			R7, R8

milhares:
			cmp.b		#'M', 0(R6)
			jeq			ehM

quinhentos:
			cmp.b		#'D', 0(R6)
			jeq			ehD
			cmp.b		#'C', 0(R6)
			jeq			ehC

cinquenta:
			cmp.b		#'L', 0(R6)
			jeq			ehL
			cmp.b		#'X', 0(R6)
			jeq			ehX

cinco:
			cmp.b		#'V', 0(R6)
			jeq			ehV
			cmp.b		#'I', 0(R6)
			jeq			ehI

			jmp			acumulador_inicial

;-------------------------------------------------------------------------------

ehM:

			mov    		#1000, 0(R7)
			incd   		R7
			inc			R6
			jmp         milhares
			nop

ehD:

			mov    		#500, 0(R7)
			incd   		R7
			inc			R6
			jmp         quinhentos
			nop

ehC:

			mov    		#100, 0(R7)
			incd   		R7
			inc			R6
			jmp         quinhentos
			nop

ehL:

			mov    		#50, 0(R7)
			incd   		R7
			inc			R6
			jmp         cinquenta
			nop

ehX:

			mov    		#10, 0(R7)
			incd   		R7
			inc			R6
			jmp         cinquenta
			nop

ehV:

			mov    		#5, 0(R7)
			incd   		R7
			inc			R6
			jmp         cinco
			nop

ehI:

			mov    		#1, 0(R7)
			incd   		R7
			inc			R6
			jmp         cinco
			nop

;-------------------------------------------------------------------------------

acumulador_inicial:

			clr         R5					;
			add			0(R7), R5
			sub			#2, R7

acumulador:

			cmp			2(R7), 0(R7)
			jl			subtrai

			add			0(R7), R5			; else

acumulador_loop_sub:

			sub			#2, R7
			cmp			R8, R7
			jl			fim

			jmp			acumulador			; else

subtrai:

			sub			0(R7), R5
			jmp			acumulador_loop_sub


fim:

			pop			R8
			pop			R7
			pop			R6

			ret

;------------------------------------------------------------------------------

				.data

NUM_ROM:			.byte	"MMMCDXIX",0		;2019, ou 7E3  D5B


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
