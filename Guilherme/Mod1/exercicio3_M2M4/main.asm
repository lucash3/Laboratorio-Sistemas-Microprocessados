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

			mov			#vetor1, R5			; R6 recebe mult2, R7 recebe mult4
			call		#M2M4
			jmp			$
			nop

M2M4:
			mov.b	0(R5), R8				; R8 = tamanho do vetor
			add		#1, R5					; caminha no vetor
			clr		R14
			clr		R6
			clr		R7

loop_divisores:

			mov.b	0(R5), R13				; passa o numero atual pro R13
			mov		#0x0002, R12			; sera dividido por 2
			call	#udiv					; faz a divisao
			cmp		#1, R4					; se eh divisivel, a flag retorna 1
			jeq		aumenta_divisiveis_2	; incrementa R6

dividir_por_4:

			clr		R4						; limpa o ultimo resultado
			mov		#0x0004, R12			; dividiremos por 4
			call	#udiv					; faz a divisão
			cmp		#1, R4					; ve a flag de retorno
			jeq		aumenta_divisiveis_4	; incrementa R7

proximo_passo_divisao:

			clr		R4						; limpa a flag
			add		#1, R5					; vai pro proximo endereço
			add		#1, R14					; aumenta o contador de qnts numeros analisamos

			cmp		R8, R14					; if numeros analisados = numeros no vetor, acaba o programa
			jeq		fim_algoritmo			; else, faz dnv
			jl		loop_divisores

aumenta_divisiveis_2:
			inc		R6
			jmp		dividir_por_4

aumenta_divisiveis_4:
			inc		R7
			jmp		proximo_passo_divisao
                                            
udiv:

			push	R13					; quociente, contexto dado pois ele sera suibtraido
			push	R14					; resultado da divisao, n esta sendo analisado logo vira contexto
			clr		R14					; queremos apenas a flag R4
			cmp		R12, R13
			jl		fim_divisao

udiv_loop:

			inc		R14
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

			pop		R14
			pop		R13
			ret

fim_algoritmo:

			ret


			.data

vetor1	 	.byte		6, 12, 4, 2, 6, 10, 8
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
            
