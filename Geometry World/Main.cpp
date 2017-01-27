//####################                 GEOMETRY WORLD                  ####################
//####################                 Desarrollador:                  ####################
//####################                 Dylan Rodríguez Barboza         ####################

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
#define numeroEnemigosPentagono 2
#define numeroEnemigosTriangulo 2
#define numeroBalasPersonaje 10
#define numeroBalasEnemigo 10

enum Direccion {UP, DOWN, RIGHT, LEFT};
enum Teclas {S, E, D, F};

bool teclasDireccion[4] = {false, false, false, false};
bool teclasDisparo[4] = { false, false, false, false };

EnemigoPentagono *enemigosPentagono[numeroEnemigosPentagono];
EnemigoTriangulo *enemigosTriangulo[numeroEnemigosTriangulo];
BalaPersonaje *balasPersonaje[numeroBalasPersonaje];
BalaEnemigo *balasEnemigo[numeroBalasEnemigo];

PersonajePrincipal *personaje;

ALLEGRO_DISPLAY *pantalla = NULL;

ALLEGRO_COLOR transparente = al_map_rgb(0, 0, 0);

ALLEGRO_BITMAP *principalBuffer;
ALLEGRO_BITMAP *enemigoPentagonoBuffer;

ALLEGRO_BITMAP *principalIzquierda;
ALLEGRO_BITMAP *principalDerecha;
ALLEGRO_BITMAP *enemigoPentagono;
ALLEGRO_BITMAP *enemigoTriangulo;
ALLEGRO_BITMAP *balaPersonajeH;
ALLEGRO_BITMAP *balaPersonajeV;
ALLEGRO_BITMAP *balaEnemigo;
ALLEGRO_BITMAP *ultimo;

//iniciarElementosJuego: función que se encarga de inicializar todos los elementos pertenecientes al videojuego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarElementosJuego(){
	personaje = new PersonajePrincipal(10, 10, 3, 100.0);
	enemigosPentagono[0] = new EnemigoPentagono(400, 400, 1, 0, dannoEnemigoPentagono);
	enemigosPentagono[1] = new EnemigoPentagono(500, 500, 2, 0, dannoEnemigoPentagono);
	enemigosTriangulo[0] = new EnemigoTriangulo(100, 60, RIGHT, 1, dannoEnemigoTriangulo);
	enemigosTriangulo[1] = new EnemigoTriangulo(400, 60, LEFT, 2, dannoEnemigoTriangulo);

	for (int i = 0; i < numeroBalasPersonaje; i++){
		balasPersonaje[i] = new BalaPersonaje(0, 0, 0);
	}

	for (int i = 0; i < numeroBalasEnemigo; i++){
		balasEnemigo[i] = new BalaEnemigo(0, 0, dannoBalaEnemigo);
	}
}


//limpiarPantalla: función que limpia el elemento display creado al inicio del programa
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPantalla(){
	al_clear_to_color(transparente);
}

//dibujarPrincipal: función encargada de dibujar en pantalla al personaje principal
//Entradas: eje x, eje y, dirección del personaje
//Salidas: ninguna
//Restricciones: se toma en cuenta si es dirección izquierda o derecha, por lo que se tomará la imagen correspondiente
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

//dibujarEnemigoPentagono: función encargada de dibujar en pantalla al enemigo pentágono
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

//dibujarEnemigoTriangulo: función encargada de dibujar en pantalla al enemigo triángulo
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


//cambiarSpriteEnemigoPentagono: función encargada de sumar en uno la variable split del enemigo pentágono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo pentágono
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


//moverEnemigoPentagono: función encargada de sumar los ejer x,y del enemigo pentágono
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúan las posiciones el personaje principal para que el enemigo lo siga
void moverEnemigoPentagono(int movimiento, int tiempo){
	for (int i = 0; i < 2; i++){
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

//moverEnemigoTriangulo: función encargada de sumar los ejer x,y del enemigo triángulo
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúa si el enemigo está al punto mínimo o máximo del eje x, para así cambiar el rumbo
void moverEnemigoTriangulo(int movimiento, int tiempo){
	for (int i = 0; i < 2; i++){
		
		if (enemigosTriangulo[i]->tiempo == tiempo){
			
			if (enemigosTriangulo[i]->direccion == LEFT){
				if (enemigosTriangulo[i]->x == 0) enemigosTriangulo[i]->direccion = RIGHT;
				else enemigosTriangulo[i]->x -= movimiento;
			}

			if (enemigosTriangulo[i]->direccion == RIGHT){
				if (enemigosTriangulo[i]->x == 600) enemigosTriangulo[i]->direccion = LEFT;
				else enemigosTriangulo[i]->x += movimiento;
			}



		}
	}
}

//moverPersonaje: función encargada de sumar los ejer x,y del personaje principal
//Entradas: movimiento (lo que se le suma a los ejes)
//Salidas: ninguna
//Restricciones: ninguna
void moverPersonaje(int movimiento){
	if (teclasDireccion[DOWN]){
		if (personaje->y < 450) personaje->y += movimiento;
	}
	if (teclasDireccion[UP]){
		if (personaje->y > 0) personaje->y -= movimiento;
	}
	if (teclasDireccion[RIGHT]){
		if (personaje->x < 600) personaje->x += movimiento;
	}
	if (teclasDireccion[LEFT]){
		if (personaje->x > 0) personaje->x -= movimiento;
	}
}

bool sinSalud(){
	if (personaje->salud <= 0.0) return true;

	else return false;
}

bool sinVidas(){
	if (personaje->vidas == 0) return true;
	else return false;
}

void restarSalud(float danno){
	personaje->salud -= danno;
}


void restarVidas(){
	personaje->vidas -= 1;
}

void restaurarSalud(){
	personaje->salud = 100.0;
}

void dispararPersonaje(int direccion){
	if (direccion == DOWN || direccion == UP || direccion == RIGHT || direccion == LEFT){
		
		for (int i = 0; i < numeroBalasPersonaje; i++){
			
			if (!balasPersonaje[i]->activado){
				balasPersonaje[i]->x = personaje->x;
				balasPersonaje[i]->y = personaje->y;
				balasPersonaje[i]->orientacion = direccion;
				break;
			
			}
		}
	}
}

void moverBalaPersonaje(int movimiento){
	for (int i = 0; i < numeroBalasPersonaje; i++){
		if (balasPersonaje[i]->activado){

			if (balasPersonaje[i]->orientacion == DOWN) balasPersonaje[i]->y += movimiento;

			if (balasPersonaje[i]->orientacion == UP) balasPersonaje[i]->y -= movimiento;

			if (balasPersonaje[i]->orientacion == RIGHT) balasPersonaje[i]->x += movimiento;
				
			if (balasPersonaje[i]->orientacion == LEFT) balasPersonaje[i]->x -= movimiento;
		
		}

	}
}

//colisionPentagono: función encargada de evaluar si existe alguna colisión entre algún enemigo pentágono y el personaje principal
//Entradas: ninguna
//Salidas: true: si existe una colisión, false: si no existe colisión
//Restricciones: ninguna
void colisionPentagono(){
	for (int i = 0; i < 2; i++){
		if (personaje->x == enemigosPentagono[i]->x && personaje->y == enemigosPentagono[i]->y) restarSalud(enemigosPentagono[i]->danno);
	}
	
}

void colisionTriangulo(){
	for (int i = 0; i < 2; i++){
		if (personaje->x == enemigosTriangulo[i]->x && personaje->y == enemigosTriangulo[i]->y) restarSalud(enemigosTriangulo[i]->danno);
	}
	
}



int main(int argc, char **argv){

	

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//Esta línea de código permite que la ventana tenga la capacidad de cambiar de tamaño
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);


	pantalla = al_create_display(anchoPantalla, largoPantalla);
	al_set_window_position(pantalla, 200, 200);
	al_set_window_title(pantalla, "Geometry World");
	if (!pantalla) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	//Líneas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();
	al_init_ttf_addon();
	//*******************

	//ALLEGRO_FONT *fuente = al_load_font("Archristy.ttf", 12, NULL);
	//al_draw_text(fuente, al_map_rgb(44, 117, 225), anchoPantalla/2, largoPantalla/2, ALLEGRO_ALIGN_CENTRE, "Hola");

	
	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();
	//*******************

	//Se crean los contenedores de las imágenes que se utilizarán en el juego
	//*******************
	principalBuffer = al_create_bitmap(30, 30);
	enemigoPentagonoBuffer = al_create_bitmap(30, 30);
	//*******************

	//Se cargan las imágenes que se van a utilizar en el juego
	//*******************
	principalIzquierda = al_load_bitmap("Imagenes/PrincipalIzquierda.png");
	principalDerecha = al_load_bitmap("Imagenes/PrincipalDerecha.png");
	enemigoPentagono = al_load_bitmap("Imagenes/EnemigoPentagono.png");
	enemigoTriangulo = al_load_bitmap("Imagenes/EnemigoTriangulo.png");
	balaPersonajeH = al_load_bitmap("Imagenes/BalaPersonajeH.png");
	balaPersonajeV = al_load_bitmap("Imagenes/BalaPersonajeV.png");
	balaEnemigo = al_load_bitmap("Imagenes/BalaEnemigo.png");
	//*******************

	//Líneas para obtener las funcionalidades del teclado
	//*******************
	al_init_primitives_addon();
	al_install_keyboard();
	//*******************


	
	bool hecho = false;
	int disparo = -1;
	int movimiento = 10;
	int direccion = RIGHT;


	

	ALLEGRO_KEYBOARD_STATE estadoTeclado;

	ALLEGRO_TIMER *primerTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *segundoTimer = al_create_timer(1.0 / FPS1);
	ALLEGRO_TIMER *tercerTimer = al_create_timer(1.0 / FPS2);
	ALLEGRO_TIMER *cuartoTimer = al_create_timer(1.0 / FPS2);

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

		/*Evento que toma en cuenta la tecla más actual presionada, se van activando en un array que representa cada movimiento
		Observación: Si se presiona una segunda tecla, al mismo tiempo que se está presionando la primera, este evento será capaz de detectar las dos teclas
		y representarlas en el array de activación (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (eventos.keyboard.keycode){

			case ALLEGRO_KEY_DOWN:
				teclasDireccion[DOWN] = true;
				direccion = DOWN;
				break;

			case ALLEGRO_KEY_UP:
				teclasDireccion[UP] = true;
				direccion = UP;
				break;

			case ALLEGRO_KEY_RIGHT:
				teclasDireccion[RIGHT] = true;
				direccion = RIGHT;
				break;

			case ALLEGRO_KEY_LEFT:
				teclasDireccion[LEFT] = true;
				direccion = LEFT;
				break;

			case ALLEGRO_KEY_S:
				direccion = LEFT;
				break;

			case ALLEGRO_KEY_E:
				direccion = UP;
				break;

			case ALLEGRO_KEY_D:
				direccion = DOWN;
				break;

			case ALLEGRO_KEY_F:
				direccion = RIGHT;
				break;

			case ALLEGRO_KEY_ESCAPE:
				hecho = true;
				break;
			}

		}

		/*Evento que toma en cuenta cuando se deja de presionar alguna tecla, funciona de la misma forma que el evento anterior, cuando se deja de presionar una
		tecla, se desactiva del array de activación (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_UP){
			switch (eventos.keyboard.keycode){

			case ALLEGRO_KEY_DOWN:
				teclasDireccion[DOWN] = false;
				break;

			case ALLEGRO_KEY_UP:
				teclasDireccion[UP] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				teclasDireccion[RIGHT] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				teclasDireccion[LEFT] = false;
				break;

		
			case ALLEGRO_KEY_ESCAPE:
				hecho = true;
				break;
			}

		}
		
		

		if (eventos.type == ALLEGRO_EVENT_TIMER){
			if (eventos.timer.source == primerTimer){
				dispararPersonaje(disparo);
				moverPersonaje(movimiento);
				disparo = -1;
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

		colisionPentagono();
		
		if (sinSalud()){
			restarVidas();
			restaurarSalud();
		}

		if (sinVidas()) hecho = true;

		cout << personaje->salud << " " << personaje->vidas << endl;


		

	}


	//Se liberan los elementos de allegro que se utilizaron
	
	//al_destroy_font(fuente);
	
	al_destroy_display(pantalla);

	al_destroy_event_queue(colaEventos);
    
	al_destroy_bitmap(principalIzquierda);
	al_destroy_bitmap(principalDerecha);
	al_destroy_bitmap(enemigoPentagono);
	al_destroy_bitmap(enemigoTriangulo);
	al_destroy_bitmap(balaPersonajeH);
	al_destroy_bitmap(balaPersonajeV);
	al_destroy_bitmap(balaEnemigo);
	al_destroy_bitmap(enemigoPentagonoBuffer);
	
	return 0;
}