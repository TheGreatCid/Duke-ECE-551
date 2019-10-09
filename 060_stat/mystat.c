#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void getpermission(long st_mode, char * permission) {
  //last character
  permission[10] = '\0';

  //first character
  switch (st_mode & S_IFMT) {
    case S_IFBLK:
      permission[0] = 'b';
      break;
    case S_IFCHR:
      permission[0] = 'c';
      break;
    case S_IFDIR:
      permission[0] = 'd';
      break;
    case S_IFIFO:
      permission[0] = 'p';
      break;
    case S_IFLNK:
      permission[0] = 'l';
      break;
    case S_IFREG:
      permission[0] = '-';
      break;
    case S_IFSOCK:
      permission[0] = 's';
      break;
    default:
      permission[0] = '?';
      fprintf(stderr, "filetype unknown\n");
      break;
  }

  //second character
  permission[1] = ((st_mode & S_IRUSR) != 0) ? 'r' : '-';
  //third character
  permission[2] = ((st_mode & S_IWUSR) != 0) ? 'w' : '-';
  //forth character
  permission[3] = ((st_mode & S_IXUSR) != 0) ? 'x' : '-';
  //fifth character
  permission[4] = ((st_mode & S_IRGRP) != 0) ? 'r' : '-';
  //sixth character
  permission[5] = ((st_mode & S_IWGRP) != 0) ? 'w' : '-';
  //seventh character
  permission[6] = ((st_mode & S_IXGRP) != 0) ? 'x' : '-';
  //eighth character
  permission[7] = ((st_mode & S_IROTH) != 0) ? 'r' : '-';
  //ninth character
  permission[8] = ((st_mode & S_IWOTH) != 0) ? 'w' : '-';
  //tenth character
  permission[9] = ((st_mode & S_IXOTH) != 0) ? 'x' : '-';
}

int printfstat(char * filename) {
  struct stat sb;

  if (lstat(filename, &sb) == -1) {
    fprintf(stderr, "Failure in reading stat into a struct\n");
    exit(EXIT_FAILURE);
  }

  //first line
  printf("  File: %s\n", filename);

  //second line
  char * filetype;
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
      filetype = "block device";
      break;
    case S_IFCHR:
      filetype = "character device";
      break;
    case S_IFDIR:
      filetype = "directory";
      break;
    case S_IFIFO:
      filetype = "FIFO/pipe";
      break;
    case S_IFLNK:
      filetype = "symlink";
      break;
    case S_IFREG:
      filetype = "regular file";
      break;
    case S_IFSOCK:
      filetype = "socket";
      break;
    default:
      filetype = "unknown?";
      break;
  }

  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         sb.st_size,
         sb.st_blocks,
         sb.st_blksize,
         filetype);

  //third line
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
         sb.st_dev,
         sb.st_dev,
         sb.st_ino,
         sb.st_nlink);

  //forth line
  char permission[11] = "";
  getpermission(sb.st_mode, permission);
  printf("Access: (%04o/%s)\n", sb.st_mode & S_IFMT, permission);

  return 0;
}

//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr, "missing input operand\n");
    return EXIT_FAILURE;
  }

  for (int i = 1; i < argc; i++) {
    printfstat(argv[i]);
  }
  return EXIT_SUCCESS;
}
