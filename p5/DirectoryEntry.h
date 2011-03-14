/**
 * Represents a directory entry on a FAT disk.
 * 
 * 2-byte fields are little-endian.
 * 
 * The format of an entry is:
 * byte(s) contents
 * =============================================================================
 * 0-7     file name
 *           padded with spaces (0x20)
 *           first byte is 0xE5 if file was deleted
 *           first byte is 0x05 if real name starts with 0xE5
 * 
 * 8-10    file extension
 * 
 * 11      attributes - a bit-vector with values as follows:
 *           bit 0 = read-only
 *           bit 1 = hidden
 *           bit 2 = system file
 *           bit 3 = volume label
 *           bit 4 = subdirectory (first two sub-entries will be for . and ..)
 *           bit 5 = archive
 *           bit 6 = unused
 *           bit 7 = unused
 * 
 * 12-21   reserved
 * 
 * 22-23   time
 *           5 bits for hours
 *           6 bits for minutes
 *           5 bits for 2 * seconds
 * 
 * 24-25   date
 *           7 bits for year since 1980
 *           4 bits for month
 *           5 bits for day
 * 
 * 26-27   starting cluster (0 if file is empty)
 * 
 * 28-31   size of file, in bytes
 */

#define byte unsigned char

#ifndef DIRECTORY_ENTRY_H
#define DIRECTORY_ENTRY_H

class DirectoryEntry
{
  public:
    DirectoryEntry(int is, int start);
    byte* getName();
    byte* getExtension();
    
    bool isDeleted();
    bool isHidden();
    bool isReadOnly();
    bool isSystemFile();
    bool isVolumeLabel();
    bool isSubdirectory();
    bool isArchived();
    
    int getYear();
    int getMonth();
    int getDay();
    int getHour();
    int getMinute();
    int getSecond();
    
    int getStartingCluster();
    int getFileSize();
  
	private:
    void readDirectoryEntry();
    
    int imageStream;
    int startAddress;
    
    bool deleted;
    
    byte name[9];
    byte extension[4];
    byte attributes[1];
    byte reserved[10];
    byte time[2];
    byte date[2];
    byte startingCluster[2];
    byte fileSize[4];
};

#endif
