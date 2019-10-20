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

#include "../subprojects/midifile/include/MidiFile.h"
#include "../subprojects/midifile/include/MidiEventList.h"

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
    /*
    MidiNote note = {
        96, 1, 0
    };
    MidiNote note2 = {
        100,2,2
    };
    MidiNote note3 = {
        103,3,5
    };
    MidiNote note4 = {
        104,4,8
    };
    pianoroll->add_note(note);
    pianoroll->add_note(note2);
    pianoroll->add_note(note3);
    pianoroll->add_note(note4);*/

    smf::MidiFile midifile;
    midifile.read("./cola-kisu.mid");
    midifile.doTimeAnalysis();
    std::cout << "Tracks: " << midifile.getTrackCount() << std::endl;
    std::cout << "TPQ: " << midifile.getTicksPerQuarterNote() << std::endl;

    for (int track=0; track<midifile.getTrackCount(); track++) {
      if (midifile.getTrackCount() > 1) std::cout << "\nTrack " << track << std::endl;
      std::cout << "Tick\tSeconds\tDur\tMessage" << std::endl;
      /*
      for (int event=0; event<midifile[track].size(); event++) {
         std::cout << std::dec << midifile[track][event].tick;
         std::cout << '\t' << std::dec << midifile[track][event].seconds;
         std::cout << '\t';
         if (midifile[track][event].isNoteOn())
            std::cout << midifile[track][event].getDurationInSeconds();
         std::cout << '\t' << std::hex;
         for (int i=0; i<midifile[track][event].size(); i++)
            std::cout << (int)midifile[track][event][i] << ' ';
         std::cout << std::endl;
      }*/
  }


    pianoroll->reset_with_notes(midifile[2]);
    pianoroll->show();
}
