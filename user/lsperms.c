// ls with permissions display (like ls -l in Ubuntu)
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
print_perms(uint16 mode)
{
  printf((mode & 0400) ? "r" : "-");
  printf((mode & 0200) ? "w" : "-");
  printf((mode & 0100) ? "x" : "-");
  printf((mode & 0040) ? "r" : "-");
  printf((mode & 0020) ? "w" : "-");
  printf((mode & 0010) ? "x" : "-");
  printf((mode & 0004) ? "r" : "-");
  printf((mode & 0002) ? "w" : "-");
  printf((mode & 0001) ? "x" : "-");
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "lsperms: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "lsperms: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    print_perms(st.mode);
    printf(" %d %d %s %d\n", st.uid, st.nlink, fmtname(path), (int)st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("lsperms: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("lsperms: cannot stat %s\n", buf);
        continue;
      }
      print_perms(st.mode);
      printf(" %d %d %s %d\n", st.uid, st.nlink, fmtname(buf), (int)st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
