/**
 * Represents the FAT.
 */

#ifndef FILE_ALLOCATION_TABLE_H
#define FILE_ALLOCATION_TABLE_H

class FileAllocationTable
{
  public:
    FileAllocationTable(int is, int start);
  
	private:
    void something();
    
    int imageStream;
    int startAddress;
};

#endif
