typedef struct stk_node* StackPointer_t;
struct stk_node 
{
	void *key;
	StackPointer_t next;
};

StackPointer_t create_stk_node(void *k);
StackPointer_t pop(StackPointer_t stk, void (*Destroy_key)(void *dato));

StackPointer_t push(StackPointer_t stk, void *k, void *(*Dup_dato)(const void *dato));

void *top(StackPointer_t stk);
void destroy_stk(StackPointer_t stk, void (*Destroy_key)(void *dato));

int is_empty(StackPointer_t stk);
