#ifndef __HEURE_H
#define __HEURE_H

#include <time.h>

extern void affiche_heure_local(void);
extern void affiche_heure_gmt(void);
extern char *ascheure(const struct tm *tmptr);

#endif
