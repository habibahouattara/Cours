#ifndef TYPE__H
#define TYPE__H

#include <stddef.h>
#include <stdbool.h>

typedef enum { INT_T, BOOL_T, ERR_T, UKN_T } types_symbole;
typedef enum { FCT, VLOC, PARAM } types_variable;

//  structure capable de gérer une table des symboles à l'aide d'une liste
//    simplement chaînée
typedef struct _table_symbole {
  char *id; // nom du symbole
  types_symbole type_s; // type du symbole
  size_t ordre; // ordre d'ajout dans la table
  types_variable type_v; // quel genre de variable est le symbole
  struct _table_symbole *next; // un pointeur vers le symbole suivant
} *table_symbole;

//  addSymbol: crée une structure de données correspondant initialement à une table
//    de symbole vide. Ajoute un nouveau symbole de nom pointé par id dans la table
//     des symboles. Renvoie NULL en cas de dépassement de capacité. Renvoie sinon
//    un pointeur vers l'objet qui gére la struture de données
table_symbole addSymbol(char *id, types_variable tv, size_t ordre, table_symbole ts);

//  inTS: renvoie NULL si le symbole de nom pointé par id n'est pas présent dans la
//    table des symboles. Renvoie l'adresse du symbole trouvé sinon
table_symbole inTS(char *id, table_symbole ts);

//  freeFirstCell: libère les ressources allouées à la première cellule de la table
//    des symboles
void freeFirstCell();

#endif
