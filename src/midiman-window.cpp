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

#include "midiman-window.h"
#include "pianoroll-window.h"

#include <iostream>

MidimanWindow::MidimanWindow()
	: Glib::ObjectBase("MidimanWindow")
	, Gtk::Window()
	, headerbar(nullptr)
	, startbutton(nullptr)
{
	builder = Gtk::Builder::create_from_resource("/work/tinax/MidiMan/midiman-window.ui");
	builder->get_widget("headerbar", headerbar);
	builder->get_widget("startbutton", startbutton);
	add(*startbutton);
	startbutton->show();
	startbutton->signal_clicked().connect(sigc::ptr_fun(&(this->on_startbutton_clicked)));
	set_titlebar(*headerbar);
	headerbar->show();
}

void MidimanWindow::on_startbutton_clicked()
{
    std::cout << "startbutton clicked" << std::endl;
    PianorollWindow *pianoroll = new PianorollWindow();
    MidiNote note = {
        1, 3, 5
    };
    MidiNote note2 = {
        2,4,6
    };
    MidiNote note3 = {
        20,30,50
    };
    MidiNote note4 = {
        30,40,60
    };
    pianoroll->add_note(note);
    pianoroll->add_note(note2);
    pianoroll->add_note(note3);
    pianoroll->add_note(note4);
    pianoroll->show();
}
