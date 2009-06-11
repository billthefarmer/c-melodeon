################################################################################
#
#  Makefile for Melodeon
#
################################################################################

GCC	= gcc

all:	Melodeon

clean:
	rm Melodeon

%:	%.c
	$(GCC) $< -o $* `pkg-config --cflags --libs gtk+-2.0 fluidsynth`
