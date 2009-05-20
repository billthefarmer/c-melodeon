////////////////////////////////////////////////////////////////////////////////
//
//  Sruti box - A simple Sruti box emulator written in C.
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

// Midi codes

#define NOTEOFF 0x80
#define NOTEON  0x90
#define CHANGE  0xc0

// Tool ids

#define STAT 1001
#define INST 1002
#define TXTS 1003
#define KEYS 1004
#define VOLM 1005
#define QUIT 1006
#define BTNS 1010

// Max volume

#define MAXVOL 127

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
    {"C", "G"};

int keyvals[] =
    {0, -5};

UINT key;

// Button handles

HWND buttons[12];
BOOL states[12];

// Volume value

UINT volume = MAXVOL;

// Function prototypes.

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
UINT ChangeInstrument(HWND);
UINT ChangeKey(HWND);
UINT ChangeVolume(WPARAM, LPARAM);
UINT ButtonClicked(WPARAM, LPARAM);
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
	    GetStockObject(WHITE_BRUSH);  // white background brush
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
		     "Sruti box",    // title-bar string
		     WS_OVERLAPPED |
		     WS_MINIMIZEBOX |
		     WS_SYSMENU,    // top level window
		     CW_USEDEFAULT, // default horizontal position
		     CW_USEDEFAULT, // default vertical position
		     CW_USEDEFAULT, // default width
		     CW_USEDEFAULT, // default height
		     (HWND)NULL,    // no owner window
		     (HMENU)NULL,   // use class menu
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
    BOOL bmsg;

    while ((bmsg = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0)
    {
	if (bmsg == -1)
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
    static HWND htxt;
    static HWND inst;
    static HWND hkey;
    static HWND hvol;
    static HWND quit;
    static HWND stat;

    // Combo box positions

    static int ival;
    static int kval;

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
	int width = 550 + border;
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
			 " Click the buttons to turn on"
			 " notes.",   // Text.
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
			 530,         // width.
			 86,          // height.
			 hWnd,        // Parent window.
			 (HMENU)INST, // Id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create text

	htxt =
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
			 WS_VISIBLE | WS_CHILD |
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

	// Create text

	htxt =
	    CreateWindow(WC_STATIC,   // Predefined class.
			 "Key:",      // Text.
			 WS_VISIBLE | WS_CHILD |
			 SS_LEFT,     // Styles.
			 420,         // x position.
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
			 458,         // x position.
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

	htxt =
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
	    CreateWindow(WC_SCROLLBAR, // scroll bar control class
			 NULL,         // no text
			 WS_VISIBLE | WS_CHILD |
			 SBS_HORZ,     // scroll bar styles
			 102,          // horizontal position
			 59,           // vertical position
			 168,          // width of the scroll bar
			 16,           // height of the scroll bar
			 hWnd,         // handle to main window
			 (HMENU)VOLM,  // id
			 hinst,        // instance owning this window
			 NULL);        // pointer not needed

	SetScrollRange(hvol,
		       SB_CTL,
		       0,
		       MAXVOL,
		       FALSE);

	SetScrollPos(hvol,
		     SB_CTL,
		     volume,
		     FALSE);

	// Create quit button

	quit =
	    CreateWindow(WC_BUTTON,  // Predefined class.
			 "Quit",     // Text.
			 WS_VISIBLE | WS_CHILD |
			 BS_PUSHBUTTON, // Styles.
			 457,         // x position.
			 53,          // y position.
			 74,          // width.
			 26,          // height.
			 hWnd,        // Parent window.
			 (HMENU)QUIT, // Id.
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
			 90,          // y position.
			 530,         // width.
			 bottom - 100,// height.
			 hWnd,        // Parent window.
			 NULL,        // No id.
			 hinst,       // handle to application instance
			 NULL);       // Pointer not needed.

	// Create buttons

	for (i = 0; i != LENGTH(buttons); i++)
	{
	    int top;

	    switch (i)
	    {
	    case 1:
	    case 3:
	    case 6:
	    case 8:
	    case 10:
		top = 108;
		break;

	    default:
		top = 208;
		break;
	    }

	    buttons[i] =
		CreateWindow(WC_BUTTON,  // Predefined class.
			     NULL,       // No text.
			     WS_VISIBLE | WS_CHILD | BS_PUSHLIKE |
			     BS_CHECKBOX, // Styles.
			     21 + 43 * i, // x position.
			     top,         // y position.
			     34,          // width.
			     134,         // height.
			     hWnd,        // Parent window.
			     (HMENU)(BTNS + i), // Id.
			     hinst,       // handle to application instance
			     NULL);       // Pointer not needed.
	}

	// Open a midi out device

	midiOutOpen(&hmdo, 0, 0, 0, CALLBACK_NULL);

	// Change the instrument

	ChangeInstrument(inst);

	// Change the key

	ChangeKey(hkey);
	break;

	// Colour static text, defeat DefWindowProc() by capturing
	// this message

    case WM_CTLCOLORSTATIC:
	break;

	// Character key pressed

    case WM_CHAR:
	switch (wParam)
	{
	    // Change key

	case 'C':
	case 'c':
	    s = "C";
	    break;

	case 'G':
	case 'g':
	    s = "G";
	    break;

	    // Anything else

	default:
	    return 0;
	}

	// Change key

	SendMessage(hkey, CB_SELECTSTRING, -1, (LPARAM)s);
	ChangeKey(hkey);
	break;

    case WM_HSCROLL:
	ChangeVolume(wParam, lParam);

	// Set the focus back to the window

	SetFocus(hWnd);
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
		SetFocus(hWnd);
		break;

	    case CBN_SELENDCANCEL:
		SendMessage((HWND)lParam, CB_SETCURSEL, ival, 0);
		SetFocus(hWnd);
		break;
	    }
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
		SetFocus(hWnd);
		break;

	    case CBN_SELENDCANCEL:
		SendMessage((HWND)lParam, CB_SETCURSEL, kval, 0);
		SetFocus(hWnd);
		break;
	    }
	    break;

	    // Buttons

	default:
	    if (HIWORD(wParam) == BN_CLICKED)
		ButtonClicked(wParam, lParam);
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
    int i;
    int inst = SendMessage(hinst, CB_GETCURSEL, 0, 0);

    midiOutReset(hmdo);
    ShortMessage(CHANGE, inst, 0);

    for (i = 0; i != LENGTH(buttons); i++)
	if (states[i])
	    ShortMessage(NOTEON, 60 + i + keyvals[key], volume);
}

// Change key

UINT ChangeKey(HWND hkey)
{
    int i;

    key = SendMessage(hkey, CB_GETCURSEL, 0, 0);

    midiOutReset(hmdo);
    for (i = 0; i != LENGTH(buttons); i++)
	if (states[i])
	    ShortMessage(NOTEON, 60 + i + keyvals[key], volume);
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
    }

    // Set the new position

    SetScrollPos((HWND)lParam,
		 SB_CTL,
		 volume,
		 TRUE);
    int i;

    midiOutReset(hmdo);
    for (i = 0; i != LENGTH(buttons); i++)
	if (states[i])
	    ShortMessage(NOTEON, 60 + i + keyvals[key], volume);
}

// Button clicked

UINT ButtonClicked(WPARAM wParam, LPARAM lParam)
{
    int button = LOWORD(wParam)- BTNS;

    // Range check

    if (button < 0 || button > LENGTH(buttons))
	return;

    states[button] = !states[button];
    SendMessage((HWND)lParam, BM_SETCHECK,
		states[button]? BST_CHECKED: BST_UNCHECKED, 0);

    // Play note

    if (states[button])
	ShortMessage(NOTEON, 60 + button + keyvals[key], volume);

    else
	ShortMessage(NOTEOFF, 60 + button + keyvals[key], 0);
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

    return midiOutShortMsg(hmdo, smsg.dw);
}
