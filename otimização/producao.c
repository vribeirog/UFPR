#include <stdio.h>
#include <stdlib.h>

int main()
{

	// quantidade de produtos e compostos
	int produtos, compostos;
	fscanf(stdin, "%d", &produtos);
	fscanf(stdin, "%d", &compostos);

	// valor de venda de cada produtos
	int valores[produtos];
	for (int i = 0; i < produtos; i++)
	{
		fscanf(stdin, "%d", &valores[i]);
	}

	// custo e o limite diário de volume de cada composto
	int custo_compostos[compostos];
	int limites_compostos[compostos];
	for (int i = 0; i < compostos; i++)
	{
		fscanf(stdin, "%d", &custo_compostos[i]);
		fscanf(stdin, "%d", &limites_compostos[i]);
	}

	// armazena os coeficientes usados na função objetivo
	float coeficientes_obj[produtos];
	// variável auxiliar usada para armazenar o custo de fabricação do produto i
	float custo = 0;
	// quantidade de compostos necessarios para fabricar cada produto
	float matriz_produtos[produtos][compostos];
	// calcula os coeficientes da função objetivo
	for (int i = 0; i < produtos; i++)
	{
		for (int j = 0; j < compostos; j++)
		{
			fscanf(stdin, "%f", &matriz_produtos[i][j]);
			custo = custo + matriz_produtos[i][j] * custo_compostos[j];
		}
		coeficientes_obj[i] = valores[i] - custo;
		custo = 0;
	}

	// imprimir a função objetivo
	printf("max : ");
	for (int i = 0; i < produtos; i++)
	{
		if (i != produtos - 1)
			printf("%.2fx%d + ", coeficientes_obj[i], i);
		else
			printf("%.2fx%d;\n", coeficientes_obj[i], i);
	}
	printf("\n");

	// imprimir as restrições
	for (int i = 0; i < compostos; i++)
	{
		for (int j = 0; j < produtos; j++)
			if (j != produtos - 1)
				printf("%.2fx%d + ", matriz_produtos[j][i], j);
			else
				printf("%.2fx%d ", matriz_produtos[j][i], j);

		printf("<= %d;\n", limites_compostos[i]);
	}

	return 0;
}
