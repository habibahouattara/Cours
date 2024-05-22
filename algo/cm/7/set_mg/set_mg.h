//  Génération d'interfaces et d'implantations spécialisées à tout type donné
//    pour la spécification ENS du TDA Ens(T) et son extension ENSMIN. L'accès à
//    l'extension ENSMIN requiert la définition de la macroconstante SETMIN
//  Les macrofonctions SET_INTERFACE et SET_IMPLEMENTATION, appelées avec trois
//    paramètres <settype>, <elemtype> et <elem_cmp>, génèrent respectivement
//    les parties interface et implantation. La macrofonction SET, appelée avec
//    les mêmes paramètres, génère les deux parties. Les autres macros définies
//    dans ce fichier sont privées et ne doivent pas être utilisées
//  La structure générée est struct <settype> et est désignée par le nom
//    <settype>. L'expression <elemtype> désigne le type des éléments.
//    L'identificateur <elem_cmp> désigne la fonction de comparaison des
//    éléments. Toutes les fonctions générées sont préfixées par <settype>_

#ifndef SET__H
#define SET__H

#include <stdbool.h>
#include <stdlib.h>
#include "assume.h"

#define SET(settype, elemtype, elem_cmp)                                       \
  SET_INTERFACE(settype, elemtype, elem_cmp)                                   \
  SET_IMPLEMENTATION(settype, elemtype, elem_cmp)

//  struct <settype>, <settype> : structure regroupant les informations
//    permettant de gérer un ensemble dont les éléments sont des objets du type
//    <elemtype>. La création de la structure de données associée est confiée à
//    la fonction <settype>_empty. Une structure de données peut également être
//    créée par copie d'une autre en recourant à la fonction <settype>_duplicate
//
//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type <settype> * n'est pas l'adresse d'un objet préalablement renvoyé
//    par <settype>_empty ou <settype>_duplicate et non révoqué depuis par
//    <settype>_dispose. Cette règle ne souffre que d'une seule exception :
//    <setype>_dispose tolère que la déréférence de son argument ait pour valeur
//    NULL
//
//  <settype> *<settype>_empty(void) : crée une structure de données
//    correspondant initialement à l'ensemble vide. Il est mis fin au programme
//    en cas de dépassement de capacité. Renvoie un pointeur vers l'objet qui
//    gère la structure de données
//  void <settype>_add(<settype> *s, <elemtype> x) : ajoute x à l'ensemble
//    associé à s. Il est mis fin au programme en cas de dépassement de capacité
//  void <settype>_remove(<settype> *s, <elemtype> x) : retire x de l'ensemble
//    associé à s
//  bool <settype>_is_empty(const <settype> *s) : renvoie true ou false selon
//    que l'ensemble associé à s est vide ou non
//  bool <settype>_is_in(const <settype> *s, <elemtype> x) : renvoie true ou
//    false selon que x appartient ou non à l'ensemble associé à s
//  size_t <settype>_card(const <settype> *s) : renvoie le cardinal de
//    l'ensemble associé à s
//  <elemtype> <settype>_min(const <settype> *s) : recherche l'élément minimal
//    au sens de <elem_cmp> de l'ensemble associé à s. Il est mis fin au
//    programme si l'ensemble est vide. Renvoie l'élément trouvé. Requiert la
//    définition de la macroconstante SETMIN
//  void <settype>_dispose(<settype> **sptr) : si *sptr ne vaut pas NULL, libère
//    les ressources allouées à la structure de données associée à *sptr puis
//    affecte à *sptr la valeur NULL
//  <settype> *<settype>_duplicate(const <settype> *s) : crée une structure de
//    données dont l'ensemble qu'elle représente contient les mêmes éléments que
//    celui associé à s. Il est mis fin au programme en cas de dépassement de
//    capacité. Renvoie un pointeur vers l'objet qui gère la structure de
//    données créée

#define SET_INTERFACE(settype, elemtype, elem_cmp)                             \
                                                                               \
  typedef struct settype settype;                                              \
                                                                               \
  extern settype *settype ## _empty(void);                                     \
  extern void settype ## _add(settype * s, elemtype x);                        \
  extern void settype ## _remove(settype * s, elemtype x);                     \
  extern bool settype ## _is_empty(const settype * s);                         \
  extern bool settype ## _is_in(const settype * s, elemtype x);                \
  extern size_t settype ## _card(const settype * s);                           \
  SET__INTERFACE_SETMIN(settype, elemtype, elem_cmp)                           \
  extern void settype ## _dispose(settype * *sptr);                            \
  extern settype *settype ## _duplicate(const settype * s);

#ifndef SETMIN
#define SET__INTERFACE_SETMIN(settype, elemtype, elem_cmp)
#else
#define SET__INTERFACE_SETMIN(settype, elemtype, elem_cmp)                     \
  extern elemtype settype ## _min(const settype * s);
#endif

//  struct <settype>, <settype> : gestion des éléments dans l'ensemble à l'aide
//    d'un tableau, array, géré de manière préfixielle et sans doublon. Le
//    composant card mémorise la longueur du préfixe
//  Invariant structurel pour une structure de type <settype> correctement créée
//    puis utilisée :
//    1) card <= SET_CARD_MAX ;
//    2) x appartient à l'ensemble implanté si et seulement si x possède une
//      occurrence dans le sous-tableau array[0 ... card - 1] au sens de
//      <elem_compar> ;
//    3) le sous-tableau array[0 ... card - 1] ne contient aucun doublon au sens
//      de <elem_compar>

#define SET__CARD_MAX 100

#define SET_IMPLEMENTATION(settype, elemtype, elem_cmp)                        \
                                                                               \
  struct settype {                                                             \
    size_t card;                                                               \
    elemtype array[SET__CARD_MAX];                                             \
  };                                                                           \
                                                                               \
  settype *settype ## _empty(void) {                                           \
    settype *s = malloc(sizeof *s);                                            \
    ASSUME(s != NULL, "lack of memory space");                                 \
    s->card = 0;                                                               \
    return s;                                                                  \
  }                                                                            \
                                                                               \
  bool settype ## _is_empty(const settype * s) {                               \
    return s->card == 0;                                                       \
  }                                                                            \
                                                                               \
  size_t settype ## _card(const settype * s) {                                 \
    return s->card;                                                            \
  }                                                                            \
                                                                               \
  /*  settype ## __search : recherche dans le sous-tableau \
   *    s->array[0 ... s->card - 1] un composant de valeur égale à x au sens \
   *    de elem_cmp.  Renvoie l'adresse du composant en cas de succès, NULL en \
   *    cas d'échec \
   */                                                                          \
  static elemtype *settype ## __search(const settype * s, elemtype x) {        \
    for (elemtype const *p = s->array; p < s->array + s->card; ++p) {          \
      if (elem_cmp(*p, x) == 0) {                                              \
        return (elemtype *) p;                                                 \
      }                                                                        \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  bool settype ## _is_in(const settype * s, elemtype x) {                      \
    return settype ## __search(s, x) != NULL;                                  \
  }                                                                            \
                                                                               \
  void settype ## _add(settype * s, elemtype x) {                              \
    if (settype ## __search(s, x) != NULL) {                                   \
      return;                                                                  \
    }                                                                          \
    ASSUME(s->card < SET__CARD_MAX, "lack of memory space");                   \
    s->array[s->card] = x;                                                     \
    s->card += 1;                                                              \
  }                                                                            \
                                                                               \
  void settype ## _remove(settype * s, elemtype x) {                           \
    elemtype *p = settype ## __search(s, x);                                   \
    if (p == NULL) {                                                           \
      return;                                                                  \
    }                                                                          \
    s->card -= 1;                                                              \
    *p = s->array[s->card];                                                    \
  }                                                                            \
                                                                               \
  SET__IMPLEMENTATION_SETMIN(settype, elemtype, elem_cmp)                      \
                                                                               \
  void settype ## _dispose(settype * *sptr) {                                  \
    free(*sptr);                                                               \
    *sptr = NULL;                                                              \
  }                                                                            \
                                                                               \
  settype *settype ## _duplicate(const settype * s) {                          \
    settype *s2 = malloc(sizeof *s2);                                          \
    ASSUME(s2 != NULL, "lack of memory space");                                \
    s2->card = s->card;                                                        \
    elemtype *q = s2->array;                                                   \
    for (elemtype const *p = s->array; p < s->array + s->card; ++p) {          \
      *q = *p;                                                                 \
      ++q;                                                                     \
    }                                                                          \
    return s2;                                                                 \
  }

#ifndef SETMIN
#define SET__IMPLEMENTATION_SETMIN(settype, elemtype, elem_cmp)
#else
#define SET__IMPLEMENTATION_SETMIN(settype, elemtype, elem_cmp)                \
                                                                               \
  elemtype settype ## _min(const settype * s) {                                \
    ASSUME(s->card != 0, "empty set");                                         \
    elemtype const *q = s->array;                                              \
    for (elemtype const *p = q + 1; p < s->array + s->card; ++p) {             \
      if (elem_cmp(*p, *q) < 0) {                                              \
        q = p;                                                                 \
      }                                                                        \
    }                                                                          \
    return *q;                                                                 \
  }
#endif

#endif
