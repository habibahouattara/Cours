#ifndef SUB2__H
#define SUB2__H

#include "set_mg.h"

SET_INTERFACE(setstr, const char *, strcmp)

//  setstr_print : affiche le contenu de l'ensemble associé à s. Utilise les
//    délimiteurs "{" et "}" pour l'ensemble et le séparateur ", " pour les
//    éléments
void setstr_print(const setstr *s);

#endif
