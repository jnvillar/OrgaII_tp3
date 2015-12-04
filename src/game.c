/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

#include <stdarg.h>

int ticks_maximos = 5000;
int ticks_actuales = 5000;
int modoDebug = 0;
uint pantallaa[50][80];
uint pantallac[50][80];
static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

int escondites[ESCONDITES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, HUESOS)
                                        {76,  25, 5}, {12, 15, 5}, {9, 10, 5}, {47, 21, 5} ,
                                        {34,  11, 5}, {75, 38, 5}, {40, 21, 5}, {72, 17, 5}
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

void esPerro(){
	
	if(sched_tarea_actual()!=NULL){
		if(modoDebug == 1){
			int i,j;
			for ( i = 0; i < 50; ++i)
			{
				for ( j = 0; j < 80; ++j)
				{
					pantallaa[i][j] = p[i][j].a;
					pantallac[i][j] = p[i][j].c;
				}
			}			
			pantallaDebug();
		}
	}
	
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
	int i = 0;
	int huesos = 0;
	while(i<ESCONDITES_CANTIDAD){	
		huesos = huesos + escondites[i][3];	
		i++;
	}

	ticks_actuales--;
	

	if (huesos == 0 || ticks_actuales == 0){
		i = 0;
		while(TRUE){
			print("Game Over",39,i,3);
			
			if(i == 49){
				i = 0;
			}
			i++;
		}
	}

	

}






