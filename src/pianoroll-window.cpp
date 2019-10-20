/* pianoroll-window.cpp
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

#define QUARTER_LENGTH (measure_width / measure_division)

#define WHITE_KEYS 75

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
    scwindow->set_policy(Gtk::PolicyType::POLICY_ALWAYS, Gtk::PolicyType::POLICY_ALWAYS);


    // Create DrawingArea and add to ScrolledWindow
    drawarea = new Gtk::DrawingArea();
    drawarea->set_size_request(2000, 2000);

    // Add each other
    scwindow->add(*drawarea);
    this->add(*scwindow);

    this->show_all_children();

    // Signals
    drawarea->signal_draw().connect(sigc::mem_fun(this, &PianorollWindow::drawarea_on_draw));
    scwindow->get_hadjustment()->signal_value_changed().connect(sigc::mem_fun(this, &PianorollWindow::request_redraw));
    scwindow->get_vadjustment()->signal_value_changed().connect(sigc::mem_fun(this, &PianorollWindow::request_redraw));
}

PianorollWindow::~PianorollWindow()
{
    delete drawarea;
}

bool PianorollWindow::on_motion_notify_event(GdkEventMotion *motion_event)
{
    MidiNote note = {
        int(127 - motion_event->y),
        1,
        int(motion_event->x)
    };
    //this->add_note(note);
    return true;
}

void PianorollWindow::draw_note(MidiNote note)
{

}

void PianorollWindow::update_notes_drawing(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgba(1, 0, 0, 1);
    const double key_height = white_height * 7 / 12;

    while (!note_drawed.empty())
    {
        DrawCord cord = note_drawed.front();
        //std::cout << "Cord: " << cord.x << ", " << cord.y << std::endl;
        cr->rectangle(cord.x, cord.y, cord.length, key_height);
        cr->fill();

        note_drawed.pop();
    }
}

void PianorollWindow::add_note(MidiNote note)
{
    notes.push_back(note);
}

bool PianorollWindow::drawarea_on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    /*auto allocation = get_allocation();
    const double window_width = allocation.get_width();
    const double window_height = allocation.get_height();*/

    auto hadjustment = scwindow->get_hadjustment();
    auto vadjustment = scwindow->get_vadjustment();
    const double left_up_x = hadjustment->get_value();
    const double left_up_y = vadjustment->get_value();

    // Sync with vertical scroll
    cr->translate(0, left_up_y);
    draw_timeline(cr);
    cr->translate(0, -left_up_y);
    
    // Sync with horizontal scroll
    cr->translate(left_up_x, pianoroll_upmargin);
    draw_keyboard(cr);
    cr->translate(pianoroll_leftmargin + white_width, 0);
    draw_gridhelper(cr);



    // Set background color
    /*cr->set_source_rgba(0.86, 0.86, 0.86, 0.8);
    cr->rectangle(0, 0, width, height);
    cr->fill();*/

    // Do not sync with scroll
    cr->translate(-left_up_x, 0);
    calculate_note_positions();
    update_notes_drawing(cr);

    //drawarea->set_size_request(2000, 2000);
    //drawarea->queue_draw_area(left_up_x, left_up_y, window_width, window_height);

    return true;
}

void PianorollWindow::request_redraw()
{
    auto allocation = get_allocation();
    const double window_width = allocation.get_width();
    const double window_height = allocation.get_height();

    auto hadjustment = scwindow->get_hadjustment();
    auto vadjustment = scwindow->get_vadjustment();
    const double left_up_x = hadjustment->get_value();
    const double left_up_y = vadjustment->get_value();
    
    drawarea->queue_draw_area(left_up_x, left_up_y, window_width, window_height);
}

/*
bool PianorollWindow::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{

}
*/

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
    cr->set_font_size(13);
    // Draw white keys
    for (int i=0; i<WHITE_KEYS-1; i++) {
        cr->set_source_rgba(0, 0, 0, 1);
        cr->rectangle(0, i * white_height, white_width, white_height);
        cr->stroke();
        cr->rectangle(0, i * white_height, white_width, white_height);
        cr->set_source_rgba(1, 1, 1, 1);
        cr->fill();
        if ((i-4) % 7 == 0) {
            cr->set_source_rgba(0, 0, 0, 1);
            cr->move_to(white_width/2+2, (i+1) * white_height - 5);
            int index = (i-4) / 7;
            char str[4];
            sprintf(str, "C%d", 9-index);
            cr->show_text(str);
        }
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
    const double key_height = white_height * 7 / 12;
    const double pianoroll_width = max_width - white_width;

    cr->set_source_rgba(1, 1, 1, 0.8);
    cr->set_line_width(1.3);
    cr->save();

    double last_base_white = -2 * white_height;
    int key_index = 3;
    int octave_index = 0;
    for (int i=0; i<127; i++) {
        if (key_index == 11) {
            key_index = 0;
            octave_index++;
            continue;
        } else if (key_index == 0) {
            cr->set_source_rgba(0.2, 0.2, 0.2, 1);
            cr->set_line_width(2);
            last_base_white = (7 * octave_index - 2) * white_height;
            cr->rectangle(0, last_base_white, pianoroll_width, key_height);
            //std::cout << "set: " << last_base_white << std::endl;
            key_heights[i] = last_base_white;
            cr->restore();
            cr->save();
        } else {
            double y = last_base_white + key_height * key_index;
            cr->rectangle(0, y, pianoroll_width, key_height);
            //std::cout << "set: " << y << std::endl;
            key_heights[i] = y;
        }
        cr->stroke();

        key_index++;
    }

    cr->move_to(0, 0);
    cr->line_to(0, WHITE_KEYS * white_height);
    cr->stroke();
}

void PianorollWindow::calculate_note_positions()
{
    for (auto it=notes.begin(); it!=notes.end(); it++) {
        auto note = *it;
        //std::cout << key_heights[127-note.note] << std::endl;
        DrawCord cord = DrawCord {note.start / resolution * QUARTER_LENGTH, key_heights[127-note.note], note.length / resolution * QUARTER_LENGTH};
        this->note_drawed.push(cord);
    }
}

void PianorollWindow::draw_timeline(const Cairo::RefPtr<Cairo::Context>& cr)
{
    //Gtk::Allocation allocation = get_allocation();
    //const double height = allocation.get_height();
    const double height = WHITE_KEYS * white_height;
    const double width = max_width;
    const double left_margin = pianoroll_leftmargin + white_width;
    const double interval = measure_width / measure_division;

    cr->set_source_rgba(0.4, 0.4, 0.4, 1);
    cr->set_line_width(1);
    cr->set_font_size(20);
    cr->save();

    int i = 0;
    double start;
    do {
        start = left_margin + i * interval;

        if (i % measure_division == 0) {
            char measures[5];
            sprintf(measures, "%d", i / measure_division + 1);
            cr->move_to(start, pianoroll_upmargin);
            cr->show_text(measures);

            cr->move_to(start, 0);
            cr->set_line_width(2);
            cr->set_source_rgba(0, 0, 1, 1);
        }
        
        cr->move_to(start, 0);
        cr->line_to(start, height);
        cr->stroke();
        cr->restore();
        cr->save();
        i++;
    } while (start < width);
}

void PianorollWindow::reset_with_notes(smf::MidiEventList event_list)
{
    for (int i=0; i<event_list.size(); i++) {
        add_midi_note(event_list[i]);
    }
}

void PianorollWindow::add_midi_note(smf::MidiEvent event)
{
    if (event.isNoteOn()) {
        short key = event[1];
        short velocity = event[2];
        _note_tmp[key].push(event);
    } else if (event.isNoteOff()) {
        if (_note_tmp.empty())
            return;
        short key = event[1];
        smf::MidiEvent on_event = _note_tmp[key].top();
        MidiNote note = {
            key,
            event.tick - on_event.tick,
            on_event.tick,
        };
        notes.push_back(note);
        _note_tmp[key].pop();
        std::cout << "New note! note: " << key << " time(tick): " << on_event.tick << "(length " << event.tick - on_event.tick << ")" << std::endl;
        if (event.tick > max_width)
            max_width = event.tick;
    }
}
