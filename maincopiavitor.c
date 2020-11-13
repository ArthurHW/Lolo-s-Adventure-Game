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

void menu(); // funcao para o menu do jogo
char validaentrada(); // funcao para validar a entrada da opcao do jogo
void novoJogo(); // funcao para comecar um novo jogo
void carregarJogo(); // funcao para carregar um jogo ja comecado
void mostraCreditos(); // funcao para mostrar os creditos
void sair(); // funcao para mostrar a mensagem de saida do jogo
void teste_movimentacao(); // funcao para teste da movimentacao para ficar mais facil de chamar
void hidecursor(); // funcao pra esconder o cursor

int main()
{
    teste_movimentacao();
    return 0;
}

void hidecursor() // https://stackoverflow.com/questions/30126490/how-to-hide-console-cursor-in-c
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void teste_movimentacao()
{
    char lolo = '@', caracter;
    int x = 2, y = 2;
    int fase[13][13], l, c;
    int oldX = x, oldY = y;

    for (l = 0; l < 13; l++) // imprimir os limites 13x13 p ter uma nocao
    {
        printf("P");
        for (c = 1; c < 13; c++)
        {
            if (l == 0 || l == 12)
            {
                printf("P");
            }
            else
            {
                if (c == 12)
                    printf("P");
                else
                    printf(" ");
            }
        }
        printf("\n");
    }
//    _setcursortype(_NOCURSOR);
    hidecursor();
    gotoxy(x, y);
    do
    {
        gotoxy(oldX, oldY);
        cprintf(" ");
        gotoxy(x, y);
        printf("%c", lolo);
        caracter = getch();
        oldX = x;
        oldY = y;
        switch (caracter)
        {
            case S_CIMA:  if (y != 2) // limite
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
    while(caracter != ESC);
    gotoxy(13,13); // so pra nao retornar a mensagem q finalizou em cima da matriz
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

