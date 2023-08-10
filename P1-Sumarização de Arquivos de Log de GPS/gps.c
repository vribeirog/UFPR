#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "bike.h"

int main(int argc, char *argv[]){
	int nlog, valida;
	long op;
	char nomeDir[LINESIZE];
	char nomeBike[LINESIZE];
	char *ptr;
	infos *reg;
	
	if(strcmp(argv[1],"-d")==0){
		printf("Diretório: %s\n", argv[2]);
		strcpy(nomeDir, argv[2]);
		remove_palavra(nomeDir,"\n");

		nlog = contaDir(nomeDir);
	
		// Aloca um vetor de struct infos com tamanho do número de arquivos .log.
		reg = (infos*)malloc(nlog*sizeof(infos));

		// Processa os dados do logs.
		printf("Os logs estão sendo processados.\n.\n.\n.\n");
		leDir(nomeDir,reg);
		printf("OS DADOS FORAM PROCESSADOS!\n");
	
		imprimeResumo(reg,nlog,nomeBike);

		printf("\n---------MENU---------\n1-Lista de Bicicletas\n2-Resumo de uma Bicicleta\n3-Bicicletas/Data\n4-Bicicletas/Distancia\n5-Subida Acumulada\n6-Histograma\n0-SAIR\n");
		
		// Verifica a opção escolhida.
		valida = 0;
		while(valida == 0){
			printf("\nDigite a opção desejada: ");
			strcpy(nomeBike,"");
			setbuf(stdin,NULL);
			fgets(nomeBike,LINESIZE,stdin);
			op = strtol(nomeBike,&ptr,10);
			if((op >= 0)&&(op <= 6))
				valida = 1;
			else
				printf("\nOpção inválida!\n");
		}

		while((op != 0)&&(valida == 1)){
			
			// Opção Um - imprime os nomes das bicicletas presentes nos arquivos .log.
			if(op == 1)
				imprimeNomes(reg,nlog,nomeBike);
			
			// Opção Dois - mostra as informações de uma bicicleta escolhida e lista as informações sobre.
			if(op == 2){
				strcpy(nomeBike,"");
				printf("\nDigite o nome da Bicicleta: ");
				setbuf(stdin,NULL);
				fgets(nomeBike,LINESIZE,stdin);
				imprimeBike(reg,nlog,nomeBike);
			}

			// Opção Três - lista as informações agrupadas por bicicleta e ordenadas por data.
			if(op == 3)
				bikeData(reg,nlog,nomeBike);		

			// Opção Quatro - lista as informações agrupadas por bicicleta e ordenadas por distância.
			if(op == 4)
				bikeDist(reg, nlog, nomeBike);
			
			// Opção Cinco - lista as informações ordenadas pela subida acumulada.
			if(op == 5)
				subAcumulada(reg,nlog);
			
			// Opção Seis - apresenta o histograma de distâncias da bicicleta escolhida.
			if(op == 6){
				strcpy(nomeBike,"");
				printf("\nDigite o nome da Bicicleta: ");
				setbuf(stdin,NULL);
				fgets(nomeBike,LINESIZE,stdin);
				imprimeHistograma(reg,nlog,nomeBike);
			}
		
			printf("\n---------MENU---------\n1-Lista de Bicicletas\n2-Resumo de uma Bicicleta\n3-Bicicletas/Data\n4-Bicicletas/Distancia\n5-Subida Acumulada\n6-Histograma\n0-SAIR\n");

			valida = 0;
			while(valida == 0){
				printf("\nDigite a opção desejada: ");
				strcpy(nomeBike,"");
				setbuf(stdin,NULL);
				fgets(nomeBike,LINESIZE,stdin);
				op = strtol(nomeBike,&ptr,10);
				if((op >= 0)&&(op <= 6))
					valida = 1;
				else 
					printf("\nOpção inválida!\n");
			}
		}

		free(reg);
	}else
		printf("ERRO!\n");

	printf("-------------------FIM DO PROGRAMA-------------------\n\n");
	return 0;
}

