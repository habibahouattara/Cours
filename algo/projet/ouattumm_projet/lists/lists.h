#ifndef LISTS__H
#define LISTS__H

#include <stdbool.h>
#include <stdio.h>

typedef struct lists lists;

//  lists_empty : crée un objet de type lists. La liste est initialement vide.
//    Renvoie un pointeur vers l'objet en cas de succès, NULL en cas d'échec
extern lists *lists_empty(void);

//  lists_is_empty : renvoie true ou false selon que la liste associée à s est
//    vide ou non
extern bool lists_is_empty(const lists *s);

//  lists_length : renvoie la longueur de la liste associée à s
extern size_t lists_length(const lists *s);

//  lists_key_empty : crée un objet de type lists de longueur n et qui a pour
//    valeur l'objet pointé par *ptr. La liste est initialement vide. Renvoie un
//    pointeur vers l'objet en cas de succès, NULL en cas d'échec
extern lists *lists_key_empty(size_t n, const void *ptr);

//  lists_insert_tail : insère une copie de *ptr en queue de la liste associée
//    à s. Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec
extern int lists_insert_tail(lists *s, const void *ptr);

//  lists_compar : compare les éléments de la liste associés à s1 avec les
//    éléments de la liste associés à s2. Renvoie une valeur strictement
//    inférieure, égale ou strictement supérieure à zéro selon que le premier
//    élément est strictement inférieur, égal ou strictement supérieur au second
extern int lists_compar(const lists *s1, const lists *s2);

//  lists_display : écrit sur le flot contrôlé par l'objet pointé par stream les
//    éléments de la liste associé à s. Les éléments sont séparés par une
//    tabulation. Renvoie zéro en cas de succès, une valeur non nulle en cas
//    d'échec
extern int lists_display(const lists *s, FILE *stream);

//  lists_value_hashfun : adaptation de la fonction de pré-hachage donnée sur le
//    support de cours appliquée sur une liste. Hachage de la valeur de la
//    cellule
extern size_t lists_value_hashfun(const lists *s);

//  list_index_to_value : retourne l'élément d'indice k de la liste associée à s 
extern void *lists_index_to_value(lists *s, int k);

//  lists_new_createdlist : crée une nouvelle liste en copiant les éléments
//    associés à la liste s. Supprime l'élément de tête et ajoute en queue une
//    copie de *ptr. Renvoie NULL en cas d'échec, un pointeur vers l'objet crée
//    sinon
extern lists *lists_new_createdlist(lists *s, const void *ptr);

//  lists_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées à
//    *sptr ainsi qu'à la liste associée puis affecte la valeur NULL à *sptr
extern void lists_dispose(lists **sptr);

#endif
