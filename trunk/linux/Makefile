################################################################################
#
#  Makefile for Melodeon and Sruti box
#
################################################################################

GCC	= gcc

all:	Melodeon

clean:
	rm *.exe

%:	%.c
	$(GCC) $< -o $* `pkg-config --cflags --libs gtk+-2.0`
