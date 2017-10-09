; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"


BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

y_apreto: db 0x00 
en_modo_debug: db 0x00

mensaje_0 		db 'Divide Error'
mensaje_0_len 	equ $ - mensaje_0
mensaje_1 		db 'RESERVED'
mensaje_1_len 	equ $ - mensaje_1
mensaje_2 		db 'NMI Interrupt'
mensaje_2_len 	equ $ - mensaje_2
mensaje_3 		db 'Breakpoint'
mensaje_3_len 	equ $ - mensaje_3
mensaje_4 		db 'Overflow'
mensaje_4_len 	equ $ - mensaje_4
mensaje_5 		db 'BOUND Range Exceeded'
mensaje_5_len 	equ $ - mensaje_5
mensaje_6 		db 'Invalid Opcode'
mensaje_6_len 	equ $ - mensaje_6
mensaje_7 		db 'Device Not Available'
mensaje_7_len 	equ $ - mensaje_7
mensaje_8 		db 'Double Fault'
mensaje_8_len 	equ $ - mensaje_8
mensaje_9 		db 'Coprocessor Segment Overrun'
mensaje_9_len 	equ $ - mensaje_9
mensaje_10 		db 'Invalid TSS'
mensaje_10_len 	equ $ - mensaje_10
mensaje_11 		db 'Segment Not Present'
mensaje_11_len 	equ $ - mensaje_11
mensaje_12 		db 'Stack-Segment Fault'
mensaje_12_len 	equ $ - mensaje_12
mensaje_13 		db 'General Protection'
mensaje_13_len 	equ $ - mensaje_13
mensaje_14 		db 'Page Fault'
mensaje_14_len 	equ $ - mensaje_14
mensaje_16 		db 'Floating-Point Error'
mensaje_16_len 	equ $ - mensaje_16
mensaje_17 		db 'Aligment Check'
mensaje_17_len 	equ $ - mensaje_17
mensaje_18 		db 'Machine Check'
mensaje_18_len 	equ $ - mensaje_18
mensaje_19 		db 'SIMD Floating-Point Exception'
mensaje_19_len 	equ $ - mensaje_19


;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

extern screen_actualizar_reloj_global
extern screen_pintar
extern screen_pintar_rect
extern screen_pintar_linea_h

extern game_atender_teclado
extern game_perro_mover
extern game_perro_cavar
extern game_perro_olfatear
extern game_perro_orden
extern game_perro_termino
extern game_debug


;;PANTALLA
extern screen_guardar_pantalla_actual
extern screen_cargar_pantalla_actual
;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
	;~ xchg bx, bx; por alguna razon el esp y el ebp estan muuy lejos, por eso se llena de cosas el stack.
	push eax
	mov eax, esp
	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi
	mov ecx, eax
	mov ecx, [ecx+16] ;es el esp antes de la interrupción
	push ecx
	mov ecx, eax
	mov ecx, [ecx+4] ;es el eip antes de la interrupción
	push ecx
	mov ecx, eax
	mov ecx, [ecx+8] ;es el cs antes de la interrupción
	push ecx
	push ds
	push es
	push fs
	push gs
	mov ecx, eax
	mov ecx, [ecx+20] ;es el ss antes de la interrupción
	push ecx
	mov ecx, eax
	mov ecx, [ecx+12] ;es el eflags antes de la interrupción
	push ecx
	;hay que hacer add esp, una cantidad
	xor eax, eax
	mov al, [y_apreto]
	cmp al, 0x1
	jne .seguir
	
	; aca hay que guardar la pantalla y generar el cuadrado con la información, despues dejo que mate el perro y salte a la idle
	mov eax, 0x1
	mov [en_modo_debug], al ;significa que rompí
	
	call game_debug
	xchg bx, bx
	.seguir:
		call sched_tarea_actual
		push eax
		call game_perro_termino
		add esp, 4
		
		jmp 0x68:0
	
	imprimir_texto_mp mensaje_%1, mensaje_%1_len, 0x07, 3, 0
    mov eax, %1
	add esp, 28 ;una cantidad que hay que calcular

    jmp $
	;~ ret
%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32
	
_isr32:
	pushad

	call fin_intr_pic1
	
	xor eax, eax
	mov al, [en_modo_debug]
	cmp al, 0x1
	jne .nodebug ;si nunca rompio sigo como estaba 
	
	xor eax, eax
	mov al, [y_apreto]
	cmp al, 0x1
	je .fin ; va a quedar en la idle y el juego queda pausado
	cmp al, 0x2
	jne .nodebug ;esto solo lo hace cuando es 0x0
	
	call screen_cargar_pantalla_actual	;aca hay que dejar la pantalla como estaba
	
	mov al, 0x1
	mov [y_apreto], al ;restauro, para que se vuelva a mostrar la info en pantalla la proxima vez que muera un perro
	mov al, 0x0
	mov [en_modo_debug], al ;restauro modo debug para que vuelva a la normalidad el juego
	;despues de esto vuelve a la normalidad
	.nodebug:
		call sched_atender_tick
		shl eax, 3	
		
		str cx
		cmp ax, cx
		je .fin
		
		mov [sched_tarea_selector], ax
		jmp far [sched_tarea_offset] 
	
	.fin:

		popad
		iret
	
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
global _isr33

_isr33:
	pushad
	call fin_intr_pic1
	xor eax, eax
	in al, 0x60
	xor ebx, ebx
	mov bl, al
	
	cmp bl, 0x15
	jne .atender
	
	xor eax, eax
	mov al, [y_apreto]
	cmp al, 0x0
	jne .comparar
	mov al, 0x1
	mov [y_apreto], al
	jmp .fin
	
	.comparar:
		cmp al, 0x1
		jne .atender
		mov al, 0x2
		mov [y_apreto], al
		jmp .fin
		
	.atender:
		mov al, [en_modo_debug]
		cmp al, 0x1
		je .fin
		push ebx
		call game_atender_teclado
		add esp, 4
		
	.fin:
		popad
		iret
;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

global _isr70

_isr70:
	pushad
	mov esi, eax
	mov edi, ecx
	call fin_intr_pic1
	call sched_tarea_actual
	
	cmp esi, 0x1
	je .moverse
	cmp esi, 0x2
	je .cavar
	cmp esi, 0x3
	je .olfatear
	cmp esi, 0x4
	je .orden
	jmp .fin
	
	.moverse:
		mov ebx, cr3
		push ebx
		push edi
		push eax
		
		call game_perro_mover
		add esp, 12
		jmp .fin
		
	.cavar:
		push eax
		call game_perro_cavar
		add esp, 4
		jmp .fin
		
	.olfatear:
		push eax
		call game_perro_olfatear
		add esp, 4
		jmp .fin
		
	.orden:
		push eax
		call game_perro_orden
		add esp, 4
		jmp .fin
		
	.fin:
		jmp 0x68:0
		mov [esp+28], eax
		popad
		iret


