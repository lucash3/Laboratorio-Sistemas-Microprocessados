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

main:
			mov			#vetor1,R5
			mov			#vetor2,R6
			call		#SUM16
			jmp			$

;-------------------------------------------------------------------------------

SUM16:
			push		R5					;salva contexto
			push		R6
			push		R8
			push		R9

			mov			#0x2020,R7			;aponta para endereco do novo vetor
			mov			R7,R8				;mesma coisa de cima, para andar
			mov.w		@R5+,R9				;salva tam. dos vet. (contador)
			add			#2,R6				;pula tamanho, pois sao iguais
			mov.w		R9,0(R7)			;coloca tamanho no novo vetor
			add			#2,R8				;pula end. do tamanho do novo vetor

SUM16_loop:
			mov.w		#0,0(R8)			;zera valor do elem. do novo vetor
			add.w		@R5+,0(R8)			;adiciona valor do vetor 1
			add.w		@R6+,0(R8)			;adiciona valor do vetor 2

			add			#2,R8				;proximo elem. do vetor
			dec			R9					;decrementa contador
			jnz			SUM16_loop			;continua loop apropriadamente

			pop			R9					;restaura contexto
			pop			R8
			pop			R6
			pop			R5
			ret

;-------------------------------------------------------------------------------

			.data
vetor1:		.word	5,1,2,3,4,5
vetor2:		.word	5,1,2,3,4,5

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
            
