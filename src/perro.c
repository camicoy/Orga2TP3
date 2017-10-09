
#include "game.h"
#include "mmu.h"
#include "screen.h"
#include "tss.h"
int printf(const char *fmt, ...);

// realiza inicialización básica de un perro. El perro aun no está vivo ni por lanzarse. Setea jugador, indice, etc
void game_perro_inicializar(perro_t *perro, jugador_t *j, uint index, uint id)
{
	perro->id   = id;
    perro->index = index;
    perro->jugador = j;
	perro->libre = TRUE;
	perro->tiempoEnCucha = 0;

}

// toma un perro ya existente libre y lo recicla seteando x e y a la cucha.
// luego inicializa su mapeo de memoria, tss, lo agrega al sched y pinta la pantalla
void game_perro_reciclar_y_lanzar(perro_t *perro, uint tipo)
{
	
	jugador_t *j = perro->jugador;
	perro->x = j->x_cucha;
	perro->y = j->y_cucha;
	perro->tipo = tipo;
	perro->libre = FALSE;
	perro->tiempoEnCucha = 0;
	
	int base;
	if(j->index == 1){
		completar_tss_de_tarea(&(tss_jugadorB[perro->index]), perro);
		base = (int)&tss_jugadorB[perro->index];
	}else{
		completar_tss_de_tarea(&(tss_jugadorA[perro->index]), perro);
		base = (int)&tss_jugadorA[perro->index];
	}
	completar_base_selector(perro->id, base);
	sched_agregar_tarea(perro);
	screen_pintar_perro(perro);
	screen_pintar_reloj_perro(perro);
	// ahora debo llamar a rutinas que inicialicen un nuevo mapa de
	// memoria para el nuevo perro, que carguen su tss correspondiente,
	// lo scheduleen y finalmente lo pinten en pantalla
}

// el perro descargó sus huesos o realizó una acción no válida y caputó, hay que sacarlo del sistema.
void game_perro_termino(perro_t *perro)
{
	perro -> libre = TRUE;
	sched_remover_tarea(perro -> id);
	screen_borrar_perro(perro);
}

// transforma código de dirección en valores x e y 
uint game_dir2xy(/* in */ direccion dir, /* out */ int *x, /* out */ int *y)
{
	switch (dir)
	{
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

// recibe una direccion y un perro, al cual debe mover en esa dirección
// *** viene del syscall mover ***
uint game_perro_mover(perro_t *perro, direccion dir, uint cr3)
{
	int x, y;
	game_dir2xy(dir, &x, &y);
	int nuevo_x = perro->x + x;
	int nuevo_y = perro->y + y;

	perro_t *pos = game_perro_en_posicion(nuevo_x, nuevo_y);

	if(game_es_posicion_valida(nuevo_x, nuevo_y) &&
	 ( pos == NULL || pos->jugador != perro->jugador)){
		perro -> tiempoEnCucha = 0;
		perro -> x = nuevo_x;
		perro -> y = nuevo_y;
		uint dirFiMapa = mmu_xy2fisica(nuevo_x, nuevo_y);
		uint dirVirMapa = mmu_xy2virtual(nuevo_x, nuevo_y);
		mmu_mapear_pagina(dirVirMapa, cr3, dirFiMapa, 0x00000005);
		uint dirCodigo = 0x10000 + perro->jugador->index *0x2000 + perro->tipo *0x1000;
		mmu_unmapear_pagina(0x401000, cr3);
		mmu_mapear_pagina(0x401000, cr3, dirFiMapa, 0x00000007);
		mmu_copiar_pagina(dirCodigo, 0x401000);
		screen_pintar_perro(perro);
	}
	
    return 0;
}

// recibe un perro, el cual debe cavar en su posición
// *** viene del syscall cavar ***
uint game_perro_cavar(perro_t *perro)
{
	if(perro -> huesos < 10 && game_huesos_en_posicion(perro -> x, perro -> y) > 0){
		int i;
		for (i = 0; i < ESCONDITES_CANTIDAD; i++){
			if (escondites[i][0] == perro -> x && escondites[i][1] == perro -> y){
					escondites[i][2]--;
					perro -> huesos ++;
					return 1; 
			}
		}
	}
	return 0;
}

// recibe un perro, devueve la dirección del hueso más cercano
// *** viene del syscall olfatear ***
uint game_perro_olfatear(perro_t *perro)
{
	//int x_actual, y_actual;
	int x_actual_diff = 1000, y_actual_diff = 1000;


	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][2] == 0) continue;

		int diff_x = escondites[i][0] - (int)perro->x;
		int diff_y = escondites[i][1] - (int)perro->y;

		if (x_actual_diff * x_actual_diff + y_actual_diff * y_actual_diff > diff_x * diff_x + diff_y * diff_y)
		{
			//~ x_actual = escondites[i][0];
			//~ y_actual = escondites[i][1];
			x_actual_diff = diff_x;
			y_actual_diff = diff_y;
		}
   	}

	if (x_actual_diff == 0 && y_actual_diff == 0)
		return AQUI;

	if (x_actual_diff * x_actual_diff > y_actual_diff * y_actual_diff)
	{
		return x_actual_diff > 0 ? DER : IZQ;
	}
	else 
	{
		return y_actual_diff > 0 ? ABA : ARR;
	}

    return 0;
}


// chequea si el perro está en la cucha y suma punto al jugador o lo manda a dormir
void game_perro_ver_si_en_cucha(perro_t *perro)
{
	if (perro->x != perro->jugador->x_cucha || perro->y != perro->jugador->y_cucha)
		return;
	
	if (perro->huesos == 0){
		perro->tiempoEnCucha++;
		if(perro->tiempoEnCucha == 30){
			game_perro_termino(perro);
		}
		return;
	}
	else{
	game_jugador_anotar_punto(perro->jugador);
	perro->huesos--;
	}
	//~ if (perro->huesos == 0)
		//~ 
		//~ game_perro_termino(perro);
}

uint game_perro_orden(perro_t *perro){
	return perro -> jugador -> ultimaOrden;
}
