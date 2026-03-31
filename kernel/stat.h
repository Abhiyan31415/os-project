#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

// Permission bit definitions (Unix-style)
// Only define if not already defined by system headers (for mkfs compatibility)
#ifndef S_IRUSR
#define S_IRUSR 0400  // Owner: read permission
#define S_IWUSR 0200  // Owner: write permission
#define S_IXUSR 0100  // Owner: execute permission
#define S_IROTH 0004  // World: read permission
#define S_IWOTH 0002  // World: write permission
#define S_IXOTH 0001  // World: execute permission

// Convenience macros for permission combinations
#define S_IRWXU 0700  // Owner: rwx
#define S_IRWXO 0007  // World: rwx
#endif

struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
  uint16 uid;  // Owner user ID
  uint16 mode; // Permission bits
};
