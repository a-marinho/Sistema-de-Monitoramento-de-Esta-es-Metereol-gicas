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
    char copia[11]; //Precisamos criar string auxiliar, pois strtok destroi a string analisada
    strcpy(copia,data); //Copia data para copia

    //Ponteiros vão analisar se a string data é válida
    char *a1=strtok(copia,"/");
    char *a2=strtok(NULL,"/");
    char *a3=strtok(NULL,"/");
    //Verifica se existem números em cada ponteiro(atoi(NULL) da problema)
    if(a1==NULL||a2==NULL||a3==NULL) return 0; 

    dia = atoi(a1);
    mes = atoi(a2);
    ano = atoi(a3);

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
    if(id>= 0 && id <= 9999) return 1; //se não existir, verifica se esta no intervalo
    return 0;
}

int verifica_n (int n) { //1 para n valido
    if(n>0 && n<9999) return 1;
    return 0;
}

int verifica_string(char string[], int n) { //0 para string valida
    int i;
    for(i=0; i<n && string[i] != '\0'; i++) {
        //Verifica se a string contem apenas letras e espaços
        if(!isalpha(string[i]) && string[i]!=' ' && string[i] != '\0') { 
            return 1; 
        }
    }
    return 0;
}

int conta_linhas() { //Conta as linhas do arquivo (essencial para achar estacoes)
    FILE*arq = fopen("arquivo.csv", "r");
     if(arq == NULL) {
        // Arquivo não existe: cria com o cabeçalho
        arq = fopen("arquivo.csv", "w");
        if(arq == NULL) {
            printf("Erro ao criar o arquivo!\n");
            return 0;
        }
        fprintf(arq, "ID,Nome,Operador,Sensor,Data,N,Media,Variancia,DesvioPadrao,Leituras\n");
        fclose(arq);
        return 0;
    }
    int contador = 0; 
    char linha[2000];
    fgets(linha,sizeof(linha),arq); //Consome o cabeçalho sem contar
    while(fgets(linha, sizeof(linha), arq) != NULL) { //Enquanto houver linhas aumenta 1 no contador
        contador++;
    }
    fclose(arq);
    return contador;
}

//Retorna o índice da estação com o id buscado
int buscar_estacao(struct Estacao lista_estacoes[], int id, int linhas) {  
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
    //Variancia é a soma da diferença de cada elemento em relação à média ao quadrado sobre n
    for(i=0; i<n; i++) { 
        variancia = variancia +(leituras[i] - media) * (leituras[i] - media); 
    }
    return variancia/n;
}

void imprimir_estacao(struct Estacao lista_estacoes[], int indice) { //Imprime os dados de uma estação(opção 4)
    struct Estacao estacao = lista_estacoes[indice]; //Cria uma struct auxiliar
    printf("Id: %d\n", estacao.id);
    printf("Nome: %s\n", estacao.nome);
    printf("Operador: %s\n", estacao.operador);
    printf("Sensor: %s\n", estacao.sensor);
    printf("Data: %02d/%02d/%04d\n", estacao.data.dia, estacao.data.mes, estacao.data.ano);
    printf("Quantidade de leituras: %d\n", estacao.n);
    printf("Media: %.2f\n", estacao.media);
    printf("Variancia: %.2f\n", estacao.variancia);
    printf("Desvio Padrao: %.2f\n", estacao.desvioPadrao);
    printf("Leituras:\n");
    for(int i=0; i<estacao.n; i++) {
        printf("%.2f ", estacao.leituras[i]);
    }
    printf("\n");
}

void altera_estacao(struct Estacao lista_estacoes[], int indice, int alterador) { //Altera estação (opção 2)
    struct Estacao estacao = lista_estacoes[indice];
    if (alterador==1) {
        printf("Insira o novo nome da estacao: ");
        fgets(estacao.nome, sizeof(estacao.nome), stdin);
        //Importante pois verifica_string não valida o \n
        estacao.nome[strcspn(estacao.nome, "\n")] = '\0'; 
        while(verifica_string(estacao.nome,sizeof(estacao.nome)) != 0) { //Le e verifica nome
            printf("Nome invalido! Digite um nome que contenha apenas letras e espacos: ");
            fgets(estacao.nome,sizeof(estacao.nome), stdin);
            estacao.nome[strcspn(estacao.nome,"\n")] = '\0'; 
        }
        strcpy(lista_estacoes[indice].nome, estacao.nome); //grava a nova estacao
    }

    if (alterador==2) {
        printf("Insira o novo nome do operador: ");
        fgets(estacao.operador, sizeof(estacao.operador), stdin);
        estacao.operador[strcspn(estacao.operador,"\n")]='\0';
        while(verifica_string(estacao.operador,sizeof(estacao.operador)) != 0) { //Le e verifica operador
            printf("Nome de operador invalido! Digite um nome que contenha apenas letras e espacos: ");
            fgets(estacao.operador,sizeof(estacao.operador),stdin);
            estacao.operador[strcspn(estacao.operador,"\n")]='\0';
        }
        strcpy(lista_estacoes[indice].operador, estacao.operador); //grava o novo operador
    }

    if(alterador==3) {
        printf("Insira o novo tipo do sensor: ");
        fgets(estacao.sensor, sizeof(estacao.sensor), stdin);
        estacao.sensor[strcspn(estacao.sensor,"\n")]='\0';
        while(verifica_string(estacao.sensor,sizeof(estacao.sensor)) != 0) { //Le e verifica operador
            printf("Nome de sensor invalido! Digite um nome que contenha apenas letras e espacos: ");
            fgets(estacao.sensor,sizeof(estacao.sensor),stdin);
            estacao.sensor[strcspn(estacao.sensor,"\n")]='\0';
        }
        strcpy(lista_estacoes[indice].sensor, estacao.sensor); //grava o novo sensor
    }

    if(alterador==4) {
        printf("Insira a nova data (dd/mm/aaaa): ");
        char data[30];
        fgets(data, sizeof(data), stdin);
        if(strchr(data,'\n') == NULL) { //Verifica se precisa limpar o buffer
            while(getchar() != '\n'); //limpa o buffer
        }
        data[strcspn(data,"\n")] ='\0';
        while(verifica_data(data) != 1) { //verifica se a data é valida
            printf("Data Invalida! Insira outra data: ");
            fgets(data,sizeof(data),stdin); 
            if(strchr(data,'\n') == NULL) { //Mesma lógica da anterior
                while(getchar() != '\n'); 
            }
            data[strcspn(data,"\n")] ='\0';
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
            printf("Numero inválido! Digite um numero entre 1 e %d: ", estacao.n);
            scanf("%d", &indice_novaleitura);
            getchar();
        }
        while(1) {
            printf("Digite o novo valor da leitura: ");
            if (scanf("%f", &nova_leitura) == 1) {
                while(getchar() != '\n'); //Limpa o buffer
                break;
            }
            else{
                while(getchar() != '\n');
                printf("Leitura invalida! Digite um valor valido: ");
            }
        }
        lista_estacoes[indice].leituras[indice_novaleitura-1] = nova_leitura; //grava o novo valor
        //Atualiza media, variancia e desvio com o novo valor de leitura
        lista_estacoes[indice].media = calcularMediaRecursiva(lista_estacoes[indice].leituras, estacao.n);
        lista_estacoes[indice].variancia = calcularVariancia(lista_estacoes[indice].leituras, estacao.n);
        lista_estacoes[indice].desvioPadrao = sqrt(lista_estacoes[indice].variancia);
    }    
    printf("Alterado com sucesso!");  
    printf("\n");   
}

//Imprime as estações que tenham o mesmo operador 
void imprimir_operador(struct Estacao lista_estacoes[], int linhas, char operador[]) {
    int i;
    for(i=0; i<linhas; i++) {
        if(strcmp(lista_estacoes[i].operador, operador) == 0) { //Checa o operador de cada estação
            imprimir_estacao(lista_estacoes,i);
            printf("\n"); 
        }
    }
}


void imprimir_menu() { //Imprime as funcionalidade do programa
    printf("   MENU  \n");
    printf("1. Adicionar Estacao\n");
    printf("2. Editar Estacao\n");
    printf("3. Remover Estacao\n");
    printf("4. Listar Estacoes\n");
    printf("5. Buscar por operador\n");
    printf("6. Detectar anomalias\n");
    printf("7. Sair\n");
    printf("\n");
}

//Abre o arquivo e grava os dados em uma array de structs (lista_estacoes)
int abrirCSV(struct Estacao lista_estacoes[], int linhas) { 
    FILE*arq = fopen("arquivo.csv", "r");
    if(arq == NULL) { //Checa se foi possível abir o arquivo
        printf("Erro ao abrir o arquivo!\n");
        return 0; 
    }
    int i,j;
    char linha[2000];
    fgets(linha, sizeof(linha),arq); //Pula a linha do cabeçalho
    for (i=0; i<linhas; i++) { //Leitura das estações presentes no arquivo
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
            lista_estacoes[i].leituras[j] = atof(strtok(NULL, ",;\n")); //Lê as leituras da estação
        }
    }
    fclose(arq);
    return 1;
}

//No final do programa salva todas as alterações feitas em lista_estacoes para o arquivo CSV
void salvarCSV(struct Estacao lista_estacoes[], int linhas) {
    FILE*arq = fopen("arquivo.csv", "w");
    if (arq == NULL) { //Checa se foi possível abrir o arquivo
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    //Grava o cabeçalho 
    fprintf(arq, "ID,Nome,Operador,Sensor,Data,N,Media,Variancia,DesvioPadrao,Leituras\n");
    int i, j;
    for (int i = 0; i < linhas; i++) {
        fprintf(arq, "%d,%s,%s,%s,%02d/%02d/%04d,%d,%.2f,%.2f,%.2f", //Grava todas as informações
                lista_estacoes[i].id,
                lista_estacoes[i].nome,
                lista_estacoes[i].operador,
                lista_estacoes[i].sensor,
                lista_estacoes[i].data.dia,
                lista_estacoes[i].data.mes,
                lista_estacoes[i].data.ano,
                lista_estacoes[i].n,
                lista_estacoes[i].media,
                lista_estacoes[i].variancia,
                lista_estacoes[i].desvioPadrao);
        for (int j = 0; j < lista_estacoes[i].n; j++) {
            if(j==0) fprintf(arq, ",%.2f", lista_estacoes[i].leituras[j]);
            else fprintf(arq, ";%.2f", lista_estacoes[i].leituras[j]); 
        }
        fprintf(arq, "\n");
    }
    fclose(arq);
}

//codigo opção 1
//linhas deve ser ponteiro para alteração (linhas++) refletir no main
void Adicionar_estacao(struct Estacao lista_estacoes[], int *linhas) { 
    int i;
    printf("Id: "); //Armazena o ID da nova estação (quando válido)
    scanf("%d", &lista_estacoes[*linhas].id);
    getchar();
    while (verifica_id(lista_estacoes[*linhas].id, lista_estacoes, *linhas) != 1) { //le e verifica id
        printf("Id invalido! Digite um id menor ou igual que 9999 e que nao exista: ");
        scanf("%d", &lista_estacoes[*linhas].id);
        getchar();
    }

    printf("Nome da Estacao: "); //Armazena o nome da estação (quando válido)
    fgets(lista_estacoes[*linhas].nome, sizeof(lista_estacoes[*linhas].nome), stdin);
    //Importante, pois verifica_string não valia \n
    lista_estacoes[*linhas].nome[strcspn(lista_estacoes[*linhas].nome, "\n")] = '\0'; 
    while(verifica_string(lista_estacoes[*linhas].nome, sizeof(lista_estacoes[*linhas].nome)) != 0) { //le e verifica nome
        printf("Nome invalido! Digite um nome que contenha apenas letras e espacos: ");
        fgets(lista_estacoes[*linhas].nome, sizeof(lista_estacoes[*linhas].nome), stdin);
        lista_estacoes[*linhas].nome[strcspn(lista_estacoes[*linhas].nome, "\n")] = '\0'; 
    }

    printf("Operador: "); //Armazena o nome do operador(quando válido)
    fgets(lista_estacoes[*linhas].operador,sizeof(lista_estacoes[*linhas].operador), stdin);
    lista_estacoes[*linhas].operador[strcspn(lista_estacoes[*linhas].operador, "\n")] = '\0';
    while(verifica_string(lista_estacoes[*linhas].operador, sizeof(lista_estacoes[*linhas].operador)) != 0) { //le e verifica operador
        printf("Operador invalido! Digite um nome que contenha apenas letras e espacos: ");
        fgets(lista_estacoes[*linhas].operador,sizeof(lista_estacoes[*linhas].operador), stdin);
        lista_estacoes[*linhas].operador[strcspn(lista_estacoes[*linhas].operador, "\n")] = '\0';
    }

    printf("Sensor: "); //Armazena o nome da string(quando válida)
    fgets(lista_estacoes[*linhas].sensor,sizeof(lista_estacoes[*linhas].sensor), stdin);
    lista_estacoes[*linhas].sensor[strcspn(lista_estacoes[*linhas].sensor, "\n")] = '\0';
    while(verifica_string(lista_estacoes[*linhas].sensor, sizeof(lista_estacoes[*linhas].sensor)) != 0) { //le e verifica sensor
        printf("Sensor invalido! Digite um nome que contenha apenas letras e espacos: ");
        fgets(lista_estacoes[*linhas].sensor,sizeof(lista_estacoes[*linhas].sensor), stdin);
        lista_estacoes[*linhas].sensor[strcspn(lista_estacoes[*linhas].sensor, "\n")] = '\0';
    }

    printf("Data (dd/mm/aaaa): ");
    char data[30];
    fgets(data, sizeof(data), stdin);
    //Nesse caso strchr procura se em data temos \n
    //Se não houver, retorna NULL. O que significa que sobrou coisa no buffer que deve ser limpa
    if(strchr(data,'\n') == NULL) { 
        while(getchar() != '\n'); //Limpa o buffer
    }
    data[strcspn(data,"\n")] ='\0'; //Troca o \n da string se houver
    
    while(verifica_data(data) != 1) { //verifica se a data é valida
        printf("Data Invalida! Insira outra data: ");
        fgets(data,sizeof(data),stdin); 
        if(strchr(data,'\n') == NULL) { //Mesma lógica da anterior
            while(getchar() != '\n'); 
        }
        data[strcspn(data,"\n")] ='\0';
    }
    //Grava os valores válidos da data na struct
    lista_estacoes[*linhas].data.dia = atoi(strtok(data, "/"));
    lista_estacoes[*linhas].data.mes = atoi(strtok(NULL, "/"));
    lista_estacoes[*linhas].data.ano = atoi(strtok(NULL, "/"));

    printf("Quantidade de leituras: ");
    scanf("%d", &lista_estacoes[*linhas].n);
    getchar();
    while(verifica_n(lista_estacoes[*linhas].n) != 1) { //le e verifica n
        printf("Quantidade invalida! Digite um numero maior que 0 e menor que 9999: ");
        scanf("%d", &lista_estacoes[*linhas].n);
        getchar();
    }

    float*leituras=(float*)malloc(lista_estacoes[*linhas].n * sizeof(float)); //Aloca dinamicamente mem
    for(i=0; i<lista_estacoes[*linhas].n; i++) {
        while(1) { //Loop opera ate leitura válida
            printf("Leitura %d: ", i+1);
            if(scanf("%f", &leituras[i]) == 1) { //le e verifica leituras
                while(getchar() != '\n'); //Limpa o buffer de entrada
                break;
            } else {
                while(getchar() != '\n'); //Limpa o buffer para uma próxima leitura
                printf("Valor invalido! Digite um número valido: ");
            }
        }
    }
    lista_estacoes[*linhas].leituras = leituras;
    //Calcula media, variancia e desvio automaticamente após valores de leitura serem inseridos
    lista_estacoes[*linhas].media = calcularMediaRecursiva(leituras, lista_estacoes[*linhas].n);
    lista_estacoes[*linhas].variancia = calcularVariancia(leituras,lista_estacoes[*linhas].n);
    lista_estacoes[*linhas].desvioPadrao = sqrt(lista_estacoes[*linhas].variancia);

    printf("Adicionada com sucesso!\n"); 
    printf("\n");

    (*linhas)++; //adiciona mais uma linha na contagem total
    salvarCSV(lista_estacoes, *linhas); //Salva a estação no Arquivo CSV
}

//Código opção 2
void EditarEstacao(struct Estacao lista_estacoes[], int linhas) { 
    int id;
    printf("Digite o id da estacao que deseja alterar: ");
    scanf("%d", &id);
    getchar();
    int indice = buscar_estacao(lista_estacoes, id, linhas); //Busca o indice da estação com id
    while(indice== -1) { //Checa se existe estação com esse id
        printf("Id nao encontrado!\n");
        printf("Digite o id da estacao que deseja alterar: ");
        scanf("%d", &id);
        getchar();
        indice = buscar_estacao(lista_estacoes, id, linhas);
        }

    printf("Estacao encontrada!\n");
    printf("\n"); 
    imprimir_estacao(lista_estacoes, indice); //Imprime a estação com esse id
    printf("\n");
        
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

//Código opção 3
//Linhas é ponteiro para linhas-- refletir no main
void RemoveEstacao(struct Estacao lista_estacoes[], int *linhas) { 
    int id, i;
    printf("Digite o id da estacao que deseja remover: ");
    scanf("%d", &id);
    getchar();
    int indice = buscar_estacao(lista_estacoes, id, *linhas); //Procura o indice com o id digitado
    while(indice== -1) { //Valida o id
        printf("Id nao encontrado!\n");
        printf("Digite o id da estacao que deseja remover: ");
        scanf("%d", &id);
        getchar();
        indice = buscar_estacao(lista_estacoes, id, *linhas);
        }

    free(lista_estacoes[indice].leituras); //Limpa a memória alocada dinamicamente
    for(i=indice; i<(*linhas)-1; i++) {
        lista_estacoes[i] = lista_estacoes[i+1]; //Remove estação (passa o proximo espaço p/ anterior)
        }
    printf("Estacao removida com sucesso!\n ");
    printf("\n"); 

    (*linhas)--; //diminui a contagem de linhas, uma vez que removemos uma estação
}

void ListaEstacoes (struct Estacao lista_estacoes[], int linhas) { //Código opção 4
        int i;
        for(i=0; i<linhas; i++) {
            printf("Estacao %d:\n", i+1);
            imprimir_estacao(lista_estacoes, i); //Imprime todas as estações do arquivo CSV
            printf("\n");
        }
}

void BuscarporOperador(struct Estacao lista_estacoes[], int linhas) { //Código opção 5
    char operador[60];
    printf("Digite o nome do operador que deseja buscar: ");
    fgets(operador,sizeof(operador), stdin);
    operador[strcspn(operador, "\n")] = '\0';
    
    while(verifica_string(operador, sizeof(operador)) != 0) { //Valida o nome digitado
        printf("Nome de operador invalido! Digite um nome que contenha apenas letras e espacos: ");
        fgets(operador,sizeof(operador),stdin);
    }
    operador[strcspn(operador, "\n")] = '\0';
    imprimir_operador(lista_estacoes, linhas, operador); //Imprime estações atreladas ao operador
}

void Anomalias(struct Estacao lista_estacoes[], int linhas) { //Código opção 6
    int id;
    printf("Insira o id da estacao que deseja analisar: ");
    scanf("%d", &id);
    getchar();
    int indice= buscar_estacao(lista_estacoes,id,linhas); 
    //Checa se o id existe
    while(indice== -1) {
        printf("Id nao encontrado! Digite um id valido: ");
        scanf("%d", &id);
        getchar();
        indice=buscar_estacao(lista_estacoes,id,linhas);
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
        printf("\n"); 
}

int main(void) {
    int opcao;
    int linhas;
    int i,j;
    linhas = conta_linhas(); //Conta quantas estações já estão cadastradas no arquivo

    //Array de structs declarada dinamicamente
    struct Estacao *lista_estacoes = malloc(10000 * sizeof(struct Estacao)); 
    if(lista_estacoes == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1; 
    }

   //Grava as informações do arquivo CSV na array de structs
    if(linhas>0) abrirCSV(lista_estacoes,linhas); 

    imprimir_menu();
    printf("Insira a funcionalidade que deseja executar: ");
    while(scanf("%d", &opcao) != 1){
        printf("Opcao invalida! Digite um numero de 1 a 7: ");
        while(getchar() != '\n');
    }

    //Enquanto o usuário não sair o programa continua rodando
    while(opcao != 7) { 
        if (opcao == 1) { //opção para adicionar uma estacao
       Adicionar_estacao(lista_estacoes,&linhas);
    }
    else if(opcao==2) { //Opção para editar uma estação
       EditarEstacao(lista_estacoes,linhas);
    }
    else if(opcao==3) { //Opção para remover uma estação
        RemoveEstacao(lista_estacoes,&linhas);
    }
    else if(opcao==4) { //Opção para listar as estações
        ListaEstacoes(lista_estacoes,linhas);
    }
    else if(opcao==5) { //Buscar por operador}
        BuscarporOperador(lista_estacoes,linhas);
    }
    else if(opcao==6) { //Buscar anomalias de uma estação
       Anomalias(lista_estacoes,linhas);
    }
    else{
        printf("Opcao invalida! Digite um numero de 1 a 7: ");
    }
    imprimir_menu();
    scanf("%d", &opcao);
    getchar();
  }

  salvarCSV(lista_estacoes, linhas); //Salva as alterações feitas no programa no arquivo CSV

  //Libera a memória alocada dinamicamente para cada estação
  for(i=0; i<linhas; i++) {
    free(lista_estacoes[i].leituras); 
  }

  free(lista_estacoes);
  return 0;
}