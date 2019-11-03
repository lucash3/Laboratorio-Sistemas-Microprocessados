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

			mov		#vetor1, R5
			mov		#vetor2, R6
			mov		#0x2020, R7
			call	#SUM16			; para funcionar, primeiro endereco representa o tamanho, igual em ambos os vetores

			jmp		$
			nop

;-------------------------------------------------------------------------------

SUM16:
			push	R5				; endereço do vetor 1
			push	R6				; endereço do vetor 2
			push	R7				; endereço da resposta
			push	R8				; variavel temporaria pra acumular a resposta
			push	R9				; tamanho do vetor
			push	R10				; conta loop

inicia_alg:

			mov		0(R5), R9		; tamanho do vetor
			clr		R10				; contador de loop limpo

			incd	R5				; proximo endereco do vetor, o primeiro eh tamanho
			incd	R6				; proximo endereco do vetor, o primeiro eh tamanho

SUM16_loop:

			clr		R8				; limpa o temporario
			add		0(R5), R8		; add no temp
			add		0(R6), R8		; add no temp
			mov		R8, 0(R7)		; bota temp no endereco atual da resposta
			incd	R7				; incrementa resposta
			incd	R6				; incrementa vetor 2
			incd	R5				; incrementa vetor 1
			inc		R10				; incrementa em 1 o contador

			cmp		R9, R10			; se o contador esta igual ao tamanho do vetor, fim
			jeq		fim
			jmp		SUM16_loop		; else, repete


fim:
			pop		R10
			pop		R9
			pop		R8
			pop		R7
			pop		R6
			pop		R5

			ret

;-------------------------------------------------------------------------------

			.data

vetor1:		.word		7, 65000, 	50054, 	26472, 	53000, 	60606, 	  814, 	41121
vetor2:		.word		7, 226, 	3400, 	26472, 	470, 	 1020, 	44444, 	12345

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
            
