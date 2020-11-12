/*
  Programa: troca.c
  Autor: Liviu Gabriel Anghel
  Objetivo: Fazer o cursor andar na tela
	    Aprender usar 'switch' e 'gotoxy'
  Data: 14/05/2002
*/

/*	Bibliotecas usadas pelo programa */
#include <stdio.h>
#include <conio2.h>
#define CIMA 72
#define BAIXO 80
#define ESQ 77
#define DIR 75

void main ( )
{
    int i,x,y;
    char c;
    //_setcursortype(_NOCURSOR);
    clrscr();
    x=15;
    y=10;
    gotoxy(x,y);
    printf("*");

    do
    {
        c = getch();
        if (c == 0)
        {
            c=getch();

            switch (c)
            {
                case CIMA:
                    gotoxy(x,y);
                    printf(" ");
                    if(y != 1) y--;
                    gotoxy(x,y);
                    printf("*");
                    break;

                case BAIXO:
                    gotoxy(x,y);
                    printf(" ");
                    if (y != 24) y++;
                    gotoxy(x,y);
                    printf("*");
                    break;

                case ESQ:
                    gotoxy(x,y);
                    printf(" ");
                    if (x != 80) x++;
                    gotoxy(x,y);
                    printf("*");
                    break;

                case DIR:
                    gotoxy(x,y);
                    printf(" ");
                    if (x != 1) x--;
                    gotoxy(x,y);
                    printf("*");
                    break;
            }
        }
    }
    while(c !='a');
}
