#include <conio.h>
#include <stdio.h>

// Le tecla
int getInput(char *c)
{
    if (kbhit())
    {
         *c = getch();
         *c = getch();
         return 1; // Foi pressionada uma tecla
    }
	return 0; // Nao foi pressioanda uma tecla
}

// Imprime sem parar um caracter em posiçoes diferentes da tela e
// Quando le um caracter, exibe na tela
int main( void )
{
     int x = 40;
     int y = 13;
	 char key = '#';

     while (1)
     {

       system("cls");

       x--;

       gotoxy(x,y);
       printf("%c",key);

       if (x == 0)
        x = 40;

       getInput(&key);

     }

    return 0;
}
