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


			mov		#5, R12
			mov		#2, R13
			call	#udiv16
			jmp		$
			nop
                                            
udiv16:

			clr 	R14
			clr     R15

			push	R12
			push	R13

udiv16_loop:

			inc		R14						; quociente, quantas vezes dá pra subtrair um numero de outro
			sub		R13, R12				; R12 = R12 - R13

			cmp		#0, R12
			jeq		fim_divisao

			cmp		R13, R12
			jl		fim_divisao				; nesse caso, teve resto

			jhs		udiv16_loop


fim_divisao:

			mov		R12, R15

			pop     R13						; resto
			pop		R12						; quociente

			ret

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
            
