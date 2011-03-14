#include <fcntl.h>
#include <unistd.h>
#include "BootStrapSector.h"

using namespace std;

// Initializes a new BootStrapSector.
BootStrapSector::BootStrapSector(int is)
{
  imageStream = is;
  readBootStrapSector();
}

// Returns the number of bytes in the FAT.
int BootStrapSector::getNumBytesInFAT()
{
  int fatSectors  = (numSectorsInFAT[1] << 8)   + numSectorsInFAT[0],
      sectorBytes = (numBytesPerSector[1] << 8) + numBytesPerSector[0];
  
  return fatSectors * sectorBytes;
}

// Returns the number of clusters on the disk.
int BootStrapSector::getNumClusters()
{
  int diskSectors = (numSectors[1] << 8) + numSectors[0];
  
  return diskSectors / numSectorsPerCluster[0];
}

// Returns the number of entries in the root directory.
int BootStrapSector::getNumEntriesInRootDir()
{
  return (numEntriesRootDir[1] << 8) + numEntriesRootDir[0];
}

// Returns the number of bytes in reserved sectors.
int BootStrapSector::getNumBytesInReservedSectors()
{
  int reservedSectors = (numReservedSectors[1] << 8) + numReservedSectors[0],
      sectorBytes     = (numBytesPerSector[1] << 8)  + numBytesPerSector[0];
  
  return reservedSectors * sectorBytes;
}

// Returns the total number of copies of the FAT.
int BootStrapSector::getNumCopiesFAT()
{
  return numCopiesFAT[0];
}

// Returns the number of bytes per cluster.
int BootStrapSector::getNumBytesPerCluster()
{
  int sectorBytes = (numBytesPerSector[1] << 8) + numBytesPerSector[0];
  
  return numSectorsPerCluster[0] * sectorBytes;
}

// Returns the volume label, padded with blanks to a length of 11 bytes.
byte* BootStrapSector::getVolumeLabel()
{
  return volumeLabel;
}

// Returns the volume ID (serial number).
byte* BootStrapSector::getVolumeSerialNumber()
{
  return volumeSN;
}

// Returns the FAT file system type, padded with blanks to a length of 8 bytes.
byte* BootStrapSector::getFormatType()
{
  return formatType;
}

// Reads the bootstrap sector from the disk image and populates the related
// fields.
// 
// NOTE: Currently, there is no checking for I/O errors here.
void BootStrapSector::readBootStrapSector()
{
  // Read values sequentially from the boot sector
  read(imageStream, firstInstruction,     3);
  read(imageStream, OEM,                  8);
  read(imageStream, numBytesPerSector,    2);
  read(imageStream, numSectorsPerCluster, 1);
  read(imageStream, numReservedSectors,   2);
  read(imageStream, numCopiesFAT,         1);
  read(imageStream, numEntriesRootDir,    2);
  read(imageStream, numSectors,           2);
  read(imageStream, mediaDescriptor,      1);
  read(imageStream, numSectorsInFAT,      2);
  read(imageStream, numSectorsPerTrack,   2);
  read(imageStream, numSides,             2);
  read(imageStream, numHiddenSectors,     2);
  
  // Read other values from the Extended BIOS Parameter Block
  lseek(imageStream, 0x36, SEEK_SET);
  read(imageStream, formatType, 8);
  formatType[8] = '\0';
  
  lseek(imageStream, 0x2B, SEEK_SET);
  read(imageStream, volumeLabel, 11);
  volumeLabel[11] = '\0';
  
  lseek(imageStream, 0x27, SEEK_SET);
  read(imageStream, volumeSN, 4);
  volumeSN[4] = '\0';
  
  lseek(imageStream, 0x1FE, SEEK_SET);
  read(imageStream, hex55AA, 2);
}
