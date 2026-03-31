#include <stdio.h>
#include <stdint.h>

// Replicate the xv6 structures to check sizes
typedef uint16_t uint16;
typedef uint32_t uint;

#define NDIRECT 12

struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEVICE only)
  short minor;          // Minor device number (T_DEVICE only)
  short nlink;          // Number of links to inode in file system
  uint16 uid;           // Owner user ID
  uint16 mode;          // Permission bits
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};

int main() {
    printf("sizeof(struct dinode) = %lu bytes\n", sizeof(struct dinode));
    printf("Expected: 68 bytes\n");
    printf("BSIZE / sizeof(struct dinode) = %lu inodes per block\n", 1024 / sizeof(struct dinode));
    printf("Expected: 15 inodes per block\n");
    
    if (sizeof(struct dinode) == 68) {
        printf("dinode size is correct\n");
        return 0;
    } else {
        printf(" dinode size is incorrect\n");
        return 1;
    }
}
