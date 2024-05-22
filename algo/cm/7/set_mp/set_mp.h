//  Interface polymorphe pour la spécification ENS du TDA Ens(T) et son
//    extension ENSMIN. L'accès à l'extension ENSMIN requiert la définition de
//    la macroconstante SETMIN

#ifndef SET__H
#define SET__H

#include <stdbool.h>
#include <stddef.h>

//  struct set, set : structure regroupant les informations permettant de gérer
//    un ensemble dont les éléments sont des objets quelconques. La création
//    de la structure de données associée est confiée à la fonction set_empty
//    dont le paramètre compar précise la fonction de comparaison des éléments.
//    Une structure de données peut également être créée par copie d'une autre
//    en recourant à la fonction set_duplicate
//  La structure de données ne stocke pas les objets mais des pointeurs vers ces
//    objets. Les pointeurs sont du type générique const void *
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans l'ensemble. Si des opérations d'allocation dynamique sont
//    effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets
typedef struct set set;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type set * n'est pas l'adresse d'un objet préalablement renvoyé par
//    set_empty ou set_duplicate et non révoqué depuis par set_dispose. Cette
//    règle ne souffre que d'une seule exception : set_dispose tolère que la
//    déréférence de son argument ait pour valeur NULL

//  set_empty : crée une structure de données correspondant initialement à
//    l'ensemble vide. La fonction de comparaison des éléments est compar. Il
//    est mis fin au programme en cas de dépassement de capacité. Renvoie un
//    pointeur vers l'objet qui gère la structure de données
extern set *set_empty(int (*compar)(const void *, const void *));

//  set_add : ajoute l'élément d'adresse xptr à l'ensemble associé à s. Il est
//    mis fin au programme en cas de dépassement de capacité
extern void set_add(set *s, const void *xptr);

//  set_remove : retire l'élément d'adresse xptr de l'ensemble associé à s
extern void set_remove(set *s, const void *xptr);

//  set_is_empty : renvoie true ou false selon que l'ensemble associé à s est
//    vide ou non
extern bool set_is_empty(const set *s);

//  set_is_in : renvoie true ou false selon que l'élément d'adresse xptr
//    appartient ou non à l'ensemble associé à s
extern bool set_is_in(const set *s, const void *xptr);

//  set_card : renvoie le cardinal de l'ensemble associé à s
extern size_t set_card(const set *s);

#ifdef SETMIN

//  set_min : recherche l'élément minimal de l'ensemble associé à s. Il est mis
//    fin au programme si l'ensemble est vide. Renvoie l'adresse de l'élément
//    trouvé
extern const void *set_min(const set *s);

#endif

//  set_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées à
//    la structure de données associée à *sptr puis affecte à *sptr la valeur
//    NULL
extern void set_dispose(set **sptr);

//  set_duplicate : crée une structure de données dont l'ensemble qu'elle
//    représente contient les mêmes éléments que celui associé à s et qui
//    possède la même fonction de comparaison des éléments. Il est mis fin au
//    programme en cas de dépassement de capacité. Renvoie un pointeur vers
//    l'objet qui gère la structure de données créée
extern set *set_duplicate(const set *s);

#endif
