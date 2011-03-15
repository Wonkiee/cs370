CC = g++
DIRSRC=	msdosdir.cpp BootStrapSector.cpp DirectoryEntry.cpp
EXTRSRC=	msdosextr.cpp BootStrapSector.cpp FileAllocationTable.cpp DirectoryEntry.cpp
SAMPLE=	samplefat.bin

default : list extract
	

list : msdosdir $(SAMPLE)
	./msdosdir $(SAMPLE)

extract : msdosextr $(SAMPLE)
	./msdosextr $(SAMPLE)

msdosdir : $(DIRSRC)
	$(CC) -o msdosdir $(DIRSRC)

msdosextr : $(EXTRSRC)
	$(CC) -o msdosextr $(EXTRSRC)
