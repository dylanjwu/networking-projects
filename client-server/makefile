#
#
#  Makefile for speak and speakd examples
#
#

# Edit the following line to reflect the current project number
PROJNUM = 1

HDR = speak.h speakd.h
SRC = speak.c speakd.c unixspeak.c unixspeakd.c

OBJ = unixspeak.o unixspeakd.o speakd.o speak.o
GRD = makefile ${SRC} ${HDR}

CC=gcc
CFLAGS = -g0

# Create all files

all:	speak speakd

speak:	unixspeak.o speak.o
	$(CC)	$(CFLAGS) unixspeak.o speak.o -o speak

speakd:	unixspeakd.o speakd.o history.o
	$(CC)	$(CFLAGS) unixspeakd.o speakd.o history.o -o speakd

unixspeak.o:	unixspeak.c speak.h
	$(CC)	$(CFLAGS) -c unixspeak.c
	
speak.o:	speak.c speak.h
	$(CC)	$(CFLAGS) -c speak.c

unixspeakd.o:	unixspeakd.c speakd.h
	$(CC)	$(CFLAGS) -c unixspeakd.c

speakd.o:	speakd.c speakd.h
	$(CC)	$(CFLAGS) -c speakd.c

#
# Clean up script
#
clean:
	/bin/rm -f *.o speak speakd

