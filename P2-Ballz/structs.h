/* Trabalho 2 de Programação 2.
 * Victor Ribeiro Garcia */

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{
	float centro_x;
	float centro_y;
	float mov_x;
	float mov_y;
	float raio;
	bool nova;
} bola;

typedef struct{
	int c1;
	int c2;
	int c3;
} c_t;

typedef struct{
	float x1;
	float x2;
	float y1;
	float y2;
	int valor;
} b_t;

typedef struct{
	b_t bloco[12][9];
} q_t;

typedef struct{
	float pos_x;
        float pos_y;
        float ini_x;
        float ini_y;
        float centro_x;
        float centro_y;
        float mov_x;
        float mov_y;
        float hipot;
        float tam_x;
        float tam_y;
        float total_x;
        float total_y;
        float speed;

        bool lanca;
        bool done;
        bool redraw;

	int size;
        int cont;
        int tempo;
        int val;
} d_t;

void inicializaBola(bola *bolas, int cx, int cy, int i);
void inicializaDados(d_t *dados);
void inicializaQuadrados(q_t *quadrados);

d_t* destroiDados(d_t *dados);
bola* destroiBola(bola *bolas);
c_t* destroiCor(c_t *color);
q_t* destroiQuadrado(q_t *quadrados);

int aleat(int min, int max);
int geraValor(int val1, int val2);
void geraQuadrados(q_t *quadrados, d_t *dados);
void desceQuadrado(q_t *quadrados);

void leCor(c_t *color);
void registraCor(c_t *color, int c1, int c2, int c3);

void leScore(int *best, int *coins);
void registraScore(int *best, int *coins);

void destroi(ALLEGRO_BITMAP *background, ALLEGRO_BITMAP *imgmenu, ALLEGRO_BITMAP *pause, ALLEGRO_BITMAP *pausemenu, ALLEGRO_BITMAP *imgfim, ALLEGRO_BITMAP *imgspeed, ALLEGRO_BITMAP *imgcoin, ALLEGRO_BITMAP *imginfo, ALLEGRO_BITMAP *imgloja, ALLEGRO_BITMAP *imgblock, ALLEGRO_BITMAP *imgeaster, ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font20, ALLEGRO_FONT *font32, ALLEGRO_FONT *font42, ALLEGRO_SAMPLE *songmenu, ALLEGRO_SAMPLE_INSTANCE *song);

#endif
