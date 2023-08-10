/* Trabalho 2 de Programação 2.
 * Victor Ribeiro Garcia */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "structs.h"
#include "render.h"
#include "game.h"

#define BACKGROUND_FILE "./resources/images/santos.jpeg"
#define MENU_FILE "./resources/images/menu.jpeg"
#define PAUSE_FILE "./resources/images/pause.png"
#define FIM_FILE "./resources/images/fimdejogo.jpeg"
#define PAUSE_MENU "./resources/images/pausemenu.jpeg"
#define SPEED_FILE "./resources/images/speed.jpeg"
#define COIN_FILE "./resources/images/santoscoin.jpeg"
#define INFO_FILE "./resources/images/info.jpeg"
#define LOJA_FILE "./resources/images/loja.jpeg"
#define BLOCK_FILE "./resources/images/bloqueado.png"
#define EASTER_FILE "./resources/images/pele.png"

#define MENU_SONG "./resources/sounds/hino.wav"

#define FONTE "./resources/font/8bit.TTF" 

#define largura 540
#define altura 720

int main(){
	const float FPS = 60.0;
	
	int i;
	int state = 0;
	int som = -1;
	int nbolas = 300;
	int best, coins;
	
	leScore(&best, &coins);

	c_t *color;
	color = malloc(sizeof(c_t));
	leCor(color);

	d_t *dados;
	dados = malloc(sizeof(d_t));
	
	bola *bolas;
	bolas = malloc(nbolas * sizeof(bola));

	q_t *quadrados;
	quadrados = malloc(sizeof(q_t));

	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse(); 
	al_install_audio();	
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();

	ALLEGRO_EVENT ev;

	ALLEGRO_BITMAP *background = al_load_bitmap(BACKGROUND_FILE);
	ALLEGRO_BITMAP *imgmenu = al_load_bitmap(MENU_FILE);
	ALLEGRO_BITMAP *pause = al_load_bitmap(PAUSE_FILE);
	ALLEGRO_BITMAP *pausemenu = al_load_bitmap(PAUSE_MENU);
	ALLEGRO_BITMAP *imgspeed = al_load_bitmap(SPEED_FILE);
	ALLEGRO_BITMAP *imgfim = al_load_bitmap(FIM_FILE);
	ALLEGRO_BITMAP *imgcoin = al_load_bitmap(COIN_FILE);
	ALLEGRO_BITMAP *imginfo = al_load_bitmap(INFO_FILE);
	ALLEGRO_BITMAP *imgloja = al_load_bitmap(LOJA_FILE);
	ALLEGRO_BITMAP *imgblock = al_load_bitmap(BLOCK_FILE);
	ALLEGRO_BITMAP *imgeaster = al_load_bitmap(EASTER_FILE);
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_FONT *font20 = al_load_font(FONTE,20,0);
	ALLEGRO_FONT *font32 = al_load_font(FONTE,32,0);
	ALLEGRO_FONT *font42 = al_load_font(FONTE,42,0);

	ALLEGRO_SAMPLE *songmenu = al_load_sample(MENU_SONG);
	al_reserve_samples(1);

	ALLEGRO_SAMPLE_INSTANCE *song = al_create_sample_instance(songmenu);
	al_set_sample_instance_playmode(song, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(song, al_get_default_mixer());

	display = al_create_display(largura,altura);
	event_queue = al_create_event_queue();

	if(!display)
		state = -1;

	al_register_event_source(event_queue,al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	while(state != -2){
		if(state == -1){
			dados = destroiDados(dados);
			bolas = destroiBola(bolas);
			quadrados = destroiQuadrado(quadrados);
			color = destroiCor(color);
			destroi(background, imgmenu, pause, pausemenu, imgfim, imgspeed, imgcoin, imginfo, imgloja, imgblock, imgeaster, timer, display, event_queue, font20, font32, font42, songmenu, song);
			state = -2;
		}

		if(state == 0){
			inicializaDados(dados);

			for(i = 0; i < nbolas; i++){
				inicializaBola(bolas, dados->centro_x, dados->centro_y, i);
			}

			inicializaQuadrados(quadrados);
			geraQuadrados(quadrados, dados);
			desceQuadrado(quadrados);
			state = menu(bolas, quadrados, dados, imgmenu, ev, event_queue, font42, song, &som, nbolas);
		}

		if(state == 1)
			state = jogo(bolas, color, quadrados, dados, background, pause, pausemenu, imgspeed, imgcoin, imgeaster, timer, ev, event_queue, font20, font32, song, &som, &best, &coins, &nbolas);

		if(state == 2)
			state = fimdejogo(bolas, quadrados, dados, imgfim, ev, event_queue, font20, font32, font42, song, &best, &coins, nbolas);

		if(state == 3)
			state = ajuda(bolas, quadrados, dados, imginfo, ev, event_queue);

		if(state == 4){
			state = loja(color, imgloja, imgblock, font20, ev, event_queue, &coins);
			registraScore(&best, &coins);
		}
       }		
	
	return 0;
}
