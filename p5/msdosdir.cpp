#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "BootStrapSector.h"

using namespace std;

// Produces a catalog listing of the given disk image.
int main(int argc, char *argv[])
{
  int imageHandle;
  BootStrapSector *boot;
  byte* sn;
  
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
  
  // Print the catalog information
  printf("Volume name is %s\n", boot->getVolumeLabel());
  
  sn = boot->getVolumeSerialNumber();
  
  printf("Volume Serial Number is %X%X-%X%X", sn[0], sn[1], sn[2], sn[3]);
  printf("\n");
  
  // Print root directory entries
  // TODO
  
  // Print directory summary
  printf("       %d file(s)        %d bytes\n", boot->getNumEntriesInRootDir(), 0);
  // TODO: Replace 0 above with total file size
  
  // Clean up stuff
  delete boot;
  close(imageHandle);
  
  return 0;
}
