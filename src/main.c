#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "utils.h"
#include "abr.h"
	
#define MAX_ABR 15

#define MAX_ARG 	50
#define MAX_ARG_LEN 20

enum types{ INTEGER, REAL, STRING, POINT };
struct s_abr
{
	Abr_t root;
	char *alias;    
	int active;		/* Un abr può essere attivo anche se root = NULL. es.: un abr appena creato. */
	struct operations *op;
	enum types type;
};

int get_cmd(char arg[][MAX_ARG_LEN]);
void show_help(void);
int search_abr(struct s_abr abr[], const char *alias);
int set_next_block(struct s_abr abr[]);
void clear_in_buffer(void);
struct operations *dup_op(struct operations *a);

int main(void)
{
	struct s_abr abr[MAX_ABR];
	char cmd[MAX_ARG][MAX_ARG_LEN];
	int narg, abr_pos, exit_cmd;		
	int i, j;								/* indici di uso generale*/

	/* Inizializzazione */
	for(i = 0; i < MAX_ABR; i++)
		abr[i].root = NULL, abr[i].active = 0;
	srand(time(NULL));
	exit_cmd = 0;

	printf("Scrivi help per visualizzare l'aiuto.\n");
	do
	{
		printf("> ");
		narg = get_cmd(cmd);

		if(strcmp(cmd[0], "help") == 0)
			show_help();
		else if(strcmp(cmd[0], "preorder") == 0)
		{
			if(narg != 2)
				printf("usage: preorder <abr>\n");
			else if((abr_pos = search_abr(abr, cmd[1])) < 0)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				printf("%s: ", abr[abr_pos].alias);
				preorder(abr[abr_pos].root, abr[abr_pos].op);
				printf("\n");
			}
		}
		else if(strcmp(cmd[0], "postorder") == 0)
		{
			if(narg != 2)
				printf("usage: postorder <abr>\n");
			else if((abr_pos = search_abr(abr, cmd[1])) < 0)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				printf("%s: ", abr[abr_pos].alias);
				postorder(abr[abr_pos].root, abr[abr_pos].op);
				printf("\n");
			}
		}
		else if(strcmp(cmd[0], "inorder") == 0)
		{
			if(narg != 2)
				printf("usage: inorder <abr>\n");
			else if((abr_pos = search_abr(abr, cmd[1])) < 0)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				printf("%s: ", abr[abr_pos].alias);
				inorder(abr[abr_pos].root, abr[abr_pos].op);
				printf("\n");
			}
		}
		else if(strcmp(cmd[0], "newabr") == 0)
		{
			if(narg != 3)
				printf("usage: newabr <t> <alias> dove <t> = int | real | string\n");
			else if(search_abr(abr, cmd[2]) != -1)
				printf("%s già esiste\n", cmd[2]);
			else if((i = set_next_block(abr)) == -1)
				printf("impossibile creare più di 15 alberi\n");
			else if((abr[i].alias = (char *) malloc(sizeof(char) * (strlen(cmd[2]) + 1))) == NULL)
				printf("memoria esaurita.\n");
			else
			{
				if(strcmp(cmd[1], "int") == 0)
				{
					abr[i].op = initialize(int_cmp, dup_int, rand_int, is_even_int, int_store_at, free, print_key_int, get_input_key_int);
					abr[i].type = INTEGER;
				}
				else if(strcmp(cmd[1], "real") == 0)
				{
					abr[i].op = initialize(double_cmp, dup_double, rand_double, is_even_double, double_store_at, free, print_key_double, get_input_key_double);
					abr[i].type = REAL;
				}
				else if(strcmp(cmd[1], "string") == 0)
				{
					abr[i].op = initialize(string_cmp, dup_string, rand_string, is_even_string, string_store_at, free, print_key_string, get_input_key_string);
					abr[i].type = STRING;
				}
				else if(strcmp(cmd[1], "point") == 0)
				{
					abr[i].op = initialize(points_cmp, dup_point, rand_point, is_even_point, point_store_at, free, print_key_point, get_input_key_point);
					abr[i].type = POINT;
				}
				else
				{
					printf("not supported type\n");
					free(abr[i].alias);
					free(abr[i].op);
					continue;
				}
				
				if(abr[i].op == NULL)
				{
					printf("errore\n");
					free(abr[i].alias);
					free(abr[i].op);
					continue;
				}

				abr[i].active = 1;
				strcpy(abr[i].alias, cmd[2]);
			}
		}
		else if(strcmp(cmd[0], "listabr") == 0)
		{
			for(i = 0; i < MAX_ABR; i++)
				if(abr[i].active)
					printf("%s\n", abr[i].alias);
			printf("\n");
		}
		else if(strcmp(cmd[0], "newnode") == 0)
		{
			if(narg != 3)
				printf("usage: newnode <abr> <n>\n");
			else if((j = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				int n = atoi(cmd[2]);
				for(i = 1; i <= n; i++)
				{
					printf("%d: ", i);
					void *key = (abr[j].op)->Get_input_key();
					if(key == NULL)
					{
						printf("input error\n");
						continue;
					}

					abr[j].root = abr_insert(abr[j].root, key, abr[j].op);
					(abr[j].op)->Destroy_key(key);
				}
			}
		}
		else if(strcmp(cmd[0], "delnode") == 0)
		{
			if(narg != 3)
				printf("usage: delnode <abr> <n>\n");
			else if((j = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				int n = atoi(cmd[2]);
				for(i = 1; i <= n; i++)
				{
					printf("%d: ", i);
					void *key = (abr[j].op)->Get_input_key();
					if(key == NULL)
					{
						printf("input error\n");
						continue;
					}

					abr[j].root = abr_del_node(abr[j].root, key, abr[j].op);
					(abr[j].op)->Destroy_key(key);
				}

				if(abr[j].root == NULL)
				{
					printf("%s è vuoto. Vuoi disattivarlo? (y/n) ", abr[j].alias);
					if(getchar() != 'n')
					{
						abr[j].active = 0;
						free(abr[j].alias);
						free(abr[j].op);
						printf("abr disattivato\n");
					}
					else
						printf("abr non disattivato\n");

					clear_in_buffer();

				}
			}

		}
		else if(strcmp(cmd[0], "delabr") == 0)
		{
			if(narg != 2)
				printf("usage: delabr <abr>\n");
			else if((i = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				abr[i].root = abr_destroy(abr[i].root, abr[i].op);

				printf("Tutti i nodi di %s sono stati cancellati. Vuoi disattivarlo? (y/n) ", abr[i].alias);
				if(getchar() == 'y')
				{
					abr[i].active = 0;
					free(abr[i].alias);
					free(abr[i].op);
					printf("abr disattivato\n");
				}
				else
					printf("abr non disattivato\n");

				clear_in_buffer();
			}
		}
		else if(strcmp(cmd[0], "cmpabr") == 0)
		{
			int abr1_index, abr2_index;

			if(narg < 3)
				printf("usage: cmpabr <abr-1> .. <abr-n>\n");
			else
			{
				for(i = 1; (cmd[i+1][0] != '\0') && ((abr1_index = search_abr(abr, cmd[i])) != -1); i++)
				{
					for(j = i + 1; (cmd[j][0] != '\0') && ((abr2_index = search_abr(abr, cmd[j])) != -1); j++)
					{
						if(abr[abr1_index].type == abr[abr2_index].type)
						{
							if(abr_equal(abr[abr1_index].root, abr[abr2_index].root, abr[abr1_index].op))
								printf("%s e %s sono uguali\n", abr[abr1_index].alias, abr[abr2_index].alias);
							else
								printf("%s e %s sono diversi\n", abr[abr1_index].alias, abr[abr2_index].alias);
						}
						else
							printf("%s e %s non sono confrontabili\n", abr[abr1_index].alias, abr[abr2_index].alias);
					}
				}

				if(abr1_index == -1)
					printf("%s non esiste\n", cmd[i]);
				else if(abr2_index == -1)
					printf("%s non esiste\n", cmd[j]);

			}
		}
		else if(strcmp(cmd[0], "getarr") == 0)
		{
			if(narg != 2)
				printf("usage: getarr <abr>\n");
			else if((j = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				int n = get_n_node(abr[j].root);
				void *v[n];

				get_array(abr[j].root, v, n, abr[j].op);
				for(i = 0; i < n; i++)
				{
					(abr[j].op)->Print_key(v[i]);
					(abr[j].op)->Destroy_key(v[i]);					
				}

				printf("\n");
			}
		}
		else if(strcmp(cmd[0], "apb") == 0)
		{
			if(narg != 3)
				printf("usage: apb <abr> <n_abr>\n");
			else if((j = search_abr(abr, cmd[1])) == -1) 	/* verifica che il primo argomento sia alias di un abr già esistente */ 	
				printf("%s non esiste\n", cmd[1]);
			else if(search_abr(abr, cmd[2]) != -1) 			/* verifica che il secondo argomento non sia alias di un abr già esistente */
				printf("%s già esiste\n", cmd[2]);
			else if((i = set_next_block(abr)) == -1)
				printf("non è possibile allocare spazio per un nuovo abr.\n");
			else if((abr[i].alias = (char *) malloc(sizeof(char) * (strlen(cmd[2]) + 1))) == NULL)
				printf("memoria esaurita.\n");
			else
			{
				int n = get_n_node(abr[j].root);
				void *v[n];
				
				get_array(abr[j].root, v, n, abr[j].op);
				abr[i].root = build_APB(v, n, abr[j].op);
				strcpy(abr[i].alias, cmd[2]);
				abr[i].type = abr[j].type;
				abr[i].op = dup_op(abr[j].op);
				abr[i].active = 1;

				for(i = 0; i < n; i++)
					(abr[j].op)->Destroy_key(v[i]);					
			}
		}
		else if(strcmp(cmd[0], "bet") == 0)
		{
			if(narg != 3)
				printf("usage: bet <abr> <p>\n");
			else if((i = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else				
			{
				char c;

				printf("a: ");
				void *a = (abr[i].op)->Get_input_key();
				printf("b: ");
				void *b = (abr[i].op)->Get_input_key();
				abr[i].root = bet(abr[i].root, a, b, atoi(cmd[2]), abr[i].op);
				
				if(abr[i].root == NULL)
				{
					printf("%s è vuoto. Vuoi disattivarlo? (y/n) ", abr[i].alias);
					if((c = getchar()) != 'n')
					{
						abr[i].active = 0;
						free(abr[i].alias);
						printf("abr disattivato\n");
					}
					else
						printf("abr non disattivato\n");

					clear_in_buffer();
				}
				
				abr[i].op->Destroy_key(a);
				abr[i].op->Destroy_key(b);
				if(abr[i].root == NULL && c != 'n')
					free(abr[i].op);
			}
		}
		else if(strcmp(cmd[0], "genabr") == 0)
		{
			if(narg != 4)
				printf("usage: genabr <t> <alias> <n> dove <t> = int|real|string\n");
			else if(search_abr(abr, cmd[2]) != -1)
				printf("abr esistente. scegliere un nuovo nome.\n");
			else if((i = set_next_block(abr)) == -1)
				printf("impossibile creare un nuovo abr\n");
			else if((abr[i].alias = (char *) malloc(sizeof(char) * (strlen(cmd[2]) + 1))) == NULL)
				printf("memoria insufficiente.\n");
			else
			{
				if(strcmp(cmd[1], "int") == 0)
				{
					abr[i].op = initialize(int_cmp, dup_int, rand_int, is_even_int, int_store_at, free, print_key_int, get_input_key_int);
					abr[i].type = INTEGER;
				}
				else if(strcmp(cmd[1], "real") == 0)
				{
					abr[i].op = initialize(double_cmp, dup_double, rand_double, is_even_double, double_store_at, free, print_key_double, get_input_key_double);
					abr[i].type = REAL;
				}
				else if(strcmp(cmd[1], "string") == 0)
				{
					abr[i].op = initialize(string_cmp, dup_string, rand_string, is_even_string, string_store_at, free, print_key_string, get_input_key_string);
					abr[i].type = STRING;
				}
				else if(strcmp(cmd[1], "point") == 0)
				{
					abr[i].op = initialize(points_cmp, dup_point, rand_point, is_even_point, point_store_at, free, print_key_point, get_input_key_point);
					abr[i].type = POINT;
				}
				else
				{
					printf("not supported type\n");
					continue;
				}
				
				if(abr[i].op == NULL)
				{
					printf("errore\n");
					continue;
				}
				abr[i].active = 1;
				strcpy(abr[i].alias, cmd[2]);	
		
				if((abr[i].root = gen_abr(atoi(cmd[3]), abr[i].op)) == NULL)
				{
					printf("impossibile generare l'albero.\n");
					free(abr[i].alias);
					free(abr[i].op);
					abr[i].active = 0;
				}
				else
					printf("albero correttamente generato\n");

			}
		}
		else if(strcmp(cmd[0], "dupabr") == 0)
		{
			if(narg != 3)
				printf("usage: dupabr <abr1> <abr2>\n");
			else if((i = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste.\n", cmd[1]);
			else if(search_abr(abr, cmd[2]) != -1)
				printf("%s già esiste.\n", cmd[2]);
			else if((j = set_next_block(abr)) == -1)
				printf("impossibile creare un nuovo abr\n");
			else if((abr[j].alias = (char *) malloc(sizeof(char) * (strlen(cmd[2]) + 1))) == NULL)
				printf("memoria insufficiente.\n");
			else
			{
				strcpy(abr[j].alias, cmd[2]);
				abr[j].active = 1;
				abr[j].root = dup_abr(abr[i].root, abr[i].op);
				abr[j].op = dup_op(abr[i].op);
				abr[j].type = abr[i].type;
			}
		}
		else if(strcmp(cmd[0], "search") == 0)
		{
			if(narg != 2)
				printf("usage: search <abr>\n");
			else if((i = search_abr(abr, cmd[1])) == -1)
				printf("%s non esiste\n", cmd[1]);
			else
			{
				void *key = NULL;

				printf("nodo: ");
				key = (abr[i].op)->Get_input_key();
				if(abr_search(abr[i].root, key, abr[i].op) == NULL)
					printf("il nodo non esiste in %s\n", abr[i].alias);
				else
					printf("il nodo esiste.\n");

				abr[i].op->Destroy_key(key);
			}
		}
		else if(strcmp(cmd[0], "exit") == 0)
			exit_cmd = 1;
		else
			printf("comando non riconosciuto\n");

	}while(!exit_cmd);

	/*libera memoria*/
	for(i = 0; i < MAX_ABR; i++)
	{
		if(abr[i].active)
		{
			abr[i].root = abr_destroy(abr[i].root, abr[i].op);
			free(abr[i].alias);
			free(abr[i].op);
		}
	}

	
	return exit_cmd;
} 

/* DESCRIZIONE
   Mostra messaggio d'aiuto.	*/
void show_help(void)
{
	printf("preorder <abr>\t\t\t\tEsegue una visita preorder di <abr>\n"
		   "postorder <abr>\t\t\t\tEsegue una visita postorder di <abr>\n"
		   "inorder <abr>\t\t\t\tEsegue una vista inorder di <abr>\n"
		   "newabr <t> <alias>\t\t\tCrea un nuovo abr di nome <alias> <i> = int | real | string\n"
		   "listabr\t\t\t\t\tMostra gli abr attivi\n"
		   "newnode <abr> <n>\t\t\tInserisci <n> nodi nell'abr dato\n"
		   "delnode <abr> <n>\t\t\tElimina <n> nodi dall'abr dato\n"
		   "delabr <abr>\t\t\t\tElimina tutti i nodi appartenenti ad <abr>\n"
		   "cmpabr <abr-1> .. <abr-n>\t\tConfronta n abr\n"
		   "getarr <abr>\t\t\t\tCostruisce un array contenente le chiavi di <abr> in ordine crescente\n"
		   "apb <abr> <n_abr>\t\t\tPartendo dall'abr <abr> costruisce un albero perfettamente bilanciato con alias <n_abr>\n"
		   "bet <abr>\t\t\t\tElimina tutti i nodi che contengono una stringa compresa tra <a> e <b> e che sia pari(1) o dispari(0)\n"
		   "genabr <t> <alias> <n>\t\t\tGenera un abr di nome <alias> di tipo <t> = int | real | string contenente n stringhe casualmente generate\n"
		   "dupabr <abr1> <abr2>\t\t\tCrea un nuovo abr di nome <abr2>, duplicando <abr1>.\n"
		   "search <abr>\t\t\t\tCerca un nodo in <abr>\n"
		   "exit\t\t\t\t\tEsci dall'applicativo\n");
}

/* DESCRIZIONE
   Ottiene l'input dall'utente e lo inserisce nella matrice arg. Ogni spazio inserito dall'utente,
   corrisponde ad una nuova riga della matrice.

   PARAMETRI DI INPUT
   char arg[][MAX_ARG_LEN]: Matrice utilizzata per restituire i comandi dell'utente, opportunamente
   							formattati.

   VALORE DI RITORNO
   Matrice degli argomenti e numero degli argomenti.		*/
int get_cmd(char arg[][MAX_ARG_LEN])
{
	int i, j;

	i = 0, j = 0;
	while((j < MAX_ARG_LEN-1) && (i < MAX_ARG-1) && ((arg[i][j++] = getchar()) != '\n'))
	{
		if(isspace(arg[i][j-1]))
		{
			arg[i][j-1] = '\0';
			i++;
			j = 0;
		}
	}

	arg[i][j-1] = '\0';
	arg[i+1][0] = '\0';

	return i+1;
}

/*  DESCRIZIONE
	Ricerca nell'insieme degli abr attivi (quelli creati dall'utente), un abr corrispondere ad alias;

   	PARAMETRI DI INPUT
   	struct s_abr abr[]: Insieme degli abr.
   	const char *alias:	alias dell'abr da ricercare.	

   	VALORI DI RITORNO
   	La funzione restituisce -1 se non esiste un abr di nome alias, altrimenti retituisce l'indice
   	dell'abr.			*/
int search_abr(struct s_abr abr[], const char *alias)
{
	int n, i;
	int trovato;

	i = 0;
	trovato = 0;
	while(i < MAX_ABR && !trovato)
	{
		if(abr[i].active)
			if(strcmp(abr[i].alias, alias) == 0)
				trovato = 1;
		i++;
	}


	if(trovato)
		return i-1;
	else
		return -1;
	
}

/*  DESCRIZIONE
	Trova il successivo blocco libero nell'insieme degli abr. Il successivo blocco libero è
	la locazione sucessiva a empty_block dell'array abr che non contiene un abr attivo.

    PARAMETRI DI INPUT
    struct s_abr abr[]: Insieme degli abr.

    VALORI DI RITORNO
    Restituisce il successivo blocco libero dell'array abr.*/
int set_next_block(struct s_abr abr[])
{
	static int empty_block;
	int start;

	start = mod_i(empty_block-1, MAX_ABR);
	while(abr[empty_block].active && ((empty_block = mod_i(empty_block+1, MAX_ABR)) != start))
		;

	if(abr[empty_block].active)
		return -1;
	else
		return empty_block;
}

/* 	DESCRIZIONE
	Puscisce il buffer di input 			*/
void clear_in_buffer(void)
{
	while(getchar() != '\n')
		;
}

struct operations *dup_op(struct operations *a)
{
	struct operations *b = NULL;

	if(a)
	{
		b = (struct operations *) malloc(sizeof(struct operations));
		if(b)
		{
			b->Cmp_function = a->Cmp_function;
			b->Dup_dato = a->Dup_dato;
			b->Rand_function = a->Rand_function;
			b->Is_even = a->Is_even;
			b->Store_at = a->Store_at;
			b->Destroy_key = a->Destroy_key;
			b->Print_key = a->Print_key;
			b->Get_input_key = a->Get_input_key;
		}
	}

	return b;
}
