#include <sound_widget.hh>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/table.h>
#include <main_window_RPS.hh>

#include <iostream>


sound_widget::sound_widget(Soundfile &s)
: sound (s)
{
   set_label(s.Name());

   Gtk::Table *container = manage(new Gtk::Table());

   Gtk::Label *lrepeat  = manage(new class Gtk::Label(s.RepeatStr()));
   container->attach(*lrepeat,0,1,0,1,Gtk::SHRINK,Gtk::SHRINK);

   Gtk::Label *ltime  = manage(new class Gtk::Label(s.Time()));
   container->attach(*ltime,1,2,0,1,Gtk::SHRINK,Gtk::SHRINK);

   Gtk::Button *stop  = manage(new class Gtk::Button("Stop"));
   stop->signal_clicked().connect(SigC::bind(SigC::slot(
         *this, &sound_widget::button_pressed),s));
   container->attach(*stop,2,3,0,1,Gtk::SHRINK,Gtk::SHRINK);

   int volume = s.get_volume();
   Gtk::Adjustment *vs_ma=manage(new class Gtk::Adjustment(volume, 0, 100, 1, 1, 1));
   Gtk::HScale *vs_m = manage(new class Gtk::HScale(*vs_ma));
   vs_ma->signal_value_changed().connect(SigC::bind(
         SigC::slot(*this,&sound_widget::value_changed),vs_ma));
   vs_m->set_draw_value(true);
   vs_m->set_digits(0);
   container->attach(*vs_m,0,3,2,3,Gtk::EXPAND|Gtk::SHRINK|Gtk::FILL,Gtk::SHRINK);
#if 0
    vs_m->set_usize(-1, 148);
#endif
   add(*container);
   show_all();

  start_time =  time(NULL);
  Glib::signal_timeout().connect(SigC::bind(SigC::slot(
      *this,&sound_widget::timeout_handler), ltime),1000);
   
}


#include <itos.hh>
bool sound_widget::timeout_handler(Gtk::Label *l)
{
   time_t tnow = time(NULL);
   time_t ire = sound.Seconds() - (tnow-start_time);

   if(ire<0) { start_time = tnow-1; ire=sound.Seconds()-1; }


   int min = int(ire/60+0.5);
   int sec = ire%60;
   std::string sre = itos(min)+":"+itos(sec,true);
//   std::cout << ire<<'\t'<<sre<<'\n';
   l->set_text(sre);
   return true;
}


void sound_widget::button_pressed(Soundfile s)
{
 main_window_RPS::self_main_window_RPS->getRPS().remove_from_playlist(s,true);
}


void sound_widget::value_changed(Gtk::Adjustment *a)
{
   int wert = (int)(a->get_value());
//std::cout << "New Value: "<<wert<<'\n';
  if (wert<0) wert=0; // Nur zur Sicherheit
  else if (wert>100) wert=100; // Nur zur Sicherheit
  sound.set_volume(wert);
  main_window_RPS::self_main_window_RPS->
            getRPS().getFileList().set_default_volume(sound,wert);
}
        
