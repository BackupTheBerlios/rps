#include <sound_widget.hh>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/frame.h>

#include <iostream>

sound_widget::sound_widget(Soundfile &s)
{
   Gtk::Label *sound  = manage(new class Gtk::Label(s.Name()));
   Gtk::Label *time  = manage(new class Gtk::Label(s.Time()));
   Gtk::Button *stop  = manage(new class Gtk::Button("Stop"));

   int volume = s.get_volume();
   Gtk::Adjustment *vs_ma =
       manage(new class Gtk::Adjustment(100-volume, 0, 100, 1, 1, 1));
   Gtk::VScrollbar *vs_m = manage(new class Gtk::VScrollbar(*vs_ma));


   Gtk::Frame *f_m = manage(new class Gtk::Frame(s.Name()));
#if 0
    f_m->set_label_align(0, 0);
    f_m->set_shadow_type(GTK_SHADOW_ETCHED_IN);
    f_m->add(*vs_m);
    f_m->set_label_align(0, 0);
    f_m->set_shadow_type(GTK_SHADOW_ETCHED_IN);
#endif
#if 0
    vs_m->set_usize(-1, 148);
#endif

#if 0 
    vs_m->show();
    f_m->show();
#endif

   stop->signal_clicked().connect(SigC::slot(*this, &sound_widget::button_pressed_));
#if 0
   stop->signal_clicked().connect(SigC::bind(SigC::slot(
         this, &sound_widget::button_pressed),s));
#endif
#if 0
   stop->signal_clicked().connect(SigC::bind(SigC::slot(
     *static_cast<class sound_widget*>(this), 
     &sound_widget::button_pressed),s));
#endif

#if 0
    vs_ma->value_changed.connect(SigC::bind(
      SigC::slot(this,&sound_widget::value_changed),vs_ma,s));
#endif
    
   pack_start(*sound);
   pack_start(*time);
   pack_start(*vs_m);
   pack_start(*stop);
   show_all();
//    table->attach(*f_m,*count,*count+1,1,2);
}

void sound_widget::button_pressed_()
{
std::cout << "BP\n";
}


void sound_widget::button_pressed(Soundfile &s)
{
 std::cout << "BP22222222\n";
}


void sound_widget::value_changed(Gtk::Adjustment *a,Soundfile &s)
{
  int wert = 100-(int)(a->get_value());
std::cout << "New Value: "<<wert<<'\n';
  if (wert<0)wert=0; // Nur zur Sicherheit
//  s.set_volume(wert);
}
        
