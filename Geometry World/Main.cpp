//####################                 GEOMETRY WORLD                  ####################
//####################                 Desarrollador:                  ####################
//####################                 Dylan Rodr�guez Barboza         ####################

#include <stdio.h>
#include <iostream>

#include "PersonajePrincipal.h"
#include "EnemigoPentagono.h"
#include "EnemigoTriangulo.h"
#include "BalaPersonaje.h"
#include "BalaEnemigo.h"

#include <allegro5/allegro.h>

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include <allegro5/allegro_image.h>

using namespace std;

#define anchoPantalla 630
#define largoPantalla 480
#define FPS 30.0
#define FPS1 10.0
#define FPS2 15.0
#define dannoBalaEnemigo 3.5
#define dannoEnemigoPentagono 6.5
#define dannoEnemigoTriangulo 8.5

enum Direccion {UP, DOWN, RIGHT, LEFT, SPACE};
enum Teclas {W, E, R, S, D, F};

bool teclas[4] = {false, false, false, false};

EnemigoPentagono *enemigosPentagono[2];
EnemigoTriangulo *enemigosTriangulo[2];

PersonajePrincipal *personaje;

ALLEGRO_DISPLAY *pantalla = NULL;

ALLEGRO_COLOR transparente = al_map_rgb(0, 0, 0);

ALLEGRO_BITMAP *principalBuffer;
ALLEGRO_BITMAP *enemigoPentagonoBuffer;

ALLEGRO_BITMAP *principalIzquierda;
ALLEGRO_BITMAP *principalDerecha;
ALLEGRO_BITMAP *enemigoPentagono;
ALLEGRO_BITMAP *enemigoTriangulo;
ALLEGRO_BITMAP *ultimo;

//iniciarElementosJuego: funci�n que se encarga de inicializar todos los elementos pertenecientes al videojuego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarElementosJuego(){
	personaje = new PersonajePrincipal(10, 10, 3, 100.0);
	enemigosPentagono[0] = new EnemigoPentagono(400, 400, 1, 0, dannoEnemigoPentagono);
	enemigosPentagono[1] = new EnemigoPentagono(500, 500, 2, 0, dannoEnemigoPentagono);
	enemigosTriangulo[0] = new EnemigoTriangulo(100, 60, RIGHT, 1, dannoEnemigoTriangulo);
	enemigosTriangulo[1] = new EnemigoTriangulo(400, 60, LEFT, 2, dannoEnemigoTriangulo);
}


//limpiarPantalla: funci�n que limpia el elemento display creado al inicio del programa
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPantalla(){
	al_clear_to_color(transparente);
}

//dibujarPrincipal: funci�n encargada de dibujar en pantalla al personaje principal
//Entradas: eje x, eje y, direcci�n del personaje
//Salidas: ninguna
//Restricciones: se toma en cuenta si es direcci�n izquierda o derecha, por lo que se tomar� la imagen correspondiente
void dibujarPrincipal(int x, int y, int direccion){
	
		switch (direccion){
		case LEFT:
			al_draw_bitmap(principalIzquierda, x, y, NULL);
			ultimo = principalIzquierda;
			break;

		case RIGHT:
			al_draw_bitmap(principalDerecha, x, y, NULL);
			ultimo = principalDerecha;
			break;

		default:
			al_draw_bitmap(ultimo, x, y, NULL);
			break;
		}

		al_flip_display();
		
	
}

//dibujarEnemigoPentagono: funci�n encargada de dibujar en pantalla al enemigo pent�gono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarEnemigoPentagono(){
	for (int i = 0; i < 2; i++){
		al_set_target_bitmap(enemigoPentagonoBuffer);
		al_draw_bitmap_region(enemigoPentagono, enemigosPentagono[i]->sprite * 30, 0, 30, 30, 0, 0, NULL);
		al_set_target_bitmap(al_get_backbuffer(pantalla));
		al_draw_bitmap(enemigoPentagonoBuffer, enemigosPentagono[i]->x, enemigosPentagono[i]->y, NULL);
		al_flip_display();
	}
}

//dibujarEnemigoTriangulo: funci�n encargada de dibujar en pantalla al enemigo tri�ngulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarEnemigoTriangulo(){
	for (int i = 0; i < 2; i++){
		al_set_target_bitmap(al_get_backbuffer(pantalla));
		al_draw_bitmap(enemigoTriangulo, enemigosTriangulo[i]->x, enemigosTriangulo[i]->y, NULL);
		al_flip_display();
	}
}


//cambiarSpriteEnemigoPentagono: funci�n encargada de sumar en uno la variable split del enemigo pent�gono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo pent�gono
void cambiarSpriteEnemigoPentagono(){
	for (int i = 0; i < 2; i++){
		if (enemigosPentagono[i]->sprite == 4) enemigosPentagono[i]->sprite = 0;
		else{
			if (enemigosPentagono[i]->sumarSprite == 0){
				enemigosPentagono[i]->sprite += 1;
				enemigosPentagono[i]->sumarSprite = 2;
			}
			else enemigosPentagono[i]->sumarSprite -= 1;
		}
	}
}


//moverEnemigoPentagono: funci�n encargada de sumar los ejer x,y del enemigo pent�gono
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se eval�an las posiciones el personaje principal para que el enemigo lo siga
void moverEnemigoPentagono(int movimiento, int tiempo){
	for (int i = 0; i < 2; i++){
		cout << enemigosPentagono[i]->tiempo << tiempo << endl;
		if (enemigosPentagono[i]->tiempo == tiempo){
			
			if (personaje->y > enemigosPentagono[i]->y){
				if (enemigosPentagono[i]->y < 450)
					enemigosPentagono[i]->y += movimiento;
			}

			if (personaje->y < enemigosPentagono[i]->y){
				if (enemigosPentagono[i]->y > 0)
					enemigosPentagono[i]->y -= movimiento;
			}

			if (personaje->x > enemigosPentagono[i]->x){
				if (enemigosPentagono[i]->x < 600)
					enemigosPentagono[i]->x += movimiento;
			}

			if (personaje->x < enemigosPentagono[i]->x){
				if (enemigosPentagono[i]->x > 0)
					enemigosPentagono[i]->x -= movimiento;
			}

		}
	}
}

//moverEnemigoTriangulo: funci�n encargada de sumar los ejer x,y del enemigo tri�ngulo
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se eval�a si el enemigo est� al punto m�nimo o m�ximo del eje x, para as� cambiar el rumbo
void moverEnemigoTriangulo(int movimiento, int tiempo){
	for (int i = 0; i < 2; i++){
		
		if (enemigosTriangulo[i]->tiempo == tiempo){
			
			if (enemigosTriangulo[i]->direccion == LEFT){
				if (enemigosTriangulo[i]->x == 0) enemigosTriangulo[i]->direccion = RIGHT;
				else enemigosTriangulo[i]->x -= movimiento;
			}

			if (enemigosTriangulo[i]->direccion == RIGHT){
				if (enemigosTriangulo[i]->x == 450) enemigosTriangulo[i]->direccion = LEFT;
				else enemigosTriangulo[i]->x += movimiento;
			}



		}
	}
}

//moverPersonaje: funci�n encargada de sumar los ejer x,y del personaje principal
//Entradas: movimiento (lo que se le suma a los ejes)
//Salidas: ninguna
//Restricciones: ninguna
void moverPersonaje(int movimiento){
	if (teclas[DOWN]){
		if (personaje->y < 450) personaje->y += movimiento;
	}
	if (teclas[UP]){
		if (personaje->y > 0) personaje->y -= movimiento;
	}
	if (teclas[RIGHT]){
		if (personaje->x < 600) personaje->x += movimiento;
	}
	if (teclas[LEFT]){
		if (personaje->x > 0) personaje->x -= movimiento;
	}
}

//colisionPentagono: funci�n encargada de evaluar si existe alguna colisi�n entre alg�n enemigo pent�gono y el personaje principal
//Entradas: ninguna
//Salidas: true: si existe una colisi�n, false: si no existe colisi�n
//Restricciones: ninguna
bool colisionPentagono(){
	for (int i = 0; i < 2; i++){
		if (personaje->x == enemigosPentagono[i]->x && personaje->y == enemigosPentagono[i]->y) return true;
	}
	return false;
}
int main(int argc, char **argv){

	

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//Esta l�nea de c�digo permite que la ventana tenga la capacidad de cambiar de tama�o
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);


	pantalla = al_create_display(anchoPantalla, largoPantalla);
	al_set_window_position(pantalla, 200, 200);
	al_set_window_title(pantalla, "Geometry World");
	if (!pantalla) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	//L�neas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();
	al_init_ttf_addon();
	//*******************

	//ALLEGRO_FONT *fuente = al_load_font("Archristy.ttf", 12, NULL);
	//al_draw_text(fuente, al_map_rgb(44, 117, 225), anchoPantalla/2, largoPantalla/2, ALLEGRO_ALIGN_CENTRE, "Hola");

	
	//L�nea para obtener las funcionalidades de las im�genes
	//*******************
	al_init_image_addon();
	//*******************

	//Se crean los contenedores de las im�genes que se utilizar�n en el juego
	//*******************
	principalBuffer = al_create_bitmap(30, 30);
	enemigoPentagonoBuffer = al_create_bitmap(30, 30);
	//*******************

	//Se cargan las im�genes que se van a utilizar en el juego
	//*******************
	principalIzquierda = al_load_bitmap("Imagenes/PrincipalIzquierda.png");
	principalDerecha = al_load_bitmap("Imagenes/PrincipalDerecha.png");
	enemigoPentagono = al_load_bitmap("Imagenes/EnemigoPentagono.png");
	enemigoTriangulo = al_load_bitmap("Imagenes/EnemigoTriangulo.png");
	//*******************

	//L�neas para obtener las funcionalidades del teclado
	//*******************
	al_init_primitives_addon();
	al_install_keyboard();
	//*******************


	
	bool hecho = false;
	int movimiento = 10;
	int direccion = RIGHT;


	

	ALLEGRO_KEYBOARD_STATE estadoTeclado;

	ALLEGRO_TIMER *primerTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *segundoTimer = al_create_timer(1.0 / FPS1);
	ALLEGRO_TIMER *tercerTimer = al_create_timer(1.0 / FPS2);

	ALLEGRO_EVENT_QUEUE *colaEventos = al_create_event_queue();

	//Registramos los eventos
	//*******************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(tercerTimer));
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	//*******************

	al_start_timer(primerTimer);
	al_start_timer(segundoTimer);
	al_start_timer(tercerTimer);

	iniciarElementosJuego();
	dibujarPrincipal(personaje->x, personaje->y, direccion);


	while (!hecho){

		ALLEGRO_EVENT eventos;
		al_wait_for_event(colaEventos, &eventos);

		al_get_keyboard_state(&estadoTeclado);

		/*Evento que toma en cuenta la tecla m�s actual presionada, se van activando en un array que representa cada movimiento
		Observaci�n: Si se presiona una segunda tecla, al mismo tiempo que se est� presionando la primera, este evento ser� capaz de detectar las dos teclas
		y representarlas en el array de activaci�n (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (eventos.keyboard.keycode){

			case ALLEGRO_KEY_DOWN:
				teclas[DOWN] = true;
				direccion = DOWN;
				break;

			case ALLEGRO_KEY_UP:
				teclas[UP] = true;
				direccion = UP;
				break;

			case ALLEGRO_KEY_RIGHT:
				teclas[RIGHT] = true;
				direccion = RIGHT;
				break;

			case ALLEGRO_KEY_LEFT:
				teclas[LEFT] = true;
				direccion = LEFT;
				break;

			case ALLEGRO_KEY_ESCAPE:
				hecho = true;
				break;
			}

		}

		/*Evento que toma en cuenta cuando se deja de presionar alguna tecla, funciona de la misma forma que el evento anterior, cuando se deja de presionar una
		tecla, se desactiva del array de activaci�n (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_UP){
			switch (eventos.keyboard.keycode){

			case ALLEGRO_KEY_DOWN:
				teclas[DOWN] = false;
				break;

			case ALLEGRO_KEY_UP:
				teclas[UP] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				teclas[RIGHT] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				teclas[LEFT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				hecho = true;
				break;
			}

		}
		
		

		if (eventos.type == ALLEGRO_EVENT_TIMER){
			if (eventos.timer.source == primerTimer){
				moverPersonaje(movimiento);
			}

			if (eventos.timer.source == segundoTimer){
				moverEnemigoPentagono(movimiento, 1);
				moverEnemigoTriangulo(movimiento, 1);
			}

			if (eventos.timer.source == tercerTimer){
				moverEnemigoPentagono(movimiento, 2);
				moverEnemigoTriangulo(movimiento, 2);
			}
			
		}
			
		dibujarPrincipal(personaje->x, personaje->y, direccion);
		dibujarEnemigoPentagono();
		dibujarEnemigoTriangulo();
		cambiarSpriteEnemigoPentagono();
		limpiarPantalla();
		if (colisionPentagono()) hecho = true;


		

	}


	//Se liberan los elementos de allegro que se utilizaron
	
	//al_destroy_font(fuente);
	
	al_destroy_display(pantalla);

	al_destroy_event_queue(colaEventos);
    
	al_destroy_bitmap(principalIzquierda);
	al_destroy_bitmap(principalDerecha);
	al_destroy_bitmap(enemigoPentagono);
	al_destroy_bitmap(enemigoTriangulo);
	al_destroy_bitmap(enemigoPentagonoBuffer);
	
	return 0;
}