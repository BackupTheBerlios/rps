#include <sound_widget.hh>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/table.h>
#include <main_window_RPS.hh>

#include <iostream>

sound_widget::sound_widget(Soundfile &s)
{
   set_label(s.Name());

   Gtk::Table *container = manage(new Gtk::Table());

//   Gtk::Label *sound  = manage(new class Gtk::Label(s.Name()));
//   container->attach(*sound,0,2,0,1,Gtk::SHRINK,Gtk::SHRINK);

   Gtk::Label *time  = manage(new class Gtk::Label(s.Time()));
   container->attach(*time,0,1,1,2,Gtk::SHRINK,Gtk::SHRINK);

   Gtk::Button *stop  = manage(new class Gtk::Button("Stop"));
   stop->signal_clicked().connect(SigC::bind(SigC::slot(
         *this, &sound_widget::button_pressed),s));
   container->attach(*stop,1,2,1,2,Gtk::SHRINK,Gtk::SHRINK);

   int volume = s.get_volume();
   Gtk::Adjustment *vs_ma =
       manage(new class Gtk::Adjustment(100-volume, 0, 100, 1, 1, 1));
   Gtk::HScrollbar *vs_m = manage(new class Gtk::HScrollbar(*vs_ma));
    vs_ma->signal_value_changed().connect(SigC::bind(
      SigC::slot(*this,&sound_widget::value_changed),vs_ma,s));
   container->attach(*vs_m,0,2,2,3,Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL,Gtk::SHRINK);
#if 0
    vs_m->set_usize(-1, 148);
#endif
   add(*container);
   show_all();
}


void sound_widget::button_pressed(Soundfile s)
{
 main_window_RPS::self_main_window_RPS->getRPS().remove_from_playlist(s,true);
}


void sound_widget::value_changed(Gtk::Adjustment *a,Soundfile s)
{
  int wert = 100-(int)(a->get_value());
std::cout << "New Value: "<<wert<<'\n';
  if (wert<0)wert=0; // Nur zur Sicherheit
  s.set_volume(wert);
}
        
