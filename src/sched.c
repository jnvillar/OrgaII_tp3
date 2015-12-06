/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

int printf(const char *fmt, ...);


sched_t scheduler;

void sched_inicializar()
{
	scheduler.current = 0;
	scheduler.tasks[0].gdt_index = 13;
	scheduler.tasks[0].perro = NULL;
	scheduler.ultimojugador = 0;
	scheduler.ultimoperroA = 0;
	scheduler.ultimoperroB = 0;

	int i = 1;
	while(i<=MAX_CANT_TAREAS_VIVAS){		
		scheduler.tasks[i].gdt_index = 13 + i;
		scheduler.tasks[i].perro = NULL; 
		i++;		
	}
}




int sched_buscar_indice_tarea(uint gdt_index) { 
    int i = 0;
    while(i<=MAX_CANT_TAREAS_VIVAS){
    	if(scheduler.tasks[i].perro != NULL && scheduler.tasks[i].gdt_index == gdt_index){
    		return i;
    	}
    	i++;
    }
    //error();
    return 1000;    
}

uint sched_buscar_gdt_tarea(perro_t *perro) {
    int i = 0;
    while(i<=MAX_CANT_TAREAS_VIVAS){
    	if(scheduler.tasks[i].perro == perro ){
    		return scheduler.tasks[i].gdt_index;
    	}
    	i++;
    }
    return -1;    
}


int sched_buscar_tarea_libre(uint jugador)
{
	int i;
	if(jugador == 0){
		i = 1;
	}else{
		i = 9;
	}
	int j = 0;
	
	while(j<8){
		if(scheduler.tasks[i].perro == NULL){
			return i;			
		}
		i++;
		j++;
	}
	return 0;
}



perro_t* sched_tarea_actual()
{
    return scheduler.tasks[scheduler.current].perro;
}

void sched_agregar_tarea(perro_t *perro)
{
	int i = sched_buscar_tarea_libre(perro->jugador->index);
	//breakpoint();
	if(i != 0){
		scheduler.tasks[i].perro = perro;
	}
}



void sched_remover_tarea(unsigned int gdt_index){	
	//printf("%d\n", sched_buscar_indice_tarea(gdt_index));
	scheduler.tasks[sched_buscar_indice_tarea(gdt_index)].perro->libre = TRUE;	
	screen_pintar_reloj_perro(scheduler.tasks[sched_buscar_indice_tarea(gdt_index)].perro);
	/*
	int i = 1;
	while(i<=MAX_CANT_TAREAS_VIVAS){
		if(scheduler.tasks[i].perro != NULL && scheduler.tasks[i].gdt_index == gdt_index){
			scheduler.tasks[i].perro = NULL;
		}
		i++;
	}
	*/
	scheduler.tasks[sched_buscar_indice_tarea(gdt_index)].perro = NULL;

	if (scheduler.tasks[scheduler.current].gdt_index == gdt_index){
		scheduler.current = 0;
		__asm __volatile(        
        			"int $0x46     \n"       
  		 );
	}
}

void sched_saltar_idle(){
	
	scheduler.current = 0;
}



uint sched_proxima_a_ejecutar()
{

	int j = 0;
	if (scheduler.tasks[scheduler.current].perro != NULL){ // SI NO ESTOY EN LA IDLE
		uint jugador = scheduler.tasks[scheduler.current].perro->jugador->index;
		if (jugador == 0){
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroB+j+1)%8)+9].perro != NULL){
					return ((scheduler.ultimoperroB+j+1)%8)+9;
				}
				j++;
			}

		} else {
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroA+1+j)%8)+1].perro != NULL){
					return ((scheduler.ultimoperroA+1+j)%8)+1;
				}
				j++;
			}
		}		
	} else { 			// SI ESTOY EN LA IDLE
		if (scheduler.ultimojugador == 0){
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroB+j+1)%8)+9].perro != NULL){
					//breakpoint();
					return ((scheduler.ultimoperroB+j+1)%8)+9;
				}
				j++;
			}
		} else {
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroA+1+j)%8)+1].perro != NULL){
					return ((scheduler.ultimoperroA+1+j)%8)+1;
				}
				j++;
			}
		}		 
	}

	j = 0; 		// EN CASO DE QUE EL PROXIMO JUGADOR NO TENGA PERROS VIVOS
	if (scheduler.tasks[scheduler.current].perro != NULL && scheduler.tasks[scheduler.current].perro->jugador->index == 0){ 	
		while (j<=7){
			if (scheduler.tasks[((scheduler.ultimoperroA+1+j)%8)+1].perro != NULL){
				return ((scheduler.ultimoperroA+1+j)%8)+1;
			}
			j++;
		}

	} else if (scheduler.tasks[scheduler.current].perro != NULL && scheduler.tasks[scheduler.current].perro->jugador->index == 1){
		while (j<=7){
			if (scheduler.tasks[((scheduler.ultimoperroB+j+1)%8)+9].perro != NULL){
				return ((scheduler.ultimoperroB+j+1)%8)+9;
			}
			j++;
		}
	} else {			// IDLE
		if (scheduler.ultimojugador == 0){
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroA+1+j)%8)+1].perro != NULL){
					return ((scheduler.ultimoperroA+1+j)%8)+1;
				}
				j++;
				
			}
		} else {
			while (j<=7){
				if (scheduler.tasks[((scheduler.ultimoperroB+j+1)%8)+9].perro != NULL){
					return ((scheduler.ultimoperroB+j+1)%8)+9;
				}
				j++;
			}
		}
	}
	

	return 0;   
	
}

ushort sched_atender_tick()
{
   
    int prox = sched_proxima_a_ejecutar();

    if (scheduler.tasks[prox].perro != NULL){
    	screen_actualizar_reloj_perro(scheduler.tasks[prox].perro);
    }
    if(sched_tarea_actual() == scheduler.tasks[prox].perro){	
       	return scheduler.tasks[scheduler.current].gdt_index;    	
    }else{   

    	if (scheduler.tasks[prox].perro != NULL && scheduler.tasks[prox].perro->jugador->index == 0){
			scheduler.ultimoperroA = prox -1;
			scheduler.ultimojugador = 0;
		} else if (scheduler.tasks[prox].perro != NULL && scheduler.tasks[prox].perro->jugador->index == 1){
			scheduler.ultimoperroB = prox-9;
			scheduler.ultimojugador = 1;
		}
    	scheduler.current = prox;  
    	return scheduler.tasks[scheduler.current].gdt_index;
    }
    
}



