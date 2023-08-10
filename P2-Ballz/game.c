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

#define LINESIZE 1024

#define ITENS "./resources/data/loja.txt"

int jogo(bola *bolas, c_t *color, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *pausemenu, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_TIMER *timer, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int *best, int *coins, int *nbolas){

	int estado = 1;
	int i = 0;

	redesenha(background, pause, imgspeed, imgcoin, imgeaster, font32, font20, bolas, color, quadrados, dados, best, coins);

	al_start_timer(timer);
	while(dados->done == false){
	
		if(*som == 1)
                	al_play_sample_instance(song);

		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER)
                	dados->redraw = true;

		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			dados->done = true;
			return -1;
		}

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 20) && (ev.mouse.x <= 52) && (ev.mouse.y >= 20) && (ev.mouse.y <= 52)){
			estado = pausa(bolas, quadrados, dados, pausemenu, ev, event_queue, song, som, nbolas);
			if(estado != 1)
				return estado;
			else
				dados->redraw = true;
		}else if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 480) && (ev.mouse.x <= 546) && (ev.mouse.y >= 10) && (ev.mouse.y <= 56)&&(dados->lanca == false)){
			dados->speed = 1.5;
		}else if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (dados->centro_y == 590) && (dados->lanca == true)){
			dados->ini_x = ev.mouse.x;
			dados->ini_y = ev.mouse.y;
			al_wait_for_event(event_queue, &ev);
			while(ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_UP){
				al_wait_for_event(event_queue, &ev);
				if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){
					al_clear_to_color(al_map_rgb(255,255,255));
					al_draw_bitmap(background,237,615,0);
					al_draw_bitmap(pause,20,20,0);
					al_draw_textf(font32, al_map_rgb(0,0,0), 270, 20, 0, "%d",dados->cont);
					al_draw_textf(font20, al_map_rgb(0,0,0), 60, 610, 0, "best %d",*best);
					al_draw_textf(font20, al_map_rgb(0,0,0), 60, 640, 0, "coins %d",*coins);
					desenhaQuadrados(quadrados, font20, imgcoin);
					al_draw_rectangle(60,60,480,600,al_map_rgb(0,0,0),0);
					al_draw_textf(font20, al_map_rgb(0,0,0),dados->centro_x+15,dados->centro_y-20,0,"*%d",dados->size);
        				al_draw_filled_circle(dados->centro_x,dados->centro_y,10,al_map_rgb(color->c1,color->c2,color->c3));
					dados->total_x = 0;
					dados->total_y = 0;
					if(dados->ini_y < ev.mouse.y){
						dados->tam_x = (dados->ini_x - ev.mouse.x) / 5;
						dados->tam_y = (dados->ini_y - ev.mouse.y) / 5;
						for(i = 7; i >= 3; i--){
							dados->total_x += dados->tam_x;
							dados->total_y += dados->tam_y;
							al_draw_filled_circle(dados->centro_x + dados->total_x, dados->centro_y + dados->total_y, i, al_map_rgb(0,0,0));
						}
					}
        				al_flip_display();
					al_wait_for_event(event_queue, &ev);
				}
			}
			dados->pos_x = ev.mouse.x;
			dados->pos_y = ev.mouse.y;
			if(dados->ini_y < dados->pos_y){
                       		dados->mov_x = (dados->ini_x - dados->pos_x);
				dados->mov_y = (dados->ini_y - dados->pos_y);
				dados->hipot = sqrt((dados->mov_x * dados->mov_x)+(dados->mov_y * dados->mov_y));
				dados->mov_x = 10 * (dados->mov_x / dados->hipot);
				dados->mov_y = 10 * (dados->mov_y / dados->hipot);

				dados->centro_x = 0;
				dados->centro_y = 0;

				dados->lanca = false;
				dados->speed = 1;
				dados->tempo = 0;

				for(i = 0; i < dados->size; i++){
					bolas[i].mov_x = dados->mov_x;
					bolas[i].mov_y = dados->mov_y;
				}
			}
		}

		if((dados->redraw == true) && (al_is_event_queue_empty(event_queue))){

			dados->tempo++;
			
			for(int j = 0; j < dados->speed; j++){
				i = 0;
				while((i*4 < dados->tempo)&&(i < dados->size)){
					
					if(bolas[i].nova == false)
						colisãoQuadrados(quadrados, bolas, i, dados, coins);;
    
					if((dados->size+1) >= *nbolas){
						bolas = realloc(bolas, (2 * *nbolas * sizeof(bola)));
						*nbolas *= 2;
					}

					if((bolas[i].centro_x + bolas[i].mov_x) > 470){
						bolas[i].centro_x = 470;
						bolas[i].mov_x = -1*(bolas[i].mov_x);
					}

					if((bolas[i].centro_x + bolas[i].mov_x) < 70){
						bolas[i].centro_x = 70;
						bolas[i].mov_x = -1*(bolas[i].mov_x);
					}


					if((bolas[i].centro_y + bolas[i].mov_y) < 70){
						bolas[i].centro_y = 70;
						bolas[i].mov_y = -1*(bolas[i].mov_y);
					}

					if((bolas[i].centro_y + bolas[i].mov_y) > 590){
						bolas[i].centro_y = 590;
                                                bolas[i].mov_x = 0;
                                                bolas[i].mov_y = 0;
						dados->val++;

						if((bolas[i].nova == false) && (dados->centro_x == 0) && (dados->centro_y == 0)){
                                                        dados->centro_x = bolas[i].centro_x;
                                                        dados->centro_y = bolas[i].centro_y;
                                                }
						
						bolas[i].nova = false;
					}

					if((bolas[i].centro_y + bolas[i].mov_y) <= 590){
						bolas[i].centro_x += bolas[i].mov_x;
						bolas[i].centro_y += bolas[i].mov_y;
					}

					i++;
				}
			}

			if(dados->val == dados->size){
				dados->lanca = true;
				dados->cont++;
				dados->tempo = 0;
				dados->val = 0;
				geraQuadrados(quadrados, dados);
				desceQuadrado(quadrados);
				juntaBolas(bolas, color, quadrados, dados, background, pause, imgspeed, imgcoin, imgeaster, font32, font20, best, coins);
				if(verificaFim(quadrados))
                                        return 2;
			}

			redesenha(background, pause, imgspeed, imgcoin, imgeaster, font32, font20, bolas, color, quadrados, dados, best, coins);

			dados->redraw = false;
		}
	}

	return 1;
}

int menu(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imgmenu, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font52, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int nbolas){
        bool m = true;

	if(*som == -1){
		al_play_sample_instance(song);
		*som = 1;
	}
	
	al_draw_bitmap(imgmenu,0,0,0);
        al_draw_text(font52, al_map_rgb(255,255,255), 270, 470, ALLEGRO_ALIGN_CENTER, "Press Start");
        al_flip_display();

        while(m){
                al_wait_for_event(event_queue, &ev);

		//SAI DO JOGO
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return -1;

		//MUTA E DESMUTA
		else if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 85) && (ev.mouse.x <= 170) && (ev.mouse.y >= 608) && (ev.mouse.y <= 688)){
                        if(*som == 1){
                                al_set_sample_instance_playing(song, false);
                        	*som = 0;
                        }else if(*som == 0){
                                al_set_sample_instance_playing(song, true);
                                *som = 1;
                        }
		}
		
		//AJUDA
		else if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 390) && (ev.mouse.x <= 470) && (ev.mouse.y >= 612) && (ev.mouse.y <= 690))
			return 3;
		
		//LOJA
		else if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 236) && (ev.mouse.x <= 321) && (ev.mouse.y >= 612) && (ev.mouse.y <= 690))
			return 4;

		//ENTRA NO JOGO
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			return 1;
	}

	return 0;
}

int pausa(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *pausemenu, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int *nbolas){
	bool pausado = true;
	int i;

	al_draw_bitmap(pausemenu,0,0,0);
	al_flip_display();

	while(pausado){
		al_wait_for_event(event_queue, &ev);

		//MUTA E DESMUTA
                if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 227) && (ev.mouse.x <= 314) && (ev.mouse.y >= 595) && (ev.mouse.y <= 675)){
                        if(*som == 1){
                                al_set_sample_instance_playing(song, false);
                                *som = 0;
                        }else if(*som == 0){
                                al_set_sample_instance_playing(song, true);
                                *som = 1;
                        }
                }

		//SAI DO JOGO
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return -1;

		//CONTINUE
                if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 120) && (ev.mouse.x <= 430) && (ev.mouse.y >= 273) && (ev.mouse.y <= 347))
                        return 1;

		//RESTART
                if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 120) && (ev.mouse.x <= 430) && (ev.mouse.y >= 375) && (ev.mouse.y <= 450)){		
			inicializaDados(dados);

        		for(i = 0; i < *nbolas; i++){
                		inicializaBola(bolas, dados->centro_x, dados->centro_y, i);
        		}

			inicializaQuadrados(quadrados);
			geraQuadrados(quadrados, dados);		
			desceQuadrado(quadrados);
			
                        return 1;
                }

		//VOLTA AO MENU
		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 120) && (ev.mouse.x <= 430) && (ev.mouse.y >= 477) && (ev.mouse.y <= 550))
			return 0;
	}

	return 1;
}

void desenhaLoja(ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_FONT *font20, int c1, int c2, int c3, int c4, int c5, int c6, int *coins){
	al_draw_bitmap(imgloja,0,0,0);
        al_draw_textf(font20, al_map_rgb(255,255,255),450,15,ALLEGRO_ALIGN_CENTER,"COINS %d", *coins);

        al_draw_filled_circle(153,205,20,al_map_rgb(0,0,0));    //preto
        if(c1 == 0)
                al_draw_bitmap(imgblock,92,145,0);

        al_draw_filled_circle(153,382,20,al_map_rgb(255,0,0));  //vermelho
        if(c2 == 0)
                al_draw_bitmap(imgblock,92,320,0);

        al_draw_filled_circle(153,559,20,al_map_rgb(0,255,255)); //azul
        if(c3 == 0)
                al_draw_bitmap(imgblock,92,498,0);

        al_draw_filled_circle(394,203,20,al_map_rgb(203,63,191)); //rosa
        if(c4 == 0)
                al_draw_bitmap(imgblock,334,143,0);

        al_draw_filled_circle(398,383,20,al_map_rgb(255,233,35)); //amarelo
        if(c5 == 0)
                al_draw_bitmap(imgblock,338,323,0);

        al_draw_filled_circle(402,552,20,al_map_rgb(96,48,129)); //roxo
        if(c6 == 0)
                al_draw_bitmap(imgblock,342,492,0);

        al_flip_display();
}

void registraLoja(int c1, int c2, int c3, int c4, int c5, int c6){
	FILE *arq;
        arq = fopen(ITENS,"w+");

        if(!arq){
                perror("Erro ao abrir arquivo.");
                exit(1);
        }
	
	fprintf(arq,"%d",c1);
        fprintf(arq,"\n");
	fprintf(arq,"%d",c2);
        fprintf(arq,"\n");
	fprintf(arq,"%d",c3);
        fprintf(arq,"\n");
	fprintf(arq,"%d",c4);
        fprintf(arq,"\n");
	fprintf(arq,"%d",c5);
        fprintf(arq,"\n");
	fprintf(arq,"%d",c6);

	fclose(arq);
}

int loja(c_t *color, ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_FONT *font20, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, int *coins){
	bool l = true;
	int c1, c2, c3, c4, c5, c6;
	char aux[LINESIZE+1];

	FILE *arq;
        arq = fopen(ITENS,"r");

        if(!arq){
                perror("Erro ao abrir arquivo.");
                exit(1);
        }

        fgets(aux,LINESIZE,arq);
	c1 = atoi(aux);
        fgets(aux,LINESIZE,arq);
        c2 = atoi(aux);
	fgets(aux,LINESIZE,arq);
        c3 = atoi(aux);
	fgets(aux,LINESIZE,arq);
        c4 = atoi(aux);
	fgets(aux,LINESIZE,arq);
        c5 = atoi(aux);
	fgets(aux,LINESIZE,arq);
        c6 = atoi(aux);

        fclose(arq);

	desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);

	while(l){
		al_wait_for_event(event_queue, &ev);

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 93) && (ev.mouse.x <= 213) && (ev.mouse.y >= 145) && (ev.mouse.y <= 265)){
			if(c1 == 1){
				registraCor(color,0,0,0);
				leCor(color);
			}else if((c1 == 0)&&(*coins >= 100)){
				*coins = *coins - 100;
				c1 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
			}		
		}

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 93) && (ev.mouse.x <= 213) && (ev.mouse.y >= 322) && (ev.mouse.y <= 442)){
                        if(c2 == 1){
                                registraCor(color,255,0,0);
                                leCor(color);
                        }else if((c2 == 0)&&(*coins >= 100)){
                                *coins = *coins - 100;
				c2 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
                        }
                }

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 93) && (ev.mouse.x <= 213) && (ev.mouse.y >= 499) && (ev.mouse.y <= 619)){
			if(c3 == 1){
				registraCor(color,0,255,255);
				leCor(color);
			}else if((c3 == 0)&&(*coins >= 100)){
				*coins = *coins - 100;
				c3 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
			}
		}

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 334) && (ev.mouse.x <= 454) && (ev.mouse.y >= 143) && (ev.mouse.y <= 263)){
			if(c4 == 1){
				registraCor(color,203,63,191);
				leCor(color);
			}else if((c4 == 0)&&(*coins >= 100)){
				*coins = *coins - 100;
				c4 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
			}
		}

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 338) && (ev.mouse.x <= 458) && (ev.mouse.y >= 323) && (ev.mouse.y <= 443)){
			if(c5 == 1){
				registraCor(color,255,233,35);
				leCor(color);
			}else if((c5 == 0)&&(*coins >= 100)){
				*coins = *coins - 100;
				c5 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
			}
		}

		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 342) && (ev.mouse.x <= 462) && (ev.mouse.y >= 492) && (ev.mouse.y <= 612)){
			if(c6 == 1){
				registraCor(color,96,48,129);
				leCor(color);
			}else if((c6 == 0)&&(*coins >= 100)){
				*coins = *coins - 100;
				c6 = 1;
				registraLoja(c1, c2, c3, c4, c5, c6);
				desenhaLoja(imgloja, imgblock, font20, c1, c2, c3, c4, c5, c6, coins);
			}
		}

		//SAI DO JOGO
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return -1;

		//RETORNA AO MENU
		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 4) && (ev.mouse.x <= 60) && (ev.mouse.y >= 10) && (ev.mouse.y <= 45))
			return 0;
	}

	return 4;
}

int ajuda(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imginfo, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue){
	bool help = true;

	al_draw_bitmap(imginfo,0,0,0);
	al_flip_display();

	while(help){
		al_wait_for_event(event_queue, &ev);

		//SAI DO JOGO
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return -1;

		//RETORNA AO MENU
		if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 4) && (ev.mouse.x <= 60) && (ev.mouse.y >= 10) && (ev.mouse.y <= 45))
			return 0;
	}

	return 3;
}

bool verificaFim(q_t *quadrados){
	for(int j = 1; j < 8; j++){
		if(quadrados->bloco[9][j].valor > 0)
			return true;
		else if(quadrados->bloco[9][j].valor < 0)
			quadrados->bloco[9][j].valor = 0;
	}
	return false;
}	

int fimdejogo(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imgfim, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_FONT *font52, ALLEGRO_SAMPLE_INSTANCE *song, int *best, int *coins, int nbolas){
	
	bool fim = true;
	int i;
	al_draw_bitmap(imgfim,0,0,0);
	al_draw_textf(font20, al_map_rgb(255,255,255), 450, 20, ALLEGRO_ALIGN_CENTER, "coins %d", *coins);
	al_draw_textf(font32, al_map_rgb(255,255,255), 270, 80, ALLEGRO_ALIGN_CENTER, "best %d", *best);
	al_draw_text(font52, al_map_rgb(255,255,255), 270, 150, ALLEGRO_ALIGN_CENTER, "SCORE");
	al_draw_textf(font52, al_map_rgb(255,255,255), 270, 210, ALLEGRO_ALIGN_CENTER, "%d", dados->cont);
	al_flip_display();
	
	if(dados->cont > *best)
                *best = dados->cont;

	registraScore(best, coins);

	while(fim){
                al_wait_for_event(event_queue, &ev);
		
                //SAI DO JOGO
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                        return -1;

                //RESTART
                if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 105) && (ev.mouse.x <= 453) && (ev.mouse.y >= 320) && (ev.mouse.y <= 400)){
			        inicializaDados(dados);
			
			        for(i = 0; i < nbolas; i++){
               			inicializaBola(bolas, dados->centro_x, dados->centro_y, i);
        		    }

			        inicializaQuadrados(quadrados);
			        geraQuadrados(quadrados, dados);
			        desceQuadrado(quadrados);

                    return 1;
                }

                //VOLTA AO MENU
                if((ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) && (ev.mouse.x >= 105) && (ev.mouse.x <= 453) && (ev.mouse.y >= 450) && (ev.mouse.y <= 525))
			        return 0;
	}

        return 2;
}
