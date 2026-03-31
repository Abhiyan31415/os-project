// whoami command (like Ubuntu's whoami)
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int uid = getuid();
  
  if(uid == 0){
    printf("root (uid=%d)\n", uid);
  } else {
    printf("user%d (uid=%d)\n", uid, uid);
  }
  
  exit(0);
}
