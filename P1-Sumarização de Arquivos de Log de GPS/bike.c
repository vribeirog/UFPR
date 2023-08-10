#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include "bike.h"

// Conta número de logs em um diretório por meio do nome do diretório ou path.
int contaDir(char *nomeDir){
	DIR *d;
	struct dirent *dir;
	int contalog = 0;
	char *ret;

	d = opendir(nomeDir);

	if (!d){
		perror("Erro ao abrir diretório.");
		exit(1);
	}

	dir = readdir(d);

	// Caminha pelo diretório
	while((dir = readdir(d)) != NULL){ 
		
		// Verifica se o arquivo tem .log
		ret = strstr(dir->d_name,".log");

		if (!strcmp(dir->d_name,".") || !strcmp(dir->d_name,".."))
			continue;
		else if(ret != NULL){
			contalog++;
		}
		
	}

	closedir(d);
	return contalog;
}

// Acessa diretório com logs por meio do nome do diretório ou path.
int leDir(char *nomeDir, infos *reg){
	DIR *d;
	struct dirent *dir;
	int n = 0;
	char *ret;

	d = opendir(nomeDir);

	if (!d)
	{
		perror("Erro ao abrir diretório.");
		exit(1);
	}
	
	dir = readdir(d);

	// Caminha pelo diretório
	while((dir = readdir(d)) != NULL){ 
		char path[LINESIZE];
		strcpy(path,nomeDir);
		strcat(path,"/");

		// Verifica se o arquivo tem .log
		ret = strstr(dir->d_name,".log");

		if (!strcmp(dir->d_name,".") || !strcmp(dir->d_name,".."))
			continue;
		else if(ret != NULL){
			strcpy(reg[n].log_name,dir->d_name);
			strcat(path, dir->d_name);
			leArq(path,reg,n);
			n++;
		}
		
	}
	closedir(d);
	return 0;
}

// Acessa arquivo .log e faz a leitura do cabeçalho com nome da bicicleta e data. 
int leArq(char *path, infos *reg, int n){
	FILE *arq;
	int k;
	char line[LINESIZE+1];

	arq = fopen(path,"r");

	if (!arq){
		perror("Erro ao abrir arquivo.");
		exit(1);
	}
	else{
		inicializaInfo(reg, n);

		fgets(line,LINESIZE,arq);
		remove_palavra(line,"Gear: ");
		strcpy(reg[n].bike_name ,line);

		fgets(line,LINESIZE,arq);
		remove_palavra(line,"Date: ");
		calculaData(reg,n,line);

		fgets(line,LINESIZE,arq);
		while(!feof(arq)){
			k = strcmp(line,"\n");
			if(k != 0){
				leHeader(line,reg,n);
				fgets(line,LINESIZE,arq);
			}
			else{
				fgets(line,LINESIZE,arq);
			}
		}
		converteUnidades(reg,n);
	}
	fclose(arq);
	return 0;
}

// Processa a informação de uma linha do log.
int leHeader(char *line, infos *reg, int n){
	char *token;
	int auxi;
	float auxf;

	token = strtok(line," ");
	while(token){

		auxi = 0;
		auxf = 0;

		if(strcmp(token,"altitude:")==0){
			token = strtok(NULL," ");
			reg[n].elev_atual = atof(token);
			auxf = reg[n].elev_atual - reg[n].elev_anterior;
			if(reg[n].elev == -1){
				reg[n].elev = 0;
			}else if((reg[n].elev != -1)&&(auxf > 0)){
				reg[n].elev += auxf;
			}
			reg[n].elev_anterior = reg[n].elev_atual;
		}

		if(strcmp(token,"cadence:")==0){
			token = strtok(NULL," ");
			reg[n].anterior_cad = reg[n].atual_cad;
			reg[n].atual_cad = atoi(token);
		}

		if(strcmp(token,"heart_rate:")==0){
			token = strtok(NULL," ");
			reg[n].anterior_hr = reg[n].atual_hr;
			reg[n].atual_hr = atoi(token);
			if(reg[n].atual_hr > reg[n].max_hr)
				reg[n].max_hr = reg[n].atual_hr;
		}

		if(strcmp(token,"speed:")==0){
			token = strtok(NULL," ");
			reg[n].anterior_speed = reg[n].atual_speed;
			reg[n].atual_speed = atof(token);
			if(reg[n].atual_speed > reg[n].max_speed)
				reg[n].max_speed = reg[n].atual_speed;
		}

		if(strcmp(token,"distance:")==0){
			token = strtok(NULL," ");
			auxf = atof(token) / 1000;
			reg[n].dist = auxf;
		}

		if(strcmp(token,"timestamp:")==0){
			token = strtok(NULL," ");
			token = strtok(NULL," ");
			reg[n].tempo_atual = converteSegundos(token);
			auxi = reg[n].tempo_atual - reg[n].tempo_anterior;
			if(reg[n].tempo == -1){
				reg[n].tempo = 0;
			}
			if(reg[n].tempo_anterior != -1){
				calculaMedia(reg,n,auxi);
				reg[n].tempo += auxi;
			}
			reg[n].tempo_anterior = reg[n].tempo_atual;
		}	
			
		token = strtok(NULL," ");
	}
	return 0;
}

// Converte unidades dos registros e faz o cálculo final das médias.
void converteUnidades(infos *reg, int n){
	if(reg[n].tempo_cad != 0)
		reg[n].media_cad = round(reg[n].media_cad / reg[n].tempo_cad);	
	else
		reg[n].media_cad = 0;
	
	if(reg[n].tempo_speed != 0)
		reg[n].media_speed = (reg[n].media_speed / reg[n].tempo_speed) * 3.6;
	else
		reg[n].media_speed = 0;
	
	if(reg[n].tempo_hr != 0)	
		reg[n].media_hr = round(reg[n].media_hr / reg[n].tempo_hr);
	else
		reg[n].media_hr = 0;

	reg[n].max_speed = reg[n].max_speed * 3.6;
}

// Processa o valor das médias em uma média ponderada e o tempo das médias.
void calculaMedia(infos *reg, int n, int tempo){
	if(tempo != 0){
		if(reg[n].anterior_speed != 0){
			reg[n].media_speed += (reg[n].anterior_speed * tempo);
			reg[n].tempo_speed += tempo;
		}
	
		if(reg[n].anterior_hr != 0){
			reg[n].media_hr += (reg[n].anterior_hr * tempo);
                	reg[n].tempo_hr += tempo;
		}

		if(reg[n].anterior_cad != 0){
			reg[n].media_cad += (reg[n].anterior_cad * tempo);
			reg[n].tempo_cad += tempo;
		}
	}
}

// Remove um substring(palavra) de uma string(frase).
void remove_palavra(char *frase, char *palavra){
	int tamanhoPalavra = strlen(palavra);

	if((frase == NULL) || (palavra == NULL))
		return;

	// Ponteiro para o início da palavra
	char *ponteiro = strstr(frase, palavra);
	if(ponteiro){
		// Posição da palavra na frase original
		int posicao = (ponteiro - frase);

		// Incrementamos o ponteiro para ir na posição final da palavra procurada
		ponteiro = ponteiro + tamanhoPalavra;

		// Percorre a frase e copia os caracteres após a palavra
		int i;
		for(i = 0; i < strlen(ponteiro); i++)
			frase[posicao + i] = ponteiro [i];

		// A frase agora tem que terminar antes
		frase[posicao + i] = 0;
	}
}

// Converte data do formato string: Mês Dia, Ano para inteiros dia, mês e ano.
void calculaData(infos *reg, int n, char *line){
	const char *months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char *pt;
	int i = 0;

	pt = strtok(line," ");
	while(reg[n].data_mes == 0){
		if(strcmp(pt,months[i])==0)
			reg[n].data_mes = i + 1;
		i++;
	}

	pt = strtok(NULL," ");
	remove_palavra(pt,",");
	reg[n].data_dia = atoi(pt);

	pt = strtok(NULL," ");
	remove_palavra(pt,",");
	reg[n].data_ano = atoi(pt);
}

// Converte horas, minutos e segundos em segundos.
int converteSegundos(char *token){
	char *pt;
	int seg;

	pt = strtok(token,":");
	seg = atoi(pt) * 3600;
	pt = strtok(NULL,":");
	seg = seg + (atoi(pt) * 60);
	pt = strtok(NULL,":");
	seg = seg + atoi(pt);
	
	return seg;
}

// Inicializa a struct infos.
void inicializaInfo(infos *reg, int n){
	reg[n].elev = -1;
	reg[n].elev_atual = -1;
	reg[n].elev_anterior = -1;
	reg[n].media_speed = 0;
	reg[n].max_speed = 0;
	reg[n].anterior_speed = 0;
	reg[n].atual_speed = 0;
	reg[n].media_hr = 0;
	reg[n].max_hr = 0;
	reg[n].anterior_hr = 0;
	reg[n].atual_hr = 0;
	reg[n].media_cad = 0;
	reg[n].anterior_cad = 0;
	reg[n].atual_cad = 0;
	reg[n].tempo = -1;
	reg[n].tempo_anterior = -1;
	reg[n].tempo_hr = 0;
	reg[n].tempo_speed = 0;
	reg[n].tempo_cad = 0;
	reg[n].data_mes = 0;
}

// Função de comparação de nome de bicicletas para o qsort.
int comparbike(const void *a, const void *b){
	return strcmp(((struct infos *)a)->bike_name,((struct infos *)b)->bike_name);
}

// Função de comparação de data para o qsort.
int compardata(const void *a, const void *b){
	if(((struct infos *)a)->data_ano > ((struct infos *)b)->data_ano)
		return 1;
	if(((struct infos *)a)->data_ano < ((struct infos *)b)->data_ano)
		return -1;
	if(((struct infos *)a)->data_mes > ((struct infos *)b)->data_mes)
		return 1;
	if(((struct infos *)a)->data_mes < ((struct infos *)b)->data_mes)
		return -1;
	if(((struct infos *)a)->data_dia > ((struct infos *)b)->data_dia)
		return 1;
	if(((struct infos *)a)->data_dia < ((struct infos *)b)->data_dia)
		return -1;
	return 0;
}

// Função de comparação de altitude/subida acumulada para o qsort.
int comparalt(const void *a, const void *b){
	if(((struct infos *)a)->elev > ((struct infos *)b)->elev)
		return 1;
	if(((struct infos *)a)->elev < ((struct infos *)b)->elev)
                return -1;
	return 0;
}

// Função de comparação de distância para o qsort.
int compardist(const void *a, const void *b){
        if(((struct infos *)a)->dist > ((struct infos *)b)->dist)
                return 1;
        if(((struct infos *)a)->dist < ((struct infos *)b)->dist)
                return -1;
        return 0;
}

// Imprime o resumo das bicicletas
void imprimeResumo(infos *reg, int nlog, char *nomeBike){
	double mediaDist;
	double totalDist = 0;
	double menorDist = 100000;
	double maiorDist = 0;
	int n = 0;
 	int cont = 1;

	strcpy(nomeBike,"");
	qsort(&reg[n],nlog,sizeof(infos),comparbike);
	
	while(n < nlog){
		if(strcmp(reg[n].bike_name,nomeBike) != 0){
			cont = 1;
			printf("\nBicicleta: %s",reg[n].bike_name);
			printf("                                      CADÊNCIA        HR MÉDIO         HR MÁX.      VELOC. MÉDIA      VELOC. MÁX.       DISTÂNCIA       SUBIDA ACUMULADA\n");
			printf("        DATA            LOG            (rpm)           (bpm)           (bpm)          (Km/h)           (Km/h)             (Km)               (m)\n");
			strcpy(nomeBike,reg[n].bike_name);
		}
		if(menorDist > reg[n].dist)
			menorDist = reg[n].dist;
		if(maiorDist < reg[n].dist)
			maiorDist = reg[n].dist;
		printf("%3d: ", cont);
		imprimeInfo(reg,n);
		totalDist += reg[n].dist;
		cont++;
		n++;
	}
	mediaDist = totalDist / nlog;
	
	printf("\n");
	printf("\n------------RESUMO------------\n");
	printf("Quantidade de Atividades: %d\n", nlog);
	printf("Total Percorrido em km: %.2f\n", totalDist);
	printf("Pedal mais longo em km: %.2f\n", maiorDist);
	printf("Pedal mais curto em km: %.2f\n", menorDist);
	printf("Distância Média em km: %.2f\n", mediaDist);
	printf("\n");
}

// Imprime uma struct infos.
void imprimeInfo(infos *reg, int n){
	printf("%02d/%02d/%4d    ", reg[n].data_dia, reg[n].data_mes, reg[n].data_ano);
	printf("%13s",reg[n].log_name);
	printf("       %3.0f      ",reg[n].media_cad);
	printf("      %4.0f      ",reg[n].media_hr);
	printf("      %4d      ",reg[n].max_hr);
	printf("      %5.2f      ", reg[n].media_speed);
	printf("      %5.2f      ", reg[n].max_speed);
	printf("      %6.2f      ", reg[n].dist);
	printf("      %7.2f\n", reg[n].elev);
}

// Lista o nome das bicicletas.
void imprimeNomes(infos *reg, int nlog, char *nomeBike){
	int n = 0;
	int cont = 1;

	strcpy(nomeBike,"");
	qsort(&reg[n],nlog,sizeof(infos),comparbike);
        while(n < nlog){
		if(strcmp(reg[n].bike_name,nomeBike) != 0){
			printf("%2d- ", cont);
                   	printf("%s", reg[n].bike_name);
			cont ++;
		}
		strcpy(nomeBike,reg[n].bike_name);
		n++;
	}
}

// Imprime infos da bicicleta especificada.
void imprimeBike(infos *reg, int nlog, char *nomeBike){
	int n = 0;
	int ini = -1;
	int cont = 1;

	qsort(&reg[n],nlog,sizeof(infos),comparbike);

	while(n < nlog){
		if((ini == -1)&&(strcmp(reg[n].bike_name,nomeBike)==0))
			ini = n;
		n++;
	}

	if(ini != -1){
		printf("                                      CADÊNCIA        HR MÉDIO         HR MÁX.      VELOC. MÉDIA      VELOC. MÁX.       DISTÂNCIA       SUBIDA ACUMULADA\n");
		printf("        DATA            LOG            (rpm)           (bpm)           (bpm)          (Km/h)           (Km/h)             (Km)               (m)\n");
	}else{
		printf("\nBicicleta não foi encontrada.\n");
	}

	while((ini < nlog)&&(ini != -1)){
		if(strcmp(nomeBike,reg[ini].bike_name)==0){
			printf("%3d: ", cont);
			imprimeInfo(reg,ini);
			cont++;
		}
		ini++;
	}
}

// Imprime os logs ordenados por bicicleta e data.
void bikeData(infos *reg, int nlog, char *nomeBike){
	int n = 0;
	int cont = 1;
				
	strcpy(nomeBike,"");
	qsort(&reg[n],nlog,sizeof(infos),compardata);
	qsort(&reg[n],nlog,sizeof(infos),comparbike);
	while(n < nlog){
		if(strcmp(reg[n].bike_name,nomeBike) != 0){
			cont = 1;
			printf("\nBicicleta: %s",reg[n].bike_name);
			printf("                                      CADÊNCIA        HR MÉDIO         HR MÁX.      VELOC. MÉDIA      VELOC. MÁX.       DISTÂNCIA       SUBIDA ACUMULADA\n");
			printf("        DATA            LOG            (rpm)           (bpm)           (bpm)          (Km/h)           (Km/h)             (Km)               (m)\n");
			strcpy(nomeBike,reg[n].bike_name);
		}
		printf("%3d: ", cont);
		imprimeInfo(reg,n);
		cont++;
		n++;
	}
}

// Imprime os logs ordenados por bicicleta e distância.
void bikeDist(infos *reg, int nlog, char *nomeBike){
	int n = 0;
	int cont = 1;
	
	strcpy(nomeBike,"");
	qsort(&reg[n],nlog,sizeof(infos),compardist);
	qsort(&reg[n],nlog,sizeof(infos),comparbike);
	while(n < nlog){
		if(strcmp(reg[n].bike_name,nomeBike) != 0){
			cont = 1;
			printf("\nBicicleta: %s",reg[n].bike_name);
			printf("                                      CADÊNCIA        HR MÉDIO         HR MÁX.      VELOC. MÉDIA      VELOC. MÁX.       DISTÂNCIA       SUBIDA ACUMULADA\n");
			printf("        DATA            LOG            (rpm)           (bpm)           (bpm)          (Km/h)           (Km/h)             (Km)               (m)\n");
			strcpy(nomeBike,reg[n].bike_name);
		}
		printf("%3d: ", cont);
		imprimeInfo(reg,n);
		cont++;
		n++;
	}
}

// Imprime os logs ordenados por subida acumulada.
void subAcumulada(infos *reg, int nlog){
	int n = 0;
	int cont = 1;
	
	qsort(&reg[n],nlog,sizeof(infos),comparalt);
	printf("                                      CADÊNCIA        HR MÉDIO         HR MÁX.      VELOC. MÉDIA      VELOC. MÁX.       DISTÂNCIA       SUBIDA ACUMULADA\n");
	printf("        DATA            LOG            (rpm)           (bpm)           (bpm)          (Km/h)           (Km/h)             (Km)               (m)\n");
	while(n < nlog){
 		printf("%3d: ", cont);
		imprimeInfo(reg,n);
		cont++;
		n++;
	}
}

// Imprime o histograma de distância dos logs.
void imprimeHistograma(infos *reg, int nlog, char *nomeBike){
	int i, fim, menord, aux;
	int ini = -1;
	int n = 0;
	int cont = 0;

	qsort(&reg[n],nlog,sizeof(infos),compardist);
	qsort(&reg[n],nlog,sizeof(infos),comparbike);

	for(i=0;i<nlog;i++){
		if((ini == -1)&&(strcmp(nomeBike,reg[i].bike_name)==0))
			ini = i;
		if(strcmp(nomeBike,reg[i].bike_name)==0)
			fim = i;
	}
	
	if(ini > -1){
		// ini é onde a bicicleta escolhido se inicia no vetor
		n = ini;
		// menor distância da bicicleta
		menord = reg[ini].dist;

		printf("\n-------------------HISTOGRAMA-------------------\n");
		printf("Distância|         Quantidade\n");

		while(n <= fim){
			aux = menord + 10;

			while((n <= fim)&&(reg[n].dist < aux)){
				cont++;
				n++;
			}
			printf("%3d -%3d |",menord,menord+9);

			for(i=1;i<=cont;i++){
				printf("*");
			}
			printf("\n");

			cont = 0;
			menord += 10;
		}

		printf("\n          123456789#123456789#123456789#123456789#\n");
		printf("\n");
	}else{
		printf("\nBicicleta não foi encontrada.\n");
	}
}
