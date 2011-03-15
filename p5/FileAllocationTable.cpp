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

// Retrieves the FAT entry for the given cluster.  The 12-bit FAT entry is
// prepended by 0s to a 16-bit integer.
int FileAllocationTable::getEntry(int cluster)
{
  byte entry[2];
  
  int value;
  
  // Seek to the start of the FAT in the disk image
  lseek(imageStream, startAddress, SEEK_SET);
  
  // Seek to the proper byte for this cluster number, and read the entry there
  lseek(imageStream, (3 * cluster) / 2, SEEK_CUR);
  read(imageStream, entry, 2);
  
  // Use the proper 12 bits, depending on the parity of the cluster number
  if (cluster % 2 == 0)
  {
    value = ((entry[1] & 0x0F) << 4) + entry[0];
  }
  else
  {
    value = (entry[1] << 4) + ((entry[0] & 0xF0) >> 4);
  }
  
  return value;
}
