
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

//	~~~ completar si es necesario ~~~

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

	

	mmu_inicializar_memoria_perro(perro,perro->jugador->index, tipo);
	sched_agregar_tarea(perro);
	tss_completar(perro->jugador->index, perro->index, perro);

	screen_actualizar_reloj_perro(perro);



}

// el perro descargó sus huesos o realizó una acción no válida y caputó, hay que sacarlo del sistema.
void game_perro_termino(perro_t *perro)
{	
	if (perro == NULL){
		breakpoint();
		return;
	}
	uint aux = sched_buscar_gdt_tarea(perro);
	sched_remover_tarea(aux);
	
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
uint game_perro_mover(perro_t *perro, direccion dir)
{	

	int x, y;
	uint res = game_dir2xy(dir, &x, &y);
	int nuevo_x = perro->x + x;
	int nuevo_y = perro->y + y;
    int viejo_x = perro->x;
    int viejo_y = perro->y;

    // ~~~ completar ~~~

    if (game_perro_en_posicion(nuevo_x,nuevo_y) != NULL){
    	if (game_perro_en_posicion(nuevo_x,nuevo_y)->jugador == perro->jugador){

    	}
    } else{


   		perro->x = nuevo_x;
		perro->y = nuevo_y;		
		if(nuevo_x>79 || nuevo_y>43 || nuevo_x<0 || nuevo_y < 0 ){
			game_perro_termino(perro);
		} else {
			mmu_mover_perro(perro, viejo_x, viejo_y);
			screen_actualizar_posicion_mapa(perro->x, perro->y);
		}
    }
	
   	
    

    return nuevo_x + nuevo_y + viejo_x + viejo_y + res; // uso todas las variables para que no tire warning->error.
}

// recibe un perro, el cual debe cavar en su posición
// *** viene del syscall cavar ***
uint game_perro_cavar(perro_t *perro)
{
	int x = perro->x;
	int y = perro->y;
	int i = 0;
	while (i<ESCONDITES_CANTIDAD){
		if (escondites[i][0] == x && escondites[i][1] == y){
			while (perro->huesos < 10){
				if (escondites[i][2] != 0){
					perro->huesos++;
					escondites[i][2]--;
					ticks_actuales = ticks_maximos;

				} else {
					break;
				}
			}			
		}
		i++;
	}

	return 0;
}

// recibe un perro, devueve la dirección del hueso más cercano
// *** viene del syscall olfatear ***
uint game_perro_olfatear(perro_t *perro)
{
	int x_actual_diff = 1000, y_actual_diff = 1000;


	int i;
	for (i = 0; i < ESCONDITES_CANTIDAD; i++)
	{
		if (escondites[i][2] == 0) continue;

		int diff_x = escondites[i][0] - (int)perro->x;
		int diff_y = escondites[i][1] - (int)perro->y;

		if (x_actual_diff * x_actual_diff + y_actual_diff * y_actual_diff > diff_x * diff_x + diff_y * diff_y)
		{
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

	if (perro->huesos == 0)
		return;

	game_jugador_anotar_punto(perro->jugador);

	perro->huesos--;
	if (perro->huesos == 0){

		game_perro_termino(perro);

	}
}

int game_perro_recibir_orden(perro_t *perro){
	if (perro->jugador->index == 0){
		return ordenJugadorA;
	} else {
		return ordenJugadorB;
	}
}