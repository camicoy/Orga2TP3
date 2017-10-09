/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "i386.h"

#include <stdarg.h>

int printf(const char *fmt, ...);

int escondites[ESCONDITES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, HUESOS)
                                        {76,  25, 50}, {12, 15, 50}, {9, 10, 100}, {47, 21, 100} ,
                                        {34,  11, 50}, {75, 38, 50}, {40, 21, 100}, {72, 17, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;

perro_t *game_perro_actual = NULL;
int ultimo_cambio = MAX_SIN_CAMBIOS;

void ASSERT_OR_ERROR(uint value, char* error_msg)
{
	if (!value) {
		print(error_msg, 5, 5, C_BG_LIGHT_GREY | C_FG_BLACK);
		breakpoint();
	}
}

void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_es_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}


void game_inicializar()
{
	game_jugador_inicializar(&jugadorA);
	game_jugador_inicializar(&jugadorB);

    screen_pintar_puntajes();
}


// devuelve la cantidad de huesos que hay en la posición pasada como parametro
uint game_huesos_en_posicion(uint x, uint y)
{
	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][0] == x && escondites[i][1] == y)
			return escondites[i][2];
	}
	return 0;
}




// devuelve algun perro que esté en la posicion pasada (hay max 2, uno por jugador)
perro_t* game_perro_en_posicion(uint x, uint y)
{
	
	int i;
	for (i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
	{	
		
		if (!jugadorA.perros[i].libre && jugadorA.perros[i].x == x && jugadorA.perros[i].y == y)
			return &jugadorA.perros[i];
		if (!jugadorB.perros[i].libre && jugadorB.perros[i].x == x && jugadorB.perros[i].y == y)
			return &jugadorB.perros[i];
	}
	return NULL;
}


// termina si se agotaron los huesos o si hace tiempo que no hay ningun cambio
void game_terminar_si_es_hora()
{
	int cantHuesos = 0;
	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		cantHuesos = cantHuesos + escondites[i][2];
	}
	if (cantHuesos == 0){
		if( jugadorA.puntos < jugadorB.puntos){
			screen_stop_game_show_winner(&jugadorB);
		}else{
			screen_stop_game_show_winner(&jugadorA);
		}
	}
	
}

void game_debug(uint eflags,uint ss, uint gs,uint fs, uint es, uint ds, uint cs, uint eip, uint esp, uint edi, uint esi, uint ebp, uint ebx, uint edx,uint ecx, uint eax){
	screen_guardar_pantalla_actual();
	screen_pintar_rect(0,0,6,25,35,30);
	screen_pintar_rect(219,7,7,26,33,28);
	screen_pintar_linea_h(219,4,7,26,28);
	print("eax", 27, 9, 15);
	print_hex(eax, 8, 31, 9, 15);
	print("ebx", 27, 11, 15);
	print_hex(ebx, 8, 31, 11, 15);
	print("ecx", 27, 13, 15);
	print_hex(ecx, 8, 31, 13, 15);
	print("edx", 27, 15, 15);
	print_hex(edx, 8, 31, 15, 15);
	print("ebp", 27, 17, 15);
	print_hex(ebp, 8, 31, 17, 15);
	print("esp", 27, 19, 15);
	print_hex(esp, 8, 31, 19, 15);
	print("eip", 27, 21, 15);
	print_hex(eip, 8, 31, 21, 15);
	print("cs", 28, 23, 15);
	cs = cs & 0x0000FFFF;
	print_hex(cs, 4, 31, 23, 15);
	print("ds", 28, 25, 15);
	ds = ds & 0x0000FFFF;
	print_hex(ds, 4, 31, 25, 15);
	print("es", 28, 27, 15);
	cs = es & 0x0000FFFF;
	print_hex(es, 4, 31, 27, 15);
	print("fs", 28, 29, 15);
	cs = fs & 0x0000FFFF;
	print_hex(fs, 4, 31, 29, 15);
	print("gs", 28, 31, 15);
	gs = gs & 0x0000FFFF;
	print_hex(gs, 4, 31, 31, 15);
	print("ss", 28, 33, 15);
	ss = ss & 0x0000FFFF;
	print_hex(ss, 4, 31, 33, 15);
	print("eflags", 28, 35, 15);
	print_hex(eflags, 8, 34, 35, 15);
	
	print("cr0", 41, 9, 15);
	print("cr2", 41, 11, 15);
	print("cr3", 41, 13, 15);
	print("cr4", 41, 15, 15);
	print("stack", 41, 22, 15);
	
	
	unsigned int cr0 = rcr0();
	unsigned int cr2 = rcr2();
	unsigned int cr3 = rcr3();
	unsigned int cr4 = rcr4();
	
	//~ print_hex(eax, 8, 31, 9, 15);
	print_hex(cr0, 8, 45, 9, 15);
	print_hex(cr2, 8, 45, 11, 15);
	print_hex(cr3, 8, 45, 13, 15);
	print_hex(cr4, 8, 45, 15, 15);
	
	uint* pesp = (uint*) esp;
	uint* pebp = (uint*) ebp;
	int i = 0;
	while(pesp < pebp){
		//~ breakpoint();
		print_hex(*pesp, 8, 41, 23+i,15 );
		i++;
		pesp = pesp+4;
	}
	//~ breakpoint();
	//~ unsigned short taskRegister = rtr();
	
}

