#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "BootStrapSector.h"

using namespace std;

// Extracts the files from the given disk image.
int main(int argc, char *argv[])
{
  int imageHandle;
  BootStrapSector *boot;
  
  if (argc != 2)
  {
    printf("Usage: %s FILE\n", argv[0]);
    printf("  FILE - The full or relative path of the disk image to read.\n");
    
    return -1;
  }
  
  // Open a file handle to the disk image
  imageHandle = open(argv[1], O_RDONLY);
  
  if (imageHandle == -1)
  {
    printf("Failed to open file: %s\n", argv[1]);
    
    return -1;
  }
  
  boot = new BootStrapSector(imageHandle);
  
  // Print number of files being extracted
  printf("Extracting: %d files\n", boot->getNumEntriesInRootDir());
  
  // Extract files, printing each file's name
  // TODO
  
  // Clean up stuff
  delete boot;
  close(imageHandle);
  
  return 0;
}
