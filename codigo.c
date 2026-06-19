#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int verifica_data(int dia, int mes, int ano) { //1 para data válida 
    if(mes == 2) { //analisa dias de fevereiro com ano bissexto
        if(ano%4 == 0 && (ano%100 != 0 || ano%400 == 0)) {
            if(dia >= 1 && dia <= 29) {
                return 1;
            } else {
                return 0;
            }
        } else {
            if(dia >= 1 && dia <= 28) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    if(mes == 4 || mes == 6 || mes == 9 || mes == 11) { //analisa meses com 30 dias
        if(dia >= 1 && dia <= 30) {
            return 1;
        } else {
            return 0;
        }
    }
    if(dia >= 1 && dia <= 31) { //analisa meses com 31 dias
        return 1;
    } else {
        return 0;
    }
}

int verifica_id(int id, struct Estacao lista_estacoes[], int linhas) { //1 para id valido
    int i;
    for(i=0; i<linhas; i++) {
        if(lista_estacoes[i].id == id) { //verifica se o id já existe
            return 0;
        }
    }
    if(id>= 0 && id < 9999) return 1;
    return 0;
}

int conta_linhas() {
    FILE*arq = fopen("arquivo.csv", "r");
    int contador = 1; //Começa com 1 para contar a linha de cabeçalho
    char linha[100];
    while(fgets(linha, sizeof(linha), arq) != NULL) { //Enquanto houver linhas aumenta 1 no contador
        contador++;
    }
    fclose(arq);
    return contador;
}

struct DataLeitura{
    int dia;
    int mes;
    int ano;
};

struct Estacao{
    int id;
    char nome[60];
    char operador[60];
    char sensor[60]; //Ex: "temperatura, umidade, etc"
    int n; //quantidade de leituras
    float *leituras; //vetor alocado dinamicamente
    float media;
    float variancia;
    float desvioPadrao;
    struct DataLeitura data; //data da coleta
};

int main(void) {
    printf("   MENU  \n"); //Apresenta as funcionalidades do programa
    printf("1. Adicionar Estação\n");
    printf("2. Editar Estação\n");
    printf("3. Remover Estação\n");
    printf("4. Listar Estações\n");
    printf("5. Buscar por operador\n");
    printf("6. Detectar anomalias\n");
    printf("7. Sair\n");

    int opcao;
    int linhas;
    int i, j;
    scanf("%d", &opcao);
    linhas = conta_linhas(); //Conta quantas estações já estão cadastradas no arquivo
    struct Estacao lista_estacoes[linhas]; //Vetor para armazenar as estações

    for (i=0; i<linhas; i++) { //Leitura das estações presentes no arquivo
        FILE*arq = fopen("arquivo.csv", "r");
        char linha[300];
        fgets(linha, sizeof(linha), arq); 
        lista_estacoes[i].id = atoi(strtok(linha, ",")); 
        strcpy(lista_estacoes[i].nome, strtok(NULL, ",")); 
        strcpy(lista_estacoes[i].operador, strtok(NULL, ","));
        strcpy(lista_estacoes[i].sensor, strtok(NULL, ","));
        lista_estacoes[i].data.dia = atoi(strtok(NULL, "/")); 
        lista_estacoes[i].data.mes = atoi(strtok(NULL, "/"));
        lista_estacoes[i].data.ano = atoi(strtok(NULL, ",")); 
        lista_estacoes[i].n = atoi(strtok(NULL, ",")); 
        lista_estacoes[i].media = atof(strtok(NULL, ","));
        lista_estacoes[i].variancia = atof(strtok(NULL, ","));
        lista_estacoes[i].desvioPadrao = atof(strtok(NULL, ","));

        lista_estacoes[i].leituras = (float*)malloc(lista_estacoes[i].n * sizeof(float));
        for (j=0; j<lista_estacoes[i].n; j++) {
            lista_estacoes[i].leituras[j] = atof(strtok(NULL, ";")); //Lê as leituras da estação
        }
        fclose(arq);
    }

    if (opcao == 1) {
        FILE*arq = fopen("estacoes.txt", "a"); //Abre o arquivo para escrita
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }

        printf("Id: "); 
        scanf("%d", &lista_estacoes[linhas-1].id);
        while (verifica_id(lista_estacoes[linhas-1].id, lista_estacoes, linhas) != 1) { //le e verifica id
            printf("Id inválido! Digite um id menor que 9999 e que não exista: ");
            scanf("%d", &lista_estacoes[linhas-1].id);
        }

        printf("Nome da Estação: ");
        fgets(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome), stdin);

        printf("Operador: ");
        fgets(lista_estacoes[linhas-1].operador,sizeof(lista_estacoes[linhas-1].operador), stdin);

        printf("Sensor: ");
        fgets(lista_estacoes[linhas-1].sensor,sizeof(lista_estacoes[linhas-1].sensor), stdin);

        printf("Data (dd/mm/aaaa): ");
        char data[11];
        fgets(data, sizeof(data), stdin);
        lista_estacoes[linhas-1].data.dia = atoi(strtok(data, "/"));
        lista_estacoes[linhas-1].data.mes = atoi(strtok(NULL, "/"));
        lista_estacoes[linhas-1].data.ano = atoi(strtok(NULL, "/"));

        printf("Quantidade de leituras: ");
        scanf("%d", &lista_estacoes[linhas-1].n);

        float*leituras=(float*)malloc(lista_estacoes[linhas-1].n * sizeof(float));
        for(i=0; i<lista_estacoes[linhas-1].n; i++) {
            printf("Leitura %d: ", i+1);
            scanf("%f", &leituras[i]);
        }
        lista_estacoes[linhas-1].leituras = leituras;
        linhas++;
    }
}