#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


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

int verifica_n (int n) { //1 para n valido
    if(n>0 && n<9999) return 1;
    return 0;
}

int verifica_string(char string[], int n) { //1 para string valida
    int i;
    for(i=0; i<n; i++) {
        if(!isalpha(string[i]) && !isspace(string[i])) { //verifica se a string contém apenas letras e espaços
            return 1;
        }
    }
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

int buscar_estacao(struct Estacao lista_estacoes[], int id, int linhas) {
    int i;
    for(i=0;i<linhas;i++) {
        if(lista_estacoes[i].id == id) { //Retorna o indice da estação com o id buscado
            return i;
        }
    }
    return 0;
}

void imprimir_estacao(struct Estacao lista_estacoes[], int indice) { //Imprime os dados de uma estação
    struct Estacao estacao = lista_estacoes[indice];
    printf("Id: %d\n", estacao.id);
    printf("Nome: %s\n", estacao.nome);
    printf("Operador: %s\n", estacao.operador);
    printf("Sensor: %s\n", estacao.sensor);
    printf("Data: %02d/%02d/%04d\n", estacao.data.dia, estacao.data.mes, estacao.data.ano);
    printf("Quantidade de leituras: %d\n", estacao.n);
    printf("Média: %.2f\n", estacao.media);
    printf("Variância: %.2f\n", estacao.variancia);
    printf("Desvio Padrão: %.2f\n", estacao.desvioPadrao);
    printf("Leituras:\n");
    for(int i=0; i<estacao.n; i++) {
        printf("%.2f ", estacao.leituras[i]);
    }
    printf("\n");
}

void altera_estacao(struct Estacao lista_estacoes[], int indice, int alterador) {
    struct Estacao estacao = lista_estacoes[indice];
    if (alterador==1) {
        printf("Insira o novo nome da estação: ");
        fgets(estacao.nome, sizeof(estacao.nome), stdin);
    }
    if (alterador==2) {
        printf("Insira o novo nome do operador: ");
        fgets(estacao.operador, sizeof(estacao.operador), stdin);
    }
    if(alterador==3) {
        printf("Insira o novo tipo do sensor: ");
        fgets(estacao.sensor, sizeof(estacao.sensor), stdin);
    }
    if(alterador==4) {
        printf("Insira a nova data (dd/mm/aaaa): ");
        char data[11];
        fgets(data, sizeof(data), stdin);
        estacao.data.dia = atoi(strtok(data, "/"));
        estacao.data.mes = atoi(strtok(NULL, "/"));
        estacao.data.ano = atoi(strtok(NULL, "/"));
    }
    if (alterador==5) {
        int indice_novaleitura;
        float nova_leitura;
        printf("Digite o numero da leitura a ser alterada (1-%d): ", estacao.n);
        scanf("%d", &indice_novaleitura);
        while (indice_novaleitura < 1 || indice_novaleitura > estacao.n) {
            printf("Número inválido! Digite um número entre 1 e %d: ", estacao.n);
            scanf("%d", &indice_novaleitura);
        }
        printf("Digite o novo valor da leitura: ");
        scanf("%f", &nova_leitura);
        estacao.leituras[indice_novaleitura-1] = nova_leitura;
    }    
    printf("Alterado com sucesso!");    
}

void imprimir_operador(struct Estacao lista_estacoes[], int linhas, char operador[]) {
    int i;
    for(i=0; i<linhas; i++) {
        if(strcmp(lista_estacoes[i].operador, operador) == 0) {
            imprimir_estacao(lista_estacoes,i);
        }
    }
}

void verifica_leitura(float leitura) {

}

void imprimir_menu() { //Imprime as funcionalidade do programa
    printf("   MENU  \n");
    printf("1. Adicionar Estação\n");
    printf("2. Editar Estação\n");
    printf("3. Remover Estação\n");
    printf("4. Listar Estações\n");
    printf("5. Buscar por operador\n");
    printf("6. Detectar anomalias\n");
    printf("7. Sair\n");
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
    imprimir_menu();

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


    while(opcao != 7) {

    if (opcao == 1) { //opção para adicionar uma estacao
        FILE*arq = fopen("arquivo.csv", "a"); //Abre o arquivo para escrita
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
        while(verifica_string(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome)) != 0) { //le e verifica nome
            printf("Nome inválido! Digite um nome que contenha apenas letras e espaços: ");
            fgets(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome), stdin);
        }
        lista_estacoes[linhas-1].nome[strcspn(lista_estacoes[linhas-1].nome, "\n")] = 0; 

        printf("Operador: ");
        fgets(lista_estacoes[linhas-1].operador,sizeof(lista_estacoes[linhas-1].operador), stdin);
        while(verifica_string(lista_estacoes[linhas-1].operador, sizeof(lista_estacoes[linhas-1].operador)) != 0) { //le e verifica operador
            printf("Operador inválido! Digite um nome que contenha apenas letras e espaços: ");
            fgets(lista_estacoes[linhas-1].operador,sizeof(lista_estacoes[linhas-1].operador), stdin);
        }
        lista_estacoes[linhas-1].operador[strcspn(lista_estacoes[linhas-1].operador, "\n")] = 0;

        printf("Sensor: ");
        fgets(lista_estacoes[linhas-1].sensor,sizeof(lista_estacoes[linhas-1].sensor), stdin);
        while(verifica_string(lista_estacoes[linhas-1].sensor, sizeof(lista_estacoes[linhas-1].sensor)) != 0) { //le e verifica sensor
            printf("Sensor inválido! Digite um nome que contenha apenas letras e espaços: ");
            fgets(lista_estacoes[linhas-1].sensor,sizeof(lista_estacoes[linhas-1].sensor), stdin);
        }
        lista_estacoes[linhas-1].sensor[strcspn(lista_estacoes[linhas-1].sensor, "\n")] = 0;

        printf("Data (dd/mm/aaaa): ");
        char data[11];
        fgets(data, sizeof(data), stdin);
        lista_estacoes[linhas-1].data.dia = atoi(strtok(data, "/"));
        lista_estacoes[linhas-1].data.mes = atoi(strtok(NULL, "/"));
        lista_estacoes[linhas-1].data.ano = atoi(strtok(NULL, "/"));

        printf("Quantidade de leituras: ");
        scanf("%d", &lista_estacoes[linhas-1].n);
        while(verifica_n(lista_estacoes[linhas-1].n) != 1) { //le e verifica n
            printf("Quantidade inválida! Digite um número maior que 0 e menor que 9999: ");
            scanf("%d", &lista_estacoes[linhas-1].n);
        }

        float*leituras=(float*)malloc(lista_estacoes[linhas-1].n * sizeof(float));
        for(i=0; i<lista_estacoes[linhas-1].n; i++) {
            printf("Leitura %d: ", i+1);
            while(scanf("%f", &leituras[i]) != 1) { //le e verifica leituras
                while(getchar() != '\n'); //Limpa o buffer 
                printf("Leitura inválida! Digite um número válido para a leitura %d: ", i+1);
                scanf("%f", &leituras[i]);
            }
        }
        lista_estacoes[linhas-1].leituras = leituras;

        fprintf(arq, "%d;%s;%s;%s;%02d/%02d/%04d;%d", //Salva no CSV os dados da estação
                lista_estacoes[linhas-1].id,
                lista_estacoes[linhas-1].nome,
                lista_estacoes[linhas-1].operador,
                lista_estacoes[linhas-1].sensor,
                lista_estacoes[linhas-1].data.dia,
                lista_estacoes[linhas-1].data.mes,
                lista_estacoes[linhas-1].data.ano,
                lista_estacoes[linhas-1].n);

        for (i = 0; i < lista_estacoes[linhas-1].n; i++) {
            fprintf(arq, ";%.2f", leituras[i]);
        }
        fprintf(arq, "\n");

        fclose(arq);
        linhas++;
    }

    if(opcao==2) { //Opção para editar uma estação
        int id;
        printf("Digite o id da estação que deseja alterar: ");
        scanf("%d", &id);
        int indice = buscar_estacao(lista_estacoes, id, linhas);
        while(indice== 0) {
            printf("Id não encontrado!\n");
            printf("Digite o id da estação que deseja alterar: ");
            scanf("%d", &id);
            indice = buscar_estacao(lista_estacoes, id, linhas);
        }

        printf("Estação encontrada!\n");
        imprimir_estacao(lista_estacoes, indice);
        
        int alterador;
        printf("O Que deseja alterar?\n");
        printf("1. Nome\n");
        printf("2. Operador\n");            
        printf("3. Sensor\n");
        printf("4. Data\n");
        printf("5. Leituras\n");
        scanf("%d", &alterador);

        altera_estacao(lista_estacoes,indice,alterador);

    }

    if(opcao==3) { //Opção para remover uma estação
        int id;
        printf("Digite o id da estação que deseja remover: ");
        scanf("%d", &id);
        int indice = buscar_estacao(lista_estacoes, id, linhas);
        while(indice== 0) {
            printf("Id não encontrado!\n");
            printf("Digite o id da estação que deseja remover: ");
            scanf("%d", &id);
            indice = buscar_estacao(lista_estacoes, id, linhas);
        }
        for(i=indice; i<linhas-1; i++) {
            lista_estacoes[i] = lista_estacoes[i+1]; //Remove estação (passa o proximo espaço p/ anterior)
        }
        linhas--;
    }

    if(opcao==4) { //Opção para listar as estações
        for(i=0; i<linhas; i++) {
            printf("Estação %d:\n", i+1);
            imprimir_estacao(lista_estacoes, i);
            printf("\n");
        }
    }

    if(opcao==5) { //Buscar por operador}
        char operador[60];
        printf("Digite o nome do operador que deseja buscar: ");
        fgets(operador,sizeof(operador), stdin);
        imprimir_operador(lista_estacoes, linhas, operador);
    }

    if(opcao==6) {

    }

    imprimir_menu();
    scanf("%d", &opcao);
  }
  return 0;
}