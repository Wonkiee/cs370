/**
 * BootStrapSector is the first 512 bytes of the FAT.
 * Two-byte fields are little-endian.
 *
 * The format of this sector is:
 * byte(s) contents
 * =============================================================================
 *  0-2     first instruction of bootstrap routine
 *  3-10    OEM name
 *  11-12   number of bytes per sector
 *  13      number of sectors per cluster
 *  14-15   number of reserved sectors
 *  16      number of copies of the file allocation table
 *  17-18   number of entries in root directory
 *  19-20   total number of sectors
 *  21      media descriptor byte
 *  22-23   number of sectors in each copy of file allocation table
 *  24-25   number of sectors per track
 *  26-27   number of sides
 *  28-29   number of hidden sectors
 *  30-509  bootstrap routine and partition information
 *  510     hexadecimal 55
 *  511     hexadecimal AA
 */

#define byte unsigned char

#ifndef BOOTSTRAP_SECTOR_H
#define BOOTSTRAP_SECTOR_H

class BootStrapSector
{
  public:
    BootStrapSector(int is);
    int getNumBytesInFAT();
    int getNumClusters();
    int getNumEntriesInRootDir();
    int getNumBytesInReservedSectors();
    int getNumCopiesFAT();
    int getNumBytesPerCluster();
    byte* getVolumeLabel();
    byte* getVolumeSerialNumber();
    byte* getFormatType();
  
	private:
    void readBootStrapSector();
    
    int imageStream;
    
    byte firstInstruction[3];  // This is often a jump instruction to the boot sector code itself
    byte OEM[8];  		
    byte numBytesPerSector[2];
    byte numSectorsPerCluster[1];
    byte numReservedSectors[2];
    byte numCopiesFAT[1];
    byte numEntriesRootDir[2];
    byte numSectors[2];
    byte mediaDescriptor[1];
    byte numSectorsInFAT[2];
    byte numSectorsPerTrack[2];
    byte numSides[2];
    byte numHiddenSectors[2];
    byte formatType[9]; // FAT12 or FAT16 in this program
    byte hex55AA[2];	// the last bytes of the boot sector are, by definition, 55 AA.  This is a sanity check.
    byte volumeLabel[12];
    byte volumeSN[5];
};

#endif
