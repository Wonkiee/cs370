#include <fcntl.h>
#include <unistd.h>
#include "DirectoryEntry.h"

using namespace std;

// Initializes a new DirectoryEntry.  The entry starts at the given byte
// address in the given file.
DirectoryEntry::DirectoryEntry(int is, int start)
{
  imageStream  = is;
  startAddress = start;
  
  readDirectoryEntry();
}

// Returns the name of the file or subdirectory associated with this entry.
byte* DirectoryEntry::getName()
{
  return name;
}

// Returns the file extension contained in this entry.
byte* DirectoryEntry::getExtension()
{
  return extension;
}

// Returns whether this entry refers to a file that has been deleted.
bool DirectoryEntry::isDeleted()
{
  return deleted;
}

// Returns whether this entry refers to a read-only file.
bool DirectoryEntry::isReadOnly()
{
  return (attributes[0] & 0x01) == 0x01;
}

// Returns whether this entry refers to a hidden file/directory.
bool DirectoryEntry::isHidden()
{
  return (attributes[0] & 0x02) == 0x02;
}

// Returns whether this entry refers to a system file.
bool DirectoryEntry::isSystemFile()
{
  return (attributes[0] & 0x04) == 0x04;
}

// Returns whether this entry refers to a volume label.
bool DirectoryEntry::isVolumeLabel()
{
  return (attributes[0] & 0x08) == 0x08;
}

// Returns whether this entry refers to a subdirectory.
bool DirectoryEntry::isSubdirectory()
{
  return (attributes[0] & 0x10) == 0x10;
}

// Returns whether this entry has its "archive" attribute set.
bool DirectoryEntry::isArchived()
{
  return (attributes[0] & 0x20) == 0x20;
}

// Returns the year of the file's creation.
int DirectoryEntry::getYear()
{
  return 1980 + (date[1] >> 1);
}

// Returns the month of the file's creation.
int DirectoryEntry::getMonth()
{
  return ((date[1] & 0x01) << 3) + (date[0] >> 5);
}

// Returns the day (of the month) of the file's creation.
int DirectoryEntry::getDay()
{
  return date[0] & 0x1F;
}

// Returns the hour of the file's creation.
int DirectoryEntry::getHour()
{
  return time[1] >> 3;
}

// Returns the minute of the file's creation.
int DirectoryEntry::getMinute()
{
  return ((time[1] & 0x07) << 3) + (time[0] >> 5);
}

// Returns the second of the file's creation.
int DirectoryEntry::getSecond()
{
  return time[0] & 0x1F;
}

// Returns the starting cluster of the file or directory.  If the file is empty,
// this should be 0.
int DirectoryEntry::getStartingCluster()
{
  return (startingCluster[1] << 8) + startingCluster[0];
}

// Returns the size of the file, in bytes.
int DirectoryEntry::getFileSize()
{
  return (fileSize[3] << 24)
    + (fileSize[2] << 16)
    + (fileSize[1] << 8)
    + fileSize[0];
}

// Reads the directory entry from the disk image and populates the related
// fields.
// 
// NOTE: Currently, there is no checking for I/O errors here.
void DirectoryEntry::readDirectoryEntry()
{
  // Seek to the start of the entry in the disk image
  lseek(imageStream, startAddress, SEEK_SET);
  
  // Read values sequentially from the directory entry
  read(imageStream, name,            8);
  read(imageStream, extension,       3);
  read(imageStream, attributes,      1);
  read(imageStream, reserved,        10);
  read(imageStream, time,            2);
  read(imageStream, date,            2);
  read(imageStream, startingCluster, 2);
  read(imageStream, fileSize,        4);
  
  // Null-terminate string values
  name[8] = '\0';
  extension[3] = '\0';
  
  // Check for deleted file
  deleted = (name[0] == 0xE5);
  
  // Fix file name if necessary
  if (name[0] == 0x05)
  {
    name[0] = 0xE5;
  }
}
