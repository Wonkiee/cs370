/**
 * Represents the FAT.
 */

#define byte unsigned char

#ifndef FILE_ALLOCATION_TABLE_H
#define FILE_ALLOCATION_TABLE_H

class FileAllocationTable
{
  public:
    FileAllocationTable(int is, int start);
    
    int getEntry(int cluster);
  
	private:
    int imageStream;
    int startAddress;
};

#endif
