/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"


tss tss_inicial;
tss tss_idle;

void tss_inicializar() {
	tss_idle.cr3 = 0x27000; //PONER EL MISMO CR3 QUE EL KERNEL
	tss_idle.cs=0x40;
	tss_idle.ds=0x50;
	tss_idle.dtrap=0;
	tss_idle.eax=0;
	tss_idle.ebp=0x27000;
	tss_idle.ebx=0;
	tss_idle.ecx=0;
	tss_idle.edi=0;
	tss_idle.edx=0;
	tss_idle.eflags=0x00000202;
	tss_idle.eip=0x16000;
	tss_idle.es=0x50;
	tss_idle.esi=0;
	tss_idle.esp=0x27000;
	tss_idle.esp0=0;
	tss_idle.esp1=0;
	tss_idle.esp2=0;
	tss_idle.fs=0x60;
	tss_idle.gs=0x50;
	tss_idle.iomap=0xFFFF;
	tss_idle.ldt=0;
	tss_idle.ptl=0;
	tss_idle.ss=0x50;
	tss_idle.ss0=0;
	tss_idle.ss1=0;
	tss_idle.ss2=0;
	
	tss_idle.unused0=0;
	tss_idle.unused1=0;
	tss_idle.unused2=0;
	tss_idle.unused3=0;
	tss_idle.unused4=0;
	tss_idle.unused5=0;
	tss_idle.unused6=0;
	tss_idle.unused7=0;
	tss_idle.unused8=0;
	tss_idle.unused9=0;
	tss_idle.unused10=0;
	
	int dir_idle = (int) &tss_idle;
	int dir_inicial = (int) &tss_inicial;
	
	completar_base_selector(13, dir_idle);
	completar_base_selector(14, dir_inicial);
}

void completar_tss_de_tarea (tss* _tss, perro_t* _perro) {
	uint jugador = _perro -> jugador -> index;
	uint tipo = _perro -> tipo;
	uint pd = mmu_inicializar_memoria_perro(_perro, jugador, tipo);
	_tss->cr3 = pd;
	_tss->cs=0x4B;
	_tss->ds=0x5B;
	_tss->dtrap=0;
	_tss->eax=0;
	_tss->ebp=0x401000-12;
	_tss->ebx=0;
	_tss->ecx=0;
	_tss->edi=0;
	_tss->edx=0;
	_tss->eflags=0x00000202;
	_tss->eip=0x401000;
	_tss->es=0x5B;
	_tss->esi=0;
	_tss->esp=0x401000-12;
	uint stack0 = mmu_proxima_pagina_fisica_libre();
	_tss->esp0=stack0+0x1000;
	_tss->esp1=0;
	_tss->esp2=0;
	_tss->fs=0x5B;
	_tss->gs=0x5B;
	_tss->iomap=0xFFFF;
	_tss->ldt=0;
	_tss->ptl=0;
	_tss->ss=0x5B;
	_tss->ss0=0x50;
	_tss->ss1=0;
	_tss->ss2=0;
	
	_tss->unused0=0;
	_tss->unused1=0;
	_tss->unused2=0;
	_tss->unused3=0;
	_tss->unused4=0;
	_tss->unused5=0;
	_tss->unused6=0;
	_tss->unused7=0;
	_tss->unused8=0;
	_tss->unused9=0;
	_tss->unused10=0;
}


perro_t * crearPerro(){
	
	
	game_perro_actual->jugador=&jugadorA;
	game_perro_actual->index=0;
	game_perro_actual->id =0x78;
	game_perro_actual->tipo =0;
	game_perro_actual->libre=1;
	completar_tss_de_tarea(&tss_jugadorA[0], game_perro_actual);
	int base = (int)&tss_jugadorA[0];
	completar_base_selector(0xf, base);
	
	
	return game_perro_actual;
}
