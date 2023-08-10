#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct no{
	int chave;
	struct no *esq, *dir;
	int altura;
	int nivel;
} No;

int max(int a, int b){
	return (a>b) ? a : b;
}

int altura(No* no){
	if(no == NULL)
		return 0;
	else
		return 1 + max(altura(no->esq),altura(no->dir));
}

int fatorBalanceamento(No* no){
	if(no == NULL)
		return 0;
	return altura(no->esq) - altura(no->dir);
}

void emOrdem(No* no){
	if(no==NULL)
		return;
	emOrdem(no->esq);
	printf("%d,%d\n", no->chave,no->nivel);
	emOrdem(no->dir);
}

No *DirRot(No *y){
	No *x = y->esq;
	No *rot = x->dir;

	x->dir = y;
	y->esq = rot;

	y->altura = max(altura(y->esq),altura(y->dir))+1;
	x->altura = max(altura(x->esq),altura(x->dir))+1;

	return x;
}

No *EsqRot(No *x){
	No *y = x->dir;
	No *rot = y->esq;

	y->esq = x;
	x->dir = rot;

	x->altura = max(altura(x->esq),altura(x->dir))+1;
	y->altura = max(altura(y->esq),altura(y->dir))+1;

	return y;
}

No* NovoNo(int x){
	No *no = malloc(sizeof(No));
	no->chave = x;
	no->esq = NULL;
	no->dir = NULL;
	no->altura = 0;
	no->nivel = 0;
	return no;
}

No *insere(No* no, int x){
	if(no == NULL)
		return NovoNo(x);
	if(x < no->chave)
		no->esq = insere(no->esq,x);
	else if(x > no->chave)
		no->dir = insere(no->dir,x);
	else
		return no;

	int balanceamento = fatorBalanceamento(no);

	if((balanceamento > 1) && (x < no->esq->chave))
		return DirRot(no);
	if((balanceamento < -1) && (x > no->dir->chave))
		return EsqRot(no);
	if((balanceamento > 1) && (x > no->esq->chave)){
		no->esq = EsqRot(no->esq);
		return DirRot(no);
	}
	if((balanceamento < -1) && (x < no->dir->chave)){
		no->dir = DirRot(no->dir);
		return EsqRot(no);
	}

	return no;
}

No *minimo(No* no){
	No *menor = no;

	while (menor->esq != NULL)
		menor = menor->esq;

	return menor;
}

No *exclui(No* no, int x){
	if(no == NULL)
		return no;

	if(x < no->chave)
		no->esq = exclui(no->esq,x);
	else if (x > no->chave)
		no->dir = exclui(no->dir,x);
	else{
		if((no->esq==NULL) || (no->dir==NULL)){
			No *temp = no->esq ? no->esq : no->dir;
			if(temp == NULL){
				temp = no;
				no = NULL;
			}else
				*no = *temp;
			free(temp);
		}else{
			No *temp = minimo(no->dir);
			no->chave = temp->chave;
			no->dir = exclui(no->dir,temp->chave);
		}
	}

	if(no == NULL)
		return no;

	no->altura = 1 + max(altura(no->esq),altura(no->dir));

	int balanceamento = fatorBalanceamento(no);
	if((balanceamento > 1) && (fatorBalanceamento(no->esq) >= 0))
		return DirRot(no);
	if((balanceamento > 1) && (fatorBalanceamento(no->esq) < 0)){
		no->esq = EsqRot(no->esq);
		return DirRot(no);
	}
	if((balanceamento < -1) && (fatorBalanceamento(no->dir) <= 0))
		return EsqRot(no);
	if((balanceamento < -1) && (fatorBalanceamento(no->dir) > 0)){
		no->dir = DirRot(no->dir);
		return EsqRot(no);
	}

	return no;
}		

void calcular_nivel(No* no, int nivel){
	if(no==NULL)
		return;

	no->nivel = nivel;
	calcular_nivel(no->esq,nivel+1);
	calcular_nivel(no->dir,nivel+1);
}

int main(){
	char op;
	int x;
	int nivel = 0;
	No *no = NULL;
	
	scanf ("%c %d",&op,&x);
	while(!feof(stdin)){
		if(op == 'i'){
			no = insere(no,x);
		}
		else if(op == 'r'){
			no = exclui(no,x);
		}
		scanf("%c %d",&op,&x);
	}
	calcular_nivel(no,nivel);
	emOrdem(no);
	return 0;
}
