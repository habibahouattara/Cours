//  Interface polymorphe pour la spécification TABLE du TDA Table(T, T')

#ifndef TABLE__H
#define TABLE__H

#include <stddef.h>

//  struct table, table : structure regroupant les informations permettant de
//    gérer une table dont les clés et les valeurs sont des objets quelconques.
//    La création de la structure de données associée est confiée à la fonction
//    table_empty dont le paramètre compar précise la fonction de comparaison
//    des clés
//  La structure de données ne stocke pas les objets mais des pointeurs vers ces
//    objets. Les pointeurs sont du type générique const void *
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la table. Si des opérations d'allocation dynamique sont
//    effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets
typedef struct table table;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type table * n'est pas l'adresse d'un objet préalablement renvoyé par
//    table_empty et non révoqué depuis par table_dispose. Cette règle ne
//    souffre que d'une seule exception : table_dispose tolère que la
//    déréférence de son argument ait pour valeur NULL

//  table_empty : crée une structure de données correspondant initialement
//    à la table vide. La fonction de comparaison des clés est compar Renvoie
//    NULL en cas de dépassement de capacité. Renvoie sinon un pointeur vers
//    l'objet qui gère la structure de données
extern table *table_empty(int (*compar)(const void *, const void *));

//  table_add : recherche dans la table associée à t une clé égale à celle
//    d'adresse keyptr au sens de compar. Si une telle clé existe, remplace
//    l'adresse de la valeur correspondante par valptr. Sinon, le couple
//    (keyptr, valptr) est ajouté à la structure. Renvoie NULL si valptr vaut
//    NULL ou en cas de dépassement de capacité. Renvoie sinon valptr
extern const void *table_add(table *t, const void *keyptr,
    const void *valptr);

//  table_remove : recherche dans la table associée à t une clé égale à celle
//    d'adresse keyptr au sens de compar. Si un telle clé existe, retire le
//    couple (ekeyptr, evalptr) de la structure, où ekeyptr est l'adresse de
//    la clé trouvée et evalptr l'adresse de la valeur correspondante. Renvoie
//    NULL si aucune clé de la sorte n'existe. Renvoie sinon evalptr
extern const void *table_remove(table *t, const void *keyptr);

//  table_search : recherche dans la table associée à t une clé égale à celle
//    d'adresse keyptr au sens de compar. Renvoie NULL si aucune clé de la sorte
//    n'existe. Renvoie sinon l'adresse de la valeur correspondant à la clé
//    trouvée
extern const void *table_search(const table *t, const void *keyptr);

//  table_dispose : si *tptr ne vaut pas NULL, libère les ressources allouées à
//    la structure de données associée à *tptr puis affecte à *tptr la valeur
//    NULL
extern void table_dispose(table **tptr);

#endif
