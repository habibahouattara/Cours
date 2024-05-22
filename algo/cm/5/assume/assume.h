#ifndef ASSUME__H
#define ASSUME__H

//  ASSUME : arrêt conditionnel du programme. Inopérant si la valeur de
//    l'expression expr est vraie (différente de zéro). Sinon : envoie sur la
//    sortie erreur le message failure, puis le nom du fichier source, la ligne
//    dans le fichier source et le nom de la fonction dans lesquels ou à
//    laquelle apparait l'appel à ASSUME, puis le code de l'expression expr ;
//    appelle ensuite exit(EXIT_FAILURE). Si la macroconstante
//    ASSUME_EXPR_EXPANSION n'est pas définie, le code de l'expression expr est
//    livré tel quel ; il est macro-évalué sinon

#define ASSUME__FAIL(failedtest, failure) \
  assume__fail(__FILE__, __LINE__, __func__, failedtest, failure)

#ifndef ASSUME_EXPR_EXPANSION

#define ASSUME(expr, failure) \
  (expr) ? ((void) 0) : ASSUME__FAIL(#expr, failure)

#else

#define ASSUME__EVAL(s) #s
#define ASSUME(expr, failure) \
  (expr) ? ((void) 0) : ASSUME__FAIL(ASSUME__EVAL(expr), failure)

#endif

//  assume__fail : envoie sur la sortie erreur le message failure, le nom de
//    fichier file, le numéro de ligne line, le nom de fonction func puis du
//    code failedtest
extern void assume__fail(const char *file, long int line, const char *func,
    const char *failedtest, const char *failure);

#endif
