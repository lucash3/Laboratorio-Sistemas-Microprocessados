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

MATR		.set	17016
                                            
			mov		#MATR, R6				; numero em R6 virará nibbles respectivos em R6
			mov		#0x2000, R5				; endereço de escrita final
			call	#W16_ASC
			jmp		$
			nop

W16_ASC:
			push	R6
			push 	R5

			mov		R6, R13					; move nosso numero pra R13
			mov		#0x1000, R12			; veremos quantos "1000" em hex encaixa no nosso numero
			call	#udiv

			call	#NIB_ASC				; converte R14
			mov		R14, 0(R5)				; sera o maior numero em ascii no endereço
			clr		R14
			add		#2, R5					; vai pro proximo, e a cada passada o numero é abatido do R13, copia do nosso numero

			mov		#0x0100, R12
			call	#udiv

			call	#NIB_ASC				; converte R14
			mov		R14, 0(R5)
			clr		R14
			add		#2, R5

			mov		#0x0010, R12
			call	#udiv

			call	#NIB_ASC				; converte R14
			mov		R14, 0(R5)
			clr		R14
			add		#2, R5

			mov		#0x0001, R12
			call	#udiv

			call	#NIB_ASC				; converte R14
			mov		R14, 0(R5)
			clr		R14
			add		#2, R5

			jmp		fim

NIB_ASC:

			cmp		#0x0009, R14
			jeq		temos_9
			cmp		#0x0008, R14
			jeq		temos_8
			cmp		#0x0007, R14
			jeq		temos_7
			cmp		#0x0006, R14
			jeq		temos_6
			cmp		#0x0005, R14
			jeq		temos_5
			cmp		#0x0004, R14
			jeq		temos_4
			cmp		#0x0003, R14
			jeq		temos_3
			cmp		#0x0002, R14
			jeq		temos_2
			cmp		#0x0001, R14
			jeq		temos_1
			cmp		#0x0000, R14
			jeq		temos_0

			cmp		#0x000A, R14
			jeq		temos_A
			cmp		#0x000B, R14
			jeq		temos_B
			cmp		#0x000C, R14
			jeq		temos_C
			cmp		#0x000D, R14
			jeq		temos_D
			cmp		#0x000E, R14
			jeq		temos_E
			cmp		#0x000F, R14
			jeq		temos_F


temos_A:
			mov		#0x0041, R14
			ret
temos_B:
			mov		#0x0042, R14
			ret
temos_C:
			mov		#0x0043, R14
			ret
temos_D:
			mov		#0x0044, R14
			ret
temos_E:
			mov		#0x0045, R14
			ret
temos_F:
			mov		#0x0046, R14
			ret

temos_9:
			mov		#0x0039, R14
			ret
temos_8:
			mov		#0x0038, R14
			ret
temos_7:
			mov		#0x0037, R14
			ret
temos_6:
			mov		#0x0036, R14
			ret
temos_5:
			mov		#0x0035, R14
			ret
temos_4:
			mov		#0x0034, R14
			ret
temos_3:
			mov		#0x0033, R14
			ret
temos_2:
			mov		#0x0032, R14
			ret
temos_1:
			mov		#0x0031, R14
			ret
temos_0:
			mov		#0x0030, R14
			ret

udiv:
			clr		R14
			cmp		R12, R13
			jl		fim_divisao

udiv_loop:

			inc		R14						 ; retorna R14  como resposta da divisão
			sub		R12, R13

			cmp		#0, R13
			jeq		divisivel
			cmp		R12, R13
			jeq		udiv_loop
			jhs		udiv_loop
			jl		nao_divisivel

divisivel:
			mov		#1, R4
			jmp		fim_divisao

nao_divisivel:

			mov		#0, R4
			jmp		fim_divisao

fim_divisao:
			ret



fim:
			pop		R5
			pop		R6
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
            
