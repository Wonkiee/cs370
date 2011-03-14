#include <fcntl.h>
#include <unistd.h>
#include "FileAllocationTable.h"

using namespace std;

// Initializes a new FileAllocationTable.  The FAT starts at the given byte
// address in the given file.
FileAllocationTable::FileAllocationTable(int is, int start)
{
  imageStream  = is;
  startAddress = start;
}

// TOOD
void FileAllocationTable::something()
{
  // TODO
}
