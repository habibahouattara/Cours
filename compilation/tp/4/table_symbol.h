#include "types.h"

// pour chaque fonction il y a une seule liste
typedef struct _liste_symb {
	char * id;
	types_symbole type;
	size_t ordre;
	types_var type_v;
	struct _liste_symb *next;
} *liste_symboles;

liste_symboles add_symbole(char *s, type_var tv, size_t ordre, liste_symboles ts);
liste_symboles inTS(char *id, liste_symboles ts);
