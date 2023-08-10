/* Trabalho 2 de Programação 2.
 * Victor Ribeiro Garcia */

#ifndef GAME_H
#define GAME_H

int jogo(bola *bolas, c_t *color, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *pausemenu, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_TIMER *timer, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int *best, int *coins, int *nbolas);

int menu(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imgmenu, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font52, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int nbolas);

int pausa(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *pausemenu, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_SAMPLE_INSTANCE *song, int *som, int *nbolas);

void desenhaLoja(ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_FONT *font20, int c1, int c2, int c3, int c4, int c5, int c6, int *coins);
void registraLoja(int c1, int c2, int c3, int c4, int c5, int c6);
int loja(c_t *color, ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_FONT *font20, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, int *coins);

int ajuda(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imginfo, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue);

bool verificaFim(q_t *quadrados);
int fimdejogo(bola *bolas, q_t *quadrados, d_t *dados, ALLEGRO_BITMAP *imgfim, ALLEGRO_EVENT ev, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_FONT *font52, ALLEGRO_SAMPLE_INSTANCE *song, int *best, int *coins, int nbolas);

#endif
