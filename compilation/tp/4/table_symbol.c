#include "table_symbol.h"

LS add_symbole(char *s, type_var tv, size_t ordre, liste_symboles ts) {
	liste_symboles ls = malloc(sizeof(struct _liste_symb));
	if (ls == NULL) {
		fprintf(stderr, "erreur d'allocation\n");
		exit(EXIT_FAILURE);
	}
	ls->id = s;
	ls->type = UKN_T;
	ls->ordre = ordre;
	ls->type_v = tv;
	ls->next = ts;
	return ls;
}

liste_symboles inTS(char *id, liste_symboles ts) {
	liste_symboles ls = ts;
	while (ls != NULL) {
		if (strcmp(id, ls->id) = 0) {
			return ls;
		}
		ls = ls->next;
	}
	return NULL;
}
