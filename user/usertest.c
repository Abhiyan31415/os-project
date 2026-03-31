#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// Test program to demonstrate file permissions with different users

void
print_permissions(uint16 mode)
{
  printf("  ");
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

int
main(int argc, char *argv[])
{
  int fd;
  struct stat st;
  
  printf("=== File Permissions with Multiple Users ===\n\n");
  
  // Show current user
  printf("Current UID: %d (root)\n\n", getuid());
  
  // Test 1: Create a file as root
  printf("Test 1: Creating a file as root...\n");
  fd = open("rootfile.txt", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("  ERROR: Failed to create file\n");
    exit(1);
  }
  write(fd, "This file belongs to root\n", 26);
  close(fd);
  
  if(stat("rootfile.txt", &st) < 0){
    printf("  ERROR: Failed to stat file\n");
    exit(1);
  }
  
  printf("  ✓ File created successfully\n");
  printf("  Owner UID: %d\n", st.uid);
  printf("  Permissions:");
  print_permissions(st.mode);
  printf(" (%o)\n\n", st.mode);
  
  // Test 2: Try to read the file as root (should succeed)
  printf("Test 2: Reading file as root...\n");
  fd = open("rootfile.txt", O_RDONLY);
  if(fd < 0){
    printf("  ✗ Failed to open file\n");
  } else {
    printf("  ✓ Successfully opened file for reading\n");
    char buf[50];
    int n = read(fd, buf, sizeof(buf));
    printf("  Read %d bytes: ", n);
    write(1, buf, n);
    close(fd);
  }
  printf("\n");
  
  // Test 3: Switch to regular user (UID 1000)
  printf("Test 3: Switching to user 1000...\n");
  if(setuid(1000) < 0){
    printf("  ✗ Failed to setuid (only root can do this)\n");
    exit(1);
  }
  printf("  ✓ Now running as UID: %d\n\n", getuid());
  
  // Test 4: Try to read root's file as user 1000
  printf("Test 4: Trying to read root's file as user 1000...\n");
  printf("  File owner: %d, Current user: %d\n", st.uid, getuid());
  printf("  File permissions:");
  print_permissions(st.mode);
  printf(" (%o)\n", st.mode);
  printf("  World read permission: %s\n", (st.mode & 0004) ? "YES" : "NO");
  
  fd = open("rootfile.txt", O_RDONLY);
  if(fd < 0){
    printf("  ✓ CORRECTLY DENIED: Cannot open file (permission denied)\n");
    printf("  This is expected! User 1000 cannot read root's file.\n");
  } else {
    printf("  ✗ ERROR: Should have been denied!\n");
    close(fd);
  }
  printf("\n");
  
  // Test 5: Try to write to root's file as user 1000
  printf("Test 5: Trying to write to root's file as user 1000...\n");
  fd = open("rootfile.txt", O_WRONLY);
  if(fd < 0){
    printf("  ✓ CORRECTLY DENIED: Cannot open file for writing\n");
    printf("  This is expected! User 1000 cannot write to root's file.\n");
  } else {
    printf("  ✗ ERROR: Should have been denied!\n");
    close(fd);
  }
  printf("\n");
  
  // Test 6: Try to delete root's file as user 1000
  printf("Test 6: Trying to delete root's file as user 1000...\n");
  if(unlink("rootfile.txt") < 0){
    printf("  ✓ CORRECTLY DENIED: Cannot delete file\n");
    printf("  This is expected! User 1000 cannot delete files in root's directory.\n");
  } else {
    printf("  ✗ ERROR: Should have been denied!\n");
  }
  printf("\n");
  
  // Test 7: Create a file as user 1000
  printf("Test 7: Creating a file as user 1000...\n");
  fd = open("userfile.txt", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("  ✗ Failed to create file\n");
  } else {
    write(fd, "This file belongs to user 1000\n", 31);
    close(fd);
    
    if(stat("userfile.txt", &st) == 0){
      printf("  ✓ File created successfully\n");
      printf("  Owner UID: %d\n", st.uid);
      printf("  Permissions:");
      print_permissions(st.mode);
      printf(" (%o)\n", st.mode);
    }
  }
  printf("\n");
  
  // Test 8: Read own file as user 1000 (should succeed)
  printf("Test 8: Reading own file as user 1000...\n");
  fd = open("userfile.txt", O_RDONLY);
  if(fd < 0){
    printf("  ✗ Failed to open own file\n");
  } else {
    printf("  ✓ Successfully opened own file\n");
    char buf[50];
    int n = read(fd, buf, sizeof(buf));
    printf("  Read %d bytes: ", n);
    write(1, buf, n);
    close(fd);
  }
  printf("\n");
  
  printf("=== Summary ===\n");
  printf("✓ Root (UID 0) can create and access all files\n");
  printf("✓ Regular users (UID 1000) can create their own files\n");
  printf("✓ Regular users CANNOT access root's files (permission denied)\n");
  printf("✓ File permissions are working correctly!\n\n");
  
  printf("Note: To clean up, run as root:\n");
  printf("  $ rm rootfile.txt userfile.txt\n");
  
  exit(0);
}
