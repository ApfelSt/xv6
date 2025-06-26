#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#define SIZE 512
#define SLASH '/'
#define SPACE ' '
#define ME "."
#define FATHER ".."

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != SLASH; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), SPACE, DIRSIZ-strlen(p));
  return buf;
}

void find(char *dir, char *name)
{
  char buf[SIZE], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(dir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dir);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", dir);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(strcmp(fmtname(dir), name)==0){
    printf("%s\n", dir);
    }
    break;

  case T_DIR:
    if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: dir too long\n");
      break;
    }
    /*if(strcmp(dir,".")==0 || strcmp(dir, "..")){
        fprintf(1, "find: oops %s\n", dir);
        break;
    }*/
    strcpy(buf, dir);
    p = buf+strlen(buf);
    *p++ = SLASH;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name,ME)==0 || strcmp(de.name, FATHER)==0)
          continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, name);
    }
    break;
  }
  close(fd);
}



int
main(int argc, char *argv[])
{
  char name[DIRSIZ+1];
  strcpy(name, fmtname(argv[2]));
  find(argv[1], name);
  exit(0);
}
