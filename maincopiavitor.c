#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio2.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define S_CIMA 72
#define S_BAIXO 80
#define S_ESQ 75
#define S_DIR 77
#define ESC 27

typedef struct gravacao {
    int identificador;
    int totalpts;
    int ultimafase;
    int vidas;
    char nomejogador[9];
}save;

typedef struct fase {
    int tamanhox;
    int tamanhoy;
    char elementos[14][14]; // 13 por 14 ele nao lia uma parte do mapa ai tive que botar 14x14
}fase;

typedef struct ponto {
    char x;
    char y;
}ponto;

typedef struct jogador_st {
    int totalpts;
    int fase;
    int vidas;
    char nomejogador[9];
}jogador;

// protótipos das funções auxiliares:
void menu();
void imprime_menu(); // funcao para imprimir o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
int novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void imprime_saves(FILE*);// dado um save imprime ele na tela formatado
fase gera_fase(int); // dado o numero de uma fase, retorna uma struct fase
void movimentacao(fase); // funcao para a movimentacao, dada uma fase
void imprime_mapa(fase); // imprime a fase
void hidecursor(); // funcao pra esconder o cursor
void contato_lolo(int, int*, int*);

// Função principal
int main()
{
    fase fase1;
//    menu();
    fase1 = gera_fase(2);
    imprime_mapa(fase1);
//    movimentacao(fase1);
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

void menu(){
    int volta = 0;
    char opcao;

        do {
            printf("---- Lolo's Adventure ----\n");
            imprime_menu();
            opcao = validaentrada();
            switch (opcao)
            {
                case 'N':   novoJogo();
                            break;
                case 'C':   carregarJogo();
                            break;
                case 'M':   mostraCreditos();
                            break;
                case 'S':   sair();
                            break;
            }
        } while (opcao != 'S' && volta != 0);

}

void imprime_menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(S) - Sair\n");

}

void mostraCreditos()
{
    clrscr();
    printf("Creditos do jogo:\nDesenvolvido por: Vitor Caruso Rodrigues Ferrer (00327023)\nArthur Henrique Wiebusch (00324318)\n");
    system("pause");
    fflush(stdin);
    clrscr();
}

void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

char validaentrada()
{
    char opcao;
    do
    {
        printf("Digite uma opcao: ");
        fflush(stdin);
        scanf(" %c", &opcao);
        opcao = toupper(opcao); // para sempre ficar maiusculo, mesmo que o usuario digite minusculo
        if (opcao != 'N' && opcao != 'C' && opcao != 'M' && opcao != 'S')
        {
            printf("Opcao invalida\n");
            imprime_menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S')); // enquanto nao for um dos caracteres validos
    return opcao;
}

// Cria um novo jogo para o usuario
int novoJogo()
{
    char op = '0';
    char idchar = '0';
    int id;
    save novo_jogador, buffer;
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
        if(id >= 3){
            printf("Limite de Jogos salvos atingido (3), deseja sobrescrever seus dados?\n(1) - Sim\n(2)- Nao\n");
            printf("(V) - Voltar\n");
            do{
                scanf(" %c", &op);
            } while(op != '1' && op != '2');
            if (toupper(op) == 'V')
                return 0;
        }

        if (op != '2'){
            // solicitar e salvar o nome do jogador
            if (op == '1'){
                imprime_saves(arq);
                printf("Qual id deseja sobrecrever?\n");
                printf("(V) - Voltar\n");
                do {
                scanf(" %c", &idchar);
                if (toupper(idchar) == 'V')
                    return 1;
                id = atoi(idchar);
                } while(id != 0 && id != 1 && id != 2);
                fseek(arq, sizeof(save) * id, SEEK_SET);
            }
            printf("Digite seu nome (maximo de 8 caracteres): ");
            fflush(stdin);
            fgets(novo_jogador.nomejogador, 8, stdin);
            novo_jogador.nomejogador[strlen(novo_jogador.nomejogador)-1] = '\0';

            // salvar os outros dados do jogador
            novo_jogador.identificador = id;
            novo_jogador.totalpts = 0;
            novo_jogador.ultimafase = 1;
            novo_jogador.vidas = 3;


            // escrever os dados no arquivo binario
            if(fwrite(&novo_jogador,sizeof(save), 1, arq) != 1)
                printf("Erro na escrita do save!\n");
        }
    }
    fclose(arq);
    return 0;

}

// Função para carregar um jogo salvo
void carregarJogo()
{
    save buffer;
    FILE *arq;
    int id;
    // abre o arquivo dos saves
    if(!(arq = fopen("saves.bin", "r+b")))
        printf("Erro na abertura do arquivo!\n");
    else {
        // imprime na tela os dados
        imprime_saves(arq);
        // solicita o jogo que o usuario deseja acessar
        printf("Digite o id que deseja acessar: \n");
        scanf("%d", &id);
        fseek(arq, sizeof(save)*id, SEEK_SET);
        if(fread(&buffer, sizeof(save), 1, arq) != 1)
            printf("Erro na abertura do arquivo!\n");
    }
    fclose(arq);
}

// dado o arquivo saves.bin, com as gravações de todos os jogadores, imprime na tela as
// informações dos jogadores, com formatação adequada
void imprime_saves(FILE* arq){
    save jogador;
    rewind(arq);
    // enquanto não chegar no final do arquivo
    while(!feof(arq))
    {       // lê o arquivo
            if(fread(&jogador, sizeof(save), 1, arq) == 1)
                {
                // e imprime os saves
                printf("\nJogador %d\n", jogador.identificador);
                printf("Nome: %s\n", jogador.nomejogador);
                printf("Pontos: %d\n", jogador.totalpts);
                printf("Fase: %d\n", jogador.ultimafase);
                printf("Vidas: %d\n", jogador.vidas);
                }
    }
}

fase gera_fase(int numerofase)
{
    FILE* arq;
    int linha, coluna;
    fase fasea;
    char nome[10];

    switch (numerofase)
    {
        case 1: strncpy(nome, "Fase1.txt", 10);
                break;
        case 2: strncpy(nome, "Fase2.txt", 10);
                break;
        case 3: strncpy(nome, "Fase3.txt", 10);
                break;
    }

    arq = fopen(nome, "r");
    if (arq == NULL)
        perror("Erro ao abrir o arquivo");
    else
    {
        fasea.tamanhox = 14;
        fasea.tamanhoy = 14;
        for (linha = 0; linha < fasea.tamanhox; linha++){
            for (coluna = 0; coluna < fasea.tamanhoy; coluna++){
                fasea.elementos[linha][coluna] = getc(arq);
        }
    }
    fclose(arq);
    return fasea;
    }
}


void imprime_mapa(fase fasea)
{
    int linha, coluna;

    for (linha = 0; linha < fasea.tamanhoy; linha++){
        for (coluna = 0; coluna < fasea.tamanhox; coluna++){
            printf("%c", fasea.elementos[linha][coluna]);
        }
    }
}

void movimentacao(fase fasea)
{
    char caracter, lolo = '@';
    int x, y;
    int linha, coluna;
    int oldX, oldY;

    hidecursor();
    for (linha = 0; linha < fasea.tamanhoy; linha++){
        for (coluna = 0; coluna < fasea.tamanhox; coluna++){
            if (fasea.elementos[linha][coluna] == '@')
            {
                x = coluna;
                y = linha;
            }
        }
    }
    oldX = x;
    oldY = y;
    hidecursor();
    gotoxy(x, y);
    while (caracter != ESC)
    {
        gotoxy(oldX, oldY);
        printf(" ");
        gotoxy(x, y);
        printf("%c", lolo);
        caracter = getch();
        oldX = x;
        oldY = y;
        switch (caracter)
        {
            case S_CIMA:  contato_lolo(S_CIMA, &x, &y);
                          break;
            case S_BAIXO: contato_lolo(S_BAIXO, &x, &y);
                          break;
            case S_ESQ:   contato_lolo(S_ESQ, &x, &y);
                          break;
            case S_DIR:   contato_lolo(S_DIR, &x, &y);
                          break;
        }
    }
    gotoxy(13,13); // so pra nao retornar a mensagem q finalizou em cima da fase
}

void contato_lolo(int seta, int *x, int *y)
{
    int novoX = *x, novoY = *y;
    char caracter;

    switch(seta)
    {
        case S_CIMA:  novoY--;
                      break;
        case S_BAIXO: novoY++;
                      break;
        case S_ESQ:   novoX--;
                      break;
        case S_DIR:   novoX++;
                      break;
    }
    gotoxy(novoX, novoY);
//    caracter = getchar();
//    gotoxy(30, 10);
//    printf("%c", caracter);
//    switch (caracter)
//    {
//        case 'P':
    *x = novoX;
//
    *y = novoY;
//    }
}




