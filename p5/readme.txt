CS 370 - Operating Systems
==========================
Team Members:
  Jason Economou
  Evan Haas
  Ashwin Hamal
  Phil Trinh


Links
=====
The team page/wiki is located at:

http://code.google.com/p/cs370

-----

To check out the source tree:

svn checkout http://cs370.googlecode.com/svn/trunk/ cs370-read-only

-----

For write access to the repository, contact one of the team members.



Project 5: MS-DOS FAT Reader/Extractor
======================================
For this project, we wrote C++ programs to read and extract file data from a
FAT12 disk image.  The resources linked in the project description proved
valuable, as the MS-DOS file system has several odd rules that have to be
accounted for.

The main difficulty was translating the 12-bit packed FAT entries and
little-endian fields.  We were very thankful that the sample disk image
contained a number of plain text files, which made it easy to tell when we had
done the bit shifting and arithmatic correctly.

For this assignment, all code has been submitted alongside this readme.  It may
also be found in the "p5" folder in the repository.  The code has been tested in
the tux environment.

Type "make" to test both programs against the sample disk image (included).
To test only the reader, type "make list".
To test only the extractor, type "make extract".

To test either program with a different disk image, use one of the following:
  ./msdosdir FILE
  ./msdosextr FILE
