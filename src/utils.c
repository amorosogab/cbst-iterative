#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"

/*	DESCRIZIONE
	Genera un numero casuale appartenente a [a, b].

	PARAMETRI DI INPUT
	int a: Estremo inferiore.
	int b: Estremo superiore.

	VALORI DI RITORNO 
	Restituisce un numero casuale compreso tra a e b estremi inclusi.	*/
int rand_num(int a, int b)
{
	return a + (rand() % (b-a+1));
}

/*	DESCRIZIONE
	Restituisce il resto della divisione di x per m. Tratta anche in caso in cui x è minore di 0.	*/
int mod_i(int x, int m)
{
	return (x%m + m) % m;
}

/* 	DESCRIZIONE
	Modulo di un numero double. 	*/
double mod_d(double x, int m)
{
	return fmod(fmod(x, m) + m, m);
}
