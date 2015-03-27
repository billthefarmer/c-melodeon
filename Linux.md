# Introduction #

A linux version has been released using [Gtk+](http://www.gtk.org).

![http://melodeon.googlecode.com/files/Screenshot-Melodeon.png](http://melodeon.googlecode.com/files/Screenshot-Melodeon.png)

## Issues ##

There are several things to be done to get this emulator working on any given platform:

  * Learn the gui for the platform, in this case there is a choice of [Gtk+](http://www.gtk.org) or [QT](http://www.qtsoftware.com/products). I chose [Gtk+](http://www.gtk.org) because I use [Ubuntu](http://www.ubuntu.com), which has a [Gnome](http://www.gnome.org) desktop.

  * Find out how to capture user keystrokes. This isn't necessarily as simple as it seems, as we are using the function keys, and the control and alt keys. In windows the F10 key goes somewhere different from the others.

  * Find out how to stop unwanted keyboard side effects, like alt-spacebar, alt-F4, etc. I have done this in windows, but not in linux.

  * Find out how to send midi events to either a built in midi synthesizer (windows), or a software synthesizer.

## Progress ##

  * I have created the user interface, with interaction with the tool widgets and the keyboard. I have found it is not possible to stop the alt-spacebar, alt-F4, etc. side effects, because they are either defined by the desktop, or built in to Xorg. This is potential show stopper.

  * I have found out how to use ALSA to send midi messages to the sound card. That doesn't help much unless you have an AWE32/64 sound card, with a built in synthesizer, in which case you need sfxload/asfxload to load a sound bank. You can get one from the [ALSA unofficial wiki](http://alsa.opensrc.org/index.php/SoundfontHandling): [8MBGMSFX.SF2](http://www.alsa-project.org/~james/sound-fonts/8MBGMSFX.SF2).

  * If you have a sound card without a synthesizer, you need to use [Timidity](http://timidity.sourceforge.net) or [Fluidsynth](http://fluidsynth.resonance.org/trac). I have chosen Fluidsynth because it has a shared library, so users only need the library, not the whole thing.

  * In order to stop notes repeating, I have had to disable auto repeat on the keyboard. This is global for the session, so it needs to be restored afterwards. Gtk and xlib offer no means to test for repeated keys that I can find.

  * I have released Melodeon 1.05 for linux. The tarball is rather large because it includes the sound font file.