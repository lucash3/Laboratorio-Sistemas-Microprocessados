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

			mov		#MEMO, R5
			call	#ASC_W16
OK			jc		OK
NOK			jnc		NOK

ASC_W16:
			push	R9
			push	R8
			push	R7
			push	R6
			; R10 eh um auxiliar que retorna a resposta

			mov.b	0(R5), R6
			mov.b	1(R5), R7
			mov.b	2(R5), R8
			mov.b	3(R5), R9
			clr		R10

			mov		R6, R10
			call	#converter
			mov		R10, R6

			mov		R7, R10
			call	#converter
			mov		R10, R7

			mov		R8, R10
			call	#converter
			mov		R10, R8

			mov		R9, R10
			call	#converter
			mov		R10, R9

			jmp		multiplicar_adicionar

converter:

			cmp		#0x30, R10
			jeq		temos0
			cmp		#0x31, R10
			jeq		temos1
			cmp		#0x32, R10
			jeq		temos2
			cmp		#0x33, R10
			jeq		temos3
			cmp		#0x34, R10
			jeq		temos4
			cmp		#0x35, R10
			jeq		temos5
			cmp		#0x36, R10
			jeq		temos6
			cmp		#0x37, R10
			jeq		temos7
			cmp		#0x38, R10
			jeq		temos8
			cmp		#0x39, R10
			jeq		temos9

			cmp		#0x41, R10
			jeq		temosA
			cmp		#0x42, R10
			jeq		temosB
			cmp		#0x43, R10
			jeq		temosC
			cmp		#0x44, R10
			jeq		temosD
			cmp		#0x45, R10
			jeq		temosE
			cmp		#0x46, R10
			jeq		temosF


temos9:
			mov		#0x09, R10
			ret
temos8:
			mov		#0x08, R10
			ret
temos7:
			mov		#0x07, R10
			ret
temos6:
			mov		#0x06, R10
			ret
temos5:
			mov		#0x05, R10
			ret
temos4:
			mov		#0x04, R10
			ret
temos3:
			mov		#0x03, R10
			ret
temos2:
			mov		#0x02, R10
			ret
temos1:
			mov		#0x01, R10
			ret
temos0:
			mov		#0x00, R10
			ret

temosA:
			mov		#0x0A, R10
			ret
temosB:
			mov		#0x0B, R10
			ret
temosC:
			mov		#0x0C, R10
			ret
temosD:
			mov		#0x0D, R10
			ret
temosE:
			mov		#0x0E, R10
			ret
temosF:
			mov		#0x0F, R10
			ret

multiplicar_adicionar:						; periférico da FR2355, multiplicador

			mov		#16, &MPY
			mov		R7, &OP2
			mov		&RES0, R7

			mov		#256, &MPY
			mov		R8, &OP2
			mov		&RES0, R8

			mov		#4096, &MPY
			mov		R9, &OP2
			mov		&RES0, R9

			mov		R6, R10
			add		R7, R10
			add		R8, R10
			add		R9, R10

fim:

			pop		R6
			pop		R7
			pop		R8
			pop		R9

			ret


			.data
MEMO:		.byte	'8', '9', 'A', 'B'

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
            
