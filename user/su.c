// su command - switch user (like Ubuntu's su)
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int target_uid;
  int current_uid = getuid();

  if(argc != 2){
    fprintf(2, "Usage: su UID\n");
    fprintf(2, "Example: su 0    (switch to root)\n");
    fprintf(2, "Example: su 1000 (switch to user 1000)\n");
    exit(1);
  }

  target_uid = atoi(argv[1]);
  
  if(target_uid < 0 || target_uid > 65535){
    fprintf(2, "su: invalid UID '%s'\n", argv[1]);
    exit(1);
  }

  // Only root can switch to any user
  // Non-root users can only switch to themselves (no-op)
  if(current_uid != 0 && current_uid != target_uid){
    fprintf(2, "su: permission denied (only root can switch users)\n");
    exit(1);
  }

  if(setuid(target_uid) < 0){
    fprintf(2, "su: failed to set UID to %d\n", target_uid);
    exit(1);
  }

  printf("Switched to UID %d\n", target_uid);
  
  // Start a new shell with the new UID
  char *args[] = { "sh", 0 };
  exec("sh", args);
  
  fprintf(2, "su: exec sh failed\n");
  exit(1);
}
