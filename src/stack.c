#include <stdlib.h>

#include "stack.h"

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di creare un nuovo nodo di tipo stackpointer, contenente
	come chiave il parametro dato in input e di restituire il nuovo nodo creato in output.

	PARAMETRI DI INPUT
	void *key: 	Nodo chiave da creare.
	
	VALORE DI RITORNO 	
	Restituisce il nuovo nodo creato, altrimenti restituisce NULL in caso di errori. 			*/
StackPointer_t create_stk_node(void *key)
{
	StackPointer_t new;

	new = (StackPointer_t) malloc(sizeof(struct stk_node));
	if(new == NULL /*|| key == NULL*/)
	{
		/*free(new);*/
		return NULL;
	}
	new->key = key;
	new->next = NULL;

	return new;
}

/* 	DESCRIZIONE 
	Estrae un elemento dalla testa dello stack.

	PARAMETRI DI INPUT
	StackPointer_t stk: 				Stack da cui estrarre la testa.
	void (*Destroy_key)(void *dato): 	Puntatore a funzione utilizzabile nel caso in cui il campo key dello stack contenga un dato
										non primitivo, il che significa che ha bisogno di una propria funzione di deallocazione, oppure
										nel caso in cui si voglia delegare alla funzione di pop la distruzione del dato.
	
	VALORE DI RITORNO 
	Restituisce il nuovo stack. 	*/
StackPointer_t pop(StackPointer_t stk, void (*Destroy_key)(void *dato))
{
	if(!is_empty(stk))
	{
		StackPointer_t tmp = stk;
		stk = stk->next;
		if(Destroy_key != NULL)
			Destroy_key(tmp->key);
		free(tmp);
	}

	return stk;
}

/* 	DESCRIZIONE 
	Effettua una push del nodo 'node' sullo stack 'stk'.

	PARAMETRI DI INPUT
	StackPointer_t stk: 	Stack.
	void *k: 				Chiave da inserire.
	void *(*Dup_dato)(const void *dato): 	Puntatore a funzione utilizzabile nel caso in cui si voglia che lo stack
											mantenga una copia del dato di cui fa la push in modo tale che se in qualsiasi
											punto del programma, il dato viene deallocato, il campo key dello stack non punterà
											a NULL.
	
	VALORE DI RITORNO
	Restituisce lo stack con il nuovo nodo in testa. 	*/
StackPointer_t push(StackPointer_t stk, void *k, void *(*Dup_dato)(const void *dato))
{
	StackPointer_t node = NULL;

	if(Dup_dato != NULL)
		node = create_stk_node(Dup_dato(k));
	else
		node = create_stk_node(k);

	if(node != NULL)
	{
		node->next = stk;
		stk = node;
	}

	return stk;
}

/* 	DESCRIZIONE
	Distugge l'intero stack dato in input, deallocando, se richiesto, anche le chiavi associate ad
	ogni nodo.

	PARAMETRI DI INPUT
	StackPointer_t stk: 				Stack.
	void (*Destroy_key)(void *dato):  	Puntatore a funzione da utilizzare nel caso in cui venga richiesto
	 									di deallocare anche le chiavi associate ai nodi. Se NULL, non deallocazione
										le chiavi. 																	*/
void destroy_stk(StackPointer_t stk, void (*Destroy_key)(void *dato))
{
	while(!is_empty(stk))
		stk = pop(stk, Destroy_key);
}

/* 	DESCRIZIONE
	Restituisce la chiave della cima dello stack. 		*/
void *top(StackPointer_t stk)
{
	if(stk)
		return stk->key;
	else
		return NULL;
}

/* 	DESCRIZIONE 
	Verifica se lo stack dato in ingresso è vuoto oppure no.
	
	PARAMETRI DI INPUT
	StackPointer_t stk: Stack.
	
	VALORE DI RITORNO 	
	Restituisce 1 se lo stack è vuoto, 0 altrimenti. 		*/
int is_empty(StackPointer_t stk)
{
	if(!stk)
		return 1;
	else
		return 0;
}
