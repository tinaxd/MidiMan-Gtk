/* pianoroll-winow.h
 *
 * Copyright 2019 tinaxd
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name(s) of the above copyright
 * holders shall not be used in advertising or otherwise to promote the sale,
 * use or other dealings in this Software without prior written
 * authorization.
 */

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/viewport.h>

#include <queue>
#include <vector>

struct MidiNote
{
  int note;
  int length;
  int start;
};

struct DrawCord
{
  double x;
  double y;
  double length;
};

class PianorollWindow: public Gtk::Window
{
public:
PianorollWindow();
virtual ~PianorollWindow();
void add_note(MidiNote note);

void set_x_scale(double scale);
void set_y_scale(double scale);

void calculate_note_positions();

private:
// Vector<MidiEvent> midiInfo;
Gtk::DrawingArea *drawarea;
Gtk::ScrolledWindow *scwindow;
Gtk::Viewport *viewport;

bool drawarea_on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
//std::queue<DrawCord> drawing_queue;

std::queue<DrawCord> note_drawed;
std::vector<MidiNote> notes;

bool draw_initialized;

double y_scale = 5.0;
double x_scale = 5.0;

double key_heights[127];

protected:
virtual bool on_motion_notify_event(GdkEventMotion *motion_event);
void draw_note(MidiNote note);
virtual void update_notes_drawing(const Cairo::RefPtr<Cairo::Context>& cr);
virtual void draw_keyboard(const Cairo::RefPtr<Cairo::Context>& cr);
virtual void draw_gridhelper(const Cairo::RefPtr<Cairo::Context>& cr);
virtual void draw_timeline(const Cairo::RefPtr<Cairo::Context>& cr);

double white_width = 60;
double white_height = 30;
double black_width = 25;
double black_height = 20;
double pianoroll_leftmargin = 5;
double pianoroll_upmargin = 25;

int measure_division = 4;
double measure_width = 300;
};

