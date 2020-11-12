/*
  Programa: limpa.c
  Autor: Liviu Gabriel Anghel
  Objetivo: limpar tela
	    Aprender usar clrscr();
  Data: 14/05/2002
*/

/*	Bibliotecas usadas pelo programa */
#include <stdio.h>
#include <conio2.h>

void main (void)
{
	char s[80];

	puts("Entre com uma frase");
	gets(s);
	puts("Aperta qualquer tecla para limpar a tela");
	getchar();
	clrscr();
	puts("Parabens!");
	getchar();

}
