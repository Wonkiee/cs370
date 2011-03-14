#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "BootStrapSector.h"

using namespace std;

// Produces a catalog listing of the given disk image.
int main(int argc, char *argv[])
{
  int imageHandle;
  
  BootStrapSector     *boot;
  FileAllocationTable *fat;
  DirectoryEntry      *rootDir;
  
  int  rootDirAddress;
  byte *sn;
  
  int e;
  
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
  
  // Read the boot sector and create references to the FAT and root directory
  boot = new BootStrapSector(imageHandle);
  fat  = new FileAllocationTable(imageHandle, boot->getNumBytesInReservedSectors());
  
  rootDirAddress = boot->getNumBytesInReservedSectors()
    + (boot->getNumBytesInFAT() * boot->getNumCopiesFAT());
  
  rootDir = new DirectoryEntry(imageHandle, rootDirAddress);
  
  // Print the catalog information
  printf("Volume name is %s\n", boot->getVolumeLabel());
  
  sn = boot->getVolumeSerialNumber();
  
  printf("Volume Serial Number is %X%X-%X%X", sn[0], sn[1], sn[2], sn[3]);
  printf("\n");
  
  // Print root directory entries
  for (e = 0; e < boot->getNumEntriesInRootDir(); e++)
  {
    // TODO
  }
  
  // Print directory summary
  printf("       %d file(s)        %d bytes\n", boot->getNumEntriesInRootDir(), 0);
  // TODO: Replace 0 above with total file size
  
  // Clean up stuff
  delete boot;
  close(imageHandle);
  
  return 0;
}
