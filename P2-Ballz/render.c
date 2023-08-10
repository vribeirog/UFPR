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

void desenhaQuadrados(q_t *quadrados, ALLEGRO_FONT *font20, ALLEGRO_BITMAP *imgcoin){
	for(int i = 1; i < 12; i++){
		for(int j = 1; j < 9; j++){
			if(quadrados->bloco[i][j].valor > 0){
				al_draw_filled_rectangle(quadrados->bloco[i][j].x1, quadrados->bloco[i][j].y1, quadrados->bloco[i][j].x2, quadrados->bloco[i][j].y2, al_map_rgb((quadrados->bloco[i][j].valor * 20),(quadrados->bloco[i][j].valor * 10),255 - (quadrados->bloco[i][j].valor * 50)));
				al_draw_rectangle(quadrados->bloco[i][j].x1, quadrados->bloco[i][j].y1, quadrados->bloco[i][j].x2, quadrados->bloco[i][j].y2,al_map_rgb(255,255,255),3);
				al_draw_textf(font20, al_map_rgb(255,255,255), ((quadrados->bloco[i][j].x2-quadrados->bloco[i][j].x1)/2)+quadrados->bloco[i][j].x1, quadrados->bloco[i][j].y1+18, ALLEGRO_ALIGN_CENTER, "%d",quadrados->bloco[i][j].valor);
			}

			if(quadrados->bloco[i][j].valor == -1){
				al_draw_filled_circle(quadrados->bloco[i][j].x1+30,quadrados->bloco[i][j].y1+30,10,al_map_rgb(50,255,255));
				al_draw_circle(quadrados->bloco[i][j].x1+30,quadrados->bloco[i][j].y1+30,20,al_map_rgb(204,204,0),3);
			}
			
			if(quadrados->bloco[i][j].valor == -2)
				al_draw_bitmap(imgcoin,quadrados->bloco[i][j].x1,quadrados->bloco[i][j].y1,0);
		}
	}
}

void colisãoQuadrados(q_t *quadrados, bola *bolas, int i, d_t *dados, int *coins){
	int x, y, c, l;

	if(bolas[i].mov_x >= 0)
		x = 1;
	else
		x = -1;

	if(bolas[i].mov_y >= 0)
		y = 1;
	else
		y = -1;

	c = (bolas[i].centro_x + (bolas[i].raio * x)) / 60;
	l = (bolas[i].centro_y + (bolas[i].raio * y)) / 60;

	if(quadrados->bloco[l][c+x].valor > 0){
		if(floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x){
			bolas[i].mov_x = -1*(bolas[i].mov_x);
			quadrados->bloco[l][c+x].valor--;
		}
	}

	if(quadrados->bloco[l+y][c].valor > 0){
		if(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y){
			bolas[i].mov_y = -1*(bolas[i].mov_y);
			quadrados->bloco[l+y][c].valor--;
		}
	}
	
	if(quadrados->bloco[l+y][c+x].valor > 0){
		if((floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x)&&(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y)){
			bolas[i].mov_x = -1*(bolas[i].mov_x);
			bolas[i].mov_y = -1*(bolas[i].mov_y);
			quadrados->bloco[l+y][c+x].valor--;
		}
	}

	if(quadrados->bloco[l][c+x].valor == -1){
                if(floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x){
			inicializaBola(bolas, quadrados->bloco[l][c+x].x1+30,quadrados->bloco[l][c+x].y1+30, dados->size);
			bolas[dados->size].nova = true;
			bolas[dados->size].mov_y = 10;
			dados->size++;
			quadrados->bloco[l][c+x].valor = 0;
                }
        }

        if(quadrados->bloco[l+y][c].valor == -1){
                if(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y){
                        inicializaBola(bolas, quadrados->bloco[l+y][c].x1+30,quadrados->bloco[l+y][c].y1+30, dados->size);
			bolas[dados->size].nova = true;
			bolas[dados->size].mov_y = 10;
			dados->size++;
                        quadrados->bloco[l+y][c].valor = 0;
                }
        }

        if(quadrados->bloco[l+y][c+x].valor == -1){
                if((floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x)&&(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y)){
			inicializaBola(bolas, quadrados->bloco[l+y][c+x].x1+30,quadrados->bloco[l+y][c+x].y1+30, dados->size);
			bolas[dados->size].nova = true;
			bolas[dados->size].mov_y = 10;
			dados->size++;
                        quadrados->bloco[l+y][c+x].valor = 0;
                }
	}

	if(quadrados->bloco[l][c+x].valor == -2){
                if(floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x){
			*coins = *coins + 1;
			quadrados->bloco[l][c+x].valor = 0;
                }
        }

        if(quadrados->bloco[l+y][c].valor == -2){
                if(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y){
			*coins = *coins + 1;
			quadrados->bloco[l+y][c].valor = 0;		
                }
        }

        if(quadrados->bloco[l+y][c+x].valor == -2){
                if((floor((bolas[i].centro_x + bolas[i].mov_x + (bolas[i].raio * x)) / 60) == c + x)&&(floor((bolas[i].centro_y + bolas[i].mov_y + (bolas[i].raio * y)) / 60) == l + y)){
			*coins = *coins + 1;
	 		quadrados->bloco[l+y][c+x].valor = 0;		
                }
        }
}

void redesenha(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_FONT *font1, ALLEGRO_FONT *font2, bola * bolas, c_t *color, q_t *quadrados, d_t *dados, int *best, int *coins){
	int cont = 0;

	al_clear_to_color(al_map_rgb(255,255,255));
	al_draw_bitmap(background,237,615,0);
	al_draw_bitmap(pause,20,20,0);
	al_draw_textf(font1, al_map_rgb(0,0,0), 270, 20, 0, "%d",dados->cont);
	al_draw_textf(font2, al_map_rgb(0,0,0), 60, 610, 0, "best %d",*best);
	al_draw_textf(font2, al_map_rgb(0,0,0), 60, 640, 0, "coins %d",*coins);
	desenhaQuadrados(quadrados, font2, imgcoin);
	al_draw_rectangle(60,60,480,600,al_map_rgb(0,0,0),0);
	if((dados->tempo >= 300) && (dados->lanca == false))
		al_draw_bitmap(imgspeed,480,10,0);
	if(dados->centro_y == 590){
		for(int i = 0; i < dados->size; i++){
			if(bolas[i].centro_y == 590)
				cont++;
		}
		if(cont == dados->size)
        		al_draw_textf(font2, al_map_rgb(0,0,0),dados->centro_x + 15,dados->centro_y - 20,0,"*%d",dados->size);
	}
	for(int i = 0; i < dados->size; i++){
		if(bolas[i].nova == true)
			al_draw_filled_circle(bolas[i].centro_x,bolas[i].centro_y,bolas[i].raio,al_map_rgb(50,255,255));
		else	
			al_draw_filled_circle(bolas[i].centro_x,bolas[i].centro_y,bolas[i].raio,al_map_rgb(color->c1,color->c2,color->c3));
	}

	//EASTER EGG
	if((dados->tempo >= 1000) && (dados->lanca == true))
		al_draw_bitmap(imgeaster,400,600,0);	

	al_flip_display();
}

void juntaBolas(bola * bolas, c_t *color, q_t *quadrados, d_t * dados, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_FONT *font32, ALLEGRO_FONT *font20, int *best, int *coins){
	int cont = 0;
	int i;

	while(cont < 60){
		for(i = 0; i < dados->size; i++){
			bolas[i].centro_x += (dados->centro_x - bolas[i].centro_x) / 20;
			bolas[i].centro_y += (dados->centro_y - bolas[i].centro_y) / 20;
		}

		cont++;
		redesenha(background, pause, imgspeed, imgcoin, imgeaster, font32, font20, bolas, color, quadrados, dados, best, coins);
	}

	for(i = 0; i < dados->size; i++){
		bolas[i].centro_x = dados->centro_x;
		bolas[i].centro_y = dados->centro_y;
	}

	redesenha(background, pause, imgspeed, imgcoin, imgeaster, font32, font20, bolas, color, quadrados, dados, best, coins);
}
