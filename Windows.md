# Introduction #

Comments on the Windows version.

Version 1.06 fixes a bug in the combo drop-down boxes that were not working correctly in Vista and Windowd 7.

### Photo Editor had been superseded by a very nice free editor [paint.net](http://www.getpaint.net). ###

![http://melodeon.googlecode.com/files/Photo_editor.png](http://melodeon.googlecode.com/files/Photo_editor.png)

## Comments ##

  * There is a very nice tool bundled with earlier versions of Microsoft Office, which enables you to take screen shots of new applications, zoom in on them, and use the selection tool to measure how your widgets line up. All the dimensions are in the status line in units of your choice. I haven't found a similar tool for linux. But then Gtk lines them up for you, but you might want to do a bit of adjustment.

  * It is possible to install a copy of the Photo Editor without all the Office bloatware that you get if you follow the Microsoft instructions. This is what you do:
    * Install Office from the Office XP CD, as per the instructions, making sure you select the Photo Editor and all the graphics filters.
    * Make a copy of the C:\Program Files\Common Files\Microsoft Shared\Grphflt and C:\Program Files\Common Files\Microsoft Shared\PhotoEd folders and stash them somewhere temporarily.
    * Start up the registry editor (Start->Run->regedt32), and export a copy of the 'HKEY\_LOCAL\_MACHINE\SOFTWARE\Microsoft\Shared Tools\Graphics Filters' key to a file.
    * Uninstall Office completely.
    * Put the Grphflt and PhotoEd folders back where they were.
    * Restore the Graphics Filters key to the registry. Just double clicking on the file will do the trick.
> You should now have a working Photo Editor without the file type associations. That was too difficult, as they are dotted all over the registry.

  * I have changed the volume control for a slider, which it should have been to start with. My excuse is that the slider is called a trackbar. I have also changed the background colour.