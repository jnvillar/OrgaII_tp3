/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "screen.h"
#include "game.h"
#include "tss.h"


#define POSICION_RELOJES_F   46
#define POSICION_RELOJES_C_A 4
#define POSICION_RELOJES_C_B 60
extern int ultimo_cambio;
int printf(const char *fmt, ...);

int indexPerros;
extern jugador_t jugadorA, jugadorB;
extern int modoDebug;
extern uint pantallaA[50][80];
extern uint pantallaC[50][80];


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4


void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}


void imprim(char letra){
    if (juegoFrenado == 1){
        if (letra == 0x15){
            print("y",0,0,3);
            seguirJuego();
            return;
        } else {
            return;
        }
    }
    if (letra == 0x1E){
        print("a",0,0,3);      
        game_jugador_moverse(&jugadorA,-1,0);
    }
    if (letra == 0x1F){
        print("s",0,0,3);     
        game_jugador_moverse(&jugadorA,0,1);
    }
    if (letra == 0x20){
        print("d",0,0,3);      
        game_jugador_moverse(&jugadorA,1,0);
    }
    if (letra == 0x11){
        print("w",0,0,3);
        game_jugador_moverse(&jugadorA,0,-1);
    }
    if (letra == 0x24){
        print("j",0,0,3);
        game_jugador_moverse(&jugadorB,-1,0);
    }
    if (letra == 0x26){
        print("L",0,0,3);
        game_jugador_moverse(&jugadorB,1,0);
    }
    if (letra == 0x17){
        print("i",0,0,3);
        game_jugador_moverse(&jugadorB,0,-1);
    }
    if (letra == 0x25){
        print("k",0,0,3);
        game_jugador_moverse(&jugadorB,0,1);

    }
    if (letra == 0x10){
        print("q",0,0,3);

        indexPerros = sched_buscar_tarea_libre(0);        
        if (indexPerros != 0){
            game_perro_reciclar_y_lanzar(&jugadorA.perros[indexPerros-1], 0);
        }      



    }
    if (letra == 0x12){
        
        print("e",0,0,3);
        indexPerros = sched_buscar_tarea_libre(0);       
        if (indexPerros != 0){
            game_perro_reciclar_y_lanzar(&jugadorA.perros[indexPerros-1], 1);
        }
        
    }
    if (letra == 0x16){
        print("u",0,0,3);
        indexPerros = sched_buscar_tarea_libre(1);
        if (indexPerros != 0){
            game_perro_reciclar_y_lanzar(&jugadorB.perros[indexPerros-9], 0);
        } 

    }
    if (letra == 0x18){
        print("o",0,0,3);
        indexPerros = sched_buscar_tarea_libre(1);       
        if (indexPerros != 0){
            game_perro_reciclar_y_lanzar(&jugadorB.perros[indexPerros-9], 1);
        }
    }
    if (letra == 0x2c){
        print("z",0,0,3);
        game_jugador_dar_orden(&jugadorA, 1);
    }
    if (letra == 0x2d){
        print("x",0,0,3);
        game_jugador_dar_orden(&jugadorA, 2);
    }
    if (letra == 0x2e){
        print("c",0,0,3);
        game_jugador_dar_orden(&jugadorA, 3);
    }
    if (letra == 0x30){
        print("b",0,0,3);
        game_jugador_dar_orden(&jugadorB, 1);
    }
    if (letra == 0x31){
        print("n",0,0,3);
        game_jugador_dar_orden(&jugadorB, 2);
    }
    if (letra == 0x32){
        print("m",0,0,3);
        game_jugador_dar_orden(&jugadorB, 3);
    }
    if (letra == 0x15){
        print("y",0,0,3);

        if (modoDebug == 0){
            modoDebug = 1;
        }else{                
            modoDebug = 0;         
        }     
        

        
    }
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}


void screen_pintar_rect(uchar letra, uchar color, int fila, int columna, int alto, int ancho)
{
    int f, c;
    for (f = fila; f < fila + alto; f++)
    {
        for (c = columna; c < columna+ancho; c++)
            screen_pintar(letra, color, f, c);
    }
}

void screen_pintar_linea_h(uchar c, uchar color, int fila, int columna, int ancho)
{
    screen_pintar_rect(c, color, fila, columna, 1, ancho);
}

void screen_pintar_linea_v(uchar c, uchar color, int fila, int columna, int alto)
{
    screen_pintar_rect(c, color, fila, columna, alto, 1);
}

void screen_inicializar()
{
    screen_pintar_rect(' ', C_BG_BLACK | C_FG_WHITE, 0, 0, VIDEO_FILS, VIDEO_COLS);
    screen_pintar_rect(' ', C_BG_RED   | C_FG_WHITE, 45, 33, 5, 7);
    screen_pintar_rect(' ', C_BG_BLUE  | C_FG_WHITE, 45, 40, 5, 7);


    screen_pintar_relojes();
    screen_pintar_puntajes();

    int i, j;
    for (i = 0; i < MAPA_ALTO; i++)
    {
        for (j = 0; j < MAPA_ANCHO; j++)
            screen_actualizar_posicion_mapa(j, i);
    }
}

void screen_pintar_puntajes()
{
    print_dec(jugadorA.puntos, 3, 33+2, 45+2, C_BG_RED  | C_FG_WHITE);
    print_dec(jugadorB.puntos, 3, 40+2, 45+2, C_BG_BLUE | C_FG_WHITE);
}

uchar screen_color_jugador(jugador_t *j)
{
    if (j == NULL)
        return C_FG_LIGHT_GREY;

    if (j->index == JUGADOR_A)
        return C_FG_RED;
    else
        return C_FG_BLUE;
}

uchar screen_caracter_perro(uint tipo)
{
    if (tipo == TIPO_1) return '1';
    if (tipo == TIPO_2) return '2';
    while(1){};
    return '?';
}


void screen_pintar_reloj_perro(perro_t *perro)
{
	jugador_t *j = perro->jugador;
    uint columna = j->index == JUGADOR_A ? POSICION_RELOJES_C_A : POSICION_RELOJES_C_B;

    uchar col_libre = C_BG_BLACK | screen_color_jugador(j);

    uchar c, col;
    if (perro->libre == TRUE) {
        c = 'X';
        col = col_libre;
    } else {
        c = reloj[perro->indice_reloj];
        col = C_BW;
    }

    screen_pintar('1' + perro->index, C_BW, POSICION_RELOJES_F    , columna + perro->index * 2);
    screen_pintar(                  c,  col, POSICION_RELOJES_F + 2, columna + perro->index * 2);
}

void screen_pintar_reloj_perros(jugador_t *j)
{
    int i;
    for(i = 0; i < MAX_CANT_PERROS_VIVOS; i++)
        screen_pintar_reloj_perro(&j->perros[i]);
}

void screen_pintar_relojes()
{
    screen_pintar_reloj_perros(&jugadorA);
    screen_pintar_reloj_perros(&jugadorB);
}

void screen_actualizar_reloj_perro (perro_t *perro)
{
    perro->indice_reloj = (perro->indice_reloj + 1) % reloj_size;
    screen_pintar_reloj_perro(perro);
}


void screen_pintar_perro(perro_t *perro)
{
    uchar c     = screen_caracter_perro(perro->tipo);
    uchar color = C_MAKE_BG(screen_color_jugador(perro->jugador)) | C_FG_WHITE;

    screen_pintar(c, color, perro->y+1, perro->x);
}

void screen_borrar_perro(perro_t *perro)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, perro->y+1, perro->x);
    screen_actualizar_posicion_mapa(perro->x, perro->y);
}

void screen_pintar_jugador(jugador_t *j)
{
    uchar c     = 'A' + j->index;
    uchar color = C_MAKE_BG(screen_color_jugador(j)) | C_FG_WHITE;

    screen_pintar(c, color, j->y+1, j->x);
}

void screen_borrar_jugador(jugador_t *j)
{
    screen_pintar('.', C_BG_GREEN | C_FG_BLACK, j->y+1, j->x);
    screen_actualizar_posicion_mapa(j->x, j->y);
}


uchar screen_caracter_tesoro(int valor) {
    if (valor > 100)
        return 'O';
    else
        return 'o';
}

void screen_actualizar_posicion_mapa(uint x, uint y)
{
    uchar bg = C_BG_GREEN;

    uchar letra;
    uint valor = game_huesos_en_posicion(x,y);
    perro_t *perro = game_perro_en_posicion(x, y);
    if (perro != NULL) {
        letra = screen_caracter_perro(perro->tipo);
    } else if (valor > 0) {
        letra = screen_caracter_tesoro(valor);
    } else if ((jugadorA.x_cucha == x && jugadorA.y_cucha == y) || (jugadorB.x_cucha == x && jugadorB.y_cucha == y))
    {
        letra = 'x';
    }
    else
    {
        letra = screen_valor_actual(y+1, x);
    }

    screen_pintar(letra, bg | C_FG_BLACK, y+1, x);

}


void screen_stop_game_show_winner(jugador_t *j) {
    int offy = 14; //(50/2 - 11);
    int offx = 20; //(80/2 - 20);

    int ancho = 40;
    int alto = 17;

    uchar color = screen_color_jugador(j);

    screen_pintar_rect(' ', C_MAKE_BG(color) | C_FG_WHITE, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect('*', C_BW, offy, offx, alto, ancho);

    offy++; offx++; alto -= 2; ancho-=2;

    screen_pintar_rect(' ', C_BG_LIGHT_GREY | C_FG_BLACK, offy, offx, alto, ancho);

    print("EL GANADOR ES EL JUGADOR",   offx+5  , offy+3 , C_BG_LIGHT_GREY | C_FG_BLACK);

    if(j == NULL)      print("EMPATE", offx+14, offy+6, C_MAKE_BG(color) | C_FG_BLACK);
    if(j == &jugadorA) print("<-- A",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    if(j == &jugadorB) print("B -->",  offx+15, offy+6, C_BG_LIGHT_GREY  | color);
    // a partir de aca se termina el unviverso (STOP GAME)
    __asm __volatile( "cli\n" : : : );
    while(1){}
}



void aux_limpiarPantalla(){
    int i = 0;
    while (i<45){
        int j = 0;
        while(j<80){
            p[i][j].c = 219;
            p[i][j].a = 7;
            j++;
        }
        i++;
    }
 
}

void restaurarPantalla(){
    aux_limpiarPantalla();
    
    int i,j;
    for ( i = 0; i < 50; i++){
        for ( j = 0; j < 80; j++){
            p[i][j].a = pantallaA[i][j];
            p[i][j].c = pantallaC[i][j];
        }
    }
}


void imprimirNombre(){
    print("Te voy a dar un byte", 60, 0, 3);
}

void pantallaDebug(char* excepcion){
    print(excepcion, 0, 0, 15);
    int i;
    for (i = 5; i <= 42; i++){
                int j;
                for (j = 24 ; j <= 54; j++){
                    p[i][j].a = 0;
                    p[i][j].c = 219;

                }   
    }
    for (i = 7; i <= 41; i++){
                int j;
                for (j = 25 ; j <= 53; j++){
                    p[i][j].a = 7;
                    p[i][j].c = 219;

                }   
    }
     for (i = 6; i <= 6; i++){
                int j;
                for (j = 25 ; j <= 53; j++){
                    p[i][j].a = 4;
                    p[i][j].c = 219;

                }   
    }

   
    tss tss_perro = darTss(sched_tarea_actual()->jugador->index,sched_tarea_actual()->index);
     
      
    print("eax", 26, 9, 112);
                                                print_hex(tss_perro.eax, 8, 30, 9, 127);
    print("ebx", 26,11, 112);
                                                print_hex(tss_perro.ebx, 8, 30, 11, 127);
    print("ecx", 26, 13, 112);
                                                 print_hex(tss_perro.ecx, 8, 30, 13, 127);
    print("edx", 26, 15, 112);
                                                 print_hex(tss_perro.edx, 8, 30, 15, 127);
    print("esi", 26, 17, 112);
                                                print_hex(tss_perro.esi, 8, 30, 17, 127);
    print("edi", 26, 19, 112);
                                                 print_hex(tss_perro.edi, 8, 30, 19, 127);
     print("ebp", 26, 21, 112);
                                                 print_hex(tss_perro.ebp, 8, 30, 21, 127);
    print("esp", 26, 23, 112);
                                                 print_hex(tss_perro.esp, 8, 30, 23, 127);
    print("eip", 26, 25, 112);
                                                 print_hex(tss_perro.eip, 8, 30, 25, 127);
    print("cs", 27, 27, 112);
                                                 print_hex(tss_perro.cs, 8, 30, 27, 127);
    print("ds", 27, 29, 112);
                                                print_hex(tss_perro.ds, 8, 30, 29, 127);
    print("es", 27, 31, 112);
                                                print_hex(tss_perro.es, 8, 30, 31, 127);
    print("fs", 27, 33, 112);
                                                print_hex(tss_perro.fs, 8, 30, 33, 127);
    print("gs", 27, 35, 112);
                                                print_hex(tss_perro.gs, 8, 30, 35, 127);
    print("ss", 27, 37, 112);
                                                print_hex(tss_perro.ss, 8, 30, 37, 127);
    print("eflags", 27, 39, 112);
                                                print_hex(tss_perro.eflags, 8, 33, 39, 127);
    print("cr0", 40, 9, 112);
                                               print_hex(rcr0(), 8, 44, 9, 127);
    print("cr2", 40, 11, 112);
                                               print_hex(rcr2(), 8, 44, 11, 127);
    print("cr3", 40, 13, 112);
                                                print_hex(tss_perro.cr3, 8, 44, 13, 127);
    print("cr4", 40, 15, 112);    
                                               print_hex(rcr4(), 8,44, 15, 127);

    print("stack", 40, 26, 112);    
                                                   print_hex(*((uint *) tss_perro.esp0), 8, 40, 29, 127);
                                                   print_hex(*((uint *) tss_perro.esp0-8), 8, 40, 30, 127);
                                                   print_hex(*((uint *) tss_perro.esp0-16), 8, 40, 31, 127);
                                                   print_hex(*((uint *) tss_perro.esp0-24), 8, 40, 32, 127);

                                            

    
    
}