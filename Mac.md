# Introduction #

A Mac version has been released.

![http://melodeon.googlecode.com/files/Snapshot-Melodeon.png](http://melodeon.googlecode.com/files/Snapshot-Melodeon.png)

## Playing ##
Hold the keyboard facing away from you with the Esc button at the bottom and the Pause button under your chin. Play the function keys, waggling the space bar as it were the bellows. If you are really good you can play bass simultaneously on the Control and Alt keys. The key may be changed by pressing the letter key for the desired playing key. The buttons may be reversed by pressing the 'R' key. If you don't know how to play button accordion / melodeon, see http://info.melodeon.net. Also see http://www.youtube.com/watch?v=JXFDcEOhIX8 for a truly amazing performance by an anonymous user.

## Issues ##

There are several things to be done to get this emulator working on any given platform:

  * Learn the gui for the platform, in this case there is a possibility of using Carbon.

  * Find out how to capture user keystrokes. This isn't necessarily as simple as it seems, as we are using the function keys, and the control and command keys. In windows the F10 key goes somewhere different from the others. The keys may be different on a Mac for all I know.

  * Find out how to stop unwanted keyboard side effects, like alt-spacebar. I have done this in windows.

  * Find out how to send midi events to either a built in midi synthesizer (windows), or a synthesizer on the sound card. I assume the sound hardware is built in on a Mac.

## Progress ##

  * I don't have a Mac. I have made a quick investigation into cross development on windows or linux, but it doesn't look hopeful.
  * I have discovered that it is possible to run versions of Mac OSX on a PC, so I am currently investigation development on a Hackintosh using Carbon.
  * I now have the mac version mainly working, debugging to go. I love the concert hall reverberation you get with the built in midi synthesizer without asking.
  * Mac version released. You need to disable several keyboard short cuts for the function keys to work.

## Donate ##
[![](https://www.paypal.com/en_GB/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=TZNQZU88LDRAY&lc=GB&item_name=Accordion&item_number=mac&currency_code=GBP&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted)