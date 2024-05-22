#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>

#define FUN_FAILURE -1
#define FUN_SUCCESS 0

/**
 * 
 */
int status(const char *filename);

/**
 * 
 */
void strmode(mode_t mode, char *buf);

/**
 * 
 */
void lsl(void);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  //~ if (status(argv[1]) != 0) {
    //~ fprintf(stderr, "erreur\n");
    //~ exit(EXIT_FAILURE);
  //~ }
  
  lsl();
  
  return EXIT_SUCCESS;
}

void strmode(mode_t mode, char *buf) {
  const char s[] = "rwxrwxrwx";
  for (size_t i = 0; i < 9; i++) {
    // 
    buf[i] = mode & (1 << (8 -i)) ? s[i] : '-';
  }
  buf[9] = '\0';
}

void lsl(void) {
  DIR *d = opendir(".");
  struct dirent *di;
  
  while ((di = readdir(d)) != NULL) {
    status(di->d_name);
  }
  if (closedir(d) == -1) {
    perror("closedir");
    exit(EXIT_FAILURE);
  }
}

int status(const char *filename) {
  struct stat info;
  
  if (stat(filename, &info) != 0) {
    perror("stat");
    return FUN_FAILURE;
  }
  
  // numero ino
  fprintf(stdout, "%ld ", (long) info.st_ino);

  // le type de fichier possible
  switch(info.st_mode & S_IFMT) {
    case S_IFDIR:
      fprintf(stdout, "d");
      break;
    case S_IFIFO:
      fprintf(stdout, "f");
      break;
    case S_IFBLK:
      fprintf(stdout, "b");
      break;
    case S_IFCHR:
      fprintf(stdout, "c");
      break;
    case S_IFLNK:
      fprintf(stdout, "l");
      break;
    case S_IFSOCK:
      fprintf(stdout, "s");
      break;
    default:
      fprintf(stdout, "-");
  }
  char charmode[10];
  strmode(info.st_mode, charmode);
  fprintf(stdout, "%s ", charmode);

  // nombre de liens
  fprintf(stdout, "%ld ", (long) info.st_nlink);

  // propriétaire
  struct passwd *pwd = getpwuid(info.st_uid);
  if (pwd == NULL) {
    perror("getpwuid");
    return FUN_FAILURE;
  }
  fprintf(stdout, "%s ", pwd->pw_name);

  // groupe
  struct group *grp = getgrgid(info.st_uid);
  if (grp == NULL){
    perror("getgrgid");
    return FUN_FAILURE;
  }
  fprintf(stdout, "%s ", grp->gr_name);

  // taille
  fprintf(stdout, "%lld ", (long long) info.st_size);

  // date dernière modification
  struct tm *t = localtime(&info.st_mtime);
  if (t == NULL) {
    return FUN_FAILURE;
  }
  char str[50];
  strftime(str, 50, " %b. %d %H:%M", t);
  fprintf(stdout, "%s ", str);

  fprintf(stdout, "%s\n", filename);
  return FUN_SUCCESS;
}
