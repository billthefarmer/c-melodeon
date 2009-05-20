                              Melodeon
                              ========

Simple one row melodeon emulator with bass. Uses the standard PC
keyboard function keys F1-F12 as melodeon buttons, the Control and Alt
keys as bass buttons, and the space bar as the bellows. Unfortunately,
depending on your luck, the keyboard on your PC may not detect more
than two or three buttons pressed at once. I found an old keyboard
worked much better. You can see what is going on in the bottom half of
the melodeon window, which shows what keys the program thinks are
pressed.

Controls
========

There are three drop down controls, a check box, a slider, and a
button in the top half of the window. The instruments drop down lets
you choose what midi instrument you would like to attempt to play. The
reverse buttons check box lets you play widdershins. The key drop down
lets you choose a key to play in. There is a choice of Eb, Bb, F, C,
G, D, A. The key defaults to C. More kays can be added by popular
request, or you can hack the program yourself. The layout drop down
lets you choose a layout. The volume slider lets you change the
volume. The volume defaults to maximum. The quit button lets you give
up, or you can click on the window close icon. The key may be changed
by pressing the letter key corresponding to the desired playing
key. The buttons may be reversed by pressing the 'R' key.

Playing
=======

Hold the PC keyboard in a vertical position, so the function keys are
on the side you want to play the melody on. Adjust the reverse check
box according to which hand you want to use. Press the keys as if they
were melodeon buttons, and press the space bar to emulate waggling the
bellows.

                              Sruti box
                              =========

Simple sruti box emulator with 12 buttons in C or G. Choice of midi
instruments. Use the instrument and key drop downs to change the midi
instrument and key, use the volume control to change the volume.

Files
=====

Changes			- Change log
COPYING			- Licence file
Makefile		- Build file
Melodeon.c		- Source file
Melodeon.exe		- Application
Melodeon.exe.manifest	- Manifest file
Melodeon.nsi		- NSIS installer script
Srutibox.c		- Source file
Srutibox.exe		- Application
Srutibox.exe.manifest	- Manifest file
Srutibox.nsi		- NSIS installer script
Readme.txt		- This file

Building
========

Use the makefile with Mingw or Cygwin GCC, or use a Microsoft C
compiler to build. There are no resources, as the Mingw tools don't
appear to suppost them. The manifest can be built into the application
using Microsoft tools. The manifest file enables the use of new style
Windows tools, but the application works fine without.
