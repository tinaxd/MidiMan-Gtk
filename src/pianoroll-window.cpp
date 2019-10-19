/* midiman-window.cpp
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

#include "pianoroll-window.h"

#include <iostream>

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/adjustment.h>

PianorollWindow::PianorollWindow()
{
    this->set_title("Pianoroll");

    /*
    Gtk::Adjustment *_hadjustment = new Gtk::Adjustment(400, 300, 500);
    Gtk::Adjustment *_vadjustment = new Gtk::Adjustment(400, 300, 500);
    Glib::RefPtr<Gtk::Adjustment> hadjustment(_hadjustment);
    Glib::RefPtr<Gtk::Adjustment> vadjustment(_vadjustment);*/

    // Create ScrolledWindow
    scwindow = new Gtk::ScrolledWindow();
    //viewport = new Gtk::Viewport(hadjustment, vadjustment);
    //scwindow->add(*viewport);
    //scwindow->set_hexpand(true);
    //scwindow->set_vexpand(true);
    //scwindow->set_size_request(300, 300);
    scwindow->set_policy(Gtk::PolicyType::POLICY_ALWAYS, Gtk::PolicyType::POLICY_ALWAYS);

    // Create DrawingArea and add to ScrolledWindow
    drawarea = new Gtk::DrawingArea();
    drawarea->set_size_request(500, 500);

    // Add each other
    scwindow->add(*drawarea);
    this->add(*scwindow);

    // Signals

}

PianorollWindow::~PianorollWindow()
{
    delete drawarea;
}

bool PianorollWindow::on_motion_notify_event(GdkEventMotion *motion_event)
{
    std::cout << "Motion event!: " << motion_event->x << ", " << motion_event->y << std::endl;
    return true;
}

void PianorollWindow::draw_note(MidiNote note)
{

}

void PianorollWindow::update_notes_drawing(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgba(1, 0, 0, 1);
    cr->save();
    const double key_height = white_height * 7 / 12;

    while (!note_drawed.empty())
    {
        DrawCord cord = note_drawed.front();
        std::cout << "Cord: " << cord.x << ", " << cord.y << std::endl;
        cr->rectangle(cord.x, cord.y, cord.length, key_height);
        cr->fill();

        note_drawed.pop();
    }
}

void PianorollWindow::add_note(MidiNote note)
{
    notes.push_back(note);
}

bool PianorollWindow::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    cr->translate(0, pianoroll_upmargin);

    draw_keyboard(cr);

    cr->translate(pianoroll_leftmargin + white_width, 0);

    draw_gridhelper(cr);

    // Set background color
    cr->set_source_rgba(0.86, 0.86, 0.86, 0.8);
    cr->rectangle(0, 0, width, height);
    cr->fill();


    calculate_note_positions();
    update_notes_drawing(cr);

    return true;
}

void PianorollWindow::set_x_scale(double scale)
{
    this->x_scale = scale;
}

void PianorollWindow::set_y_scale(double scale)
{
    this->y_scale = scale;
}

void PianorollWindow::draw_keyboard(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_line_width(1);
    // Draw white keys
    for (int i=0; i<127; i++) {
        cr->set_source_rgba(0, 0, 0, 1);
        cr->rectangle(0, i * white_height, white_width, white_height);
        cr->stroke();
        cr->rectangle(0, i * white_height, white_width, white_height);
        cr->set_source_rgba(1, 1, 1, 1);
        cr->fill();
    }

    // Draw (the half of) the first black key
    cr->set_source_rgba(0, 0, 0, 1);
    cr->rectangle(0, 0, black_width, black_height / 2);
    cr->fill();

    int black_index = 2;
    // Draw the rest of the black keys
    for (int i=0; i<53; i++) {
        if (black_index == 3) {
            black_index++;
            continue;
        } else if (black_index == 6) {
            black_index = 0;
            continue;
        } else {
            double left_up_y = white_height * (i + 1) - black_height / 2;
            cr->rectangle(0, left_up_y, black_width, black_height);
            cr->fill();
            black_index++;
        }
    }
}

void PianorollWindow::draw_gridhelper(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int window_width = allocation.get_width();
    const int window_height = allocation.get_height();

    const double key_height = white_height * 7 / 12;
    const double pianoroll_width = window_width - white_width;

    cr->set_source_rgba(1, 1, 1, 1);
    cr->set_line_width(1.3);

    double last_base_white = -2 * white_height;
    int key_index = 3;
    int octave_index = 0;
    for (int i=0; i<127; i++) {
        if (key_index == 11) {
            key_index = 0;
            octave_index++;
            continue;
        } else if (key_index == 0) {
            last_base_white = (7 * octave_index - 2) * white_height;
            cr->rectangle(0, last_base_white, pianoroll_width, key_height);
            std::cout << "set: " << last_base_white << std::endl;
            key_heights[i] = last_base_white;
        } else {
            double y = last_base_white + key_height * key_index;
            cr->rectangle(0, y, pianoroll_width, key_height);
            std::cout << "set: " << y << std::endl;
            key_heights[i] = y;
        }
        cr->stroke();

        key_index++;
    }

    cr->move_to(0, 0);
    cr->line_to(0, window_height);
    cr->stroke();
}

void PianorollWindow::calculate_note_positions()
{
    for (auto it=notes.begin(); it!=notes.end(); it++) {
        auto note = *it;
        std::cout << key_heights[127-note.note] << std::endl;
        DrawCord cord = DrawCord {note.start * measure_width / measure_division, key_heights[127-note.note], note.length * measure_width / measure_division};
        this->note_drawed.push(cord);
    }
}
