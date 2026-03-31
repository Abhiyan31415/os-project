// chmod command (like Ubuntu's chmod)
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Convert octal string to integer
int
octal_to_int(char *str)
{
  int result = 0;
  while(*str){
    if(*str < '0' || *str > '7'){
      return -1;
    }
    result = result * 8 + (*str - '0');
    str++;
  }
  return result;
}

int
main(int argc, char *argv[])
{
  int mode;

  if(argc != 3){
    fprintf(2, "Usage: chmod MODE FILE\n");
    fprintf(2, "Example: chmod 644 myfile\n");
    fprintf(2, "Example: chmod 755 mydir\n");
    exit(1);
  }

  mode = octal_to_int(argv[1]);
  if(mode < 0 || mode > 0777){
    fprintf(2, "chmod: invalid mode '%s'\n", argv[1]);
    fprintf(2, "Mode must be octal (0-777)\n");
    exit(1);
  }

  if(chmod(argv[2], mode) < 0){
    fprintf(2, "chmod: cannot change permissions of '%s'\n", argv[2]);
    exit(1);
  }

  exit(0);
}
