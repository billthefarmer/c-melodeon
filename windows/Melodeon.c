////////////////////////////////////////////////////////////////////////////////
//
//  Melodeon - A simple Melodeon emulator written in C.
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
///////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <commctrl.h>

// Macros

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))

// Tool ids

#define INST 101
#define REVS 102
#define KEYS 103
#define VOLM 104
#define LAYT 105
#define QUIT 106
#define TXTS 107
#define BTNS 108
#define STAT 109

// Midi codes

#define NOTEOFF 0x80
#define BASSOFF 0x81
#define CHRDOFF 0x82
#define NOTEON  0x90
#define BASSON  0x91
#define CHRDON  0x92
#define CHANGE  0xc0

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

// Global handles

HINSTANCE hinst;
HMIDIOUT hmdo;

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

// List of keys and offset values

char *keys[] =
    {"Eb", "Bb", "F", "C", "G", "D", "A"};

int keyvals[] =
    {3, -2, 5, 0, -5, 2, -3};

UINT key;

HWND hkey;

// Layouts

char *layouts[] =
    {"Extended", "Hohner"};

UINT layout;

// Midi notes for 'C'

BYTE notes[LENGTH(layouts)][BUTTONS][2] =
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
BYTE bass[LENGTH(keys)][2] =
    {{39, 46},
     {46, 41},
     {41, 36},
     {36, 43},
     {43, 38},
     {38, 45},
     {45, 40}};

byte chord[LENGTH(keys)][2][2] =
    {{{63, 70}, {70, 65}},
     {{70, 65}, {65, 60}},
     {{65, 60}, {60, 67}},
     {{60, 67}, {67, 62}},
     {{67, 62}, {62, 69}},
     {{62, 69}, {69, 64}},
     {{69, 64}, {64, 71}}};
#endif
// Buttons

BOOL buttons[BUTTONS];
#ifdef BASSBUTTONS
BOOL control;
BOOL menu;
#endif
// Bellows handle

BOOL bellows;

// Reverse value

BOOL reverse;

HWND hrev;

// Volume value

UINT volume = MAXVOL;

// Display handles

HWND display[BUTTONS];
#ifdef BASSBUTTONS
HWND bassdisp[2];
#endif
HWND spacebar;

// Function prototypes.

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
UINT ChangeInstrument(HWND);
UINT ReverseButtons(HWND);
UINT ChangeLayout(HWND);
UINT ChangeKey(HWND);
UINT ChangeVolume(WPARAM, LPARAM);
UINT CharPressed(WPARAM, LPARAM);
UINT KeyDown(WPARAM, LPARAM);
UINT KeyUp(WPARAM, LPARAM);
UINT ShortMessage(BYTE, BYTE, BYTE);

// Application entry point.

int WINAPI WinMain(HINSTANCE hInstance,
		   HINSTANCE hPrevInstance,
		   LPSTR lpszCmdLine,
		   int nCmdShow)
{
    // Initialize common controls to get the new style controls, also
    // dependent on manifest file

    InitCommonControls();

    // Check for a previous instance of this app

    if (!hPrevInstance)
    {
	WNDCLASS wc;

	// Fill in the window class structure with parameters
	// that describe the main window.

	wc.style =
	    CS_HREDRAW | CS_VREDRAW;      // redraw if size changes
	wc.lpfnWndProc = MainWndProc;     // points to window procedure
	wc.cbClsExtra = 0;                // no extra class memory
	wc.cbWndExtra = 0;                // no extra window memory
	wc.hInstance = hInstance;         // handle to instance
	wc.hIcon =
	    LoadIcon(NULL,
		     IDI_WINLOGO);        // predefined app. icon
	wc.hCursor =
	    LoadCursor(NULL,
		       IDC_ARROW);        // predefined arrow
	wc.hbrBackground =
	    GetSysColorBrush(COLOR_3DFACE);
// 	    GetStockObject(WHITE_BRUSH);  // white background brush
	wc.lpszMenuName =  "MainMenu";    // name of menu resource
	wc.lpszClassName = "MainWClass";  // name of window class

	// Register the window class.

	if (!RegisterClass(&wc))
	    return FALSE;
    }

    // Save the application-instance handle.

    hinst = hInstance;

    // Create the main window.

    HWND hwnd =
	CreateWindow("MainWClass",  // name of window class
		     "Melodeon",    // title-bar string
		     WS_OVERLAPPED |
		     WS_MINIMIZEBOX |
		     WS_SYSMENU,    // top level window
		     CW_USEDEFAULT, // default horizontal position
		     CW_USEDEFAULT, // default vertical position
		     CW_USEDEFAULT, // default width
		     CW_USEDEFAULT, // default height
		     NULL,          // no owner window
		     NULL,          // use class menu
		     hinst,         // handle to application instance
		     NULL);         // no window-creation data

    // If the main window cannot be created, terminate
    // the application.

    if (!hwnd)
	return FALSE;

    // Show the window and send a WM_PAINT message to the window
    // procedure.

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Process messages

    MSG msg;
    BOOL flag;

    while ((flag = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0)
    {
	if (flag == -1)
	    break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

// Main window procedure

LRESULT CALLBACK MainWndProc(HWND hWnd,
			     UINT uMsg,
			     WPARAM wParam,
			     LPARAM lParam)
{
    // Tool handles

    static HWND hgrp;
    static HWND inst;
    static HWND hlay;
    static HWND hvol;
    static HWND quit;
    static HWND text;
    static HWND stat;

    // Selection values

    static int ival;
    static int kval;
    static int lval;

    // Window dimensions

    RECT rwnd;
    RECT rclt;

    char *s;

    switch (uMsg)
    {
    case WM_CREATE:

	// Get the window and client dimensions

	GetWindowRect(hWnd, &rwnd);
	GetClientRect(hWnd, &rclt);

	// Calculate desired window width and height

	int border = (rwnd.right - rwnd.left) - rclt.right;
	int width  = 560 + border;
	int height = width * 3 / 4;

	// Set new dimensions

	SetWindowPos(hWnd,          // Window
		     NULL, 0, 0,    // No z, x, y
		     width,         // width
		     height,        // height
		     SWP_NOMOVE |   // Don't move
		     SWP_NOZORDER); // Don't change the Z order

	// Create status bar

	stat =
	    CreateWindow(STATUSCLASSNAME, // Predefined class.
			 "\tPress the function keys F1-F12 as"
			 " melodeon buttons and the space bar"
			 " as the bellows. 4th button"
			 " start.",   // Text.
			 WS_VISIBLE | WS_CHILD, // Styles.
			 0, 0, 0, 0,  // no size or position.
			 hWnd,        // Parent window.
			 (HMENU)STAT, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create group box

	hgrp =
	    CreateWindow(WC_BUTTON,   // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 BS_GROUPBOX, // Styles.
			 10,          // x position.
			 2,           // y position.
			 540,         // width.
			 86,          // height.
			 hWnd,        // Parent window.
			 NULL,        // No id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create text

	text =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Instrument:",// Text.
			 WS_VISIBLE | WS_CHILD |
			 SS_LEFT,     // Styles.
			 20,          // x position.
			 24,          // y position.
			 76,          // width.
			 20,          // height.
			 hWnd,        // Parent window.
			 (HMENU)TXTS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create instruments pulldown

	inst =
	    CreateWindow(WC_COMBOBOX, // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD | WS_VSCROLL |
			 CBS_DROPDOWNLIST, // Styles.
			 102,         // x position.
			 20,          // y position.
			 168,         // width.
			 24,          // height.
			 hWnd,        // Parent window.
			 (HMENU)INST, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Add the instrument names

	int i;

	for (i = 0; i != LENGTH(instruments); i++)
	    SendMessage(inst, CB_ADDSTRING, 0, (LPARAM)instruments[i]);

	// Select the Accordion

	SendMessage(inst, CB_SELECTSTRING, -1, (LPARAM)"Accordion");

	// Create reverse tickbox

	hrev =
	    CreateWindow(WC_BUTTON,  // Predefined class.
			 "Reverse buttons:", // Text.
			 WS_VISIBLE | WS_CHILD | BS_LEFTTEXT |
			 BS_CHECKBOX, // Styles.
			 282,         // x position.
			 20,          // y position.
			 130,         // width.
			 24,          // height.
			 hWnd,        // Parent window.
			 (HMENU)REVS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create text

	text =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Key:",      // Text.
			 WS_VISIBLE | WS_CHILD |
			 SS_LEFT,     // Styles.
			 429,         // x position.
			 24,          // y position.
			 76,          // width.
			 20,          // height.
			 hWnd,        // Parent window.
			 (HMENU)TXTS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create keys pulldown

	hkey =
	    CreateWindow(WC_COMBOBOX, // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 CBS_DROPDOWNLIST, // Styles.
			 467,         // x position.
			 20,          // y position.
			 72,          // width.
			 24,          // height.
			 hWnd,        // Parent window.
			 (HMENU)KEYS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Add the keys

	for (i = 0; i != LENGTH(keys); i++)
	    SendMessage(hkey, CB_ADDSTRING, 0, (LPARAM)keys[i]);

	// Select C

	SendMessage(hkey, CB_SELECTSTRING, -1, (LPARAM)"C");

	// Create text

	text =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Volume:",   // Text.
			 WS_VISIBLE | WS_CHILD |
			 SS_LEFT,     // Styles.
			 20,          // x position.
			 58,          // y position.
			 54,          // width.
			 20,          // height.
			 hWnd,        // Parent window.
			 (HMENU)TXTS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create volume control

	hvol =
	    CreateWindow(TRACKBAR_CLASS, // track bar control class
			 NULL,        // no text
			 WS_VISIBLE | WS_CHILD |
			 TBS_HORZ,    // track bar styles
			 102,         // horizontal position
			 54,          // vertical position
			 168,         // width of the track bar
			 24,          // height of the track bar
			 hWnd,        // handle to main window
			 (HMENU)VOLM, // id
			 hinst,       // instance owning this window
			 NULL);       // pointer not needed

	SendMessage(hvol, TBM_SETRANGE, TRUE, MAKELONG(0, MAXVOL));
	SendMessage(hvol, TBM_SETPOS, TRUE, volume);

	// Create text

	text =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Layout:",   // Text.
			 WS_VISIBLE | WS_CHILD |
			 SS_LEFT,     // Styles.
			 282,         // x position.
			 58,          // y position.
			 54,          // width.
			 20,          // height.
			 hWnd,        // Parent window.
			 (HMENU)TXTS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create layout pulldown

	hlay =
	    CreateWindow(WC_COMBOBOX, // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 CBS_DROPDOWNLIST, // Styles.
			 347,         // x position.
			 54,          // y position.
			 110,         // width.
			 24,          // height.
			 hWnd,        // Parent window.
			 (HMENU)LAYT, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Add the layouts

	for (i = 0; i != LENGTH(layouts); i++)
	    SendMessage(hlay, CB_ADDSTRING, 0, (LPARAM)layouts[i]);

	// Select Hohner

	SendMessage(hlay, CB_SELECTSTRING, -1, (LPARAM)"Hohner");

	// Create quit button

	quit =
	    CreateWindow(WC_BUTTON,  // Predefined class.
			 "Quit",     // Text.
			 WS_VISIBLE | WS_CHILD |
			 BS_PUSHBUTTON, // Styles.
			 466,         // x position.
			 53,          // y position.
			 74,          // width.
			 26,          // height.
			 hWnd,        // Parent window.
			 (HMENU)QUIT, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create group box

	hgrp =
	    CreateWindow(WC_BUTTON,   // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 BS_GROUPBOX, // Styles.
			 10,          // x position.
			 90,          // y position.
			 540,         // width.
			 162,         // height.
			 hWnd,        // Parent window.
			 NULL,        // No id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create text

	text =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Melodeon\n\n" // Text.
			 "Play melodeon on your keyboard",
			 WS_VISIBLE | WS_CHILD |
			 SS_CENTER,   // Styles.
			 20,          // x position.
			 149,         // y position.
			 520,         // width.
			 52,          // height.
			 hWnd,        // Parent window.
			 (HMENU)TXTS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Get status bar dimensions

	GetWindowRect(stat, &rwnd);

	// Get window client dimensions

	GetClientRect(hWnd, &rclt);

	// Calculate top of status bar

	int bottom = rclt.bottom - (rwnd.bottom - rwnd.top);

	// Create group box

	hgrp =
	    CreateWindow(WC_BUTTON,   // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 BS_GROUPBOX, // Styles.
			 10,          // x position.
			 bottom - 118, // y position.
			 540,         // width.
			 108,         // height.
			 hWnd,        // Parent window.
			 NULL,        // No id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create spacebar

	spacebar =
	    CreateWindow(WC_BUTTON,   // Predefined class.
			 NULL,        // No text.
			 WS_VISIBLE | WS_CHILD |
			 BS_PUSHBUTTON, // Styles.
			 153,         // x position.
			 bottom - 98, // y position.
			 254,         // width.
			 SIZE,        // height.
			 hWnd,        // Parent window.
			 (HMENU)BTNS, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create bass buttons
#ifdef BASSBUTTONS
	for (i = 0; i != LENGTH(bassdisp); i++)
	{
	    bassdisp[i] =
		CreateWindow(WC_BUTTON,  // Predefined class.
			     NULL,       // No text.
			     WS_VISIBLE | WS_CHILD |
			     BS_PUSHBUTTON, // Styles.
			     21 + 88 * i, // x position.
			     bottom - 98, // y position.
			     SIZE,        // width.
			     SIZE,        // height.
			     hWnd,        // Parent window.
			     (HMENU)BTNS, // Id.
			     hinst,       // handle to application instance
			     NULL);       // Pointer not needed.
	}
#endif
	// Create row of display buttons

	for (i = 0; i != LENGTH(display); i++)
	{
	    display[i] =
		CreateWindow(WC_BUTTON,  // Predefined class.
			     NULL,       // No text.
			     WS_VISIBLE | WS_CHILD |
			     BS_PUSHBUTTON, // Styles.
			     21 + 44 * i, // x position.
			     bottom - 54, // y position.
			     SIZE,        // width.
			     SIZE,        // height.
			     hWnd,        // Parent window.
			     (HMENU)BTNS, // Id.
			     hinst,       // handle to application instance
			     NULL);       // Pointer not needed.
	}

	// Open a midi out device

	midiOutOpen(&hmdo, -1, 0, 0, CALLBACK_NULL);

	// Change the instrument

	ChangeInstrument(inst);

	// Change the key

	ChangeKey(hkey);

	// Change layout

	ChangeLayout(hlay);
	break;

	// Colour static text, defeat DefWindowProc() by capturing
	// this message. Changed background colour instead.

//     case WM_CTLCOLORSTATIC:
// 	break;

	// Disable menus by capturing this message

    case WM_INITMENU:
	break;

	// Capture system character key to stop pop up menus and other
	// nonsense

    case WM_SYSCHAR:
	break;

	// Character key pressed

    case WM_CHAR:
	CharPressed(wParam, lParam);
	break;

	// F10 key generates a WM_SYSKEYDOWN message

    case WM_SYSKEYDOWN:

	// All the other function keys and the space bar

    case WM_KEYDOWN:
	KeyDown(wParam, lParam);
	break;

	// F10 key generates a WM_SYSKEYUP message

    case WM_SYSKEYUP:

	// All the other function keys and the space bar

    case WM_KEYUP:
	KeyUp(wParam, lParam);
	break;

	// Volume control

    case WM_HSCROLL:
	ChangeVolume(wParam, lParam);

	// Set the focus back to the window

	SetFocus(hWnd);
	break;

	// Set the focus back to the window by clicking

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
	SetFocus(hWnd);
	break;

	// Focus lost

    case WM_KILLFOCUS:

	// Stop all notes

	midiOutReset(hmdo);

	// Reset all the buttons

	for (i = 0; i != LENGTH(buttons); i++)
	{
	    if (buttons[i])
		buttons[i] = FALSE;

	    if (SendMessage(display[i], BM_GETSTATE, 0, 0))
		SendMessage(display[i], BM_SETSTATE, FALSE, 0);
	}
#ifdef BASSBUTTONS
	if (control)
	    control = FALSE;

	if (menu)
	    menu = FALSE;

	for (i = 0; i != LENGTH(bassdisp); i++)
	{
	    if (SendMessage(bassdisp[i], BM_GETSTATE, 0, 0))
		SendMessage(bassdisp[i], BM_SETSTATE, FALSE, 0);
	}
#endif
	if (bellows)
	    bellows = FALSE;

	if (SendMessage(spacebar, BM_GETSTATE, 0, 0))
	    SendMessage(spacebar, BM_SETSTATE, FALSE, 0);
	break;

	// WM_COMMAND from one of the controls

    case WM_COMMAND:
	switch (LOWORD(wParam))
	{
	    // Instrument control

	case INST:
	    switch (HIWORD(wParam))
	    {
	    case CBN_DROPDOWN:
		ival = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
		break;

	    case CBN_SELENDOK:
		ChangeInstrument((HWND)lParam);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;

	    case CBN_SELENDCANCEL:
		SendMessage((HWND)lParam, CB_SETCURSEL, ival, 0);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;
	    }
	    break;

	    // Reverse control

	case REVS:
	    if (HIWORD(wParam) == BN_CLICKED)
		ReverseButtons((HWND)lParam);

	    // Set the focus back to the window

	    SetFocus(hWnd);
	    break;

	    // Key control

	case KEYS:
	    switch (HIWORD(wParam))
	    {
	    case CBN_DROPDOWN:
		kval = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
		break;

	    case CBN_SELENDOK:
		ChangeKey((HWND)lParam);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;

	    case CBN_SELENDCANCEL:
		SendMessage((HWND)lParam, CB_SETCURSEL, kval, 0);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;
	    }
	    break;

	    // Layout control

	case LAYT:
	    switch (HIWORD(wParam))
	    {
	    case CBN_DROPDOWN:
		lval = SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
		break;

	    case CBN_SELENDOK:
		ChangeLayout((HWND)lParam);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;

	    case CBN_SELENDCANCEL:
		SendMessage((HWND)lParam, CB_SETCURSEL, lval, 0);

		// Set the focus back to the window

		SetFocus(hWnd);
		break;
	    }
	    break;

	    // Keyboard buttons, set the focus back to the window

	case BTNS:
	    SetFocus(hWnd);
	    break;

	    // Quit button

	case QUIT:
	    if (HIWORD(wParam) == BN_CLICKED)
	    {
		if (MessageBox(hWnd,
			       "Really?",
			       "Quit",
			       MB_OKCANCEL |
			       MB_ICONQUESTION |
			       MB_DEFBUTTON1) == IDOK)
		{
		    midiOutReset(hmdo);
		    midiOutClose(hmdo);
		    PostQuitMessage(0);
		}
	    }
	    break;
	}
	break;

        // Process other messages.

    case WM_DESTROY:

	// Stop all notes

	midiOutReset(hmdo);
	midiOutClose(hmdo);
	PostQuitMessage(0);
	break;

	// Everything else

    default:
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

// Change instrument

UINT ChangeInstrument(HWND hinst)
{
    int inst = SendMessage(hinst, CB_GETCURSEL, 0, 0);
    ShortMessage(CHANGE, inst, 0);
#ifdef BASSBUTTONS
    ShortMessage(CHANGE + 1, inst, 0);
    ShortMessage(CHANGE + 2, inst, 0);
#endif
}

// Reverse buttons

UINT ReverseButtons(HWND hrev)
{
    midiOutReset(hmdo);

    reverse = !reverse;
    SendMessage(hrev, BM_SETCHECK,
		reverse? BST_CHECKED: BST_UNCHECKED, 0);
}

// Change key

UINT ChangeKey(HWND hkey)
{
    key = SendMessage(hkey, CB_GETCURSEL, 0, 0);
}

UINT ChangeVolume(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	    // Adjustments

	case SB_THUMBPOSITION:
	    volume = HIWORD(wParam);
	    break;

	case SB_LINELEFT:
	    volume--;
	    if (volume < 0)
		volume = 0;
	    break;

	case SB_LINERIGHT:
	    volume++;
	    if (volume > MAXVOL)
		volume = MAXVOL;
	    break;

	case SB_PAGELEFT:
	    volume -= 10;
	    if (volume < 0)
		volume = 0;
	    break;

	case SB_PAGERIGHT:
	    volume += 10;
	    if (volume > MAXVOL)
		volume = MAXVOL;
	    break;

	default:
	    return;
	}

	// Set the new position

	SendMessage((HWND)lParam, TBM_SETPOS, TRUE, volume);
}

// Change layout

UINT ChangeLayout(HWND hlay)
{
    layout = SendMessage(hlay, CB_GETCURSEL, 0, 0);
}

// Char pressed

UINT CharPressed(WPARAM w, LPARAM l)
{
    char *s;

    switch (w)
    {
	// Change key

    case 'E':
    case 'e':
	s = "Eb";
	break;

    case 'B':
    case 'b':
	s = "Bb";
	break;

    case 'F':
    case 'f':
	s = "F";
	break;

    case 'C':
    case 'c':
	s = "C";
	break;

    case 'G':
    case 'g':
	s = "G";
	break;

    case 'D':
    case 'd':
	s = "D";
	break;

    case 'A':
    case 'a':
	s = "A";
	break;

	// Reverse buttons

    case 'R':
    case 'r':
	ReverseButtons(hrev);
	return;

	// Anything else

    default:
	return;
    }

    // Change key

    SendMessage(hkey, CB_SELECTSTRING, -1, (LPARAM)s);
    ChangeKey(hkey);
}

// Key pressed

UINT KeyDown(WPARAM w, LPARAM l)
{
    // Check for a function key

    if (w >= VK_F1 && w <= VK_F12)
    {
	// Get the index

	int n = reverse? w - VK_F1: VK_F12 - w;
	int m = reverse? VK_F12 - w: w - VK_F1;

	// Check the button isn't pressed

	if (!buttons[n])
	{
	    // Look up the note and play it

	    buttons[n] = TRUE;
	    SendMessage(display[m], BM_SETSTATE, TRUE, 0);
	    int note = notes[layout][n][bellows] + keyvals[key];
	    ShortMessage(NOTEON, note, volume);
	}
    }

    // Check for the control key
#ifdef BASSBUTTONS
    else if (w == VK_CONTROL)
    {
	if (!control)
	{
	    control = TRUE;
	    SendMessage(bassdisp[BASS], BM_SETSTATE, TRUE, 0);
	    int note = bass[key][bellows];
	    ShortMessage(BASSON, note, volume);
	}
    }

    // Check for the menu key

    else if (w == VK_MENU)
    {
	if (!menu)
	{
	    menu = TRUE;
	    SendMessage(bassdisp[CHORD], BM_SETSTATE, TRUE, 0);
	    int note = chord[key][0][bellows];
	    ShortMessage(CHRDON, note, volume);
	    note = chord[key][1][bellows];
	    ShortMessage(CHRDON, note, volume);
	}
    }
#endif
    // Check for the space bar

    else if (w == VK_SPACE)
    {
	if (!bellows)
	{
	    int i;

	    // If there's a change of direction, reset the channel,
	    // and play all the notes for buttons that are pressed

	    bellows = TRUE;
	    SendMessage(spacebar, BM_SETSTATE, TRUE, 0);
	    midiOutReset(hmdo);
#ifdef BASSBUTTONS
	    if (control)
	    {
		int note = bass[key][bellows];
		ShortMessage(BASSON, note, volume);
	    }

	    if (menu)
	    {
		int note = chord[key][0][bellows];
		ShortMessage(CHRDON, note, volume);
		note = chord[key][1][bellows];
		ShortMessage(CHRDON + 1, note, volume);
	    }
#endif
	    for (i = 0; i != LENGTH(buttons); i++)
	    {
		if (buttons[i])
		{
		    int note = notes[layout][i][bellows] + keyvals[key];
		    ShortMessage(NOTEON, note, volume);
		}
	    }
	}
    }
}

// Key released

UINT KeyUp(WPARAM w, LPARAM l)
{
    // Check for a function key

    if (w >= VK_F1 && w <= VK_F12)
    {
	// Get the index

	int n = reverse? w - VK_F1: VK_F12 - w;
	int m = reverse? VK_F12 - w: w - VK_F1;

	// Check the button is pressed

	if (buttons[n])
	{
	    // Look up the note and stop it

	    buttons[n] = FALSE;
	    SendMessage(display[m], BM_SETSTATE, FALSE, 0);
	    int note = notes[layout][n][bellows] + keyvals[key];
	    ShortMessage(NOTEOFF, note, 0);
	}
    }

    // Check for the control key
#ifdef BASSBUTTONS
    else if (w == VK_CONTROL)
    {
	if (control)
	{
	    control = FALSE;
	    SendMessage(bassdisp[BASS], BM_SETSTATE, FALSE, 0);
	    int note = bass[key][bellows];
	    ShortMessage(BASSOFF, note, 0);
	}
    }

    // Check for the menu key

    else if (w == VK_MENU)
    {
	if (menu)
	{
	    menu = FALSE;
	    SendMessage(bassdisp[CHORD], BM_SETSTATE, FALSE, 0);
	    int note = chord[key][0][bellows];
	    ShortMessage(CHRDOFF, note, 0);
	    note = chord[key][1][bellows];
	    ShortMessage(CHRDOFF, note, 0);
	}
    }
#endif
    // Check for the space bar

    else if (w == VK_SPACE)
    {
	if (bellows)
	{
	    int i;

	    // If there's a change of direction, reset the channel,
	    // and play all the notes for buttons that are pressed

	    bellows = FALSE;
	    SendMessage(spacebar, BM_SETSTATE, FALSE, 0);
	    midiOutReset(hmdo);
#ifdef BASSBUTTONS
	    if (control)
	    {
		int note = bass[key][bellows];
		ShortMessage(BASSON, note, volume);
	    }

	    if (menu)
	    {
		int note = chord[key][0][bellows];
		ShortMessage(CHRDON, note, volume);
		note = chord[key][1][bellows];
		ShortMessage(CHRDON, note, volume);
	    }
#endif
	    for (i = 0; i != LENGTH(buttons); i++)
	    {
		if (buttons[i])
		{
		    int note = notes[layout][i][bellows] + keyvals[key];
		    ShortMessage(NOTEON, note, volume);
		}
	    }
	}
    }
}

// Send a short midi message

UINT ShortMessage(BYTE s, BYTE n, BYTE v)
{
    union
    {
	DWORD dw;
	BYTE b[4];
    } smsg;

    smsg.b[0] = s;
    smsg.b[1] = n;
    smsg.b[2] = v;
    smsg.b[3] = 0;

    midiOutShortMsg(hmdo, smsg.dw);
}
