/*
  Programa: quadrado.c
  Autor: Liviu Gabriel Anghel
  Objetivo: Criar uma matriz colorida
	    Aprender usar cores
  Data: 14/05/2002
*/

/*	Bibliotecas usadas pelo programa */
#include <conio2.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>


int main (void){

	int i,j;
	clrscr();

	for (i = 20; i < 50; i++)
		for (j = 5; j < 20; j++){
			textcolor(rand() % 15 + 1);
			gotoxy(i,j);
			cprintf("*");

		}
	getche();
	gotoxy(20,5);
	for (i = 0; i < 15; i++){
		delline();
		//delay(100);
	}

	return 0;
}
