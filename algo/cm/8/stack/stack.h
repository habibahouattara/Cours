//  Interface polymorphe pour la spécification PILE du TDA Pile(T)

#ifndef STACK__H
#define STACK__H

#include <stdbool.h>
#include <stddef.h>

//  struct stack, stack : structure regroupant les informations permettant de
//    gérer une pile dont les éléments sont des objets quelconques. La création
//    de la structure de données associée est confiée à la fonction stack_empty.
//    Une structure de données peut également être créée par copie d'une autre
//    en recourant à la fonction stack_duplicate
//  La structure de données ne stocke pas les objets mais des pointeurs vers ces
//    objets. Les pointeurs sont du type générique const void *
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la pile. Si des opérations d'allocation dynamique sont
//    effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets
typedef struct stack stack;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type stack * n'est pas l'adresse d'un objet préalablement renvoyé par
//    stack_empty ou stack_duplicate et non révoqué depuis par stack_dispose.
//    Cette règle ne souffre que d'une seule exception : stack_dispose tolère
//    que la déréférence de son argument ait pour valeur NULL

//  stack_empty : crée une structure de données correspondant initialement à la
//    pile vide. Renvoie NULL en cas de dépassement de capacité. Renvoie sinon
//    un pointeur vers l'objet qui gère la structure de données
extern stack *stack_empty(void);

//  stack_push : empile xptr sur la pile associée à s. Renvoie NULL si xptr vaut
//    NULL ou en cas de dépassement de capacité. Renvoie sinon xptr
extern const void *stack_push(stack *s, const void *xptr);

//  stack_pop : dépile la pile associée à s. Renvoie NULL si la pile est vide.
//    Renvoie sinon l'adresse qui était au sommet de la pile avant le dépilement
extern const void *stack_pop(stack *s);

//  stack_top : renvoie NULL si la pile associée à s est vide. Renvoie sinon
//    l'adresse qui est au sommet de la pile
extern const void *stack_top(const stack *s);

//  stack_is_empty : renvoie true ou false selon que la pile associée à s est
//    vide ou non
extern bool stack_is_empty(const stack *s);

//  stack_height : renvoie la hauteur de la pile associée à s
extern size_t stack_height(const stack *s);

//  stack_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées à
//    la structure de données associée à *sptr puis affecte à *sptr la valeur
//    NULL
extern void stack_dispose(stack **sptr);

//  stack_duplicate : crée une structure de données dont la pile qu'elle
//    représente est identique à celle associée à s. Renvoie NULL en cas de
//    dépassement de capacité. Renvoie sinon un pointeur vers l'objet qui gère
//    la structure de données créée
extern stack *stack_duplicate(const stack *s);

#endif
