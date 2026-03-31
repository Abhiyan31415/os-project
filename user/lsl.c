#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// ls -l style program that shows permissions and ownership

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
print_permissions(uint16 mode)
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

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "lsl: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "lsl: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s ", st.type == T_DIR ? "d" : "-");
    print_permissions(st.mode);
    printf(" %5d %5d %s\n", st.uid, (int)st.size, fmtname(path));
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("lsl: path too long\n");
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
        printf("lsl: cannot stat %s\n", buf);
        continue;
      }
      printf("%s ", st.type == T_DIR ? "d" : "-");
      print_permissions(st.mode);
      printf(" %5d %5d %s\n", st.uid, (int)st.size, fmtname(buf));
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  printf("Type Permissions  UID   Size Name\n");
  printf("---- ----------- ----- ------ --------------\n");
  
  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
