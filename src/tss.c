/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
int printf(const char *fmt, ...);



tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PERROS_VIVOS];
tss tss_jugadorB[MAX_CANT_PERROS_VIVOS];

void tss_inicializar() {
	
	// modificamos aca la gdt 
	// tarea inicial
	gdt[12].base_0_15 = (uint )&tss_inicial & 0x0000FFFF;
	gdt[12].base_23_16 = ((uint )&tss_inicial & 0x00FF0000) >> 16;
	gdt[12].base_31_24 = ((uint )&tss_inicial & 0xFF000000) >> 24;
	// idle
	gdt[13].base_0_15 = (uint )&tss_idle & 0x0000FFFF;
	gdt[13].base_23_16 = ((uint )&tss_idle & 0x00FF0000) >> 16;
	gdt[13].base_31_24 = ((uint )&tss_idle & 0xFF000000) >> 24;
	
	tss_idle.esp = 0x27000;
	tss_idle.ebp = 0x27000;
	tss_idle.cr3 = 0x27000;
	tss_idle.eip = 0x16000;	
	tss_idle.esp0 = 0x27000;
	tss_idle.ds = 0x48;
	tss_idle.ss0 = 0x48;
	tss_idle.ss = 0x48;
	tss_idle.fs = 0x48;
	tss_idle.gs = 0x48;
	tss_idle.es = 0x48;
	tss_idle.cs = 0x40;
	tss_idle.eflags = 0x202;


	
}

void tss_completar(int jugador, int perro, perro_t *rrope){
	uint espCero = mmu_proxima_pagina_fisica_libre();
	int posicion = perro;
	if (jugador == 0 ){		
	    tss_jugadorA[posicion].cs = 0x5B;
	    tss_jugadorA[posicion].es = 0x53;
	    tss_jugadorA[posicion].gs = 0x53;
	    tss_jugadorA[posicion].ss = 0x53;
	    tss_jugadorA[posicion].ds = 0x53;
	    tss_jugadorA[posicion].fs = 0x53;
	    tss_jugadorA[posicion].eax = 0x0;
	    tss_jugadorA[posicion].ebx = 0x0;
	    tss_jugadorA[posicion].ecx = 0x0;
	    tss_jugadorA[posicion].edx = 0x0;
	    tss_jugadorA[posicion].esi = 0x0;
	    tss_jugadorA[posicion].edi = 0x0;
	    tss_jugadorA[posicion].esp = 0x0402000-0xC;
	    tss_jugadorA[posicion].eip = 0x00401000;
	    tss_jugadorA[posicion].eflags = 0x202;
		tss_jugadorA[posicion].esp0 = (espCero+4096);
		tss_jugadorA[posicion].iomap = 0xFFFF;
		tss_jugadorA[posicion].ldt = 0x00000000;
		tss_jugadorA[posicion].ss0 = 0x48;
		uint nuevoCr3 = mmu_inicializar_memoria_perro(rrope, jugador, perro);
		tss_jugadorA[posicion].cr3 = nuevoCr3;


		gdt[rrope->id].base_0_15 =   (uint )&tss_jugadorA[posicion] & 0x0000FFFF;
		gdt[rrope->id].base_23_16 = ((uint )&tss_jugadorA[posicion] & 0x00FF0000) >> 16;
		gdt[rrope->id].base_31_24 = ((uint )&tss_jugadorA[posicion] & 0xFF000000) >> 24;

	} else {
	    tss_jugadorB[posicion].cs = 0x5B;
	    tss_jugadorB[posicion].es = 0x53;
	    tss_jugadorB[posicion].gs = 0x53;
	    tss_jugadorB[posicion].ss = 0x53;
	    tss_jugadorB[posicion].ds = 0x53;
	    tss_jugadorB[posicion].fs = 0x53;
	    tss_jugadorB[posicion].eax = 0x0;
	    tss_jugadorB[posicion].ebx = 0x0;
	    tss_jugadorB[posicion].ecx = 0x0;
	    tss_jugadorB[posicion].edx = 0x0;
	    tss_jugadorB[posicion].esi = 0x0;
	    tss_jugadorB[posicion].edi = 0x0;
	    tss_jugadorB[posicion].esp = 0x0402000-0xC;
	    tss_jugadorB[posicion].eip = 0x00401000;
	    tss_jugadorB[posicion].eflags = 0x202;
		tss_jugadorB[posicion].esp0 = (espCero+4096);
		tss_jugadorB[posicion].iomap = 0xFFFF;
		tss_jugadorB[posicion].ldt = 0x00000000;
		tss_jugadorB[posicion].ss0 = 0x48;

		uint nuevoCr3 = mmu_inicializar_memoria_perro(rrope, jugador, perro);
		
		tss_jugadorB[posicion].cr3 = nuevoCr3;
		gdt[rrope->id].base_0_15 =   (uint )&tss_jugadorB[posicion] & 0x0000FFFF;
		gdt[rrope->id].base_23_16 = ((uint )&tss_jugadorB[posicion] & 0x00FF0000) >> 16;
		gdt[rrope->id].base_31_24 = ((uint )&tss_jugadorB[posicion] & 0xFF000000) >> 24;
	}
}
