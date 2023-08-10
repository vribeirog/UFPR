#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"

int main(){
        char op;
        int x, tam;
        Hash T1, T2;

	/* Vetor Auxiliar */
	int valores[2*m];

	/* Inicializando as tabelas. */
        inicializaHash(T1);
        inicializaHash(T2);

        scanf("%c %d",&op, &x);
        while(!feof(stdin)){
                if(op == 'i'){
                        Insere(T1,T2,x);
		}
                else if(op == 'r'){
                      Exclui(T1,T2,x);
                }
                scanf("%c %d",&op, &x);
        }
	/* Adiciona as chaves das tabelas no vetor. */
	tam = adicionaNoVetor(T1,T2,valores);

	/* Ordena de maneira crescente o vetor. */
	quicksort(valores,0,tam);

        Imprime(T1,T2,valores,tam);
	return 0;
}
