// Biblioteca para informações dos logs e manipulação deles

#ifndef __BIKE__
#define __BIKE__

#define LINESIZE 1024

typedef struct infos{
	char log_name[20]; // nome do .log
	char bike_name[30]; // nome da bicicleta usada na atividade
	int data_dia;
	int data_mes;
	int data_ano;
	float dist; // distância percorrida
	float elev; // elevação acumulada
	float elev_atual; //elevação atual
	float elev_anterior; // elevação anterior
	double anterior_speed; // velocidade anterior
	double atual_speed; // velocidade atual
	double media_speed; // velocidade média
	float max_speed; // velocidade máxima
	int tempo_speed; // tempo das velocidades
	double anterior_hr; //heart rate anterior
	double atual_hr; // heart rate atual
	double media_hr; // heart rate médio
	int max_hr; // heart rate máximo
	int tempo_hr; // tempo do heart rate
	double anterior_cad; // cadencia anterior
	double atual_cad;
	double media_cad; // cadência média
	int tempo_cad; // tempo das cadências
	int tempo; // tempo da ativadade em segundos
	int tempo_atual; // tempo da atividade atual
	int tempo_anterior; // tempo da atividade anterior
} infos;

int contaDir(char *nomeDir);
int leDir(char *nomeDir, infos *reg);
int leArq(char *path, infos *reg, int n);
int leHeader(char *line, infos *reg, int n);
void converteUnidades(infos *reg, int n);
void calculaMedia(infos *reg, int n, int tempo);
void remove_palavra(char *frase, char *palavra);
void calculaData(infos *reg, int n, char *line);
int converteSegundos(char *token);
void inicializaInfo(infos *reg, int n);
int comparbike(const void *a, const void *b);
int compardata(const void *a, const void *b);
int comparalt(const void *a, const void *b);
int compardist(const void *a, const void *b);
void imprimeResumo(infos *reg, int nlog, char *nomeBike);
void imprimeInfo(infos *reg, int n);
void imprimeNomes(infos *reg, int nlog, char *nomeBike);
void imprimeBike(infos *reg, int nlog, char *nomeBike);
void bikeData(infos *reg, int nlog, char *nomeBike);
void bikeDist(infos *reg, int nlog, char *nomeBike);
void subAcumulada(infos *reg, int nlog);
void imprimeHistograma(infos *reg, int nlog, char *nomeBike);

#endif
