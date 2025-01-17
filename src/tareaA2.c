/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/




#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"
//~ #include "printf.h"
int printf(const char *fmt, ...);

void ir_hacia_desde(char x_dest, char y_dest, char x_actual, char y_actual);
void actualizar(int *x, int *y, int direccion);


void task(int x_origen, int y_origen) {
    /* Tarea */
	

	int x_actual = x_origen;
	int y_actual = y_origen;

	while (TRUE)
	{
		int direccion = syscall_olfatear();
		if (direccion == AQUI)
			break;
		
		actualizar(&x_actual, &y_actual, direccion);
		syscall_moverse(direccion);
	}
	while (syscall_cavar() != 0)
	{}
	int b = 0;
	int a = 5/b;
	a = a+5; //PARA PROBAR EL DEBUG 
	

	ir_hacia_desde(x_origen, y_origen, x_actual, y_actual);

    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}

void actualizar(int *x, int *y, int direccion)
{
	switch (direccion)
	{
		case ARR: (*y)--; break;
		case ABA: (*y)++; break;
		case IZQ: (*x)--; break;
		case DER: (*x)++; break;
	}
}

void ir_hacia_desde(char x_dest, char y_dest, char x_actual, char y_actual)
{
	char dir_h = x_dest > x_actual ? DER : IZQ;
	char dir_v = y_dest > y_actual ? ABA : ARR;

	int dist_x = x_dest > x_actual ? x_dest - x_actual : x_actual - x_dest;	
	int dist_y = y_dest > y_actual ? y_dest - y_actual : y_actual - y_dest;	

	int i = 0;
	for (i = dist_x; i>0; i--)
		syscall_moverse(dir_h);

	for (i = dist_y; i>0; i--)
		syscall_moverse(dir_v);

}
//~ typedef struct extra_t {
	//~ char mapa[80*50];
	//~ int next_id;
	//~ int tesoros[8][2];
	//~ int next_id_minero;
	//~ 
//~ } extra_t;
//~ 
//~ 
//~ void task(int x_target, int y_target) {
    //~ /* Tarea */
//~ 
//~ /*	extra_t *extra = (extra_t*)0x7FF000;
//~ 
	//~ int id = extra->next_id_minero++;
	//~ if (extra->next_id_minero == 8) extra->next_id_minero = 0;
//~ 
	//~ if (x_target == 0)
	//~ {
		//~ int ii = 0;
		//~ for (ii = 0; ii < 8; ii++)
			//~ if (extra->tesoros[7-ii] != 0)
			//~ {
				//~ x_target = extra->tesoros[7-ii];
				//~ y_target = extra->tesoros[7-ii];
				//~ break;
			//~ }
		//~ }
	//~ }*/
//~ 
 //~ /*   //breakpoint();
    //~ int posicion = syscall_posicion(-1);
	//~ int pos_x = posicion & 0xFF;
	//~ int pos_y = (posicion >> 8) & 0xFF;
//~ 
//~ 
	//~ char lado;
	//~ if (pos_x == 1)
		//~ lado = 0;
	//~ else
		//~ lado = 1;
//~ 
//~ 
	//~ char avance = lado == 0? DER : IZQ;
	//~ char retroc = lado == 0? IZQ : DER;
	//~ char izq    = lado == 0? ARR : ABA;
	//~ char der    = lado == 0? ABA : ARR;
//~ 
    //~ //breakpoint();
	//~ if (lado == 0) {
		//~ int i;
		//~ for(i=pos_x; i < x_target; i++)
		    //~ syscall_mover(avance);
//~ 
		//~ for(i=pos_y; i < y_target; i++)
		    //~ syscall_mover(der);
	//~ }
	//~ else {
		//~ int i;
		//~ for(i=pos_x; i > x_target; i--)
		    //~ syscall_mover(avance);
//~ 
		//~ for(i=pos_y; i > y_target; i--)
		    //~ syscall_mover(der);
	//~ }
	//~ 
    //~ while(1) {syscall_cavar();}
//~ */
  //~ while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
//~ }
