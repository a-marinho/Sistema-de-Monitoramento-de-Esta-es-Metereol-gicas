#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

struct DataLeitura{ //define struct de data (já presente no problema)
    int dia;
    int mes;
    int ano;
};

struct Estacao{ //define struct da estação (já presente no problema)
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

int verifica_data(char data[]) { //1 para data válida 
    int ano, mes, dia;

    dia = atoi(strtok(data, "/"));
    mes = atoi(strtok(NULL, "/"));
    ano = atoi(strtok(NULL, "/"));

    if(ano<1900 || ano>2100) return 0; //analisa anos válidos(pedido no problema)
    if(mes<=0 || mes>12) return 0; //analisa meses válidos

    if(mes == 2) { //analisa dias de fevereiro com ano bissexto
        if(ano%4 == 0 && (ano%100 != 0 || ano%400 == 0)) {
            if(dia >= 1 && dia <= 29) { //Anos bissextos tem o dia 29
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
    if(linhas==0 && id>=0 && id<= 9999) return 1; //Se não existem linhas, qualquer id é válido
    for(i=0; i<linhas; i++) {
        if(lista_estacoes[i].id == id) { //verifica se o id já existe
            return 0;
        }
    }
    if(id>= 0 && id < 9999) return 1; //se não existir, verifica se esta no intervalo
    return 0;
}

int verifica_n (int n) { //1 para n valido
    if(n>0 && n<9999) return 1;
    return 0;
}

int verifica_string(char string[], int n) { //1 para string valida
    int i;
    for(i=0; i<n && string[i] != '\0'; i++) {
        if(!isalpha(string[i]) && !isspace(string[i])) { //verifica se a string contém apenas letras e espaços
            return 1;
        }
    }
    return 0;
}
int conta_linhas() { //Conta as linhas do arquivo (essencial para achar estacoes)
    FILE*arq = fopen("arquivo.csv", "r");
    if(arq == NULL) {
        printf("Arquivo não encontrado!\n");
        return 1; 
    }
    int contador = 1; //Começa com 1 para contar a linha de cabeçalho
    char linha[100];
    while(fgets(linha, sizeof(linha), arq) != NULL) { //Enquanto houver linhas aumenta 1 no contador
        contador++;
    }
    fclose(arq);
    return contador;
}

int buscar_estacao(struct Estacao lista_estacoes[], int id, int linhas) { //Retorna o indice da estação com o id buscado
    int i;
    for(i=0;i<linhas;i++) {
        if(lista_estacoes[i].id == id) {
            return i;
        }
    }
    return -1; //Coloquei -1 e não 0, pois podemos ter i=0 (no caso da primeira estação)
}

float calcularMediaRecursiva(float leituras[], int n) { //Calcula a media recursivamente para opção 1
    if(n==0) return 0;
    return(leituras[n-1] + calcularMediaRecursiva(leituras, n-1) * (n-1)) / n;
}

float calcularVariancia(float leituras[], int n) { //útil para opção 1
    float media = calcularMediaRecursiva(leituras,n);
    if(n==0) return 0;
    float variancia = 0;
    int i;
    i=0;
    for(i=0; i<n; i++) { //Variancia é a soma da diferença de cada elemento em relação à média ao quadrado
        variancia = variancia +(leituras[i] - media) * (leituras[i] - media); 
    }
    return variancia;
}

void imprimir_estacao(struct Estacao lista_estacoes[], int indice) { //Imprime os dados de uma estação(opção 4)
    struct Estacao estacao = lista_estacoes[indice]; //Cria uma struct auxiliar
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

void altera_estacao(struct Estacao lista_estacoes[], int indice, int alterador) { //Altera estação (opção 3)
    struct Estacao estacao = lista_estacoes[indice];
    if (alterador==1) {
        printf("Insira o novo nome da estação: ");
        fgets(estacao.nome, sizeof(estacao.nome), stdin);
        while(verifica_string(estacao.nome,sizeof(estacao.nome)) != 0) { //Le e verifica nome
            printf("Nome invalido! Digite um nome que contenha apenas lestras e espaços: ");
            fgets(estacao.nome,sizeof(estacao.nome), stdin);
        }
        estacao.nome[strcspn(estacao.nome,"\n")] = '\0'; 
        strcpy(lista_estacoes[indice].nome, estacao.nome); //grava a nova estacao
    }
    if (alterador==2) {
        printf("Insira o novo nome do operador: ");
        fgets(estacao.operador, sizeof(estacao.operador), stdin);
        while(verifica_string(estacao.operador,sizeof(estacao.operador)) != 0) { //Le e verifica operador
            printf("Nome de operador invalido! Digite um nome que contenha apenas letras e espaços: ");
            fgets(estacao.operador,sizeof(estacao.operador),stdin);
        }
        estacao.operador[strcspn(estacao.operador,"\n")]='\0';
        strcpy(lista_estacoes[indice].operador, estacao.operador); //grava o novo operador
    }
    if(alterador==3) {
        printf("Insira o novo tipo do sensor: ");
        fgets(estacao.sensor, sizeof(estacao.sensor), stdin);
        while(verifica_string(estacao.sensor,sizeof(estacao.sensor)) != 0) { //Le e verifica operador
            printf("Nome de sensor invalido! Digite um nome que contenha apenas letras e espaços: ");
            fgets(estacao.sensor,sizeof(estacao.sensor),stdin);
        }
        estacao.sensor[strcspn(estacao.operador,"\n")]='\0';
        strcpy(lista_estacoes[indice].sensor, estacao.sensor); //grava o novo sensor
    }
    if(alterador==4) {
        printf("Insira a nova data (dd/mm/aaaa): ");
        char data[11];
        fgets(data, sizeof(data), stdin);
        data[strcspn(data,"\n")] ='\0';
        while(getchar() != '\n'); 
        while(verifica_data(data) != 1) { //verifica se a data é valida
            printf("Data Inválida! Insira outra data: ");
            fgets(data,sizeof(data),stdin); 
            data[strcspn(data,"\n")] ='\0';
            while(getchar() != '\n'); //Limpa o buffer
        }
        lista_estacoes[indice].data.dia = atoi(strtok(data,"/")); //grava novo dia, mes e ano
        lista_estacoes[indice].data.mes = atoi(strtok(NULL, "/"));
        lista_estacoes[indice].data.ano = atoi(strtok(NULL, "/"));
    }
    if (alterador==5) {
        int indice_novaleitura;
        float nova_leitura;
        printf("Digite o numero da leitura a ser alterada (1-%d): ", estacao.n);
        scanf("%d", &indice_novaleitura); //le o numero digitado da leitura a ser alterada
        getchar();
        while (indice_novaleitura < 1 || indice_novaleitura > estacao.n) { //checa se o numero é válido
            printf("Número inválido! Digite um número entre 1 e %d: ", estacao.n);
            scanf("%d", &indice_novaleitura);
            getchar();
        }
        while(1) {
            printf("Digite o novo valor da leitura: ");
            if (scanf("%f", &nova_leitura) == 1) {
                while(getchar != '\n'); //Limpa o buffer
                break;
            }
            else{
                while(getchar != '\n');
                printf("Leitura inválida! Digite um valor válido: ");
            }
        }
        lista_estacoes[indice].leituras[indice_novaleitura-1] = nova_leitura; //grava o novo valor
        //Atualiza media, variancia e desvio com o novo valor de leitura
        lista_estacoes[indice].media = calcularMediaRecursiva(lista_estacoes[indice].leituras, estacao.n);
        lista_estacoes[indice].variancia = calcularVariancia(lista_estacoes[indice].leituras, estacao.n);
        lista_estacoes[indice].desvioPadrao = sqrt(lista_estacoes[indice].variancia);
    }    
    printf("Alterado com sucesso!");    
}

//Imprime as estações que tenham o mesmo operador 
void imprimir_operador(struct Estacao lista_estacoes[], int linhas, char operador[]) {
    int i;
    for(i=0; i<linhas; i++) {
        if(strcmp(lista_estacoes[i].operador, operador) == 0) { //Checa o operador de cada estação
            imprimir_estacao(lista_estacoes,i);
        }
    }
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

//Abre o arquivo e grava os dados em uma array de structs (lista_estacoes)
void abrirCSV(struct Estacao lista_estacoes[], int linhas) { 
    FILE*arq = fopen("arquivo.csv", "r");
    if(arq == NULL) { //Checa se foi possível abir o arquivo
        printf("Erro ao abrir o arquivo!\n");
        return 1; 
    }
    int i,j;
    for (i=0; i<linhas; i++) { //Leitura das estações presentes no arquivo
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
    }
    fclose(arq);
}

//No final do programa salva todas as alterações feitas em lista_estacoes para o arquivo CSV
void salvarCSV(struct Estacao lista_estacoes[], int linhas) {
    FILE*arq = fopen("arquivo.csv", "w");
    if (arq == NULL) { //Checa se foi possível abrir o arquivo
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    int i, j;
    for (int i = 0; i < linhas; i++) {
        fprintf(arq, "%d,%s,%s,%s,%d,%.2f,%.2f,%.2f,%02d/%02d/%d", //Grava todas as informações
                lista_estacoes[i].id,
                lista_estacoes[i].nome,
                lista_estacoes[i].operador,
                lista_estacoes[i].sensor,
                lista_estacoes[i].n,
                lista_estacoes[i].media,
                lista_estacoes[i].variancia,
                lista_estacoes[i].desvioPadrao,
                lista_estacoes[i].data.dia,
                lista_estacoes[i].data.mes,
                lista_estacoes[i].data.ano);
        for (int j = 0; j < lista_estacoes[i].n; j++) {
            fprintf(arq, ";%.2f", lista_estacoes[i].leituras[j]);
        }
        fprintf(arq, "\n");
    }
    fclose(arq);
}

void Adicionar_estacao(struct Estacao lista_estacoes[], int linhas) { //código opção 1
    FILE*arq = fopen("arquivo.csv", "a"); //Abre o arquivo para escrita
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    int i;
    printf("Id: "); //Armazena o ID da nova estação (quando válido)
    scanf("%d", &lista_estacoes[linhas-1].id);
    getchar();
    while (verifica_id(lista_estacoes[linhas-1].id, lista_estacoes, linhas) != 1) { //le e verifica id
        printf("Id inválido! Digite um id menor que 9999 e que não exista: ");
        scanf("%d", &lista_estacoes[linhas-1].id);
        getchar();
    }

    printf("Nome da Estação: "); //Armazena o nome da estação (quando válido)
    fgets(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome), stdin);
    while(verifica_string(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome)) != 0) { //le e verifica nome
        printf("Nome inválido! Digite um nome que contenha apenas letras e espaços: ");
        fgets(lista_estacoes[linhas-1].nome, sizeof(lista_estacoes[linhas-1].nome), stdin);
    }
    lista_estacoes[linhas-1].nome[strcspn(lista_estacoes[linhas-1].nome, "\n")] = '\0'; 

    printf("Operador: "); //Armazena o nome do operador(quando válido)
    fgets(lista_estacoes[linhas-1].operador,sizeof(lista_estacoes[linhas-1].operador), stdin);
    while(verifica_string(lista_estacoes[linhas-1].operador, sizeof(lista_estacoes[linhas-1].operador)) != 0) { //le e verifica operador
        printf("Operador inválido! Digite um nome que contenha apenas letras e espaços: ");
        fgets(lista_estacoes[linhas-1].operador,sizeof(lista_estacoes[linhas-1].operador), stdin);
    }
    lista_estacoes[linhas-1].operador[strcspn(lista_estacoes[linhas-1].operador, "\n")] = '\0';

    printf("Sensor: "); //Armazena o nome da string(quando válida)
    fgets(lista_estacoes[linhas-1].sensor,sizeof(lista_estacoes[linhas-1].sensor), stdin);
    while(verifica_string(lista_estacoes[linhas-1].sensor, sizeof(lista_estacoes[linhas-1].sensor)) != 0) { //le e verifica sensor
        printf("Sensor inválido! Digite um nome que contenha apenas letras e espaços: ");
        fgets(lista_estacoes[linhas-1].sensor,sizeof(lista_estacoes[linhas-1].sensor), stdin);
    }
    lista_estacoes[linhas-1].sensor[strcspn(lista_estacoes[linhas-1].sensor, "\n")] = '\0';

    printf("Data (dd/mm/aaaa): ");
    char data[11];
    fgets(data, sizeof(data), stdin);
    data[strcspn(data,"\n")] ='\0';
    while(getchar() != '\n'); 
    while(verifica_data(data) != 1) { //verifica se a data é valida
        printf("Data Inválida! Insira outra data: ");
        fgets(data,sizeof(data),stdin); 
        data[strcspn(data,"\n")] ='\0';
        while(getchar() != '\n'); //Limpa o buffer
    }
    //Grava os valores válidos da data na struct
    lista_estacoes[linhas-1].data.dia = atoi(strtok(data, "/"));
    lista_estacoes[linhas-1].data.mes = atoi(strtok(NULL, "/"));
    lista_estacoes[linhas-1].data.ano = atoi(strtok(NULL, "/"));

    printf("Quantidade de leituras: ");
    scanf("%d", &lista_estacoes[linhas-1].n);
    getchar();
    while(verifica_n(lista_estacoes[linhas-1].n) != 1) { //le e verifica n
        printf("Quantidade inválida! Digite um número maior que 0 e menor que 9999: ");
        scanf("%d", &lista_estacoes[linhas-1].n);
        getchar();
    }

    float*leituras=(float*)malloc(lista_estacoes[linhas-1].n * sizeof(float)); //Aloca dinamicamente mem
    for(i=0; i<lista_estacoes[linhas-1].n; i++) {
        while(1) { //Loop opera ate leitura válida
            printf("Leitura %d: ", i+1);
            if(scanf("%f", &leituras[i]) == 1) { //le e verifica leituras
                while(getchar() != '\n'); //Limpa o buffer de entrada
                break;
            } else {
                while(getchar() != '\n'); //Limpa o buffer para uma próxima leitura
                printf("Valor inválido! Digite um número válido: ");
            }
        }
    }
    lista_estacoes[linhas-1].leituras = leituras;
    //Calcula media, variancia e desvio automaticamente após valores de leitura serem inseridos
    lista_estacoes[linhas-1].media = calcularMediaRecursiva(leituras, lista_estacoes[linhas-1].n);
    lista_estacoes[linhas-1].variancia = calcularVariancia(leituras,lista_estacoes[linhas-1].n);
    lista_estacoes[linhas-1].desvioPadrao = sqrt(lista_estacoes[linhas-1].variancia);

    fprintf(arq, "%d,%s,%s,%s,%d,%.2f,%.2f,%.2f,%02d/%02d/%04d,%d", //Salva no CSV os dados 
            lista_estacoes[linhas-1].id,
            lista_estacoes[linhas-1].nome,
            lista_estacoes[linhas-1].operador,
            lista_estacoes[linhas-1].sensor,
            lista_estacoes[linhas-1].n,
            lista_estacoes[linhas-1].media,
            lista_estacoes[linhas-1].variancia,
            lista_estacoes[linhas-1].desvioPadrao,
            lista_estacoes[linhas-1].data.dia,
            lista_estacoes[linhas-1].data.mes,
            lista_estacoes[linhas-1].data.ano);

    for (i = 0; i < lista_estacoes[linhas-1].n; i++) { //Percorre todas as leituras e grava no CSV
        fprintf(arq, ";%.2f", leituras[i]);
    }
    fprintf(arq, "\n");

    fclose(arq);
    linhas++; //adiciona mais uma linha na contagem total
    free(leituras); //libera a memória alocada dinamicamente
    leituras=NULL; //se eu colocar um free sem querer... free(NULL) não da nada
}


void EditarEstacao(struct Estacao lista_estacoes[], int linhas) { //Código opção 2
    int id;
    printf("Digite o id da estação que deseja alterar: ");
    scanf("%d", &id);
    getchar();
    int indice = buscar_estacao(lista_estacoes, id, linhas); //Busca o indice da estação com id
    while(indice== 0) { //Checa se existe estação com esse id
        printf("Id não encontrado!\n");
        printf("Digite o id da estação que deseja alterar: ");
        scanf("%d", &id);
        getchar();
        indice = buscar_estacao(lista_estacoes, id, linhas);
        }

    printf("Estação encontrada!\n");
    imprimir_estacao(lista_estacoes, indice); //Imprime a estação com esse id
        
    int alterador; //Define o que será alterado na struct
    printf("O Que deseja alterar?\n");
    printf("1. Nome\n");
    printf("2. Operador\n");            
    printf("3. Sensor\n");
    printf("4. Data\n");
    printf("5. Leituras\n");
    scanf("%d", &alterador);
    getchar();
    //Altera a estação de acordo com o alterador e dados digitados
    altera_estacao(lista_estacoes,indice,alterador); 
}


void RemoveEstacao(struct Estacao lista_estacoes[], int linhas) { //Código opção 3
    int id, i;
    printf("Digite o id da estação que deseja remover: ");
    scanf("%d", &id);
    getchar();
    int indice = buscar_estacao(lista_estacoes, id, linhas); //Procura o indice com o id digitado
    while(indice== 0) { //Valida o id
        printf("Id não encontrado!\n");
        printf("Digite o id da estação que deseja remover: ");
        scanf("%d", &id);
        getchar();
        indice = buscar_estacao(lista_estacoes, id, linhas);
        }
    for(i=indice; i<linhas-1; i++) {
        lista_estacoes[i] = lista_estacoes[i+1]; //Remove estação (passa o proximo espaço p/ anterior)
        }
    free(lista_estacoes[indice].leituras); //Limpa a memória alocada dinamicamente
    linhas--; //diminui a contagem de linhas, uma vez que removemos uma estação
}

void ListaEstacoes (struct Estacao lista_estacoes[], int linhas) { //Código opção 4
        int i;
        for(i=0; i<linhas; i++) {
            printf("Estação %d:\n", i+1);
            imprimir_estacao(lista_estacoes, i); //Imprime todas as estações do arquivo CSV
            printf("\n");
        }
}

void BuscarporOperador(struct Estacao lista_estacoes[], int linhas) { //Código opção 5
    char operador[60];
    printf("Digite o nome do operador que deseja buscar: ");
    fgets(operador,sizeof(operador), stdin);
    while(verifica_string(operador, sizeof(operador)) != 0) { //Valida o nome digitado
        printf("Nome de operador inválido! Digite um nome que contenha apenas letras e espaços: ");
        fgets(operador,sizeof(operador),stdin);
    }
    operador[strcspn(operador, "\n")] = '\0';
    imprimir_operador(lista_estacoes, linhas, operador); //Imprime estações atreladas ao operador
}

void Anomalias(struct Estacao lista_estacoes[], int linhas) { //Código opção 6
    int id;
    printf("Insira o id da estação que deseja analisar: ");
    scanf("%d", &id);
    getchar();
    //Checa se o id existe
    while(verifica_id(buscar_estacao(lista_estacoes, id, linhas), lista_estacoes, linhas) != 0) {
        printf("Id não encontrado! Digite um id válido: ");
        scanf("%d", &id);
        getchar();
        }
        int i,j;
        float x,y;
        i = buscar_estacao(lista_estacoes, id, linhas);
        struct Estacao estacao = lista_estacoes[i];
        //Anomalias são valores que apresentam mais de dois desvios padrões em relação à média
        x= (2*estacao.desvioPadrao) + estacao.media;
        y= estacao.media - (2*estacao.desvioPadrao);
        printf("Anomalias encontradas:\n");
        for(j=0; j<estacao.n; j++) {
            if(estacao.leituras[j] > x || estacao.leituras[j] < y) { //Busca as leituras anômalas
                printf("Leitura %d: %.2f\n", j+1, estacao.leituras[j]);
            }
        }
}

int main(void) {
    imprimir_menu();

    int opcao;
    int linhas;
    int i,j;
    scanf("%d", &opcao);
    getchar();
    linhas = conta_linhas(); //Conta quantas estações já estão cadastradas no arquivo
    struct Estacao lista_estacoes[linhas]; //Vetor para armazenar as estações
   
    //Enquanto o usuário não sair o programa continua rodando
    while(opcao != 7) { 
        if (opcao == 1) { //opção para adicionar uma estacao
       Adicionar_estacao(lista_estacoes,linhas);
    }
    else if(opcao==2) { //Opção para editar uma estação
       EditarEstacao(lista_estacoes,linhas);
    }
    else if(opcao==3) { //Opção para remover uma estação
        RemoveEstacao(lista_estacoes,linhas);
    }
    else if(opcao==4) { //Opção para listar as estações
        ListaEstacao(lista_estacoes,linhas);
    }
    else if(opcao==5) { //Buscar por operador}
        BuscarporOperador(lista_estacoes,linhas);
    }
    else if(opcao==6) { //Buscar anomalias de uma estação
       Anomalias(lista_estacoes,linhas);
    }
    imprimir_menu();
    scanf("%d", &opcao);
    getchar();
  }

  salvarCSV(lista_estacoes, linhas); //Salva as alterações feitas no programa no arquivo CSV
  return 0;
}