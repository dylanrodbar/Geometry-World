//####################                 GEOMETRY WORLD                  ####################
//####################                 Desarrollador:                  ####################
//####################                 Dylan Rodríguez Barboza         ####################

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>


#include "PersonajePrincipal.h"
#include "EnemigoPentagono.h"
#include "EnemigoTriangulo.h"
#include "BalaPersonaje.h"
#include "BalaEnemigo.h"
#include "Bonus.h"

#include <allegro5/allegro.h>

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include <allegro5/allegro_image.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


using namespace std;

//Define que se implementaron para este juego
//**********************************************************
#define anchoPantalla 630
#define largoPantalla 480
#define posicionRectanguloJuego 60
#define FPS 30.0
#define FPS1 10.0
#define FPS2 15.0
#define FPS3 45.0
#define FPS4 1.0
#define FPS5 20.0
#define FPS6 20.0
#define dannoBalaEnemigo 1.5
#define dannoEnemigoPentagono 2.5
#define dannoEnemigoTriangulo 3.5
#define numeroEnemigosPentagono 3
#define numeroEnemigosTriangulo 3
#define numeroBalasPersonaje 5
#define numeroBalasEnemigo 5
#define numeroBonus 2
#define saludBonus 40.5
#define sumaPuntajeColision 50
#define sumaPuntajeTimer 25
#define sumaPuntajeBonusSalud 100
#define sumaPuntajeBonusVida 150
#define posicionInformacion  500
#define mejoresPuntajesJuego 10
//**********************************************************

//Globales
//**********************************************************
enum Direccion {UP, DOWN, RIGHT, LEFT};
enum Teclas {S, E, D, F};

bool teclasDireccion[4] = {false, false, false, false};
bool teclasDisparo[4] = { false, false, false, false };

int puntaje = 0;

FILE *documento;


EnemigoPentagono *enemigosPentagono[numeroEnemigosPentagono];
EnemigoTriangulo *enemigosTriangulo[numeroEnemigosTriangulo];
BalaPersonaje *balasPersonaje[numeroBalasPersonaje];
BalaEnemigo *balasEnemigo[numeroBalasEnemigo];
Bonus *bonus[numeroBonus];
int mejoresPuntajes[mejoresPuntajesJuego];

PersonajePrincipal *personaje;
//**********************************************************

//Elementos de allegro que se utilizarán para el juego
//**********************************************************
ALLEGRO_DISPLAY *pantalla;

ALLEGRO_COLOR transparente;

ALLEGRO_BITMAP *principalBuffer;
ALLEGRO_BITMAP *enemigoPentagonoBuffer;
ALLEGRO_BITMAP *enemigoTrianguloBuffer;

ALLEGRO_BITMAP *principalIzquierda;
ALLEGRO_BITMAP *principalDerecha;
ALLEGRO_BITMAP *principalAturdidoIzquierda;
ALLEGRO_BITMAP *principalAturdidoDerecha;
ALLEGRO_BITMAP *principalCriticoIzquierda;
ALLEGRO_BITMAP *principalCriticoDerecha;
ALLEGRO_BITMAP *enemigoPentagono;
ALLEGRO_BITMAP *enemigoTriangulo;
ALLEGRO_BITMAP *bonusSalud;
ALLEGRO_BITMAP *bonusVida;
ALLEGRO_BITMAP *balaPersonajeH;
ALLEGRO_BITMAP *balaPersonajeV;
ALLEGRO_BITMAP *balaEnemigo;
ALLEGRO_BITMAP *ultimo;

ALLEGRO_SAMPLE *musicaJuego;
ALLEGRO_SAMPLE *disparoPersonaje;
ALLEGRO_SAMPLE *disparoEnemigo;
ALLEGRO_SAMPLE *colisionEnemigo;

ALLEGRO_SAMPLE_INSTANCE *instanciaSonido;

ALLEGRO_FONT *fuente;
//**********************************************************

//Funciones que se utilizarán para el juego
//**********************************************************
void reproducirMusicaJuego();
void reproducirDisparoPersonaje();
void reproducirDisparoEnemigo();
void reproducirColisionEnemigo();

void dibujarMenu();
void dibujarPuntaje();
void dibujarSalud();
void dibujarBarraSalud();
void dibujarVidas();
void dibujarRectanguloJuego();
void dibujarBonus();
void dibujarPrincipal(int, int, int);
void dibujarEnemigoPentagono();
void dibujarEnemigoTriangulo();
void dibujarBalasEnemigoTriangulo();
void dibujarBalasPersonaje();
void dibujarMejoresPuntajes();
void limpiarPantalla();

void limpiarPuntajes();

void guardarPuntajes();
void cargarPuntajes();

void sumarPuntaje(int);
void sumarSalud(float);
void sumarVida(int);

void restarSalud(float);
void restarVidas();
void restaurarSalud();

bool sinSalud();
bool sinVidas();

void cambiarSpriteEnemigoPentagono();
void cambiarSpriteEnemigoTriangulo();

void generarEnemigoPentagono();
void generarBonus();

void crearBalaEnemigoTriangulo(int, int);

void desactivarBalaPersonaje(int);
void desactivarEnemigoPentagono(int);
void desactivarBonus(int);
void desactivarBalaEnemigoTriangulo(int);

void dispararPersonaje(int);
void dispararEnemigoTriangulo();

void moverEnemigoPentagono(int, int);
void moverEnemigoTriangulo(int, int);
void moverPersonaje(int);
void moverBalaPersonaje(int);
void moverBalaEnemigoTriangulo(int);

void colisionPentagono();
void colisionTriangulo();
void colisionBalaEnemigoPentagono();
void colisionBalaEnemigoTriangulo();
void colisionBonus();

void iniciarPuntajes();
void iniciarPersonaje();
void iniciarEnemigosPentagono();
void iniciarEnemigosTriangulo();
void iniciarBalasPersonaje();
void iniciarBalasEnemigo();
void iniciarBonus();
//**********************************************************

//reproducirMusicaJuego: función encargada de reproducir el tema principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirMusicaJuego(){
	al_play_sample(musicaJuego, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}


//reproducirDisparoPersonaje: función encargada de reproducir el sonido para el disparo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoPersonaje(){
	instanciaSonido = al_create_sample_instance(disparoPersonaje);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirDisparoEnemigo: función encargada de reproducir el sonido para el disparo del enemigo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoEnemigo(){
	instanciaSonido = al_create_sample_instance(disparoEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirColisionEnemigo: función encargada de reproducir el sonido para la colisión entre el enemigo y la bala del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirColisionEnemigo(){
	instanciaSonido = al_create_sample_instance(colisionEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//dibujarMenu: función encargada de dibujar en pantalla las opciones disponibles del menu del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMenu(){
	al_draw_text(fuente, al_map_rgb(44, 117, 225), 300, 160, ALLEGRO_ALIGN_CENTRE, "1- Jugar");
	al_draw_text(fuente, al_map_rgb(44, 117, 225), 300, 210, ALLEGRO_ALIGN_CENTRE, "2- Ver mejores puntajes");
	al_draw_text(fuente, al_map_rgb(44, 117, 225), 300, 260, ALLEGRO_ALIGN_CENTRE, "3- Salir");
	al_flip_display();
}


//dibujarPuntaje: función encargada de dibujar en pantalla el puntaje actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarPuntaje(){
	char puntajeCadena[10];
	sprintf(puntajeCadena, "%i", puntaje);
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion - 50, posicionRectanguloJuego + 25, ALLEGRO_ALIGN_CENTRE, "Puntaje:");
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion, posicionRectanguloJuego + 25, ALLEGRO_ALIGN_CENTRE, puntajeCadena);
	al_flip_display();
}


//dibujarSalud: función encargada de dibujar en pantalla el la salud actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarSalud(){
	char saludCadena[10];
	sprintf(saludCadena, "%.2f", personaje->salud);
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion - 50, posicionRectanguloJuego + 40, ALLEGRO_ALIGN_CENTRE, "Salud:");
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion, posicionRectanguloJuego + 40, ALLEGRO_ALIGN_CENTRE, saludCadena);
	al_flip_display();
}


//dibujarBarraSalud: función encargada de dibujar en pantalla una barra que sea equivalente a la salud actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarBarraSalud(){
	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_filled_rectangle(posicionInformacion - 65, posicionRectanguloJuego + 60, posicionInformacion + personaje->salud, posicionRectanguloJuego + 65, al_map_rgb(44, 117, 255));
	al_flip_display();
}


//dibujarVidas: función encargada de dibujar en pantalla las vidas actuales
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarVidas(){
	char vidasCadena[10];
	sprintf(vidasCadena, "%i", personaje->vidas);
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion - 50, posicionRectanguloJuego + 70, ALLEGRO_ALIGN_CENTRE, "Vidas:");
	al_draw_text(fuente, al_map_rgb(44, 117, 225), posicionInformacion, posicionRectanguloJuego + 70, ALLEGRO_ALIGN_CENTRE, vidasCadena);
	al_flip_display();
}


//dibujarRectanguloJuego: función encargada de dibujar en pantalla el delimitador entre los enemigos triángulo y los demás componentes del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarRectanguloJuego(){
	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_filled_rectangle(0, posicionRectanguloJuego, anchoPantalla, posicionRectanguloJuego + 5, al_map_rgb(44, 117, 255));
	al_flip_display();
}


//dibujarBonus: función encargada de dibujar en pantalla los bonus disponibles
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay bonus activados en el array (valor diferente de NULL)
void dibujarBonus(){
	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] != NULL){
			al_set_target_bitmap(al_get_backbuffer(pantalla));

			if (bonus[i]->tipo == 0) al_draw_bitmap(bonusSalud, bonus[i]->x, bonus[i]->y, NULL);

			else if (bonus[i]->tipo == 1) al_draw_bitmap(bonusVida, bonus[i]->x, bonus[i]->y, NULL);

			al_flip_display();
		}
	}

}


//dibujarPrincipal: función encargada de dibujar en pantalla al personaje principal
//Entradas: eje x, eje y, dirección del personaje
//Salidas: ninguna
//Restricciones: se toma en cuenta si es dirección izquierda o derecha, por lo que se tomará la imagen correspondiente
void dibujarPrincipal(int x, int y, int direccion){

	switch (direccion){
	case LEFT:
		if (personaje->salud > 50 && personaje->salud <= 100){
			al_draw_bitmap(principalIzquierda, x, y, NULL);
			ultimo = principalIzquierda;
		}

		else if (personaje->salud > 25 && personaje->salud <= 50){
			al_draw_bitmap(principalAturdidoIzquierda, x, y, NULL);
			ultimo = principalAturdidoIzquierda;
		}

		else if (personaje->salud > 0 && personaje->salud <= 25){
			al_draw_bitmap(principalCriticoIzquierda, x, y, NULL);
			ultimo = principalCriticoIzquierda;
		}

		break;

	case RIGHT:
		if (personaje->salud > 50 && personaje->salud <= 100){
			al_draw_bitmap(principalDerecha, x, y, NULL);
			ultimo = principalDerecha;
		}

		else if (personaje->salud > 25 && personaje->salud <= 50){
			al_draw_bitmap(principalAturdidoDerecha, x, y, NULL);
			ultimo = principalAturdidoDerecha;
		}

		else if (personaje->salud > 0 && personaje->salud <= 25){
			al_draw_bitmap(principalCriticoDerecha, x, y, NULL);
			ultimo = principalCriticoDerecha;
		}

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
	for (int i = 0; i < numeroEnemigosPentagono; i++){

		if (enemigosPentagono[i] != NULL){
			al_set_target_bitmap(enemigoPentagonoBuffer);
			al_draw_bitmap_region(enemigoPentagono, enemigosPentagono[i]->sprite * 30, 0, 30, 30, 0, 0, NULL);
			al_set_target_bitmap(al_get_backbuffer(pantalla));
			al_draw_bitmap(enemigoPentagonoBuffer, enemigosPentagono[i]->x, enemigosPentagono[i]->y, NULL);
			al_flip_display();
		}
	}
}


//dibujarEnemigoTriangulo: función encargada de dibujar en pantalla al enemigo triángulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarEnemigoTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){
		if (enemigosTriangulo[i] != NULL){
			
			al_set_target_bitmap(enemigoTrianguloBuffer);
			al_clear_to_color(transparente);
			al_draw_bitmap_region(enemigoTriangulo, enemigosTriangulo[i]->sprite * 60, 0, 55, 60, 0, 0, NULL);
			al_set_target_bitmap(al_get_backbuffer(pantalla));
			al_draw_bitmap(enemigoTrianguloBuffer, enemigosTriangulo[i]->x, enemigosTriangulo[i]->y, NULL);
			al_flip_display();
		}
	}
}


//dibujarBalasEnemigoTriangulo: función encargada de dibujar en pantalla las balas que tira el enemigo triángulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si la bala está activada (valor diferente de NULL)
void dibujarBalasEnemigoTriangulo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] != NULL){
			al_set_target_bitmap(al_get_backbuffer(pantalla));
			al_draw_bitmap(balaEnemigo, balasEnemigo[i]->x, balasEnemigo[i]->y, NULL);
			al_flip_display();
		}
	}
}


//dibujarBalasPersonaje: función encargada de dibujar en pantalla las balas que irá disparando el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evaluará si hay una estructura bala en el array disponible (que no tenga valor NULL)
void dibujarBalasPersonaje(){
	for (int i = 0; i < numeroBalasPersonaje; i++){

		if (balasPersonaje[i] != NULL){

			al_set_target_bitmap(al_get_backbuffer(pantalla));
			if (balasPersonaje[i]->orientacion == UP || balasPersonaje[i]->orientacion == DOWN)

				al_draw_bitmap(balaPersonajeV, balasPersonaje[i]->x, balasPersonaje[i]->y, NULL);

			if (balasPersonaje[i]->orientacion == LEFT || balasPersonaje[i]->orientacion == RIGHT)

				al_draw_bitmap(balaPersonajeH, balasPersonaje[i]->x, balasPersonaje[i]->y, NULL);

			al_flip_display();
		}
	}
}


//dibujarMejoresPuntajes: función encargada de dibujar en pantalla el top 10 de los puntajes del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMejoresPuntajes(){
	char puntajeCadena[10];
	char posicionCadena[10];
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		sprintf(puntajeCadena, "%i", mejoresPuntajes[i]);
		sprintf(posicionCadena, "%i", i + 1);

		al_draw_text(fuente, al_map_rgb(44, 117, 225), 20, i * 30, ALLEGRO_ALIGN_CENTRE, posicionCadena);
		al_draw_text(fuente, al_map_rgb(44, 117, 225), 30, i * 30, ALLEGRO_ALIGN_CENTRE, " - ");
		al_draw_text(fuente, al_map_rgb(44, 117, 225), 60, i * 30, ALLEGRO_ALIGN_CENTRE, puntajeCadena);

	}
	al_flip_display();
}


//limpiarPantalla: función que limpia el elemento display creado al inicio del programa
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPantalla(){
	al_clear_to_color(transparente);
}


//limpiarPuntajes: función encargada de setear en 0 los elementos del array de puntajes
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPuntajes(){
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		mejoresPuntajes[i] = 0;
	}
}


//guardarPuntajes: función encargada de guardar en un txt los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void guardarPuntajes(){
	documento = fopen("Puntajes.txt", "a");
	fprintf(documento, "%i \n", puntaje);
	fclose(documento);

}


//cargarPuntajes: función encargada de cargar los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void cargarPuntajes(){
	char puntajeTxt[10];
	int punt;
	int aux;
	documento = fopen("Puntajes.txt", "r");
	while (!feof(documento)){
		fgets(puntajeTxt, 10, documento);
		punt = atoi(puntajeTxt);
		for (int i = 0; i < mejoresPuntajesJuego; i++){
			if (punt >= mejoresPuntajes[i]){
				aux = mejoresPuntajes[i];
				mejoresPuntajes[i] = punt;
				punt = aux;
			}
		}

	}
}


//sumarPuntaje: función encargada de aumentar el puntaje obtenido en el juego
//Entradas: el valor que se sumará
//Salidas: ninguna
//Restricciones: ninguna
void sumarPuntaje(int suma){
	puntaje += suma;
}


//sumarSalud: función encargada de sumar valor a la salud del personaje
//Entradas: el valor que se le suma a la salud
//Salidas: ninguna
//Restricciones: si la suma es mayor a 100.0, se le asigna 100.0 a la salud
void sumarSalud(float salud){
	personaje->salud += salud;
	if (personaje->salud > 100.0) personaje->salud = 100.0;
}


//sumarVida: función encargada de sumar un valor a las vidas del personaje
//Entradas: vida que se suma
//Salidas: ninguna
//Restricciones: ninguna
void sumarVida(int vida){
	personaje->vidas += vida;
}


//restarSalud: función encargada de restar salud al personaje principal
//Entradas: el daño que se restará a la salud
//Salidas: ninguna
//Restricciones: ninguna
void restarSalud(float danno){
	personaje->salud -= danno;
}


//restarVidas: función encargadas de restar vidas al personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restarVidas(){
	personaje->vidas -= 1;
}


//restaurarSalud: función encargada de restaurar toda la salud del personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restaurarSalud(){
	personaje->salud = 100.0;
}


//sinSalud: función encargada de evaluar si el personaje principal tiene o no salud
//Entradas: ninguna
//Salidas: true, si no tiene salud false: si aún tiene salud
//Restricciones: se evalúa si la salud es menor o igual a 0
bool sinSalud(){
	if (personaje->salud <= 0.0) return true;

	else return false;
}


//sinVidas: función encargada de evaluar si el personaje principal tiene vidas o no
//Entradas: ninguna
//Salidas: true: si aún posee vidas false: si no posee vidas
//Restricciones: se evalúa si la cantidad de vidas es igual a 0
bool sinVidas(){
	if (personaje->vidas == 0) return true;
	else return false;
}


//cambiarSpriteEnemigoPentagono: función encargada de sumar en uno la variable split del enemigo pentágono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo pentágono
void cambiarSpriteEnemigoPentagono(){
	for (int i = 0; i < numeroEnemigosPentagono; i++){

		if (enemigosPentagono[i] != NULL){

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
}


//cambiarSpriteEnemigoTriangulo: función encargada de sumar en uno la variable split del enemigo triángulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo triángulo
void cambiarSpriteEnemigoTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i] != NULL){

			if (enemigosTriangulo[i]->sprite == 4) enemigosTriangulo[i]->sumando = false;
			
			if (enemigosTriangulo[i]->sprite == 0) enemigosTriangulo[i]->sumando = true;
			
			if (enemigosTriangulo[i]->sumando){
				if (enemigosTriangulo[i]->sumarSprite == 0){
					enemigosTriangulo[i]->sprite += 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}
			
			if (!enemigosTriangulo[i]->sumando){
				if (enemigosTriangulo[i]->sumarSprite == 0){
					enemigosTriangulo[i]->sprite -= 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}
		}
	}
}


//generarEnemigoPentagono: función encargada de agregar un nuevo enemigo al array de enemigos pentágono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si el array de enemigos pentágono tiene un espacio disponible (o sea, valor NULL)
void generarEnemigoPentagono(){
	int x;
	int y;

	for (int i = 0; i < numeroEnemigosPentagono; i++){

		if (enemigosPentagono[i] == NULL){
			x = rand() % 41 + 5;
			y = rand() % 41 + 10;
			enemigosPentagono[i] = new EnemigoPentagono(x * 10, y * 10, i + 1, 0, dannoEnemigoPentagono);
			break;
		}
	}
}


//generarBonus: función encargada de crear un bonus en el juego (se agrega al array de bonus)
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si se posee espacio en el array (valor NULL)
void generarBonus(){
	int x;
	int y;

	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] == NULL){
			x = 33 + 0;
			y = rand() % 6 + 7;
			bonus[i] = new Bonus(x * 10, y * 10, i % 2, saludBonus);
			break;
		}
	}
}


//crearBalaEnemigoTriangulo: función encargada de crear una nueva bala en el array de balas del enemigo
//Entradas: eje x, eje y
//Salidas: ninguna
//Restricciones: se evalúa si hay un valor desactivado (NULL) para asignar la bala
void crearBalaEnemigoTriangulo(int x, int y){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] == NULL){
			balasEnemigo[i] = new BalaEnemigo(x, y, dannoBalaEnemigo);
			break;
		}
	}
}


//desactivarBalaPersonaje: función encargada de dejar un espacio libre en el array de balas
//Entradas: posición del array que se liberará
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaPersonaje(int posicion){

	balasPersonaje[posicion] = NULL;
}


//desactivarEnemigoPentagono: función encargada de quitar la disponibilidad de un enemigo del juego
//Entradas: la posición del enemigo que se elimina
//Salidas: ninguna
//Restricciones: ninguna
void desactivarEnemigoPentagono(int posicion){
	enemigosPentagono[posicion] = NULL;
}


//desactivarBonus: función encargada de desactivar el bonus del array (hacerlo NULL)
//Entradas: la posición del array que se elimina
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBonus(int posicion){
	bonus[posicion] = NULL;
}


//desactivarBalaEnemigoTriangulo: función encargada de desactivar balas del array de balas de enemigos
//Entradas: posición
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaEnemigoTriangulo(int posicion){

	balasEnemigo[posicion] = NULL;
}


//dispararPersonaje: función encargada de crear nuevas balas
//Entradas: la dirección (UP, DOWN, LEFT, RIGHT)
//Salidas: ninguna
//Restricciones: se evalúa si existe algún espacio disponible en el array (o sea, si es NULL)
void dispararPersonaje(int direccion){
	if (direccion == DOWN || direccion == UP || direccion == RIGHT || direccion == LEFT){

		for (int i = 0; i < numeroBalasPersonaje; i++){

			if (balasPersonaje[i] == NULL){
				balasPersonaje[i] = new BalaPersonaje(personaje->x, personaje->y, direccion);
				reproducirDisparoPersonaje();
				break;

			}
		}
	}
}


//dispararEnemigoTriangulo: función encargada de llamar a la función para crear balas del enemigo en caso de cumplirse cierta condición
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa que haya enemigos activados (diferente de NULL), se evalúa si se cumple el rango para lanzar la bala
void dispararEnemigoTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i] != NULL){

			if (abs(enemigosTriangulo[i]->x - personaje->x) == 50){
				crearBalaEnemigoTriangulo(enemigosTriangulo[i]->x, enemigosTriangulo[i]->y);
				reproducirDisparoEnemigo();
			}

		}
	}
}


//moverEnemigoPentagono: función encargada de sumar los ejer x,y del enemigo pentágono
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúan las posiciones el personaje principal para que el enemigo lo siga
void moverEnemigoPentagono(int movimiento, int tiempo){
	for (int i = 0; i < numeroEnemigosPentagono; i++){

		if (enemigosPentagono[i] != NULL){

			if (enemigosPentagono[i]->tiempo == tiempo){

				if (personaje->y > enemigosPentagono[i]->y){
					if (enemigosPentagono[i]->y < (largoPantalla - 30))
						enemigosPentagono[i]->y += movimiento;
				}

				if (personaje->y < enemigosPentagono[i]->y){
					if (enemigosPentagono[i]->y > posicionRectanguloJuego + 5)
						enemigosPentagono[i]->y -= movimiento;
				}

				if (personaje->x > enemigosPentagono[i]->x){
					if (enemigosPentagono[i]->x < (anchoPantalla - 30))
						enemigosPentagono[i]->x += movimiento;
				}

				if (personaje->x < enemigosPentagono[i]->x){
					if (enemigosPentagono[i]->x > 0)
						enemigosPentagono[i]->x -= movimiento;
				}

			}
		}
	}
}


//moverEnemigoTriangulo: función encargada de sumar los ejer x,y del enemigo triángulo
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúa si el enemigo está al punto mínimo o máximo del eje x, para así cambiar el rumbo
void moverEnemigoTriangulo(int movimiento, int tiempo){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i]->tiempo == tiempo){

			if (enemigosTriangulo[i]->direccion == LEFT){
				if (enemigosTriangulo[i]->x == 0) enemigosTriangulo[i]->direccion = RIGHT;
				else enemigosTriangulo[i]->x -= movimiento;
			}

			if (enemigosTriangulo[i]->direccion == RIGHT){
				if (enemigosTriangulo[i]->x == (anchoPantalla - 50)) enemigosTriangulo[i]->direccion = LEFT;
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
		if (personaje->y < (largoPantalla - 30)) personaje->y += movimiento;
	}
	if (teclasDireccion[UP]){
		if (personaje->y > posicionRectanguloJuego + 10) personaje->y -= movimiento;
	}
	if (teclasDireccion[RIGHT]){
		if (personaje->x < (anchoPantalla - 30)) personaje->x += movimiento;
	}
	if (teclasDireccion[LEFT]){
		if (personaje->x > 0) personaje->x -= movimiento;
	}
}


//moverBalaPersonaje: función encargada de cambiar el valor de los ejes de las balas disponibles en el array
//Entradas: movimiento (valor que se sumará a los ejes)
//Salidas: ninguna
//Restricciones: se evalúa si hay alguna bala acticava, además de llamar a la función para desactivarla si llega a los ejes límite
void moverBalaPersonaje(int movimiento){
	for (int i = 0; i < numeroBalasPersonaje; i++){

		if (balasPersonaje[i] != NULL){

			if (balasPersonaje[i]->orientacion == DOWN){
				if (balasPersonaje[i]->y == (largoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == UP){
				if (balasPersonaje[i]->y == 60) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y -= movimiento;
			}

			else if (balasPersonaje[i]->orientacion == RIGHT){
				if (balasPersonaje[i]->x == (anchoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->x += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == LEFT){
				if (balasPersonaje[i]->x == 0) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->x -= movimiento;
			}



		}

	}
}


//moverBalaEnemigoTriangulo: función encargada de cambiar los ejes de las balas de los enemigos en el array
//Entradas: movimiento (valor que se suma a los ejes)
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL), se evalúa si las balas están fuera del rango
void moverBalaEnemigoTriangulo(int movimiento){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] != NULL){
			if (balasEnemigo[i]->y == (largoPantalla - 30)) desactivarBalaEnemigoTriangulo(i);
			else balasEnemigo[i]->y += movimiento;
		}
	}
}


//colisionPentagono: función encargada de evaluar si existe alguna colisión entre algún enemigo pentágono y el personaje principal
//Entradas: ninguna
//Salidas: true: si existe una colisión, false: si no existe colisión
//Restricciones: ninguna
void colisionPentagono(){
	for (int i = 0; i < numeroEnemigosPentagono; i++){
		if (enemigosPentagono[i] != NULL){
			if (personaje->x == enemigosPentagono[i]->x && personaje->y == enemigosPentagono[i]->y) restarSalud(enemigosPentagono[i]->danno);
		}
	}

}


//colisionTriangulo: función encargada de evaluar si existe colisión entre el personaje principal y algún enemigo triángulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si hay colisión, se llama a la función para restar salud del personaje principal
void colisionTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){
		if (personaje->x == enemigosTriangulo[i]->x && personaje->y == enemigosTriangulo[i]->y) restarSalud(enemigosTriangulo[i]->danno);
	}

}


//colisionBalaEnemigoPentagono: función encargada de evaluar si existe una colisión entre una bala del personaje principal y un enemigo pentágono
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas, se evalúa si hay enemigos pentágono activados, si existe colisión, se llama a la función para desactivar al enemigo pentágono
void colisionBalaEnemigoPentagono(){
	for (int i = 0; i < numeroBalasPersonaje; i++){

		for (int j = 0; j < numeroEnemigosPentagono; j++){

			if (enemigosPentagono[j] != NULL && balasPersonaje[i] != NULL){

				if (balasPersonaje[i]-> x == enemigosPentagono[j]->x && balasPersonaje[i]->y == enemigosPentagono[j]->y){
					desactivarEnemigoPentagono(j);
					desactivarBalaPersonaje(i);
					sumarPuntaje(sumaPuntajeColision);
					reproducirColisionEnemigo();
				}
			}
		}
	}
}


//colisionBalaEnemigoTriangulo: función encargada de evaluar si existe colisión entre alguna bala del enemigo y el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL)
void colisionBalaEnemigoTriangulo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){

		if (balasEnemigo[i] != NULL){
			if ((personaje->x - 15 < balasEnemigo[i]->x && balasEnemigo[i]->x < personaje->x + 15) && (personaje->y - 15 < balasEnemigo[i]->y && balasEnemigo[i]->y < personaje->y + 15)){
				restarSalud(balasEnemigo[i]->danno);
				desactivarBalaEnemigoTriangulo(i);
			}
		}
	}
}


//colisionBonus: función encargada de evaluar si existe colisión entre un bonus y el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay bonus activados (diferente de NULL)
void colisionBonus(){
	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] != NULL){
			if ((personaje->x - 15 < bonus[i]->x  &&  bonus[i]->x < personaje->x + 15) && (personaje->y - 15 < bonus[i]->y && bonus[i]->y < personaje->y + 15)){

				if (bonus[i]->tipo == 0){
					sumarSalud(bonus[i]->salud);
					sumarPuntaje(sumaPuntajeBonusSalud);

				}

				else if (bonus[i]->tipo == 1){
					sumarVida(1);
					sumarPuntaje(sumaPuntajeBonusVida);
				}

				desactivarBonus(i);

			}
		}
	}
}


//iniciarPuntajes: función encargada de inicializar el array de puntajes en 0
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPuntajes(){
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		mejoresPuntajes[i] = 0;
	}
}


//iniciarPersonaje: función encargada de iniciar el personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPersonaje(){
	personaje = new PersonajePrincipal(10, 100, 3, 100.0);
}


//iniciarEnemigosPentagono: función encargada de iniciar los enemigos pentágono del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarEnemigosPentagono(){
	int x;
	int y;

	for (int i = 0; i < numeroEnemigosPentagono; i++){
		x = rand() % 21 + 5;
		y = rand() % 21 + 10;
		enemigosPentagono[i] = new EnemigoPentagono(x * 10, y * 10, i + 1, 0, dannoEnemigoPentagono);

	}

}


//iniciarEnemigosTriangulo: función encargada de iniciar los enemigos triángulo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarEnemigosTriangulo(){
	int x;

	for (int i = 0; i < numeroEnemigosTriangulo; i++){
		x = rand() % 31 + 5;
		enemigosTriangulo[i] = new EnemigoTriangulo(x * 10, 0, (i % 2 + 2), i + 1, 0, dannoEnemigoTriangulo);
	}

}


//iniciarBalasPersonaje: función encargada de iniciar las balas del personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasPersonaje(){
	for (int i = 0; i < numeroBalasPersonaje; i++){
		balasPersonaje[i] = NULL;
	}
}


//iniciarBalasEnemigo: función encargada de iniciar las balas del enemigo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasEnemigo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		balasEnemigo[i] = NULL;
	}
}


//iniciarBonus: función encargada de iniciar los bonus del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBonus(){
	for (int i = 0; i < numeroBonus; i++){
		bonus[i] = NULL;
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

	transparente = al_map_rgb(0, 0, 0);

	//Líneas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();
	al_init_ttf_addon();
	//*******************

	fuente = al_load_font("Archristy.ttf", 16, NULL);


	al_init_primitives_addon();

	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();
	//*******************

	//Se crean los contenedores de las imágenes que se utilizarán en el juego
	//*******************
	principalBuffer = al_create_bitmap(30, 30);
	enemigoPentagonoBuffer = al_create_bitmap(30, 30);
	enemigoTrianguloBuffer = al_create_bitmap(60, 60);
	//*******************

	//Se cargan las imágenes que se van a utilizar en el juego
	//*******************
	principalIzquierda = al_load_bitmap("Imagenes/PrincipalIzquierda.png");
	principalDerecha = al_load_bitmap("Imagenes/PrincipalDerecha.png");
	principalAturdidoIzquierda = al_load_bitmap("Imagenes/PrincipalAturdidoIzquierda.png");
	principalAturdidoDerecha = al_load_bitmap("Imagenes/PrincipalAturdidoDerecha.png");
	principalCriticoIzquierda = al_load_bitmap("Imagenes/PrincipalCriticoIzquierda.png");
	principalCriticoDerecha = al_load_bitmap("Imagenes/PrincipalCriticoDerecha.png");
	enemigoPentagono = al_load_bitmap("Imagenes/EnemigoPentagono.png");
	enemigoTriangulo = al_load_bitmap("Imagenes/EnemigoTriangulo.png");
	balaPersonajeH = al_load_bitmap("Imagenes/BalaPersonajeH.png");
	balaPersonajeV = al_load_bitmap("Imagenes/BalaPersonajeV.png");
	balaEnemigo = al_load_bitmap("Imagenes/BalaEnemigo.png");
	bonusSalud = al_load_bitmap("Imagenes/bonusSalud.png");
	bonusVida = al_load_bitmap("Imagenes/bonusVida.png");
	//*******************

	//Líneas para obtener las funcionalidades de los audios
	//*******************
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1000);
	//*******************

	//Se cargan los audios que se van a utilizar en el juego
	//*******************
	musicaJuego = al_load_sample("Musica/MusicaJuego.ogg");
	disparoPersonaje = al_load_sample("Musica/DisparoPersonaje.ogg");
	disparoEnemigo = al_load_sample("Musica/DisparoEnemigo.ogg");
	colisionEnemigo = al_load_sample("Musica/ColisionEnesmigo.ogg");
	//*******************

	//Líneas para obtener las funcionalidades del teclado
	//*******************
	al_init_primitives_addon();
	al_install_keyboard();
	//*******************



	bool hecho, menu = true;
	int disparo = -1;
	int movimiento = 10;
	int direccion = RIGHT;


	//Líneas de código para la llamada del menu principal

	//*******************************************************************************
	dibujarMenu();
	reproducirMusicaJuego();
	ALLEGRO_KEYBOARD_STATE estadoTeclado;

	al_get_keyboard_state(&estadoTeclado);

	ALLEGRO_EVENT eventoMenu;

	ALLEGRO_EVENT_QUEUE *colaEventoMenu = al_create_event_queue();

	al_register_event_source(colaEventoMenu, al_get_keyboard_event_source());

	iniciarPuntajes();
	while (menu){
		al_wait_for_event(colaEventoMenu, &eventoMenu);

		if (eventoMenu.type = ALLEGRO_EVENT_KEY_DOWN){

			if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_1){
				hecho = true;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_2){
				limpiarPantalla();
				cargarPuntajes();
				dibujarMejoresPuntajes();
				limpiarPuntajes();
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_3){
				hecho = false;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_S){
				limpiarPantalla();
				dibujarMenu();
			}
		}
	}
	//*******************************************************************************

	//Timers que se necesitarán para el juego
	//**********************************************************
	ALLEGRO_TIMER *primerTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *segundoTimer = al_create_timer(1.0 / FPS1);
	ALLEGRO_TIMER *tercerTimer = al_create_timer(1.0 / FPS2);
	ALLEGRO_TIMER *cuartoTimer = al_create_timer(1.0 / FPS3);
	ALLEGRO_TIMER *quintoTimer = al_create_timer(5.0 / FPS4);
	ALLEGRO_TIMER *sextoTimer = al_create_timer(20.0 / FPS4);
	ALLEGRO_TIMER *septimoTimer = al_create_timer(1.0 / FPS5);
	ALLEGRO_TIMER *octavoTimer = al_create_timer(1.0 / FPS6);
	//**********************************************************

	//Se crea una cola de eventos
	ALLEGRO_EVENT_QUEUE *colaEventos = al_create_event_queue();

	//Registro de los eventos
	//**********************************************************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(tercerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(cuartoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(quintoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(sextoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(septimoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(octavoTimer));
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	//**********************************************************


	//Inicialización de los timer
	//**********************************************************
	al_start_timer(primerTimer);
	al_start_timer(segundoTimer);
	al_start_timer(tercerTimer);
	al_start_timer(cuartoTimer);
	al_start_timer(quintoTimer);
	al_start_timer(sextoTimer);
	al_start_timer(septimoTimer);
	al_start_timer(octavoTimer);
	//**********************************************************

	//Llamado a las funciones que inicializan los componentes lógicos del juego
	//**********************************************************
	iniciarPersonaje();
	iniciarEnemigosPentagono();
	iniciarEnemigosTriangulo();
	iniciarBalasPersonaje();
	iniciarBalasEnemigo();
	iniciarBonus();
	//**********************************************************


	while (hecho){

		ALLEGRO_EVENT eventos;

		al_wait_for_event(colaEventos, &eventos);


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
				disparo = LEFT;
				break;

			case ALLEGRO_KEY_E:
				disparo = UP;
				break;

			case ALLEGRO_KEY_D:
				disparo = DOWN;
				break;

			case ALLEGRO_KEY_F:
				disparo = RIGHT;
				break;

			case ALLEGRO_KEY_ESCAPE:
				guardarPuntajes();
				hecho = false;
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
				moverPersonaje(movimiento);
			}

			else if (eventos.timer.source == segundoTimer){
				moverEnemigoPentagono(movimiento, 1);
				moverEnemigoTriangulo(movimiento, 1);
			}

			else if (eventos.timer.source == tercerTimer){
				moverEnemigoPentagono(movimiento, 2);
				moverEnemigoTriangulo(movimiento, 2);
			}

			else if (eventos.timer.source == cuartoTimer){
				dispararPersonaje(disparo);
				disparo = -1;
				moverBalaPersonaje(movimiento);

				dispararEnemigoTriangulo();
				moverBalaEnemigoTriangulo(movimiento);

			}

			else if (eventos.timer.source == quintoTimer){
				sumarPuntaje(sumaPuntajeTimer);
			}

			else if (eventos.timer.source == sextoTimer){
				generarBonus();
			}

			else if (eventos.timer.source == septimoTimer){
				dibujarPrincipal(personaje->x, personaje->y, direccion);
				dibujarEnemigoPentagono();
				dibujarEnemigoTriangulo();
				dibujarBalasPersonaje();
				dibujarBalasEnemigoTriangulo();
				dibujarBonus();
				dibujarRectanguloJuego();
				dibujarPuntaje();
				dibujarSalud();
				dibujarVidas();
				dibujarBarraSalud();

			}

			else if (eventos.timer.source == octavoTimer){
				moverEnemigoPentagono(movimiento, 3);
				moverEnemigoTriangulo(movimiento, 3);
			}

		}

		generarEnemigoPentagono();



		cambiarSpriteEnemigoPentagono();
		cambiarSpriteEnemigoTriangulo();
		limpiarPantalla();

		colisionPentagono();
		colisionBalaEnemigoPentagono();
		colisionBalaEnemigoTriangulo();
		colisionBonus();

		if (sinSalud()){
			restarVidas();
			restaurarSalud();
		}

		if (sinVidas()){
			hecho = false;
			guardarPuntajes();
		}



	}



	//Se liberan los elementos de allegro que se utilizaron

	al_destroy_font(fuente);

	al_destroy_display(pantalla);

	al_destroy_event_queue(colaEventos);
	al_destroy_event_queue(colaEventoMenu);



	al_destroy_bitmap(principalIzquierda);
	al_destroy_bitmap(principalDerecha);
	al_destroy_bitmap(principalAturdidoIzquierda);
	al_destroy_bitmap(principalAturdidoDerecha);
	al_destroy_bitmap(principalCriticoIzquierda);
	al_destroy_bitmap(principalCriticoDerecha);
	al_destroy_bitmap(enemigoPentagono);
	al_destroy_bitmap(enemigoTriangulo);
	al_destroy_bitmap(balaPersonajeH);
	al_destroy_bitmap(balaPersonajeV);
	al_destroy_bitmap(balaEnemigo);
	al_destroy_bitmap(enemigoPentagonoBuffer);
	al_destroy_bitmap(enemigoTrianguloBuffer);
	al_destroy_bitmap(bonusSalud);
	al_destroy_bitmap(bonusVida);

	al_destroy_sample(musicaJuego);
	al_destroy_sample(disparoPersonaje);
	al_destroy_sample(disparoEnemigo);
	al_destroy_sample(colisionEnemigo);

	al_destroy_sample_instance(instanciaSonido);

	al_destroy_timer(primerTimer);
	al_destroy_timer(segundoTimer);
	al_destroy_timer(tercerTimer);
	al_destroy_timer(cuartoTimer);
	al_destroy_timer(quintoTimer);
	al_destroy_timer(sextoTimer);
	al_destroy_timer(septimoTimer);
	al_destroy_timer(octavoTimer);

	//al_destroy_font(fuente);



	
	return 0;
}