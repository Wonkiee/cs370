#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "BootStrapSector.h"
#include "FileAllocationTable.h"
#include "DirectoryEntry.h"

using namespace std;

// Extracts the file or subdirectory associated with the given directory entry.
void extract(
  DirectoryEntry *entry,
  FileAllocationTable *fat,
  BootStrapSector *bootSector,
  int imageHandle)
{
  int
    c,
    fileHandle,
    cluster,
    nextCluster,
    bytesPerCluster = bootSector->getNumBytesPerCluster(),
    dataStartAddress = bootSector->getNumBytesInReservedSectors()
      + (bootSector->getNumBytesInFAT() * bootSector->getNumCopiesFAT())
      + (bootSector->getNumEntriesInRootDir() * 32);
  
  byte
    *name = entry->getName(),
    *ext = entry->getExtension();
  
  char
    separator,
    fullName[13],
    buffer[bytesPerCluster];
  
  if (entry->isDeleted()
    || entry->getName()[0] == 0x00
    || entry->isVolumeLabel())
  {
    // Deleted file, empty entry, or entry for volume label: don't do anything
    return;
  }
  
  // Truncate name and extension and combine into full filename
  for (c = 0; c < 8; c++)
  {
    if (name[c] == 0x20)
    {
      name[c] = '\0';
    }
    
    if ((c < 3) && (ext[c] == 0x20))
    {
      ext[c] = '\0';
    }
  }
  
  if (ext[0] == '\0')
  {
    separator = '\0';
  }
  else
  {
    separator = '.';
  }
  
  sprintf(fullName, "%s%c%s", name, separator, ext);
  printf("%s\n", fullName);
  
  if (entry->isSubdirectory())
  {
    // Subdirectory: Create directory and recursively extract files within
    // NOTE: NYI - Required for this assignment?
  }
  else
  {
    // File: Extract data
    // Create/open file for writing
    fileHandle = open(fullName, O_WRONLY | O_CREAT | O_TRUNC);
    
    if (fileHandle == -1)
    {
      printf("Failed to create file for extraction: %s\n", fullName);
      
      return;
    }
    
    // Traverse FAT and extract data from each cluster referenced
    cluster = entry->getStartingCluster();
    
    // NOTE: This currently doesn't check for bad clusters or reserved values
    while (cluster != 0 && cluster < 0xFF8)
    {
      // Read the cluster from the disk image
      lseek(
          imageHandle,
          dataStartAddress + ((cluster - 2) * bytesPerCluster),
          SEEK_SET);
      
      read(imageHandle, buffer, bytesPerCluster);
      
      // Get next cluster entry from the FAT
      nextCluster = fat->getEntry(cluster);
      
      if (nextCluster >= 0xFF8)
      {
        // This is the last cluster; write only until end of file
        write(fileHandle, buffer, entry->getFileSize() % bytesPerCluster);
      }
      else
      {
        // Write full cluster, then go to the next cluster
        write(fileHandle, buffer, bytesPerCluster);
      }
      
      cluster = nextCluster;
    }
    
    close(fileHandle);
  }
}

// Extracts the files from the root directory of the given disk image.
int main(int argc, char *argv[])
{
  int imageHandle;
  
  BootStrapSector     *boot;
  FileAllocationTable *fat;
  DirectoryEntry      **entries;
  
  int
    rootDirAddress,
    numEntries,
    e,
    numFiles = 0;
  
  char *fileName;
  
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
  
  fat  = new FileAllocationTable(
    imageHandle,
    boot->getNumBytesInReservedSectors());
  
  rootDirAddress = boot->getNumBytesInReservedSectors()
    + (boot->getNumBytesInFAT() * boot->getNumCopiesFAT());
  
  numEntries = boot->getNumEntriesInRootDir();
  
  // Scrape root directory entries
  entries = new DirectoryEntry*[boot->getNumEntriesInRootDir()];
  
  for (e = 0; e < numEntries; e++)
  {
    entries[e] = new DirectoryEntry(imageHandle, rootDirAddress + (e * 32));
    
    if (entries[e]->isDeleted()
      || entries[e]->getName()[0] == 0x00
      || entries[e]->isVolumeLabel())
    {
      // Deleted file, empty entry, or entry for volume label
    }
    else
    {
      numFiles++;
    }
  }
  
  // Extract files
  printf("Extracting: %d files\n", numFiles);
  printf("------------\n");
  
  for (e = 0; e < numEntries; e++)
  {
    extract(entries[e], fat, boot, imageHandle);
  }
  
  printf("--- done ---\n");
  
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
