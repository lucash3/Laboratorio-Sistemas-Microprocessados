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
NUM 		.equ		2019				;Indicar número a ser convertido
;
			mov 		#NUM,R5				;R5 = número a ser convertido
			mov			#RESP,R6			;R6 = ponteiro para escrever a resposta
			call 		#ALG_ROM			;chamar subrotina
			jmp 		$				 	;travar execução
			nop 							;exigido pelo montador
;


;	ENTRADA: 		R5: numero algs. arabes
;	SAIDA:	 		R6: numero algs. romanos
;	AUXILIARES:		R7, R8, R9, R10: digitos da entrada
;					R12, R13: operandos de divisao e multiplicacao
ALG_ROM:
			push		R5					;salva contexto
			push		R7
			push		R8
			push		R9
			push		R10
			push		R12
			push		R13

;--------------------------------------------------------------------
;			OBTENCAO DE DIGITOS
;--------------------------------------------------------------------
			mov			R5,R12				;dig1 (MSD)
			mov			#1000,R13
			call		#udiv16f
			mov			R13,R7



			pop			R13
			pop			R12
			pop			R10
			pop			R9
			pop			R8
			pop			R7
			pop			R5					;restaura contexto

			ret

;	entrada: R12 (num), R13 (div)
;	saida:	 R13 (quo)
udiv16f:
			ret

;-------------------------------------------------------------------

;	entrada: R12, R13
;	saida:	 R14, R15: (Pl, Ph)
umult16:
			ret

			.data
											; Local para armazenar a resposta (RESP = 0x2400)
RESP: 		.byte		"RRRRRRRRRRRRRRRRRR",0
                                            

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
            
