;Nome: Pedro Victor Rodrigues de Carvalho
;Matricula: 17/0113043
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
				mov			#code,R4
				mov			#msg,R5
				call		#deScrambler
				jmp			$

deScrambler:
				push		R5
				push		R6
				push		R7
				push		R8
				push		R9
				push		R10

				mov			R4,R6			;R6 e R7 auxiliares para percorrer
				mov			R5,R7			;os vetores sem perder endereco
				mov			#0x2500,R8		;R8 ponteiro na memoria
				mov			#0,R9			;R9 e R10 contadores
				mov			#0,R10

dscrmbl_copy:								;copia vetor msg na memoria @R8
				mov.b		@R7,0(R8)
				inc			R7
				inc			R8
				tst.b		0(R7)			;condicao de termino da copia
				jz			dscrmbl_init	;nao funcional
				jmp			dscrmbl_copy

dscrmbl_init:
				mov.b		#0,R8
				mov			#0x2500,R8		;reseta posicoes de ponteiros
				mov			R5,R7

dscrmbl_loop:
				tst.b		0(R8)			;verifica se terminou vetor copia
				jz			dscrmbl_end		;terminou -> encerra
				mov.b		@R6+,R10		;pega offset do vetor code
				add			R10,R7			;aplica offset no ponteiro R7
				mov.b		@R8+,0(R7)		;coloca byte no local correto
				mov			R5,R7			;reseta ponteiro
				inc			R9				;incrementa contador para code
				cmp			#3,R9			;verifica termino do vetor code
				jeq			dscrmbl_loop	;else -> continua
				jhs			dscrmbl_reset	;terminou -> recomeça
				jlo			dscrmbl_loop	;else -> continua

dscrmbl_reset:								;reinicia contagem e ponteiro
				mov			R4,R6			;do vetor code
				mov			#0,R9
				add			#4,R5			;passa para prox 4 bytes
				mov			R5,R7
				jmp			dscrmbl_loop

dscrmbl_end:
				pop			R10
				pop			R9
				pop			R8
				pop			R7
				pop			R6
				pop			R5
				ret

				.data
code:			.byte		1,3,2,0
msg:			.byte		"itssmsaemci orprcseoaods...s",0

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
            
