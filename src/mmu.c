/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

/* Atributos paginas */
/* -------------------------------------------------------------------------- */
int printf(const char *fmt, ...);

extern void aux_limpiarPantalla();
extern void print(const char * text, unsigned int x, unsigned int y, unsigned short attr);
uint pagLibre = 0x100000;
uint cantPagLibre = 768;
uint paginaJugadorA;
uint paginaJugadorB;

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */


void mmu_mapear_pagina(uint virtual, uint cr3, uint fisica, uint attrs){
	uint *pagDir = (uint *) ((cr3 & 0xFFFFF000) + ((virtual >> 22 )*4));

	uint *pageTable;
	uint *pageTableEntry;
	if ( *pagDir % 2 == 1){ 				// ESTA PRESENTE?
		
		
		pageTableEntry = (uint *) ((*pagDir & 0xFFFFF000) + ((virtual >> 12) & 0x000003FF)*4);
		uint pageDirAux = *pagDir;
		pageDirAux = pageDirAux & 0xFFFFFFFE;
		uint atr_aux = attrs;
		atr_aux = atr_aux >> 1;
		if (pageDirAux % 4 == 0 && atr_aux % 2 == 1){ 			// si r/w == 0 y attrs es de r/w
			*pagDir = *pagDir | 2;

		}
		atr_aux = atr_aux >> 1;		
		pageDirAux = pageDirAux & 0xFFFFFFFC;
		if (pageDirAux % 8 != 0 && atr_aux % 2 == 0){ 			// si u/s == 1 y attrs es de supervisor
			*pagDir = *pagDir & 0xFFFFFFFB;
		}
	} else {
		pageTable = (uint *) mmu_proxima_pagina_fisica_libre();
		*pagDir = ((uint )pageTable & 0xFFFFF000) | 0x00000007;			// LE PASAMOS LA DIRECCION DE LA TABLA Y EL PRESENTE
		
		pageTableEntry = pageTable + ((virtual >> 12) & 0x000003FF);
		mmu_inicializar_pagina(pageTable);
	}
	


	*pageTableEntry = 0x00000000 | fisica;
	*pageTableEntry = (*pageTableEntry & 0xFFFFF000) | attrs;

}



uint mmu_inicializar_dir_kernel(){
	
	mmu_inicializar_pagina((uint *)0x00027000);
	uint cr3 = 0x00027000;
	uint attrs = 0x007;
	int i = 0x00000000;
	while (i<1024){
		mmu_mapear_pagina(i*4096,cr3,i*4096,attrs);
		i++;
	}

	paginaJugadorA = mmu_proxima_pagina_fisica_libre();
	paginaJugadorB = mmu_proxima_pagina_fisica_libre();
	return cr3; 

}


uint mmu_proxima_pagina_fisica_libre(){
	pagLibre += 4096;
	cantPagLibre--;
	return pagLibre-4096;
}


void mmu_inicializar_pagina(uint * pagina){
	int i = 0;
	while(i<4096){
		*pagina = 0x00000000;
		pagina += 32;
		i++;
	}
}

uint mmu_unmapear_pagina(uint virtual, uint cr3){
	uint *pagDir = (uint *) ((cr3 & 0xFFFFF000) + ((virtual >> 22 )*4));

	uint *pageTableEntry;
	if ( *pagDir % 2 == 1){ 				// ESTA PRESENTE?
		pageTableEntry = (uint *) ((*pagDir & 0xFFFFF000) + ((virtual >> 12) & 0x000003FF)*4);
	} else {
		return 0;
	}
	*pageTableEntry = 0x00000000;
	return 0;
}



void mmu_inicializar(){

}

/*
uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo){
	
	uint *pagDir = (uint *) mmu_proxima_pagina_fisica_libre(); 	// PIDO UNA PAGINA LIBRE
	mmu_inicializar_pagina(pagDir); 	// LIMPIO PAGINA
	int i = 0x00000000;
	while (i<1024){ 				// HAGO IDENTITY MAPPING
		mmu_mapear_pagina(i*4096,(uint )pagDir,i*4096,0x007);
		i++;
	}
	
	uint aCopiar;
	if (index_jugador == 0){
		if (perro->tipo == 0){ 			// TAREA A1
			aCopiar = 0x10000;
		} else {		 				// TAREA A2
			aCopiar = 0x11000;
		}
		mmu_mapear_pagina(0x400000, (uint) pagDir, paginaJugadorA, 0x007);

	} else{
		if (perro->tipo == 0){ 			// TAREA B1
			aCopiar = 0x12000;
		} else {				 		// TAREA B2
			aCopiar = 0x13000;
		}
		mmu_mapear_pagina(0x400000, (uint) pagDir, paginaJugadorB, 0x007);

	}
	uint dondeCopiar = 0x500000 + (perro->jugador->x_cucha + perro->jugador->y_cucha*80)*4;
	
	int j = 0;
	while (j<3520){ 				// MAPEO CON EL MAPA
		mmu_mapear_pagina(0x800000+j*4096,(uint )pagDir,j*4096+0x500000,0x007);
		j++;
	}
    //breakpoint();
	mmu_mapear_pagina(0x401000, (uint) pagDir, dondeCopiar, 0x007);
    uint viejo_cr3 = rcr3();
	mmu_mapear_pagina(0x401000, (uint) 0x27000, dondeCopiar, 0x007);
    lcr3(0x27000);
	//breakpoint();
	mmu_copiar_pagina(aCopiar,0x401000);
    lcr3(viejo_cr3);

	//breakpoint();
   


	tlbflush();
	return (uint ) pagDir;
}
*/

void mmu_copiar_pagina(uint src, uint dst){
	uint *sr = (uint *) src;
	uint *ds = (uint *) dst;
	int i = 0;
	while(i<1024){
		ds[i] = sr[i];
		i++;
	}
}

void mmu_mover_perro(perro_t *perro, uint viejo_x, uint viejo_y){
	
	//mmu_mapear_pagina(mmu_xy2fisica(perro->x,perro->y), rcr3(), mmu_xy2fisica(perro->x,perro->y),0x007);
	mmu_mapear_pagina(0x401000, rcr3(), mmu_xy2fisica(perro->x,perro->y),0x007);
	mmu_copiar_pagina(mmu_xy2virtual(viejo_x,viejo_y),0x401000); 

	mmu_mapear_pagina(mmu_xy2virtual(perro->x,perro->y), rcr3(), mmu_xy2fisica(perro->x,perro->y),0x007);

	//breakpoint();

}


uint mmu_xy2fisica(uint x, uint y){
	return 0x500000+x*4096+y*4096*80;
}

uint mmu_xy2virtual(uint x, uint y){
	return 0x800000+x*4096+y*4096*80;
}



uint mmu_inicializar_memoria_perro(perro_t *perro, int index_jugador, int index_tipo){
	
	uint *pagDir = (uint *) mmu_proxima_pagina_fisica_libre(); 	// PIDO UNA PAGINA LIBRE
	mmu_inicializar_pagina(pagDir); 	// LIMPIO PAGINA
	
	int i = 0x00000000;
	while (i<1024){ 				
		mmu_mapear_pagina(i*4096,(uint )pagDir,i*4096,0x007);
		i++;
	}
	
	uint aCopiar;
	if (index_jugador == 0){
		if (perro->tipo == 0){ 			// TAREA A1
			aCopiar = 0x10000;
		} else {		 				// TAREA A2
			aCopiar = 0x11000;
		}
		mmu_mapear_pagina(0x400000, (uint) pagDir, paginaJugadorA, 0x007);

	} else{
		if (perro->tipo == 0){ 			// TAREA B1
			aCopiar = 0x12000;
		} else {				 		// TAREA B2
			aCopiar = 0x13000;
		}
		mmu_mapear_pagina(0x400000, (uint) pagDir, paginaJugadorB, 0x007);

	}
	
	
	

	mmu_mapear_pagina(mmu_xy2virtual(perro->jugador->x_cucha,perro->jugador->y_cucha),(uint )pagDir, mmu_xy2fisica(perro->jugador->x_cucha,perro->jugador->y_cucha),0x007); 	

	mmu_mapear_pagina(0x401000, (uint) pagDir, mmu_xy2fisica(perro->jugador->x_cucha,perro->jugador->y_cucha), 0x007);
    uint viejo_cr3 = rcr3();
	mmu_mapear_pagina(0x401000, (uint) 0x27000, mmu_xy2fisica(perro->jugador->x_cucha,perro->jugador->y_cucha), 0x007);
    lcr3(0x27000);
	mmu_copiar_pagina(aCopiar,0x401000);
    lcr3(viejo_cr3);

   


	tlbflush();
	return (uint ) pagDir;
}
