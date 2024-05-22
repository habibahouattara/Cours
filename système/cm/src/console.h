#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_NOM_FICHIER "out.txt"

#define CONSOLE_MSG_MAX_SIZE 500

/**
 * Redirige la sortie standard vers un fichier ouvert en mode ajout.
 */
extern void console_init(void);

/**
 * Affichage sur la sortie standard du message.
 * L'écriture est réalisée via un seul write.
 */
extern void console_log(const char *format, ...);

#endif // CONSOLE_H
