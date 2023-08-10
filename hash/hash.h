#ifndef HASH_H
#define HASH_H

#define m 11 /*tamanho da tabela Hash*/

typedef struct{
	int chave;
	int excluido;
} slot;

typedef slot Hash[m];

void inicializaHash(Hash tabela);

int h1(int chave);

int h2(int chave);

int Busca(Hash T1, Hash T2, int x);

void Insere(Hash T1, Hash T2, int x);

void Exclui(Hash T1, Hash T2, int x);

int adicionaNoVetor(Hash T1, Hash T2, int v[2*m]);
		
void quicksort(int a[m], int left, int right);

void Imprime(Hash T1, Hash T2, int *a, int tam);

#endif
