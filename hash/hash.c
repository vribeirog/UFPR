#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"

/* Inicializa uma Hash com as chaves = -1 e excluido = 0. */
void inicializaHash(Hash tabela){
	int i;
	for(i=0;i<m;i++){
		tabela[i].chave = -1;
		tabela[i].excluido = 0;
	}
}

/*Função 1.*/
int h1(int chave){
        return (chave % m);
}

/*Função 2.*/
int h2(int chave){
        int resultado, aux1, aux2, aux3;
        aux1 = chave*0.9;
        aux2 = floor(aux1);
        aux3 = m*(chave*0.9-aux2);
        resultado = floor(aux3);
        return resultado;
}

/* Busca o valor nas 2 tabelas Hash, retorna a posição na tabela se encontrar
 * em alguma delas e retorna -1 se não encontra. */
int Busca(Hash T1, Hash T2, int x){
        int f1, f2;
        f1 = h1(x);
        f2 = h2(x);
        if((T1[f1].chave == -1) && (T1[f1].excluido == 0))
                return -1;
	if(T1[f1].chave == x)
		return f1;
        if(T2[f2].chave == x)
                return f2;
	return -1;
}

/* Insere a chave X na Hash T1, porém se já tiver outra chave J em T1, passa o J
 * para T2 e insere X em T1. */
void Insere(Hash T1, Hash T2, int x){
        int f1, f2;
        f1 = h1(x);
	f2 = h2(x);
        if((T1[f1].chave == -1)||(T1[f1].excluido == 1)){
		T1[f1].chave = x;
		T1[f1].excluido = 0;
        }
        else if(T2[f2].chave == -1){
                f2 = h2(T1[f1].chave);
                T2[f2].chave = T1[f1].chave;
                T1[f1].chave = x;
        }
}

/* Se a chave X estiver em T2, apenas remove. Se X tiver em T1, marca a posição
 * como removida e remove a chave X. */
void Exclui(Hash T1, Hash T2, int x){
        int pos;
	pos = Busca(T1,T2,x);
        if(T2[pos].chave == x)
                T2[pos].chave = -1;
	else if(T1[pos].chave == x){
                T1[pos].excluido = 1;
                T1[pos].chave = -1;
        }
}

/* FUNÇÃO AUXILIAR: insere a chave das tabelas em um vetor. */
int adicionaNoVetor(Hash T1, Hash T2, int v[2*m]){
	int i;
	int j = 0;
	for(i=0;i<m;i++){
		if(T1[i].chave != -1){
			v[j] = T1[i].chave;
			j++;
		}
	}
	for(i=0;i<m;i++){
                if(T2[i].chave != -1){
                        v[j] = T2[i].chave;
			j++;
		}
        }
	return (j-1);
}

/* FUNÇÃO AUXILIAR: ordena o vetor que contém as chaves das tabelas. */
void quicksort(int a[m], int left, int right){
	int i,j,x,y;

	i = left;
	j = right;
	x = a[(i + j)/2];

	while(i<=j){
		while((a[i] < x) && (i < right)){
			i++;
		}
		while((a[j] > x) && (j > left)){
			j--;
		}
		if(i <= j){
			y = a[i];
			a[i] = a[j];
			a[j] = y;
			i++;
			j--;
		}
	}
	if(j > left){
		quicksort(a,left,j);
	}
	if(i < right){
		quicksort(a,i,right);
	}
}

/* Imprime as chaves crescentes: CHAVE, TABELA, POSIÇÃO. */
void Imprime(Hash T1, Hash T2, int *a, int tam){
        int i,pos;
        for(i=0;i<=tam;i++){
		pos = Busca(T1,T2,a[i]);
		if((pos != -1)&&(T1[pos].chave == a[i]))
			printf("%d,T1,%d\n",a[i],pos);
		if((pos != -1)&&(T2[pos].chave == a[i]))
			printf("%d,T2,%d\n",a[i],pos);
	}
}

