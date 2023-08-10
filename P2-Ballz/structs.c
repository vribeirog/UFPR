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

#define LINESIZE 1024

#define SCORE "./resources/data/score.txt"
#define COR "./resources/data/color.txt"

#define largura 540
#define altura 720

void inicializaBola(bola *bolas, int cx, int cy, int i){
	bolas[i].centro_x = cx;
	bolas[i].centro_y = cy;
	bolas[i].mov_x = 0;
	bolas[i].mov_y = 0;
	bolas[i].raio = 10;
	bolas[i].nova = false;
}

void inicializaDados(d_t *dados){
        dados->pos_x = largura / 2;
        dados->pos_y = altura / 2;
        dados->ini_x = 0;
        dados->ini_y = 0;
        dados->centro_x = 270;
        dados->centro_y = 590;
        dados->mov_x = 0;
        dados->mov_y = 0;
        dados->hipot = 0;
        dados->tam_x = 0;
        dados->tam_y = 0;
        dados->total_x = 0;
        dados->total_y = 0;
        dados->speed = 1;

        dados->lanca = true;
        dados->done = false;
        dados->redraw = true;

	dados->size = 1;
        dados->cont = 1;
        dados->tempo = 0;
        dados->val = 0;
}

void inicializaQuadrados(q_t *quadrados){
	int x1 = 0;
        int x2 = 60;
        int y1 = 0;
        int y2 = 60;

        for(int i = 0; i < 12; i++){
                x1 = 0;
                x2 = 60;
                for(int j = 0; j < 9; j++){
			        quadrados->bloco[i][j].valor = 0;
			        quadrados->bloco[i][j].x1 = x1;
			        quadrados->bloco[i][j].x2 = x2;
			        quadrados->bloco[i][j].y1 = y1;
			        quadrados->bloco[i][j].y2 = y2;
                        x1 += 60;
                        x2 += 60;
                }
                y1 += 60;
                y2 += 60;
        }
}

d_t* destroiDados(d_t *dados){
	free(dados);
	return NULL;
}

bola* destroiBola(bola *bolas){
	free(bolas);
	return NULL;
}

c_t* destroiCor(c_t *color){
	free(color);
	return NULL;
}

q_t* destroiQuadrado(q_t *quadrados){
	free(quadrados);
	return NULL;
}

int aleat(int min, int max){
        int n;
        
        n = (rand() % (max-min+1))+min;
        return n;
}

int geraValor(int val1, int val2){
	int valor, num;
	
	num = (rand() % 10) + 1;

	if(num <= 2)
		valor = val2;
	else
		valor = val1;

	return valor;
}

void geraQuadrados(q_t *quadrados, d_t *dados){
	int pos, num, aux;
	int coin = 1;	

	srand((unsigned)time(NULL));

	if(dados->cont > 1){
		pos = aleat(1,7);
		quadrados->bloco[1][pos].valor = -1;   
	}

	num = aleat(3,5);

	while(num != 0){    
		pos = aleat(1,7);

		if(quadrados->bloco[1][pos].valor == 0){
			aux = aleat(1,10);    
            
			if(aux >= 6){
				quadrados->bloco[1][pos].valor = dados->cont;
				num--;
			}

			if((aux > 2)&&(aux <= 5)){
				quadrados->bloco[1][pos].valor = 2*(dados->cont);
				num--;
			}

			if((dados->cont > 1)&&(aux <= 2)&&(coin == 1)){
				quadrados->bloco[1][pos].valor = -2;
				coin--;
				num--;
			}
		}		
	}
}

void desceQuadrado(q_t *quadrados){
	for(int i = 8; i >= 1; i--){
		for(int j = 1; j < 8; j++){
			quadrados->bloco[i+1][j].valor = quadrados->bloco[i][j].valor;
			quadrados->bloco[i][j].valor = 0;
		}
	}
}

void leCor(c_t *color){
        char cor1[LINESIZE+1];
	char cor2[LINESIZE+1];
	char cor3[LINESIZE+1];
	
	FILE *arq;
	arq = fopen(COR,"r");

	if(!arq){
		perror("Erro ao abrir arquivo.");
		exit(1);
	}

	fgets(cor1,LINESIZE,arq);
        fgets(cor2,LINESIZE,arq);
	fgets(cor3,LINESIZE,arq);
        fclose(arq);

	color->c1 = atoi(cor1);
	color->c2 = atoi(cor2);
	color->c3 = atoi(cor3);
}

void registraCor(c_t *color, int c1, int c2, int c3){
        FILE *arq;
        arq = fopen(COR,"w+");

        if(!arq){
                perror("Erro ao abrir arquivo.");
                exit(1);
        }

        fprintf(arq,"%d",c1);
        fprintf(arq,"\n");
        fprintf(arq,"%d",c2);
	fprintf(arq,"\n");
	fprintf(arq,"%d",c3);

        fclose(arq);
}

void leScore(int *best, int *coins){
	char melhor[LINESIZE+1];
        char moeda[LINESIZE+1];

	FILE *arq;
        arq = fopen(SCORE,"r");

        if(!arq){
                perror("Erro ao abrir arquivo.");
                exit(1);
        }

        fgets(melhor,LINESIZE,arq);
        fgets(moeda,LINESIZE,arq);
        fclose(arq);

        *best = atoi(melhor);
        *coins = atoi(moeda);
}

void registraScore(int *best, int *coins){
	FILE *arq;
	arq = fopen(SCORE,"w+");

        if(!arq){
                perror("Erro ao abrir arquivo.");
                exit(1);
        }

        fprintf(arq,"%d",*best);
        fprintf(arq,"\n");
        fprintf(arq,"%d",*coins);

        fclose(arq);
}

void destroi(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *imgmenu, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *pausemenu, ALLEGRO_BITMAP *imgfim, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imginfo, ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_BITMAP *imgeaster, ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_FONT *font42, ALLEGRO_SAMPLE *songmenu, ALLEGRO_SAMPLE_INSTANCE *song){
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_bitmap(background);
	al_destroy_bitmap(imgmenu);
	al_destroy_bitmap(pause);
	al_destroy_bitmap(pausemenu);
	al_destroy_bitmap(imgfim);
	al_destroy_bitmap(imgspeed);
        al_destroy_bitmap(imgcoin);
        al_destroy_bitmap(imginfo);
	al_destroy_bitmap(imgloja);
    al_destroy_bitmap(imgblock);
    al_destroy_bitmap(imgeaster);
        al_destroy_font(font20);
        al_destroy_font(font32);
        al_destroy_font(font42);
        al_destroy_event_queue(event_queue);
        al_destroy_sample(songmenu);
	al_destroy_sample_instance(song);
}
