#include "types.h"

typedef struct abr_node* Abr_t; 
struct abr_node
{
	void *key;
	Abr_t left;
	Abr_t right;
};

Abr_t abr_search(Abr_t root, const void *key, struct operations *op);
Abr_t abr_del_node(Abr_t root, void *key, struct operations *op);
Abr_t abr_destroy(Abr_t root, struct operations *op);
Abr_t bet(Abr_t root, const void *a, const void *b, const int p, struct operations *op);
Abr_t abr_insert(Abr_t root, const void *key, struct operations *op);
Abr_t create_abr_node(const void *key, struct operations *op);
Abr_t abr_min(Abr_t root);
Abr_t abr_max(Abr_t root);
Abr_t build_APB_f(void *v[], int start, int end, struct operations *op);
Abr_t build_APB(void *v[], int n, struct operations *op);
Abr_t dup_abr(Abr_t a, struct operations *op);
Abr_t gen_abr(int n, struct operations *op);

int abr_equal(Abr_t abr1, Abr_t abr2, struct operations *op);
int get_n_node(Abr_t root);
int get_array_f(Abr_t root, void *v[], int start, int end, struct operations *op);
int get_array(Abr_t root, void *v[], int n, struct operations *op);

void destroy_node(Abr_t node, struct operations *op);
void preorder(Abr_t root, struct operations *op);
void postorder(Abr_t root, struct operations *op);
void inorder(Abr_t root, struct operations *op);
