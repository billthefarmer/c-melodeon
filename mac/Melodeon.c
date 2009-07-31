////////////////////////////////////////////////////////////////////////////////
//
//  Melodeon - A Melodeon emulator written in C.
//
//  Copyright (C) 2009  Bill Farmer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//  Bill Farmer  william j farmer [at] tiscali [dot] co [dot] uk.
//
////////////////////////////////////////////////////////////////////////////////

#include <Carbon/Carbon.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>

// Macros

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))

// Offsets

#define BASS  0
#define CHORD 1

// Max volume

#define MAXVOL 127

// Bass buttons

#define BASSBUTTONS

// Buttons

#define BUTTONS 12

// Button size

#define SIZE 34

// Command IDs

enum {
    kCommandInst    = 'inst',
    kCommandReverse = 'rvrs',
    kCommandKey     = 'key ',
    kCommandVolume  = 'vol ',
    kCommandLayout  = 'lay '};

// HIView IDs

HIViewID kHIViewIDReverse =
    {kCommandReverse, 101};

HIViewID kHIViewIDKey =
    {kCommandKey, 102};

// Key IDs

enum {
    kSpaceKey = 0x31,
    kAKey     = 0x00,
    kBKey     = 0x0b,
    kCKey     = 0x08,
    kDKey     = 0x02,
    kEKey     = 0x0e,
    kFKey     = 0x03,
    kGKey     = 0x05,
    kRKey     = 0x0f};

// Function keys F1-F12

int keyCodes[] =
    {0x7a, 0x78, 0x63, 0x76,
     0x60, 0x61, 0x62, 0x64,
     0x65, 0x6d, 0x67, 0x6f};

// Keyboard modifiers

enum {
    kKeyboardCommandMask = 0x0100,
    kKeyboardShiftMask   = 0x0200,
    kKeyboardOptionMask  = 0x0800,
    kKeyboardControlMask = 0x1000};

// Midi codes

enum {
    kMidiMessageNoteOn        = 0x90,
    kMidiMessageNoteOff       = 0x80,
    kMidiMessageBassOn        = 0x91,
    kMidiMessageBassOff       = 0x81,
    kMidiMessageChordOn       = 0x92,
    kMidiMessageChordOff      = 0x82,
    kMidiMessageProgramChange = 0xC0};

// Status text

CFStringRef statusText =
    CFSTR("Press the function keys F1-F12 as melodeon buttons "
          "and the space bar as the bellows. 4th button start.");

// List of midi instruments

char *instruments[] =
    {"Acoustic Grand Piano", "Bright Acoustic Piano",
     "Electric Grand Piano", "Honky-tonk Piano",
     "Electric Piano 1", "Electric Piano 2", "Harpsichord",
     "Clavi", "Celesta", "Glockenspiel", "Music Box",
     "Vibraphone", "Marimba", "Xylophone", "Tubular Bells",
     "Dulcimer", "Drawbar Organ", "Percussive Organ",
     "Rock Organ", "Church Organ", "Reed Organ",
     "Accordion", "Harmonica", "Tango Accordion",
     "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)",
     "Electric Guitar (jazz)", "Electric Guitar (clean)",
     "Electric Guitar (muted)", "Overdriven Guitar",
     "Distortion Guitar", "Guitar harmonics",
     "Acoustic Bass", "Electric Bass (finger)",
     "Electric Bass (pick)", "Fretless Bass",
     "Slap Bass 1", "Slap Bass 2", "Synth Bass 1",
     "Synth Bass 2", "Violin", "Viola", "Cello",
     "Contrabass", "Tremolo Strings", "Pizzicato Strings",
     "Orchestral Harp", "Timpani", "String Ensemble 1",
     "String Ensemble 2", "SynthStrings 1", "SynthStrings 2",
     "Choir Aahs", "Voice Oohs", "Synth Voice",
     "Orchestra Hit", "Trumpet", "Trombone", "Tuba",
     "Muted Trumpet", "French Horn", "Brass Section",
     "SynthBrass 1", "SynthBrass 2", "Soprano Sax",
     "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe",
     "English Horn", "Bassoon", "Clarinet", "Piccolo",
     "Flute", "Recorder", "Pan Flute", "Blown Bottle",
     "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)",
     "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)",
     "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)",
     "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)",
     "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)",
     "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)",
     "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)",
     "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)",
     "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar", "Banjo",
     "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle",
     "Shanai", "Tinkle Bell", "Agogo", "Steel Drums",
     "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum",
     "Reverse Cymbal", "Guitar Fret Noise", "Breath Noise",
     "Seashore", "Bird Tweet", "Telephone Ring",
     "Helicopter", "Applause", "Gunshot"};

int instrument;

// List of keys and offset values

char *keys[] =
    {"Eb", "Bb", "F", "C", "G", "D", "A"};

int keyvals[] =
    {3, -2, 5, 0, -5, 2, -3};

int key;

// Layouts

char *layouts[] =
    {"Extended", "Hohner"};

int layout;

// Midi notes for 'C'

int notes[LENGTH(layouts)][BUTTONS][2] =
    {{{48, 53},
      {52, 57},
      {55, 59},
      {60, 62},
      {64, 65},
      {67, 69},
      {72, 71},
      {76, 74},
      {79, 77},
      {84, 81},
      {88, 83},
      {91, 86}},
     {{48, 53},
      {52, 55},
      {55, 59},
      {60, 62},
      {64, 65},
      {67, 69},
      {72, 71},
      {76, 74},
      {79, 77},
      {84, 81},
      {88, 83},
      {91, 86}}};

// Midi notes for bass
#ifdef BASSBUTTONS
int bass[LENGTH(keys)][2] =
    {{39, 46},
     {46, 41},
     {41, 36},
     {36, 43},
     {43, 38},
     {38, 45},
     {45, 40}};

int chord[LENGTH(keys)][2][2] =
    {{{63, 70}, {70, 65}},
     {{70, 65}, {65, 60}},
     {{65, 60}, {60, 67}},
     {{60, 67}, {67, 62}},
     {{67, 62}, {62, 69}},
     {{62, 69}, {69, 64}},
     {{69, 64}, {64, 71}}};
#endif
// Synthesizer unit

AudioUnit synthUnit;

// Buttons

Boolean buttons[BUTTONS];
ControlRef display[BUTTONS];
#ifdef BASSBUTTONS
Boolean control;
Boolean command;

ControlRef bassdisp[2];
#endif
// Bellows handle

ControlRef spacebar;
Boolean bellows;

// Reverse value

Boolean reverse = false;

// Volume value

int volume = MAXVOL;

// Function prototypes

OSStatus  KeyboardHandler(EventHandlerCallRef, EventRef, void*);
OSStatus  CommandHandler(EventHandlerCallRef, EventRef, void*);
OSStatus  ComboBoxHandler(EventHandlerCallRef, EventRef, void*);

// Function main

int main(int argc, char *argv[])
{
    WindowRef window;
    HIViewRef content;
    HIViewRef combo;
    ControlRef group;
    ControlRef check;
    ControlRef text;
    ControlRef slider;
    ControlRef quit;
    MenuRef menu;
    HIRect rect;
    int i;

    // Window bounds

    Rect bounds = {0, 0, 394, 590};

    // Create window

    CreateNewWindow(kDocumentWindowClass,
                    kWindowStandardFloatingAttributes |
                    kWindowStandardHandlerAttribute |
		    kWindowInWindowMenuAttribute |
                    kWindowCompositingAttribute,
                    &bounds, &window);

    // Set the title

    SetWindowTitleWithCFString(window, CFSTR("Melodeon"));

    // Create a window menu

    CreateStandardWindowMenu(0, &menu);

    // Insert the menu

    InsertMenu(menu, 0);

//     menu = GetMenuRef(kMenuStdMenuProc);
//     printf("menu = %x\n", menu);
//     InsertMenuItemTextWithCFString(menu, CFSTR("About Melodeon"),
//                                    0, 0, kHICommandAbout);

    // Show and position the window

    ShowWindow(window);
    RepositionWindow(window, NULL, kWindowCascadeOnMainScreen);

    // Find the window content

    HIViewFindByID(HIViewGetRoot(window),
                   kHIViewWindowContentID,
                   &content);

    // Set bounds for group box

    bounds.bottom = 92;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 20);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right = 74;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Instrument:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 16, 18);

    // Bounds of combo box

    rect.size.height = 20;
    rect.size.width = 160;    

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("Accordion"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetCommandID(combo, kCommandInst); 

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 100, 16);

    // Add the instruments

    for (i = 0; i < LENGTH(instruments); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      instruments[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set the current instrument

        if (strcmp(instruments[i], "Accordion") == 0)
            instrument = i;
    }

    // Bounds of check box

    bounds.bottom = 18;
    bounds.right = 124;

    // Create check box

    CreateCheckBoxControl(window, &bounds, CFSTR("Reverse Buttons"),
                          false, true, &check);

    // Set the control ID and the command ID

    HIViewSetID(check, kHIViewIDReverse);
    HIViewSetCommandID(check, kCommandReverse); 

    // Place in the group box

    HIViewAddSubview(group, check);
    HIViewPlaceInSuperviewAt(check, 290, 17);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 32;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Key:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 430, 18);

    // Bounds of combo box

    rect.size.width = 60;

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("C"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetID(combo, kHIViewIDKey);
    HIViewSetCommandID(combo, kCommandKey); 

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 470, 16);

    // Add keys

    for (i = 0; i < LENGTH(keys); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      keys[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set current key

        if (strcmp(keys[i], "C") == 0)
            key = i;
    }

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 54;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Volume:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 16, 56);
    
    // Bounds of slider

    bounds.bottom = 16;
    bounds.right  = 160;

    // Create slider

    CreateSliderControl(window, &bounds, MAXVOL, 0, MAXVOL,
                        kControlSliderDoesNotPoint, 0, false, NULL, &slider);

    // Set command ID

    HIViewSetCommandID(slider, kCommandVolume); 

    // Place in the group box

    HIViewAddSubview(group, slider);
    HIViewPlaceInSuperviewAt(slider, 100, 58);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 48;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Layout:"), NULL, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 290, 56);

    // Bounds of combo box

    rect.size.width = 84;

    // Create combo box

    HIComboBoxCreate(&rect, CFSTR("Hohner"), NULL, NULL,
                     kHIComboBoxStandardAttributes,
                     &combo);

    // Set visible and set command ID

    HIViewSetVisible(combo, true);
    HIViewSetCommandID(combo, kCommandLayout); 

    // Place in the group box

    HIViewAddSubview(group, combo);
    HIViewPlaceInSuperviewAt(combo, 352, 54);

    // Add layouts

    for (i = 0; i < LENGTH(layouts); i++)
    {
        HIComboBoxAppendTextItem(combo,
            CFStringCreateWithCString(kCFAllocatorDefault,
                                      layouts[i],
                                      kCFStringEncodingMacRoman), NULL);

        // Set current layout

        if (strcmp(layouts[i], "Hohner") == 0)
            layout = i;
    }

    // Bounds of push button

    bounds.bottom = 20;
    bounds.right  = 60;

    // Create push button

    CreatePushButtonControl(window, &bounds, CFSTR("Quit:"), &quit);

    // Set command ID

    HIViewSetCommandID(quit, kHICommandQuit); 

    // Place in the group box

    HIViewAddSubview(group, quit);
    HIViewPlaceInSuperviewAt(quit, 471, 54);

    // Group box bounds

    bounds.bottom = 92;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 132);

    // Font style

    ControlFontStyleRec style;
    style.flags = kControlUseFontMask|kControlUseJustMask;
    style.font = kControlFontBigSystemFont;
    style.just = teCenter;

    // Bounds of text
    
    bounds.bottom = 16;
    bounds.right  = 550;

    // Create static text

    CreateStaticTextControl(window, &bounds, CFSTR("Melodeon"),
                            &style, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 26);

    // Bounds of text

    bounds.bottom = 16;
    bounds.right  = 550;

    // Create static text

    CreateStaticTextControl(window, &bounds,
                            CFSTR("Play melodeon on your keyboard"),
                            &style, &text);

    // Place in the group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 50);

    // Group box bounds

    bounds.bottom = 110;
    bounds.right  = 550;

    // Create group box

    CreateGroupBoxControl(window, &bounds, NULL, true, &group);

    // Place in the window

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, 20, 244);

    // Space bar button bounds

    bounds.bottom = SIZE;
    bounds.right  = ((SIZE + 10) * 5) + SIZE;

    // Create space bar

    CreateBevelButtonControl(window, &bounds, NULL,
                             kControlBevelButtonNormalBevel,
                             kControlBehaviorPushbutton,
                             NULL, 0, 0, 0, &spacebar);

    // Place in the group box

    HIViewAddSubview(group, spacebar);
    HIViewPlaceInSuperviewAt(spacebar, 148, 16);

    // button bounds

    bounds.bottom = SIZE;
    bounds.right  = SIZE;
#ifdef BASSBUTTONS
    for (i = 0; i < LENGTH(bassdisp); i++)
    {
        // Create bass button

        CreateBevelButtonControl(window, &bounds, NULL,
                                 kControlBevelButtonNormalBevel,
                                 kControlBehaviorPushbutton,
                                 NULL, 0, 0, 0, &bassdisp[i]);

        // Place in the group box

        HIViewAddSubview(group, bassdisp[i]);
        HIViewPlaceInSuperviewAt(bassdisp[i], 16 + (i * (SIZE + 10) * 2), 16);
    }
#endif
    for (i = 0; i < LENGTH(display); i++)
    {
        // Create button

        CreateBevelButtonControl(window, &bounds, NULL,
                                 kControlBevelButtonNormalBevel,
                                 kControlBehaviorPushbutton,
                                 NULL, 0, 0, 0, &display[i]);

        // Place in the group box

        HIViewAddSubview(group, display[i]);
        HIViewPlaceInSuperviewAt(display[i], 16 + (i * 44), 60);
    }

    // Group box bounds, wider than the window to hide rounded corners

    bounds.bottom = 20;
    bounds.right = 598;

    // Create group box for fake status bar

    CreateGroupBoxControl(window, &bounds, NULL, false, &group);

    // Place in window at negative offset to hide rounded corners

    HIViewAddSubview(content, group);
    HIViewPlaceInSuperviewAt(group, -4, 374);

    // Text bounds

    bounds.bottom = 16;
    bounds.right  = 590;

    // Font style

    style.flags = kControlUseFontMask|kControlUseJustMask;
    style.font = kControlFontSmallSystemFont;
    style.just = teCenter;

    // Create static text

    CreateStaticTextControl(window, &bounds, statusText, &style, &text);

    // Place in group box

    HIViewAddSubview(group, text);
    HIViewPlaceInSuperviewAt(text, 0, 2);

    // Combo box events type spec

    EventTypeSpec comboBoxEvents[] =
        {{kEventClassHIComboBox, kEventComboBoxListItemSelected}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(ComboBoxHandler),
                                   LENGTH(comboBoxEvents), comboBoxEvents,
                                   NULL, NULL);

    // Command events type spec

    EventTypeSpec commandEvents[] =
        {{kEventClassCommand, kEventCommandProcess}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(CommandHandler),
                                   LENGTH(commandEvents), commandEvents,
                                   NULL, NULL);

    // Keyboard events type spec

    EventTypeSpec keyboardEvents[] =
        {{kEventClassKeyboard, kEventRawKeyDown},
         {kEventClassKeyboard, kEventRawKeyUp},
         {kEventClassKeyboard, kEventRawKeyModifiersChanged}};

    // Install event handler

    InstallApplicationEventHandler(NewEventHandlerUPP(KeyboardHandler),
                                   LENGTH(keyboardEvents), keyboardEvents,
                                   NULL, NULL);

    // AU graph

    AUGraph graph;

    // Synthesizer and output node

    AUNode synthNode;
    AUNode outNode;

    // Component description

    ComponentDescription cd;
    cd.componentManufacturer = kAudioUnitManufacturer_Apple;
    cd.componentFlags = 0;
    cd.componentFlagsMask = 0;

    // New AU graph

    NewAUGraph(&graph);

    // Synthesizer

    cd.componentType = kAudioUnitType_MusicDevice;
    cd.componentSubType = kAudioUnitSubType_DLSSynth;

    // New node

    AUGraphNewNode(graph, &cd, 0, NULL, &synthNode);

    // Output

    cd.componentType = kAudioUnitType_Output;
    cd.componentSubType = kAudioUnitSubType_DefaultOutput;
 
    // New node

    AUGraphNewNode(graph, &cd, 0, NULL, &outNode);

    // Open graph

    AUGraphOpen(graph);

    // Connect nodes

    AUGraphConnectNodeInput(graph, synthNode, 0, outNode, 0);

    // Get a synth unit

    AUGraphGetNodeInfo(graph, synthNode, NULL, 0, NULL, &synthUnit);

    // Initialise

    AUGraphInitialize(graph);

    // Start

    AUGraphStart(graph);

    // Show the graph

//     CAShow(graph);

    // Set instrument

    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 0,
                         instrument, 0, 0);
    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 1,
                         instrument, 0, 0);
    MusicDeviceMIDIEvent(synthUnit, kMidiMessageProgramChange + 2,
                         instrument, 0, 0);

    // Run the application event loop

    RunApplicationEventLoop();

    // Stop the graph

    AUGraphStop(graph);

    // Dispose of the graph

    DisposeAUGraph(graph);

    // Exit

    return 0;
}

// Control handler

OSStatus CommandHandler(EventHandlerCallRef next,
                        EventRef event, void *data)
{
    HICommandExtended command;
    UInt32 value;

    // Get the command

    GetEventParameter(event, kEventParamDirectObject,
                      typeHICommand, NULL, sizeof(command),
                      NULL, &command);

    // Get the value

    value = HIViewGetValue(command.source.control);

    // Switch on the command ID

    switch (command.commandID)
    {
        // Reverse

    case kCommandReverse:
        reverse = value;
        break;

        // Volume

    case kCommandVolume:
        volume = value;
        break;

        // Quit

    case kHICommandQuit:

        // Let the default handler handle it

    default:
        return eventNotHandledErr;
    }

    // Report success

    return noErr;
}

// Combo box handler

OSStatus ComboBoxHandler(EventHandlerCallRef next,
                         EventRef event, void *data)
{
    ControlRef combo;
    CFIndex index;
    UInt32 id;

    // Get the control

    GetEventParameter(event, kEventParamDirectObject,
                      typeControlRef, NULL, sizeof(combo),
                      NULL, &combo);

    // Get the index

    GetEventParameter(event, kEventParamComboBoxListSelectedItemIndex,
                      typeCFIndex, NULL, sizeof(index),
                      NULL, &index);

    // Get the command id

    HIViewGetCommandID(combo, &id);

    // Switch on the command id
        
    switch (id)
    {
        // Instrument

    case kCommandInst:
        instrument = index;
        break;

        // Key

    case kCommandKey:
        key = index;
        break;

        // Layout

    case kCommandLayout:
        layout = index;
        break;

        // Something else

    default:
        return eventNotHandledErr;
    }

    // Report success

    return noErr;
}

// Keyboard handler

OSStatus  KeyboardHandler(EventHandlerCallRef next,
                          EventRef event, void *data)
{
    UInt32 kind;
    UInt32 keycode;
    UInt32 modifiers;

    // Get the event kind

    kind = GetEventKind(event);

    // Switch on event kind

    switch (kind)
    {
        // Modifiers changed

    case kEventRawKeyModifiersChanged:

	// Get the modifiers

	GetEventParameter(event, kEventParamKeyModifiers, typeUInt32,
			  NULL, sizeof(modifiers), NULL, &modifiers);

        if (modifiers & kKeyboardControlMask)
        {
	    control = true;
            HIViewSetValue(bassdisp[0], true);

	    int note = bass[key][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageBassOn,
				 note, volume, 0);
        }

        else
        {
	    control = false;
	    HIViewSetValue(bassdisp[0], false);

 	    int note = bass[key][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageBassOff,
				 note, 0, 0);
        }

        if (modifiers & kKeyboardCommandMask)
	{
	    command = true;
            HIViewSetValue(bassdisp[1], true);

	    int note = chord[key][0][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageChordOn,
				 note, volume, 0);
	    note = chord[key][1][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageChordOn,
				 note, volume, 0);
	}

        else
	{
	    command = false;
            HIViewSetValue(bassdisp[1], false);

	    int note = chord[key][0][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageChordOff,
				 note, 0, 0);
	    note = chord[key][1][bellows];
	    MusicDeviceMIDIEvent(synthUnit, kMidiMessageChordOff,
				 note, 0, 0);
	}

        return noErr;

	// Key down or key up

    case kEventRawKeyDown:
    case kEventRawKeyUp:

        // Get the key code

        GetEventParameter(event, kEventParamKeyCode, typeUInt32,
                          NULL, sizeof(keycode), NULL, &keycode);
        break;

    default:
        return eventNotHandledErr;
    }

    Boolean found = false;
    int i, n, m;
    char *s;

    for (i = 0; i < LENGTH(keyCodes); i++)
    {
        if (keyCodes[i] == keycode)
        {
            found = true;
            n = reverse? LENGTH(display) - i - 1: i;
            m = reverse? i: LENGTH(display) - i - 1;
            break;
        }
    }

    if (found)
    {
        switch (kind)
        {
        case kEventRawKeyDown:
            if (!buttons[n])
            {
                buttons[n] = true;
                HIViewSetValue(display[n], true);

		int note = notes[layout][m][bellows] + keyvals[key];
		MusicDeviceMIDIEvent(synthUnit, kMidiMessageNoteOn,
				     note, volume, 0);
            }
            break;

        case kEventRawKeyUp:
            if (buttons[n])
            {
                buttons[n] = false;
                HIViewSetValue(display[n], false);

		int note = notes[layout][m][bellows] + keyvals[key];
		MusicDeviceMIDIEvent(synthUnit, kMidiMessageNoteOff,
				     note, 0, 0);
            }
            break;

        default:
            return eventNotHandledErr;
        }

        return noErr;
    }

    // Window and control

    WindowRef window;
    HIViewRef control;

    // Get the window
    
    window = ActiveNonFloatingWindow();

    // Find the key combo box

    HIViewFindByID(HIViewGetRoot(window),
                   kHIViewIDKey,
                   &control);

    switch (kind)
    {
    case kEventRawKeyDown:
        switch (keycode)
        {
        case kSpaceKey:
            if (!bellows)
            {
                bellows = true;
                HIViewSetValue(spacebar, true);
            }
            break;

        default:
            return eventNotHandledErr;
        }
        break;

    case kEventRawKeyUp:
        switch (keycode)
        {
        case kSpaceKey:
            if (bellows)
            {
                bellows = false;
                HIViewSetValue(spacebar, false);
            }
            break;

        case kEKey:
            key = 0;
            HIViewSetText(control, CFSTR("Eb"));
            break;

        case kBKey:
            key = 1;
            HIViewSetText(control, CFSTR("Bb"));
            break;

        case kFKey:
            key = 2;
            HIViewSetText(control, CFSTR("F"));
            break;

        case kCKey:
            key = 3;
            HIViewSetText(control, CFSTR("C"));
            break;

        case kGKey:
            key = 4;
            HIViewSetText(control, CFSTR("G"));
            break;

        case kDKey:
            key = 5;
            HIViewSetText(control, CFSTR("D"));
            break;

        case kAKey:
            key = 6;
            HIViewSetText(control, CFSTR("A"));
            break;

        case kRKey:

            // Find the reverse toggle

            HIViewFindByID(HIViewGetRoot(window),
                           kHIViewIDReverse,
                           &control);

            // Change the value

            reverse = !reverse;
            HIViewSetValue(control, reverse);
            break;

        default:
            return eventNotHandledErr;
        }
        break;

    default:
        return eventNotHandledErr;
    }

    // Report success

    return noErr;
}
