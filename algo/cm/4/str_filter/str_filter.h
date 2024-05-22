#ifndef STR_FILTER__H
#define STR_FILTER__H

//  str_filter : alloue une chaine de caractères dont le contenu est la suite
//    extraite de s constituée des caractères satisfaisant filter. Renvoie cette
//    chaine en cas de succès, NULL en cas d'échec
char *str_filter(const char *s, int (*filter)(int));

#endif
