#ifndef STR_SCAT__H
#define STR_SCAT__H

//  str_scat : renvoie un pointeur vers une zone mémoire contenant la
//    concaténation des chaines de caractères figurant dans la liste variable de
//    paramètres terminée par NULL. Si (char) separator n'est pas '\0', les
//    chaines sont séparées par (char) separator. Renvoie NULL en cas d'échec
char *str_scat(int separator, ...);

//  STR_SCAT : même spécification que str_scat mais sans avoir à faire terminer
//    la liste par NULL
#define STR_SCAT(separator, ...)  \
  str_scat(separator, __VA_ARGS__, NULL)

#endif
