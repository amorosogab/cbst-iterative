#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abr.h"
#include "stack.h"
#include "utils.h"

/* DESCRIZIONE
   Ricerca del nodo contenente la chiave k.

   PARAMETRI DI INPUT
   Abr_t root: 	   			Radice dell'albero binario di ricerca o di un qualsiasi sottoalbero.
   const void *k:  			chiave.
   struct operations *op: 	struttura delle operationi.

   VALORI DI RITORNO
   abr_search restituisce il nodo contenente la chiave k se tale nodo esiste,
   NULL altrimenti.								*/
Abr_t abr_search(Abr_t root, const void *k, struct operations *op)
{	
	while(root && op->Cmp_function(root->key, k))
		root = op->Cmp_function(root->key, k) > 0 ? root->left : root->right;
	
	return root;
}


/* DESCRIZIONE
   Inserimento di un nodo nell'albero binario di ricerca dato.

   PARAMETRI DI INPUT
   Abr_t root: 				Radice dell'albero.
   const void *key: 		chiave del nodo da inserire nell'albero.
   struct operations *op: 	Struttura delle operazioni.

   VALORI DI RITORNO
   Restituisce la radice del nuovo albero binario di ricerca.	*/
Abr_t abr_insert(Abr_t root, const void *k, struct operations *op)
{
	Abr_t curr = root, padre = NULL;
	while(curr && op->Cmp_function(curr->key, k))
		padre = curr, curr = op->Cmp_function(curr->key, k) > 0 ? curr->left : curr->right;

	if(curr == NULL)
	{
		Abr_t new = create_abr_node(k, op);
		if(padre == NULL)
			root = new;
		else
		{
			if(op->Cmp_function(padre->key, k) > 0)
				padre->left = new;
			else
				padre->right = new;
		}
	}

	return root;
}

/* 	DESCRIZIONE
	Genera un albero binario di ricerca contenente n nodi casualmente generati.

	PARAMETRI DI INPUT
	int n: 					Numero di nodi da generare.
	struct operations *op: 	Struttura delle operazioni.
	
	VALORE DI RITORNO
	Restituisce un abr con n nodi. 							*/
Abr_t gen_abr(int n, struct operations *op)
{
	Abr_t new_abr;
	int i;
	void *key = NULL;

	for(i = 1, new_abr = NULL; i <= n && (key = op->Rand_function()); i++)
	{
		if(abr_search(new_abr, key, op) == NULL)
		{
			new_abr = abr_insert(new_abr, key, op);
			op->Destroy_key(key);
		}
		else
			continue; /* non incrementa i */
	}
	
	return new_abr;
}

/*	DESCRIZIONE
	Confronta due abr dati in input.

	VALORE DI RITORNO
	Restituisce 1 se i due abr hanno le stesse chiavi e la stessa forma, 0 altrimenti.*/
int abr_equal(Abr_t abr1, Abr_t abr2, struct operations *op)
{
	StackPointer_t stk_abr1 = NULL, stk_abr2 = NULL;
	short equal;

	while((stk_abr1 && stk_abr2) || (abr1 && abr2))
	{
		if(abr1) /* anche abr2 è diverso da null */
		{
			if(!(equal = op->Cmp_function(abr1->key, abr2->key) == 0))
				break;
			stk_abr1 = push(stk_abr1, abr1->right, NULL);
			stk_abr2 = push(stk_abr2, abr2->right, NULL);
			abr1 = abr1->left;
			abr2 = abr2->left;
		}
		else
		{
			abr1 = top(stk_abr1);
			stk_abr1 = pop(stk_abr1, NULL);
			abr2 = top(stk_abr2);
			stk_abr2 = pop(stk_abr2, NULL);
		}
	}

	return equal;
}

/* DESCRIZIONE
   Crea un nuovo nodo che ha come chiave la stringa str data in input. La funzione inizializza
   opportunamente i puntatori left e right del nodo appena creato.
   
   PARAMETRI DI INPUT
   const void *str: 		Chiave del nodo da creare.
   struct operations *op: 	Struttura delle operazioni.

   VALORI DI RITORNO
   Nuovo nodo contenente come chiave la stringa str se quest'ultima è diversa da NULL, altrimenti,
   la funzione restituisce NULL.			*/
Abr_t create_abr_node(const void *key, struct operations *op)
{
	Abr_t newnode = NULL;

	if(key == NULL)
		return NULL;

	newnode = (Abr_t) malloc(sizeof(struct abr_node));
	if(newnode == NULL)
		NULL;
	newnode->key = op->Dup_dato(key);
	newnode->left = NULL;
	newnode->right = NULL;

	return newnode;
}

/* 	DESCRIZIONE
	Duplica un abr dato in ingresso.

	PARAMETRI DI INPUT
	Abr_t a: 				Abr da duplicare;
	struct operations *op: 	Struttura delle operazioni.
	
	VALORE DI RITORNO
	Restituisce una copia esatta dell'albero binario a. 	*/
Abr_t dup_abr(Abr_t a, struct operations *op)
{
	StackPointer_t stk_abr1 = NULL, stk_abr2 = NULL;
	Abr_t dup = NULL, root_dup = NULL;

	if(a && (root_dup = create_abr_node(a->key, op)) == NULL)
		return NULL;

	dup = root_dup;
	while(stk_abr1 || a)
	{
		if(a)
		{
			if((a->left && (dup->left = create_abr_node(a->left->key, op)) == NULL) ||
			   (a->right && (dup->right = create_abr_node(a->right->key, op)) == NULL))
				break;
				
			stk_abr1 = push(stk_abr1, a->right, NULL); 	
			stk_abr2 = push(stk_abr2, dup->right, NULL);

			a = a->left;
			dup = dup->left;
		}
		else
		{
			a = top(stk_abr1);
			stk_abr1 = pop(stk_abr1, NULL);
			dup = top(stk_abr2);
			stk_abr2 = pop(stk_abr2, NULL);
		}
	}

	destroy_stk(stk_abr1, NULL);
	destroy_stk(stk_abr2, NULL);
	return root_dup;
}

/* DESCRIZIONE
   Cancella il nodo contenente la chiave key dall'albero binario di ricerca.

   PARAMETRI DI INPUT
   Abr_t root:  			Radice dell'albero
   void *key: 				Chiave contenuta nel nodo da eliminare.
   struct operations *op: 	Struttura delle operazioni.

   VALORI DI RITORNO
   Restituisce la radice del nuovo albero di ricerca. 					*/
Abr_t abr_del_node(Abr_t root, void *key, struct operations *op)
{
	Abr_t p, z, x, y;

	p = NULL;
	z = root;
	while(z && op->Cmp_function(key, z->key) != 0) /* ricerca di key */
	{
		p = z;
		if(op->Cmp_function(z->key, key) > 0)
			z = z->left;
		else
			z = z->right;
	}
	
	if(z == NULL) 	/* nulla da cancellare */
		return root;
	if(z->left == NULL || z->right == NULL)
		y = z;
	else 			/* z ha due figli, si cerca quindi il successore */
	{
		y = z->right;
		p = z;
		while(y->left)
		{
			p = y;
			y = y->left;
		}
	}

	if(y->left)
		x = y->left;
	else
		x = y->right;
	
	if(p == NULL) 	/* si sta cancellando la radice */
		root = x;
	else if(y == p->left)
		p->left = x;
	else
		p->right = x;

	if(y != z) 	/* z ha due figli */
	{
		op->Destroy_key(z->key);
		z->key = op->Dup_dato(y->key);
	}

	destroy_node(y, op);
	return root;
}

/* 	DESCRIZIONE
	Dato un albero binario di ricerca, elimina tutti i nodi compresi tra a e b (estremi inclusi) e che
	siano pari o dispari in base al parametro p.

	PARAMETRI DI INPUT
	Abr_t root: 			Radice abr.
	const char *a: 			Estremo inferiore.
	const char *b 			Estremi superiore.
	int p: 					Parità. Se p >= 1, si vogliono eliminare le stringhe pari, altrimenti quelle dispari.
	struct operations *op: 	Struttura delle operazioni.
	
	VALORE DI RITORNO
	Restituisce un albero binario di ricerca. 									*/
Abr_t bet(Abr_t root, const void *a, const void *b, const int p, struct operations *op)
{
	StackPointer_t stk = NULL, stk_node_to_del = NULL;
	Abr_t curr = root, last = NULL, to_del = NULL;

	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			if(op->Cmp_function(curr->key, b) > 0)
				curr = curr->left;
			else if(op->Cmp_function(curr->key, a) < 0)
				curr = curr->right;
			else
			{
				if((p > 0 && op->Is_even(curr->key)) || (p == 0 && !op->Is_even(curr->key)))
					stk_node_to_del = push(stk_node_to_del, curr, NULL);
				curr = curr->left;
			}
			
		}
		else
		{
			curr = top(stk);
			if(curr->right == NULL || last == curr->right)
			{
				to_del = top(stk_node_to_del);
				if(to_del == curr) 			  
				{
					stk = pop(stk, NULL);
					if(curr != root)
						curr = abr_del_node(top(stk), curr->key, op); 	/* gli passo il padre di curr */
					else
						root = abr_del_node(root, root->key, op); 	/* cancella la radice e restituisce la nuova radice */
			
					stk_node_to_del = pop(stk_node_to_del, NULL);
				}
				else
				{
					last = curr;
					stk = pop(stk, NULL);
				}
				curr = NULL;
			}
			else
				curr = curr->right;
		}
	}
	
	return root;
}

/*	DESCRIZIONE
	Costruisce un albero binario di ricerca perfettamente bilanciato, partendo dall'array dato
	in ingresso.

	PARAMETRI DI INPUT
	void *v[]:				Array contenente le chiavi.
	int n: 					Numero di elementi di v.
	struct operations *op: 	Struttura delle operazioni.

	VALORE DI RITORNO
	Restituisce un albero binario di ricerca perfettamente bilanciato contenente le chiavi corrispondenti
	all'array dato in ingresso.			*/
Abr_t build_APB(void *v[], int n, struct operations *op)
{
	int start, end, mid, tmp;
	Abr_t root = NULL, curr = NULL, nodo;
	StackPointer_t stk_apb = NULL, stk_ends = NULL;

	start = 0, end = n-1;
	while(start <= end || stk_apb != NULL)
	{
		if(start <= end)
		{
			mid = start + (end - start) / 2;
			nodo = create_abr_node(v[mid], op);
			if(root == NULL)
				root = curr = nodo;
			else
				curr->left = nodo, curr = nodo;
			stk_apb = push(stk_apb, nodo, NULL);
			stk_ends = push(stk_ends, &end, dup_int);
			tmp = mid + 1;
			stk_ends = push(stk_ends, &tmp, dup_int);
			end = mid - 1;
		}
		else
		{
			start = *((int *) top(stk_ends));
			stk_ends = pop(stk_ends, free);
			end = *((int *) top(stk_ends));
			stk_ends = pop(stk_ends, free);

			curr = top(stk_apb);
			stk_apb = pop(stk_apb, NULL);

			if(start <= end)
			{
				mid = start + (end - start) / 2;
				nodo = create_abr_node(v[mid], op);
				curr->right = nodo;
				curr = nodo;
				stk_apb = push(stk_apb, nodo, NULL);
				stk_ends = push(stk_ends, &end, dup_int);
				tmp = mid + 1;
				stk_ends = push(stk_ends, &tmp, dup_int);
				end = mid - 1;
			}
		}
	}

	return root;
}

/*	DESCRIZIONE
	Riempie un array con le chiavi dell'abr dato in input, in ordine crescente.

	PARAMETRI DI INPUT
	Abr_t root:  			Radice dell'alber da trattare
	char *a[]: 				Array dove verranno memorizzate le chiavi.	
	struct operations *op: 	Struttura delle operazioni. 			*/
int get_array(Abr_t root, void *a[], int n, struct operations *op)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root;

	int i = 0;
	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			stk = pop(stk, NULL);
			op->Store_at(a, curr->key, i);
			i++;
			curr = curr->right;
		}
	}

	return i;
}

/*	DESCRIZIONE
	Conta il numero di nodi presenti in un albero binario di ricerca.

	PARAMETRI DI INPUT
	Abr_t root: Abr_t di cui si vuole conoscere il numero di nodi.

	VALORE DI RITORNO
	Restituisce il valore intero corrispondente al numero di nodi dell'albero dato
	in ingresso.				*/
int get_n_node(Abr_t root)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root;
	int count = 0;

	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			stk = pop(stk, NULL);
			count++;
			curr = curr->right;
		}
	}

	return count;
}

/*	DESCRIZIONE
	Elimina tutti i nodi appartenenti all'albero dato in input.

	PARAMETRI DI INPUT
	Abr_t root: 			Radice dell'albero da eliminare.
	struct operations *op: 	Strutture delle operazioni.

	VALORE DI RITORNO
	Restituisce NULL in ogni caso. 					*/
Abr_t abr_destroy(Abr_t root, struct operations *op)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root, last = NULL;

	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			if(curr->right == NULL || last == curr->right)
			{
				destroy_node(curr, op);
				last = curr;
				stk = pop(stk, NULL);
				curr = NULL;
			}
			else
				curr = curr->right;
		}
	}

	return NULL;
}

/* 	DESCRIZIONE
	Dealloca il nodo dato in input.

 	PARAMETRI
	Abr_t node: 			Nodo da deallocare.
	struct operations *op: 	Struttura delle operazioni. */
void destroy_node(Abr_t node, struct operations *op)
{
	op->Destroy_key(node->key);
	free(node);
}

/* DESCRIZIONE
   La funzione stampa i nodi dell'albero dato in input, utilizzando la visita pre-order.

   PARAMETRI DI INPUT
   Abr_t root: 				Radice dell'arbero o di un qualsiasi sottoalbero.
   struct operations *op: 	Struttura delle operazioni 				*/
void preorder(Abr_t root, struct operations *op)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root;

	while(stk || curr)
	{
		if(curr)
		{
			op->Print_key(curr->key);
			stk = push(stk, curr->right, NULL); 	/* Se curr->right è NULL, non viene creato nessun nodo */
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			stk = pop(stk, NULL);
		}
	}
}

/* DESCRIZIONE
   La funzione stampa i nodi dell'albero dato in input, utilizzando la visita post-order.

   PARAMETRI DI INPUT
   Abr_t root: radice dell'arbero o di un qualsiasi sottoalbero.
   struct operations *op: 	Struttura delle operazioni 					*/
void postorder(Abr_t root, struct operations *op)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root, last = NULL;

	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			if(curr->right == NULL || last == curr->right)
			{
				op->Print_key(curr->key);
				last = curr;
				stk = pop(stk, NULL);
				curr = NULL;
			}
			else
				curr = curr->right;
		}
	}
}

/* DESCRIZIONE
   La funzione stampa i nodi dell'albero dato in input, utilizzando la visita in-order.

   PARAMETRI DI INPUT
   Abr_t root: radice dell'arbero o di un qualsiasi sottoalbero.
   struct operations *op: 	Struttura delle operazioni 					 */
void inorder(Abr_t root, struct operations *op)
{
	StackPointer_t stk = NULL;
	Abr_t curr = root;

	while(stk || curr)
	{
		if(curr)
		{
			stk = push(stk, curr, NULL);
			curr = curr->left;
		}
		else
		{
			curr = top(stk);
			stk = pop(stk, NULL);
			op->Print_key(curr->key);
			curr = curr->right;
		}
	}
}
