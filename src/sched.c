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

void sched_inicializar()
{
	scheduler.current = 0;
	scheduler.tasks[0].gdt_index = 13;
	scheduler.tasks[0].perro = NULL;

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

void sched_remover_tarea(unsigned int gdt_index)
{
	int i = 0;

	while(i<=MAX_CANT_TAREAS_VIVAS){
		if(scheduler.tasks[i].gdt_index == gdt_index){			
			scheduler.tasks[i].perro = NULL;
		}
		i++;
	}
	if (scheduler.tasks[scheduler.current].gdt_index == gdt_index){
		scheduler.current = 0;
		//sched_saltar_idle();
	}
}
/*
sched_saltar_idle(){

}
*/

uint sched_proxima_a_ejecutar()
{
	int i = scheduler.current;
	int j = 0;
	if (scheduler.tasks[scheduler.current].perro != NULL){
		uint jugador = scheduler.tasks[scheduler.current].perro->jugador->index;
		while(j<=MAX_CANT_TAREAS_VIVAS){
			if(scheduler.tasks[i%(MAX_CANT_TAREAS_VIVAS+1)].perro != NULL && scheduler.tasks[i%(MAX_CANT_TAREAS_VIVAS+1)].perro->jugador-> index != jugador ){
				return i%(MAX_CANT_TAREAS_VIVAS+1);
			}
			j++;
			i++;
		}  
		j = 0;
		i = (scheduler.current+1)%(MAX_CANT_TAREAS_VIVAS+1);
		while(j<=MAX_CANT_TAREAS_VIVAS){
			if(scheduler.tasks[i%(MAX_CANT_TAREAS_VIVAS+1)].perro != NULL){
				return i%(MAX_CANT_TAREAS_VIVAS+1);
			}
			j++;
			i++;
		} 
	} else {
		while(j<=MAX_CANT_TAREAS_VIVAS){
			if(scheduler.tasks[i%(MAX_CANT_TAREAS_VIVAS+1)].perro != NULL){
				return i%(MAX_CANT_TAREAS_VIVAS+1);
			}
			j++;
			i++;
		} 
	}
	return 0;   
	
}


ushort sched_atender_tick()
{
   
    int prox = sched_proxima_a_ejecutar();
    if(sched_tarea_actual() == scheduler.tasks[prox].perro){    	
    	
    	return scheduler.tasks[scheduler.current].gdt_index;
    	
    }else{
    	scheduler.current = prox;
    	return scheduler.tasks[scheduler.current].gdt_index;
    }
    
}



