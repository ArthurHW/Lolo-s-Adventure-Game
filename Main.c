#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include<conio2.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define S_CIMA 72
#define S_BAIXO 80
#define S_ESQ 75
#define S_DIR 77
#define ESC 27

// definição das structs:
// representa os dados de um registro de um jogador
typedef struct gravacao {
    int identificador;
    int totalpts;
    int ultimafase;
    int vidas;
    char nomejogador[9];
}save;

// representa uma fase, com seu tamanho, elementos e numero de inimigos
typedef struct fase {
    int tamanhox;
    int tamanhoy;
    int inimigos;
    char elementos[14][14]; // era pra ser 13 por 14 por causa do \n, mas deu um erro ai a gente teve que alterar pra 14 por 14
                            // o erro ja foi corrigido, mas deixamos assim pq n faz dferença no resto do programa
}fase;

typedef struct ponto {
    char x;
    char y;
    int vivo;
}ponto;

// protótipos das funções auxiliares:
int menu(save*); /*função que administra os menus do jogo, ela retorna 1 para a main caso o usuario queira sair do programa
e 0 caso o usuario esteja carregando ou criando um novo jogo, ela recebe uma struct do tipo save por referência e atualiza ela
com os dados do save que o jogador escolheu; caso o usuario só queira sair do programa, não faz nada com o save dado.*/
void imprime_menu(); // funcao para imprimir o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo, retorna a opção selecionada pelo usuario
int novoJogo(save*); /* funcao para comecar um novo jogo, ela retorna 1, caso o usuario digitou para voltar ao menu principal
e 0 caso o usuario tenha ido até o fim do processo de criar um novo jogo. Ela recebe um save por referencia e atualiza ele
para corresponder aos dados do save que acabou de ser criado*/
int carregarJogo(save*); /* funcao para carregar um jogo ja comecado, assim como função novo jogo retorna 1 para voltar ao menu principal
e 0 para seguir para o jogo, também recebe um save que será atualizado na função*/
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void instrucoes(); // imprime na tela as instruções do jogo
int imprime_saves(FILE*);// dado um arquivo de saves, imprime todos os saves dele na tela formatados; retorna o número de saves do arquivo
void mostra_info(save, int); // dado um save passado por cópia, mostra na tela seu nome, fase atual, total de pts e número de vidas.
void salvar_arquivo(save); // dado um save passado por cópia, escreve os dados dele alterados no arquivo de saves
void hidecursor(); // funcao pra esconder o cursor
fase gera_fase(int); // dado o numero de uma fase, preenche os elementos daquela fase em uma matriz contendo suas posicoes, e imprime a fase na tela
int movimentacao(fase*, save*); // funcao para a movimentacao, dada uma fase e um save, retorna o status do lolo, 1 para se morreu e 0 para se passou de fase
int contato_lolo(int, ponto*, int*, fase*, save*);
/* contato do lolo com os blocos, dada uma seta(direcao), o ponteiro para a posicao do lolo (ponto), contagem de coracoes (poder) do lolo, uma fase e um save (para atualizar os dados)
retorna o status do lolo, 1 se morreu e 0 se passou*/
int movimenta_inimigo(ponto*, fase*, int*, int*, save*);
void salvar_arquivo(save); // dado um save passado por cópia, escreve os dados dele alterados no arquivo de saves
void game_over(save); // dado o save de um jogador, apaga esse save do arquivo de saves e printa na tela game over
void morreu(save*); // informa ao jogador que ele morreu e atualiza os dados necessarios
void passou_de_fase(save*); // informa ao jogador que ele passou de fase, atualiza os dados necessarios e se ele está n ultima fase,
// informa que ele zerou o jogo


// Função principal
int main()
{
    int sair, status;
    save jogador;
    fase fase1;
    do {
        sair = menu(&jogador);
        if (!sair){ // se o usuario n quer sair, roda o jogo com a fase obtida no menu
            do
            {
                fase1 = gera_fase(jogador.ultimafase);
                status = movimentacao(&fase1, &jogador);
            }
            while (jogador.vidas > 0 && jogador.ultimafase <3 && status != -1); // executa enquanto o jogador possui vidas e nao passou de fase
            if (jogador.vidas > 0 && jogador.ultimafase == 3 && status != -1){ // if para não gerar um loop
                fase1 = gera_fase(jogador.ultimafase);
                status = movimentacao(&fase1, &jogador);
            }
        }
        if (jogador.vidas == 0 && sair == 0)
            game_over(jogador);
    } while (!sair);
    return 0;
}

// Funções Auxiliares

void hidecursor() // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

// função que coordena os menus, ela retorna 0, caso o usuario tenha entrado em um save, ou 1 caso o usuario queira sair do programa
int menu(save* jogador){
    int volta = 1;
    char opcao;

        do { // repetir enquanto
            clrscr(); // limpa a tela
            printf("---- Lolo's Adventure ----\n");
            imprime_menu(); // imprime o menu
            opcao = validaentrada(); // testa a entrada
            switch (opcao)
            {
                case 'N':   volta = novoJogo(jogador);
                            break;
                case 'C':   volta = carregarJogo(jogador);
                            break;
                case 'M':   mostraCreditos();
                            break;
                case 'S':   sair();
                            return 1;
                            break;
                case 'I':   instrucoes();
                            break;
            }
        } while (opcao != 'S'&& volta); // enquanto o usuario nao queia sair do programa, ou não tenha entrado em um "save"
    clrscr();
    return 0;
}

// função que imprime os menus
void imprime_menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(I) - Instrucoes\n(S) - Sair\n");

}

// função para mostrar os créditos:
void mostraCreditos()
{
    clrscr();
    printf("Creditos do jogo:\nDesenvolvido por: \nVitor Caruso Rodrigues Ferrer (00327023)\nArthur Henrique Wiebusch (00324318)\n");
    system("pause");
    fflush(stdin);
    clrscr();
}

// função para mostrar as instrucoes para o usuario
void instrucoes()
{
    clrscr();
    printf("======================================================================================================================\n                                                  INSTRUCOES ");
    printf("\n======================================================================================================================\n\n");
    printf("  Controles:\nUtilize as setas do teclado para mover o Lolo\nDurante o jogo, pressione ESC para voltar ao menu principal\n\n  Cenario:\nL - Lolo -> Personagem controlado pelo jogador\nP - Pedra -> Lolo nao pode se mover para esta posicao\n");
    printf("A - Agua -> Cuidado Lolo nao sabe nadar\nC - Coracao -> Ao pegar um coracao, Lolo pode tocar em um inimigo para elimina-lo\nB - Bloco movivel -> Lolo pode movimentar esse bloco, mas cuidado para nao ficar preso!\n");
    printf("E - Inimigo -> Os inimigos sao mais rapidos que o Lolo, entao na chegue muito perto, nao se esqueca dos coracoes\nT - Bau -> Apos derrotar todos os inimigos, corra ate ele para terminar a fase\n\n");
    printf("  Objetivo:\nO seu objetivo eh encontrar os coracoes, derrotar todos os inimigos e se dirigir ao bau para ir para a proxima fase\n\n");
    printf("  Pontuacao e vidas:\nLolo comeca o jogo com 3 vidas e 0 pontos, para cada inimigo eliminado Lolo ganha um ponto, a cada 10 pontos\nconsecutivos Lolo ganha uma vida extra");
    printf("se for eliminado a fase recomeca com a pontuacao zerada, alem de Lolo perder uma \nvida\n\n");
    printf("  Numero de gravacoes\nVoce pode ter ate 3 gravacoes simultaneas, com cada uma correspondendo ao ponto do jogo onde parou. \nObs: Ao entrar em um gravacao a fase em que voce estava eh resetada.\n");
    system("pause");
    clrscr();
}

// mensagem de encerramento do programa
void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

// função para validação da entrada do usuário para navegar entre os menus
char validaentrada()
{
    char opcao;
    do
    {
        printf("Digite uma opcao: ");
        fflush(stdin);
        scanf(" %c", &opcao);
        opcao = toupper(opcao); // para sempre ficar maiusculo, mesmo que o usuario digite minusculo
        if (opcao != 'N' && opcao != 'C' && opcao != 'M' && opcao != 'S' && opcao != 'I')
        {
            printf("Opcao invalida\n");
            imprime_menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S') && (opcao != 'I')); // enquanto nao for um dos caracteres validos
    return opcao;
}


// Cria um novo jogo para o usuario
int novoJogo(save* novo_jogador)
{
    clrscr();
    int op = 0;
    int id = 0;
    save buffer;
    FILE* arq;


    // abrir o arquivo com os saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");

    // ver quantos saves tem e salvar na variavel id
    else {
        while(!feof(arq)){
            if(fread(&buffer, sizeof(save), 1, arq) == 1)
                id++;

        }

        // se o limite de saves foi atingido, perguntar para o usuario se ele quer sobrescrever seus dados
        if(id >= 3){
            printf("Limite de Jogos salvos atingido (3), deseja sobrescrever seus dados?\n(1) - Sim\n(2) - Nao - Voltar ao menu principal\n");
            do{
                scanf(" %d", &op);
            } while(op != 1 && op != 2);
            // se ele nao quer sobrescrever os dados, encerrar a funcao e retornar ao menu
            if (op == 2){
                fclose(arq);
                return 1;
            }


        }


        if (op != 2){
            // se o usuario quer sobrescrever seus dados, perguntar qual jogador sobrescrever
            if (op == 1){
                imprime_saves(arq);
                printf("Qual Jogador deseja sobrecrever?\n");
                printf("-1 -> Voltar ao menu principal\n");
                do {
                scanf(" %d", &id);
                // caso ele mude de ideia ainda pode voltar ao menu principal
                if (id == -1){
                    fclose(arq);
                    return 1;
                }
                } while(id != 0 && id != 1 && id != 2);
                fseek(arq, sizeof(save) * id, SEEK_SET); // buscar o id que ele quer sobrecrever
            }
            // solicitar o nome
            printf("Digite seu nome (maximo de 8 caracteres): ");
            fflush(stdin);
            fgets((*novo_jogador).nomejogador, 8, stdin);
            (*novo_jogador).nomejogador[strlen((*novo_jogador).nomejogador)-1] = '\0';

            // salvar os outros dados do jogador
            (*novo_jogador).identificador = id;
            (*novo_jogador).totalpts = 0;
            (*novo_jogador).ultimafase = 1;
            (*novo_jogador).vidas = 3;


            // escrever os dados no arquivo binario
            if(fwrite(novo_jogador,sizeof(save), 1, arq) != 1)
                printf("Erro na escrita do save!\n");
        }
    }
    // fechar o arquivo e não voltar ao menu, pois o retorno 1 encerra a função menu também
    clrscr();
    fclose(arq);
    return 0;

}

// Função para carregar um jogo salvo
int carregarJogo(save* jogador)
{
    clrscr();
    FILE *arq;
    int id, numSaves;
    // abre o arquivo dos saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else {
        // imprime na tela os dados
        numSaves = imprime_saves(arq);
        // verifica se o numero de saves é 0, neste caso solicita ao usuario que crie um jogo antes
        if (numSaves == 0){
            printf("Parece que voce ainda nao possui nenhum jogo salvo, volte ao menu principal e crie um novo jogo!\n");
            system("pause");
            return 1;
        }
        else{
            // se ja existem jogos salvos, solicita ao usuario o jogo que ele deseja acessar
            printf("Caso deseje resetar todos os saves, digite 10 (nao escolha essa opcao a menos que tenha certeza de sua decisao!)\n");
            printf("Digite o id que deseja acessar: \n");
            printf("-1 -> Voltar ao menu principal\n");
            do{
                scanf("%d", &id);
                // o usuário pode voltar ao menu principal neste ponto também, digitando -1
                if (id == -1){
                    fclose(arq);
                    return 1;
                }
                else if(id == 10){
                    fclose(arq);
                    if(!(arq = fopen("saves.bin", "wb")))
                        printf("Erro na abertura do arquivo!\n");
                    fclose(arq);
                    return 1;
                }
            } while(0 > id || id > numSaves); // ler o id que o usuario está tentando acessar até que ele digite um id que existe, ou -1 para voltar ao menu
            // após isso, achar o arquivo, e passar seus dados para a variavel *jogador que foi passada por referência
            fseek(arq, sizeof(save)*id, SEEK_SET);
            if(fread(jogador, sizeof(save), 1, arq) != 1)
                printf("Erro na abertura do arquivo!\n");
        }
    }
    // caso o usuario chegue aqui, salva o arquivo e não retorna ao menu, pois ele pode iniciar o jogo
    fclose(arq);
    return 0;
}

// dado o arquivo saves.bin, com as gravações de todos os jogadores, imprime na tela as
// informações dos jogadores, com formatação adequada, o retorno da função é o número de saves lido
int imprime_saves(FILE* arq){
    save jogador;
    int numSaves = 0;
    rewind(arq);
    // enquanto não chegar no final do arquivo
    while(!feof(arq))
    {       // lê o arquivo
            if(fread(&jogador, sizeof(save), 1, arq) == 1)
                {
                // e imprime os saves
                numSaves++;
                printf("Jogador %d\n", jogador.identificador);
                printf("Nome: %s\n", jogador.nomejogador);
                printf("Pontos: %d\n", jogador.totalpts);
                printf("Fase: %d\n", jogador.ultimafase);
                printf("Vidas: %d\n\n", jogador.vidas);
                }
    }
    return numSaves; // retorna o número de saves lidos
    // a função não fecha o arquivo, pois isso é responsabilidade da função que a chamou
}

// função para mostrar as informações do jogador durante o jogo
void mostra_info(save jogador, int poder){

    cprintf("Fase: %d\nCoracoes: %d\nPontos: %d\nVidas: %d", jogador.ultimafase, poder, jogador.totalpts, jogador.vidas);
}

// função para salvar os dados do save a cada fase
void salvar_arquivo(save jogador){
    FILE *arq;
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else{
        fseek(arq,sizeof(jogador)* jogador.identificador, SEEK_SET);
        if ((fwrite(&jogador,sizeof(save), 1, arq)) != 1)
            printf("Erro ao escrever no arquivo\n");
        fclose(arq);
    }
}

// essa função apaga o registro do jogador, que perdeu
void game_over(save jogador){
    FILE* arqTemp;
    FILE* arq;
    save buffer;
    int id = 0;
    // abertura do arquivo temporário e do arquivo permanente
    if (!(arqTemp = fopen("savestemp.bin", "wb+")))
        printf("Erro na abertura do arquivo temporário\n");
    else{
        if (!(arq = fopen("saves.bin", "r+b"))){
            printf("Erro na abertura do arquivo de saves\n");
        }
        // enquanto não chegar no fim do arquivo permanente, lê ele e grava os jogadores que não tem o id do jogador
        // dado no arquivo temporário, assim o arquivo temporário tem uma cópia do arquivo permanente, com os dados do jogador
        // dado "excluídos"
        else {
            while (!feof(arq)){
                if((fread(&buffer, sizeof(save), 1, arq)) == 1){
                    if (buffer.identificador != jogador.identificador){
                        if ((fwrite(&buffer,sizeof(save), 1,arqTemp)) != 1)
                            printf("Erro ao escrever no arquivo\n");
                    }
                }
            }
            // fecha o arquivo tempoário e abre ele denovo, em modo de escrita, deletando seus dados
            fclose(arq);
            if (!(arq = fopen("saves.bin", "w+b"))){
                printf("Erro na abertura do arquivo de saves em modo de escrita\n");
            }
            // Passa os saves do arquivo temporário para o arquivo permanente, porém com o id atualizado
            else {
                rewind(arqTemp);
                while (!feof(arqTemp)){
                    if((fread(&buffer, sizeof(save), 1, arqTemp)) == 1){
                            buffer.identificador = id;
                            id++;
                        if ((fwrite(&buffer,sizeof(save), 1,arq)) != 1)
                            printf("Erro ao escrever no arquivo\n");
                    }
                }
            }
        // printa na tela o game over
        clrscr();
        printf("=======================================================================================================\n");
        printf("                                         GAME OVER\n");
        printf("=======================================================================================================\n");
        system("pause");
        // fecha os arquivos
        fclose(arq);
        fclose(arqTemp);
        }
    }
}

// dado o numero da fase, cria uma struct fase correspondente ao numero da fase dado e ja imprime na tela, o mapa correspondente
fase gera_fase(int numerofase)
{
    FILE* arq;
    int linha, coluna;
    fase fasea;
    char nome[10];

    switch (numerofase) // coloca o nome do arquivo dependendo do numero da fase
    {
        case 1: strncpy(nome, "Fase1.txt", 10);
                break;
        case 2: strncpy(nome, "Fase2.txt", 10);
                break;
        case 3: strncpy(nome, "Fase3.txt", 10);
                break;
    }

    arq = fopen(nome, "r"); // abre o arquivo correto
    if (arq == NULL)
        perror("Erro ao abrir o arquivo");
    else
    {
        fasea.inimigos = 0;
        fasea.tamanhox = 14;
        fasea.tamanhoy = 14;
        for (linha = 0; linha < fasea.tamanhox; linha++){
            for (coluna = 0; coluna < fasea.tamanhoy; coluna++){
                fasea.elementos[linha][coluna] = getc(arq); // preenche os elementos da fase
                if (fasea.elementos[linha][coluna] == 'E')
                    fasea.inimigos++;
                printf("%c", fasea.elementos[linha][coluna]); // imprime esses elementos
        }
    }
    fclose(arq);
    return fasea;
    }
}

// controla toda a movimentacao do jogo, retorna 1 se o Lolo morreu, ou 2 se ele passou de fase
int movimentacao(fase *fasea, save *jogador)
{
    int numInimigos;
    int status = 0;
    int statusmove_E = 0;
    char caracter = 'a', lolo = 'L';
    ponto pos_lolo, oldpos_lolo, inimigos[fasea->inimigos];
    int poder = 0, linha, coluna, contador = 0, i;
    numInimigos = fasea->inimigos;
    hidecursor();
    // percorre toda a matriz
    for (linha = 0; linha < fasea->tamanhoy; linha++){
        for (coluna = 0; coluna < fasea->tamanhox; coluna++){
            // para achar a posição do Lolo
            if (fasea->elementos[linha][coluna] == 'L')
            {
                pos_lolo.x = coluna+1; // +1 porque a matriz comeca em [0][0] e as coordenada em (1,1)
                pos_lolo.y = linha+1;
            }
            // e a posição de cada inimigo
            if (fasea->elementos[linha][coluna] == 'E'){
                (inimigos[contador]).x = coluna + 1;
                (inimigos[contador]).y = linha + 1;
                (inimigos[contador]).vivo = 1;
                contador++;
            }
        }
    }
    oldpos_lolo.x = pos_lolo.x;
    oldpos_lolo.y = pos_lolo.y;
    hidecursor();
    gotoxy(13,13); // posiciona antes pra aparecer a info zerada
    printf("\n");
    mostra_info(*jogador, poder); // funcao da info
    gotoxy(pos_lolo.x, pos_lolo.y);
    fasea->elementos[oldpos_lolo.y-1][oldpos_lolo.x-1] = ' ';
    fasea->elementos[pos_lolo.y-1][pos_lolo.x-1] = 'L';
    while (caracter != ESC && status != 1 && status != 2) // o usuario pode se movimentar ate clicar ESC, ou o Lolo morrer, ou passsar de fase
    {
        gotoxy(oldpos_lolo.x,  oldpos_lolo.y);
        printf(" ");
        gotoxy(pos_lolo.x, pos_lolo.y);
        printf("%c", lolo);
        // do while, pq ao ler uma seta, o getch sempre lia dois valores, um -32 e depois o caracter valido da seta
        do
            caracter = getch();
        while(caracter != S_CIMA && caracter != S_BAIXO && caracter != S_ESQ && caracter != S_DIR && caracter != ESC); // executa ate ser uma tecla valida
        oldpos_lolo.x = pos_lolo.x;
        oldpos_lolo.y = pos_lolo.y;
        for (contador = 0; contador < numInimigos; contador++)
        {
            i = 0;
            if (inimigos[contador].vivo == 1) // executa se o inimigo estiver vivo
            {
                do
                {
                    statusmove_E = movimenta_inimigo(&inimigos[contador], fasea, &status, &poder, jogador);
                    i++;
                }
                while (statusmove_E == 0 && i<5); // continua no loop enquanto o inimigo nao se mexeu, ou enquanto i for <5, para evitar
                                                  // crashar o programa, caso o inimigo fique preso
            }
        }
        // se o inimigo não matou o Lolo, então trata a ação dele
        if (status != 1)
        {
            switch (caracter)
            {
                case S_CIMA:  status = contato_lolo(S_CIMA, &pos_lolo, &poder, fasea, jogador);
                              break;
                case S_BAIXO: status = contato_lolo(S_BAIXO, &pos_lolo, &poder, fasea, jogador);
                              break;
                case S_ESQ:   status = contato_lolo(S_ESQ, &pos_lolo, &poder, fasea, jogador);
                              break;
                case S_DIR:   status = contato_lolo(S_DIR, &pos_lolo, &poder, fasea, jogador);
                              break;
            }
        }
        fasea->elementos[oldpos_lolo.y-1][oldpos_lolo.x-1] = ' '; // atualiza a posicao antiga e atual do
        fasea->elementos[pos_lolo.y-1][pos_lolo.x-1] = 'L';
        gotoxy(13,13);
        printf("\n");
        mostra_info(*jogador, poder); // funcao da info
    }
    if (caracter == ESC){
        status = -1;
    }
    else if (status == 1)
    {
        morreu(jogador); // como jogador ja e um endereço n precisa do &
    }
    else if (status == 2){
        passou_de_fase(jogador);
    }
    return status;
}

// trata a ação do Lolo
int contato_lolo(int seta, ponto *pos_lolo, int *poder, fase *fasea, save *jogador)
{
    int status = 0;
    ponto newpos_lolo, newpos_B;
    char caracter;

    newpos_lolo.x = pos_lolo->x-1; // -1 porque eles vao ser posicoes da matriz
    newpos_lolo.y = pos_lolo->y-1;
    newpos_B.x = pos_lolo->x-1;
    newpos_B.y = pos_lolo->y-1;
    switch(seta) // atualiza o x ou o y dependendo da tecla apertada pelo usuario
    {
        case S_CIMA:
            newpos_lolo.y--;
            newpos_B.y = newpos_B.y - 2; // atualiza a posicao do bloco movel, ja que eh sempre 1 a mais dependendo da direcao p onde o usuario se moveu
            break;
        case S_BAIXO:
            newpos_lolo.y++;
            newpos_B.y = newpos_B.y + 2;
            break;
        case S_ESQ:
            newpos_lolo.x--;
            newpos_B.x = newpos_B.x - 2;
            break;
        case S_DIR:
            newpos_lolo.x++;
            newpos_B.x = newpos_B.x + 2;
            break;
    }
    gotoxy(newpos_lolo.x, newpos_lolo.y);
    caracter = fasea->elementos[newpos_lolo.y][newpos_lolo.x];
    switch (caracter)
    {
        case ' ': // espaco vazio
            pos_lolo->x = newpos_lolo.x+1; // e volta para o +1, ja que agora eles sao coordenadas
            pos_lolo->y = newpos_lolo.y+1;
            break;
        case 'C': // coracao
            fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' '; // apaga a posicao
            pos_lolo->x = newpos_lolo.x+1;
            pos_lolo->y = newpos_lolo.y+1;
            (*poder)++;
            break;
        case 'E': // inimigo
            if (*poder != 0) // se o lolo tiver poder
            {
                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' ';
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
                (*poder)--; // diminui 1 do poder e dos inimigos
                fasea->inimigos--;
                jogador->totalpts++;
                if (jogador->totalpts % 10 == 0) // aumenta em 1 o numero de vidas do jogador a cada 10 pontos
                    jogador->vidas++;
            }
            else
            {
                gotoxy(pos_lolo->x, pos_lolo->y);
                printf(" ");
                status = 1; // senao, ele morre
            }
            break;
        case 'T': // bau
            if (fasea->inimigos == 0) // caso nao haja mais nenhum inimigo, ele pode pegar o bau
            {
                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' ';
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
                status = 2;
            }
            break;
        case 'B': // bloco movel
            if (fasea->elementos[newpos_B.y][newpos_B.x] == ' ') //  caso o espaco que o bloco movel vai ir seja vazio
            {
                fasea->elementos[newpos_lolo.y][newpos_lolo.x] = ' '; // apaga o espaco q tava o bloco movel
                pos_lolo->x = newpos_lolo.x+1;
                pos_lolo->y = newpos_lolo.y+1;
                fasea->elementos[newpos_B.y][newpos_B.x] = 'B'; // coloca um b na posicao desse novo espaco
                gotoxy(newpos_B.x+1,newpos_B.y+1); // posiciona e imprime o b (lembrando do +1 por causa da diferenca de posicao da matriz e coordenada)
                printf("%c", fasea->elementos[newpos_B.y][newpos_B.x]);
            }
            break;
        case 'A': // caso o bloco for agua, ele morre
            gotoxy(pos_lolo->x, pos_lolo->y); // apaga a posicao do lolo
            printf(" ");
            status = 1;
    }
    return status;
}

// Função que controla a movimentação do inimigo
int movimenta_inimigo(ponto* inimigo, fase* fasea, int* status, int* poder, save* jogador){
    int direcao, statusmove = 0;
    ponto oldpos_inimigo, pos_inimigo;
    char inimigo_char = 'E';
    char caracter;

    pos_inimigo.x = inimigo->x-1;
    pos_inimigo.y = inimigo->y-1;
    oldpos_inimigo.x = inimigo->x-1;
    oldpos_inimigo.y = inimigo->y-1;
    if (fasea->elementos[pos_inimigo.y][pos_inimigo.x] == 'E'){ // se o caracter na posicao for um E, ou seja, o lolo nao matou ele
        if (inimigo->vivo){
            direcao = (rand() % 4); // faz a movimentação aleatoria
            switch (direcao)
            {
                case 0:  pos_inimigo.x++;
                                break;
                case 1:  pos_inimigo.x--;
                                break;
                case 2:  pos_inimigo.y++;
                                break;
                case 3:  pos_inimigo.y--;
                                break;
            }
            // o inimigo so pode se mover para espaços em branco
            caracter = fasea->elementos[pos_inimigo.y][pos_inimigo.x];
            if (caracter == ' '){
                inimigo->x = pos_inimigo.x + 1;
                inimigo->y = pos_inimigo.y + 1;
                fasea->elementos[oldpos_inimigo.y][oldpos_inimigo.x] = ' ';
                fasea->elementos[pos_inimigo.y][pos_inimigo.x] = 'E';
                gotoxy(oldpos_inimigo.x+1, oldpos_inimigo.y+1);
                cprintf(" ");
                gotoxy(inimigo->x, inimigo->y);
                cprintf("E");
                statusmove = 1;
            }
            // ou para onde o Lolo está
            else if (caracter == 'L'){
                // se o Lolo não tem coração, mata ele
                if (*poder == 0){
                    gotoxy(oldpos_inimigo.x+1,oldpos_inimigo.y+1);
                    cprintf(" ");
                    gotoxy(pos_inimigo.x+1,pos_inimigo.y+1);
                    cprintf("E");
                    *status = 1;
                    statusmove = 1;
                }
                // se não morre
                else {
                    (*poder)--;
                    fasea->inimigos--;
                    inimigo->vivo = 0;
                    fasea->elementos[oldpos_inimigo.y][oldpos_inimigo.x] = ' ';
                    gotoxy(oldpos_inimigo.x+1,oldpos_inimigo.y+1);
                    cprintf(" ");
                    jogador->totalpts++;
                    if (jogador->totalpts % 10 == 0)
                        jogador->vidas++;
                    statusmove = 1;
                }
            }
        }
    }
    else {
        statusmove = 1;
    }
    return statusmove; // retorna o status de movimentacao do inimigo, 0 caso ele nao tenha se mexido (default) e 1 caso tenha se mexido
}

void morreu(save* jogador)
{
        Sleep(500);
        clrscr();
        printf("Voce morreu!\n");
        jogador->vidas--;
        jogador->totalpts = 0;
        printf("Vidas restantes: %d\n", jogador->vidas);
        printf("Salvando dados ...\n");
        salvar_arquivo(*jogador);
        Sleep(1000);
        clrscr(); // limpa a tela para imprimir a outra fase
}

void passou_de_fase(save* jogador)
{
    if (jogador->ultimafase == 3){
        clrscr();
        printf("=======================================================================================================\n");
        printf("                                         VOCE VENCEU\n");
        printf("=======================================================================================================\n");
        printf("Parabens vocc resgatou a princesa!!\n");
        system("pause");
        clrscr();
    }
    else{
        Sleep(500);
        clrscr();
        printf("Voce passou de fase!\n");
        jogador->ultimafase++;
        printf("Vidas restantes: %d\n", jogador->vidas);
        printf("Salvando dados ...\n");
        salvar_arquivo(*jogador);
        Sleep(2000);
        clrscr();
    }
}
