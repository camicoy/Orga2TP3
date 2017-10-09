; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start
global magicBreakpoint

extern GDT_DESC
extern IDT_DESC
extern screen_inicializar
extern idt_inicializar
extern mmu_inicializar_dir_kernel
extern mmu_inicializar
extern mmu_unmapear_pagina
extern mmu_inicializar_memoria_perro

extern crearPerro

extern resetear_pic
extern habilitar_pic
extern tss_inicializar
extern tss_inicial_dir
extern tss_idle_dir
extern sched_inicializar
extern game_jugador_inicializar
extern jugadorA
extern jugadorB

;extern screen_actualizar_reloj_global
;; Saltear seccion de dato
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli


    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font


    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0


    ; Habilitar A20
	call habilitar_A20 ;Biennnn	

	
    ; Cargar la GDT
	lgdt [GDT_DESC]

	
    ; Setear el bit PE del registro CR0
	mov eax,cr0
	or eax,1
	mov cr0,eax


    ; Saltar a modo protegido
	jmp 0x40:modoprotegido
	BITS 32
	modoprotegido:


    ; Establecer selectores de segmentos
	mov ax,	0x50
	mov ds,	ax
	mov es,	ax
	mov gs,	ax
	mov ss,	ax
	
	mov ax,	0x60
	mov fs,	ax

       
    ; Establecer la base de la pila
    mov esp, 0x27000
    mov ebp, 0x27000

    
    ; Imprimir mensaje de bienvenida
	imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

		
    ; Inicializar el juego
    push jugadorA
	call game_jugador_inicializar
	add esp, 4
	push jugadorB
	call game_jugador_inicializar
	add esp, 4
	
    ; Inicializar pantalla
	call screen_inicializar


	; Inicializar el manejador de memoria
	call mmu_inicializar	


    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel

    
    ; Cargar directorio de paginas
	;~ xchg bx, bx ;breakpoint
	mov cr3, eax

		
    ; Habilitar paginacion
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	
    ; Inicializar tss


    ; Inicializar tss de la tarea Idle
	call tss_inicializar
	
	
    ; Inicializar el scheduler
	call sched_inicializar

	
    ; Inicializar la IDT
	call idt_inicializar

 
    ; Cargar IDT
    lidt [IDT_DESC]

	
    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic

	
    ; Cargar tarea inicial

	mov ax, 0x70 ; porque es el indice 14 de la GDT, Marce CONFIA en esto, puede que se 0x70 que es 14*8
	ltr ax


    ; Habilitar interrupciones
    sti


    ; Saltar a la primera tarea: Idle
    ;~ call crearPerro
    
    ;~ jmp 0x78:0
    
    jmp 0x68:0 ; porque es 13*8 en hexa porque es el selector
    xchg bx, bx ;breakpoint	
	
	

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;
magicBreakpoint:
	xchg bx, bx
	ret
%include "a20.asm"






;funciones que no se donde meter

darEax:
	
