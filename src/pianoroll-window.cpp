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
    //DrawCord cord {motion_event->x, motion_event->y};
    //drawing_queue.push(cord);
    return true;
}

void PianorollWindow::draw_note(MidiNote note)
{

}

void PianorollWindow::update_notes_drawing(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgba(1, 0, 0, 1);
    cr->save();

    for (auto it=note_drawed.begin(); it!=note_drawed.end(); it++)
    {
        cr->rectangle((*it).x + white_width, (*it).y, (*it).length * x_scale, 5 * y_scale);
        cr->fill();
    }
}

void PianorollWindow::add_note(MidiNote note)
{
    DrawCord cord = DrawCord {note.start, note.note, note.length};
    this->note_drawed.push_back(cord);
}

bool PianorollWindow::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    draw_keyboard(cr);
    draw_gridhelper(cr);

    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);
    cr->rectangle(60, 0, 1000, 1000);
    cr->fill();
    //draw_initialized = true;

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
