################################################################################
#
#  Makefile for Melodeon and Sruti box
#
################################################################################

GCC	= gcc

all:	Melodeon.exe Srutibox.exe

clean:
	rm *.exe

%.exe:	%.c
	$(GCC) $< -o $* -mwindows -lcomctl32 -lwinmm
