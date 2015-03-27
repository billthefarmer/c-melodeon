# Introduction #

An android melodeon emulator. Emulates a one row melodeon and a one and a half row organetto. The organetto half row buttons are reversals. There is a choice of midi intruments and keys. It may also be found on [Github](https://github.com/billthefarmer/melodeon). The app is now on [F-Droid](https://f-droid.org/repository/browse/?fdid=org.billthefarmer.melodeon).

![http://melodeon.googlecode.com/svn/wiki/images/Melodeon.png](http://melodeon.googlecode.com/svn/wiki/images/Melodeon.png)
![http://melodeon.googlecode.com/svn/wiki/images/Melodeon-organetto.png](http://melodeon.googlecode.com/svn/wiki/images/Melodeon-organetto.png)
![http://melodeon.googlecode.com/svn/wiki/images/Melodeon-settings.png](http://melodeon.googlecode.com/svn/wiki/images/Melodeon-settings.png)

  * Keys: Eb, Bb, F, C, G, D, A
  * Instruments: Standard midi set, defaults to accordion
  * Layouts: Melodeon and organetto.
  * Choice of fascia images

## Playing ##

Hold the tablet facing away from you the right way up. Play the buttons, touching the vertical grey bar as the bellows. If you don't know how to play button accordion, see http://info.melodeon.net. The buttons may be reversed using the slide button at the bottom.

## Technical Note ##

The standard documented android system does not support sending midi messages to the built in midi synthesizer. This app uses an undocumented interface to the synthesizer, which may possibly change with future android releases. This hasn't happened so far, but if it does the app may no longer work correctly. Hopefully, if this happens it will get fixed.

The button images are one of the buttons on my 70+ year old Hohner Lilliput.