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

			mov		#0x2000, R11
			mov		#0x2002, R10
			call	#unsigFibonacci
			jmp		$
			nop


unsigFibonacci:

			clr		R9

			push	R10						; endereco de resposta
			push	R9						; contador
			push	R7						; A(0)
			push	R6						; A(1)

			mov 	#1, R7					; inicializa os 2 primeiros numeros da serie
			mov		R7, 0(R10)
			mov 	#2, R6
			mov		R6, 2(R10)
			add		#4, R10

loop:
											; R5 = R6 + R7, sendo que damos overwrite no  R5 com o numero anterior na serie
			mov 	R6, R5					; R5 = R6
			add 	R7, R5					; R5 = R5 + R7

			mov		R5, 0(R10)				; botamos na memoria
			add		#2, R10					; passamos endereco pra frente

			mov 	R6, R7					; atualiza os novos registros que mantem fibonacci
			mov 	R5, R6

			cmp		#16, R9
			jeq		fim

			inc		R9						; incrementa contador
			jmp 	loop					; repete o loop

fim:
			pop		R10
			pop		R9
			pop		R7
			pop		R6
			ret								; faz ate o maximo possivel em 16 bits
                                            

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
            
