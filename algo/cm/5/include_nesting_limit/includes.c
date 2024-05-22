/*
 *  La norme C11 impose une profondeur d'imbrication d'inclusions #include
 *    minimum de 15.
 *  Elle est arbitrairement fixée au maximum à 200 sous gcc.
 *  Mais plus de trace dans les limits dans la documentation depuis la 4.8 !!!
 *  Test ici.
 *  Pour rediriger la sortie erreur vers err.txt puis compter le nombre de
 *    lignes à la main ou tout autre moyen :
 *    gcc includes.c 2> err.txt
 *  Ou alors, directement :
 *    gcc includes.c 2>&1 | wc -l
 */

#include <stdlib.h>
#include "includes.h"

int main(void) {
  return EXIT_SUCCESS;
}
