struct operations
{
	int (*Cmp_function)(const void *elem1, const void *elem2);
	void *(*Dup_dato)(const void *dato);
	void *(*Rand_function)(void);
	int (*Is_even)(const void *elem);
	int (*Store_at)(void *a[], const void *elem, int i);
	void (*Destroy_key)(void *key);
	void (*Print_key)(const void *key);
	void *(*Get_input_key)(void);
};

int string_cmp(const void *str1, const void *str2);
int int_cmp(const void *a, const void *b);
int double_cmp(const void *a, const void *b);
int points_cmp(const void *a, const void *b);

void *dup_int(const void *dato);
void *dup_double(const void *dato);
void *dup_string(const void *dato);
void *dup_point(const void *dato);

void *rand_int(void);
void *rand_double(void);
void *rand_string(void);
void *rand_point(void);

int is_even_int(const void *a);
int is_even_double(const void *a);
int is_even_string(const void *a);
int is_even_point(const void *a);

int int_store_at(void *a[], const void *elem, int i);
int double_store_at(void *a[], const void *elem, int i);
int string_store_at(void *a[], const void *elem, int i);
int point_store_at(void *a[], const void *elem, int i);

void print_key_int(const void *key);
void print_key_double(const void *key);
void print_key_string(const void *key);
void print_key_point(const void *key);

struct operations *initialize(int (*Cmp_function)(const void *elem1, const void *elem2),
							  void *(*Dup_dato)(const void *dato),
							  void *(*Rand_function)(void),
							  int (*Is_even)(const void *elem),
	   						  int (*Store_at)(void *a[], const void *elem, int i),
							  void (*Destroy_key)(void *key),
	 						  void (*Print_key)(const void *key),
							  void *(*Get_input_key)(void));

void *get_input_key_int(void);
void *get_input_key_double(void);
void *get_input_key_string(void);
void *get_input_key_point(void);

