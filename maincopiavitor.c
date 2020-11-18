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
    char elementos[13][14];
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


void menu(); // funcao para o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
void novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void movimentacao(char, int, int); // funcao para a movimentacao, dado o lolo e seu x e y
void hidecursor(); // funcao pra esconder o cursor
void le_mapa(int); // funcao que imprime o mapa na tela dado o numero daquela fase

int main()
{
    char lolo = '@';
    le_mapa(2);
    return 0;
}

void menu()
{
    printf("(N) - Novo Jogo\n(C) - Carregar Jogo\n(M) - Mostrar Creditos\n(S) - Sair\n");
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
            menu(); // imprime o menu para o usuario novamente
        }
    }
    while ((opcao != 'N') && (opcao != 'C') && (opcao != 'M') && (opcao != 'S')); // enquanto nao for um dos caracteres validos
    return opcao;
}

void novoJogo()
{
    char nome [13];

    printf("Digite seu nome (maximo de 10 caracteres): ");
    fflush(stdin);
    fgets(nome, 12, stdin);
    nome[strlen(nome)-1] = '\0';

}

void carregarJogo()
{
    printf("Carregando a lista de jogos salvos, aguarde um minuto.\n");
}

void mostraCreditos()
{
    printf("Creditos do jogo:\nDesenvolvido por: Vitor Caruso Rodrigues Ferrer (00327023)\n");
}

void sair()
{
    printf("Encerrando o jogo, ateh a proxima!\n");
}

void hidecursor() // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void movimentacao(char lolo, int x, int y)
{
    char caracter;
    int oldX = x, oldY = y;

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
            case S_CIMA:  if (y != 2) // limite da borda (ainda precisa colocar uma funcao aqui p testar os blocos)
                            y--;
                          break;
            case S_BAIXO: if (y != 12)
                            y++;
                          break;
            case S_ESQ:   if (x != 2)
                            x--;
                          break;
            case S_DIR:   if (x != 12)
                            x++;
                          break;
        }
    }
    gotoxy(13,13); // so pra nao retornar a mensagem q finalizou em cima da fase
}

void le_mapa(int numerofase)
{
    FILE *arq;
    int x, y;
    char letra, lolo, nome[10];

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
        while (!feof(arq))
        {
             letra = getc(arq);
             printf("%c", letra);
             if (letra == '@')
             {
                lolo = letra;
                x = wherex()-1; // -1 porque o cursor fica depois do ultimo caracter lido
                y = wherey();
             }
        }
        movimentacao(lolo, x, y);
    }
    fclose(arq);
}




