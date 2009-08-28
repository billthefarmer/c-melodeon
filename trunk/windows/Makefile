################################################################################
#
#  Makefile for Melodeon
#
################################################################################

GCC	= gcc

all:	Melodeon.exe

Melodeon.exe:	Melodeon.c Melodeon.o

clean:
	rm *.exe

%.exe:	%.c
	$(GCC) $< -o $* -mwindows -lcomctl32 -lwinmm

%.o:	%.rc
	$(WINDRES) -o $@ $<
