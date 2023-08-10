/* Trabalho 2 de Programação 2.
 * Victor Ribeiro Garcia */

#ifndef RENDER_H
#define RENDER_H

void desenhaQuadrados(q_t *quadrados, ALLEGRO_FONT *font20, ALLEGRO_BITMAP *imgcoin);
void colisãoQuadrados(q_t *quadrados, bola *bolas, int i, d_t *dados, int *coins);
void redesenha(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_FONT *font1, ALLEGRO_FONT *font2, bola * bolas, c_t *color, q_t *quadrados, d_t *dados, int *best, int *coins);
void juntaBolas(bola * bolas, c_t *color, q_t *quadrados, d_t * dados, ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imgeaster, ALLEGRO_FONT *font32, ALLEGRO_FONT *font20, int *best, int *coins);

#endif
