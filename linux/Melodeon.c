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

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <fluidsynth.h>

// Macros

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))

// Midi channels

#define NOTE_CHANNEL 0
#define BASS_CHANNEL 1
#define CHRD_CHANNEL 2

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

// Space

#define MARGIN 10

// Sount font file

#define SOUND_FONT_FILE "/usr/local/share/soundfonts/8MBGMSFX.SF2"

// List of midi instruments

static char *instruments[] =
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

static char *keys[] =
    {"Eb", "Bb", "F", "C", "G", "D", "A"};

int keyvals[] =
    {3, -2, 5, 0, -5, 2, -3};

int key;

GtkWidget *key_combo;

// Layouts

static char *layouts[] =
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
// Buttons

gboolean buttons[BUTTONS];
#ifdef BASSBUTTONS
gboolean control;
gboolean alt;
#endif
gboolean bellows;

// Display widgets

GtkWidget *display[BUTTONS];
#ifdef BASSBUTTONS
GtkWidget *bassdisp[2];
#endif
GtkWidget *spacebar;

// Reverse buttons

gboolean reverse;

GtkWidget *reverse_button;

// Volume value

int volume = MAXVOL;

// Midi ports

char **ports;

int port;

// Function declarations
int instrument_changed(GtkWidget*, fluid_synth_t*);
int key_changed(GtkWidget*, GtkWindow*);
int layout_changed(GtkWidget*, GtkWindow*);
int reverse_changed(GtkWidget*, GtkWindow*);
int volume_changed(GtkWidget*, gdouble, GtkWindow*);
int quit_clicked(GtkWidget*, GtkWindow*);
int button_clicked(GtkWidget*, gboolean*);
int key_press(GtkWidget*, GdkEventKey*, fluid_synth_t*);
int key_release(GtkWidget*, GdkEventKey*, fluid_synth_t*);

// Main function

int main(int argc, char *argv[])
{
    // Widgets

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *wbox;
    GtkWidget *hbox;
    GtkWidget *gbox;
    GtkWidget *table;
    GtkWidget *frame;
    GtkWidget *label;
    GtkWidget *combo;
    GtkWidget *quit;
    GtkWidget *volume;
    GtkWidget *reverse;
    GtkWidget *separator;

    // Fluidsynth

    fluid_synth_t *synth;
    fluid_settings_t *settings;
    fluid_audio_driver_t *adriver;

    int id;
    int i;

    // Create fluidsynth settings

    settings = new_fluid_settings();

    // Set driver

    fluid_settings_setstr(settings, "audio.driver", "alsa");

    // Create synthesizer

    synth = new_fluid_synth(settings);

    // Create audio driver

    adriver = new_fluid_audio_driver(settings, synth);

    // Load soundfont

    id = fluid_synth_sfload(synth, SOUND_FONT_FILE, 0);

    // Check the soundfont has loaded

    if (id == -1)
    {
	// Clean up

	delete_fluid_audio_driver(adriver);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);

	return 1;
    }

    // Initialise GTK

    gtk_init(&argc, &argv);

    // Create main window

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Melodeon");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // W box, this contains the fake status bar and the rest of the
    // display

    wbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), wbox);

    // Label, this label and separator are a fake status bar that can
    // have small text

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
		    "<small>Press the function keys F1-F12 as"
		    " melodeon buttons and the space bar"
		    " as the bellows.</small>");
    gtk_box_pack_end(GTK_BOX(wbox), label, FALSE, FALSE, 0);

    // Separator

    separator = gtk_hseparator_new();
    gtk_box_pack_end(GTK_BOX(wbox), separator, FALSE, FALSE, 0);

    // V box, this contains three frames

    vbox = gtk_vbox_new(FALSE, MARGIN);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), MARGIN);
    gtk_box_pack_start(GTK_BOX(wbox), vbox, FALSE, FALSE, 0);

    // Frame, this contains all the tool widgets

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // W Box

    wbox = gtk_vbox_new(FALSE, MARGIN);
    gtk_container_set_border_width(GTK_CONTAINER(wbox), MARGIN);
    gtk_container_add(GTK_CONTAINER(frame), wbox);

    // H box

    hbox = gtk_hbox_new(FALSE, MARGIN);
    gtk_box_pack_start(GTK_BOX(wbox), hbox, FALSE, FALSE, 0);

    // Instrument label

    label = gtk_label_new("Instrument:");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    // Instrument combo box

    combo = gtk_combo_box_new_text();

    for (i = 0; i != LENGTH(instruments); i++)
    {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), instruments[i]);
	if (strcmp(instruments[i], "Accordion") == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
    }

    gtk_box_pack_start(GTK_BOX(hbox), combo, FALSE, FALSE, 0);

    // Instrument changed

    g_signal_connect(G_OBJECT(combo), "changed",
		     G_CALLBACK(instrument_changed), synth);

    instrument_changed(combo, synth);

    // Key combo box

    combo = gtk_combo_box_new_text();
    for (i = 0; i != LENGTH(keys); i++)
    {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), keys[i]);
	if (strcmp(keys[i], "C") == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
    }
    gtk_widget_set_size_request(combo, 53, 29);
    gtk_box_pack_end(GTK_BOX(hbox), combo, FALSE, FALSE, 1);
    key_combo = combo;

    // Key changed

    g_signal_connect(G_OBJECT(combo), "changed",
		     G_CALLBACK(key_changed), window);

    key_changed(combo, GTK_WINDOW(window));

    // Key label

    label = gtk_label_new("Key:");
    gtk_box_pack_end(GTK_BOX(hbox), label, FALSE, FALSE, 1);

    // Reverse check button

    reverse = gtk_check_button_new_with_mnemonic("_Reverse buttons");
    gtk_box_pack_end(GTK_BOX(hbox), reverse, FALSE, FALSE, 0);
    reverse_button = reverse;

    // Reverse toggled

    g_signal_connect(G_OBJECT(reverse), "toggled",
		     G_CALLBACK(reverse_changed), window);

    // H box

    hbox = gtk_hbox_new(FALSE, MARGIN);
    gtk_box_pack_start(GTK_BOX(wbox), hbox, FALSE, FALSE, 0);

    // Quit button

    quit = gtk_button_new_with_mnemonic("_Quit");
    gtk_widget_set_size_request(quit, 55, 31);
    gtk_box_pack_end(GTK_BOX(hbox), quit, FALSE, FALSE, 0);

    // Quit clicked

    g_signal_connect(G_OBJECT(quit), "clicked",
		     G_CALLBACK(quit_clicked), window);

    // Layout combo box

    combo = gtk_combo_box_new_text();
    for (i = 0; i != LENGTH(layouts); i++)
    {
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), layouts[i]);
	if (strcmp(layouts[i], "Hohner") == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
    }
    gtk_box_pack_end(GTK_BOX(hbox), combo, FALSE, FALSE, 0);

    // Layout changed

    g_signal_connect(G_OBJECT(combo), "changed",
		     G_CALLBACK(layout_changed), window);

    layout_changed(combo, GTK_WINDOW(window));

    // Layout label

    label = gtk_label_new("Layout:");
    gtk_box_pack_end(GTK_BOX(hbox), label, FALSE, FALSE, 0);

    // G box

    gbox = gtk_hbox_new(FALSE, MARGIN);
    gtk_box_pack_end(GTK_BOX(hbox), gbox, FALSE, FALSE, MARGIN);

    // Volume label

    label = gtk_label_new("Volume:");
    gtk_box_pack_start(GTK_BOX(gbox), label, FALSE, FALSE, 0);

    // Volume button

    volume = gtk_volume_button_new();
    gtk_scale_button_set_value(GTK_SCALE_BUTTON(volume), 1.0);
    gtk_box_pack_start(GTK_BOX(gbox), volume, FALSE, FALSE, 0);

    // Volume changed

    g_signal_connect(G_OBJECT(volume), "value-changed",
		     G_CALLBACK(volume_changed), window);

    // Frame

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // Label

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
			 "\n\n<big><b>Melodeon</b>\n\n"
			 "Play Melodeon on your keyboard</big>\n\n");

    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(frame), label);

    // Frame

    frame = gtk_frame_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

    // Table

    table = gtk_table_new(12, 2, FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(table), MARGIN);
    gtk_table_set_row_spacing(GTK_TABLE(table), 0, MARGIN);
    gtk_table_set_col_spacings(GTK_TABLE(table), MARGIN);
    gtk_container_add(GTK_CONTAINER(frame), table);

    // Bass buttons
#ifdef BASSBUTTONS
    for (i = 0; i != LENGTH(bassdisp); i++)
    {
	bassdisp[i] = gtk_toggle_button_new();
	gtk_widget_set_size_request(bassdisp[i], SIZE, SIZE);
	gtk_table_attach_defaults(GTK_TABLE(table),
				  bassdisp[i], i * 2, i * 2 + 1, 0, 1);

	// Button callback

	g_signal_connect(G_OBJECT(bassdisp[i]), "clicked",
			 G_CALLBACK(button_clicked),
			 i == 0? &control: &alt);
    }
#endif
    // Spacebar

    spacebar = gtk_toggle_button_new();
    gtk_widget_set_size_request(spacebar,
				(SIZE + MARGIN) * 5 + SIZE, SIZE);
    gtk_table_attach_defaults(GTK_TABLE(table), spacebar, 3, 9, 0, 1);

    // Button callback

    g_signal_connect(G_OBJECT(spacebar), "clicked",
		     G_CALLBACK(button_clicked), &bellows);

    // Buttons

    for (i = 0; i != LENGTH(display); i++)
    {
	display[i] = gtk_toggle_button_new();
	gtk_widget_set_size_request(display[i], SIZE, SIZE);
	gtk_table_attach_defaults(GTK_TABLE(table),
				  display[i], i, i + 1, 1, 2);

	// Button callback

	g_signal_connect(G_OBJECT(display[i]), "clicked",
			 G_CALLBACK(button_clicked), &buttons[i]);
    }

    // Stop autorepeat, this is a bit arcane and is global, so must be
    // restored back again

    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
    GdkDisplay *gd = gdk_screen_get_display(screen);
    Display *disp = gdk_x11_display_get_xdisplay(gd);

    // Structures for the keyboard control functions

    XKeyboardState state;
    XKeyboardControl control;

    // Get the state of the keyboard

    XGetKeyboardControl(disp, &state);

    // Turn the auto repeat off

    control.auto_repeat_mode = AutoRepeatModeOff;
    XChangeKeyboardControl(disp, KBAutoRepeatMode, &control);

    // Key pressed callback

    g_signal_connect(G_OBJECT(window), "key_press_event",
		     G_CALLBACK(key_press), synth);

    // Key released callback

    g_signal_connect(G_OBJECT(window), "key_release_event",
		     G_CALLBACK(key_release), synth);

    // Destroy window callback

    g_signal_connect(window, "destroy",
		     G_CALLBACK(gtk_main_quit), NULL);

    // Show the window

    gtk_widget_show_all(window);

    // Set focus to the window

    gtk_window_set_focus(GTK_WINDOW(window), NULL);

    // Interact with user

    gtk_main();

    // Clean up

    delete_fluid_audio_driver(adriver);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);

    // Set auto repeat back

    control.auto_repeat_mode = state.global_auto_repeat;
    XChangeKeyboardControl(disp, KBAutoRepeatMode, &control);

    // Make sure it gets there

    XFlush(disp);

   // Exit

    return 0;
}

// Quit callback

int quit_clicked(GtkWidget *widget, GtkWindow *window)
{
    // Create a message dialog

    GtkWidget *dialog =
	gtk_message_dialog_new(GTK_WINDOW(window),
			       GTK_DIALOG_MODAL,
			       GTK_MESSAGE_QUESTION,
			       GTK_BUTTONS_OK_CANCEL,
			       "Quit");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
					     "Really?");
    int result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_OK)
	gtk_main_quit();
}

// Instrument changed

int instrument_changed(GtkWidget *widget, fluid_synth_t *synth)
{
    int inst = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    fluid_sfont_t *sfont = fluid_synth_get_sfont(synth, 0);
    int id = sfont->id;

    fluid_synth_program_select(synth, NOTE_CHANNEL, id, 0, inst); 

#ifdef BASSBUTTONS

    fluid_synth_program_select(synth, BASS_CHANNEL, id, 0, inst);
    fluid_synth_program_select(synth, CHRD_CHANNEL, id, 0, inst);

#endif
}

// Key changed

int key_changed(GtkWidget *widget, GtkWindow *window)
{
    key = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Layout changed

int layout_changed(GtkWidget *widget, GtkWindow *window)
{
    layout = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Reverse changed

int reverse_changed(GtkWidget *widget, GtkWindow *window)
{
    reverse = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Volume changed

int volume_changed(GtkWidget *widget, gdouble value, GtkWindow *window)
{
    volume = value * MAXVOL;
    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Key press event

int key_press(GtkWidget *window, GdkEventKey *event, fluid_synth_t *synth)
{
    switch (event->keyval)
    {
	// Space bar

    case GDK_space:
	if (!bellows)
	{
	    int i;

	    // If there's a change of direction, stop and play all the
	    // notes for buttons that are pressed

	    bellows = TRUE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(spacebar),
					 TRUE);
#ifdef BASSBUTTONS
	    if (control)
	    {
		int note = bass[key][!bellows];
		fluid_synth_noteoff(synth, BASS_CHANNEL, note);
		note = bass[key][bellows];
		fluid_synth_noteon(synth, BASS_CHANNEL, note, volume);
	    }

	    if (alt)
	    {
		int note = chord[key][0][!bellows];
		fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
		note = chord[key][1][!bellows];
		fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
		note = chord[key][0][bellows];
		fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
		note = chord[key][1][bellows];
		fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
	    }
#endif
	    for (i = 0; i != LENGTH(buttons); i++)
	    {
		if (buttons[i])
		{
		    int note = notes[layout][i][!bellows] + keyvals[key];
		    fluid_synth_noteoff(synth, NOTE_CHANNEL, note);
		    note = notes[layout][i][bellows] + keyvals[key];
		    fluid_synth_noteon(synth, NOTE_CHANNEL, note, volume);
		}
	    }
	}
	break;
#ifdef BASSBUTTONS
	// Control key

    case GDK_Control_L:
    case GDK_Control_R:
	if (!control)
	{
	    control = TRUE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisp[BASS]),
					 TRUE);
	    int note = bass[key][bellows];
	    fluid_synth_noteon(synth, BASS_CHANNEL, note, volume);
	}
	break;

	// Alt key

    case GDK_Alt_L:
    case GDK_Alt_R:
	if (!alt)
	{
	    alt = TRUE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisp[CHORD]),
					 TRUE);
	    int note = chord[key][0][bellows];
	    fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
	    note = chord[key][1][bellows];
	    fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
	}
	break;
#endif
	// Function keys

    default:
	if (event->keyval >= GDK_F1 && event->keyval <= GDK_F12)
	{
	    // Get the index

	    int n = reverse? event->keyval - GDK_F1: GDK_F12 - event->keyval;
	    int m = reverse? GDK_F12 - event->keyval: event->keyval - GDK_F1;

	    // Check the button is pressed

	    if (!buttons[n])
	    {
		buttons[n] = TRUE;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[m]),
					     TRUE);
		int note = notes[layout][n][bellows] + keyvals[key];
		fluid_synth_noteon(synth, NOTE_CHANNEL, note, volume);
	    }
	}
    }

    gtk_window_set_focus(GTK_WINDOW(window), NULL);
}

// Key release event

int key_release(GtkWidget *window, GdkEventKey *event, fluid_synth_t *synth)
{
    int i;
    char *s = "";

    switch (event->keyval)
    {
    case GDK_space:
	if (bellows)
	{

	    int i;

	    // If there's a change of direction, stop and play all the
	    // notes for buttons that are pressed

	    bellows = FALSE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(spacebar),
					 FALSE);
#ifdef BASSBUTTONS
	    if (control)
	    {
		int note = bass[key][!bellows];
		fluid_synth_noteoff(synth, BASS_CHANNEL, note);
		note = bass[key][bellows];
		fluid_synth_noteon(synth, BASS_CHANNEL, note, volume);
	    }

	    if (alt)
	    {
		int note = chord[key][0][!bellows];
		fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
		note = chord[key][1][!bellows];
		fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
		note = chord[key][0][bellows];
		fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
		note = chord[key][1][bellows];
		fluid_synth_noteon(synth, CHRD_CHANNEL, note, volume);
	    }
#endif
	    for (i = 0; i != LENGTH(buttons); i++)
	    {
		if (buttons[i])
		{
		    int note = notes[layout][i][!bellows] + keyvals[key];
		    fluid_synth_noteoff(synth, NOTE_CHANNEL, note);
		    note = notes[layout][i][bellows] + keyvals[key];
		    fluid_synth_noteon(synth, NOTE_CHANNEL, note, volume);
		}
	    }
	}
	return;

    case GDK_r:
    case GDK_R:
	reverse = !reverse;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(reverse_button),
				     reverse);
	return;

    case GDK_q:
    case GDK_Q:
	quit_clicked(NULL, GTK_WINDOW(window));
	return;

    case GDK_e:
    case GDK_E:
	s = "Eb";
	break;

    case GDK_b:
    case GDK_B:
	s = "Bb";
	break;

    case GDK_f:
    case GDK_F:
	s = "F";
	break;

    case GDK_c:
    case GDK_C:
	s = "C";
	break;

    case GDK_g:
    case GDK_G:
	s = "G";
	break;

    case GDK_d:
    case GDK_D:
	s = "D";
	break;

    case GDK_a:
    case GDK_A:
	s = "A";
	break;

#ifdef BASSBUTTONS
	// Control key

    case GDK_Control_L:
    case GDK_Control_R:
	if (control)
	{
	    control = FALSE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisp[BASS]),
					 FALSE);
	    int note = bass[key][bellows];
	    fluid_synth_noteoff(synth, BASS_CHANNEL, note);
	}
	return;

	// Alt key

    case GDK_Alt_L:
    case GDK_Alt_R:
	if (alt)
	{
	    alt = FALSE;
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bassdisp[CHORD]),
					 FALSE);
	    int note = chord[key][0][bellows];
	    fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
	    note = chord[key][1][bellows];
	    fluid_synth_noteoff(synth, CHRD_CHANNEL, note);
	}
	return;
#endif
	// Function keys

    default:
	if (event->keyval >= GDK_F1 && event->keyval <= GDK_F12)
	{
	    // Get the index

	    int n = reverse? event->keyval - GDK_F1: GDK_F12 - event->keyval;
	    int m = reverse? GDK_F12 - event->keyval: event->keyval - GDK_F1;

	    // Check the button is pressed

	    if (buttons[n])
	    {
		buttons[n] = FALSE;
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(display[m]),
					     FALSE);
		int note = notes[layout][n][bellows] + keyvals[key];
		fluid_synth_noteoff(synth, NOTE_CHANNEL, note);
	    }
	}
	return;
    }

    for (i = 0; i != LENGTH(keys); i++)
	if (strcmp(s, keys[i]) == 0)
	    gtk_combo_box_set_active(GTK_COMBO_BOX(key_combo), i);
}

// Reset the toggle button if clicked

int button_clicked(GtkWidget *button, gboolean *pressed)
{
//     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button),
//     				 *pressed);
}
