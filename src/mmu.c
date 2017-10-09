/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
//extern void magicBreakpoint();
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */


void mmu_inicializar(){
	cantUsadas = 0;
}

void mmu_copiar_pagina (uint src, uint dst){
	int i = 0;
	char* dest = (char*)dst;
	char* orig = (char*)src;
	while(i< 4096) {
		*(dest + i) = *(orig + i);
		i++;
	}
}

uint mmu_proxima_pagina_fisica_libre(){
	uint direcPagina = cantUsadas*0x1000;//4096
	direcPagina += 0x100000;
	cantUsadas++;
	return direcPagina;
}

void mmu_inicializar_pagina(uint * pagina){
		uint i = 0;
	while(i < 1024){
		uint *segmento = (uint*)(pagina+4*i);
		*segmento = 0x00000000;
		i++;
	}
}

void mmu_mapear_pagina  (uint virtual, uint cr3, uint fisica, uint attrs){
	
	uint pd = cr3 & 0xFFFFF000;
	uint direc = virtual >> 22; //shifteamos 22 bits para quedarnos con los primeros 10
	direc = direc*4;
	direc += pd;
	attrs = attrs & 0x00000FFF;
	uint *pag = (uint*)direc;
	if(*pag % 2 == 0){
		uint nuePagina = mmu_proxima_pagina_fisica_libre();
		mmu_inicializar_pagina((uint*)nuePagina);
		nuePagina = nuePagina | attrs;
		*pag = nuePagina;
	}
	uint pagina = *pag;
	pagina = pagina & 0xFFFFF000;
	uint table = virtual << 10;
	table = table >> 22;
	pagina = pagina + (table*4);
	fisica = fisica & 0xFFFFF000;
	fisica = fisica | attrs;
	uint *dir = (uint*)pagina;
	*dir = fisica;
	tlbflush();
}

uint mmu_inicializar_dir_kernel(){
	uint i =0;
	while( i <= 0x3FFFFF ){
		mmu_mapear_pagina(i,0x00027000,i,0x00000003);
				i+=0x1000;//4096

	}
	//~ cantUsadas = 0X73 ; //La idea es "engañar" la funcion, para que ahora arranque de 100000, asi lo usamos para el directorio
	return 0x00027000;
}

uint mmu_unmapear_pagina(uint virtual, uint cr3){
	uint pd = cr3 & 0xFFFFF000;
	uint direc = virtual >> 22;
	direc = direc*4;
	direc += pd;
	uint *pag = (uint*)direc;
	if(*pag % 2 == 1){	
		uint pagina = *pag;
		pagina = pagina & 0xFFFFF000;
		uint table = virtual << 10;
		table = table >> 22;
		pagina += (table*4);
		uint *dir = (uint*)pagina;
		*dir = 0x00000000;
	}
	tlbflush();
	return 0x0BABAFEA;//POR AHI DEVUELVE ALGO UTIL. Ahora no
}
uint mmu_xy2fisica(uint x, uint y){
	
	return (80*y+x)*0x1000+MAPA_BASE_FISICA;;
}

uint mmu_xy2virtual(uint x, uint y){
	
	return (80*y+x)*0x1000+MAPA_BASE_VIRTUAL;
}


uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo){
	uint i =0;
	uint direcTarea = mmu_proxima_pagina_fisica_libre();
		mmu_inicializar_pagina((uint*)direcTarea);
	while( i <= 0x3FFFFF ){
		mmu_mapear_pagina(i,direcTarea,i,0x00000007);
		i+=0x1000;//4096
	}//TEORICAMENTE mapeamos la parte del kernel TEORICAMENTE 
	
	uint parteComun= 0x3FFFFF - index_jugador*0x1000; //DUDO :(
	mmu_mapear_pagina(0x400000,direcTarea,parteComun,0x00000007);
	
	jugador_t *jug = perro->jugador;
	//magicBreakpoint();
	uint dirFisica = mmu_xy2fisica(jug->x_cucha, jug->y_cucha);
	uint dirVirtual = mmu_xy2virtual(jug->x_cucha, jug->y_cucha);
	uint base = 0x10000;
	uint dirEfectiva = base + index_jugador*0x2000 + index_tipo*0x1000; //obtenemos la direccion del perro en base a què jugador es y que raza es.
	
	mmu_mapear_pagina(0x401000,direcTarea,dirFisica,0x00000007);//mapeo la direccion actual (la cucha)
	
	mmu_mapear_pagina(dirVirtual,direcTarea,dirFisica,0x00000005);//mapeo las direcciones que conozco (la cucha)
	
	mmu_mapear_pagina(0x500000,rcr3(),dirFisica,0x00000007);//tiene permisos de usuario
	mmu_copiar_pagina(dirEfectiva, 0x500000); //copiamos el codigo del perro en la cucha.
	mmu_unmapear_pagina(0x500000,rcr3());
	return direcTarea;
}

