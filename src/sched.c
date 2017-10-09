/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"


sched_t scheduler;

uint anteriorA;
uint anteriorB;
uint jugadorAnterior; //puede ser 0,1,2. Si es 0 es idle, si es 1 es jugadorA y si es 2 es jugadorB.
int hayTareas = FALSE;

void sched_inicializar()
{
	scheduler.tasks[0].gdt_index = 13;
	scheduler.tasks[0].perro = NULL;
	scheduler.tasks[1].gdt_index = 15;
	scheduler.tasks[1].perro = NULL;
	scheduler.tasks[2].gdt_index = 16;
	scheduler.tasks[2].perro = NULL;
	scheduler.tasks[3].gdt_index = 17;
	scheduler.tasks[3].perro = NULL;
	scheduler.tasks[4].gdt_index = 18;
	scheduler.tasks[4].perro = NULL;
	scheduler.tasks[5].gdt_index = 19;
	scheduler.tasks[5].perro = NULL;
	scheduler.tasks[6].gdt_index = 20;
	scheduler.tasks[6].perro = NULL;
	scheduler.tasks[7].gdt_index = 21;
	scheduler.tasks[7].perro = NULL;
	scheduler.tasks[8].gdt_index = 22;
	scheduler.tasks[8].perro = NULL;
	scheduler.tasks[9].gdt_index = 23;
	scheduler.tasks[9].perro = NULL;
	scheduler.tasks[10].gdt_index = 24;
	scheduler.tasks[10].perro = NULL;
	scheduler.tasks[11].gdt_index = 25;
	scheduler.tasks[11].perro = NULL;
	scheduler.tasks[12].gdt_index = 26;
	scheduler.tasks[12].perro = NULL;
	scheduler.tasks[13].gdt_index = 27;
	scheduler.tasks[13].perro = NULL;
	scheduler.tasks[14].gdt_index = 28;
	scheduler.tasks[14].perro = NULL;
	scheduler.tasks[15].gdt_index = 29;
	scheduler.tasks[15].perro = NULL;
	scheduler.tasks[16].gdt_index = 30;
	scheduler.tasks[16].perro = NULL;
	
	scheduler.current = 0;
	jugadorAnterior = 0;
	anteriorA = 0;
	anteriorB = 8;
}


int sched_buscar_indice_tarea(uint gdt_index) {
    return MAX_CANT_TAREAS_VIVAS;
}


int sched_buscar_tarea_libre()
{
//<<<REMOVE END>>>
    // ee_printf("gdt_i0 %x",scheduler.tasks[0].gdt_index);
    // ee_printf("gdt_i1 %x",scheduler.tasks[1].gdt_index);
    // ee_printf("gdt_i2 %x",scheduler.tasks[2].gdt_index);
	int i = sched_buscar_indice_tarea(GDT_IDX_NULL_DESC);
    
	ASSERT_OR_ERROR(i != MAX_CANT_TAREAS_VIVAS, "no hay tareas disponibles!?");

    return i;
//<<<REMOVE END>>>
}



perro_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro)
{
	hayTareas = TRUE;
	int i = 0;
	while(scheduler.tasks[i].gdt_index != perro->id){
		i++;
	}
	scheduler.tasks[i].perro = perro;
}

void sched_remover_tarea(unsigned int gdt_index)
{
	int i;
	for(i = 0; i < MAX_CANT_TAREAS_VIVAS; i++){
		if(scheduler.tasks[i+1].gdt_index == gdt_index){
			scheduler.tasks[i+1].perro = NULL;
		}
	}
	i = 1;
	while(scheduler.tasks[i].perro == NULL && i < 17){
		i++;
	}
	if(i == 17){
		hayTareas = FALSE;
	}
}


uint sched_proxima_a_ejecutar()
{	
	
	if(hayTareas == FALSE){
		scheduler.current = 0;
		jugadorAnterior = 0;
		return 0;
	}
	uint cantidad = 0;
	uint proxJugadorB;
	uint proxJugadorA;
	if(jugadorAnterior == 1 || jugadorAnterior == 0){
		proxJugadorB = anteriorB + 1;
		cantidad = 0;
		while (scheduler.tasks[proxJugadorB].perro == NULL && cantidad < 8){
			if(proxJugadorB == 16){
				proxJugadorB = 8;
			}
			proxJugadorB++;
			cantidad++;
		}
		if (cantidad !=8)
		{
			anteriorB= proxJugadorB;
			
			jugadorAnterior =2;
			scheduler.current=proxJugadorB;
			return 0;
		}
		else{
			cantidad =0;
			proxJugadorA = anteriorA + 1;
			
			while (scheduler.tasks[proxJugadorA].perro == NULL && cantidad < 8){
				if(proxJugadorA == 8){
					proxJugadorA = 0;
				}
				proxJugadorA++;
				cantidad++;
			}
			if (cantidad !=8)
			{
				anteriorA= proxJugadorA;
				
				jugadorAnterior =1;
				scheduler.current=proxJugadorA;
				return 0;
			}
			else{
				scheduler.current = 0;
				jugadorAnterior = 0;
				return 0;
			}
		
		}
	}
	
	else if(jugadorAnterior ==2){
			cantidad =0;
			proxJugadorA = anteriorA + 1;
		
			while (scheduler.tasks[proxJugadorA].perro == NULL && cantidad < 8){
				if(proxJugadorA == 8){
					proxJugadorA = 0;
				}
				proxJugadorA++;
				cantidad++;
			}
			if (cantidad !=8)
			{
				anteriorA= proxJugadorA;
				
				jugadorAnterior =1;
				scheduler.current=proxJugadorA;
				return 0;
			}
			else{
				cantidad = 0;
				proxJugadorB = anteriorB + 1;
				while (scheduler.tasks[proxJugadorB].perro == NULL && cantidad < 8){
					if(proxJugadorB == 16){
						proxJugadorB = 8;
					}
					proxJugadorB++;
					cantidad++;
				}
				if (cantidad !=8)
				{
					anteriorB= proxJugadorB;
					
					jugadorAnterior =2;
					scheduler.current=proxJugadorB;
					return 0;
				}
				else{
					scheduler.current = 0;
					jugadorAnterior = 0;
					return 0;
				}
			
			}
		}
	return 0;
}


ushort sched_atender_tick()
{
	game_atender_tick(scheduler.tasks[scheduler.current].perro);
	sched_proxima_a_ejecutar();
    return scheduler.tasks[scheduler.current].gdt_index;
}


