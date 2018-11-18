#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "types.h"
#include "utils.h"

/* 	DESCRIZIONE
	Confronta due stringe date in input.

	PARAMETRI DI INPUT
	const void *str1, const void *str2: 	Strighe da confrontare.
	
	VALORE DI RITORNO
	Restituisce -1 se in senso lessicografico str1 compare prima di str2, restituisce
	0 se str1 e str2 sono uguali, altrimenti restituisce 1 								*/
int string_cmp(const void *str1, const void *str2)
{
	if(strcmp((const char *) str1, (const char *) str2) < 0)
		return -1;
	else if(strcmp((const char *) str1, (const char *) str2) > 0)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Confronta due interi.

	PARAMETRI DI INPUT
	const void *a, const void *b: 	Interi da confrontare.
	
	VALORE DI RITORNO
	Restituisce -1 se a < b, 0 se a = b oppure 1 se a > b. 					*/
int int_cmp(const void *a, const void *b)
{
	if(*((int *) a) < *((int *) b))
		return -1;
	else if(*((int *) a) > *((int *) b))
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Confronta due numeri reali.

	PARAMETRI DI INPUT
	const void *a, const void *b: 	Reali da confrontare.
	
	VALORE DI RITORNO
	Restituisce -1 se a < b, 0 se a = b oppure 1 se a > b. 					*/
int double_cmp(const void *a, const void *b)
{
	if(*((double *) a) < *((double *) b))
		return -1;
	else if(*((double *) a) > *((double *) b))
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Confronta due punti.

	PARAMETRI DI INPUT
	const void *a, const void *b: 	Punti da confrontare.
	
	VALORE DI RITORNO
	Restituisce -1 se a < b, 0 se a = b oppure 1 se a > b. 					*/
int points_cmp(const void *a, const void *b)
{
	struct point *p1 = (struct point *) a;
	struct point *p2 = (struct point *) b;
	int mod_p1 = sqrt(pow(p1->x, 2) + pow(p1->y, 2));
	int mod_p2 = sqrt(pow(p2->x, 2) + pow(p2->y, 2));

	return int_cmp(&mod_p1, &mod_p2);
}

/* 	DESCRIZIONE
	Duplica l'intero dato in input.

	PARAMETRI DI INPUT
	const void *dato: 	Intero da duplicare.
	
	VALORI DI RITORNO
	Restituisce un puntatore all'intero duplicato. Restituisce NULL in caso di errori. 		*/
void *dup_int(const void *dato)
{
	int *p = NULL;

	if((p = (int *) malloc(sizeof(int))) == NULL)
		return NULL;
	*p = *((int *) dato);

	return p;
}

/* 	DESCRIZIONE
	Duplica un double dato in input.

	PARAMETRI DI INPUT
	const void *dato: 	Double da duplicare.
	
	VALORI DI RITORNO
	Restituisce un puntatore al numero duplicato, altrimenti restituisce NULL in caso di errori. 		*/
void *dup_double(const void *dato)
{
	double *p = NULL;

	if((p = (double *) malloc(sizeof(double))) == NULL)
		return NULL;
	*p = *((double *) dato);

	return p;
}

/* 	DESCRIZIONE
	Duplica la stringa data in input.

	PARAMETRI DI INPUT
	const void *dato: 	Stringa da duplicare.
	
	VALORI DI RITORNO
	Restituisce un puntatore alla stringa duplicata, altrimenti restituisce NULL in caso di errori. 		*/
void *dup_string(const void *dato)
{
	char *p;

	if((p = (char *) malloc(sizeof(char) * (strlen(((char *) dato)) + 1))) == NULL)
		return NULL;
	strcpy(p, ((char *) dato));

	return p;
}

/* 	DESCRIZIONE
	Duplica il Punto dato in input.

	PARAMETRI DI INPUT
	const void *dato: 	Punto da duplicare.
	
	VALORI DI RITORNO
	Restituisce un puntatore al punto duplicato. Restituisce NULL in caso di errori. 		*/
void *dup_point(const void *dato)
{
	struct point *dst, *src = (struct point *) dato;

	if((dst = (struct point *) malloc(sizeof(struct point))) == NULL)
		return NULL;
	dst->x = src->x;
	dst->y = src->y;

	return dst;
}

/* 	DESCRIZIONE
	Genera un intero casuale, compreso tra MIN_INT e MAX_INT. */
void *rand_int(void)
{
	int p = rand_num(MIN_INT, MAX_INT); 
	return dup_int(&p);
}

/* 	DESCRIZIONE
 	Genera un numero reale casuale. 				*/
void *rand_double(void)
{
	double p = (double) rand() / ((double) RAND_MAX) + rand_num(MIN_INT, MAX_INT);
	return dup_double(&p);
}

/*	DESCRIZIONE
	Genera una stringa arbitraria di lunghezza arbitraria.

	VALORI DI RITORNO
	Restituisce una stringa casualmente generata, di lunghezza arbitraria. 			*/
void *rand_string(void)
{
	int n, i;
	char *str;

	n = rand_num(2, MAX_STR_LEN);
	if((str = (char *) malloc(n * sizeof(char) + 1)) == NULL)
		return NULL;

	for(i = 0; i < n; i++)
		str[i] = rand_num(MIN_CH, MAX_CH);
	str[n] = '\0';

	return str;
}

/* 	DESCRIZIONE
	Genera un punto arbitrario.

	VALORE DI RITORNO
	Restituisce uno stract point. Restituisce NULL nel caso in cui la funzione non riesce ad
	allocare memoria. 																*/
void *rand_point(void)
{
	struct point *p;

	if((p = (struct point *) malloc(sizeof(struct point))) == NULL)
		return NULL;
	p->x = rand_num(MIN_INT, MAX_INT);
	p->y = rand_num(MIN_INT, MAX_INT);

	return p;
}

/* 	DESCRIZIONE
	Verifica se un intero è pari o dispari.

	VALORE DI RITORNO
	Restituisce 1 nel caso in cui l'intero sia pari, altrimenti restituisce 0. 			*/
int is_even_int(const void *a)
{
	return !mod_i(*((int *) a), 2);
}

/* 	DESCRIZIONE
	Verifica se un double è pari o dispari.

	VALORE DI RITORNO
	Restituisce 1 nel caso in cui il numero sia pari, altrimenti restituisce 0. 			*/
int is_even_double(const void *a)
{
	return !mod_d(*((double *) a), 2);
}

/* 	DESCRIZIONE
	Verifica se la lunghezza di una stringa è pari o dispari.

	VALORE DI RITORNO
	Restituisce 1 nel caso in cui a lunghezza della stringa sia pari, altrimenti restituisce 0. 	*/
int is_even_string(const void *a)
{
	return !mod_i(strlen(((char *) a)), 2);
}

/* 	DESCRIZIONE
	Verifica se un punto è pari o dispari. Un punto è pari se il suo modulo è pari.

	VALORE DI RITORNO
	Restituisce 1 nel caso in cui il punto sia pari, altrimenti restituisce 0. 			*/
int is_even_point(const void *a)
{
	struct point *p = (struct point *) a;
	int mod_p = sqrt(pow(p->x, 2) + pow(p->y, 2));

	return !mod_i(mod_p, 2);
}

/* 	DESCRIZIONE
	Immagazzina un intero nella posizione i-esima dell'array a.

	PARAMETRI DI INPUT
	void *a[]: 			Array di puntatori.
	const void *data: 	Valore da immagazzinare nella posizione i-esime dell'array.
	int i: 				Indice di a.

	VALORE DI RITORNO
	Restituisce 1 se l'operazione è andata a buon fine, -1 altrimenti. 				*/
int int_store_at(void *a[], const void *elem, int i)
{
	int **b = (int **) a;
	b[i] = dup_int(elem);
	if(b[i] == NULL)
		return -1;
	
	return 1;
}

/* 	DESCRIZIONE
	Immagazzina un double nella posizione i-esima dell'array a.

	PARAMETRI DI INPUT
	void *a[]: 			Array di puntatori.
	const void *data: 	Valore da immagazzinare nella posizione i-esime dell'array.
	int i: 				Indice di a.

	VALORE DI RITORNO
	Restituisce 1 se l'operazione è andata a buon fine, -1 altrimenti. 				*/
int double_store_at(void *a[], const void *elem, int i)
{
	double **b = (double **) a;
	b[i] = dup_double(elem);
	if(b[i] == NULL)
		return -1;
	
	return 1;
}

/* 	DESCRIZIONE
	Immagazzina una stringa nella posizione i-esima dell'array a.

	PARAMETRI DI INPUT
	void *a[]: 			Array di puntatori.
	const void *data: 	Stringa da immagazzinare nella posizione i-esime dell'array.
	int i: 				Indice di a.

	VALORE DI RITORNO
	Restituisce 1 se l'operazione è andata a buon fine, -1 altrimenti. 				*/
int string_store_at(void *a[], const void *elem, int i)
{
	char **b = (char **) a;
	b[i] = dup_string(elem);
	if(b[i] == NULL)
		return -1;

	return 1;
}

/* 	DESCRIZIONE
	Immagazzina un punto nella posizione i-esima dell'array a.

	PARAMETRI DI INPUT
	void *a[]: 			Array di puntatori.
	const void *data: 	Valore da immagazzinare nella posizione i-esime dell'array.
	int i: 				Indice di a.

	VALORE DI RITORNO
	Restituisce 1 se l'operazione è andata a buon fine, -1 altrimenti. 				*/
int point_store_at(void *a[], const void *elem, int i)
{
	struct point **b = (struct point **) a;
	b[i] = dup_point(elem);
	if(b[i] == NULL)
		return -1;

	return 1;
}

/* 	DESCRIZIONE
	Funzione di stampa di un intero. 		*/
void print_key_int(const void *key)
{
	printf("[%d] ", *((int *) key));
}

/* 	DESCRIZIONE
	Funzione di stampa di un double. 		*/
void print_key_double(const void *key)
{
	printf("[%lf] ", *((double *) key));
}

/* 	DESCRIZIONE
	Funzione di stampa di una stringa. 		*/
void print_key_string(const void *key)
{
	printf("[%s] ", ((char *) key));
}

/* 	DESCRIZIONE
	Funzione di stampa di un punto. 		*/
void print_key_point(const void *key)
{
	struct point *p = (struct point *) key;
	printf("[(%d, %d)] ", p->x, p->y);
}

/* 	DESCRIZIONE
	Funzione che si occupa della creazione e dell'inizializzazione della struttura delle operazioni.
	Tale funzione dovrebbe sempre essere chiamata all'inizio del programma.
	
	PARAMETRI DI INPUT
	int (*Cmp_function)(const void *elem1, const void *elem2): 	Funzione di comparazione di due elementi.
	void *(*Dup_dato)(const void *dato): 						Funzione di duplicazione di un dato.
	void *(*Rand_function)(void): 								Funzione che genera casualmente un tipo di dato.
	int (*Is_even)(const void *elem): 							Funzione che controlla se un valore è pari o dispari.
	int (*Store_at)(void *a[], const void *elem, int i): 		Funzione che immagazzina un dato valore nella posizione i-esima di un array.
	void (*Destroy_key)(void *key): 							Funzione che dealloca una chiave.
	void (*Print_key)(const void *key): 						Funzione di stampa.
	void *(*Get_input_key)(void)): 								Funzione di input.
	
	VALORE DI RITORNO 
	Tale funzione restituisce la struttura delle operazioni solo se tutti i puntatori a funzione risulatano diversi da NULL. 	*/
struct operations *initialize(int (*Cmp_function)(const void *elem1, const void *elem2),
							  void *(*Dup_dato)(const void *dato),
							  void *(*Rand_function)(void),
							  int (*Is_even)(const void *elem),
	   						  int (*Store_at)(void *a[], const void *elem, int i),
							  void (*Destroy_key)(void *key),
	 						  void (*Print_key)(const void *key),
							  void *(*Get_input_key)(void))
{
	if(Cmp_function == NULL || Dup_dato == NULL || Rand_function == NULL || Is_even == NULL || 
	   Store_at == NULL || Destroy_key == NULL || Print_key == NULL || Get_input_key == NULL)
	   	return NULL;

	struct operations *op = (struct operations *) malloc(sizeof(struct operations));
	if(op == NULL)
		return NULL;

	op->Cmp_function = Cmp_function;
	op->Dup_dato = Dup_dato;
	op->Rand_function = Rand_function;
	op->Is_even = Is_even;
	op->Store_at = Store_at;
	op->Destroy_key = Destroy_key;
	op->Print_key = Print_key;
	op->Get_input_key = Get_input_key;

	return op;
}

/*  DESCRIZIONE
 	Funzione di input di un intero. 		*/
void *get_input_key_int(void)
{
	char *str = get_input_key_string();
	if(str == NULL)
		return NULL;

	int *num = (int *) malloc(sizeof(int));
	if(num == NULL)
		return NULL;
	
	*num = atoi(str);
	free(str);

	return num;
}

/*  DESCRIZIONE
 	Funzione di input di una stringa. 		*/
void *get_input_key_string(void)
{
	char *str = NULL;
	int i = 0;

	if((str = (char *) malloc(sizeof(char) * MAX_STR_LEN)) == NULL)
		return NULL;

	while(i < MAX_STR_LEN && (str[i] = getchar()) != '\n' && str[i] != ' ' && str[i] >= MIN_CH && str[i] <= MAX_CH)
		i++;
	
	if(str[i-1] < MIN_CH || str[i-1] > MAX_CH)
	{
		free(str);
		return NULL;
	}

	str[i] = '\0';
	return str;
}

/*  DESCRIZIONE
 	Funzione di input di un double. 		*/
void *get_input_key_double(void)
{
	char *str = get_input_key_string();
	if(str == NULL)
		return NULL;

	double *num = (double *) malloc(sizeof(double));
	if(num == NULL)
		return NULL;
	
	*num = atof(str);
	free(str);
	return num;
}

/*  DESCRIZIONE
 	Funzione di input di un punto. 		*/
void *get_input_key_point(void)
{
	struct point *p;

	if((p = (struct point *) malloc(sizeof(struct point))) == NULL)
		return NULL;

	printf("x: ");
	int *x = (int *) get_input_key_int();
	printf("y: ");
	int *y = (int *) get_input_key_int();
	p->x = *x;
	p->y = *y;
	free(x);
	free(y);

	return p;
}
