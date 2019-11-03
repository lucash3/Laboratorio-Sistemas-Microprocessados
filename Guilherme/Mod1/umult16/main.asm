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

			mov		#5, R5
			mov		#2, R4					; 2 x 5
			call	#umult16
			jmp		$
			nop

umult16:
			cmp		R4, R5
			jhs		umult16_init			; se R5 é maior que R4, vai

			mov		R4, R6					; se R4 é maior que R5, muda os dois de lugar
			mov		R5, R4					; R6 é temporario
			mov		R6, R5

umult16_init:

			clr		R6
			clr		R7

umult16_loop:

			add		R5, R6					; R6 = R5 + R6
			addc	#0, R7					; R7 = 0 + carry
			dec		R4						; soma R5 no R6 "R4" vezes
			jnz		umult16_loop			; enquanto R4 não dá 0
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
            
