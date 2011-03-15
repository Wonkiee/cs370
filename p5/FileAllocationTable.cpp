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

// Retrieves the FAT entry for the given cluster.  The returned value is the 12-
// bit entry 0-padded into a 16-bit integer.
int FileAllocationTable::getEntry(int cluster)
{
  // TODO
}
