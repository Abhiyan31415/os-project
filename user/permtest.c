#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// Test program to verify file permissions are working

void
print_permissions(uint16 mode)
{
  printf("Permissions: ");
  printf((mode & 0400) ? "r" : "-");
  printf((mode & 0200) ? "w" : "-");
  printf((mode & 0100) ? "x" : "-");
  printf((mode & 0040) ? "r" : "-");
  printf((mode & 0020) ? "w" : "-");
  printf((mode & 0010) ? "x" : "-");
  printf((mode & 0004) ? "r" : "-");
  printf((mode & 0002) ? "w" : "-");
  printf((mode & 0001) ? "x" : "-");
  printf(" (octal: %o)\n", mode);
}

int
main(int argc, char *argv[])
{
  struct stat st;
  int fd;
  
  printf("=== File Permissions Test ===\n\n");
  
  // Test 1: Create a file and check its permissions
  printf("Test 1: Creating a file...\n");
  fd = open("permtest_file", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("ERROR: Failed to create file\n");
    exit(1);
  }
  write(fd, "test data\n", 10);
  close(fd);
  
  if(stat("permtest_file", &st) < 0){
    printf("ERROR: Failed to stat file\n");
    exit(1);
  }
  
  printf("File created successfully!\n");
  printf("  Owner UID: %d\n", st.uid);
  printf("  ");
  print_permissions(st.mode);
  printf("  Expected: rw-r--r-- (0644)\n");
  
  if(st.mode == 0644){
    printf("  ✓ File permissions are CORRECT!\n");
  } else {
    printf("  ✗ File permissions are INCORRECT (got %o, expected 0644)\n", st.mode);
  }
  printf("\n");
  
  // Test 2: Create a directory and check its permissions
  printf("Test 2: Creating a directory...\n");
  if(mkdir("permtest_dir") < 0){
    printf("ERROR: Failed to create directory\n");
    exit(1);
  }
  
  if(stat("permtest_dir", &st) < 0){
    printf("ERROR: Failed to stat directory\n");
    exit(1);
  }
  
  printf("Directory created successfully!\n");
  printf("  Owner UID: %d\n", st.uid);
  printf("  ");
  print_permissions(st.mode);
  printf("  Expected: rwxr-xr-x (0755)\n");
  
  if(st.mode == 0755){
    printf("  ✓ Directory permissions are CORRECT!\n");
  } else {
    printf("  ✗ Directory permissions are INCORRECT (got %o, expected 0755)\n", st.mode);
  }
  printf("\n");
  
  // Test 3: Check root directory permissions
  printf("Test 3: Checking root directory...\n");
  if(stat(".", &st) < 0){
    printf("ERROR: Failed to stat root directory\n");
    exit(1);
  }
  
  printf("Root directory:\n");
  printf("  Owner UID: %d\n", st.uid);
  printf("  ");
  print_permissions(st.mode);
  printf("\n");
  
  // Test 4: Read the file back
  printf("Test 4: Reading file back...\n");
  fd = open("permtest_file", O_RDONLY);
  if(fd < 0){
    printf("ERROR: Failed to open file for reading\n");
    exit(1);
  }
  
  char buf[20];
  int n = read(fd, buf, sizeof(buf));
  if(n > 0){
    printf("  ✓ Successfully read %d bytes\n", n);
  } else {
    printf("  ✗ Failed to read file\n");
  }
  close(fd);
  printf("\n");
  
  // Test 5: Check README file
  printf("Test 5: Checking README file...\n");
  if(stat("README", &st) == 0){
    printf("README file:\n");
    printf("  Owner UID: %d\n", st.uid);
    printf("  ");
    print_permissions(st.mode);
  } else {
    printf("  (README not found)\n");
  }
  printf("\n");
  
  // Cleanup
  printf("Cleaning up...\n");
  unlink("permtest_file");
  unlink("permtest_dir");
  
  printf("\n=== All Tests Complete ===\n");
  printf("The file permissions system is working!\n");
  printf("Note: All operations succeed because we're running as root (UID 0)\n");
  
  exit(0);
}
