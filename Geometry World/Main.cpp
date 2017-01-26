#include <stdio.h>
#include <iostream>

#include "PersonajePrincipal.h"
#include "Enemigo.h"

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

enum Direccion {UP, DOWN, RIGHT, LEFT, SPACE};

bool teclas[4] = {false, false, false, false};

PersonajePrincipal *personaje;
Enemigo *enemigoJ;

ALLEGRO_COLOR transparente = al_map_rgb(0, 0, 0);

ALLEGRO_BITMAP *principalIzquierda;
ALLEGRO_BITMAP *principalDerecha;
ALLEGRO_BITMAP *enemigo;
ALLEGRO_BITMAP *ultimo;


void iniciarElementosJuego(){
	personaje = new PersonajePrincipal(10, 10, 3, 100.0);
	enemigoJ = new Enemigo(60, 60);

}

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

void dibujarEnemigo(int x, int y){
	al_draw_bitmap(enemigo, x, y, NULL);
	al_flip_display();
}

int main(int argc, char **argv){

	ALLEGRO_DISPLAY *pantalla = NULL;

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

	//Se cargan las imágenes que se van a utilizar en el juego
	//*******************
	principalIzquierda = al_load_bitmap("Imagenes/PrincipalIzquierda.png");
	principalDerecha = al_load_bitmap("Imagenes/PrincipalDerecha.png");
	enemigo = al_load_bitmap("Imagenes/Enemigo.png");
	//*******************

	//Líneas para obtener las funcionalidades del teclado
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

	ALLEGRO_EVENT_QUEUE *colaEventos = al_create_event_queue();

	//Registramos los eventos
	//*******************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer));
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	//*******************

	al_start_timer(primerTimer);
	al_start_timer(segundoTimer);

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
		tecla, se desactiva del array de activación (teclas)
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

			if (eventos.timer.source == segundoTimer){
				if (personaje->y > enemigoJ->y){
					if (enemigoJ->y < 450)
						enemigoJ->y += movimiento;
				}

				if (personaje->y < enemigoJ->y){
					if (enemigoJ->y > 0)
						enemigoJ->y -= movimiento;
				}

				if (personaje->x > enemigoJ->x){
					if (enemigoJ->x < 600)
						enemigoJ->x += movimiento;
				}

				if (personaje->x < enemigoJ->x){
					if (enemigoJ->x > 0)
						enemigoJ->x -= movimiento;
				}
			}
			
		}
			
		dibujarPrincipal(personaje->x, personaje->y, direccion);
		dibujarEnemigo(enemigoJ->x, enemigoJ->y);
		al_clear_to_color(transparente);
			


		

	}

	

	
	//al_rest(10.0);

	//Se liberan los elementos de allegro que se utilizaron
	
	//al_destroy_font(fuente);
	
	al_destroy_display(pantalla);

	al_destroy_event_queue(colaEventos);
    
	al_destroy_bitmap(principalIzquierda);
	al_destroy_bitmap(principalDerecha);
	return 0;
}