#include <sound_widget.hh>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/frame.h>


sound_widget::sound_widget(const Soundfile &s)
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
//    vs_ma->value_changed.connect(SigC::bind(
//      SigC::slot(this,&sound_widget::value_changed),vs_ma,s));
//    table->attach(*f_m,*count,*count+1,1,2);
}

void sound_widget::value_changed(Gtk::Adjustment *a,const Soundfile &s)
{
  int wert = 100-(int)(a->get_value());
std::cout << "New Value: "<<wert<<'\n';
  if (wert<0)wert=0; // Nur zur Sicherheit
//  const_cast<Soundfile&>(s).set_volume(wert);
}
        
