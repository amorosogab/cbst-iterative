struct point
{
	int x;
	int y;
};

#define MAX_STR_LEN 15  /* Incluso il byte nullo */

#define MIN_CH		33  /* Più piccolo carattere consentito */
#define MAX_CH		125 /* Massimo carattere consentito */

#define MAX_INT 	9999999
#define MIN_INT 	-9999999

int rand_num(int a, int b);
int mod_i(int x, int m);
double mod_d(double x, int m);
