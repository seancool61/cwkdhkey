# compiler
CC=cl.exe
# linker
LINK=link.exe
# ccflags
CCFLAGS=/EHsc

.PHONY	: all
all : dhkey
  
# Link 
dhkey : dhkey.o
	$(LINK) /OUT:dhkey.exe dhkey.o /LIBPATH:c:\boost_1_66_0\stage\lib
 
# compile
dhkey.o	: dhkey.cpp
	$(CC) $(CCFLAGS) /c $*.cpp /Fodhkey.o -IC:\boost_1_66_0

# CLEAN
clean :
	del dhkey.o dhkey.exe