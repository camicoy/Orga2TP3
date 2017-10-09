/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
extern void magicBreakpoint();



/* Definicion de la GDT */
/* -------------------------------------------------------------------------- */

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    //~ Segmento de codigo nivel 0
	[8] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */	//500mb (primera parte)
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */	//code permisivo
        (unsigned char)     0x01,           /* s            */	//sistem: tipo (code)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x01,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo(en 0)
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x01,           /* db           */	//EN UNO (16 bit/32 bit)
        (unsigned char)     0x01,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    
    //~ Segmento codigo nivel 3
	[9] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */	//500mb (primera parte)
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x0A,           /* type         */	//code permisivo
        (unsigned char)     0x01,           /* s            */	//tipo (code)
        (unsigned char)     0x03,           /* dpl          */	//permiso nivel 3
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x01,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x01,           /* db           */	//EN UNO (16 bit/32 bit)
        (unsigned char)     0x01,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    
	    //~ Segmento data nivel 0
	[10] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */	//500mb (primera parte)
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */	//data read/write
        (unsigned char)     0x01,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x01,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x01,           /* db           */	//EN UNO (16 bit/32 bit)
        (unsigned char)     0x01,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    
    //~ Segmento data nivel 3
    [11] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */	//500mb (primera parte)
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */	//data read/write
        (unsigned char)     0x01,           /* s            */	//tipo (data)
        (unsigned char)     0x03,           /* dpl          */	//permiso nivel 3
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x01,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x01,           /* db           */	//EN UNO (16 bit/32 bit)
        (unsigned char)     0x01,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    
    //~ Segmento data nivel 0, pantalla
    [12] = (gdt_entry) {
        (unsigned short)    0xF3FF,         /* limit[0:15]  */	//500mb (primera parte)
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */	//data read/write
        (unsigned char)     0x01,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x01,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x01,           /* db           */	//EN UNO (16 bit/32 bit)
        (unsigned char)     0x01,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    }, 
    
    //~ Segmento TSS IDLE                 --> hay que hacer 18, 8 por cada jugador y un idle y una para la tarea inicial
    [13] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//el tamaño de la TSS es de 104 bytes.    
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Supongo que no se esta ejecutando, asi que le pongo 0. TOtal: 1001 = 9
        (unsigned char)     0x00,           /* s            */	//0 porque lo dice el pdf
        (unsigned char)     0x00,           /* dpl          */	//permiso. que nivel le pongo??
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//ni idea, lo dejo en 0 por ahora
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    }, 
    
    //~ Segmento TSS Tarea Inicial
    [14] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [15] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [16] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [17] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [18] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [19] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [20] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [21] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [22] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [23] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [24] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [25] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [26] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [27] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [28] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [29] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    [30] = (gdt_entry) {
        (unsigned short)    0x0067,         /* limit[0:15]  */	//
        (unsigned short)    0x0000,         /* base[0:15]   */	//empezas en 0
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */	//1 0 B 1 -> B = busy: 1 si esta ejecutandose, 0 sino. Un ayudante nos dijo que tenemos que dejarlo en 0 el bit de busy. hay q ver como hago para empezar a ejecutar la tarea inicial
        (unsigned char)     0x00,           /* s            */	//tipo (data)
        (unsigned char)     0x00,           /* dpl          */	//permiso nivel 0
        (unsigned char)     0x01,           /* p            */	//presente
        (unsigned char)     0x00,           /* limit[16:19] */	//limite (segunda parte)
        (unsigned char)     0x00,           /* avl          */	//algo que hay que dejar en algo
        (unsigned char)     0x00,           /* l            */	//EN CERO CARAJO
        (unsigned char)     0x00,           /* db           */	
        (unsigned char)     0x00,           /* g            */	//granulado (multiplica el limite por 4K)
        (unsigned char)     0x00,           /* base[31:24]  */
    }
};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};

void completar_base_selector(uint indice, int base){
	//~ magicBreakpoint();
	int parte = base;
	parte = parte & 0xFFFF;
	gdt[indice].base_0_15 = parte;
	parte = base;
	parte = parte & 0x00FF0000;
	parte = parte >> 16;
	gdt[indice].base_23_16 = parte;
	base = base & 0xFF000000;
	base = base >> 24;
	gdt[indice].base_31_24 = base;
}
