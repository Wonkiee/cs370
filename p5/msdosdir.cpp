#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "BootStrapSector.h"
#include "FileAllocationTable.h"
#include "DirectoryEntry.h"

using namespace std;

// Produces a catalog listing of the given disk image.
int main(int argc, char *argv[])
{
  int imageHandle;
  
  BootStrapSector     *boot;
  FileAllocationTable *fat;
  DirectoryEntry      **entries;
  
  byte
    *sn,
    *volumeLabel,
    *volumeLabelExt;
  
  int
    rootDirAddress,
    numEntries,
    e,
    numFiles  = 0,
    totalSize = 0;
  
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
  
  // Read the boot sector and locate the FAT and root directory
  boot = new BootStrapSector(imageHandle);
  fat  = new FileAllocationTable(imageHandle, boot->getNumBytesInReservedSectors());
  
  sn = boot->getVolumeSerialNumber();
  
  rootDirAddress = boot->getNumBytesInReservedSectors()
    + (boot->getNumBytesInFAT() * boot->getNumCopiesFAT());
  
  numEntries = boot->getNumEntriesInRootDir();
  
  // Scrape root directory entries
  entries = new DirectoryEntry*[boot->getNumEntriesInRootDir()];
  
  for (e = 0; e < numEntries; e++)
  {
    entries[e] = new DirectoryEntry(imageHandle, rootDirAddress + (e * 32));
    
    if (entries[e]->isVolumeLabel())
    {
      // Volume label is actually found as a root directory entry, apparently...
      volumeLabel    = entries[e]->getName();
      volumeLabelExt = entries[e]->getExtension();
    }
  }
  
  // Print catalog information
  printf("Volume name is %s%s\n", volumeLabel, volumeLabelExt);
  printf("Volume Serial Number is %X%X-%X%X\n", sn[3], sn[2], sn[1], sn[0]);
  printf("-------------------------------------------\n");
  
  for (e = 0; e < numEntries; e++)
  {
    if (entries[e]->isDeleted()
      || entries[e]->getName()[0] == 0x00
      || entries[e]->isVolumeLabel())
    {
      // Deleted file, empty entry, or entry for volume label: don't display
    }
    else
    {
      // Actual file or subdirectory
      printf("%s %s  %7d  %02d-%02d-%d  %02d:%02d:%02d\n",
        entries[e]->getName(),
        entries[e]->getExtension(),
        entries[e]->getFileSize(),
        entries[e]->getMonth(),
        entries[e]->getDay(),
        entries[e]->getYear(),
        entries[e]->getHour(),
        entries[e]->getMinute(),
        entries[e]->getSecond());
      
      numFiles++;
      totalSize += entries[e]->getFileSize();
    }
  }
  
  printf("-------------------------------------------\n");
  printf(" %3d file(s)  %7d bytes\n", numFiles, totalSize);
  
  // Clean up allocated memory
  delete boot;
  boot = NULL;
  
  delete fat;
  fat = NULL;
  
  for (e = 0; e < numEntries; e++)
  {
    delete entries[e];
    entries[e] = NULL;
  }
  
  delete [] entries;
  entries = NULL;
  
  close(imageHandle);
  
  return 0;
}
